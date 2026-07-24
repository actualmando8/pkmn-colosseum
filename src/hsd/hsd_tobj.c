/**
 * @file hsd_tobj.c
 * @brief HSD TObj - Texture object implementation.
 *
 * Colosseum address range: 0x801BBAC8 - 0x801BF098
 * Adapted from doldecomp/melee src/sysdolphin/baselib/tobj.c
 *
 * Colosseum emits this translation unit in the reverse of Melee's source
 * order (TObjInfoInit first, the animation entry points last), so the
 * functions below are laid out to match the binary, not tobj.c.
 *
 * The class info, the two file statics, the assert strings and the float
 * constants all live in data sections that are not yet decompiled, so they
 * are reached through their `lbl_` symbol names; the `#define`s below give
 * them their sysdolphin names.
 */

#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_tobj.h"

/* ------------------------------------------------------------------ */
/*  Externals that do not have names in the symbol map yet             */
/* ------------------------------------------------------------------ */

extern void* fn_80193828(HSD_ClassInfo* info); /* hsdNew              */
extern HSD_ClassInfo* fn_80193748(char* name); /* hsdSearchClassInfo  */
extern void* fn_80193B10(s32 size);            /* hsdAllocMemPiece    */
extern void fn_80193AF0(void* mem, s32 size);  /* hsdFreeMemPiece     */
extern void* fn_801A6928(s32 size);            /* HSD_MemAlloc        */
extern void fn_801A6960(void* mem);            /* HSD_Free            */
extern void fn_8019C6EC(u32 mask);             /* HSD_StateInvalidate */

extern void* memset(void* dst, int c, u32 n);
extern void* memcpy(void* dst, const void* src, u32 n);
extern void OSReport(const char* fmt, ...);
extern void HSD_Panic(const char* file, u32 line, const char* msg);

extern HSD_AObj* HSD_AObjLoadDesc(HSD_AObjDesc* desc);
extern void HSD_AObjRemove(HSD_AObj* aobj);
extern void HSD_AObjReqAnim(HSD_AObj* aobj, f32 startframe);
extern void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj,
                                  void (*update)(void*, u32, HSD_ObjData*));

/* ------------------------------------------------------------------ */
/*  Class info / data symbols                                          */
/* ------------------------------------------------------------------ */

extern u8 lbl_8036D3F0[]; /* hsdTObj class info */

extern char lbl_80275638[]; /* "sysdolphin_base_library" */
extern char lbl_80275650[]; /* "hsd_tobj" */
extern char lbl_8027565C[]; /* "texmtx index exceed hardware limit (%d).\n" */

/* .sdata2 strings */
extern const char lbl_8047DEB0[7]; /* __FILE__ */
extern const char lbl_8047DEB8[8]; /* "idesc" */
extern const char lbl_8047DEC0[4]; /* "tev" */
extern const char lbl_8047DEC4[8]; /* "tlut" */
extern const char lbl_8047DECC[4]; /* "new" */
extern const char lbl_8047DED0[4]; /* "0" */
extern const char lbl_8047DED4[4]; /* "" */
extern const char lbl_8047DF10[8]; /* "tobj" */

#define HSD_TOBJ_FILE lbl_8047DEB0

#define hsdTObjInfo HSD_TOBJ_INFO(lbl_8036D3F0)
#define hsdTObjClass HSD_CLASS_INFO(lbl_8036D3F0)

/* ------------------------------------------------------------------ */
/*  File statics.  Named for their addresses so that the relocations   */
/*  land on the original .sbss symbols.                                */
/* ------------------------------------------------------------------ */

static HSD_ClassInfo* lbl_8047B378 = NULL; /* default_class */
static HSD_TObj* lbl_8047B37C = NULL;      /* tobj_head */

#define default_class lbl_8047B378
#define tobj_head lbl_8047B37C

/* ------------------------------------------------------------------ */
/*  sysdolphin's small deallocators.  They are always inlined; keeping  */
/*  them as functions is what makes the caller load the pointer once.   */
/* ------------------------------------------------------------------ */

static inline void HSD_TlutFree(HSD_Tlut* tlut)
{
    fn_80193AF0(tlut, sizeof(HSD_Tlut));
}

static inline void HSD_TlutRemove(HSD_Tlut* tlut)
{
    if (tlut != NULL) {
        HSD_TlutFree(tlut);
    }
}

static inline void HSD_TObjTevFree(HSD_TObjTev* tev)
{
    fn_80193AF0(tev, sizeof(HSD_TObjTev));
}

static inline void HSD_TObjTevRemove(HSD_TObjTev* tev)
{
    if (tev != NULL) {
        HSD_TObjTevFree(tev);
    }
}

/* ------------------------------------------------------------------ */

static void TObjInfoInit(void);
static void TObjAmnesia(HSD_ClassInfo* info);
static void TObjRelease(HSD_Class* o);
static int TObjLoad(HSD_TObj* tobj, HSD_TObjDesc* td);
static void MakeTextureMtx(HSD_TObj* tobj);
static void TObjMakeTExp(HSD_TObj* tobj, u32 lightmap, u32 lightmap_done,
                         HSD_TExp** c, HSD_TExp** a, HSD_TExp** list);
static void TObjSetupMtx(HSD_TObj* tobj);

/** TObjInit - the class `init` method (0x801BBCE0). */
int fn_801BBCE0(HSD_TObj* tobj);
/** HSD_TObjInsert - splice `next` in behind `tobj` (0x801BBE3C). */
void fn_801BBE3C(HSD_TObj* tobj, HSD_TObj* next);
/** TObjUpdateFunc - the class `update` method (0x801BE85C). */
void fn_801BE85C(void* obj, u32 type, HSD_ObjData* val);

/* ========================================================================= */
/*  0x801BBAC8 | 0xEC  TObjInfoInit                                          */
/* ========================================================================= */
#pragma push
#pragma optimization_level 0
static void TObjInfoInit(void)
{
    hsdInitClassInfo(hsdTObjClass, HSD_CLASS_INFO(&hsdObj),
                     lbl_80275638, lbl_80275650,
                     sizeof(HSD_TObjInfo), sizeof(HSD_TObj));

    hsdTObjClass->init = (int (*)(HSD_Class*)) fn_801BBCE0;
    hsdTObjClass->release = TObjRelease;
    hsdTObjClass->amnesia = TObjAmnesia;
    hsdTObjInfo->load = TObjLoad;
    hsdTObjInfo->make_texp = TObjMakeTExp;
    hsdTObjInfo->make_mtx = MakeTextureMtx;
    hsdTObjInfo->update = fn_801BE85C;
}
#pragma pop

/* ========================================================================= */
/*  0x801BBBB4 | 0x60  TObjAmnesia                                           */
/* ========================================================================= */
#pragma push
#pragma optimization_level 4
static void TObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    if (info == hsdTObjClass) {
        tobj_head = NULL;
    }
    hsdTObjClass->head.parent->amnesia(info);
}
#pragma pop

/* ========================================================================= */
/*  0x801BBC14 | 0xCC  TObjRelease                                           */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
static void TObjRelease(HSD_Class* o)
{
    HSD_TObj* tobj = HSD_TOBJ(o);

    HSD_AObjRemove(tobj->aobj);
    HSD_TlutRemove(tobj->tlut);
    HSD_TObjTevRemove(tobj->tev);

    if (tobj->tluttbl != NULL) {
        int i;
        for (i = 0; tobj->tluttbl[i] != NULL; i++) {
            HSD_TlutRemove(tobj->tluttbl[i]);
        }
        fn_801A6960(tobj->tluttbl);
    }

    hsdTObjClass->head.parent->release(o);
}
#pragma pop

/* ========================================================================= */
/*  0x801BBCE0 | 0x5C  fn_801BBCE0 -- TObjInit (class `init` method)         */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
int fn_801BBCE0(HSD_TObj* tobj)
{
    int result = hsdTObjClass->head.parent->init((HSD_Class*) tobj);
    if (result >= 0) {
        tobj->anim_id = TOBJ_ID_NULL;
        result = 0;
    }
    return result;
}
#pragma pop

/* ========================================================================= */
/*  0x801BBD3C | 0x24  HSD_ImageDescFree                                     */
/* ========================================================================= */
#pragma push
#pragma optimization_level 4
void HSD_ImageDescFree(HSD_ImageDesc* idesc)
{
    fn_80193AF0(idesc, sizeof(HSD_ImageDesc));
}
#pragma pop

/* ========================================================================= */
/*  0x801BBD60 | 0x24  HSD_ImageDescRemove                                   */
/* ========================================================================= */
#pragma push
#pragma optimization_level 4
void HSD_ImageDescRemove(HSD_ImageDesc* idesc)
{
    fn_80193AF0(idesc, sizeof(HSD_ImageDesc));
}
#pragma pop

/* ========================================================================= */
/*  0x801BBD84 | 0x58  HSD_ImageDescAlloc                                    */
/* ========================================================================= */
#pragma push
#pragma optimization_level 4
HSD_ImageDesc* HSD_ImageDescAlloc(void)
{
    HSD_ImageDesc* idesc = fn_80193B10(sizeof(HSD_ImageDesc));
    if (idesc == NULL) {
        __assert(HSD_TOBJ_FILE, 0x8F7, lbl_8047DEB8);
    }
    memset(idesc, 0, sizeof(HSD_ImageDesc));
    return idesc;
}
#pragma pop

/* ========================================================================= */
/*  0x801BBDDC | 0x60  HSD_TObjAlloc                                         */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
HSD_TObj* HSD_TObjAlloc(void)
{
    HSD_TObj* tobj =
        fn_80193828(default_class != NULL ? default_class : hsdTObjClass);
    if (tobj == NULL) {
        __assert(HSD_TOBJ_FILE, 0x884, lbl_8047DECC);
    }
    return tobj;
}
#pragma pop

/* ========================================================================= */
/*  0x801BBE3C | 0x24  fn_801BBE3C -- HSD_TObjInsert                         */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
void fn_801BBE3C(HSD_TObj* tobj, HSD_TObj* next)
{
    if (tobj == NULL || next == NULL) {
        return;
    }
    next->next = tobj->next;
    tobj->next = next;
}
#pragma pop

/* ========================================================================= */
/*  0x801BBE60 | 0x74  HSD_TObjRemoveAll                                     */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
void HSD_TObjRemoveAll(HSD_TObj* tobj)
{
    while (tobj != NULL) {
        HSD_TObj* next = tobj->next;
        hsdDelete(tobj);
        tobj = next;
    }
}
#pragma pop

/* ========================================================================= */
/*  0x801BBED4 | 0x54  HSD_TObjRemove                                        */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
void HSD_TObjRemove(HSD_TObj* tobj)
{
    hsdDelete(tobj);
}
#pragma pop

/* ========================================================================= */
/*  0x801BBF28 | 0xBC  HSD_Index2TexMtx                                      */
/* ========================================================================= */
#pragma push
#pragma optimization_level 1
u32 HSD_Index2TexMtx(u32 index)
{
    switch (index) {
    case 0:
        return GX_TEXMTX0;
    case 1:
        return GX_TEXMTX0 + 3;
    case 2:
        return GX_TEXMTX0 + 6;
    case 3:
        return GX_TEXMTX0 + 9;
    case 4:
        return GX_TEXMTX0 + 12;
    case 5:
        return GX_TEXMTX0 + 15;
    case 6:
        return GX_TEXMTX0 + 18;
    case 7:
        return GX_TEXMTX0 + 21;
    case 8:
        return GX_TEXMTX0 + 24;
    case 9:
        return GX_TEXMTX9;
    case 10:
        return GX_IDENTITY;
    default:
        OSReport(lbl_8027565C, index);
        HSD_Panic(HSD_TOBJ_FILE, 0x7E1, lbl_8047DED4);
    }
    return GX_IDENTITY;
}
#pragma pop

/* ========================================================================= */
/*  0x801BE490 | 0x3C  _HSD_TObjGetCurrentByType                             */
/* ========================================================================= */
#pragma push
#pragma optimization_level 4
HSD_TObj* _HSD_TObjGetCurrentByType(HSD_TObj* from, u32 mapping)
{
    HSD_TObj* tp;

    if (from == NULL) {
        tp = tobj_head;
    } else {
        tp = from->next;
    }

    for (; tp != NULL; tp = tp->next) {
        if (tobj_coord(tp) == mapping) {
            goto END;
        }
    }

    tp = NULL;
END:
    return tp;
}
#pragma pop

/* --- Non-linked CodeCandidate placeholders. These remain intentionally
       incomplete until their target bodies are decompiled. --- */

static int TObjLoad(HSD_TObj* tobj, HSD_TObjDesc* td)
{
    (void) tobj;
    (void) td;
    return 0;
}
static void MakeTextureMtx(HSD_TObj* tobj) { (void) tobj; }
static void TObjSetupMtx(HSD_TObj* tobj) { (void) tobj; }
static void TObjMakeTExp(HSD_TObj* tobj, u32 lightmap, u32 lightmap_done,
                         HSD_TExp** c, HSD_TExp** a, HSD_TExp** list)
{
    (void) tobj;
    (void) lightmap;
    (void) lightmap_done;
    (void) c;
    (void) a;
    (void) list;
}
void fn_801BE85C(void* obj, u32 type, HSD_ObjData* val)
{
    (void) obj;
    (void) type;
    (void) val;
}

/* ========================================================================= */
/*  0x801BE4E4 | 0xCC  HSD_TObjLoadDesc                                      */
/* ========================================================================= */
HSD_TObj* HSD_TObjLoadDesc(HSD_TObjDesc* desc)
{
    HSD_TObj* tobj;
    HSD_ClassInfo* info;

    if (desc == NULL) {
        goto return_null;
    }

    if (desc->class_name != NULL) {
        info = fn_80193748(desc->class_name);
        if (info != NULL) {
            goto do_alloc_from_info;
        }
    }

    if (default_class != NULL) {
        info = default_class;
    } else {
        info = hsdTObjClass;
    }
    tobj = fn_80193828(info);
    if (tobj == NULL) {
        __assert(HSD_TOBJ_FILE, 0x884, lbl_8047DECC);
    }
    goto setup;

do_alloc_from_info:
    tobj = fn_80193828(info);
    if (tobj == NULL) {
        __assert(HSD_TOBJ_FILE, 0x1ed, lbl_8047DF10);
    }
setup:
    HSD_TOBJ_METHOD(tobj)->load(tobj, desc);
    return tobj;
return_null:
    return NULL;
}

/* ========================================================================= */
/*  0x801BE800 | 0x5C  HSD_TObjAnimAll                                       */
/* ========================================================================= */
void fn_801BE800(HSD_TObj* tobj)
{
    HSD_TObj* cur;

    if (tobj == NULL) {
        return;
    }

    cur = tobj;
    while (cur != NULL) {
        if (cur != NULL) {
            HSD_AObjInterpretAnim(cur->aobj, cur, HSD_TOBJ_METHOD(cur)->update);
        }
        cur = cur->next;
    }
}

/* ========================================================================= */
/*  0x801BEE68 | 0x74  HSD_TObjReqAnimAllByFlags                             */
/* ========================================================================= */
void fn_801BEE68(HSD_TObj* tobj, f32 val, u32 flags)
{
    HSD_TObj* cur;

    if (tobj == NULL) {
        return;
    }

    cur = tobj;
    while (cur != NULL) {
        if (cur != NULL) {
            if (flags & 0x10) {
                HSD_AObjReqAnim(cur->aobj, val);
            }
        }
        cur = cur->next;
    }
}
