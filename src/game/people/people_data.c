/**
 * @file people_data.c
 * @brief People/NPC field-level data management -- allocation, lookup,
 *        model loading, accessor functions.
 *
 * This file implements the lower-level People/NPC data layer that manages
 * NPC struct allocation, slot lookup, field getters/setters, and model
 * loading. It sits below the high-level people.c (0x80180C78+) which
 * handles floor-level init/update/spawn, and is called extensively by
 * the script system and battle system.
 *
 * Address range: 0x80140588 - 0x80144574 (approximately 0x3FEC bytes)
 * Function count: ~100 functions (83 of which are tiny getters/setters)
 *
 * Key functions:
 *
 *   fn_80140588 (peopleFieldOpen)      -- 0x514 bytes
 *     Open/spawn an NPC from field data. Takes a PeopleEntry pointer,
 *     group/index pair, spawn data, and a "force" flag. Calls into
 *     itemGetStatus (peopleFieldAlloc) to find or create a slot, loads
 *     the model, and configures the NPC's initial state.
 *     References: lbl_80478BD8 (gPeopleFieldCount),
 *                 lbl_803681E8 (gPeopleFieldLookup),
 *                 lbl_80363CE8 (gPeopleFieldArray)
 *
 *   fn_80140A9C (peopleFieldGetSlot)   -- 0x30 bytes
 *     Simple slot lookup by index. Returns pointer into gPeopleFieldArray.
 *     Called from 3 external sites (effect/shadow modules).
 *
 *   fn_80140ACC (peopleFieldLoadModel) -- 0x83C bytes
 *     Load NPC model and configure animation data. This is a large
 *     function that processes the model resource, sets up joint matrices,
 *     configures walk/run motion data, and initializes the animation
 *     state machine. References gPeopleFieldArray globals.
 *
 *   fn_80141308 (peopleFieldUpdate)    -- 0x1060 bytes (largest in range!)
 *     Per-frame update for a single NPC in the field. Handles state
 *     transitions, animation blending, position interpolation, collision
 *     checks, and rendering submission. This is the core NPC tick function.
 *     References gPeopleFieldArray and lbl_80434E64 (gPeopleFieldWork).
 *
 *   fn_80142368 (peopleFieldCleanup)   -- 0x280 bytes
 *     Cleanup/release an NPC slot. Frees model resources and resets state.
 *
 *   fn_801425E8 (peopleFieldSetup)     -- 0x39C bytes
 *     Configure an NPC's properties after model loading. Sets position,
 *     rotation, scale, motion parameters from spawn data.
 *
 *   fn_80142984 (peopleFieldGetByID)   -- 0x64 bytes
 *     Look up an NPC by its group+index ID pair. Called by 18 external
 *     functions (heavily used by the script system).
 *
 *   fn_801429E8 (peopleFieldGetEntry)  -- 0xA0 bytes
 *     Extended NPC lookup that validates the entry and returns a
 *     PeopleFieldEntry pointer. Called by 28 external functions.
 *
 *   fn_80142A88 (peopleFieldSetState)  -- 0x9C bytes
 *     Set the state of an NPC. Used by battle and cutscene systems.
 *
 *   fn_80142B24 (peopleFieldApplyMotion) -- 0x1D0 bytes
 *     Apply a motion/animation to an NPC. Blends between current and
 *     target animation states.
 *
 *   itemGetStatus (peopleFieldAlloc)     -- 0x204 bytes
 *     Allocate or find an NPC slot. If a slot with the same group+index
 *     already exists, returns it. Otherwise allocates a new slot from
 *     the free pool. Called by 38 external functions.
 *
 *   fn_80142EF8 (peopleFieldRelease)   -- 0x2B4 bytes
 *     Release an NPC slot back to the free pool.
 *
 *   fn_801431AC (peopleFieldInit)      -- 0x4F0 bytes
 *     Initialize the field people system. Allocates the NPC array,
 *     lookup table, and work buffer. Sets up global state.
 *
 * Getter/setter cluster (fn_8014369C - fn_80144064):
 *   83 tiny functions (most 0x10-0x28 bytes each) that provide safe
 *   access to PeopleFieldEntry struct fields. Each follows the pattern:
 *     if (ptr == NULL) return 0;
 *     return ptr->fieldAtOffset;
 *
 *   The struct accessed is PeopleFieldEntry (0x28 bytes per slot),
 *   stored in gPeopleFieldArray (lbl_80363CE8), indexed via
 *   gPeopleFieldLookup (lbl_803681E8).
 *
 *   PeopleFieldEntry layout (from getter offsets):
 *     0x00: f32   field_00       (fn_80143C00 get, fn_80143850 set)
 *     0x04: u8    flags_04       (fn_801436F0 bit test)
 *     0x05: s8    field_05       (fn_801436D4 get)
 *     0x06: s8    field_06       (fn_801436B8 get)
 *     0x07: s8    field_07       (fn_8014369C get)
 *     0x08: (unknown)
 *     0x0C: u8    field_0C       (fn_80143760 get)
 *     0x0D: u8    field_0D       (fn_80143748 get)
 *     0x0E: u8    field_0E       (fn_80143730 get)
 *     0x0F: u8    field_0F       (fn_80143718 get)
 *     0x10: f32   posX           (fn_80143BD0 get, fn_80143C50 set)
 *     0x14: f32   posY           (fn_80143BE0 get, fn_80143C68 set)
 *     0x18: f32   posZ           (fn_80143BF0 get, fn_80143C80 set)
 *     0x1C: f32   rotAngle       (fn_80143C10 get, fn_80143C98 set)
 *     0x20: f32   scale          (fn_80143C20 get, fn_80143CB0 set)
 *     0x24: u32   modelRef       (fn_80143C30 get, fn_80143CE0 set)
 *
 *   fn_801440A0 (peopleFieldGetByIndex) -- 0x50 bytes
 *     The single most-called function in the entire range (48 external
 *     callers). Takes a u16 index, validates against gPeopleFieldCount,
 *     looks up the slot via gPeopleFieldLookup, multiplies by 0x28 to
 *     get the offset into gPeopleFieldArray, and returns the pointer.
 *     Pattern:
 *       if (index >= gPeopleFieldCount) return NULL;
 *       slot = gPeopleFieldLookup[index];
 *       if (slot >= gMaxSlotCount) return NULL;
 *       return &gPeopleFieldArray[slot * 0x28];
 *
 * Global data:
 *   lbl_80478BD8 (sbss) -- u32 gPeopleFieldCount (max index count)
 *   lbl_80478BB0 (sbss) -- u32 gPeopleFieldMaxSlots
 *   lbl_803681E8 (bss)  -- u16[] gPeopleFieldLookup (index -> slot mapping)
 *   lbl_80363CE8 (bss)  -- PeopleFieldEntry[] gPeopleFieldArray (0x28 bytes each)
 *   lbl_80434E64 (bss)  -- PeopleFieldWork (extended work buffer)
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);     /* OSReport */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* GSmem allocator */
extern u16   fn_800E3534(u32 size);
extern void* fn_800E27B0(u16 handle);

/* Model system */
extern void  fn_800EE150(void* model, u32 param);
extern void  fn_800EE828(void* model, u32 param);
extern void  fn_800E24B0(void* model, u32 param);
extern void  fn_800E209C(void* model, u32 param);
extern void  fn_800E01F4(void* dst, void* src);
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800E019C(void* model, void* param);
extern void  fn_800C46B0(void* param1, void* param2);

/* Floor resource system */
extern void* fn_800F9318(u16 group, u16 model, u16 param);

/* Thread/task system */
extern void* fn_800FE834(u32 pri, u32 type, void* buf, void* callback);

/* Interrupt control */
extern u32  OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 level);

/* ===================================================================
 * PeopleFieldEntry -- compact NPC data for field rendering.
 *
 * 0x28 bytes per entry. Stored in a flat array at lbl_80363CE8.
 * Accessed via the 83 getter/setter functions at 0x8014369C-0x80144064.
 * =================================================================== */

typedef struct PeopleFieldEntry {
    /* 0x00 */ f32    field_00;
    /* 0x04 */ u8     flags_04;
    /* 0x05 */ s8     field_05;
    /* 0x06 */ s8     field_06;
    /* 0x07 */ s8     field_07;
    /* 0x08 */ u32    field_08;
    /* 0x0C */ u8     field_0C;
    /* 0x0D */ u8     field_0D;
    /* 0x0E */ u8     field_0E;
    /* 0x0F */ u8     field_0F;
    /* 0x10 */ f32    posX;
    /* 0x14 */ f32    posY;
    /* 0x18 */ f32    posZ;
    /* 0x1C */ f32    rotAngle;
    /* 0x20 */ f32    scale;
    /* 0x24 */ u32    modelRef;
} PeopleFieldEntry;

/* ===================================================================
 * DECOMPILED: fn_801440A0 -- peopleFieldGetByIndex
 *
 * The most-called function in the entire range (48 external callers).
 * Looks up a PeopleFieldEntry by its u16 index.
 * =================================================================== */

/* Global state (sdata/sbss) */
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
extern u32 lbl_80478BB0;   /* gPeopleFieldMaxSlots */
extern u16 lbl_803681E8[]; /* gPeopleFieldLookup */
extern u8  lbl_80363CE8[]; /* gPeopleFieldArray base */

PeopleFieldEntry* peopleFieldGetByIndex(u16 index) {
    u16 slot;

    if (index >= lbl_80478BD8) {
        return NULL;
    }

    slot = lbl_803681E8[index];
    if (slot >= lbl_80478BB0) {
        return NULL;
    }

    return (PeopleFieldEntry*)(&lbl_80363CE8[slot * 0x28]);
}

/* ===================================================================
 * STUB DECLARATIONS -- remaining functions
 *
 * Full decompilation deferred; the asm files remain authoritative.
 * Function addresses and proposed names documented for cross-reference.
 * =================================================================== */

/* fn_80140588: peopleFieldOpen (0x514 bytes) */
/* fn_80140A9C: peopleFieldGetSlot (0x30 bytes) */
/* fn_80140ACC: peopleFieldLoadModel (0x83C bytes) */
/* fn_80141308: peopleFieldUpdate (0x1060 bytes -- largest single function) */
/* fn_80142368: peopleFieldCleanup (0x280 bytes) */
/* fn_801425E8: peopleFieldSetup (0x39C bytes) */
/* fn_80142984: peopleFieldGetByID (0x64 bytes) */
/* fn_801429E8: peopleFieldGetEntry (0xA0 bytes) */
/* fn_80142A88: peopleFieldSetState (0x9C bytes) */
/* fn_80142B24: peopleFieldApplyMotion (0x1D0 bytes) */
/* itemGetStatus: peopleFieldAlloc (0x204 bytes) */
/* fn_80142EF8: peopleFieldRelease (0x2B4 bytes) */
/* fn_801431AC: peopleFieldInit (0x4F0 bytes) */

/* --- Getter/Setter cluster (83 functions) --- */
/* fn_8014369C: getField07 */
/* fn_801436B8: getField06 */
/* fn_801436D4: getField05 */
/* fn_801436F0: testFlags04 */
/* fn_80143718: getField0F */
/* fn_80143730: getField0E */
/* fn_80143748: getField0D */
/* fn_80143760: getField0C */
/* fn_80143778: getField08_lo (bit extract) */
/* fn_801437A0: getField08_hi */
/* fn_801437B8: setField08_lo */
/* fn_801437E0: getField09 */
/* fn_801437F8: setField09 */
/* fn_80143820: getField0A */
/* fn_80143838: getField0B */
/* fn_80143850: setField00 (f32) */
/* fn_80143878: setField04_bit0 */
/* fn_801438A0: setField04_bit1 */
/* fn_801438C8: setField04_bit2 */
/* fn_801438F0: setField04_bit3 */
/* fn_80143918: setField05 (s8) */
/* fn_80143940: setField06 (s8) */
/* fn_80143968: setField07 (s8) */
/* fn_80143990: setField08 (u32) */
/* fn_801439B8: setField0C (u8) */
/* fn_801439D4: setField0D (u8) */
/* fn_801439F0: setField0E (u8) */
/* fn_80143A0C: setField0F (u8) */
/* fn_80143A28: clearFlags04 */
/* fn_80143A44: setFlags04 */
/* fn_80143A6C: setField_special1 */
/* fn_80143A94: setField_special2 */
/* fn_80143ABC: setField_special3 */
/* fn_80143AF0: getField_special1 */
/* fn_80143B08: getField_special2 */
/* fn_80143B30: getField_special3 */
/* fn_80143B48: setField_special4 */
/* fn_80143B70: getField10_int (posX as int) */
/* fn_80143B80: getField14_int (posY as int) */
/* fn_80143B90: getField18_int (posZ as int) */
/* fn_80143BA0: getField1C_int (rotAngle as int) */
/* fn_80143BB0: setField10_int */
/* fn_80143BD0: getPosX (f32) */
/* fn_80143BE0: getPosY (f32) */
/* fn_80143BF0: getPosZ (f32) */
/* fn_80143C00: getField00 (f32) */
/* fn_80143C10: getRotAngle (f32) */
/* fn_80143C20: getScale (f32) */
/* fn_80143C30: getModelRef (u32) */
/* fn_80143C40: getField24_byte */
/* fn_80143C50: setPosX (f32) */
/* fn_80143C68: setPosY (f32) */
/* fn_80143C80: setPosZ (f32) */
/* fn_80143C98: setRotAngle (f32) */
/* fn_80143CB0: setScale (f32) */
/* fn_80143CC8: setModelRef */
/* fn_80143CE0: setField24 */
/* fn_80143CF8: setField24_byte */
/* fn_80143D10: getField_ext1 */
/* fn_80143D28: getField_ext2 */
/* fn_80143D40: getField_ext3 */
/* fn_80143D58: getField_ext4 */
/* fn_80143D70: getField_ext5 */
/* fn_80143D88: getField_ext6 */
/* fn_80143DA0: getField_ext7 (with extra logic) */
/* fn_80143DCC: getField_ext8 */
/* fn_80143DE4: getField_ext9 */
/* fn_80143DFC: getField_ext10 */
/* fn_80143E14: getField_ext11 */
/* fn_80143E2C: setField_ext1 */
/* fn_80143E60: setField_ext2 */
/* fn_80143E88: setField_ext3 (0x68 bytes) */
/* fn_80143EF0: setField_ext4 */
/* fn_80143F24: setField_ext5 */
/* fn_80143F54: getField_ext12 */
/* fn_80143F6C: getField_ext13 */
/* fn_80143F84: getField_ext14 */
/* fn_80143F9C: getField_ext15 */
/* fn_80143FB4: getField_ext16 */
/* fn_80143FCC: getField_ext17 */
/* fn_80143FE4: getField_ext18 */
/* fn_80143FFC: getField_ext19 */
/* fn_80144014: getField_ext20 */
/* fn_8014402C: setField_ext6 */
/* fn_80144064: setField_ext7 */
/* fn_80144088: peopleFieldQuery (0x18 bytes) -- quick status check */

/* fn_801440F0: peopleFieldOpenModel (0xB8 bytes) */
/* fn_801441A8: peopleFieldConfigModel (0x224 bytes) */
/* fn_801443CC: peopleFieldFinalizeModel (0x1A8 bytes) */
/* renamed symbols referenced by asm incs (symbolmap port) */
/* Forward declarations for self-referencing asm blocks */
extern void fn_801425E8(u32* base, u16 count, u8 mode);
extern void fn_80142B24(void*, u32, u16, u32, u32);
extern s32 itemGetStatus(u32, u16, u16, u32);
extern u16  fn_80143B30(u8* p);
extern u8*  fn_80143B48(u16 idx);
extern void fn_80143B70(u8* p, u16 val);
extern void fn_80143B80(u8* p, u16 val);
extern void fn_80143B90(u8* p, u16 val);
extern void fn_80143BA0(u8* p, u32 val);
extern void fn_80143BB0(u8* p, u16 idx, u8 val);
extern void fn_80143BD0(u8* p, u16 val);
extern void fn_80143BE0(u8* p, u16 val);
extern void fn_80143BF0(u8* p, u32 val);
extern void fn_80143C00(u8* p, u8 val);
extern void fn_80143C10(u8* p, u8 val);
extern void fn_80143C20(u8* p, u16 val);
extern void fn_80143C30(u8* p, u8 val);
extern void fn_80143C40(u8* p, u32 val);

#if 0
asm void fn_80140A9C(void) {
#include "src/game/people/people_data_fn_80140A9C.inc"
}
#else
#pragma optimization_level 4
void fn_80140A9C(u32* a, u32* b) {
    volatile u32 new_var;
    u32 tmp;
    if (a == NULL) return;
    if (b == NULL) return;
    tmp = (new_var = *a);
    *a = *b;
    *b = tmp;
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */

#define PEOPLE_ENTRY_ID(entry) ((u16)itemGetStatus((u32)(entry), 0, 0x1b, 0))
#define PEOPLE_ENTRY_COUNT(entry) ((u16)itemGetStatus((u32)(entry), 0, 0x1c, 0))
#define PEOPLE_ITEM_ID_VALID(id) \
    (itemGetStatus(0, (u16)(id), 1, 0) != 0 && (u16)(id) < lbl_80478BD8)
#define PEOPLE_ENTRY_VALID(entry) \
    ((entry) != NULL && PEOPLE_ENTRY_ID(entry) != 0 && PEOPLE_ITEM_ID_VALID(PEOPLE_ENTRY_ID(entry)))

#if 0
asm void fn_80140ACC(void) {
#include "src/game/people/people_data_fn_80140ACC.inc"
}
#else
#pragma optimization_level 4
s32 fn_80140ACC(u32* base, u16 count, u16 id, u16 amount, s16 index, u16 unusedLimit, u8 sortMode) {
    u32* entry;
    u16 remaining;
    u16 current;
    u16 next;
    u16 i;
    u8 needsSort;

    if (base == NULL) {
        return -1;
    }
    if (!PEOPLE_ITEM_ID_VALID(id)) {
        return -1;
    }

    remaining = amount;
    needsSort = 0;
    unusedLimit = unusedLimit;

    if (index < 0) {
        while (remaining != 0) {
            entry = NULL;
            for (i = 0; i < count; i++) {
                if (PEOPLE_ENTRY_VALID(base + i) && PEOPLE_ENTRY_ID(base + i) == id) {
                    entry = base + i;
                    break;
                }
            }

            if (entry == NULL) {
                break;
            }

            current = PEOPLE_ENTRY_COUNT(entry);
            if (current > remaining) {
                next = (u16)(current - remaining);
                remaining = 0;
            } else {
                next = 0;
                remaining = (u16)(remaining - current);
            }

            fn_80142B24(entry, 0, 0x1c, 0, next);
            if (next == 0) {
                fn_80142B24(entry, 0, 0x1b, 0, 0);
                fn_80142B24(entry, 0, 0x1c, 0, 0);
                needsSort = 1;
            }
        }
    } else {
        if ((u16)index >= count) {
            return -1;
        }

        entry = base + (u16)index;
        if (!PEOPLE_ENTRY_VALID(entry) || PEOPLE_ENTRY_ID(entry) != id) {
            return -1;
        }

        current = PEOPLE_ENTRY_COUNT(entry);
        if (current > remaining) {
            next = (u16)(current - remaining);
            remaining = 0;
        } else {
            next = 0;
            remaining = (u16)(remaining - current);
        }

        fn_80142B24(entry, 0, 0x1c, 0, next);
        if (next == 0) {
            fn_80142B24(entry, 0, 0x1b, 0, 0);
            fn_80142B24(entry, 0, 0x1c, 0, 0);
            needsSort = 1;
        }
    }

    if (needsSort) {
        fn_801425E8(base, count, sortMode);
    }
    return remaining;
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
#if 0
asm void fn_80141308(void) {
#include "src/game/people/people_data_fn_80141308.inc"
}
#else
#pragma optimization_level 4
s32 fn_80141308(u32* base, u16 count, u16 id, u16 amount, s16 index, u16 maxCount, u8 sortMode, u8 forceIndex) {
    u32* entry;
    u16 current;
    u16 next;
    u16 overflow;
    u16 i;
    u8 needsSort;

    if (base == NULL) {
        return -1;
    }
    if (!PEOPLE_ITEM_ID_VALID(id)) {
        return -1;
    }

    entry = NULL;
    overflow = amount;
    needsSort = 0;

    if (forceIndex == 0) {
        for (i = 0; i < count; i++) {
            if (PEOPLE_ENTRY_VALID(base + i) && PEOPLE_ENTRY_ID(base + i) == id) {
                entry = base + i;
                break;
            }
        }

        if (entry == NULL) {
            for (i = 0; i < count; i++) {
                if (!PEOPLE_ENTRY_VALID(base + i)) {
                    entry = base + i;
                    needsSort = 1;
                    break;
                }
            }
        }
    } else {
        if (index < 0 || (u16)index >= count) {
            return -1;
        }
        entry = base + (u16)index;
        needsSort = !PEOPLE_ENTRY_VALID(entry) || PEOPLE_ENTRY_ID(entry) != id;
    }

    if (entry == NULL) {
        return overflow;
    }

    if (!PEOPLE_ENTRY_VALID(entry) || PEOPLE_ENTRY_ID(entry) != id) {
        fn_80142B24(entry, 0, 0x1b, 0, 0);
        fn_80142B24(entry, 0, 0x1c, 0, 0);
        fn_80142B24(entry, 0, 0x1b, 0, id);
        current = 0;
        needsSort = 1;
    } else {
        current = PEOPLE_ENTRY_COUNT(entry);
    }

    if (current >= maxCount) {
        overflow = amount;
    } else {
        next = (u16)(current + amount);
        if (next > maxCount || next < current) {
            overflow = (u16)(next - maxCount);
            next = maxCount;
        } else {
            overflow = 0;
        }
        fn_80142B24(entry, 0, 0x1c, 0, next);
    }

    if (needsSort) {
        fn_801425E8(base, count, sortMode);
    }
    return overflow;
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
#if 0
asm void fn_80142368(void) {
#include "src/game/people/people_data_fn_80142368.inc"
}
#else
#pragma optimization_level 4
u32* fn_80142368(u32* base, u16 count, u16 id, u8 mode, u16 limit) {
    u32* entry;
    u16 i;

    if (base == NULL) {
        return NULL;
    }
    if (!PEOPLE_ITEM_ID_VALID(id)) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        entry = base + i;
        if (mode == 2) {
            if (!PEOPLE_ENTRY_VALID(entry)) {
                return entry;
            }
        } else if (PEOPLE_ENTRY_VALID(entry) && PEOPLE_ENTRY_ID(entry) == id) {
            if (mode == 1) {
                if (PEOPLE_ENTRY_COUNT(entry) < limit) {
                    return entry;
                }
            } else {
                return entry;
            }
        }
    }

    return NULL;
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
#if 0
asm void fn_801425E8(void) {
#include "src/game/people/people_data_fn_801425E8.inc"
}
#else
#pragma optimization_level 4
void fn_801425E8(u32* base, u16 count, u8 mode) {
    u32* entry;
    u32* other;
    u32 temp;
    u16 i;
    u16 j;

    if (base == NULL || count == 0) {
        return;
    }

    if (mode == 0 || mode == 1) {
        for (i = 0; i + 1 < count; i++) {
            entry = base + i;
            if (!PEOPLE_ENTRY_VALID(entry)) {
                for (j = (u16)(i + 1); j < count; j++) {
                    other = base + j;
                    if (PEOPLE_ENTRY_VALID(other)) {
                        temp = *entry;
                        *entry = *other;
                        *other = temp;
                        break;
                    }
                }
            }
        }
    }

    if (mode == 1) {
        for (i = 0; i + 1 < count; i++) {
            entry = base + i;
            if (PEOPLE_ENTRY_VALID(entry)) {
                for (j = (u16)(i + 1); j < count; j++) {
                    other = base + j;
                    if (PEOPLE_ENTRY_VALID(other) &&
                        PEOPLE_ENTRY_ID(entry) > PEOPLE_ENTRY_ID(other)) {
                        temp = *entry;
                        *entry = *other;
                        *other = temp;
                    }
                }
            }
        }
    }
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
#if 0
asm void fn_80142984(void) {
#include "src/game/people/people_data_fn_80142984.inc"
}
#else
#pragma optimization_level 4
s32 fn_80142984(u16 id) {
    s32 r;

    r = itemGetStatus(0, id, 1, 0);
    if (r == 0) return 0;
    return lbl_80478BD8 > (u16)id;
}
#endif
extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
#if 0
asm void fn_801429E8(void) {
#include "src/game/people/people_data_fn_801429E8.inc"
}
#else
#pragma optimization_level 4
s32 fn_801429E8(u32 arg) {
    u16 r31;
    u8 valid;

    if (arg == 0) return 0;

    r31 = (u16)itemGetStatus(arg, 0, 0x1b, 0);
    if (r31 == 0) return 0;

    if (itemGetStatus(0, r31, 1, 0) == 0) {
        valid = 0;
    } else if (r31 >= lbl_80478BD8) {
        valid = 0;
    } else {
        valid = 1;
    }

    return valid != 0;
}
#endif
#if 0
asm void fn_80142A88(void) {
#include "src/game/people/people_data_fn_80142A88.inc"
}
#else
#pragma optimization_level 4
void fn_80142A88(u32* base, u16 count) {
    u32* ptr;
    u16 i;

    if (base == NULL) return;

    for (i = 0; (u16)i < (u16)count; i++) {
        if ((ptr = base + i) != NULL) {
            fn_80142B24((void*)ptr, 0, 0x1b, 0, 0);
            fn_80142B24((void*)ptr, 0, 0x1c, 0, 0);
        }
    }
}
#endif
extern u8* fn_801440A0(u16 idx);
extern u8* fn_80143DA0(u16 idx);
extern void fn_8020A328(u8* p, u16 val);
extern void fn_8020A318(u8* p, u16 val);
extern void fn_8020A308(u8* p, u32 val);
extern void fn_8020A2F8(u8* p, u32 val);
extern void jumptable_80367D60();
#if 0
asm void fn_80142B24(void* p, u32 a, u16 b, u32 c, u32 d) {
#include "src/game/people/people_data_fn_80142B24.inc"
}
#else
#pragma optimization_level 4
void fn_80142B24(void* p, u32 a, u16 b, u32 c, u32 d) {
    u8* target;

    if (b == 0 || b >= 0x23) {
        return;
    }

    if (b < 0xb) {
        target = fn_801440A0((u16)a);
        if (target == NULL) {
            return;
        }
    } else if (b < 0x18) {
        target = fn_80143DA0((u16)a);
        if (target == NULL) {
            return;
        }
    } else {
        target = p;
        if (target == NULL) {
            return;
        }
    }

    switch (b) {
    case 1:
        fn_80143C40(target, d);
        break;
    case 2:
        fn_80143C30(target, (u8)d);
        break;
    case 3:
        fn_80143C20(target, (u16)d);
        break;
    case 4:
        fn_80143C10(target, (u8)d);
        break;
    case 5:
        fn_80143C00(target, (u8)d);
        break;
    case 6:
        fn_80143BF0(target, d);
        break;
    case 7:
        fn_80143BE0(target, (u16)d);
        break;
    case 8:
        fn_80143BD0(target, (u16)d);
        break;
    case 9:
        fn_80143BB0(target, (u16)c, (u8)d);
        break;
    case 10:
        fn_80143BA0(target, d);
        fn_80143B90(target, (u16)d);
        break;
    case 12:
        fn_80143B90(target, (u16)d);
        break;
    case 27:
        fn_80143B80(target, (u16)d);
        break;
    case 28:
        fn_80143B70(target, (u16)d);
        break;
    case 30:
        fn_8020A328(target, (u16)d);
        break;
    case 31:
        fn_8020A318(target, (u16)d);
        break;
    case 32:
        fn_8020A308(target, d);
        break;
    case 33:
        fn_8020A2F8(target, d);
        break;
    default:
        break;
    }
}
#endif
extern u32 fn_80144088(u8* p);
extern u8 fn_80144014(u8* p);
extern u32 fn_80143FFC(u8* p);
extern u8 fn_80143FCC(u8* p);
extern u8 fn_80143FB4(u8* p);
extern u32 fn_80143F84(u8* p);
extern u32 fn_80143F6C(u8* p);
extern u32 fn_80143F54(u8* p);
extern s32 fn_80143E2C(u8* p, u16 idx);
extern u32 fn_80143E14(u8* p);
extern u32 fn_80143D88(u8* p);
extern u32 fn_80143D70(u8* p);
extern u32 fn_80143D58(u8* p);
extern u32 fn_80143D40(u8* p);
extern u32 fn_80143D28(u8* p);
extern u32 fn_80143D10(u8* p);
extern u32 fn_80143CF8(u8* p);
extern u32 fn_80143CE0(u8* p);
extern u32 fn_80143CC8(u8* p);
extern u32 fn_80143CB0(u8* p);
extern u32 fn_80143C98(u8* p);
extern u32 fn_80143C80(u8* p);
extern u32 fn_80143C68(u8* p);
extern u32 fn_80143C50(u8* p);
extern u32 fn_8020A380(u8* p);
extern u32 fn_8020A368(u8* p);
extern u32 fn_8020A350(u8* p);
extern u32 fn_8020A338(u8* p);
extern void jumptable_80367DE8();
#if 0
asm s32 itemGetStatus(u32 a, u16 b, u16 c, u32 d) {
#include "src/game/people/people_data_fn_80142CF4.inc"
}
#else
#pragma optimization_level 4
s32 itemGetStatus(u32 a, u16 b, u16 c, u32 d) {
    u8* target;

    if (c == 0 || c >= 0x23) {
        return 0;
    }

    if (c < 0xb) {
        target = fn_801440A0(b);
        if (target == NULL) {
            return 0;
        }
    } else if (c < 0x18) {
        target = fn_80143DA0(b);
        if (target == NULL) {
            return 0;
        }
    } else if (c < 0x1a) {
        target = fn_80143B48(b);
        if (target == NULL) {
            return 0;
        }
    } else {
        if (a == 0) {
            return 0;
        }
        target = (u8*)a;
    }

    switch (c) {
    case 1:
        return fn_80144088(target);
    case 2:
        return fn_80144014(target);
    case 3:
        return (u16)fn_80143FFC(target);
    case 4:
        return fn_80143FCC(target);
    case 5:
        return fn_80143FB4(target);
    case 6:
        return fn_80143F84(target);
    case 7:
        return (u16)fn_80143F6C(target);
    case 8:
        return (u16)fn_80143F54(target);
    case 9:
        return (s8)fn_80143E2C(target, (u16)d);
    case 10:
        return fn_80143E14(target);
    case 12:
        return (u16)fn_80143D88(target);
    case 13:
        return fn_80143D70(target);
    case 14:
        return fn_80143D58(target);
    case 15:
        return fn_80143D40(target);
    case 16:
        return fn_80143D28(target);
    case 17:
        return fn_80143D10(target);
    case 18:
        return fn_80143CF8(target);
    case 19:
        return fn_80143CE0(target);
    case 20:
        return fn_80143CC8(target);
    case 21:
        return fn_80143CB0(target);
    case 22:
        return fn_80143C98(target);
    case 23:
        return fn_80143C80(target);
    case 25:
        return (u16)fn_80143B30(target);
    case 27:
        return (u16)fn_80143C68(target);
    case 28:
        return (u16)fn_80143C50(target);
    case 30:
        return (u16)fn_8020A380(target);
    case 31:
        return (u16)fn_8020A368(target);
    case 32:
        return fn_8020A350(target);
    case 33:
        return fn_8020A338(target);
    default:
        return 0;
    }
}
#endif
extern void jumptable_80367E70();
extern u8 lbl_802730E0[];

typedef struct ItemParamConvertEntry {
    u32 field;
    u32 srcOffset;
    u32 mask;
    u32 type;
} ItemParamConvertEntry;

#define PEOPLE_SET_FLAG_BYTE(byte, mask, flag) \
    ((byte) = (u8)(((byte) & (u8)~(mask)) | ((flag) ? (mask) : 0)))

#if 0
asm void fn_80142EF8(void) {
#include "src/game/people/people_data_fn_80142EF8.inc"
}
#else
#pragma optimization_level 4
void fn_80142EF8(u8* dst, u8* src) {
    ItemParamConvertEntry* table;
    ItemParamConvertEntry* row;
    u8* stream;
    u32 i;
    u32 bit;
    u32 decoded;
    u32 flag;
    u32 type;
    u8 srcByte;
    u8 maskByte;
    s8 signedValue;

    table = (ItemParamConvertEntry*)lbl_802730E0;
    stream = src + 6;

    for (i = 0; i < 0x20; i++) {
        row = table + i;
        type = row->type;
        srcByte = src[row->srcOffset];
        maskByte = (u8)(row->mask >> 24);
        decoded = 0;
        flag = 0;
        signedValue = 0;

        if (type == 2 || type == 3) {
            if ((srcByte & maskByte) != 0) {
                decoded = *stream++;
                if (type == 3) {
                    signedValue = (s8)decoded;
                }
            }
        } else {
            bit = 0;
            while (bit < 8 && ((maskByte >> bit) & 1) == 0) {
                bit++;
            }
            decoded = (u8)((srcByte & maskByte) >> bit);
            if (type == 0) {
                flag = decoded != 0;
            }
        }

        switch (row->field) {
        case 0:
            PEOPLE_SET_FLAG_BYTE(dst[0], 0x80, flag);
            break;
        case 2:
            PEOPLE_SET_FLAG_BYTE(dst[0], 0x20, flag);
            break;
        case 3:
            dst[0] = (u8)((dst[0] & (u8)~0x1e) | ((decoded & 0xf) << 1));
            break;
        case 4:
            dst[1] = (u8)((dst[1] & 0x0f) | ((decoded & 0xf) << 4));
            break;
        case 5:
            dst[1] = (u8)((dst[1] & 0xf0) | (decoded & 0xf));
            break;
        case 6:
            dst[2] = (u8)((dst[2] & 0x0f) | ((decoded & 0xf) << 4));
            break;
        case 7:
            dst[2] = (u8)((dst[2] & 0xf0) | (decoded & 0xf));
            break;
        case 8:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x80, flag);
            break;
        case 9:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x40, flag);
            break;
        case 10:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x20, flag);
            break;
        case 11:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x10, flag);
            break;
        case 12:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x08, flag);
            break;
        case 13:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x04, flag);
            break;
        case 14:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x02, flag);
            break;
        case 15:
            PEOPLE_SET_FLAG_BYTE(dst[3], 0x01, flag);
            break;
        case 16:
            PEOPLE_SET_FLAG_BYTE(dst[4], 0x80, flag);
            break;
        case 17:
            dst[8] = (u8)decoded;
            break;
        case 18:
            dst[9] = (u8)decoded;
            break;
        case 19:
            PEOPLE_SET_FLAG_BYTE(dst[4], 0x40, flag);
            break;
        case 20:
            dst[0xa] = (u8)decoded;
            break;
        case 21:
            PEOPLE_SET_FLAG_BYTE(dst[4], 0x20, flag);
            break;
        case 22:
            dst[0xb] = (u8)decoded;
            break;
        case 23:
            PEOPLE_SET_FLAG_BYTE(dst[4], 0x10, flag);
            break;
        case 24:
            dst[0xc] = (u8)decoded;
            break;
        case 25:
            dst[0xd] = (u8)decoded;
            break;
        case 26:
            dst[0xe] = (u8)decoded;
            break;
        case 27:
            dst[0xf] = (u8)decoded;
            break;
        case 28:
            PEOPLE_SET_FLAG_BYTE(dst[4], 0x08, flag);
            break;
        case 29:
            dst[5] = (u8)signedValue;
            break;
        case 30:
            dst[6] = (u8)signedValue;
            break;
        case 31:
            dst[7] = (u8)signedValue;
            break;
        default:
            break;
        }
    }
}
#endif
#if 0
asm void fn_801431AC(void) {
#include "src/game/people/people_data_fn_801431AC.inc"
}
#else
s32 itemParamGetRecoverType(u8* itemParam) {
    s32 selectedType;
    s32 selectedCount;
    s32 returnType;
    u32 bitValue;
    u8 firstGate;
    u8 statusNibble;
    u8 byteFlag;

    if (itemParam == NULL) {
        return 0x16;
    }

    if (itemParam == NULL) {
        firstGate = 0;
    } else {
        bitValue = (itemParam[0] >> 5) & 1;
        firstGate = ((-bitValue | bitValue) >> 31);
    }
    if (firstGate) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        statusNibble = 0;
    } else {
        statusNibble = (itemParam[0] >> 1) & 0xF;
    }
    if (statusNibble) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        statusNibble = 0;
    } else {
        statusNibble = (itemParam[1] >> 4) & 0xF;
    }
    if (statusNibble) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        statusNibble = 0;
    } else {
        statusNibble = itemParam[1] & 0xF;
    }
    if (statusNibble) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        statusNibble = 0;
    } else {
        statusNibble = (itemParam[2] >> 4) & 0xF;
    }
    if (statusNibble) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        statusNibble = 0;
    } else {
        statusNibble = itemParam[2] & 0xF;
    }
    if (statusNibble) {
        goto returnZero;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 7) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (!byteFlag) {
        goto afterReturnZero;
    }
returnZero:
    return 0;
afterReturnZero:

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[0] >> 6) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        return 1;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 6) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        return 2;
    }

    selectedCount = 0;

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 5) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 3;
        selectedCount = 1;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 4) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 4;
        selectedCount++;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 3) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 5;
        selectedCount++;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 2) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 6;
        selectedCount++;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[3] >> 1) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 7;
        selectedCount++;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = itemParam[3] & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 8;
        selectedCount++;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[0] >> 7) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        selectedType = 9;
        selectedCount++;
    }

    if (selectedCount > 0) {
        returnType = selectedType;
        if (selectedCount > 1) {
            returnType = 0xA;
        }
        return returnType;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xA];
    }
    if (byteFlag) {
        return 0xB;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[9];
    }
    if (byteFlag) {
        return 0xC;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[8];
    }
    if (byteFlag) {
        return 0xD;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xF];
    }
    if (byteFlag) {
        return 0xE;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xE];
    }
    if (byteFlag) {
        return 0xF;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xD];
    }
    if (byteFlag) {
        return 0x10;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xC];
    }
    if (byteFlag) {
        return 0x11;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[4] >> 4) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        return 0x12;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[4] >> 7) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        return 0x13;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[4] >> 3) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        return 0x14;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        bitValue = (itemParam[4] >> 5) & 1;
        byteFlag = ((-bitValue | bitValue) >> 31);
    }
    if (byteFlag) {
        goto returnFifteen;
    }

    if (itemParam == NULL) {
        byteFlag = 0;
    } else {
        byteFlag = itemParam[0xB];
    }
    if (!byteFlag) {
        goto returnSixteen;
    }

returnFifteen:
    return 0x15;
returnSixteen:
    return 0x16;
}
#endif
#if 0
asm void fn_801436B8(void) {
#include "src/game/people/people_data_fn_801436B8.inc"
}
#else
#pragma optimization_level 4
s8 fn_801436B8(u8* p) {
    if (p == NULL) return 0;
    return (s8)p[0x6];
}
#endif
#if 0
asm void fn_801436D4(void) {
#include "src/game/people/people_data_fn_801436D4.inc"
}
#else
#pragma optimization_level 4
s8 fn_801436D4(u8* p) {
    if (p == NULL) return 0;
    return (s8)p[0x5];
}
#endif
#if 0
asm void fn_801436F0(void) {
#include "src/game/people/people_data_fn_801436F0.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801436F0(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x4] >> 3) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143718(void) {
#include "src/game/people/people_data_fn_80143718.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143718(u8* p) {
    if (p == NULL) return 0;
    return p[0xf];
}
#endif
#if 0
asm void fn_80143730(void) {
#include "src/game/people/people_data_fn_80143730.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143730(u8* p) {
    if (p == NULL) return 0;
    return p[0xe];
}
#endif
#if 0
asm void fn_80143748(void) {
#include "src/game/people/people_data_fn_80143748.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143748(u8* p) {
    if (p == NULL) return 0;
    return p[0xd];
}
#endif
#if 0
asm void fn_80143760(void) {
#include "src/game/people/people_data_fn_80143760.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143760(u8* p) {
    if (p == NULL) return 0;
    return p[0xc];
}
#endif
#if 0
asm void fn_80143778(void) {
#include "src/game/people/people_data_fn_80143778.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143778(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x4] >> 4) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801437A0(void) {
#include "src/game/people/people_data_fn_801437A0.inc"
}
#else
#pragma optimization_level 4
u8 fn_801437A0(u8* p) {
    if (p == NULL) return 0;
    return p[0xb];
}
#endif
#if 0
asm void fn_801437B8(void) {
#include "src/game/people/people_data_fn_801437B8.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801437B8(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x4] >> 5) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801437E0(void) {
#include "src/game/people/people_data_fn_801437E0.inc"
}
#else
#pragma optimization_level 4
u8 fn_801437E0(u8* p) {
    if (p == NULL) return 0;
    return p[0xa];
}
#endif
#if 0
asm void fn_801437F8(void) {
#include "src/game/people/people_data_fn_801437F8.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801437F8(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x4] >> 6) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143820(void) {
#include "src/game/people/people_data_fn_80143820.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143820(u8* p) {
    if (p == NULL) return 0;
    return p[0x9];
}
#endif
#if 0
asm void fn_80143838(void) {
#include "src/game/people/people_data_fn_80143838.inc"
}
#else
#pragma optimization_level 4
u8 fn_80143838(u8* p) {
    if (p == NULL) return 0;
    return p[0x8];
}
#endif
#if 0
asm void fn_80143850(void) {
#include "src/game/people/people_data_fn_80143850.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143850(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x4] >> 7) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143878(void) {
#include "src/game/people/people_data_fn_80143878.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143878(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!(p[0x3] & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801438A0(void) {
#include "src/game/people/people_data_fn_801438A0.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801438A0(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 1) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801438C8(void) {
#include "src/game/people/people_data_fn_801438C8.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801438C8(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 2) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801438F0(void) {
#include "src/game/people/people_data_fn_801438F0.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_801438F0(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 3) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143918(void) {
#include "src/game/people/people_data_fn_80143918.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143918(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 4) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143940(void) {
#include "src/game/people/people_data_fn_80143940.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143940(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 5) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143968(void) {
#include "src/game/people/people_data_fn_80143968.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143968(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 6) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143990(void) {
#include "src/game/people/people_data_fn_80143990.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143990(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x3] >> 7) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_801439B8(void) {
#include "src/game/people/people_data_fn_801439B8.inc"
}
#else
#pragma optimization_level 4
u32 fn_801439B8(u8* p) {
    if (p == NULL) return 0;
    return (u32)(p[0x2] & 0xF);
}
#endif
#if 0
asm void fn_801439D4(void) {
#include "src/game/people/people_data_fn_801439D4.inc"
}
#else
#pragma optimization_level 4
u32 fn_801439D4(u8* p) {
    if (p == NULL) return 0;
    return (u32)((p[0x2] >> 4) & 0xF);
}
#endif
#if 0
asm void fn_801439F0(void) {
#include "src/game/people/people_data_fn_801439F0.inc"
}
#else
#pragma optimization_level 4
u32 fn_801439F0(u8* p) {
    if (p == NULL) return 0;
    return (u32)(p[0x1] & 0xF);
}
#endif
#if 0
asm void fn_80143A0C(void) {
#include "src/game/people/people_data_fn_80143A0C.inc"
}
#else
#pragma optimization_level 4
u32 fn_80143A0C(u8* p) {
    if (p == NULL) return 0;
    return (u32)((p[0x1] >> 4) & 0xF);
}
#endif
#if 0
asm void fn_80143A28(void) {
#include "src/game/people/people_data_fn_80143A28.inc"
}
#else
#pragma optimization_level 4
u32 fn_80143A28(u8* p) {
    if (p == NULL) return 0;
    return (u32)((p[0x0] >> 1) & 0xF);
}
#endif
#if 0
asm void fn_80143A44(void) {
#include "src/game/people/people_data_fn_80143A44.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143A44(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x0] >> 5) & 1);
    return v;
}
#pragma pop
#endif
#if 0
asm void fn_80143A6C(void) {
#include "src/game/people/people_data_fn_80143A6C.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
s32 fn_80143A6C(u8* p) {
    s32 v;
    if (p == NULL) return 0;
    v = !!((p[0x0] >> 7) & 1);
    return v;
}
#pragma pop
#endif
extern u32 lbl_80478BE0;
extern u8 lbl_80368630[];
#if 0
asm void fn_80143A94(void) {
#include "src/game/people/people_data_fn_80143A94.inc"
}
#else
#pragma optimization_level 4
u8* fn_80143A94(u8 idx) {
    u8* result = &lbl_80368630[(u8)idx * 16];
    if (idx < lbl_80478BE0) return result;
    return NULL;
}
#endif
#if 0
asm void fn_80143ABC(void) {
#include "src/game/people/people_data_fn_80143ABC.inc"
}
#else
#pragma optimization_level 4
s8 fn_80143ABC(u8* p, u16 idx) {
    if (p == NULL) return 0;
    if ((u32)(idx & 0xFFFF) >= 0x19) return 0;
    return (s8)(p[idx + 4]);
}
#endif
#if 0
asm void fn_80143AF0(void) {
#include "src/game/people/people_data_fn_80143AF0.inc"
}
#else
#pragma optimization_level 4
u32 fn_80143AF0(u8* p) {
    if (p == NULL) return 0;
    return *(u32*)p;
}
#endif
extern u32 lbl_80478BC8;
extern u8 lbl_80367F78[];
#if 0
asm void fn_80143B08(void) {
#include "src/game/people/people_data_fn_80143B08.inc"
}
#else
#pragma optimization_level 4
u8* fn_80143B08(u16 idx) {
    u8* result = &lbl_80367F78[(u16)idx * 32];
    if ((u16)idx < lbl_80478BC8) return result;
    return NULL;
}
#endif
#if 0
asm void fn_80143B30(void) {
#include "src/game/people/people_data_fn_80143B30.inc"
}
#else
#pragma optimization_level 4
u16 fn_80143B30(u8* p) {
    if (p == NULL) return 0;
    return *(u16*)p;
}
#endif
extern u32 lbl_80478BC0;
extern u8 lbl_80367EF0[];
#if 0
asm void fn_80143B48(void) {
#include "src/game/people/people_data_fn_80143B48.inc"
}
#else
#pragma optimization_level 4
u8* fn_80143B48(u16 idx) {
    u8* result = &lbl_80367EF0[(u16)idx * 2];
    if ((u16)idx < lbl_80478BC0) return result;
    return NULL;
}
#endif
#if 0
asm void fn_80143B70(void) {
#include "src/game/people/people_data_fn_80143B70.inc"
}
#else
#pragma optimization_level 4
void fn_80143B70(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0x2) = val;
}
#endif
#if 0
asm void fn_80143B80(void) {
#include "src/game/people/people_data_fn_80143B80.inc"
}
#else
#pragma optimization_level 4
void fn_80143B80(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0x0) = val;
}
#endif
#if 0
asm void fn_80143B90(void) {
#include "src/game/people/people_data_fn_80143B90.inc"
}
#else
#pragma optimization_level 4
void fn_80143B90(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0x0) = val;
}
#endif
#if 0
asm void fn_80143BA0(void) {
#include "src/game/people/people_data_fn_80143BA0.inc"
}
#else
#pragma optimization_level 4
void fn_80143BA0(u8* p, u32 val) {
    if (p == NULL) return;
    *(u32*)(p + 0x18) = val;
}
#endif
#if 0
asm void fn_80143BB0(void) {
#include "src/game/people/people_data_fn_80143BB0.inc"
}
#else
#pragma optimization_level 4
void fn_80143BB0(u8* p, u16 idx, u8 val) {
    u32 i;
    if (p == NULL) return;
    i = (u32)(idx & 0xFFFF);
    if (i >= 3) return;
    p[i + 0x24] = val;
}
#endif
#if 0
asm void fn_80143BD0(void) {
#include "src/game/people/people_data_fn_80143BD0.inc"
}
#else
#pragma optimization_level 4
void fn_80143BD0(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0xc) = val;
}
#endif
#if 0
asm void fn_80143BE0(void) {
#include "src/game/people/people_data_fn_80143BE0.inc"
}
#else
#pragma optimization_level 4
void fn_80143BE0(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0xa) = val;
}
#endif
#if 0
asm void fn_80143BF0(void) {
#include "src/game/people/people_data_fn_80143BF0.inc"
}
#else
#pragma optimization_level 4
void fn_80143BF0(u8* p, u32 val) {
    if (p == NULL) return;
    *(u32*)(p + 0x14) = val;
}
#endif
#if 0
asm void fn_80143C00(void) {
#include "src/game/people/people_data_fn_80143C00.inc"
}
#else
#pragma optimization_level 4
void fn_80143C00(u8* p, u8 val) {
    if (p == NULL) return;
    p[0x2] = val;
}
#endif
#if 0
asm void fn_80143C10(void) {
#include "src/game/people/people_data_fn_80143C10.inc"
}
#else
#pragma optimization_level 4
void fn_80143C10(u8* p, u8 val) {
    if (p == NULL) return;
    p[0x1] = val;
}
#endif
#if 0
asm void fn_80143C20(void) {
#include "src/game/people/people_data_fn_80143C20.inc"
}
#else
#pragma optimization_level 4
void fn_80143C20(u8* p, u16 val) {
    if (p == NULL) return;
    *(u16*)(p + 0x6) = val;
}
#endif
#if 0
asm void fn_80143C30(void) {
#include "src/game/people/people_data_fn_80143C30.inc"
}
#else
#pragma optimization_level 4
void fn_80143C30(u8* p, u8 val) {
    if (p == NULL) return;
    p[0x0] = val;
}
#endif
#if 0
asm void fn_80143C40(void) {
#include "src/game/people/people_data_fn_80143C40.inc"
}
#else
#pragma optimization_level 4
void fn_80143C40(u8* p, u32 val) {
    if (p == NULL) return;
    *(u32*)(p + 0x10) = val;
}
#endif
