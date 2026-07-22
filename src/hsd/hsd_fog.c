/**
 * @file hsd_fog.c
 * @brief HSD ID table, Hash table and Fog implementation.
 *
 * Colosseum address range: 0x8019B7C0 - 0x8019C690
 * Adapted from doldecomp/melee src/sysdolphin/baselib/{fog,hash,id}.c
 */

#include "hsd/hsd_fog.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_hash.h"
#include "hsd/hsd_id.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_objalloc.h"

extern void* memcpy(void* dst, const void* src, u32 size);
extern void* memset(void* dst, int val, u32 n);

/* The class-info objects for this unit live in the shared data TU
 * (src/game/data/data_8036C720.c), so they are reached by their raw labels:
 *   lbl_8036C7E8 == hsdFog     (HSD_FogInfo,    0x40 bytes)
 *   lbl_8036C828 == hsdFogAdj  (HSD_FogAdjInfo, 0x3C bytes)
 *   lbl_8036CC00 == hsdObj     (HSD_ObjInfo,    0x3C bytes)  [shared] */
extern HSD_FogInfo lbl_8036C7E8;
extern HSD_FogAdjInfo lbl_8036C828;
#define hsdFog lbl_8036C7E8
#define hsdFogAdj lbl_8036C828

/* String literals, held in the shared rodata/sdata2 TUs. */
extern const char lbl_802747B8[];   /* "sysdolphin_base_library" */
extern const char lbl_802747D0[];   /* "hsd_fogadj"              */
extern const char lbl_8047DA60;     /* "hsd_fog"                 */

static void FogRelease(HSD_Fog* fog);
void FogUpdateFunc(HSD_Fog* fog, s32 type, f32* value);

/* ===================================================================
 * Range: 0x8019B7C0 - 0x8019C690
 * =================================================================== */

/* 0x8019B7C0 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static void FogAdjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFogAdj), HSD_CLASS_INFO(&hsdObj),
                     (char*) lbl_802747B8, (char*) lbl_802747D0,
                     sizeof(HSD_FogAdjInfo), sizeof(HSD_FogAdj));
}
#pragma pop

/* 0x8019B808 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static void FogInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFog), HSD_CLASS_INFO(&hsdObj),
                     (char*) lbl_802747B8, (char*) &lbl_8047DA60,
                     sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    HSD_CLASS_INFO(&hsdFog)->release = (void (*)(HSD_Class*)) FogRelease;
    hsdFog.update = FogUpdateFunc;
}
#pragma pop

/* 0x8019B874 | 0xD4 */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
/* FogRelease virtual (class-info slot 0x30). */
static void FogRelease(HSD_Fog* fog)
{
    HSD_FogAdj* adj = fog->fog_adj;

    if (adj != NULL) {
        if (ref_DEC(adj)) {
            hsdDelete(adj);
        }
    }
    HSD_AObjRemove(fog->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFog)->release((HSD_Class*) fog);
}
#pragma pop

/* 0x8019B948 | 0x230 */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
extern void jumptable_8036C864();
extern u32 lbl_8047DA68;
extern u32 lbl_8047DA6C;
extern u32 lbl_8047DA70;
#if 0
asm void FogUpdateFunc(void) {
#include "src/hsd/hsd_fog_fn_8019B948.inc"
}
#else
/* decompiled glm6: functional (W-SDA-WRAPPER wall caps this TU <100% even for the
 * asm reference: 0x8019B948=91%, 0x8019BB78=95%, 0x8019BD18=96%; byte-exact is
 * unattainable, so faithful readable C is the target) */
/*
 * Fog AObj interpret callback. Writes one animated attribute (selected by
 * `type`, 0..0x15) into the fog object. Inferred signature:
 *   void FogUpdateFunc(HSD_Fog* fog, s32 type, f32* value)
 *   0 start | 1 end | 2..5 color R/G/B/A | 6 fog_adj.center | 7 fog_adj.width
 *   8..0x15 are no-ops (jumptable falls through to the return).
 *
 * HSD_FogAdj's header layout is verified against 0x8019B948/BB78/BD18:
 *   flags@0x08  center@0x0C  width@0x0E  mtx[4][4]@0x10.
 * The body retains explicit offsets to preserve the current codegen.
 */
void FogUpdateFunc(HSD_Fog* fog, s32 type, f32* value)
{
    f32 fvalue;
    s32 ivalue;

    if (fog == NULL) {
        return;
    }

    switch ((u32)type) {
    case 0: /* start */
        fog->start = *value;
        break;

    case 1: /* end */
        fog->end = *value;
        break;

    case 2: /* color R */
        fvalue = *value;
        if (fvalue <= *(f32*)&lbl_8047DA68) {
            fvalue = *(volatile f32*)&lbl_8047DA68;
        } else if (fvalue >= *(f32*)&lbl_8047DA6C) {
            fvalue = *(volatile f32*)&lbl_8047DA6C;
        }
        ((u8*)&fog->color)[0] = (u8)(*(f32*)&lbl_8047DA70 * fvalue);
        break;

    case 3: /* color G */
        fvalue = *value;
        if (fvalue <= *(f32*)&lbl_8047DA68) {
            fvalue = *(volatile f32*)&lbl_8047DA68;
        } else if (fvalue >= *(f32*)&lbl_8047DA6C) {
            fvalue = *(volatile f32*)&lbl_8047DA6C;
        }
        ((u8*)&fog->color)[1] = (u8)(*(f32*)&lbl_8047DA70 * fvalue);
        break;

    case 4: /* color B */
        fvalue = *value;
        if (fvalue <= *(f32*)&lbl_8047DA68) {
            fvalue = *(volatile f32*)&lbl_8047DA68;
        } else if (fvalue >= *(f32*)&lbl_8047DA6C) {
            fvalue = *(volatile f32*)&lbl_8047DA6C;
        }
        ((u8*)&fog->color)[2] = (u8)(*(f32*)&lbl_8047DA70 * fvalue);
        break;

    case 5: /* color A */
        fvalue = *value;
        if (fvalue <= *(f32*)&lbl_8047DA68) {
            fvalue = *(volatile f32*)&lbl_8047DA68;
        } else if (fvalue >= *(f32*)&lbl_8047DA6C) {
            fvalue = *(volatile f32*)&lbl_8047DA6C;
        }
        ((u8*)&fog->color)[3] = (u8)(*(f32*)&lbl_8047DA70 * fvalue);
        break;

    case 6: /* fog_adj.center (clamped to [-320, 320]) */
        ivalue = (s32)*value;
        if (fog != NULL) {
            fog = (HSD_Fog*)fog->fog_adj;
        } else {
            fog = NULL;
        }
        if (fog != NULL) {
            if (ivalue <= -0x140) {
                *(s16*)((u8*)fog + 0x0C) = -0x140;
            } else if (ivalue >= 0x140) {
                *(s16*)((u8*)fog + 0x0C) = 0x140;
            } else {
                *(s16*)((u8*)fog + 0x0C) = (s16)ivalue;
            }
        }
        break;

    case 7: /* fog_adj.width (clamped to [0, 640]) */
        ivalue = (s32)*value;
        if (fog != NULL) {
            fog = (HSD_Fog*)fog->fog_adj;
        } else {
            fog = NULL;
        }
        if (fog != NULL) {
            if (ivalue <= 0) {
                *(s16*)((u8*)fog + 0x0E) = 0;
            } else if (ivalue >= 0x280) {
                *(s16*)((u8*)fog + 0x0E) = 0x280;
            } else {
                *(s16*)((u8*)fog + 0x0E) = (s16)ivalue;
            }
        }
        break;

    case 21:
    default:
        break;
    }
}
#endif
#pragma pop

/* 0x8019BB78 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void* fn_80193828(HSD_ClassInfo* info);   /* inferred glm6: == hsdNew */
extern void fn_800BD768(void* dst);              /* inferred glm6: writes viewport/projection scratch */
extern void* memset(void* dst, int val, u32 n);
extern char lbl_8047DA74;   /* "fog.c" */
extern u8 lbl_8047DA80[4];   /* "fog"  */
extern u8 lbl_8047DA7C[4];   /* "adj"  */
#if 0
asm void HSD_FogLoadDesc(void) {
#include "src/hsd/hsd_fog_fn_8019BB78.inc"
}
#else
/* decompiled glm6: functional (W-SDA-WRAPPER wall caps this TU <100% even for the
 * asm reference: 0x8019B948=91%, 0x8019BB78=95%, 0x8019BD18=96%; byte-exact is
 * unattainable, so faithful readable C is the target) */
/*
 * Allocate and initialise an HSD_Fog (plus its HSD_FogAdj) from a descriptor.
 * Inferred signature: HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc).
 *
 * This is the monolithic binary form of what the reference code above splits
 * into HSD_FogAlloc + HSD_FogInit (+ FogAdj alloc/init): it hsdNew()s the fog,
 * copies type/start/end/color from the desc (or zero-inits defaults when the
 * desc is NULL), then hsdNew()s a FogAdj from desc->fogadjdesc and links it.
 *
 * FogAdj fields retain explicit offsets for current codegen. HSD_FogAdjDesc is
 *   flags@0x00  center@0x04  width@0x06  mtx[4][4]@0x08
 * as represented in include/hsd/hsd_fog.h.
 */
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc)
{
    HSD_Fog* fog;
    HSD_FogAdj* adj;

    fog = (HSD_Fog*)fn_80193828((HSD_ClassInfo*)&hsdFog);
    if (fog == NULL) {
        __assert((char*)&lbl_8047DA74, 0xa1, (char*)lbl_8047DA80);
    }
    if (fog == NULL) {
        __assert((char*)&lbl_8047DA74, 0xae, (char*)lbl_8047DA80);
    }

    if (fog != NULL) {
        if (desc != NULL) {
            fog->type = desc->type;
            fog->start = desc->start;
            fog->end = desc->end;
            fog->color = desc->color;
        } else {
            u8 cobj[0x18];
            fn_800BD768(cobj);
            fog->type = 2;
            fog->start = *(f32*)(cobj + 0x10);
            fog->end = *(f32*)(cobj + 0x14);
            ((u8*)&fog->color)[0] = 0xff;
            ((u8*)&fog->color)[1] = 0xff;
            ((u8*)&fog->color)[2] = 0xff;
            ((u8*)&fog->color)[3] = 0xff;
        }
    }

    if (desc != NULL && desc->fogadjdesc != NULL) {
        HSD_FogAdjDesc* fadesc = desc->fogadjdesc;
        adj = (HSD_FogAdj*)fn_80193828((HSD_ClassInfo*)&hsdFogAdj);
        if (adj == NULL) {
            __assert((char*)&lbl_8047DA74, 0xf9, (char*)lbl_8047DA7C);
        }
        if (adj == NULL) {
            __assert((char*)&lbl_8047DA74, 0x109, (char*)lbl_8047DA7C);
        }
        if (adj != NULL) {
            *(u32*)((u8*)adj + 0x08) = *(u32*)((u8*)fadesc + 0x00); /* flags  */
            *(s16*)((u8*)adj + 0x0C) = *(s16*)((u8*)fadesc + 0x04); /* center */
            *(s16*)((u8*)adj + 0x0E) = *(s16*)((u8*)fadesc + 0x06); /* width  */
            memcpy((u8*)adj + 0x10, (u8*)fadesc + 0x08, 0x40);      /* mtx4x4 */
        }
        fog->fog_adj = adj;
    }

    return fog;
}
#endif
#pragma pop

/* 0x8019BD18 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800BC8F8(u32 type, void* color, f32 start, f32 end, f32 znear, f32 zfar); /* inferred glm6: GXSetFog */
extern HSD_CObj* HSD_CObjGetCurrent(void);
extern void HSD_Panic(char* file, u32 line, char* msg);
extern f32 HSD_CObjGetFar(HSD_CObj* cobj);
extern f32 HSD_CObjGetNear(HSD_CObj* cobj);
extern void fn_800BD454(void* dst);                /* inferred glm6: writes 7-float projection desc */
extern void fn_800BCB14(void* table, u32 width, void* projmtx); /* inferred glm6: GXInitFogAdjTable */
extern void fn_800BCCDC(u32 enable, u32 center, void* table);   /* inferred glm6: GXSetFogRangeAdj */
extern u32 lbl_8047E720;   /* default fog colour (read-only) */
extern u8 lbl_802747DC[];  /* HSD_Panic message */
#if 0
asm void HSD_FogSet(void) {
#include "src/hsd/hsd_fog_fn_8019BD18.inc"
}
#else
/* decompiled glm6: functional (W-SDA-WRAPPER wall caps this TU <100% even for the
 * asm reference: 0x8019B948=91%, 0x8019BB78=95%, 0x8019BD18=96%; byte-exact is
 * unattainable, so faithful readable C is the target) */
/*
 * Apply a fog object to the GPU: GXSetFog from the fog params, then optionally
 * configure fog-range adjustment (GXInitFogAdjTable + GXSetFogRangeAdj) from the
 * fog's HSD_FogAdj. Inferred signature: void HSD_FogSet(HSD_Fog* fog).
 *
 * fn_800BD768 fills viewport/projection scratch (reads [0x00] and [0x08]).
 * HSD_FogAdj flags @0x08 select behaviour (master gate = any of bits 29..31):
 *   bit31 -> centre derived from fogadj->center, else centre = proj[0]+proj[8]/2
 *   bit30 -> width  from fogadj->width,  else width  = (s32)proj[8]
 *   bit29 -> matrix from fogadj->mtx,    else matrix rebuilt from fn_800BD454
 * The header matches this layout; the body retains explicit offsets for the
 * current codegen.
 */
void HSD_FogSet(HSD_Fog* fog)
{
    HSD_CObj* cobj;
    u32 color;
    f32 znear;
    f32 zfar;

    if (fog == NULL) {
        color = lbl_8047E720;
        fn_800BC8F8(0, &color, 0.0f, 0.0f, 0.0f, 0.0f);
        return;
    }

    cobj = HSD_CObjGetCurrent();
    if (cobj == NULL) {
        HSD_Panic((char*)&lbl_8047DA74, 0x58, (char*)lbl_802747DC);
    }

    color = fog->color;
    zfar = HSD_CObjGetFar(cobj);
    znear = HSD_CObjGetNear(cobj);
    fn_800BC8F8(fog->type, &color, fog->start, fog->end, znear, zfar);

    {
        HSD_FogAdj* adj = fog->fog_adj;
        u32 flags = (adj != NULL) ? *(u32*)((u8*)adj + 0x08) : 0;

        if (adj != NULL && (flags & 0xE0000000) != 0) {
            u8 proj[0x10];
            u8 local_mtx[0x40];
            u8 fogadj_table[0x14];
            f32 pp[7];
            f32 p0;
            f32 p8;
            s32 center_i;
            s32 width_i;
            void* mtx;
            f32* m;

            fn_800BD768(proj);
            p0 = *(f32*)(proj + 0x00);
            p8 = *(f32*)(proj + 0x08);

            if ((flags & 0x80000000) != 0) {
                /* centre from fogadj->center (+320, scaled by proj w / 640) */
                s32 c = (s32)*(s16*)((u8*)adj + 0x0C);
                center_i = (s32)(p0 + p8 * (c + 320) / 640.0f);
            } else {
                /* centre = proj[0] + proj[8]/2 */
                center_i = (s32)(p0 + p8 * 0.5f);
            }

            if ((flags & 0x40000000) != 0) {
                width_i = (s32)*(u16*)((u8*)adj + 0x0E);
            } else {
                width_i = (s32)p8;
            }

            if ((flags & 0x20000000) != 0) {
                mtx = (void*)((u8*)adj + 0x10);
            } else {
                memset(local_mtx, 0, 0x40);
                fn_800BD454(pp);
                m = (f32*)local_mtx;
                if ((s32)pp[0] == 0) {
                    m[0]  = pp[1];
                    m[2]  = pp[2];
                    m[5]  = pp[3];
                    m[6]  = pp[4];
                    m[10] = pp[5];
                    m[11] = pp[6];
                    m[14] = -1.0f;
                } else {
                    m[0]  = pp[1];
                    m[3]  = pp[2];
                    m[5]  = pp[3];
                    m[7]  = pp[4];
                    m[10] = pp[5];
                    m[11] = pp[6];
                    m[15] = 1.0f;
                }
                mtx = local_mtx;
            }

            fn_800BCB14(fogadj_table, (u32)(u16)width_i, mtx);
            fn_800BCCDC(1, (u32)(u16)center_i, fogadj_table);
        } else {
            fn_800BCCDC(0, 0, 0);
        }
    }
}
#endif
#pragma pop

/* 0x8019BFE8 | 0x110 */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
extern u8 lbl_80274800[];
extern char lbl_8047DA98;
#if 0
asm void HSD_HashSearch(void) {
#include "src/hsd/hsd_fog_HSD_HashSearch.inc"
}
#else
/**
 * Walk the collision chain of bucket @p idx, returning the entry whose key the
 * class's keycheck() reports as equal (0), or NULL. When @p ptr is non-NULL it
 * additionally receives the address of the link that points at the entry.
 */
static inline HSD_HashEntry* HashSearchEntry(HSD_Hash* hash, s32 idx,
                                             void* key, HSD_HashEntry** ptr)
{
    if (hash->table[idx] == NULL) {
        return NULL;
    }
    if (ptr != NULL) {
        HSD_HashEntry** entry;
        for (entry = &hash->table[idx]; *entry != NULL;
             entry = &(*entry)->next)
        {
            if (hash->parent.class_info->keycheck(hash, (*entry)->key, key) ==
                0)
            {
                *ptr = (HSD_HashEntry*) entry;
                return *entry;
            }
        }
    } else {
        HSD_HashEntry* entry;
        for (entry = hash->table[idx]; entry != NULL; entry = entry->next) {
            if (hash->parent.class_info->keycheck(hash, entry->key, key) == 0)
            {
                return entry;
            }
        }
    }
    return NULL;
}

void* HSD_HashSearch(HSD_Hash* hash, void* key, s32* success)
{
    HSD_HashEntry* entry;
    void* search_key;
    u32 idx;

    search_key = key;
    idx = hash->parent.class_info->getidx(hash);
    if (!(idx < hash->table_size)) {
        __assert(&lbl_8047DA98, 0x71, (char*) lbl_80274800);
    }
    entry = HashSearchEntry(hash, idx, search_key, NULL);
    if (success != NULL) {
        *success = !!entry;
    }
    if (entry != NULL) {
        return entry->value;
    }
    return NULL;
}
#endif
#pragma pop

/* 0x8019C0F8 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
/* 0x804653A8: the default (global) ID table. Defined as raw bss in
 * src/game/data/bss_80465080.c; sizeof(HSD_IDTable) == 0x194. */
extern HSD_IDTable lbl_804653A8;
#define default_table lbl_804653A8

static inline u32 hash(u32 id)
{
    return id % 0x65;
}
#if 0
asm void _HSD_IDForgetMemory(void) {
#include "src/hsd/hsd_fog__HSD_IDForgetMemory.inc"
}
#else
void _HSD_IDForgetMemory(void) {
    memset(&default_table, 0, sizeof(HSD_IDTable));
}
#endif
#pragma pop

/* 0x8019C128 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_IDGetDataFromTable(void) {
#include "src/hsd/hsd_fog_HSD_IDGetDataFromTable.inc"
}
#else
#pragma optimization_level 1
void* HSD_IDGetDataFromTable(HSD_IDTable* table, u32 id, s32* success)
{
    IDEntry* entry;

    if (table == NULL) {
        table = &default_table;
    }

    entry = table->table[hash(id)];
    while (entry != NULL) {
        if (entry->id == id) {
            if (success != NULL) {
                *success = 1;
            }
            return entry->data;
        }
        entry = entry->next;
    }

    if (success != NULL) {
        *success = 0;
    }
    return NULL;
}
#endif
#pragma pop

/* 0x8019C1B0 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void HSD_ObjFree(HSD_ObjAllocData* list, void* data);
extern HSD_ObjAllocData lbl_8046553C;

static inline void IDEntryFree(IDEntry* entry)
{
    HSD_ObjFree(&lbl_8046553C, entry);
}
#if 0
asm void HSD_IDRemoveByIDFromTable(void) {
#include "src/hsd/hsd_fog_fn_8019C1B0.inc"
}
#else
#pragma optimization_level 1
void HSD_IDRemoveByIDFromTable(HSD_IDTable* table, u32 id)
{
    IDEntry* entry;
    IDEntry* prev;
    u32 idx;

    if (table == NULL) {
        table = &default_table;
    }

    idx = hash(id);
    prev = NULL;
    for (entry = table->table[idx]; entry != NULL; entry = entry->next) {
        if (entry->id == id) {
            if (prev != NULL) {
                prev->next = entry->next;
            } else {
                table->table[idx] = entry->next;
            }
            IDEntryFree(entry);
            return;
        }
        prev = entry;
    }
}
#endif
#pragma pop

/* 0x8019C264 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern char lbl_8047DAA0;   /* "id.c"  */
extern char lbl_8047DAA8;   /* "entry" */

static inline IDEntry* IDEntryAlloc(void)
{
    IDEntry* entry;

    entry = HSD_ObjAlloc(&lbl_8046553C);
    if (entry == NULL) {
        __assert(&lbl_8047DAA0, 0x43, &lbl_8047DAA8);
    }
    memset(entry, 0, sizeof(IDEntry));

    return entry;
}

static inline IDEntry* IDEntryAllocForInsert(void)
{
    return IDEntryAlloc();
}
#if 0
asm void HSD_IDInsertToTable(void) {
#include "src/hsd/hsd_fog_HSD_IDInsertToTable.inc"
}
#else
#pragma optimization_level 1
#pragma use_lmw_stmw on
void HSD_IDInsertToTable(HSD_IDTable* table, u32 id, void* data)
{
    IDEntry* entry;
    u32 idx;

    if (table == NULL) {
        table = &default_table;
    }

    idx = hash(id);
    entry = table->table[idx];
    while (entry != NULL) {
        if (entry->id == id) {
            break;
        }
        entry = entry->next;
    }

    if (entry != NULL) {
        entry->id = id;
        entry->data = data;
    } else {
        entry = IDEntryAllocForInsert();
        entry->id = id;
        entry->data = data;
        entry->next = table->table[idx];
        table->table[idx] = entry;
    }
}
#endif
#pragma pop

/* 0x8019C358 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_IDSetup(void) {
#include "src/hsd/hsd_fog_HSD_IDSetup.inc"
}
#else
void HSD_IDSetup(void) {
    memset(&default_table, 0, sizeof(HSD_IDTable));
}
#endif
#pragma pop

/* 0x8019C388 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void HSD_ObjAllocInit(HSD_ObjAllocData* list, u32 size, u32 alignment);
#if 0
asm void HSD_IDInitAllocData(void) {
#include "src/hsd/hsd_fog_HSD_IDInitAllocData.inc"
}
#else
void HSD_IDInitAllocData(void) {
    HSD_ObjAllocInit(&lbl_8046553C, sizeof(IDEntry), 4);
}
#endif
#pragma pop

/* 0x8019C3B8 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_IDGetAllocData(void) {
#include "src/hsd/hsd_fog_HSD_IDGetAllocData.inc"
}
#else
HSD_ObjAllocData* HSD_IDGetAllocData(void) {
    return &lbl_8046553C;
}
#endif
#pragma pop
