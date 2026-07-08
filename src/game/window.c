/**
 * @file window.c
 * @brief window -- window allocation, cursor/sprite management, and the
 *        WINDOW_WORK object table (Colosseum "menu" UI subsystem). Split
 *        from the gs_model.c splitter bucket (address range 0x80103FE4 -
 *        0x801058CC, 25 fns). Corresponds to XD's window.cpp
 *        (0x80114D30-0x80116A90); 10 anchor symbols (windowGetAllocPtr,
 *        windowSetParam, windowSearchID, ...) strictly monotonic against
 *        the XD address run, TU head matches (windowGetAllocPtr).
 *        windowInit is called by menuInit (menu.c).
 */
#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 n);
extern void  GSlogWrite(const char* fmt, ...);         /* OSReport / GSlog */

/* GSmem */
extern u16   _toolentryAlloc__FUl(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                   /* GSmemLock */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */

/* GSgfx state */
extern u8 lbl_8047AA80[];  /* GSgfx state pointer (via sda21) */

/* Matrix math */
extern void  PSMTXIdentity(void);                         /* MTXIdentity */
extern void  PSMTXCopy(void* mtxA, void* mtxB);      /* MTXConcat */
extern void  PSMTXMultVec(void* mtx, void* vec, void* out); /* MTXMultVec3 */

/* Model resource table (BSS) */
extern u8 lbl_80402518[];  /* model resource table -- 0x2400 bytes */

/* Global model system state block */
extern u8 lbl_80404ACC[];

typedef struct GSModelStateHeader {
    u16 count;          /* 0x00 */
    u16 entryHandle;    /* 0x02 */
    u32 unk_04;         /* 0x04 */
    void* entries;      /* 0x08 */
} GSModelStateHeader;

#define GS_MODEL_STATE ((GSModelStateHeader*)lbl_80404ACC)

/* Resource index table */
extern u32 lbl_80478B20;   /* max resource index (sda21) */
extern u8  lbl_80315690[]; /* resource table, 8-byte entries */

/* Additional externs used by various functions */
extern void  GSmodelFree(u32);     /* GSmem release/unref */
extern void  fn_800D2738(void);
extern void* menuDataBiosGetPtr(void);    /* linked list head */
extern void* menuItemBiosGetPtr(s16 idx); /* node by index */
extern void* menuSeBiosGetPtr(s32);
extern u16   fn_8005D798(void*, s32);
extern void* menuSpriteBiosGetPtr(s32);
extern int   fn_80166A28(u16);
extern s32   fn_800F037C(void);    /* poll/yield -- 0 if pending */
extern void  _threadSwitch(void);    /* yield */
extern u32   fn_800BE31C(void);    /* rand or tick */
extern u32   fn_800B8FD8(void*);   /* register fn, returns handle */
extern void  fn_800BD91C(s32, s32);
extern void  fn_800B8C58(s32);
extern void  GSgfxBeginBackFBCapture(u32, void*, void*);
extern u32   GStextureCreate(s32, s32, s32, s32, s32);
extern u32   GSmodelCanAnimate(u32);
extern void  GSmodelSetAnimIndex(u32, u32);
extern void  GSmodelSetAnimRate(u32, f32);
extern void  GSmodelStartAnimation(u32);
extern void  fn_801DB100(u32);
extern u32   OSGetTick(void);

extern u32 lbl_8047ACF0;
extern u32 lbl_8047ACF4;  /* function pointer for callback */
extern u32 lbl_8047ACF8;  /* saved tick */
extern u32 lbl_8047ACEC;  /* tick counter */
extern u32 lbl_8047ACE8;  /* tick base */
extern u8 lbl_80478B28;   /* max slot byte */
extern u8 lbl_8047AD20;
extern u8 lbl_8047AD21;
extern u8 lbl_8047AD22;
extern u8 lbl_8047AD23;
extern u8 lbl_8047AD24;
extern f32 lbl_8047AD2C;
extern f32 lbl_8047AD30;
extern f32 lbl_8047AD34;
extern f32 lbl_8047AD38;
extern f32 lbl_8047AD3C;
extern u32 lbl_8047AD28;
extern u16 lbl_8047AD18;  /* GSmem handle */
extern u8* lbl_8047AD1C;  /* object pool pointer */
extern f32 lbl_8047CDC0;  /* sdata2: float constant */
extern f32 lbl_8047CDC4;  /* sdata2: float constant */
extern u16 lbl_8047CDE0;  /* sdata2: */
extern u16 lbl_8047CDE4;  /* sdata2: */
extern f32 lbl_8047CD80;  /* sdata2: float constant */
extern f32 lbl_8047CD84;  /* sdata2: float constant */
extern f32 lbl_8047CD88;  /* sdata2: float constant */
extern f32 lbl_8047CD8C;  /* sdata2: float constant */
extern f32 lbl_8047CD90;  /* sdata2: float constant */
extern f32 lbl_8047CD94;  /* sdata2: float constant */
extern f64 lbl_8047CD98;  /* sdata2: double constant */
extern f32 lbl_8047CE3C;  /* sdata2: float constant */
extern f32 lbl_8047CE50;  /* sdata2: float constant */
extern f32 lbl_8047CE5C;  /* sdata2: float constant */
extern f32 lbl_8047CE70;  /* sdata2: float constant */
extern u8  lbl_80404A98[];  /* table for display */
extern u8  lbl_80271E10[];  /* format string */
extern u8  lbl_80271E4C[];  /* format string */
extern u8  lbl_80271EE8[];  /* format string */
extern u8  lbl_80271F18[];  /* format string */
extern u8  lbl_8035B060[];  /* module name string */
extern u8  lbl_8035B3F0[];  /* module name string */

/* Additional external functions (not already declared above) */
extern void fn_800BF74C(void);
extern void fn_800D9ED8(s32);
extern void fn_800D88DC(s32);
extern void fn_800D888C(s32);
extern void fn_800D9B58(f32, f32, f32, f32);
extern void fn_800DA4C4(s32, s32, s32);
extern void fn_800DA2BC(s32, s32, s32);
extern void fn_800DA1E8(s32, s32, s32);
extern void fn_800DA028(s32);
extern void fn_800D6A00(s32);
extern void fn_800D7820(s32);
extern void fn_800D67BC(s32);
extern void fn_800D6680(f32);
extern void fn_800D5CB8(s32, s32, s32, s32, s32);
extern void fn_800D6728(void);

/* Forward declarations for functions defined later in this TU */
extern u8    menuOffScreenCheckEnable(u8 param);
extern void  windowClose(void* ptr, u32 flags);
extern void* windowSearchID(s32 param);
extern s32   _menuCBOffScreen__FP9GStextureUlPv(void);
extern void  winSpriteSetDisp(void* node, u32 enable);
extern void  windowGetValue(s32 param);
extern void  windowCheckCursor(void* p, u8 flags);
extern void  windowDrawSprite2(void* r3, void* r4, s16 r5, s16 r6, s32 r7, s32 r8, s32 r9, s32 r10);
extern u8    menuOffScreenFadeSync(u8 param);
extern void  menuOffScreenFadeSet(f32 f1, f32 f2);
extern u8    menuOffScreenCreate(u32 param);
extern void  menuOffScreenRelease(void);
extern u32   windowGetActiveID(void);
extern void* windowGetKeyInfo(void);
extern void* menuSeqBiosGetPtr(u32 idx);
extern void* windowSearchItemID(void* head, s32 key);
extern void  menuOpenCustom(void* p, u32 r4, s32 r5, s32 r6, void* r7, s32 r8, ...);
extern u8    menuOffScreenSetPriority(u8 val);
extern u8    menuOffScreenSetDisp(u8 val);
extern u32   fn_800D3088(void);
extern u8    lbl_80404B68[];  /* scratch table for fn_80107F38, fn_801081F8 */
extern u8    lbl_80404B8C[];  /* scratch table for winSeqSetMenu */
extern u8    lbl_8047AD10;     /* resource request gate byte (sda21) - authoritative decl, use as-is */

/* Cross-segment prototypes for menu/window-engine functions defined in
 * sibling segment files split from the gs_model.c splitter bucket. */
extern void fn_801019F8(void);
extern void* kaisuuBiosGetMax(u32 index);
extern void* kaisuuBiosGetMin(u32 index);
extern u32 kaisuuGetKaisuu(u32 param);
extern void fn_80101B34(u32 param);
extern void fn_80101B88(u32 val);
extern void fn_80101B90(void);
extern void fn_80101D5C(void);
extern void fn_80101D8C(void);
extern void fn_80101FB8(u8 param);
extern void menuGetOffScreenFlag(void);
extern void menuReleaseOffScreen(f32 f1);
extern void menuCreateOffScreen(f32 param);
extern s32 menuGetSelectItemNum(void);
extern s32 menuGetCursorFromItemID(void* unused, u32 param);
extern void fn_801021F8(void* p, u32 val);
extern void menuSetDisp(void* p, u32 enable);
extern void* menuGetCursorItemID(void* p, u32 target);
extern s32 menuSetCursor(void* p, u32 val);
extern s32 menuGetCursor(void* p);
extern s32 menuCloseSync(void* p, u8 flag);
extern void menuCloseFloor(void);
extern void fn_801024E8(void);
extern void menuClose(s32 p);
extern s32 menuCloseCustom(void* p, u32 mode, u8 wait);
extern s32 menuIsCheck(s32 param);
extern void menuOpen(void* p, void* q);
extern void menuOpenCustom(void* p, u32 r4, s32 r5, s32 r6, void* r7, s32 r8, ...);
extern void menuSetPosition(void* p, s16 a, s16 b);
extern void menuButtonNormal(void* p);
extern void menuPlaySe(void* p, void* q);
extern void fn_801034DC(void);
extern void _menuGetGcKeyInfo__FlPUs(void);
extern void _menuUpdateKeyInfo__FP15WINDOW_SYS_WORK(void);
extern void menuGetKeyInfo(void);
extern u8 menuGetEnablePort(void);
extern u8 menuSetEnablePort(u8 val);
extern void menuInit(void);
extern u32 cursorBiosGetPos(u16 idx);
extern u32 cursorBiosSetPos(u16 idx, u16* out);
extern void cursorBiosInit(void);
extern void fn_80103F74(void* head, u16 key, u32 data);
extern void* windowGetAllocPtr(void* ptr);
extern void* windowAllocMemory(void* p, s32 size);
extern void* windowGetFreeWork(void* ptr);
extern void windowSetParam(void* ptr, u32 idx, u32 val);
extern u32 windowGetParam(void* ptr, u32 idx);
extern void windowDrawSprite(void* p, void* a, void* b, u16 key, u32 data);
extern void windowDrawSprite2(void* r3, void* r4, s16 r5, s16 r6, s32 r7, s32 r8, s32 r9, s32 r10);
extern u8* windowGetCursorToItem(u8* arg);
extern void windowGetValue(s32 param);
extern s32 fn_801044D0(s32 param, u16* val);
extern void windowGetCursor(void);
extern void windowCheckCursor(void* p, u8 flags);
extern u32 windowGetActiveID(void);
extern void* windowSearchItemID(void* head, s32 key);
extern void* windowSearchID(s32 param);
extern void windowCloseMain(void* obj);
extern void windowClose(void* ptr, u32 flags);
extern void _windowCreateItemSprite__FP14tagWINDOW_WORK(void);
extern void windowCreateCursorSprite(void);
extern void windowOpen(void);
extern void _winCalcWindowSize__FlPC13MENU_ITEM_dd_PsPs(void);
extern void windowInit(u16 count);
extern void windowGetPortKeyInfo(void);
extern void* windowGetKeyInfo(void);
extern void fn_80105634(void);
extern void winMsgDraw(void);
extern void winMsgCtrl(void);
extern void winMsgButton(void* p);
extern void winMsgCloseLevelUpStatus(void);
extern void winMsgOpenLevelUpFiledStatus(void);
extern void winMsgOpenLevelUpStatus(void);
extern void winMsgCloseError(void);
extern void winMsgOpenError(void);
extern void winMsgCloseFight(void);
extern void winMsgCloseCheckFight(void);
extern void winMsgOpenFightNoWait(void);
extern void winMsgOpenFight(void);
extern void winMsgCheckField(void);
extern void winMsgCloseField(void);
extern void winMsgOpenFieldWithSE(void);
extern void winMsgOpenField(void);
extern void winMsgCheck(void);
extern void winMsgClose(void);
extern void winMsgOpenWithSE(void);
extern void winMsgOpen(void);
extern void fn_80106F98(void);
extern s32 winSeqCheckMove(s32 param);
extern s32 fn_80107170(s32 r3, s32 r31);
extern void winSeqMoveMenu(void);
extern s32 fn_80107E78(void* r3, s32 r4, u16 r30);
extern s32 winSeqIsCheck(s32 r3, u16 r30);
extern void fn_80107F38(s32 param, u32 key);
extern void winSeqSetMenu(s32 param, u32 key);
extern void fn_801081F8(void* r3_arg, u16 r4, u16 r5);
extern void winSetSequence(void* out, u32 idx);
extern s32 winSpriteGetDisp(void* ptr);
extern void winSpriteSetDisp(void* node, u32 enable);
extern void winSpriteRelease(void* head);
extern void* fn_80109290(void* root);
extern void winSpriteInit(void);
extern void fn_801093C8(void);
extern u8 menuOffScreenFadeSync(u8 param);
extern void menuOffScreenFadeSet(f32 f1, f32 f2);
extern u8 menuOffScreenSetPriority(u8 val);
extern u8 menuOffScreenSetDisp(u8 val);
extern u8 menuOffScreenIsDoing(void);
extern u32 menuOffScreenGetPtr(void);
extern u8 menuOffScreenCheckEnable(u8 param);
extern void menuOffScreenRelease(void);
extern u8 menuOffScreenCreate(u32 param);
extern void menuOffScreenInit(void);
extern s32 _menuCBOffScreen__FP9GStextureUlPv(void);
extern s32 menuModelSetMotion(void* p, u32 val);
extern void menuModelRender(void);
extern s32 menuModelCheck(void* obj, u8 wait);
extern s32 menuModelFree(void* p);

/* 0x80103FE4 | 0x18 */
void* windowGetAllocPtr(void* ptr) {
    if (ptr) {
        return *(void**)((u8*)ptr + 0xB0);
    }
    return (void*)0;
}

/* 0x80103FFC | 0xA4 */
#pragma push
#pragma peephole off
void* windowAllocMemory(void* p, s32 size) {
    s32 r31 = size;
    void* r30 = p;
    if (r30 == (void*)0) { return (void*)0; }
    if (*(u16*)((u8*)r30 + 0xac) != 0) {
        fn_800E24B0(*(u16*)((u8*)r30 + 0xac));
        fn_800E209C(*(u16*)((u8*)r30 + 0xac));
        *(u32*)((u8*)r30 + 0xb0) = 0;
    }
    if (r31 <= 0) { return (void*)0; }
    {
        u16 h = _toolentryAlloc__FUl((u32)r31);
        *(u16*)((u8*)r30 + 0xac) = h;
        if (*(u16*)((u8*)r30 + 0xac) != 0) {
            void* ptr = fn_800E27B0(*(u16*)((u8*)r30 + 0xac));
            *(void**)((u8*)r30 + 0xb0) = ptr;
        } else {
            return (void*)0;
        }
    }
    return *(void**)((u8*)r30 + 0xb0);
}
#pragma pop

/* 0x801040A0 | 0x18 */
void* windowGetFreeWork(void* ptr) {
    if (ptr) {
        return (void*)((u8*)ptr + 0x9C);
    }
    return (void*)0;
}

/* 0x801040B8 | 0x18 */
void windowSetParam(void* ptr, u32 idx, u32 val) {
    if (ptr == (void*)0) { return; }
    ((u32*)((u8*)ptr + 0x60))[idx] = val;
}

/* 0x801040D0 | 0x20 */
u32 windowGetParam(void* ptr, u32 idx) {
    if (ptr) {
        return ((u32*)((u8*)ptr + 0x60))[idx];
    }
    return 0;
}

/* 0x801040F0 | 0x70 */
/* menuSpriteBiosGetPtr already declared above */
void windowDrawSprite(void* p, void* a, void* b, u16 key, u32 data) {
    void* r27 = p;
    void* r28 = a;
    void* r29 = b;
    u16 r30 = key;
    u32 r31 = data;
    if ((u16)r30 != 0) {
        void* r6 = menuSpriteBiosGetPtr((s32)(u16)r30);
        windowDrawSprite2(r27, r28, *(s16*)((u8*)r6 + 0xc), *(s16*)((u8*)r6 + 0xe), (s32)r29, (s32)(u16)r30, (s32)r31, -1);
    }
}

/* 0x80104160 | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowDrawSprite2(void* r3, void* r4, s16 r5, s16 r6, s32 r7, s32 r8, s32 r9, s32 r10) {
    /* TODO: match -- 440 bytes at 0x80104160 */
    (void)r3; (void)r4; (void)r5; (void)r6; (void)r7; (void)r8; (void)r9; (void)r10;
}
#pragma pop

/* 0x80104318 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u8* windowGetCursorToItem(u8* arg) {
#pragma optimization_level 4
#pragma peephole off
    void* node;
    s32 idx;
    { extern void* menuDataBiosGetPtr(void*); node = menuDataBiosGetPtr(*(void**)(arg + 0x4)); }
    node = menuItemBiosGetPtr(*(s16*)((u8*)node + 0x4));
    idx = 0;
    while (1) {
        if (((u32)*(volatile u8*)node >> 7) & 1) {
            if ((s8)*(s8*)(arg + 0x95) == idx) {
                return (u8*)node;
            }
            idx = idx + 1;
        }
        if (((u32)*(volatile u8*)node >> 6) & 1) {
            break;
        }
        node = menuItemBiosGetPtr(*(s16*)((u8*)node + 0x18));
    }
    return (u8*)0;
}
#pragma pop

/* 0x801043A4 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowGetValue(s32 param) {
    /* TODO: match -- 300 bytes at 0x801043A4 */
    (void)param;
}
#pragma pop

/* shared model-table lookup, inlined by the find-and-act helpers below */
static inline void* mdl_find(s32 param) {
    void* r;
    if (param <= 0) { return (void*)0; }
    r = *(void**)((u8*)lbl_80404ACC + 0xc);
    while (r != (void*)0) {
        if (*(s32*)((u8*)r + 0x4) == param) { return r; }
        r = *(void**)((u8*)r + 0x10);
    }
    return (void*)0;
}

/* 0x801044D0 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_801044D0(s32 param, u16* val) {
#pragma optimization_level 2
    void* node = mdl_find(param);
    if (node != (void*)0) {
        *(u16*)((u8*)node + 0x94) = *val;
        return 1;
    }
    return 0;
}
#pragma pop

/* 0x80104530 | 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowGetCursor(void) {
    /* TODO: match -- 120 bytes at 0x80104530 */
}
#pragma pop

/* 0x801045A8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowCheckCursor(void* p, u8 flags) {
    /* TODO: match -- 272 bytes at 0x801045A8 */
    (void)p; (void)flags;
}
#pragma pop

/* 0x801046B8 | 0x10 */
u32 windowGetActiveID(void) {
    return *(u32*)(lbl_80404ACC + 0x4);
}

/* 0x801046C8 | 0x3C */
#pragma push
#pragma scheduling off
void* windowSearchItemID(void* head, s32 key) {
    if (head == (void*)0) { return (void*)0; }
    {
        void* r3 = *(void**)((u8*)head + 0x1c);
        s32 r4 = (u16)key;
        while (r3 != (void*)0) {
            s16 r0 = *(s16*)((u8*)r3 + 0x6);
            if ((s32)r0 == r4) { return r3; }
            r3 = *(void**)r3;
        }
        return (void*)0;
    }
}
#pragma pop

/* 0x80104704 | 0x48 */
void* windowSearchID(s32 param) {
    if (param <= 0) { return (void*)0; }
    {
        void* r4 = *(void**)((u8*)lbl_80404ACC + 0xc);
        while (r4 != (void*)0) {
            s32 r0 = *(s32*)((u8*)r4 + 0x4);
            if (r0 == param) { return r4; }
            r4 = *(void**)((u8*)r4 + 0x10);
        }
        return (void*)0;
    }
}

/* 0x8010474C | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowCloseMain(void* obj) {
#pragma optimization_level 4
#pragma peephole off
    void* h;
    void* nx;
    { extern void* menuDataBiosGetPtr(void*); h = menuDataBiosGetPtr(*(void**)((u8*)obj + 0x4)); }
    if (*(u32*)((u8*)h + 0x14) != 0) {
        *(u8*)((u8*)obj + 0x1) = 5;
        (*(void (*)(void*))*(u32*)((u8*)h + 0x14))(obj);
    }
    if (obj != (void*)0) {
        if (*(void**)((u8*)obj + 0x14) == (void*)0) {
            *(void**)((u8*)lbl_80404ACC + 0xc) = *(void**)((u8*)obj + 0x10);
        } else {
            *(void**)((u8*)*(void**)((u8*)obj + 0x14) + 0x10) = *(void**)((u8*)obj + 0x10);
        }
        nx = *(void**)((u8*)obj + 0x10);
        if (nx != (void*)0) {
            *(void**)((u8*)nx + 0x14) = *(void**)((u8*)obj + 0x14);
        }
        { extern void winSpriteRelease(void* head); winSpriteRelease((u8*)obj + 0x1c); }
        { extern void winSpriteRelease(void* head); winSpriteRelease((u8*)obj + 0x20); }
        if (*(u16*)((u8*)obj + 0xac) != 0) {
            fn_800E24B0(*(u16*)((u8*)obj + 0xac));
            fn_800E209C(*(u16*)((u8*)obj + 0xac));
            *(u32*)((u8*)obj + 0xb0) = 0;
            *(u16*)((u8*)obj + 0xac) = 0;
        }
        *(u8*)((u8*)obj + 0x0) = 0;
        *(u32*)((u8*)obj + 0x4) = 0;
    }
}
#pragma pop

/* 0x80104828 | 0x26C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowClose(void* ptr, u32 flags) {
    /* TODO: match -- 620 bytes at 0x80104828 */
}
#pragma pop

/* 0x80104A94 | 0x20C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void _windowCreateItemSprite__FP14tagWINDOW_WORK(void) {
    /* TODO: match -- 524 bytes at 0x80104A94 */
}
#pragma pop

/* 0x80104CA0 | 0x1E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowCreateCursorSprite(void) {
    /* TODO: match -- 480 bytes at 0x80104CA0 */
}
#pragma pop

/* 0x80104E80 | 0x474 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowOpen(void) {
    /* TODO: match -- 1140 bytes at 0x80104E80 */
}
#pragma pop

/* 0x801052F4 | 0x11C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void _winCalcWindowSize__FlPC13MENU_ITEM_dd_PsPs(void) {
    /* TODO: match -- 284 bytes at 0x801052F4 */
}
#pragma pop

/* 0x80105410 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowInit(u16 count) {
#pragma optimization_level 4
#pragma peephole off
    extern u8 lbl_80271EC4[];
    extern void winSpriteInit(void);
    u32 size;
    u16 handle;
    void* ptr;

    memset(lbl_80404ACC, 0, 0x9c);
    size = (u16)count * 0xb4;
    handle = _toolentryAlloc__FUl(size);
    GS_MODEL_STATE->entryHandle = handle;
    if ((u16)handle == 0) {
        GSlogWrite((const char*)lbl_80271EC4);
    } else {
        ptr = fn_800E27B0((u16)handle);
        GS_MODEL_STATE->entries = ptr;
        GS_MODEL_STATE->count = count;
        memset(ptr, 0, size);
        winSpriteInit();
    }
}
#pragma pop

/* 0x801054B8 | 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void windowGetPortKeyInfo(void) {
    /* TODO: match -- 364 bytes at 0x801054B8 */
}
#pragma pop

/* 0x80105624 | 0x10 */
void* windowGetKeyInfo(void) {
    return (void*)(lbl_80404ACC + 0x10);
}

/* 0x80105634 | 0x298 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105634(void) {
    /* TODO: match -- 664 bytes at 0x80105634 */
}
#pragma pop

