/**
 * @file gs_thread_hi_range_800FE35C.c
 * @brief GSthread (upper half) -- sprite-environment / rotation block.
 *
 * Address range: 0x800FE35C - 0x800FE6DC (6 functions).
 * XD class: game/pxdvs/GSAPI/GSmsg/sprite.cpp (tentative) / GSmsg.cpp
 * tail (tentative). Structural evidence (calls to tan() +
 * set__5GSvecFfff, i.e. sprite-environment/rotation math) suggests
 * sprite.cpp, but sizes don't 1:1 match XD's sprite.cpp functions, so a
 * `_range_` filename is used pending confirmation. Contains
 * spriteSetEnv (spriteSetEnv).
 *
 * Split out of the former monolithic game/gs_thread_hi.c
 * (0x800F8268-0x800FF0A0 per config/GC6E01/splits.txt).
 */
#include "dolphin/types.h"
#include "game/gs_thread.h"


/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const void* fmt, ...);          /* OSReport */
extern u16   GSmemAllocRaw(u32 size);                    /* _toolentryAlloc__FUl */
extern void* GSmemGetPtr(u16 handle);                    /* fn_800E27B0 */
extern void* GSmemLock(u16 handle);                      /* fn_800E24B0 */
extern void  GSmemFree(u16 handle);                      /* fn_800E209C */
extern u16   GSmemAlloc(u32 alignment, u32 size);        /* fn_800E2C04 */
extern void  OSSetIdleFunction(void* func, void* arg,
                          void* stackTop, u32 stackSize); /* OSCreateFiber-like */
extern void  OSDisableInterrupts(void);
extern void  OSRestoreInterrupts(void);
extern void  fn_800D30A0(void* callback);                 /* GSgfx register swap callback */
extern void  threadSaveGPRRegisters(void);                /* GSthread context init */
extern void  threadSaveFPRRegisters(void);                           /* GSthread FPU context init */
/* renamed symbols referenced by asm incs (symbolmap port) */
extern void GSscratchFree(void*);
extern void cos();   /* MSL trig (renamed fn_800CDBE0) - referenced by asm incs */

/* ===== String constants (rodata references) ===== */
extern const char lbl_80271008[]; /* "GSthreadCreate. Warning: 'usesFPU==FALE' OK?\n" */

/* ===== Forward declarations for internal functions ===== */
extern void gappVSyncCallback(void);            /* GStaskSwapCallback */
extern void fn_800F0F4C(u32 arg);          /* GSthread trampoline / entry wrapper */
extern void fn_800AB150(void* buf);
extern u32 fn_800D0F44(u32 buttonIdx);
extern void fn_800AB4FC(void*);
extern void fn_800E209C(u16 handle);
extern void* fn_800E24B0(u16 handle);
extern void* fn_800E27B0(u16 handle);
extern u16 fn_800E2C04(u32 alignment, u32 size);
extern u16 _toolentryAlloc__FUl(u32 size);
extern void fn_80080ED8(void);
extern void fn_800DBEB4(u32 a, void* b);
extern void fn_800D5CB8(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void fn_800D61E4(s16 x, s16 y);
extern void fn_800D6728(void);
extern void fn_800D67BC(s32 a);
extern void fn_800D6A00(s32 a);
extern void fn_800D7820(void* ptr);
extern void fn_800D85D4(void);
extern void fn_800D888C(u32 mask);
extern void fn_800D88DC(u32 mask);
extern void fn_800D9ED8(void);
extern void fn_800DC1D4(s32 a);
extern void logVsnprintf_float(void);
extern void GStextureUnlockImage(void* ctx);
extern void GStextureLockImage(void);
extern void fn_801669BC(u32 type);
extern void* GStextureCreate(u16 width, u16 height, u32 format, u32 tlutFormat, u8 mipLevels);
extern void fn_800CDBE0(void);
extern u32 fn_800D3088(void);
extern void fn_800DBF78(void);
extern void fn_800DBFD4(void);
extern void fn_800DC04C(void);
extern void fn_800DC0D4(void);
extern void fn_800DC14C(void);
extern void fn_800DC224(void);
extern void windowDrawSprite(void);
extern void fn_80166A28(void);
extern void fn_800D59B8(void);
extern void fn_800D5BA0(void);
extern void fn_800D9D68(u16 a, u16 b, u16 c, u16 d);
extern f64 tan(f64 angle);
extern void fn_800D7FE4(void* mtx);
extern void fn_800D834C(void);
extern void fn_800D9BD0(f32 a, f32 b, f32 c, f32 d);
extern void fn_800DA028(s32 a);
extern void fn_800DA100(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void fn_800DA1E8(s32 a, s32 b, s32 c);
extern void fn_800DA2BC(s32 a, s32 b, s32 c);
extern void fn_800DA4C4(s32 a, s32 b, s32 c);
extern void set__5GSvecFfff(void* dst, f32 x, f32 y, f32 z);
extern void fn_800E0218(void* dst, void* a, void* b, void* c);
extern void* memset(void* dest, int val, u32 n);
extern void* memcpy(void* dst, const void* src, u32 n);

/* ===== BSS/SDA symbol externs (for asm{} blocks) ===== */
/* BSS/data/rodata symbols accessed via lis/@ha + addi/@l pairs */
extern u32 lbl_80401C10;
/* .bss symbols */
extern u8  lbl_80401DE0[];
extern u8  lbl_80401E48[];
extern u8  lbl_80402418[];
extern u8  lbl_80402480[];
extern u8  lbl_804024E8[];
/* .data symbols */
extern u8  lbl_80314E08[];
extern u8  lbl_80314F98[];
extern u8  lbl_80315678[];
/* .rodata symbols */
extern u8  lbl_80271300[];
extern u8  lbl_80271500[];
extern u8  lbl_80271700[];
extern u8  lbl_80271730[];
extern u8  lbl_80271754[];
extern u8  lbl_8027177C[];
extern u8  lbl_802717B4[];
extern u8  lbl_802717D4[];
/* .sdata symbol */
extern float lbl_80478AC0;
/* sdata2 (r2) float/double constants used in asm blocks */
extern f64 lbl_8047CCC8;  /* f64 */
extern f32 lbl_8047CCD0;  /* f32 */
extern f32 lbl_8047CCD4;  /* f32 */
extern f32 lbl_8047CCD8;  /* f32 */
extern f32 lbl_8047CCDC;  /* f32 */
extern f64 lbl_8047CCE0;  /* f64 */
extern f64 lbl_8047CCE8;  /* f64 */
extern f64 lbl_8047CCF0;  /* f64 */
extern f64 lbl_8047CCF8;  /* f64 */
extern u32 lbl_8047CD00;  /* u32 (lwz) */
extern u32 lbl_8047CD04;  /* u32 (lwz) */
extern f32 lbl_8047CD08;  /* f32 */
extern f64 lbl_8047CD10;  /* f64 */
extern f64 lbl_8047CD18;  /* f64 */
extern f64 lbl_8047CD20;  /* f64 */
extern f64 lbl_8047CD28;  /* f64 */
extern f32 lbl_8047CD30;  /* f32 */
extern f32 lbl_8047CD34;  /* f32 */
extern f32 lbl_8047CD38;  /* f32 */
extern f32 lbl_8047CD3C;  /* f32 */
extern f32 lbl_8047CD40;  /* f32 */
extern f32 lbl_8047CD44;  /* f32 */
extern f32 lbl_8047CD48;  /* f32 */
extern f32 lbl_8047CD4C;  /* f32 */
extern f64 lbl_8047CD50;  /* f64 */
extern f32 lbl_8047CD58;  /* f32 */
extern f32 lbl_8047CD5C;  /* f32 */
extern f32 lbl_8047CD60;  /* f32 */
extern f32 lbl_8047CD64;  /* f32 */
extern f32 lbl_8047CD68;  /* f32 */
extern f32 lbl_8047CD6C;  /* f32 */
extern f32 lbl_8047CD70;  /* f32 */
extern f32 lbl_8047CD74;  /* f32 */
extern f32 lbl_8047CD78;  /* f32 */
/* sbss (r13) symbols -- task and thread system */
extern u32 lbl_80478B08;
extern u32 lbl_80478B10;
extern u32 lbl_80478B14;
extern u32 lbl_8047AC00;
extern u32 lbl_8047AC04;
extern u32 lbl_8047AC08;
extern u32 lbl_8047AC0C;
extern u32 lbl_8047AC10;
extern u32 lbl_8047AC14;
extern u32 lbl_8047AC18;
extern u32 lbl_8047AC1C;
extern u32 lbl_8047AC20;
extern u32 lbl_8047AC24;
extern u32 lbl_8047AC28;
extern u32 lbl_8047AC2C;
extern u32 lbl_8047AC30;
extern u32 lbl_8047AC34;
extern u32 lbl_8047AC38;
extern u32 lbl_8047AC3C;
extern u32 lbl_8047AC40;
extern u32 lbl_8047AC44;
extern u32 lbl_8047AC48;
extern u32 lbl_8047AC4C;
extern u32 lbl_8047AC50;
extern u32 lbl_8047AC54;
extern u16 lbl_8047AC58;
extern u32 lbl_8047AC5C;
extern u32 lbl_8047AC60;
extern u32 lbl_8047AC64;
extern u32 lbl_8047AC68;
extern u32 lbl_8047AC6C;
extern u32 lbl_8047AC70;
extern u32 lbl_8047AC72;
extern u32 lbl_8047AC74;
extern u32 lbl_8047AC78;
extern u32 lbl_8047AC7C;
extern u32 lbl_8047AC80;
extern u32 lbl_8047AC84;
extern u32 lbl_8047AC88;
extern u32 lbl_8047AC8C;
extern u32 lbl_8047AC90;
extern u32 lbl_8047AC94;
extern u32 lbl_8047AC98;
extern u32 lbl_8047AC9C;

/* Forward declarations for all asm-wrapped functions in this block */
extern void fn_800F8268(void);
extern void fn_800F8428();
extern void fn_800F8654();
extern void fn_800F8A54();
extern u32 fn_800F92D4(u32 key);
extern void GSresInit(u32 count);
extern u8 * fn_800F96E4();
extern u32 fn_800F9AEC(void* outbuf, u16* src, s32 mode);
extern void GScharMakeFromSJIS(void);
extern u8* GScharCpy(u8* dst, u8* src);
extern void GSmsgSetColor(void* obj);
extern s32 GSmsgGetRect();
extern void GSmsgInitRuby();
extern s32 fn_800FAEF8();
extern s32 fn_800FB43C();
extern s32 fn_800FB680();
extern s32 fn_800FB8C8();
extern s32 fn_800FBB34();
extern void GSmsgDaemon(void);
extern s32 GSmsgExec();
extern void fn_800FC2A4(void);
extern u32 fn_800FC2A8(void* ptr);
extern void* GSmsgFontOpen();
extern s32 GSmsgSetCtrlFunc(u32 val);
extern s32 GSmsgInit();
extern s32 fn_800FC7E0();
extern void fn_800FD348();
extern void fn_800FD69C();
extern u16 * _msgGetCodeInfo__FP13MSG_TASK_WORKUsPP12tagFONT_INFO();
extern s32 _msgGetLength__FPCUs(const void* str);
extern s32 _msgGetSize__FPCUs();
extern void fn_800FE35C(void);
extern void fn_800FE38C(s32 x1, s32 y1, s32 x2, s32 y2);
extern void spriteSetEnv(void);
extern void fn_800FE6A0(f32 a, f32 b);
extern void fn_800FE6AC(s16* outA, s16* outB);
extern void fn_800FE6D0(s32 a, s32 b);
extern void GSgappUnblock(u32 taskId);
extern void GSgappBlock(u32 taskId);
extern void GSgappTerminate(u32 taskId);
extern void GSgappUpdate(void);
extern u32 GSgappCreate(s32 state, u8 priority, void* param, void* func);
extern void GSgappInit();
extern void gappBackgroundCallback(void);

/* 0x800FE35C | 0x30 */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void fn_800FE35C(void) {
#include "src/game/gs_thread_fn_800FE35C.inc"
}
#else
#pragma optimization_level 2
#pragma scheduling off
void fn_800FE35C(void) {
    fn_800D9D68(0, 0, 0x27F, 0x1DF);
}
#pragma scheduling on
#endif
#pragma pop

/* 0x800FE38C | 0x148 */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void fn_800FE38C(s32 x1, s32 y1, s32 x2, s32 y2) {
#include "src/game/gs_thread_fn_800FE38C.inc"
}
#else
#pragma optimization_level 2
#pragma scheduling on
#pragma peephole off
#pragma push
#pragma optimization_level 3
void fn_800FE38C(s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 ax, ay, bx, by;
    f32 scale_x, scale_y;
    s32 cy2, cx2, cy1, cx1;

    ax = (s32)(*(s16*)&lbl_8047AC70) + x1;
    ay = (s32)(*(s16*)&lbl_8047AC72) + y1;
    bx = ax + x2;
    by = ay + y2;
    scale_x = *(f32*)&lbl_80478B10;
    scale_y = *(f32*)&lbl_80478B14;
    cx1 = (s32)((f32)ax * scale_x);
    cy1 = (s32)((f32)ay * scale_y);
    cx2 = (s32)((f32)bx * scale_x);
    cy2 = (s32)((f32)by * scale_y);
    if (cx1 >= 0x280) cx1 = 0x27F;
    if (cy1 >= 0x1E0) cy1 = 0x1DF;
    if (cx2 >= 0x280) cx2 = 0x27F;
    if (cy2 >= 0x1E0) cy2 = 0x1DF;
    if (cx1 < 0) cx1 = 0;
    if (cy1 < 0) cy1 = 0;
    if (cx2 < 0) cx2 = 0;
    if (cy2 < 0) cy2 = 0;
    fn_800D9D68(cx1, cy1, cx2, cy2);
}
#pragma pop
#endif
#pragma pop

/* 0x800FE4D4 | 0x1CC */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void spriteSetEnv(void) {
#include "src/game/gs_thread_fn_800FE4D4.inc"
}
#else
#pragma optimization_level 2
void spriteSetEnv(void) {
    f32 v0[3];
    f32 v1[3];
    f32 v2[3];
    f32 mtx[12];
    f32 x;
    f32 y;
    f32 sx;
    f32 sy;
    f32 z;
    f32 t;
    f32 width;
    f32 x_scale;
    f32 height;
    f32 y_scale;
    f32 half;

    width = lbl_8047CD58;
    x_scale = *(f32*)&lbl_80478B10;
    height = lbl_8047CD5C;
    y_scale = *(f32*)&lbl_80478B14;
    sx = width / x_scale;
    sy = height / y_scale;
    half = lbl_8047CD60;
    x = sx * half;
    y = sy * half;
    t = (f32)tan(lbl_8047CD64);
    z = y / t;

    set__5GSvecFfff(v0,
                x - (f32)(s32)*(s16*)&lbl_8047AC70,
                y - (f32)(s32)*(s16*)&lbl_8047AC72,
                z);
    set__5GSvecFfff(v1,
                x - (f32)(s32)*(s16*)&lbl_8047AC70,
                y - (f32)(s32)*(s16*)&lbl_8047AC72,
                lbl_8047CD68);
    set__5GSvecFfff(v2, lbl_8047CD68, lbl_8047CD6C, lbl_8047CD68);
    fn_800E0218(mtx, v0, v2, v1);

    fn_800D9BD0(lbl_8047CD70, -(sx / sy), lbl_8047CD74, lbl_8047CD78);
    fn_800D834C();
    fn_800D7FE4(mtx);
    fn_800DA4C4(1, 6, 7);
    fn_800D888C(0x80000000);
    fn_800DA2BC(2, 2, 1);
    fn_800DA100(0, 7, 0, 1, 7, 0);
    fn_800DA1E8(0, 2, 0);
    fn_800DA028(0);
}
#endif
#pragma pop

/* 0x800FE6A0 | 0xC */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void fn_800FE6A0(void) {
#include "src/game/gs_thread_fn_800FE6A0.inc"
}
#else
#pragma optimization_level 2
void fn_800FE6A0(f32 a, f32 b) {
    *(f32*)&lbl_80478B10 = a;
    *(f32*)&lbl_80478B14 = b;
}
#endif
#pragma pop

/* 0x800FE6AC | 0x24 */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void fn_800FE6AC(void) {
#include "src/game/gs_thread_fn_800FE6AC.inc"
}
#else
#pragma optimization_level 2
void fn_800FE6AC(s16* outA, s16* outB) {
    if (outA != (void*)0) {
        *outA = *(s16*)&lbl_8047AC70;
    }
    if (outB != (void*)0) {
        *outB = *(s16*)&lbl_8047AC72;
    }
}
#endif
#pragma pop

/* 0x800FE6D0 | 0xC */
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void fn_800FE6D0(void) {
#include "src/game/gs_thread_fn_800FE6D0.inc"
}
#else
#pragma optimization_level 2
void fn_800FE6D0(s32 a, s32 b) {
    *(u16*)&lbl_8047AC70 = (u16)a;
    *(u16*)&lbl_8047AC72 = (u16)b;
}
#endif
#pragma pop
