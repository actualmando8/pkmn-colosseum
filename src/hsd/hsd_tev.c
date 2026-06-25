/**
 * @file hsd_tev.c
 * @brief HSD TEV (Texture Environment) stage setup and management.
 *
 * Address range: 0x801B1730 - 0x801B3D1C
 * Contains TEV stage configuration, texture coordinate generation,
 * color/alpha combine setup, and render pass state management.
 * This is the core of the HSD material rendering pipeline.
 *
 * Decompiled from Melee src/sysdolphin/baselib/tev.c / texp.c
 */

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_tobj.h"
#include "hsd/hsd_memory.h"

/* ========================================================================= */
/*  Internal TEV types                                                        */
/* ========================================================================= */

/* TEV stage descriptor - internal representation */
typedef struct HSD_TevDesc {
    u32 stage;         /* GXTevStageID */
    u32 texcoord;      /* GXTexCoordID */
    u32 texmap;        /* GXTexMapID */
    u32 color_chan;     /* GXChannelID */
    /* Color combine */
    u32 color_a;       /* GXTevColorArg */
    u32 color_b;
    u32 color_c;
    u32 color_d;
    u32 color_op;      /* GXTevOp */
    u32 color_bias;    /* GXTevBias */
    u32 color_scale;   /* GXTevScale */
    u32 color_clamp;   /* GXBool */
    u32 color_out_reg; /* GXTevRegID */
    /* Alpha combine */
    u32 alpha_a;       /* GXTevAlphaArg */
    u32 alpha_b;
    u32 alpha_c;
    u32 alpha_d;
    u32 alpha_op;
    u32 alpha_bias;
    u32 alpha_scale;
    u32 alpha_clamp;
    u32 alpha_out_reg;
} HSD_TevDesc;

typedef struct HSD_TevSetupDesc {
    struct HSD_TevSetupDesc* next;
    u32 flags;
    u32 stage;
    u32 coord;
    u32 map;
    u32 color;
    union {
        struct {
            u32 clr_op;
            u32 clr_a;
            u32 clr_b;
            u32 clr_c;
            u32 clr_d;
            u32 clr_scale;
            u32 clr_bias;
            u8 clr_clamp;
            u8 pad_35[3];
            u32 clr_out_reg;
            u32 alpha_op;
            u32 alpha_a;
            u32 alpha_b;
            u32 alpha_c;
            u32 alpha_d;
            u32 alpha_scale;
            u32 alpha_bias;
            u8 alpha_clamp;
            u8 pad_59[3];
            u32 alpha_out_reg;
            u32 pad_60;
            s32 ras_swap;
            s32 tex_swap;
            u32 kcsel;
            u32 kasel;
            u32 swap_r;
            u32 swap_g;
            u32 swap_b;
            u32 swap_a;
        } tevconf;
        struct {
            u32 tevmode;
        } tevop;
    } u;
} HSD_TevSetupDesc;

typedef struct HSD_StateInvalidateEntry {
    u32 mask;
    void (*func)(void);
} HSD_StateInvalidateEntry;

typedef struct GXColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColor;

typedef struct GXColorS10 {
    s16 r;
    s16 g;
    s16 b;
    s16 a;
} GXColorS10;

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct HSD_Spline {
    u8 type;
    u8 pad_01;
    s16 numcv;
    f32 tension;
    Vec3* cv;
    f32 totalLength;
    f32* segLength;
    f32 (*segPoly)[5];
} HSD_Spline;

typedef struct HSD_TevRegState {
    GXColorS10 color;
    s32 dirty;
} HSD_TevRegState;

typedef struct HSD_ChanLocal {
    struct HSD_ChanLocal* next;
    s32 chan;
    u32 flags;
    GXColor amb_color;
    GXColor mat_color;
    u8 enable;
    u8 pad_15[3];
    s32 amb_src;
    s32 mat_src;
    u32 light_mask;
    s32 diff_fn;
    s32 attn_fn;
    void* aobj;
} HSD_ChanLocal;

typedef struct HSD_MatState {
    u32 ambient;
    u32 diffuse;
    u32 specular;
    u8 alpha;
    u8 pad_0D[3];
    f32 shininess;
} HSD_MatState;

typedef struct HSD_LObjMini {
    u8 pad_00[8];
    u16 flags;
    u8 pad_0A[6];
    GXColor color;
} HSD_LObjMini;

/* TExp node types */
#define HSD_TE_ZERO  0
#define HSD_TE_TEX   1
#define HSD_TE_RAS   2
#define HSD_TE_CNST  3
#define HSD_TE_IMM   4
#define HSD_TE_KONST 5
#define HSD_TE_ALL   6

/* TExp node structure - full definition of the union forward-declared in hsd_forward.h */
union HSD_TExp {
    struct {
        u32 type;
        void* data;
        union HSD_TExp* next;
        u32 op;
        union HSD_TExp* arg[4];
        u32 sel;
        u32 reg;
    };
};

/* Forward declarations */
extern void* memcpy(void* dst, const void* src, u32 size);
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
extern HSD_StateInvalidateEntry lbl_8036CFA8[];
extern HSD_ChanLocal lbl_8036CE88[];
extern HSD_TevRegState lbl_8036CFE8[];
extern u8 lbl_8036D018[];
extern u8 lbl_8036D0D8[];
extern u8 lbl_80465710[];
extern u32 lbl_80478C98;
extern u8 lbl_8047B318;
extern u8 lbl_8047B319;
extern u8 lbl_8047B31A;
extern u8 lbl_8047B31B;
extern u8 lbl_8047B31C;
extern u8 lbl_8047B31D;
extern u8 lbl_8047B31E;
extern s32 lbl_8047B320;
extern s32 lbl_8047B324;
extern u8 lbl_8047B328;
extern s32 lbl_8047B32C;
extern u8 lbl_8047B330;
extern s32 lbl_8047B334;
extern u8 lbl_8047B338;
extern s32 lbl_8047B33C;
extern s32 lbl_8047B340;
extern s32 lbl_8047B344;
extern s32 lbl_8047B348;
extern s32 lbl_8047B34C;
extern u8 lbl_8047B350;
extern u8 lbl_8047B351;
extern u32 lbl_8047B358;
extern s32 lbl_8047B35C;
extern u32 lbl_8047B360[];
extern u32 lbl_8047B368[];
extern u32 lbl_8047B370;
extern s32 lbl_8047B37C;
extern const char lbl_8047DE60[];
extern const char lbl_8047DE68[];
extern f32 lbl_8047DE50;
extern f32 lbl_8047DE54;
extern f32 lbl_8047DE58;
extern f32 sqrtf(f32);
extern f32 fn_801B1AD0(f32* coeffs, f32 start, f32 end);
extern void fn_800BC36C(u32 reg, GXColorS10 color);
extern void fn_800B884C(u8 count);
extern void fn_800B94F0(s32 mode);
extern void fn_800BA6B0(u32 num);
extern void fn_800BC114(u32 stage, u32 mode);
extern void fn_800BC1A0(u32 stage, u32 a, u32 b, u32 c, u32 d);
extern void fn_800BC1E4(u32 stage, u32 a, u32 b, u32 c, u32 d);
extern void fn_800BC228(u32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 out_reg);
extern void fn_800BC290(u32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 out_reg);
extern void fn_800BC454(u32 stage, u32 sel);
extern void fn_800BC4C0(u32 stage, u32 sel);
extern void fn_800BC52C(u32 stage, u32 ras_sel, u32 tex_sel);
extern void fn_800BC580(u32 id, u32 r, u32 g, u32 b, u32 a);
extern void fn_800BC618(u32 comp0, u8 ref0, u32 op, u32 comp1, u8 ref1);
extern void fn_800BC6F0(u32 stage, u32 coord, u32 map, u32 color);
extern void fn_800BC8C8(u8 num);
extern void fn_800BCDDC(u32 type, u32 src_factor, u32 dst_factor, u32 logic_op);
extern void fn_800BCE30(u32 update);
extern void fn_800BCE5C(u32 update);
extern void fn_800BCE88(u32 enable, u32 func, u32 update);
extern void fn_800BCEBC(u32 before_tex);
extern void fn_800BCFDC(u32 dither);
extern void fn_800BD008(u32 enable, u8 alpha);
extern s32 fn_801B387C();
extern void fn_801B29E4();
extern void fn_801B2F1C();
extern HSD_TExp* fn_801B3258();
extern void fn_801B3638();
extern void fn_801B3770();
extern void fn_801B3890();
extern void fn_801B3AA8();
extern HSD_TExp* fn_801B3AE8();
extern void fn_801B3D1C();
extern u32 HSD_LObjGetLightMaskSpecular(void);
extern s32 HSD_LObjGetNbActive(void);
extern HSD_LObjMini* HSD_LObjGetActiveByIndex(s32 idx);
extern HSD_LObjMini* HSD_LObjGetActiveByID(u32 id);
extern u32 HSD_LObjGetLightMaskDiffuse(void);
extern u32 HSD_LObjGetLightMaskAlpha(void);
extern void fn_801A6098(HSD_LObjMini* lobj, GXColor* color, f32 shininess);
extern void fn_801BF16C(void* lhs, void* rhs, void* dst);
extern void GXSetTevStages(u8 numStages);
extern void GXSetTevOrder(u32 stage, u32 texcoord, u32 texmap, u32 chan);
extern void GXSetTevColorIn(u32 stage, u32 a, u32 b, u32 c, u32 d);
extern void GXSetTevColorOp(u32 stage, u32 op, u32 bias, u32 scale,
                             u32 clamp, u32 out_reg);
extern void GXSetTevAlphaIn(u32 stage, u32 a, u32 b, u32 c, u32 d);
extern void GXSetTevAlphaOp(u32 stage, u32 op, u32 bias, u32 scale,
                             u32 clamp, u32 out_reg);
extern void GXSetTevSwapMode(u32 stage, u32 ras_sel, u32 tex_sel);
extern void GXSetTevSwapModeTable(u32 id, u32 r, u32 g, u32 b, u32 a);
extern void GXSetTevKColorSel(u32 stage, u32 sel);
extern void GXSetTevKAlphaSel(u32 stage, u32 sel);
extern void GXSetTevIndirect(u32 stage, u32 ind_stage, u32 format,
                              u32 bias_sel, u32 mtx_sel, u32 wrap_s,
                              u32 wrap_t, u32 add_prev, u32 utc_lod,
                              u32 alpha_sel);
extern void GXSetNumTevStages(u8 num);
/* hsdAllocMemPiece/hsdFreeMemPiece declared in hsd_class.h with s32 */

#define state_line_width lbl_8047B351
#define state_line_tex_offsets lbl_8047B350
#define state_point_size lbl_8047B350
#define state_cull_mode lbl_8047B34C
#define state_blend_type lbl_8047B348
#define state_src_factor lbl_8047B344
#define state_dst_factor lbl_8047B340
#define state_logic_op lbl_8047B33C
#define state_z_enable lbl_8047B338
#define state_z_func lbl_8047B334
#define state_z_update lbl_8047B330
#define state_alpha_comp0 lbl_8047B32C
#define state_alpha_ref0 lbl_8047B328
#define state_alpha_op lbl_8047B324
#define state_alpha_comp1 lbl_8047B320
#define state_alpha_ref1 lbl_8047B31E
#define state_color_update lbl_8047B31D
#define state_alpha_update lbl_8047B31C
#define state_enable_dst_alpha lbl_8047B31B
#define state_dst_alpha lbl_8047B31A
#define state_before_tex lbl_8047B319
#define state_dither lbl_8047B318

/* TEV state globals */
static u8 tev_num_stages;
static HSD_TevDesc tev_stages[16];

/* ========================================================================= */
/*  TEV stage management                                                     */
/* ========================================================================= */

/*
 * HSD_TevInit - 0x801B1730 | Size: 0x124
 * Initialize all TEV stages to default passthrough state.
 */
void fn_801B1730(void) {
    u32 i;

    tev_num_stages = 1;

    for (i = 0; i < 16; i++) {
        tev_stages[i].stage = i;
        tev_stages[i].texcoord = 0xFF; /* GX_TEXCOORD_NULL */
        tev_stages[i].texmap = 0xFF;   /* GX_TEXMAP_NULL */
        tev_stages[i].color_chan = 0xFF; /* GX_COLOR_NULL */

        tev_stages[i].color_a = 15; /* GX_CC_ZERO */
        tev_stages[i].color_b = 15;
        tev_stages[i].color_c = 15;
        tev_stages[i].color_d = 15;
        tev_stages[i].color_op = 0;   /* GX_TEV_ADD */
        tev_stages[i].color_bias = 0; /* GX_TB_ZERO */
        tev_stages[i].color_scale = 0; /* GX_CS_SCALE_1 */
        tev_stages[i].color_clamp = 1; /* TRUE */
        tev_stages[i].color_out_reg = 0; /* GX_TEVPREV */

        tev_stages[i].alpha_a = 7; /* GX_CA_ZERO */
        tev_stages[i].alpha_b = 7;
        tev_stages[i].alpha_c = 7;
        tev_stages[i].alpha_d = 7;
        tev_stages[i].alpha_op = 0;
        tev_stages[i].alpha_bias = 0;
        tev_stages[i].alpha_scale = 0;
        tev_stages[i].alpha_clamp = 1;
        tev_stages[i].alpha_out_reg = 0;
    }
}

/*
 * 0x801B1854 | Size: 0x30
 * Initialize TEV vtx desc list from BSS object.
 */
extern u8 lbl_804656E0[];
void fn_801B1854(void) {
    fn_801AA35C(lbl_804656E0, 0x28, 4);
}

/* Address: 0x801B1884 | Size: 0xC */
/* Get pointer to TEV vtx desc BSS object */
void* fn_801B1884(void) {
    return lbl_804656E0;
}

/*
 * HSD_TevSetColorInput - 0x801B1890 | Size: 0x48
 * Set color input selection for a TEV stage.
 */
void fn_801B1890(u32 stage, u32 a, u32 b, u32 c, u32 d) {
    if (stage < 16) {
        tev_stages[stage].color_a = a;
        tev_stages[stage].color_b = b;
        tev_stages[stage].color_c = c;
        tev_stages[stage].color_d = d;
    }
}

f32 fn_801B18D8(HSD_Spline* spl, f32 arg1)
{
    s32 idx = 0;
    f32 start = 0.0F;
    f32 end = 1.0F;
    f32 result;

    if (arg1 <= 0.0F) {
        return start;
    }

    if (arg1 >= 1.0F) {
        return end;
    }

    while (spl->segLength[idx + 1] < arg1) {
        idx++;
    }

    switch (spl->type) {
    case 0:
        result = (arg1 - spl->segLength[idx]) /
                 (spl->segLength[idx + 1] - spl->segLength[idx]);
        break;
    case 1:
    case 2:
    case 3: {
        f32 remain = spl->totalLength * (arg1 - spl->segLength[idx]);

        while (((start - end) < 0.0F ? -(start - end) : (start - end)) >=
               0.00001F)
        {
            f32 length;

            result = (start + end) * 0.5F;
            length = fn_801B1AD0(spl->segPoly[idx], start, result);
            if (remain < (0.00001F + length)) {
                end = result;
            } else {
                start = result;
                remain -= length;
            }
        }
        break;
    }
    }

    return (result + idx) / (spl->numcv - 1.0F);
}

f32 fn_801B1AD0(f32* coeffs, f32 start, f32 end)
{
    f32 dx = (end - start) / 8.0F;
    f32 t = start + dx;
    f32 sum = 0.0F;
    s32 i;

    for (i = 2; i <= 8; i++) {
        f32 t2 = t * t;
        f32 t3 = t2 * t;
        f32 t4 = t3 * t;
        f32 value = (coeffs[0] * t4) + (coeffs[1] * t3) +
                    (coeffs[2] * t2) + (coeffs[3] * t) + coeffs[4];

        if (value < 0.0F && value > -0.001F) {
            value = 0.0F;
        }

        if (i & 1) {
            sum += 2.0F * sqrtf(value);
        } else {
            sum += 4.0F * sqrtf(value);
        }
        t += dx;
    }

    {
        f32 t2 = start * start;
        f32 t3 = t2 * start;
        f32 t4 = t3 * start;
        f32 value0 = (coeffs[0] * t4) + (coeffs[1] * t3) +
                     (coeffs[2] * t2) + (coeffs[3] * start) + coeffs[4];
        f32 value1;

        if (value0 < 0.0F && value0 > -0.001F) {
            value0 = 0.0F;
        }

        t2 = end * end;
        t3 = t2 * end;
        t4 = t3 * end;
        value1 = (coeffs[0] * t4) + (coeffs[1] * t3) +
                 (coeffs[2] * t2) + (coeffs[3] * end) + coeffs[4];
        if (value1 < 0.0F && value1 > -0.001F) {
            value1 = 0.0F;
        }

        return (dx * ((sum + sqrtf(value0)) + sqrtf(value1))) / 3.0F;
    }
}

void fn_801B2038(Vec3* p, HSD_Spline* spline, f32 u)
{
    Vec3* cp;
    s16 idx;

    if (u < 0.0F || u > 1.0F) {
        return;
    }

    if (u < 1.0F) {
        f32 t = u * (spline->numcv - 1);
        idx = t;
        t -= idx;
        switch (spline->type) {
        case 0:
            cp = &spline->cv[idx];
            p->x = (t * (cp[1].x - cp[0].x)) + cp[0].x;
            p->y = (t * (cp[1].y - cp[0].y)) + cp[0].y;
            p->z = (t * (cp[1].z - cp[0].z)) + cp[0].z;
            return;
        case 1: {
            f32 u_1 = 1.0F - t;
            f32 u2 = t * t;
            f32 u_12 = u_1 * u_1;
            f32 bez0 = u_12 * u_1;
            f32 bez1 = 3.0F * t * u_12;
            f32 bez2 = 3.0F * u2 * u_1;
            f32 bez3 = u2 * t;

            cp = &spline->cv[idx * 3];
            p->x = (cp[0].x * bez0) + (cp[1].x * bez1) +
                   (cp[2].x * bez2) + (cp[3].x * bez3);
            p->y = (cp[0].y * bez0) + (cp[1].y * bez1) +
                   (cp[2].y * bez2) + (cp[3].y * bez3);
            p->z = (cp[0].z * bez0) + (cp[1].z * bez1) +
                   (cp[2].z * bez2) + (cp[3].z * bez3);
            return;
        }
        case 2: {
            f32 u2 = t * t;
            f32 u3 = u2 * t;
            f32 u_1 = 1.0F - t;
            f32 k1_6 = 1.0F / 6.0F;
            f32 b0 = k1_6 * u_1 * u_1 * u_1;
            f32 b1 = k1_6 * (4.0F + ((3.0F * u3) - (6.0F * u2)));
            f32 b2 = k1_6 * ((3.0F * ((-u3 + u2) + t)) + 1.0F);
            f32 b3 = k1_6 * u3;

            cp = &spline->cv[idx];
            p->x = (cp[0].x * b0) + (cp[1].x * b1) + (cp[2].x * b2) +
                   (cp[3].x * b3);
            p->y = (cp[0].y * b0) + (cp[1].y * b1) + (cp[2].y * b2) +
                   (cp[3].y * b3);
            p->z = (cp[0].z * b0) + (cp[1].z * b1) + (cp[2].z * b2) +
                   (cp[3].z * b3);
            return;
        }
        case 3: {
            f32 u2 = t * t;
            f32 u3 = u2 * t;
            f32 car0 = spline->tension * ((-u3 + (2.0F * u2)) - t);
            f32 car1 = (((2.0F - spline->tension) * u3) +
                         ((spline->tension - 3.0F) * u2)) +
                        1.0F;
            f32 car2 = (((spline->tension - 2.0F) * u3) +
                         ((3.0F - (2.0F * spline->tension)) * u2)) +
                        (spline->tension * t);
            f32 car3 = spline->tension * (u3 - u2);

            cp = &spline->cv[idx];
            p->x = (cp[0].x * car0) + (cp[1].x * car1) +
                   (cp[2].x * car2) + (cp[3].x * car3);
            p->y = (cp[0].y * car0) + (cp[1].y * car1) +
                   (cp[2].y * car2) + (cp[3].y * car3);
            p->z = (cp[0].z * car0) + (cp[1].z * car1) +
                   (cp[2].z * car2) + (cp[3].z * car3);
            return;
        }
        }
    } else {
        idx = spline->numcv - 1;
        switch (spline->type) {
        case 0:
            *p = spline->cv[idx];
            return;
        case 1:
            *p = spline->cv[idx * 3];
            return;
        case 2:
            cp = &spline->cv[idx] - 1;
            {
                f32 b0 = 0.0F;
                f32 b1 = 1.0F / 6.0F;
                f32 b2 = 4.0F / 6.0F;
                f32 b3 = 1.0F / 6.0F;

                p->x = (cp[0].x * b0) + (cp[1].x * b1) +
                       (cp[2].x * b2) + (cp[3].x * b3);
                p->y = (cp[0].y * b0) + (cp[1].y * b1) +
                       (cp[2].y * b2) + (cp[3].y * b3);
                p->z = (cp[0].z * b0) + (cp[1].z * b1) +
                       (cp[2].z * b2) + (cp[3].z * b3);
            }
            return;
        case 3:
            cp = &spline->cv[idx];
            *p = cp[1];
            return;
        }
    }
}

/* ========================================================================= */
/*  TEV state accessors                                                      */
/* ========================================================================= */

f32 fn_801B2560(f32 fterm, f32 time, f32 p0, f32 p1, f32 d0, f32 d1)
{
    f32 _3t2_T2;
    f32 _2t3_T3;
    f32 t3_T2;
    f32 t2_T;
    f32 t2;
    f32 _1_T2;

    _1_T2 = time * time;
    t2 = fterm * fterm;
    t2_T = _1_T2 * fterm;
    t3_T2 = t2 * (_1_T2 * time);
    _2t3_T3 = 2.0F * t3_T2 * fterm;
    _3t2_T2 = 3.0F * _1_T2 * t2;

    return (d1 * (t3_T2 - t2_T)) +
           ((d0 * (time + ((t3_T2 - t2_T) - t2_T))) +
            ((p0 * (1.0F + (_2t3_T3 - _3t2_T2))) +
             (p1 * (-_2t3_T3 + _3t2_T2))));
}

void fn_801B25C4(u32 mask) {
    s32 i;

    for (i = 0; lbl_8036CFA8[i].mask != 0; i++) {
        if (mask & lbl_8036CFA8[i].mask) {
            lbl_8036CFA8[i].func();
        }
    }
}

static u32 color_reg_used;

void fn_801B2654(void) {
    state_blend_type = -1;
    state_src_factor = -1;
    state_dst_factor = -1;
    state_logic_op = -1;
    state_z_enable = 0xFF;
    state_z_func = -1;
    state_z_update = 0xFF;
    state_alpha_comp0 = -1;
    state_alpha_ref0 = 0;
    state_alpha_op = -1;
    state_alpha_comp1 = -1;
    state_alpha_ref1 = 0;
    state_color_update = 0xFF;
    state_alpha_update = 0xFF;
    state_enable_dst_alpha = 0xFF;
    state_dst_alpha = 0;
    state_before_tex = 0xFF;
    state_dither = 0xFF;
}

/* Address: 0x801B26F8 | Size: 0x20 */
/* Wrapper calling fn_801ACD7C (display list dispatch) */
extern void fn_801ACD7C(void);
void fn_801B26F8(void) {
    fn_801ACD7C();
}

/* Address: 0x801B2718 | Size: 0x24 */
/* Reset TEV state SDA variables */
extern u8 lbl_8047B351;
extern u8 lbl_8047B350;
extern s32 lbl_8047B34C;
#pragma push
#pragma optimization_level 1
void fn_801B2718(void) {
    lbl_8047B351 = 0;
    lbl_8047B350 = 0;
    lbl_8047B34C = -1;
    lbl_8047B351 = 0xFF;
}
#pragma pop

/*
 * HSD_TevLookupReg - 0x801B273C | Size: 0x50
 * Look up which TEV register a given value is stored in.
 */
u32 fn_801B273C(u32 reg) {
    if (reg < 4) {
        return reg;
    }
    return 0xFF; /* invalid */
}

/*
 * HSD_TevAssignReg - 0x801B278C | Size: 0x50
 * Assign a value to a specific TEV register.
 */
void fn_801B278C(u32 reg, u32 value) {
    if (reg < 4) {
        /* Store the register assignment */
        color_reg_used |= (1 << reg);
    }
}

void fn_801B27DC(u32 enable, u32 func, u32 update) {
    enable = enable ? 1 : 0;
    update = update ? 1 : 0;

    if (state_z_enable != enable || state_z_func != func ||
        state_z_update != update)
    {
        fn_800BCE88(enable, func, update);
        state_z_enable = enable;
        state_z_func = func;
        state_z_update = update;
    }
}

void fn_801B2878(s32 mode) {
    if (state_cull_mode != mode) {
        fn_800B94F0(mode);
        state_cull_mode = mode;
    }
}

/* Address: 0x801B28B8 | Size: 0x10 */
/* Store float into BSS object at offset 0x10 */
void fn_801B28B8(f32 val) {
    *(f32*)(lbl_80465710 + 0x10) = val;
}

void fn_801B28C8(u32* ambient, u32* diffuse, u32* specular, f32 alpha) {
    HSD_MatState* matstate = (HSD_MatState*) lbl_80465710;

    matstate->ambient = *ambient;
    matstate->diffuse = *diffuse;
    matstate->specular = *specular;

    if (alpha <= lbl_8047DE50) {
        alpha = lbl_8047DE50;
    } else if (alpha >= lbl_8047DE54) {
        alpha = lbl_8047DE54;
    }
    matstate->alpha = lbl_8047DE58 * alpha;
}

void fn_801B294C(rendermode, pe)
u32 rendermode;
HSD_PEDesc* pe;
{
    HSD_TevSetupDesc tevdesc;

    if (fn_801B387C() == 0) {
        tevdesc.flags = 0;
        tevdesc.stage = HSD_StateAssignTev();
        tevdesc.coord = 0xFF;
        tevdesc.map = 0xFF;
        tevdesc.color = 4;
        tevdesc.u.tevop.tevmode = 4;
        fn_801B3638(&tevdesc);
    }
    fn_801B29E4(rendermode, pe);
    fn_801B3258();
    fn_801B3770();
    fn_801B3890();
    fn_801B2F1C(rendermode);
}

/* ========================================================================= */
/*  TEV expression compilation                                               */
/* ========================================================================= */

void fn_801B29E4(flags, pe)
u32 flags;
HSD_PEDesc* pe;
{
    u32 value;
    u32 value2;
    u32 value3;
    u32 value4;
    u32 value5;

    if (pe != NULL) {
        value = pe->flags & 1;
        if (state_color_update != value) {
            fn_800BCE30(value);
            state_color_update = value;
        }

        value = (pe->flags & 2) ? 1 : 0;
        if (state_alpha_update != value) {
            fn_800BCE5C(value);
            state_alpha_update = value;
        }

        value = (pe->flags & 4) ? 1 : 0;
        value2 = pe->dst_alpha;
        if (state_enable_dst_alpha != value || state_dst_alpha != value2) {
            fn_800BD008(value, value2);
            state_enable_dst_alpha = value;
            state_dst_alpha = value2;
        }

        value = pe->type;
        value2 = pe->src_factor;
        value3 = pe->dst_factor;
        value4 = pe->logic_op;
        if (state_blend_type != value || state_src_factor != value2 ||
            state_dst_factor != value3 || state_logic_op != value4)
        {
            fn_800BCDDC(value, value2, value3, value4);
            state_blend_type = value;
            state_src_factor = value2;
            state_dst_factor = value3;
            state_logic_op = value4;
        }

        value = (pe->flags & 0x10) ? 1 : 0;
        value2 = pe->z_comp;
        value3 = (pe->flags & 0x20) ? 1 : 0;
        if (state_z_enable != value || state_z_func != value2 ||
            state_z_update != value3)
        {
            fn_800BCE88(value, value2, value3);
            state_z_enable = value;
            state_z_func = value2;
            state_z_update = value3;
        }

        value = (pe->flags & 8) ? 1 : 0;
        if (state_before_tex != value) {
            fn_800BCEBC(value);
            state_before_tex = value;
        }

        value = pe->alpha_comp0;
        value2 = pe->ref0;
        value3 = pe->alpha_op;
        value4 = pe->alpha_comp1;
        value5 = pe->ref1;
        if (state_alpha_comp0 != value || state_alpha_ref0 != value2 ||
            state_alpha_op != value3 || state_alpha_comp1 != value4 ||
            state_alpha_ref1 != value5)
        {
            fn_800BC618(value, value2, value3, value4, value5);
            state_alpha_comp0 = value;
            state_alpha_ref0 = value2;
            state_alpha_op = value3;
            state_alpha_comp1 = value4;
            state_alpha_ref1 = value5;
        }

        value = (pe->flags & 0x40) ? 1 : 0;
        if (state_dither != value) {
            fn_800BCFDC(value);
            state_dither = value;
        }
    } else {
        value = 1;
        if (state_color_update != value) {
            fn_800BCE30(value);
            state_color_update = value;
        }

        value = 0;
        if (state_alpha_update != value) {
            fn_800BCE5C(value);
            state_alpha_update = value;
        }

        if (state_enable_dst_alpha != 0 || state_dst_alpha != 0) {
            fn_800BD008(0, 0);
            state_enable_dst_alpha = 0;
            state_dst_alpha = 0;
        }

        value = (flags & 0x40000000) ? 1 : 0;
        if (state_blend_type != value || state_src_factor != 4 ||
            state_dst_factor != 5 || state_logic_op != 0xF)
        {
            fn_800BCDDC(value, 4, 5, 0xF);
            state_blend_type = value;
            state_src_factor = 4;
            state_dst_factor = 5;
            state_logic_op = 0xF;
        }

        value = (flags & 0x20000000) ? 0 : 1;
        value2 = (flags & 0x08000000) ? 7 : 3;
        value3 = 1;
        if (state_z_enable != value3 || state_z_func != value2 ||
            state_z_update != value)
        {
            fn_800BCE88(value3, value2, value);
            state_z_enable = value3;
            state_z_func = value2;
            state_z_update = value;
        }

        if (!(flags & 0x20000000) && (flags & 0x40000000)) {
            if (state_before_tex != 0) {
                fn_800BCEBC(0);
                state_before_tex = 0;
            }
            if (state_alpha_comp0 != 4 || state_alpha_ref0 != 0 ||
                state_alpha_op != 0 || state_alpha_comp1 != 4 ||
                state_alpha_ref1 != 0)
            {
                fn_800BC618(4, 0, 0, 4, 0);
                state_alpha_comp0 = 4;
                state_alpha_ref0 = 0;
                state_alpha_op = 0;
                state_alpha_comp1 = 4;
                state_alpha_ref1 = 0;
            }
        } else {
            if (state_before_tex != 1) {
                fn_800BCEBC(1);
                state_before_tex = 1;
            }
            if (state_alpha_comp0 != 7 || state_alpha_ref0 != 0 ||
                state_alpha_op != 0 || state_alpha_comp1 != 7 ||
                state_alpha_ref1 != 0)
            {
                fn_800BC618(7, 0, 0, 7, 0);
                state_alpha_comp0 = 7;
                state_alpha_ref0 = 0;
                state_alpha_op = 0;
                state_alpha_comp1 = 7;
                state_alpha_ref1 = 0;
            }
        }

        if (state_dither != 0) {
            fn_800BCFDC(0);
            state_dither = 0;
        }
    }
}

void fn_801B2F1C(rendermode)
u32 rendermode;
{
    HSD_ChanLocal* channel = lbl_8036CE88;
    HSD_LObjMini* lobj;
    s32 specular = 0;
    s32 alpha = 0;
    s32 chan_mode;
    s32 amb_mode;

    chan_mode = rendermode & 3;
    if (chan_mode == 0) {
        chan_mode = 1;
    }

    amb_mode = rendermode & 0x6000;
    if (amb_mode == 0) {
        amb_mode = chan_mode << 0xD;
    }

    if (rendermode & 8) {
        s32 i;
        s32 nb_active;

        channel[0].light_mask = HSD_LObjGetLightMaskSpecular();
        fn_801B3D1C(&channel[0]);
        specular = 1;
        nb_active = HSD_LObjGetNbActive();
        for (i = 0; i < nb_active; i++) {
            lobj = HSD_LObjGetActiveByIndex(i);
            if (lobj != NULL) {
                GXColor color = lobj->color;
                fn_801A6098(lobj, &color, ((HSD_MatState*) lbl_80465710)->shininess);
            }
        }
    }

    if (rendermode & 4) {
        u8 a;

        lobj = HSD_LObjGetActiveByID(0x100);
        if (lobj != NULL && (lobj->flags & 4)) {
            fn_801BF16C(lbl_80465710, &lobj->color, &channel[1].amb_color);
        } else {
            channel[1].amb_color = *(GXColor*) &lbl_80478C98;
        }

        channel[1].mat_src = (chan_mode & 2) ? 1 : 0;
        channel[1].light_mask = HSD_LObjGetLightMaskDiffuse();
        fn_801B3D1C(&channel[1]);

        if (amb_mode & 0x4000) {
            channel[2].chan = 3;
            alpha = 1;
            fn_801B3D1C(&channel[3]);
        } else {
            channel[2].chan = 2;
        }

        channel[2].light_mask = HSD_LObjGetLightMaskAlpha();
        if (lobj != NULL && (lobj->flags & 0x10)) {
            a = lobj->color.a;
        } else {
            a = 0;
        }

        if (channel[2].light_mask != 0) {
            channel[2].enable = 1;
            channel[2].mat_color.a = 0xFF;
            channel[2].amb_color.a = a;
        } else {
            channel[2].mat_color.a = a;
            channel[2].enable = 0;
        }
        fn_801B3D1C(&channel[2]);
    } else {
        channel[4].mat_src = (chan_mode & 2) ? 1 : 0;
        fn_801B3D1C(&channel[4]);
        channel[5].mat_src = (amb_mode & 0x4000) ? 1 : 0;
        fn_801B3D1C(&channel[5]);
    }

    if (specular != 0) {
        if (alpha == 0) {
            fn_801B3AE8(3);
        }
        fn_801B3AA8(2);
    } else if (alpha != 0) {
        fn_801B3AE8(1);
        fn_801B3AA8(2);
    } else {
        fn_801B3AE8(5);
        fn_801B3AA8(1);
    }
}

/* Address: 0x801B3168 | Size: 0xC */
/* Clear TEV stage count SDA variable */
void fn_801B3168(void) {
    lbl_8047B358 = 0;
}

/*
 * 0x801B3174 | Size: 0x30
 * Clear field 0x8 of 4 entries in rodata struct array.
 */
#pragma push
#pragma optimization_level 2
void fn_801B3174(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        lbl_8036CFE8[i].dirty = 0;
    }
}
#pragma pop

/*
 * 0x801B31A4 | Size: 0x50
 * Reset TEV stages - release each stage then clear globals.
 */
extern void fn_800BBC34(s32 stage);
extern void fn_800BBC0C(s32 val);
#pragma push
#pragma optimization_level 2
void fn_801B31A4(void) {
    s32 i;
    for (i = 0; i < 16; i++) {
        fn_800BBC34(i);
    }
    fn_800BBC0C(0);
    lbl_8047B370 = 0;
}
#pragma pop

void fn_801B31F4(void)
{
    memcpy(lbl_8036D018, lbl_8036D0D8, 0xC0);
    lbl_8047B368[0] = 1;
    lbl_8047B368[1] = 1;
    lbl_8047B360[0] = 1;
    lbl_8047B360[1] = 1;
    lbl_8047B35C = -1;
}

HSD_TExp* fn_801B3258()
{
    u32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_8036CFE8[i].dirty != 0) {
            s32 reg;
            GXColorS10 color = lbl_8036CFE8[i].color;

            switch (i) {
            case 0:
                reg = 1;
                break;
            case 1:
                reg = 2;
                break;
            case 2:
                reg = 3;
                break;
            case 3:
                reg = 0;
                break;
            default:
                reg = 1;
                break;
            }

            fn_800BC36C(reg, color);
            lbl_8036CFE8[i].dirty = 0;
        }
    }
}

s32 HSD_Index2TevStage(idx)
s32 idx;
{
    switch (idx) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    case 8:
        return 8;
    case 9:
        return 9;
    case 10:
        return 10;
    case 11:
        return 11;
    case 12:
        return 12;
    case 13:
        return 13;
    case 14:
        return 14;
    case 15:
        return 15;
    default:
        HSD_ASSERT(709, 0);
        return 15;
    }
}

int fn_801B3408(desc)
HSD_TevSetupDesc* desc;
{
    s32 num = 0;
    HSD_TevSetupDesc* td;

    for (td = desc; td != NULL; td = td->next) {
        s32 tmp;

        switch (td->stage) {
        case 0:
            tmp = 1;
            break;
        case 1:
            tmp = 2;
            break;
        case 2:
            tmp = 3;
            break;
        case 3:
            tmp = 4;
            break;
        case 4:
            tmp = 5;
            break;
        case 5:
            tmp = 6;
            break;
        case 6:
            tmp = 7;
            break;
        case 7:
            tmp = 8;
            break;
        case 8:
            tmp = 9;
            break;
        case 9:
            tmp = 10;
            break;
        case 10:
            tmp = 11;
            break;
        case 11:
            tmp = 12;
            break;
        case 12:
            tmp = 13;
            break;
        case 13:
            tmp = 14;
            break;
        case 14:
            tmp = 15;
            break;
        case 15:
            tmp = 16;
            break;
        default:
            HSD_ASSERT(0x37A, 0);
            tmp = 0;
            break;
        }

        if (tmp > num) {
            num = tmp;
        }

        fn_800BC6F0(td->stage, td->coord, td->map, td->color);
        if (td->flags == 0) {
            fn_800BC114(td->stage, td->u.tevop.tevmode);
            fn_800BC52C(td->stage, 0, 0);
        } else {
            fn_800BC228(td->stage, td->u.tevconf.clr_op,
                        td->u.tevconf.clr_bias, td->u.tevconf.clr_scale,
                        td->u.tevconf.clr_clamp, td->u.tevconf.clr_out_reg);
            fn_800BC1A0(td->stage, td->u.tevconf.clr_a,
                        td->u.tevconf.clr_b, td->u.tevconf.clr_c,
                        td->u.tevconf.clr_d);
            fn_800BC290(td->stage, td->u.tevconf.alpha_op,
                        td->u.tevconf.alpha_bias,
                        td->u.tevconf.alpha_scale,
                        td->u.tevconf.alpha_clamp,
                        td->u.tevconf.alpha_out_reg);
            fn_800BC1E4(td->stage, td->u.tevconf.alpha_a,
                        td->u.tevconf.alpha_b, td->u.tevconf.alpha_c,
                        td->u.tevconf.alpha_d);
            fn_800BC580(td->u.tevconf.ras_swap, td->u.tevconf.swap_r,
                        td->u.tevconf.swap_g, td->u.tevconf.swap_b,
                        td->u.tevconf.swap_a);
            if (td->u.tevconf.tex_swap != td->u.tevconf.ras_swap) {
                fn_800BC580(td->u.tevconf.tex_swap, td->u.tevconf.swap_r,
                            td->u.tevconf.swap_g, td->u.tevconf.swap_b,
                            td->u.tevconf.swap_a);
            }
            fn_800BC52C(td->stage, td->u.tevconf.ras_swap,
                        td->u.tevconf.tex_swap);
            fn_800BC454(td->stage, td->u.tevconf.kcsel);
            fn_800BC4C0(td->stage, td->u.tevconf.kasel);
        }
    }

    lbl_8047B370 = num;
    fn_800BC8C8(lbl_8047B370);
    lbl_8047B370 = 0;
}

void fn_801B3638(desc)
HSD_TevSetupDesc* desc;
{
    fn_800BC6F0(desc->stage, desc->coord, desc->map, desc->color);
    if (desc->flags == 0) {
        fn_800BC114(desc->stage, desc->u.tevop.tevmode);
        fn_800BC52C(desc->stage, 0, 0);
    } else {
        fn_800BC228(desc->stage, desc->u.tevconf.clr_op,
                    desc->u.tevconf.clr_bias, desc->u.tevconf.clr_scale,
                    desc->u.tevconf.clr_clamp, desc->u.tevconf.clr_out_reg);
        fn_800BC1A0(desc->stage, desc->u.tevconf.clr_a,
                    desc->u.tevconf.clr_b, desc->u.tevconf.clr_c,
                    desc->u.tevconf.clr_d);
        fn_800BC290(desc->stage, desc->u.tevconf.alpha_op,
                    desc->u.tevconf.alpha_bias,
                    desc->u.tevconf.alpha_scale,
                    desc->u.tevconf.alpha_clamp,
                    desc->u.tevconf.alpha_out_reg);
        fn_800BC1E4(desc->stage, desc->u.tevconf.alpha_a,
                    desc->u.tevconf.alpha_b, desc->u.tevconf.alpha_c,
                    desc->u.tevconf.alpha_d);
        fn_800BC580(desc->u.tevconf.ras_swap, desc->u.tevconf.swap_r,
                    desc->u.tevconf.swap_g, desc->u.tevconf.swap_b,
                    desc->u.tevconf.swap_a);
        if (*(volatile s32*) &desc->u.tevconf.tex_swap != desc->u.tevconf.ras_swap) {
            fn_800BC580(desc->u.tevconf.tex_swap, desc->u.tevconf.swap_r,
                        desc->u.tevconf.swap_g, desc->u.tevconf.swap_b,
                        desc->u.tevconf.swap_a);
        }
        fn_800BC52C(desc->stage, desc->u.tevconf.ras_swap,
                    desc->u.tevconf.tex_swap);
        fn_800BC454(desc->stage, desc->u.tevconf.kcsel);
        fn_800BC4C0(desc->stage, desc->u.tevconf.kasel);
    }
}

void fn_801B3770()
{
    fn_800BC8C8(lbl_8047B370);
    lbl_8047B370 = 0;
}

int HSD_StateAssignTev()
{
    return HSD_Index2TevStage(lbl_8047B370++);
}

/* NOTE: fn_801B387C (Size: 0x8) is already decompiled in another file */

/* Address: 0x801B3884 | Size: 0xC */
/* Clear TEV expression list SDA global */
void fn_801B3884(void) {
    lbl_8047B370 = 0;
}

/*
 * HSD_TExpFreeNode - 0x801B3890 | Size: 0x30
 * Release TEV stage count from SDA variable.
 */
extern void fn_800B884C(u8 count);
void fn_801B3890(void) {
    u8 count = (u8)lbl_8047B358;
    fn_800B884C(count);
    lbl_8047B358 = 0;
}

/*
 * HSD_TExpFreeAll - 0x801B38C0 | Size: 0xD8
 * Free all nodes in a TExp tree recursively.
 */
void HSD_StateRegisterTexGen(HSD_TExp* exp) {
    u32 i;

    if (exp == NULL) {
        return;
    }

    /* Free children first */
    if (exp->type == HSD_TE_ALL) {
        for (i = 0; i < 4; i++) {
            if (exp->arg[i] != NULL) {
                HSD_StateRegisterTexGen(exp->arg[i]);
                exp->arg[i] = NULL;
            }
        }
    }

    /* Free the linked list */
    if (exp->next != NULL) {
        HSD_StateRegisterTexGen(exp->next);
        exp->next = NULL;
    }

    hsdFreeMemPiece(exp, sizeof(HSD_TExp));
}

int fn_801B3998(channel)
HSD_ChanLocal* channel;
{
    s32 num = 0;
    HSD_ChanLocal* ch;

    for (ch = channel; ch != NULL; ch = ch->next) {
        s32 tmp;

        switch (ch->chan) {
        case 0:
            tmp = 1;
            break;
        case 1:
            tmp = 2;
            break;
        case 2:
            tmp = 1;
            break;
        case 3:
            tmp = 2;
            break;
        case 4:
            tmp = 1;
            break;
        case 5:
            tmp = 2;
            break;
        case 0xFF:
            tmp = 0;
            break;
        default:
            HSD_ASSERT(0x2F1, 0);
            tmp = 0;
            break;
        }

        if (tmp > num) {
            num = tmp;
        }
        fn_801B3D1C(ch);
    }

    if (lbl_8047B35C != (u8) num) {
        fn_800BA6B0((u8) num);
        lbl_8047B35C = (u8) num;
    }
}

void fn_801B3AA8(num)
s32 num;
{
    if (lbl_8047B35C != num) {
        fn_800BA6B0((u8)num);
        lbl_8047B35C = num;
    }
}

/*
 * HSD_TExpBuildAlphaExpr - 0x801B3AE8 | Size: 0x234
 * Build a TExp alpha expression from a TObj chain.
 * Similar to color expression builder but for the alpha channel.
 */
HSD_TExp* fn_801B3AE8(HSD_TObj* tobj, HSD_TExp* list) {
    HSD_TExp* root = NULL;
    HSD_TExp* tex_node;
    HSD_TObj* t;

    for (t = tobj; t != NULL; t = t->next) {
        u32 alphamap = tobj_alphamap(t);

        if (alphamap == TEX_ALPHAMAP_NONE) {
            continue;
        }

        tex_node = (HSD_TExp*)hsdAllocMemPiece(sizeof(HSD_TExp));
        if (tex_node == NULL) {
            break;
        }
        tex_node->type = HSD_TE_TEX;
        tex_node->data = t;
        tex_node->next = NULL;

        /* Combine based on alphamap mode */
        switch (alphamap) {
        case TEX_ALPHAMAP_BLEND:
            if (root != NULL) {
                root = fn_801B3258(1 /* BLEND */, root, tex_node);
            } else {
                root = tex_node;
            }
            break;

        case TEX_ALPHAMAP_MODULATE:
            if (root != NULL) {
                root = fn_801B3258(2 /* MUL */, root, tex_node);
            } else {
                root = tex_node;
            }
            break;

        case TEX_ALPHAMAP_REPLACE:
            root = tex_node;
            break;

        case TEX_ALPHAMAP_ADD:
            if (root != NULL) {
                root = fn_801B3258(0 /* ADD */, root, tex_node);
            } else {
                root = tex_node;
            }
            break;

        case TEX_ALPHAMAP_SUB:
            if (root != NULL) {
                root = fn_801B3258(3 /* SUB */, root, tex_node);
            } else {
                root = tex_node;
            }
            break;

        default:
            root = tex_node;
            break;
        }
    }

    return root;
}

/*
 * HSD_TExpCompileMaterial - 0x801B3D1C | Size: 0x524
 * Full material expression compile.
 * Takes the TObj chain from a material and generates the complete
 * TEV stage configuration. This is the top-level entry point for
 * the texture expression compilation system.
 */
void fn_801B3D1C(HSD_TObj* tobj, u32 render_mode) {
    HSD_TExp* color_expr;
    HSD_TExp* alpha_expr;
    HSD_TExp* list = NULL;
    u32 num_stages;

    /* Build expression trees */
    color_expr = (HSD_TExp*)fn_801B3998((int)tobj);
    alpha_expr = fn_801B3AE8(tobj, list);

    /* Simplify expressions */
    color_expr = (HSD_TExp*)fn_801B3408((int)color_expr);
    alpha_expr = (HSD_TExp*)fn_801B3408((int)alpha_expr);

    /* Reset TEV state */
    color_reg_used = 0;
    fn_801B1730();

    /* Compile color expression into TEV stages */
    num_stages = 0;
    fn_801B29E4(color_expr, &num_stages, 0);

    /* Compile alpha expression into same stages */
    fn_801B2F1C(alpha_expr, NULL, 0);

    /* Set final TEV stage count */
    if (num_stages > 0) {
        fn_801B1854();
    }

    /* Free expression trees */
    HSD_StateRegisterTexGen(color_expr);
    HSD_StateRegisterTexGen(alpha_expr);
}
