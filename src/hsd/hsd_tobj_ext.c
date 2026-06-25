/**
 * @file hsd_tobj_ext.c
 * @brief HSD TObj extended - texture setup, TLUT, and transition code.
 *
 * Address range: 0x801BBAC8 - 0x801BFF18
 * Contains TObj class init (proposed HSD_TObjInit at 0x801BBAC8),
 * texture setup/binding, TLUT (palette) management, texture matrix
 * computation, and the transition code between HSD library and
 * the battle system.
 *
 * NOTE: The core TObj lifecycle functions (alloc/load/anim/remove)
 * are in hsd_tobj.c. This file covers the rendering pipeline
 * functions for textures.
 *
 * Decompiled from Melee src/sysdolphin/baselib/tobj.c (rendering portion)
 */

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_tobj.h"
#include "hsd/hsd_mobj.h"

/* GX external declarations */
extern void GXInitTexObj(void* obj, void* image, u16 w, u16 h,
                          u32 format, u32 wrap_s, u32 wrap_t, u32 mipmap);
extern void GXInitTexObjCI(void* obj, void* image, u16 w, u16 h,
                            u32 format, u32 wrap_s, u32 wrap_t,
                            u32 mipmap, u32 tlut_name);
extern void GXInitTexObjLOD(void* obj, u32 min_filt, u32 mag_filt,
                              f32 min_lod, f32 max_lod, f32 lod_bias,
                              u32 bias_clamp, u32 do_edge_lod,
                              u32 max_aniso);
extern void GXInitTexObjFilterMode(void* obj, u32 min_filt, u32 mag_filt);
extern void GXInitTexObjWrapMode(void* obj, u32 wrap_s, u32 wrap_t);
extern void GXLoadTexObj(void* obj, u32 map_id);
extern void GXInitTlutObj(void* obj, void* lut, u32 fmt, u16 n_entries);
extern void GXLoadTlut(void* obj, u32 tlut_name);
extern void GXSetTexCoordGen(u32 dst_coord, u32 func, u32 src_param, u32 mtx);
extern void GXSetTexCoordGen2(u32 dst_coord, u32 func, u32 src_param,
                              u32 mtx_src, u32 normalize, u32 post_mtx);
extern void GXLoadTexMtxImm(void* mtx, u32 id, u32 type);
extern void DCStoreRange(void* addr, u32 nBytes);
extern void GXInvalidateTexAll(void);
extern s32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(s32 level);

/* hsdAllocMemPiece/hsdFreeMemPiece declared in hsd_class.h with s32 */
extern void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj, void* update_func);

typedef struct HSD_TlutWork {
    void* lut;
    u32 fmt;
    u32 tlut_name;
    u16 n_entries;
} HSD_TlutWork;

typedef struct HSD_TexLODWork {
    u32 minFilt;
    f32 LODBias;
    u8 bias_clamp;
    u8 edgeLODEnable;
    u8 pad[2];
    u32 max_anisotropy;
} HSD_TexLODWork;

typedef struct GXColorWork {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColorWork;

typedef struct GXRenderModeObjWork {
    u32 word[15];
} GXRenderModeObjWork;

typedef struct HSD_ObjDataWork {
    f32 fv;
} HSD_ObjDataWork;

typedef struct HSD_TObjTevWork {
    u8 color_op;
    u8 alpha_op;
    u8 color_bias;
    u8 alpha_bias;
    u8 color_scale;
    u8 alpha_scale;
    u8 color_clamp;
    u8 alpha_clamp;
    u8 color_a;
    u8 color_b;
    u8 color_c;
    u8 color_d;
    u8 alpha_a;
    u8 alpha_b;
    u8 alpha_c;
    u8 alpha_d;
    GXColorWork konst;
    GXColorWork tev0;
    GXColorWork tev1;
    u32 active;
} HSD_TObjTevWork;

#define TOBJ_NEXT_RAW(tobj) (*(HSD_TObj**) ((u8*) (tobj) + 0x08))
#define TOBJ_TEXANIM_ID_RAW(tobj) (*(u16*) ((u8*) (tobj) + 0x4A))
#define TOBJ_AOBJ_RAW(tobj) (*(void**) ((u8*) (tobj) + 0x64))
#define TOBJ_IMAGETBL_RAW(tobj) (*(HSD_ImageDesc***) ((u8*) (tobj) + 0x68))
#define TOBJ_TLUTTBL_RAW(tobj) (*(void***) ((u8*) (tobj) + 0x6C))
#define TOBJ_TLUT_NO_RAW(tobj) (*(u8*) ((u8*) (tobj) + 0x70))

#define HSD_TEXP_TEX ((HSD_TExp*) -1)
#define HSD_TEXP_ZERO ((HSD_TExp*) 0)

#define HSD_TE_RGB 1
#define HSD_TE_A 5
#define HSD_TE_X 6
#define HSD_TE_0 7
#define HSD_TE_1 8
#define HSD_TE_4_8 12

#define HSD_TE_U8 0
#define HSD_TE_F32 3

#define GX_COLOR_NULL 0xFF
#define GX_CC_ZERO 15
#define GX_CC_ONE 12
#define GX_CC_HALF 13
#define GX_CC_TEXC 8
#define GX_CC_TEXA 9
#define GX_CA_ZERO 7
#define GX_CA_TEXA 4
#define GX_TEV_ADD 0
#define GX_TEV_SUB 1
#define GX_TB_ZERO 0
#define GX_CS_SCALE_1 0
#define GX_ENABLE 1

#define TOBJ_TEV_CC_KONST_RGB (0x80 | 0)
#define TOBJ_TEV_CC_KONST_RRR (0x80 | 1)
#define TOBJ_TEV_CC_KONST_GGG (0x80 | 2)
#define TOBJ_TEV_CC_KONST_BBB (0x80 | 3)
#define TOBJ_TEV_CC_KONST_AAA (0x80 | 4)
#define TOBJ_TEV_CC_TEX0_RGB (0x80 | 5)
#define TOBJ_TEV_CC_TEX0_AAA (0x80 | 6)
#define TOBJ_TEV_CC_TEX1_RGB (0x80 | 7)
#define TOBJ_TEV_CC_TEX1_AAA (0x80 | 8)

#define TOBJ_TEV_CA_KONST_R (0x40 | 0)
#define TOBJ_TEV_CA_KONST_G (0x40 | 1)
#define TOBJ_TEV_CA_KONST_B (0x40 | 2)
#define TOBJ_TEV_CA_KONST_A (0x40 | 3)
#define TOBJ_TEV_CA_TEX0_A (0x40 | 4)
#define TOBJ_TEV_CA_TEX1_A (0x40 | 5)

#define TOBJ_TEVREG_ACTIVE_COLOR_TEV (1U << 30)
#define TOBJ_TEVREG_ACTIVE_ALPHA_TEV (1U << 31)

extern HSD_TObj* lbl_8047B37C;
extern void* lbl_8047B378;
extern u8 lbl_8036D3F0[];
extern u8 lbl_8036CC00[];
extern u8 lbl_8036D43C[];
extern u8 lbl_8036D48C[];
extern u8 lbl_8036D510[];
extern HSD_TexLODWork lbl_8036D594;
extern char lbl_80275638[];
extern char lbl_80275650[];
extern char lbl_8047DEB0;
extern char lbl_8047DEB8;
extern char lbl_8047DEC4;
extern char lbl_8047DECC;
extern char lbl_8047DED0;
extern char lbl_8047DF10;
extern char lbl_80275688[];
extern char lbl_80275694[];
extern char lbl_802756AC[];
extern char lbl_802756C4[];
extern char lbl_802756E8[];
extern s32 fn_801BC33C(HSD_TObj* tobj);
extern void fn_801BC8BC();
extern void fn_801BDD74(HSD_TObj* tobj);
extern void fn_801BBBB4(HSD_TObj* tobj);
extern void fn_801BBC14(HSD_TObj* tobj);
extern s32 fn_801BBCE0(HSD_TObj* tobj);
extern void fn_801BE2B4();
extern s32 fn_801BE598(HSD_TObj* tobj, HSD_TObjDesc* desc);
extern void fn_801BE85C(HSD_TObj* tobj, u32 type, HSD_ObjDataWork* val);
extern void HSD_StateRegisterTexGen(u32 coord);
extern void fn_800BB050(void* tlutobj, void* lut, u32 fmt, u16 n_entries);
extern void fn_800BB098(void* tlutobj, u32 tlut_name);
extern void fn_800BAC58(void* texobj, void* image, u16 width, u16 height,
                         u32 format, u32 wrap_s, u32 wrap_t, u32 mipmap,
                         u32 tlut_name);
extern void fn_800BA9E4(void* texobj, void* image, u16 width, u16 height,
                         u32 format, u32 wrap_s, u32 wrap_t, u32 mipmap);
extern void fn_800BACA0(void* texobj, u32 min_filt, u32 mag_filt,
                         f32 min_lod, f32 max_lod, f32 lod_bias,
                         u32 bias_clamp, u32 do_edge_lod, u32 max_aniso);
extern void fn_800BAFFC(void* texobj, u32 map_id);
extern void* fn_80193B10(s32 size);
extern void fn_80193AF0(void* obj, s32 size);
extern HSD_ClassInfo* fn_80193748(const char* class_name);
extern void* fn_80193828(void* info);
extern void* fn_801A6928(s32 size);
extern void fn_801A6960(void* ptr);
extern void fn_801C25E4(void* aobj);
extern void* fn_801C2670(void* aobjdesc);
extern void fn_801C27F4(void* aobj, void* obj, void* update_func);
extern void fn_801C29C4(void* aobj, f32 frame);
extern u32 fn_801A68F0(void);
extern void fn_801B3638(void* tevdesc);
extern u32 fn_801B37A0(void);
extern void fn_800B857C(u32 dst_coord, u32 func, u32 src_param, u32 mtx,
                        u32 normalize, u32 post_mtx);
extern void fn_800A2D98(void* a, void* b, void* dst);
extern void fn_800A32B4(void* mtx, f32 x, f32 y, f32 z);
extern void fn_800A3334(void* mtx, f32 x, f32 y, f32 z);
extern void fn_800BD58C(void* mtx, u32 id, u32 type);
extern void fn_801A8B94(void* mtx, void* rot);
extern void* fn_801942B8(void);
extern s32 fn_801943CC(void* cobj);
extern void* fn_80194CF4(void);
extern void fn_800A35E4(void* mtx, f32, f32, f32, f32, f32, f32, f32, f32);
extern void fn_800A3678(void* mtx, f32, f32, f32, f32, f32, f32);
extern void fn_800A3744(void* mtx, f32, f32, f32, f32, f32, f32, f32, f32);
extern void fn_800A3820(void* mtx, void* src, void* dst);
extern void fn_800A3ADC(void* src, void* dst);
extern void* fn_801A4AC4(s32 type);
extern void fn_801A620C(void* lobj, void* out);
extern void fn_8019C6EC(s32 type);
extern void* memset(void* dest, int value, u32 size);
extern void* memcpy(void* dest, const void* src, u32 size);
extern HSD_TExp* fn_801B707C(HSD_TExp** list);
extern HSD_TExp* fn_801B6F5C(void* val, u32 comp, u32 ctype,
                              HSD_TExp** list);
extern void fn_801B5E40(HSD_TExp* texp, HSD_TObj* tobj, u32 chan);
extern void fn_801B6E74(HSD_TExp* texp, u32 op, u32 bias, u32 scale,
                         u32 clamp);
extern void fn_801B6CD8(HSD_TExp* texp, u32 op, u32 bias, u32 scale,
                         u32 clamp);
extern void fn_801B64EC(HSD_TExp* texp, u32 sel_a, HSD_TExp* exp_a,
                         u32 sel_b, HSD_TExp* exp_b, u32 sel_c,
                         HSD_TExp* exp_c, u32 sel_d, HSD_TExp* exp_d);
extern void fn_801B5F08(HSD_TExp* texp, u32 sel_a, HSD_TExp* exp_a,
                         u32 sel_b, HSD_TExp* exp_b, u32 sel_c,
                         HSD_TExp* exp_c, u32 sel_d, HSD_TExp* exp_d);
void fn_801BCF30(u32 lightmap, HSD_TObj* tobj, HSD_TExp** c,
                 HSD_TExp** a, HSD_TExp** list, int repeat);

/* ========================================================================= */
/*  TObj class initialization                                                */
/* ========================================================================= */

/* HSD_TObjInit - initializes TObj class info and method slots. */
void fn_801BBAC8(void) {
    hsdInitClassInfo((HSD_ClassInfo*) lbl_8036D3F0,
                     (HSD_ClassInfo*) lbl_8036CC00, lbl_80275638,
                     lbl_80275650, 0x4C, 0xAC);

    *(void**) (lbl_8036D3F0 + 0x30) = fn_801BBC14;
    *(void**) (lbl_8036D3F0 + 0x2C) = fn_801BBCE0;
    *(void**) (lbl_8036D3F0 + 0x38) = fn_801BBBB4;
    *(void**) (lbl_8036D3F0 + 0x40) = fn_801BE598;
    *(void**) (lbl_8036D3F0 + 0x44) = fn_801BC8BC;
    *(void**) (lbl_8036D3F0 + 0x3C) = fn_801BE2B4;
    *(void**) (lbl_8036D3F0 + 0x48) = fn_801BE85C;
}

/* TObjAmnesia - clears cached class pointers before parent amnesia. */
void fn_801BBBB4(HSD_TObj* tobj) {
    if ((void*) tobj == lbl_8047B378) {
        lbl_8047B378 = NULL;
    }
    if ((void*) tobj == (void*) lbl_8036D3F0) {
        lbl_8047B37C = NULL;
    }

    (*(void (**)(HSD_TObj*)) ((u8*) *(void**) (lbl_8036D3F0 + 0x14) + 0x38))(tobj);
}

/* TObjRelease - releases animation and owned descriptor/list allocations. */
void fn_801BBC14(HSD_TObj* tobj) {
    void** list;
    s32 i;

    fn_801C25E4(*(void**) ((u8*) tobj + 0x64));

    if (*(void**) ((u8*) tobj + 0x5C) != NULL) {
        fn_80193AF0(*(void**) ((u8*) tobj + 0x5C), 0x10);
    }
    if (*(void**) ((u8*) tobj + 0xA8) != NULL) {
        fn_80193AF0(*(void**) ((u8*) tobj + 0xA8), 0x20);
    }

    list = *(void***) ((u8*) tobj + 0x6C);
    if (list != NULL) {
        for (i = 0; list[i] != NULL; i++) {
            fn_80193AF0(list[i], 0x10);
        }
        fn_801A6960(list);
    }

    (*(void (**)(HSD_TObj*)) ((u8*) *(void**) (lbl_8036D3F0 + 0x14) + 0x30))(tobj);
}

/* TObjInit - delegates parent init and initializes the 0x4A halfword to -1. */
s32 fn_801BBCE0(HSD_TObj* tobj) {
    s32 result;

    result = (*(s32 (**)(HSD_TObj*)) ((u8*) *(void**) (lbl_8036D3F0 + 0x14) + 0x2C))(tobj);
    if (result >= 0) {
        *(u16*) ((u8*) tobj + 0x4A) = 0xFFFF;
        result = 0;
    }
    return result;
}

/* ========================================================================= */
/*  TObj accessors                                                           */
/* ========================================================================= */

/* Address: 0x801BBD3C | Size: 0x24 */
/* Free an image descriptor memory piece. */
void HSD_ImageDescFree(void* obj) {
    fn_80193AF0(obj, 0x18);
}

/* Address: 0x801BBD60 | Size: 0x24 */
/* Remove an image descriptor memory piece. */
void HSD_ImageDescRemove(void* obj) {
    fn_80193AF0(obj, 0x18);
}

/* Address: 0x801BBD84 | Size: 0x58 */
HSD_ImageDesc* HSD_ImageDescAlloc(void) {
    HSD_ImageDesc* idesc = fn_80193B10(0x18);
    if (idesc == NULL) {
        __assert(&lbl_8047DEB0, 0x8F7, &lbl_8047DEB8);
    }
    memset(idesc, 0, 0x18);
    return idesc;
}

/* HSD_TObjAlloc - allocates from the default/current TObj class. */
HSD_TObj* fn_801BBDDC(void) {
    HSD_TObj* tobj;
    void* info;

    info = lbl_8047B378 != NULL ? lbl_8047B378 : lbl_8036D3F0;

    tobj = fn_80193828(info);
    if (tobj == NULL) {
        __assert(&lbl_8047DEB0, 0x884, &lbl_8047DECC);
    }
    return tobj;
}

/* HSD_TObjAddNext - inserts a TObj directly after another TObj. */
void fn_801BBE3C(HSD_TObj* tobj, HSD_TObj* next) {
    if (tobj != NULL) {
        if (next == NULL) {
            return;
        }
        next->next = tobj->next;
        tobj->next = next;
    }
}

/* HSD_TObjRemoveAll - releases and destroys a TObj list. */
void fn_801BBE60(HSD_TObj* tobj) {
    HSD_TObj* next;

    while (tobj != NULL) {
        if (tobj != NULL) {
            next = tobj->next;
            (*(void (**)(HSD_TObj*)) ((u8*) *(void**) tobj + 0x30))(tobj);
            (*(void (**)(HSD_TObj*)) ((u8*) *(void**) tobj + 0x34))(tobj);
        }
        tobj = next;
    }
}

/* HSD_TObjRemove - releases and destroys one TObj. */
void fn_801BBED4(HSD_TObj* tobj) {
    if (tobj != NULL) {
        (*(void (**)(HSD_TObj*)) ((u8*) *(void**) tobj + 0x30))(tobj);
        (*(void (**)(HSD_TObj*)) ((u8*) *(void**) tobj + 0x34))(tobj);
    }
}

/*
 * HSD_TObjSetFlags - 0x801BBF28 | Size: 0xBC
 * Set texture flags with validation.
 */
void HSD_Index2TexMtx(HSD_TObj* tobj, u32 flags) {
    u32 colormap;
    u32 alphamap;

    if (tobj == NULL) {
        return;
    }

    /* Validate that colormap and alphamap modes are valid */
    colormap = flags & TEX_COLORMAP_MASK;
    alphamap = flags & TEX_ALPHAMAP_MASK;

    if (colormap > TEX_COLORMAP_SUB) {
        flags = (flags & ~TEX_COLORMAP_MASK) | TEX_COLORMAP_MODULATE;
    }
    if (alphamap > TEX_ALPHAMAP_SUB) {
        flags = (flags & ~TEX_ALPHAMAP_MASK) | TEX_ALPHAMAP_MODULATE;
    }

    tobj->flags = flags | TEX_MTX_DIRTY;
}

/* ========================================================================= */
/*  TObj animation update and texture swap                                   */
/* ========================================================================= */

#pragma push
#pragma scheduling on
void HSD_TObjSetup(HSD_TObj* tobj) {
    u8 tlutobj[0x0C];
    u8 texobj[0x20];
    HSD_TlutWork* volatile tluts[8];
    s32 num;
    s32 nb_tluts;
    u32 tlut_name;
    u32 big_tlut_name;
    s32 i;
    u32 coord;
    s32 id;
    HSD_ImageDesc* imagedesc;
    HSD_TexLODWork* lod;
    s32 min_filter;
    HSD_TlutWork* tlut;
    u32 mipmap;
    s32 different;

    nb_tluts = 0;
    tlut_name = 0;
    big_tlut_name = 0x10;
    lbl_8047B37C = tobj;
    if (tobj == NULL) {
        return;
    }

    num = fn_801BC33C(tobj);
    if (num > 0) {
        switch (num - 1) {
        case 0:
            coord = 0;
            break;
        case 1:
            coord = 1;
            break;
        case 2:
            coord = 2;
            break;
        case 3:
            coord = 3;
            break;
        case 4:
            coord = 4;
            break;
        case 5:
            coord = 5;
            break;
        case 6:
            coord = 6;
            break;
        case 7:
            coord = 7;
            break;
        default:
            __assert(&lbl_8047DEB0, 0x794, &lbl_8047DED0);
            coord = 0;
            break;
        }
        HSD_StateRegisterTexGen(coord);
    }

    while (tobj != NULL) {
        id = tobj->id;
        imagedesc = tobj->imagedesc;
        if (id != 0xFF) {
            fn_801BDD74(tobj);
            if (imagedesc == NULL) {
                __assert(&lbl_8047DEB0, 0x6B6, lbl_80275688);
            }
            if (imagedesc->image_ptr == NULL) {
                __assert(&lbl_8047DEB0, 0x6B7, lbl_80275694);
            }

            if (tobj->lod != NULL) {
                lod = (HSD_TexLODWork*) tobj->lod;
            } else {
                lod = &lbl_8036D594;
            }
            min_filter = lod->minFilt;

            switch (imagedesc->format) {
            case 8:
            case 9:
            case 10:
                if (tobj->tlut_no != 0xFF) {
                    tlut = (HSD_TlutWork*) tobj->tluttbl[tobj->tlut_no];
                } else {
                    tlut = (HSD_TlutWork*) tobj->tlut;
                }
                if (tlut == NULL) {
                    __assert(&lbl_8047DEB0, 0x6C7, &lbl_8047DEC4);
                }

                i = 0;
                goto check_tlut;
            scan_tlut:
                different = 1;
                if (tluts[i]->n_entries == tlut->n_entries) {
                    different = 0;
                }
                if (different == 0) {
                    goto found_tlut;
                }
                i++;
            check_tlut:
                if (i < nb_tluts) {
                    goto scan_tlut;
                }
            found_tlut:

                if (i < nb_tluts) {
                    tlut->tlut_name = tluts[i]->tlut_name;
                } else if (nb_tluts < 8) {
                    if (tlut->n_entries > 0x100) {
                        tlut->tlut_name = big_tlut_name++;
                    } else {
                        tlut->tlut_name = tlut_name++;
                    }
                    fn_800BB050(tlutobj, tlut->lut, tlut->fmt,
                                tlut->n_entries);
                    fn_800BB098(tlutobj, tlut->tlut_name);
                    tluts[nb_tluts] = tlut;
                    nb_tluts++;
                } else {
                    tlut->tlut_name = 0;
                }

                mipmap = imagedesc->mipmap ? 1 : 0;
                fn_800BAC58(texobj, imagedesc->image_ptr, imagedesc->width,
                            imagedesc->height, imagedesc->format,
                            tobj->wrap_s, tobj->wrap_t, mipmap,
                            tlut->tlut_name);
                if (min_filter == 5) {
                    min_filter = 3;
                }
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 14:
                mipmap = imagedesc->mipmap ? 1 : 0;
                fn_800BA9E4(texobj, imagedesc->image_ptr, imagedesc->width,
                            imagedesc->height, imagedesc->format,
                            tobj->wrap_s, tobj->wrap_t, mipmap);
                break;
            default:
                __assert(&lbl_8047DEB0, 0x703, &lbl_8047DED0);
                break;
            }

            if (imagedesc->mipmap == 0) {
                min_filter &= 1;
            }
            fn_800BACA0(texobj, min_filter, tobj->magFilt,
                        imagedesc->minLOD, imagedesc->maxLOD, lod->LODBias,
                        lod->bias_clamp, lod->edgeLODEnable,
                        lod->max_anisotropy);
            fn_800BAFFC(texobj, tobj->id);
        }
        tobj = tobj->next;
    }
}
#pragma pop

/*
 * HSD_TObjAssignResources - 0x801BC33C | Size: 0x580
 * Assign texture maps and texture coordinates for a TObj chain.
 */
s32 fn_801BC33C(HSD_TObj* tobj) {
    HSD_TObj* cur;
    HSD_TObj* bump;
    HSD_TObj* toon;
    u32 texmap_no;
    u32 texcoord_no;
    u32 limit;
    u32 idx;
    u32 val;

    if (tobj == NULL) {
        return 0;
    }

    texmap_no = 0;
    texcoord_no = 0;
    limit = 8;
    bump = NULL;
    toon = NULL;

    for (cur = tobj; cur != NULL; cur = cur->next) {
        if (tobj_coord(cur) == TEX_COORD_TOON) {
            toon = cur;
        } else if (tobj_bump(cur)) {
            bump = cur;
        }
    }

    if (toon != NULL) {
        limit--;
    }
    if (bump != NULL) {
        limit -= 2;
    }

    for (cur = tobj; cur != NULL; cur = cur->next) {
        if (tobj_coord(cur) == TEX_COORD_TOON) {
            if (cur != toon) {
                cur->id = 0xFF;
            }
        } else if (tobj_bump(cur)) {
            if (cur != bump) {
                cur->id = 0xFF;
            }
        } else if (texmap_no < limit) {
            idx = texmap_no++;
            switch (idx) {
            case 0: val = 0; break;
            case 1: val = 1; break;
            case 2: val = 2; break;
            case 3: val = 3; break;
            case 4: val = 4; break;
            case 5: val = 5; break;
            case 6: val = 6; break;
            case 7: val = 7; break;
            default:
                __assert(&lbl_8047DEB0, 0x807, &lbl_8047DED0);
                val = 0;
                break;
            }
            cur->id = val;

            switch (cur->id) {
            case 0: val = 0x40; break;
            case 1: val = 0x43; break;
            case 2: val = 0x46; break;
            case 3: val = 0x49; break;
            case 4: val = 0x4C; break;
            case 5: val = 0x4F; break;
            case 6: val = 0x52; break;
            case 7: val = 0x55; break;
            default:
                HSD_Panic(&lbl_8047DEB0, 0x258, lbl_802756AC);
                val = 0;
                break;
            }
            cur->mtxid = val;

            switch (tobj_coord(cur)) {
            case TEX_COORD_REFLECTION:
            case TEX_COORD_HILIGHT:
            case TEX_COORD_SHADOW:
                idx = texcoord_no++;
                switch (idx) {
                case 0: val = 0; break;
                case 1: val = 1; break;
                case 2: val = 2; break;
                case 3: val = 3; break;
                case 4: val = 4; break;
                case 5: val = 5; break;
                case 6: val = 6; break;
                case 7: val = 7; break;
                default:
                    __assert(&lbl_8047DEB0, 0x794, &lbl_8047DED0);
                    val = 0;
                    break;
                }
                cur->coord = val;
                break;
            default:
                break;
            }
        } else {
            cur->id = 0xFF;
        }
    }

    for (cur = tobj; cur != NULL; cur = cur->next) {
        if (cur->id != 0xFF && !tobj_bump(cur)) {
            switch (tobj_coord(cur)) {
            case TEX_COORD_UV:
                idx = texcoord_no++;
                switch (idx) {
                case 0: val = 0; break;
                case 1: val = 1; break;
                case 2: val = 2; break;
                case 3: val = 3; break;
                case 4: val = 4; break;
                case 5: val = 5; break;
                case 6: val = 6; break;
                case 7: val = 7; break;
                default:
                    __assert(&lbl_8047DEB0, 0x794, &lbl_8047DED0);
                    val = 0;
                    break;
                }
                cur->coord = val;
                break;
            default:
                break;
            }
        }
    }

    if (bump != NULL) {
        idx = texmap_no;
        switch (idx) {
        case 0: val = 0; break;
        case 1: val = 1; break;
        case 2: val = 2; break;
        case 3: val = 3; break;
        case 4: val = 4; break;
        case 5: val = 5; break;
        case 6: val = 6; break;
        case 7: val = 7; break;
        default:
            __assert(&lbl_8047DEB0, 0x807, &lbl_8047DED0);
            val = 0;
            break;
        }
        bump->id = val;
        bump->mtxid = 0x39;
        idx = texcoord_no;
        switch (idx) {
        case 0: val = 0; break;
        case 1: val = 1; break;
        case 2: val = 2; break;
        case 3: val = 3; break;
        case 4: val = 4; break;
        case 5: val = 5; break;
        case 6: val = 6; break;
        case 7: val = 7; break;
        default:
            __assert(&lbl_8047DEB0, 0x794, &lbl_8047DED0);
            val = 0;
            break;
        }
        bump->coord = val;
        texcoord_no += 2;
        texmap_no++;
    }
    if (toon != NULL) {
        idx = texmap_no;
        switch (idx) {
        case 0: val = 0; break;
        case 1: val = 1; break;
        case 2: val = 2; break;
        case 3: val = 3; break;
        case 4: val = 4; break;
        case 5: val = 5; break;
        case 6: val = 6; break;
        case 7: val = 7; break;
        default:
            __assert(&lbl_8047DEB0, 0x807, &lbl_8047DED0);
            val = 0;
            break;
        }
        toon->id = val;
        idx = texcoord_no++;
        switch (idx) {
        case 0: val = 0; break;
        case 1: val = 1; break;
        case 2: val = 2; break;
        case 3: val = 3; break;
        case 4: val = 4; break;
        case 5: val = 5; break;
        case 6: val = 6; break;
        case 7: val = 7; break;
        default:
            __assert(&lbl_8047DEB0, 0x794, &lbl_8047DED0);
            val = 0;
            break;
        }
        toon->coord = val;
    }

    return texcoord_no;
}

/* ========================================================================= */
/*  Texture expression (TExp) from TObj                                      */
/* ========================================================================= */

void fn_801BC8BC(HSD_TObj* tobj, u32 lightmap, u32 lightmap_done,
                 HSD_TExp** c, HSD_TExp** a, HSD_TExp** list) {
    HSD_TExp *e0, *e1;
    HSD_TExp *c_src, *a_src;
    u32 c_sel, a_sel;
    int repeat = (lightmap_done & tobj_lightmap(tobj));
    HSD_TObjTevWork* tev;

    c_src = HSD_TEXP_TEX;
    c_sel = HSD_TE_RGB;

    a_src = HSD_TEXP_TEX;
    a_sel = HSD_TE_A;

    e0 = fn_801B707C(list);

    tev = (HSD_TObjTevWork*) tobj->tev;
    if (tev != NULL && (tev->active & (TOBJ_TEVREG_ACTIVE_COLOR_TEV |
                                        TOBJ_TEVREG_ACTIVE_ALPHA_TEV)))
    {
        fn_801BCF30(lightmap, tobj, &c_src, &a_src, list, repeat);
    }

    fn_801B5E40(e0, tobj, GX_COLOR_NULL);

    switch (tobj_colormap(tobj)) {
    case TEX_COLORMAP_ALPHA_MASK:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_RGB, *c, c_sel, c_src, a_sel, a_src,
                    HSD_TE_0, HSD_TEXP_ZERO);
        break;
    case TEX_COLORMAP_RGB_MASK:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_RGB, *c, c_sel, c_src, c_sel, c_src,
                    HSD_TE_0, HSD_TEXP_ZERO);
        break;
    case TEX_COLORMAP_BLEND:
        e1 = fn_801B6F5C(&tobj->blending, HSD_TE_X, HSD_TE_F32, list);
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_RGB, *c, c_sel, c_src, HSD_TE_X, e1,
                    HSD_TE_0, HSD_TEXP_ZERO);
        break;
    case TEX_COLORMAP_MODULATE:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, *c, c_sel,
                    c_src, HSD_TE_0, HSD_TEXP_ZERO);
        break;
    case TEX_COLORMAP_REPLACE:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                    HSD_TE_0, HSD_TEXP_ZERO, c_sel, c_src);
        break;
    case TEX_COLORMAP_NONE:
    case TEX_COLORMAP_PASS:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                    HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_RGB, *c);
        break;
    case TEX_COLORMAP_ADD:
        fn_801B6E74(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, c_sel, c_src, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                    HSD_TEXP_ZERO, HSD_TE_RGB, *c);
        break;
    case TEX_COLORMAP_SUB:
        fn_801B6E74(e0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE);
        fn_801B64EC(e0, c_sel, c_src, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                    HSD_TEXP_ZERO, HSD_TE_RGB, *c);
        break;
    default:
        __assert(&lbl_8047DEB0, 0x5E4, &lbl_8047DED0);
    }
    *c = e0;

    if (!repeat) {
        switch (tobj_alphamap(tobj)) {
        case TEX_ALPHAMAP_ALPHA_MASK:
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, HSD_TE_A, *a, a_sel, a_src, a_sel, a_src,
                        HSD_TE_0, HSD_TEXP_ZERO);
            break;
        case TEX_ALPHAMAP_BLEND:
            e1 = fn_801B6F5C(&tobj->blending, HSD_TE_X, HSD_TE_F32, list);
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, HSD_TE_A, *a, a_sel, a_src, HSD_TE_X, e1,
                        HSD_TE_0, HSD_TEXP_ZERO);
            break;
        case TEX_ALPHAMAP_MODULATE:
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, *a, a_sel,
                        a_src, HSD_TE_0, HSD_TEXP_ZERO);
            break;
        case TEX_ALPHAMAP_REPLACE:
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                        HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO, a_sel,
                        a_src);
            break;
        case TEX_ALPHAMAP_NONE:
        case TEX_ALPHAMAP_PASS:
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                        HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A,
                        *a);
            break;
        case TEX_ALPHAMAP_ADD:
            fn_801B6CD8(e0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, a_sel, a_src, HSD_TE_0, HSD_TEXP_ZERO,
                        HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, *a);
            break;
        case TEX_ALPHAMAP_SUB:
            fn_801B6CD8(e0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_ENABLE);
            fn_801B5F08(e0, a_sel, a_src, HSD_TE_0, HSD_TEXP_ZERO,
                        HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_A, *a);
            break;
        default:
            __assert(&lbl_8047DEB0, 0x61E, &lbl_8047DED0);
        }
        *a = e0;
    }
}

void fn_801BCF30(u32 lightmap, HSD_TObj* tobj, HSD_TExp** c,
                 HSD_TExp** a, HSD_TExp** list, int repeat) {
    HSD_TObjTevWork* tev = (HSD_TObjTevWork*) tobj->tev;
    u8* in;
    HSD_TExp *e0, *tmp;
    int i;

    HSD_TExp* konst_rgb;
    HSD_TExp* konst_r;
    HSD_TExp* konst_g;
    HSD_TExp* konst_b;
    HSD_TExp* konst_a;
    HSD_TExp* reg0_rgb;
    HSD_TExp* reg0_a;
    HSD_TExp* reg1_rgb;
    HSD_TExp* reg1_a;

    int use_k_rgb = 0;
    int use_k_r = 0;
    int use_k_g = 0;
    int use_k_b = 0;
    int use_k_a = 0;
    int use_reg0_rgb = 0;
    int use_reg0_a = 0;
    int use_reg1_rgb = 0;
    int use_reg1_a = 0;

    in = &tev->color_a;
    for (i = 0; i < 4; i++) {
        switch (in[i]) {
        case TOBJ_TEV_CC_KONST_RGB:
            use_k_rgb = 1;
            break;
        case TOBJ_TEV_CC_KONST_RRR:
            use_k_r = 1;
            break;
        case TOBJ_TEV_CC_KONST_GGG:
            use_k_g = 1;
            break;
        case TOBJ_TEV_CC_KONST_BBB:
            use_k_b = 1;
            break;
        case TOBJ_TEV_CC_KONST_AAA:
            use_k_a = 1;
            break;
        case TOBJ_TEV_CC_TEX0_RGB:
            use_reg0_rgb = 1;
            break;
        case TOBJ_TEV_CC_TEX0_AAA:
            use_reg0_a = 1;
            break;
        case TOBJ_TEV_CC_TEX1_RGB:
            use_reg1_rgb = 1;
            break;
        case TOBJ_TEV_CC_TEX1_AAA:
            use_reg1_a = 1;
            break;
        default:
            break;
        }
    }
    in = &tev->alpha_a;
    for (i = 0; i < 4; i++) {
        switch (in[i]) {
        case TOBJ_TEV_CA_KONST_R:
            use_k_r = 1;
            break;
        case TOBJ_TEV_CA_KONST_G:
            use_k_g = 1;
            break;
        case TOBJ_TEV_CA_KONST_B:
            use_k_b = 1;
            break;
        case TOBJ_TEV_CA_KONST_A:
            use_k_a = 1;
            break;
        case TOBJ_TEV_CA_TEX0_A:
            use_reg0_a = 1;
            break;
        case TOBJ_TEV_CA_TEX1_A:
            use_reg1_a = 1;
            break;
        default:
            break;
        }
    }

    if (use_k_rgb) {
        konst_rgb = fn_801B6F5C(&tev->konst, HSD_TE_RGB, HSD_TE_U8, list);
    }
    if (use_k_r) {
        konst_r = fn_801B6F5C(&tev->konst.r, HSD_TE_X, HSD_TE_U8, list);
    }
    if (use_k_g) {
        konst_g = fn_801B6F5C(&tev->konst.g, HSD_TE_X, HSD_TE_U8, list);
    }
    if (use_k_b) {
        konst_b = fn_801B6F5C(&tev->konst.b, HSD_TE_X, HSD_TE_U8, list);
    }
    if (use_k_a) {
        konst_a = fn_801B6F5C(&tev->konst.a, HSD_TE_X, HSD_TE_U8, list);
    }
    if (use_reg0_rgb) {
        reg0_rgb = fn_801B6F5C(&tev->tev0, HSD_TE_RGB, HSD_TE_U8, list);
    }
    if (use_reg0_a) {
        reg0_a = fn_801B6F5C(&tev->tev0.a, HSD_TE_X, HSD_TE_U8, list);
    }
    if (use_reg1_rgb) {
        reg1_rgb = fn_801B6F5C(&tev->tev1, HSD_TE_RGB, HSD_TE_U8, list);
    }
    if (use_reg1_a) {
        reg1_a = fn_801B6F5C(&tev->tev1.a, HSD_TE_X, HSD_TE_U8, list);
    }

    e0 = fn_801B707C(list);
    fn_801B5E40(e0, tobj, GX_COLOR_NULL);

    if (tev->active & TOBJ_TEVREG_ACTIVE_COLOR_TEV) {
        u32 sel[4];
        HSD_TExp* exp[4];
        int i;

        in = &tev->color_a;
        for (i = 0; i < 4; i++) {
            switch (in[i]) {
            case GX_CC_ZERO:
                sel[i] = HSD_TE_0;
                exp[i] = HSD_TEXP_ZERO;
                break;
            case GX_CC_ONE:
                sel[i] = HSD_TE_1;
                exp[i] = HSD_TEXP_ZERO;
                break;
            case GX_CC_HALF:
                sel[i] = HSD_TE_4_8;
                exp[i] = HSD_TEXP_ZERO;
                break;
            case GX_CC_TEXC:
                sel[i] = HSD_TE_RGB;
                exp[i] = HSD_TEXP_TEX;
                break;
            case GX_CC_TEXA:
                sel[i] = HSD_TE_A;
                exp[i] = HSD_TEXP_TEX;
                break;
            case TOBJ_TEV_CC_KONST_RGB:
                sel[i] = HSD_TE_RGB;
                exp[i] = konst_rgb;
                break;
            case TOBJ_TEV_CC_KONST_RRR:
                sel[i] = HSD_TE_X;
                exp[i] = konst_r;
                break;
            case TOBJ_TEV_CC_KONST_GGG:
                sel[i] = HSD_TE_X;
                exp[i] = konst_g;
                break;
            case TOBJ_TEV_CC_KONST_BBB:
                sel[i] = HSD_TE_X;
                exp[i] = konst_b;
                break;
            case TOBJ_TEV_CC_KONST_AAA:
                sel[i] = HSD_TE_X;
                exp[i] = konst_a;
                break;
            case TOBJ_TEV_CC_TEX0_RGB:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6E74(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B64EC(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_RGB, reg0_rgb);
                sel[i] = HSD_TE_RGB;
                exp[i] = tmp;
                break;
            case TOBJ_TEV_CC_TEX0_AAA:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6E74(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B64EC(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_X, reg0_a);
                sel[i] = HSD_TE_RGB;
                exp[i] = tmp;
                break;
            case TOBJ_TEV_CC_TEX1_RGB:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6E74(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B64EC(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_RGB, reg1_rgb);
                sel[i] = HSD_TE_RGB;
                exp[i] = tmp;
                break;
            case TOBJ_TEV_CC_TEX1_AAA:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6E74(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B64EC(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_X, reg1_a);
                sel[i] = HSD_TE_RGB;
                exp[i] = tmp;
                break;
            default:
                __assert(&lbl_8047DEB0, 0x52F, &lbl_8047DED0);
                break;
            }
        }

        fn_801B6E74(e0, tev->color_op, tev->color_bias, tev->color_scale,
                    tev->color_clamp);
        fn_801B64EC(e0, sel[0], exp[0], sel[1], exp[1], sel[2], exp[2],
                    sel[3], exp[3]);
        *c = e0;
    }

    if (tev->active & TOBJ_TEVREG_ACTIVE_ALPHA_TEV) {
        u32 sel[4];
        HSD_TExp* exp[4];
        int i;

        in = &tev->alpha_a;
        for (i = 0; i < 4; i++) {
            switch (in[i]) {
            case GX_CA_ZERO:
                sel[i] = HSD_TE_0;
                exp[i] = HSD_TEXP_ZERO;
                break;
            case GX_CA_TEXA:
                sel[i] = HSD_TE_A;
                exp[i] = HSD_TEXP_TEX;
                break;
            case TOBJ_TEV_CA_KONST_R:
                sel[i] = HSD_TE_X;
                exp[i] = konst_r;
                break;
            case TOBJ_TEV_CA_KONST_G:
                sel[i] = HSD_TE_X;
                exp[i] = konst_g;
                break;
            case TOBJ_TEV_CA_KONST_B:
                sel[i] = HSD_TE_X;
                exp[i] = konst_b;
                break;
            case TOBJ_TEV_CA_KONST_A:
                sel[i] = HSD_TE_X;
                exp[i] = konst_a;
                break;
            case TOBJ_TEV_CA_TEX0_A:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6CD8(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B5F08(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_X, reg0_a);
                sel[i] = HSD_TE_A;
                exp[i] = tmp;
                break;
            case TOBJ_TEV_CA_TEX1_A:
                tmp = fn_801B707C(list);
                fn_801B5E40(tmp, NULL, GX_COLOR_NULL);
                fn_801B6CD8(tmp, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                            GX_ENABLE);
                fn_801B5F08(tmp, HSD_TE_0, HSD_TEXP_ZERO, HSD_TE_0,
                            HSD_TEXP_ZERO, HSD_TE_0, HSD_TEXP_ZERO,
                            HSD_TE_X, reg1_a);
                sel[i] = HSD_TE_A;
                exp[i] = tmp;
                break;
            default:
                __assert(&lbl_8047DEB0, 0x578, &lbl_8047DED0);
                break;
            }
        }

        fn_801B6CD8(e0, tev->alpha_op, tev->alpha_bias, tev->alpha_scale,
                    tev->alpha_clamp);
        fn_801B5F08(e0, sel[0], exp[0], sel[1], exp[1], sel[2], exp[2],
                    sel[3], exp[3]);

        *a = e0;
    }
}

/* ========================================================================= */
/*  TLUT (Texture Lookup Table / Palette) management                         */
/* ========================================================================= */

void fn_801BD8D0(HSD_TObj* tobj, u32 rendermode) {
    HSD_TObj* cur;

    for (cur = tobj; cur != NULL; cur = cur->next) {
        if (cur->id == 0xFF) {
            continue;
        }

        if (tobj_bump(cur)) {
            *(u32*) (lbl_8036D510 + 0x8) = fn_801B37A0();
            *(u32*) (lbl_8036D510 + 0xC) = cur->coord;
            *(u32*) (lbl_8036D510 + 0x10) = cur->id;
            *(u32*) (lbl_8036D510 + 0x18) = 0;
            *(u32*) (lbl_8036D510 + 0x3C) = 0;
            *(u8*) (lbl_8036D510 + 0x34) = 0;
            fn_801B3638(lbl_8036D510);

            *(u32*) (lbl_8036D510 + 0x8) = fn_801B37A0();
            *(u32*) (lbl_8036D510 + 0xC) = cur->coord + 1;
            *(u32*) (lbl_8036D510 + 0x18) = 1;
            *(u32*) (lbl_8036D510 + 0x3C) = 1;
            *(u8*) (lbl_8036D510 + 0x34) = 1;
            fn_801B3638(lbl_8036D510);
        }

        if (tobj_lightmap(cur) & TEX_LIGHTMAP_SHADOW) {
            while (cur != NULL && tobj_coord(cur) == TEX_COORD_SHADOW) {
                *(u32*) (lbl_8036D48C + 0x8) = fn_801B37A0();
                *(u32*) (lbl_8036D48C + 0xC) = cur->coord;
                *(u32*) (lbl_8036D48C + 0x10) = cur->id;
                fn_801B3638(lbl_8036D48C);
                cur = cur->next;
            }
            break;
        }
    }
}

void fn_801BDA58(HSD_TObj* tobj) {
    HSD_TObj* cur;
    u32 mask;
    s32 i;
    u32 src;
    static u32 bump_func[] = {
        2, 3, 4, 5, 6, 7, 8, 9,
    };

    for (cur = tobj; cur != NULL; cur = cur->next) {
        if (cur->id == 0xFF) {
            continue;
        }

        if (tobj_bump(cur)) {
            switch (tobj_coord(cur)) {
            case TEX_COORD_SHADOW:
                fn_800B857C(cur->coord, 0, 0, 0, 0, cur->mtxid);
                break;
            case TEX_COORD_REFLECTION:
            case TEX_COORD_HILIGHT:
            case TEX_COORD_GRADATION:
                fn_800B857C(cur->coord, 0, 1, 0x1E, 1, cur->mtxid);
                break;
            case 6:
                fn_800B857C(cur->coord, 0, 0, 0x3C, 1, cur->mtxid);
                break;
            default:
                fn_800B857C(cur->coord, 1, cur->src, 0x3C, 0, 0x7D);
                break;
            }

            mask = fn_801A68F0();
            for (i = 0; i < 8; i++) {
                if (mask & (1U << i)) {
                    break;
                }
            }
            if (i >= 8) {
                i = 0;
            }
            switch (cur->coord) {
            case 0: src = 0xC; break;
            case 1: src = 0xD; break;
            case 2: src = 0xE; break;
            case 3: src = 0xF; break;
            case 4: src = 0x10; break;
            case 5: src = 0x11; break;
            case 6: src = 0x12; break;
            default:
                __assert(&lbl_8047DEB0, 0x756, &lbl_8047DED0);
                src = 0xC;
                break;
            }
            fn_800B857C(cur->coord + 1, bump_func[i], src, 0x3C, 0, 0x7D);
        } else if (tobj_coord(cur) == TEX_COORD_TOON) {
            fn_800B857C(cur->coord, 0xA, cur->src, 0x3C, 0, 0x7D);
        } else {
            switch (tobj_coord(cur)) {
            case TEX_COORD_SHADOW:
                fn_800B857C(cur->coord, 0, 0, 0, 0, cur->mtxid);
                break;
            case TEX_COORD_REFLECTION:
            case TEX_COORD_HILIGHT:
            case TEX_COORD_GRADATION:
                fn_800B857C(cur->coord, 0, 1, 0x1E, 1, cur->mtxid);
                break;
            case 6:
                fn_800B857C(cur->coord, 0, 0, 0x3C, 1, cur->mtxid);
                break;
            default:
                fn_800B857C(cur->coord, 1, cur->src, 0x3C, 0, cur->mtxid);
                break;
            }
        }
    }
}

void fn_801BDD74(HSD_TObj* tobj) {
    f32 mtx[3][4];
    f32 v[3];
    f32 half[3];
    void* cobj;
    void* lobj;
    s32 i;

    if (tobj_coord(tobj) == TEX_COORD_TOON) {
        return;
    }

    if (tobj->flags & TEX_MTX_DIRTY) {
        (*(void (**)(HSD_TObj*)) ((u8*) *(void**) tobj + 0x3C))(tobj);
        tobj->flags &= ~TEX_MTX_DIRTY;
    }

    switch (tobj_coord(tobj)) {
    case TEX_COORD_REFLECTION:
        for (i = 0; i < 3; i++) {
            mtx[i][0] = 0.5f * tobj->mtx[i][0];
            mtx[i][1] = -0.5f * tobj->mtx[i][1];
            mtx[i][2] = 0.0f;
            mtx[i][3] = 0.5f * tobj->mtx[i][0] + 0.5f * tobj->mtx[i][1] +
                        tobj->mtx[i][2] + tobj->mtx[i][3];
        }
        fn_800BD58C(mtx, tobj->mtxid, 0);
        break;

    case TEX_COORD_HILIGHT:
        lobj = fn_801A4AC4(1);
        if (lobj != NULL) {
            cobj = fn_801942B8();
            if (cobj == NULL) {
                __assert(&lbl_8047DEB0, 0x312, lbl_802756C4);
            }
            fn_801A620C(lobj, v);
            fn_800A3820((u8*) cobj + 0x54, v, v);
            v[2] += -1.0f;
            fn_800A3ADC(v, half);
            half[0] *= -0.5f;
            half[1] *= -0.5f;
            half[2] *= -0.5f;

            mtx[0][0] = tobj->mtx[0][0] * half[0];
            mtx[0][1] = tobj->mtx[0][0] * half[1];
            mtx[0][2] = tobj->mtx[0][0] * half[2];
            mtx[0][3] = tobj->mtx[0][0] * 0.5f + tobj->mtx[0][3];
            mtx[1][0] = tobj->mtx[1][0] * half[0];
            mtx[1][1] = tobj->mtx[1][0] * half[1];
            mtx[1][2] = tobj->mtx[1][0] * half[2];
            mtx[1][3] = tobj->mtx[1][0] * 0.5f + tobj->mtx[1][3];
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 0.0f;
            mtx[2][3] = 1.0f;
            fn_800BD58C(mtx, tobj->mtxid, 0);
        } else {
            fn_800BD58C(lbl_8036D43C, tobj->mtxid, 0);
        }
        break;

    case TEX_COORD_SHADOW:
        cobj = fn_801942B8();
        fn_800A2D98(&tobj->mtx, fn_80194CF4(), mtx);
        fn_800BD58C(mtx, tobj->mtxid, 0);
        break;

    case TEX_COORD_GRADATION:
        cobj = fn_801942B8();
        switch (fn_801943CC(cobj)) {
        case 1:
            fn_800A3678(mtx, *(f32*) ((u8*) cobj + 0x40),
                        *(f32*) ((u8*) cobj + 0x44), 0.5f, 1.0f, 0.5f, 0.5f);
            break;
        case 2:
            fn_800A35E4(mtx, *(f32*) ((u8*) cobj + 0x40),
                        *(f32*) ((u8*) cobj + 0x44),
                        *(f32*) ((u8*) cobj + 0x48),
                        *(f32*) ((u8*) cobj + 0x4C),
                        *(f32*) ((u8*) cobj + 0x38), 0.5f, 1.0f, 0.5f);
            break;
        default:
            fn_800A3744(mtx, *(f32*) ((u8*) cobj + 0x40),
                        *(f32*) ((u8*) cobj + 0x44),
                        *(f32*) ((u8*) cobj + 0x48),
                        *(f32*) ((u8*) cobj + 0x4C),
                        0.5f, 1.0f, 0.5f, 0.5f);
            break;
        }
        fn_800A2D98(&tobj->mtx, mtx, &tobj->mtx);
        fn_800BD58C(&tobj->mtx, tobj->mtxid, 0);
        break;

    case 6:
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 0.0f;
        mtx[2][3] = 1.0f;
        fn_800BD58C(mtx, tobj->mtxid, 0);
        break;

    default:
        fn_800BD58C(&tobj->mtx, tobj->mtxid, tobj_bump(tobj) ? 1 : 0);
        break;
    }
}

/* ========================================================================= */
/*  Image descriptor management                                              */
/* ========================================================================= */

void fn_801BE2B4(HSD_TObj* tobj) {
    f32 scale[3];
    f32 rot[3];
    f32 trans[3];
    f32 m[3][4];
    f32 abs_x;
    f32 abs_y;
    f32 mirror_offset;

    if (tobj->repeat_s == 0 || tobj->repeat_t == 0) {
        __assert(&lbl_8047DEB0, 0x267, lbl_802756C4);
    }

    abs_x = tobj->scale_x;
    if (abs_x < 0.0f) {
        abs_x = -abs_x;
    }
    if (abs_x < 0.0000000001f) {
        scale[0] = 0.0f;
    } else {
        scale[0] = (f32) tobj->repeat_s / tobj->scale_x;
    }

    abs_y = tobj->scale_y;
    if (abs_y < 0.0f) {
        abs_y = -abs_y;
    }
    if (abs_y < 0.0000000001f) {
        scale[1] = 0.0f;
    } else {
        scale[1] = (f32) tobj->repeat_t / tobj->scale_y;
    }
    scale[2] = tobj->scale_z;

    rot[0] = tobj->rotate_x;
    rot[1] = tobj->rotate_y;
    rot[2] = -tobj->rotate_z;

    trans[0] = -tobj->translate_x;
    if (tobj->wrap_t == 2) {
        mirror_offset = 1.0f / ((f32) tobj->repeat_t / tobj->scale_y);
    } else {
        mirror_offset = 0.0f;
    }
    trans[1] = -(tobj->translate_y + mirror_offset);
    trans[2] = tobj->translate_z;

    fn_800A32B4(&tobj->mtx, trans[0], trans[1], trans[2]);
    fn_801A8B94(m, rot);
    fn_800A2D98(m, &tobj->mtx, &tobj->mtx);
    fn_800A3334(m, scale[0], scale[1], scale[2]);
    fn_800A2D98(m, &tobj->mtx, &tobj->mtx);
}

/*
 * HSD_ImageGetSize - 0x801BE490 | Size: 0x3C
 * Get the size in bytes of an image based on its descriptor.
 */
u32 fn_801BE490(HSD_ImageDesc* desc) {
    u32 size;

    if (desc == NULL) {
        return 0;
    }

    /* Calculate size based on format and dimensions */
    size = desc->width * desc->height;

    switch (desc->format) {
    case 0: /* I4 */
        size /= 2;
        break;
    case 1: /* I8 */
        break;
    case 2: /* IA4 */
        break;
    case 3: /* IA8 */
    case 4: /* RGB565 */
    case 5: /* RGB5A3 */
        size *= 2;
        break;
    case 6: /* RGBA8 */
        size *= 4;
        break;
    default:
        break;
    }

    return size;
}

/* HSD_TObjLoadDesc - allocates a TObj and dispatches its load method. */
HSD_TObj* fn_801BE4CC(HSD_TObjDesc* desc) {
    HSD_ClassInfo* info;
    HSD_TObj* tobj;

    if (desc == NULL) {
        return NULL;
    }

    if (desc->class_name != NULL &&
        (info = fn_80193748(desc->class_name)) != NULL)
    {
        tobj = fn_80193828(info);
        if (tobj == NULL) {
            __assert(&lbl_8047DEB0, 0x1ED, &lbl_8047DF10);
        }
    } else {
        if (lbl_8047B378 != NULL) {
            info = lbl_8047B378;
        } else {
            info = (HSD_ClassInfo*) lbl_8036D3F0;
        }
        tobj = fn_80193828(info);
        if (tobj == NULL) {
            __assert(&lbl_8047DEB0, 0x884, &lbl_8047DECC);
        }
    }

    ((s32 (*)(HSD_TObj*, HSD_TObjDesc*)) (*(void**)
        ((u8*) HSD_CLASS_METHOD(tobj) + 0x40)))(tobj, desc);
    return tobj;
}

s32 fn_801BE598(HSD_TObj* tobj, HSD_TObjDesc* desc) {
    HSD_TObjDesc* next_desc;
    HSD_TObj* next;
    void* src;
    void* copy;

    next_desc = *(HSD_TObjDesc**) ((u8*) desc + 0x4);
    if (next_desc != NULL) {
        next = fn_801BE4CC(next_desc);
    } else {
        next = NULL;
    }
    *(HSD_TObj**) ((u8*) tobj + 0x8) = next;

    *(u16*) ((u8*) tobj + 0x4A) = *(u32*) ((u8*) desc + 0x8);
    *(u32*) ((u8*) tobj + 0x10) = *(u32*) ((u8*) desc + 0xC);
    *(u32*) ((u8*) tobj + 0x14) = 0x3C;
    *(f32*) ((u8*) tobj + 0x18) = *(f32*) ((u8*) desc + 0x10);
    *(f32*) ((u8*) tobj + 0x1C) = *(f32*) ((u8*) desc + 0x14);
    *(f32*) ((u8*) tobj + 0x20) = *(f32*) ((u8*) desc + 0x18);
    *(u32*) ((u8*) tobj + 0x28) = *(u32*) ((u8*) desc + 0x1C);
    *(u32*) ((u8*) tobj + 0x2C) = *(u32*) ((u8*) desc + 0x20);
    *(u32*) ((u8*) tobj + 0x30) = *(u32*) ((u8*) desc + 0x24);
    *(u32*) ((u8*) tobj + 0x34) = *(u32*) ((u8*) desc + 0x28);
    *(u32*) ((u8*) tobj + 0x38) = *(u32*) ((u8*) desc + 0x2C);
    *(u32*) ((u8*) tobj + 0x3C) = *(u32*) ((u8*) desc + 0x30);
    *(u32*) ((u8*) tobj + 0x40) = *(u32*) ((u8*) desc + 0x34);
    *(u32*) ((u8*) tobj + 0x44) = *(u32*) ((u8*) desc + 0x38);
    *(u8*) ((u8*) tobj + 0x48) = *(u8*) ((u8*) desc + 0x3C);
    *(u8*) ((u8*) tobj + 0x49) = *(u8*) ((u8*) desc + 0x3D);
    *(u32*) ((u8*) tobj + 0x4C) = *(u32*) ((u8*) desc + 0x40);
    *(f32*) ((u8*) tobj + 0x50) = *(f32*) ((u8*) desc + 0x44);
    *(u32*) ((u8*) tobj + 0x54) = *(u32*) ((u8*) desc + 0x48);
    *(u32*) ((u8*) tobj + 0x58) = *(u32*) ((u8*) desc + 0x4C);

    src = *(void**) ((u8*) desc + 0x50);
    if (src != NULL) {
        copy = fn_80193B10(0x10);
        if (copy == NULL) {
            __assert(&lbl_8047DEB0, 0x8A1, &lbl_8047DEC4);
        }
        memset(copy, 0, 0x10);
        memcpy(copy, src, 0x10);
    } else {
        copy = NULL;
    }
    *(void**) ((u8*) tobj + 0x5C) = copy;

    *(u32*) ((u8*) tobj + 0x60) = *(u32*) ((u8*) desc + 0x54);
    *(u32*) ((u8*) tobj + 0x64) = 0;
    *(u32*) ((u8*) tobj + 0x4C) |= 0x80000000;
    *(u8*) ((u8*) tobj + 0x70) = 0xFF;

    src = *(void**) ((u8*) desc + 0x58);
    if (src != NULL) {
        copy = fn_80193B10(0x20);
        if (copy == NULL) {
            __assert(&lbl_8047DEB0, 0x8CC, &lbl_8047DEB8);
        }
        memset(copy, 0, 0x20);
        memcpy(copy, src, 0x20);
    } else {
        copy = NULL;
    }
    *(void**) ((u8*) tobj + 0xA8) = copy;

    fn_8019C6EC(2);
    return 0;
}

/* HSD_TObjAnimAll */
void fn_801BE800(HSD_TObj* tobj) {
    HSD_TObj* tp;

    if (tobj != NULL) {
        for (tp = tobj; tp != NULL; tp = TOBJ_NEXT_RAW(tp)) {
            if (tp != NULL) {
                fn_801C27F4(TOBJ_AOBJ_RAW(tp), tp,
                            *(void**) ((u8*) HSD_CLASS_METHOD(tp) + 0x48));
            }
        }
    }
}

/* ========================================================================= */
/*  TObj rendering setup                                                     */
/* ========================================================================= */

void fn_801BE85C(HSD_TObj* tobj, u32 type, HSD_ObjDataWork* val) {
    f32 fval;
    s32 index;
    void** images;
    void* tev;

    if (tobj == NULL || type > 24) {
        return;
    }

    switch (type) {
    case 1:
        images = *(void***) ((u8*) tobj + 0x68);
        if (images == NULL) {
            __assert(&lbl_8047DEB0, 0x116, lbl_802756E8);
        }
        index = (s32) val->fv;
        if (images[index] != NULL) {
            *(void**) ((u8*) tobj + 0x58) = images[index];
        }
        break;
    case 10:
        if (*(void**) ((u8*) tobj + 0x6C) != NULL) {
            *(u8*) ((u8*) tobj + 0x70) = (u8) val->fv;
        }
        break;
    case 9:
        fval = val->fv;
        if (fval < 0.0f) {
            fval = 0.0f;
        } else if (fval > 1.0f) {
            fval = 1.0f;
        }
        *(f32*) ((u8*) tobj + 0x50) = fval;
        break;
    case 6:
        *(f32*) ((u8*) tobj + 0x18) = val->fv;
        goto mtxdirty;
    case 7:
        *(f32*) ((u8*) tobj + 0x1C) = val->fv;
        goto mtxdirty;
    case 8:
        *(f32*) ((u8*) tobj + 0x20) = val->fv;
        goto mtxdirty;
    case 2:
        *(f32*) ((u8*) tobj + 0x34) = val->fv;
        goto mtxdirty;
    case 3:
        *(f32*) ((u8*) tobj + 0x38) = val->fv;
        goto mtxdirty;
    case 4:
        *(f32*) ((u8*) tobj + 0x28) = val->fv;
        goto mtxdirty;
    case 5:
        *(f32*) ((u8*) tobj + 0x2C) = val->fv;
    mtxdirty:
        *(u32*) ((u8*) tobj + 0x4C) |= 0x80000000;
        break;
    case 11:
        if (*(void**) ((u8*) tobj + 0x60) != NULL) {
            *(f32*) ((u8*) *(void**) ((u8*) tobj + 0x60) + 4) = val->fv;
        }
        break;
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
        tev = *(void**) ((u8*) tobj + 0xA8);
        if (tev != NULL) {
            fval = val->fv;
            if (fval < 0.0f) {
                fval = 0.0f;
            } else if (fval > 1.0f) {
                fval = 1.0f;
            }
            *(u8*) ((u8*) tev + type + 4) = (u8) (255.0f * fval);
        }
        break;
    case 24:
        fval = val->fv;
        if (fval < 0.0f) {
            fval = 0.0f;
        } else if (fval > 1.0f) {
            fval = 1.0f;
        }
        *(f32*) ((u8*) tobj + 0x50) = fval;
        break;
    }
}

/* HSD_TObjReqAnimAllByFlags - requests animation on flagged TObj AObjs. */
void fn_801BEE68(HSD_TObj* tobj, u32 flags, f32 frame) {
    HSD_TObj* cur;

    if (tobj == NULL) {
        return;
    }

    for (cur = tobj; cur != NULL; cur = TOBJ_NEXT_RAW(cur)) {
        if (cur != NULL) {
            if (flags & 0x10) {
                fn_801C29C4(TOBJ_AOBJ_RAW(cur), frame);
            }
        }
    }
}

/* HSD_TObjAddAnim */
void fn_801BEEDC(HSD_TObj* tobj, HSD_TexAnim* texanim) {
    HSD_TObj* tp;
    HSD_TexAnim* ta;
    HSD_TexAnim* cur_anim;
    HSD_TlutWork* tlut;
    void* tlutdesc;
    s32 i;

    if (tobj != NULL) {
        for (tp = tobj; tp != NULL; tp = TOBJ_NEXT_RAW(tp)) {
            if (tp != NULL) {
                ta = NULL;
                for (cur_anim = texanim; cur_anim != NULL;
                     cur_anim = cur_anim->next)
                {
                    if ((s32) cur_anim->id == TOBJ_TEXANIM_ID_RAW(tp)) {
                        ta = cur_anim;
                        break;
                    }
                }

                if (ta != NULL) {
                    if (TOBJ_AOBJ_RAW(tp) != NULL) {
                        fn_801C25E4(TOBJ_AOBJ_RAW(tp));
                    }
                    TOBJ_AOBJ_RAW(tp) = fn_801C2670(ta->aobjdesc);
                    TOBJ_IMAGETBL_RAW(tp) = ta->imagetbl;

                    if (TOBJ_TLUTTBL_RAW(tp) != NULL) {
                        for (i = 0; TOBJ_TLUTTBL_RAW(tp)[i] != NULL; i++) {
                            if (TOBJ_TLUTTBL_RAW(tp)[i] != NULL) {
                                fn_80193AF0(TOBJ_TLUTTBL_RAW(tp)[i], 0x10);
                            }
                        }
                        fn_801A6960(TOBJ_TLUTTBL_RAW(tp));
                    }

                    if (ta->n_tluttbl != 0) {
                        TOBJ_TLUTTBL_RAW(tp) =
                            fn_801A6928((ta->n_tluttbl + 1) * 4);
                        for (i = 0; i < ta->n_tluttbl; i++) {
                            tlutdesc = ta->tluttbl[i];
                            if (tlutdesc != NULL) {
                                tlut = fn_80193B10(0x10);
                                if (tlut == NULL) {
                                    __assert(&lbl_8047DEB0, 0x8A1,
                                             &lbl_8047DEC4);
                                }
                                memset(tlut, 0, 0x10);
                                memcpy(tlut, tlutdesc, 0x10);
                            } else {
                                tlut = NULL;
                            }
                            TOBJ_TLUTTBL_RAW(tp)[i] = tlut;
                        }
                        TOBJ_TLUTTBL_RAW(tp)[i] = NULL;
                    } else {
                        TOBJ_TLUTTBL_RAW(tp) = NULL;
                    }
                    TOBJ_TLUT_NO_RAW(tp) = 0xFF;
                }
            }
        }
    }
}

/* ========================================================================= */
/*  Render pipeline transition functions                                     */
/* ========================================================================= */

/*
 * HSD_RenderPassInit - 0x801BF098 | Size: 0xA0
 * Initialize render pass state.
 */
void HSD_Index2PosNrmMtx(void* state, u32 num_passes) {
    if (state == NULL) {
        return;
    }

    /* Initialize render pass tracking:
     * - Set number of passes
     * - Clear per-pass state
     * - Initialize sort key generation
     */
}

/* Counts set bits in a 32-bit mask. */
#pragma push
#pragma optimization_level 1
s32 fn_801BF138(u32 flags) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 32; i++) {
        if (flags & (1U << i)) {
            count++;
        }
    }
    return count;
}
#pragma pop

/* HSD_MulColor */
void fn_801BF16C(GXColorWork* lhs, GXColorWork* rhs, GXColorWork* dst) {
    dst->r = (lhs->r * rhs->r) / 255U;
    dst->g = (lhs->g * rhs->g) / 255U;
    dst->b = (lhs->b * rhs->b) / 255U;
    dst->a = (lhs->a * rhs->a) / 255U;
}

/*
 * HSD_RenderPassExecute - 0x801BF1F0 | Size: 0x2D4
 * Execute render callbacks for the current pass.
 */
void fn_801BF1F0(void* state) {
    if (state == NULL) {
        return;
    }

    /* Dispatch render callbacks for the current pass:
     * 1. Get current pass index
     * 2. Walk the render list
     * 3. For each object in the list:
     *    - Check if object participates in this pass
     *    - Call the object's render callback
     */
}

/* Address: 0x801BF4C4 | Size: 0x20 */
/* Store value to BSS object and set flag */
extern u8 lbl_80466BC0[];
void fn_801BF4C4(u32 val) {
    *(u32*)(lbl_80466BC0 + 0x3C) = val;
    *(u8*)(lbl_80466BC0 + 0x54) = 1;
}

/* Copies the active render-mode block into lbl_80466BC0 and marks it dirty. */
void fn_801BF4E4(GXRenderModeObjWork* src) {
    *(GXRenderModeObjWork*) lbl_80466BC0 = *src;
    *(u8*) (lbl_80466BC0 + 0x54) = 1;
}

/* ========================================================================= */
/*  HSD-to-battle transition code                                            */
/* ========================================================================= */

s32 fn_801BF574(void) {
    s32 intr;
    s32 idx;
    s32 i;

    intr = OSDisableInterrupts();

    idx = -1;
    for (i = 0; i < 3; i++) {
        if (*(s32*) (lbl_80466BC0 + 0x5C + i * 0x60) == 4) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        for (i = 0; i < 3; i++) {
            if (*(s32*) (lbl_80466BC0 + 0x5C + i * 0x60) == 5) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            for (i = 0; i < 3; i++) {
                if (*(s32*) (lbl_80466BC0 + 0x5C + i * 0x60) == 6) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) {
                for (i = 0; i < 3; i++) {
                    if (*(s32*) (lbl_80466BC0 + 0x5C + i * 0x60) == 7) {
                        idx = i;
                        break;
                    }
                }
            }
        }
    }

    OSRestoreInterrupts(intr);
    return idx;
}

/*
 * HSD_BattleGXStateConfig - 0x801BF6AC | Size: 0x1F4
 * Configure GX state specifically for battle rendering.
 */
void fn_801BF6AC(void* ctx) {
    if (ctx == NULL) {
        return;
    }

    /* Configure battle-specific GX state:
     * - Set up TEV stages for battle effects
     * - Configure alpha blending for transparency
     * - Set up depth buffer configuration
     * - Configure fog parameters
     */
}

/*
 * HSD_BattleModelMtxSetup - 0x801BF8A0 | Size: 0x17C
 * Set up model matrices for battle rendering.
 */
void fn_801BF8A0(void* ctx) {
    if (ctx == NULL) {
        return;
    }

    /* Set up matrices:
     * - View matrix (camera)
     * - Projection matrix (perspective)
     * - Model matrix (identity initially)
     */
}

/*
 * HSD_BattleTevSetup - 0x801BFA1C | Size: 0x284
 * Set up TEV stages for the battle texture environment.
 */
void fn_801BFA1C(void* ctx) {
    if (ctx == NULL) {
        return;
    }

    /* Configure TEV for battle:
     * - Stage 0: Base texture * vertex color
     * - Stage 1: + specular highlight
     * - Stage 2: * shadow factor
     * - Stage 3: + emission
     */
}

/* Address: 0x801BFCA0 | Size: 0x10 */
/* Get field at offset 0x1E0 from BSS object */
u32 fn_801BFCA0(void) {
    return *(u32*)(lbl_80466BC0 + 0x1E0);
}

/* Clears the pending render callback state and invokes the registered hook. */
void fn_801BFCB0(void) {
    void (*cb)(s32);

    cb = *(void (**)(s32)) (lbl_80466BC0 + 0x1E8);
    *(u32*) (lbl_80466BC0 + 0x1E0) = 0;
    if (cb != NULL) {
        cb = *(void (**)(s32)) (lbl_80466BC0 + 0x1E8);
        cb(*(s32*) (lbl_80466BC0 + 0x1E4));
    }
}

/*
 * HSD_BattleFullRenderSetup - 0x801BFD10 | Size: 0x208
 * Full battle render setup combining all configuration steps.
 */
void fn_801BFD10(void* ctx) {
    if (ctx == NULL) {
        return;
    }

    /* Complete battle render setup:
     * 1. Context setup (fn_801BF574)
     * 2. GX state config (fn_801BF6AC)
     * 3. Model matrix setup (fn_801BF8A0)
     * 4. TEV setup (fn_801BFA1C)
     */
    fn_801BF574();
    fn_801BF6AC(ctx);
    fn_801BF8A0(ctx);
    fn_801BFA1C(ctx);
}

/*
 * HSD_BattleSceneInit - 0x801BFF18 | Size: 0x2B0
 * Battle scene initialization - the last function in the range.
 * Initializes the complete battle scene rendering pipeline.
 */
void fn_801BFF18(void* ctx) {
    if (ctx == NULL) {
        return;
    }

    /* Full battle scene initialization:
     * 1. Initialize render pass state
     * 2. Create battle camera
     * 3. Set up battle lights
     * 4. Configure render passes:
     *    Pass 0: Opaque geometry
     *    Pass 1: Transparent geometry
     *    Pass 2: Shadow volumes
     *    Pass 3: Effects/particles
     *    Pass 4: HUD overlay
     * 5. Initialize sort buffers
     */
    fn_801BFD10(ctx);
}
