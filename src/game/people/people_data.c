/**
 * @file people_data.c
 * @brief People/NPC field-level data management -- allocation, lookup,
 *        model loading, accessor functions.
 *
 * Address range: 0x80140588 - 0x80144574 (approximately 0x3FEC bytes)
 * Function count: ~100 functions (83 of which are tiny getters/setters)
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
 * Global data (sdata/sbss)
 * =================================================================== */

extern u32 lbl_80478BD8;   /* gPeopleFieldCount */
extern u32 lbl_80478BB0;   /* gPeopleFieldMaxSlots */
extern u16 lbl_803681E8[]; /* gPeopleFieldLookup */
extern u8  lbl_80363CE8[]; /* gPeopleFieldArray base */

extern u32 lbl_80478BE0;   /* max index for 0x10-stride array */
extern u8  lbl_80368630[]; /* base of 0x10-stride array */

extern u32 lbl_80478BC8;   /* max index for 0x20-stride array */
extern u8  lbl_80367F78[]; /* base of 0x20-stride array */

extern u32 lbl_80478BC0;   /* max index for 0x02-stride array */
extern u8  lbl_80367EF0[]; /* base of 0x02-stride array */

extern u32 lbl_80478BB8;   /* max index for 0x30-stride array */
extern u8  lbl_80367AF0[]; /* base of 0x30-stride array */

extern u32 lbl_80478BD0;   /* max index for 0x08-stride array */
extern u8  lbl_80368018[]; /* base of 0x08-stride array */

/* ===================================================================
 * STUB DECLARATIONS -- large functions deferred
 * =================================================================== */

/* fn_80140588: peopleFieldOpen (0x514 bytes) */
/* fn_80140A9C: peopleFieldGetSlot (0x30 bytes) */
/* fn_80140ACC: peopleFieldLoadModel (0x83C bytes) */
/* fn_80141308: peopleFieldUpdate (0x1060 bytes) */
/* fn_80142368: peopleFieldCleanup (0x280 bytes) */
/* fn_801425E8: peopleFieldSetup (0x39C bytes) */
/* fn_80142984: peopleFieldGetByID (0x64 bytes) */
/* fn_801429E8: peopleFieldGetEntry (0xA0 bytes) */
/* fn_80142A88: peopleFieldSetState (0x9C bytes) */
/* fn_80142B24: peopleFieldApplyMotion (0x1D0 bytes) */
/* fn_80142CF4: peopleFieldAlloc (0x204 bytes) */
/* fn_80142EF8: peopleFieldRelease (0x2B4 bytes) */
/* fn_801431AC: peopleFieldInit (0x4F0 bytes) */

/* ===================================================================
 * DECOMPILED: Getter/Setter cluster (0x8014369C - 0x80144088)
 *
 * Tiny accessor functions for NPC field data structs.
 * Null-safe: returns 0 / does nothing if ptr is NULL.
 * =================================================================== */

/* Address: 0x8014369C */
s32 fn_8014369C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x7];
}

/* Address: 0x801436B8 */
s32 fn_801436B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x6];
}

/* Address: 0x801436D4 */
s32 fn_801436D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x5];
}

/* Address: 0x801436F0 */
u32 fn_801436F0(u8* ptr) {
    u32 bit;
    if (ptr == NULL) { return 0; }
    bit = (ptr[0x4] >> 3) & 1;
    return ((-bit) | bit) >> 31;
}

/* Address: 0x80143718 */
u32 fn_80143718(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xF];
}

/* Address: 0x80143730 */
u32 fn_80143730(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xE];
}

/* Address: 0x80143748 */
u32 fn_80143748(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xD];
}

/* Address: 0x80143760 */
u32 fn_80143760(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xC];
}

/* Address: 0x80143778 */
u32 fn_80143778(u8* ptr) {
    u32 bit;
    if (ptr == NULL) { return 0; }
    bit = (ptr[0x4] >> 4) & 1;
    return ((-bit) | bit) >> 31;
}

/* Address: 0x801437A0 */
u32 fn_801437A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xB];
}

/* Address: 0x801437B8 */
u32 fn_801437B8(u8* ptr) {
    u32 bit;
    if (ptr == NULL) { return 0; }
    bit = (ptr[0x4] >> 5) & 1;
    return ((-bit) | bit) >> 31;
}

/* Address: 0x801437E0 */
u32 fn_801437E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA];
}

/* Address: 0x801437F8 */
u32 fn_801437F8(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x4] >> 6) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143820 */
u32 fn_80143820(u8* ptr) { if (ptr == NULL) { return 0; } return ptr[0x9]; }

/* Address: 0x80143838 */
u32 fn_80143838(u8* ptr) { if (ptr == NULL) { return 0; } return ptr[0x8]; }

/* Address: 0x80143850 */
u32 fn_80143850(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x4] >> 7) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143878 */
u32 fn_80143878(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = ptr[0x3] & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x801438A0 */
u32 fn_801438A0(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 1) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x801438C8 */
u32 fn_801438C8(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 2) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x801438F0 */
u32 fn_801438F0(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 3) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143918 */
u32 fn_80143918(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 4) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143940 */
u32 fn_80143940(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 5) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143968 */
u32 fn_80143968(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 6) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143990 */
u32 fn_80143990(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x3] >> 7) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x801439B8 */
u32 fn_801439B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x2] & 0x0F;
}

/* Address: 0x801439D4 */
u32 fn_801439D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (ptr[0x2] >> 4) & 0x0F;
}

/* Address: 0x801439F0 */
u32 fn_801439F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x1] & 0x0F;
}

/* Address: 0x80143A0C */
u32 fn_80143A0C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (ptr[0x1] >> 4) & 0x0F;
}

/* Address: 0x80143A28 */
u32 fn_80143A28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (ptr[0x0] >> 1) & 0x0F;
}

/* Address: 0x80143A44 */
u32 fn_80143A44(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x0] >> 5) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143A6C */
u32 fn_80143A6C(u8* ptr) { u32 bit; if (ptr == NULL) { return 0; } bit = (ptr[0x0] >> 7) & 1; return ((-bit) | bit) >> 31; }

/* Address: 0x80143A94 - lookup by u8 index into 0x10-stride array */
u8* fn_80143A94(u8 index) {
    if ((u8)index >= lbl_80478BE0) {
        return NULL;
    }
    return &lbl_80368630[(u8)index * 0x10];
}

/* Address: 0x80143ABC - indexed byte getter with bounds check */
s32 fn_80143ABC(u8* ptr, u16 index) {
    if (ptr == NULL) { return 0; }
    if ((u16)index >= 0x19) { return 0; }
    return (s8)ptr[(u16)index + 0x4];
}

/* Address: 0x80143AF0 */
u32 fn_80143AF0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* Address: 0x80143B08 - lookup by u16 index into 0x20-stride array */
u8* fn_80143B08(u16 index) {
    if ((u16)index >= lbl_80478BC8) {
        return NULL;
    }
    return &lbl_80367F78[(u16)index * 0x20];
}

/* Address: 0x80143B30 */
u32 fn_80143B30(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x80143B48 - lookup by u16 index into 0x02-stride array */
u8* fn_80143B48(u16 index) {
    if ((u16)index >= lbl_80478BC0) {
        return NULL;
    }
    return &lbl_80367EF0[(u16)index * 0x02];
}

/* Address: 0x80143B70 */
void fn_80143B70(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x80143B80 */
void fn_80143B80(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x80143B90 */
void fn_80143B90(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x80143BA0 */
void fn_80143BA0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}

/* Address: 0x80143BB0 */
void fn_80143BB0(u8* ptr, u16 index, u8 val) {
    if (ptr == NULL) { return; }
    if ((u16)index >= 3) { return; }
    ptr[(u16)index + 0x24] = val;
}

/* Address: 0x80143BD0 */
void fn_80143BD0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}

/* Address: 0x80143BE0 */
void fn_80143BE0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}

/* Address: 0x80143BF0 */
void fn_80143BF0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x80143C00 */
void fn_80143C00(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0x2] = val;
}

/* Address: 0x80143C10 */
void fn_80143C10(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0x1] = val;
}

/* Address: 0x80143C20 */
void fn_80143C20(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}

/* Address: 0x80143C30 */
void fn_80143C30(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0x0] = val;
}

/* Address: 0x80143C40 */
void fn_80143C40(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x80143C50 */
u32 fn_80143C50(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x80143C68 */
u32 fn_80143C68(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x80143C80 */
u32 fn_80143C80(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}

/* Address: 0x80143C98 */
u32 fn_80143C98(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x28]);
}

/* Address: 0x80143CB0 */
u32 fn_80143CB0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x24]);
}

/* Address: 0x80143CC8 */
u32 fn_80143CC8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}

/* Address: 0x80143CE0 */
u32 fn_80143CE0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}

/* Address: 0x80143CF8 */
u32 fn_80143CF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x18]);
}

/* Address: 0x80143D10 */
u32 fn_80143D10(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x80143D28 */
u32 fn_80143D28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x80143D40 */
u32 fn_80143D40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x80143D58 */
u32 fn_80143D58(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x80143D70 */
u32 fn_80143D70(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x80143D88 */
u32 fn_80143D88(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x80143DA0 - lookup by u16 index into 0x30-stride array */
u8* fn_80143DA0(u16 index) {
    if ((u16)index >= lbl_80478BB8) {
        return NULL;
    }
    return &lbl_80367AF0[(u16)index * 0x30];
}

/* Address: 0x80143DCC */
u32 fn_80143DCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}

/* Address: 0x80143DE4 */
u32 fn_80143DE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}

/* Address: 0x80143DFC */
u32 fn_80143DFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x4];
}

/* Address: 0x80143E14 */
u32 fn_80143E14(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x18]);
}

/* Address: 0x80143E2C - indexed byte getter at 0x24 (s8) */
s32 fn_80143E2C(u8* ptr, u16 index) {
    if (ptr == NULL) { return 0; }
    if ((u16)index >= 3) { return 0; }
    return (s8)ptr[(u16)index + 0x24];
}

/* Address: 0x80143E60 - index range remap */
u32 fn_80143E60(u16 index) {
    u16 val;
    val = (u16)index;
    if (val < 0x85) {
        return 0xFF;
    }
    if (val > 0xAF) {
        return 0xFF;
    }
    return (u8)(val - 0x85);
}

/* Address: 0x80143F54 */
u32 fn_80143F54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}

/* Address: 0x80143F6C */
u32 fn_80143F6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}

/* Address: 0x80143F84 */
u32 fn_80143F84(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x80143F9C */
u32 fn_80143F9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x3];
}

/* Address: 0x80143FB4 */
u32 fn_80143FB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x2];
}

/* Address: 0x80143FCC */
u32 fn_80143FCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x1];
}

/* Address: 0x80143FE4 */
u32 fn_80143FE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8]);
}

/* Address: 0x80143FFC */
u32 fn_80143FFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}

/* Address: 0x80144014 */
u32 fn_80144014(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0x0];
}

/* Address: 0x80144064 - testWord10 nonzero */
u32 fn_80144064(u8* ptr) { u32 val; if (ptr == NULL) { return 0; } val = *(u32*)(&ptr[0x10]); return ((-val) | val) >> 31; }

/* Address: 0x80144088 */
u32 fn_80144088(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* ===================================================================
 * DECOMPILED: fn_801440A0 -- peopleFieldGetByIndex
 * =================================================================== */

typedef struct PeopleFieldEntry {
    u8 data[0x28];
} PeopleFieldEntry;

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
 * STUB -- complex/large functions (deferred)
 * =================================================================== */

/* fn_80143E88: complex getter (0x68 bytes) */
/* fn_80143EF0: global lookup (0x34 bytes) */
/* fn_80143F24: complex getter (0x30 bytes) */
/* fn_8014402C: complex conditional (0x38 bytes) */
/* fn_801440F0: peopleFieldOpenModel (0xB8 bytes) */
/* fn_801441A8: peopleFieldConfigModel (0x224 bytes) */
/* fn_801443CC: peopleFieldFinalizeModel (0x1A8 bytes) */
