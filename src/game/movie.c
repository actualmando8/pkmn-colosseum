/**
 * @file movie.c
 * @brief THP movie playback system for Pokemon Colosseum.
 *
 * Decompiled from:
 *   fn_80035EE4 (moviePlayOpeningDemo)
 *   fn_80035F34 (moviePlayAutoDemo -- fade setup portion)
 *   fn_80035F64 (moviePlayStaffRoll)
 *   fn_80035E04 (movieStopAndCleanup / movieWaitForFinish)
 *
 * Movie files referenced:
 *   "movie/openingdemo.thp" -- Opening cinematic
 *   "movie/staffroll.thp"   -- Credits roll
 *   "movie/autodemo01.thp"  -- Auto-demo / attract mode
 *   "movie/gs_logo.thp"     -- Genius Sonority logo
 *   "movie/tpc.thp"         -- The Pokemon Company logo
 *
 * The THP player is a standard Nintendo SDK component. These functions
 * coordinate movie playback with the GS engine's sound, flag, floor,
 * and effect systems to ensure proper game state during and after
 * movie playback.
 *
 * Address range: 0x80035E04 - 0x800366D0 (approx.)
 */

#include "dolphin/types.h"
#include "game/movie.h"

/* ===== THP Player SDK functions ===== */
extern u8   fn_801E1874(void);                        /* THPPlayerGetState */
extern void fn_801E189C(const char* path, u32 loop);  /* THPPlayerOpen */

/* ===== GS Engine external functions ===== */
extern void fn_800F0308(void);                        /* GSthread yield / step */
extern void fn_80165A20(u32 sndId, u32 fade, u32 vol); /* sndPlay (BGM start) */
extern void fn_801C41C8(u32 mode, f32 speed);         /* fade set mode+speed */
extern void fn_801C40F0(u32 enable);                   /* fade enable */
extern void fn_80190528(u32 flagId);                   /* GSflagSet (used for cutscene flags) */
extern u32  fn_801902E0(u32 flagId);                   /* GSflagGet */
extern void fn_80113828(u32 a, u32 b);                /* floor resource unload helper */
extern void fn_8011288C(u32 a, u32 b);                /* floor resource alloc helper */
extern u32  fn_8011394C(void);                         /* floor state query */
extern void* fn_80129280(u32 a, u32 b);               /* battle/effect state setup */
extern void fn_80135030(void* ctx, u32 a, u32 b);     /* effect parameter set */
extern void* fn_80135168(void* ctx, u32 a);            /* effect query */
extern void fn_8012A450(u32 a, u32 b, u32 c);         /* effect system control */
extern void* fn_8012A5B0(u32 a, u32 b, u32 c);        /* effect system query */
extern void fn_80106D3C(u32 a, u32 b, u32 c, u32 d);  /* floor transition trigger */
extern s8   fn_8001E074(u32 a, u32 b, u32 c, u32 d);  /* input poll / wait */
extern void* fn_801D036C(void);                        /* battle state query */
extern u32  fn_801D0748(u32 a, u32 b, u32 c);         /* battle mode set */

/* ===== String constants (rodata) ===== */
extern const char lbl_80266FE8[]; /* "movie/openingdemo.thp" */
extern const char lbl_80267000[]; /* "movie/staffroll.thp" */
extern const char lbl_80267014[]; /* "movie/autodemo01.thp" */
extern const char lbl_8026702C[]; /* "movie/gs_logo.thp" */
extern const char lbl_80267040[]; /* "movie/tpc.thp" */

/* ===== Float constants (sdata2) ===== */
extern f32 lbl_8047BA30; /* 1.0f -- fade speed */

/* =======================================================================
 *  movieWaitForFinish (internal helper)
 *
 *  Polls THPPlayerGetState in a loop. While state == THP_STATE_PLAYING,
 *  yields the current thread via fn_800F0308.
 *
 *  This pattern is used before starting a new movie or after requesting
 *  a stop -- ensures the THP player is idle before proceeding.
 *
 *  Assembly pattern (appears at multiple sites):
 *    .loop:
 *      bl fn_800F0308           ; yield
 *    .check:
 *      bl fn_801E1874           ; THPPlayerGetState
 *      clrlwi r0, r3, 24       ; mask to byte
 *      cmplwi r0, 1             ; == THP_STATE_PLAYING?
 *      beq .loop
 * ======================================================================= */
void movieWaitForFinish(void) {
    u8 state;

    do {
        fn_800F0308();  /* yield thread */
        state = fn_801E1874();
    } while ((state & 0xFF) == THP_STATE_PLAYING);
}

/* =======================================================================
 *  moviePlayOpeningDemo / fn_80035EE4
 *  Address: 0x80035EE4, Size: 0x50
 *
 *  Assembly:
 *    lfs f1, lbl_8047BA30@sda21(r0)  ; f1 = 1.0
 *    li r3, 2
 *    bl fn_801C41C8                  ; fade mode 2 (fade-in), speed 1.0
 *    li r3, 1
 *    bl fn_801C40F0                  ; enable fade
 *    lis r3, lbl_80266FE8@ha
 *    li r4, 0
 *    addi r3, r3, lbl_80266FE8@l    ; "movie/openingdemo.thp"
 *    bl fn_801E189C                  ; THPPlayerOpen(path, loop=0)
 *    li r3, 0x495
 *    li r4, 0
 *    li r5, 0x7F
 *    bl fn_80165A20                  ; sndPlay(0x495, 0, 127) -- opening BGM
 *    blr
 * ======================================================================= */
void moviePlayOpeningDemo(void) {
    /* Set up screen fade: mode 2 (fade-in from black), speed 1.0 */
    fn_801C41C8(2, lbl_8047BA30);
    fn_801C40F0(1);

    /* Open and start the opening demo THP movie (no loop) */
    fn_801E189C(lbl_80266FE8, 0);

    /* Start opening BGM: sound ID 0x0495, no fade, max volume */
    fn_80165A20(0x0495, 0, 0x7F);
}

/* =======================================================================
 *  moviePlayAutoDemo / fn_80035F34
 *  Address: 0x80035F34, Size: 0x30
 *
 *  Assembly:
 *    lfs f1, lbl_8047BA30@sda21(r0)  ; f1 = 1.0
 *    li r3, 3
 *    bl fn_801C41C8                  ; fade mode 3 (special), speed 1.0
 *    li r3, 1
 *    bl fn_801C40F0                  ; enable fade
 *    blr
 *
 *  Note: This function only sets up the fade. The actual movie open
 *  for autodemo01.thp happens in a separate call chain. The pattern
 *  of "open movie -> wait -> cleanup" is handled by the caller.
 * ======================================================================= */
void moviePlayAutoDemo(void) {
    fn_801C41C8(3, lbl_8047BA30);
    fn_801C40F0(1);
}

/* =======================================================================
 *  movieStopAndCleanup / fn_80035E04
 *  Address: 0x80035E04, Size: 0xE0
 *
 *  Waits for THP playback to finish, then stops BGM, cleans up
 *  floor resources, and restores game state.
 *
 *  Assembly (simplified):
 *    ; Wait loop for THP player
 *    .wait:
 *      bl fn_800F0308            ; yield
 *      bl fn_801E1874            ; THPPlayerGetState
 *      clrlwi r0, r3, 24
 *      cmplwi r0, 1
 *      beq .wait
 *
 *    ; Stop BGM
 *    li r3, 1                    ; sound group 1 (BGM)
 *    li r4, 0                    ; fade time 0
 *    li r5, 0x7F                 ; volume 127
 *    bl fn_80165A20              ; sndPlay -- stops/fades BGM
 *
 *    ; Set game flag 0x08D0
 *    li r3, 0x08D0
 *    bl fn_80190528              ; GSflagSet
 *
 *    ; Unload floor resources
 *    li r3, 1
 *    li r4, 0
 *    bl fn_80113828
 *
 *    ; Reset floor allocation (0x59608)
 *    li r3, 0
 *    lis r4, 0x596
 *    addi r4, r4, 8
 *    bl fn_8011288C
 * ======================================================================= */
void movieStopAndCleanup(void) {
    /* Wait for any playing THP movie to finish */
    movieWaitForFinish();

    /* Stop BGM: group 1, fade 0, volume 127 */
    fn_80165A20(1, 0, 0x7F);

    /* Set game flag to mark movie as completed */
    fn_80190528(0x08D0);

    /* Unload floor resources that were active during movie */
    fn_80113828(1, 0);

    /* Reset floor allocation parameters */
    fn_8011288C(0, 0x59608);
}

/* =======================================================================
 *  moviePlayStaffRoll / fn_80035F64
 *  Address: 0x80035F64, Size: 0x25C
 *
 *  The staff roll is the most complex movie function. It:
 *    1. Waits for any current THP playback to finish
 *    2. Stops current BGM
 *    3. Checks floor state to determine the credits variant
 *    4. Queries game flags for special conditions
 *       (flag 0x0476 = "game completed", flag 0x0478 = post-game)
 *    5. Sets up battle/effect state for the credits scene
 *    6. Opens the appropriate movie
 *    7. Polls input to allow the player to skip
 *    8. Handles the "with Raikou/Entei/Suicune" special credits
 *       by checking further game flags
 *    9. Cleans up and restores state
 *
 *  Assembly (heavily abbreviated -- this is a 0x25C-byte function):
 *    ; Wait for THP player
 *    .waitLoop:
 *      bl fn_800F0308
 *    .checkState:
 *      bl fn_801E1874
 *      cmplwi r0, 1
 *      beq .waitLoop
 *
 *    ; Stop BGM
 *    li r3, 1; li r4, 0; li r5, 0x7F
 *    bl fn_80165A20
 *
 *    ; Check floor state
 *    bl fn_8011394C
 *    cmplwi r3, 0x76          ; floor 0x76 = credits floor?
 *    bne .skipSpecial
 *
 *    ; Check game flags
 *    li r3, 0x0476
 *    bl fn_801902E0            ; GSflagGet
 *    cmplwi r0, 1
 *    bne .skipSpecial
 *
 *    li r3, 0x0478
 *    bl fn_80190528            ; GSflagSet
 *
 *    ; Set up battle/effect state for credits scene
 *    li r3, 0; li r4, 1
 *    bl fn_80129280            ; allocate effect context
 *    ; ... extensive effect parameter setup ...
 *
 *    ; Poll input for skip
 *    li r3, 0; li r4, 0x3C; li r5, 0xAA; li r6, 0
 *    bl fn_8001E074
 *    ; if input detected, check for skip
 *    ...
 *
 *    ; Open staff roll movie
 *    lis r3, lbl_80267000@ha
 *    addi r3, r3, lbl_80267000@l  ; "movie/staffroll.thp"
 *    li r4, 0
 *    bl fn_801E189C
 *    ...
 * ======================================================================= */
void moviePlayStaffRoll(void) {
    u32 floorState;
    u8 flagVal;
    void* effectCtx;
    void* effectCtx2;
    s8 inputResult;
    void* savedCtx;

    /* Step 1: Wait for any current THP playback */
    movieWaitForFinish();

    /* Step 2: Stop BGM */
    fn_80165A20(1, 0, 0x7F);

    /* Step 3: Check floor state */
    floorState = (u32)fn_8011394C();
    if (floorState != 0x76) {
        goto openMovie;
    }

    /* Step 4: Check game flags for special credits conditions */
    flagVal = (u8)fn_801902E0(0x0476);
    if ((flagVal & 0xFF) != 1) {
        goto openMovie;
    }

    /* Set post-game flag */
    fn_80190528(0x0478);

    /* Step 5: Set up battle/effect state for credits scene */
    effectCtx = fn_80129280(0, 1);
    fn_80135030(effectCtx, 5, 2);
    fn_80135030(effectCtx, 7, 1);
    fn_80135030(effectCtx, 8, 1);

    fn_8012A450(0, 0x18, 1);

    savedCtx = fn_801D036C();

    effectCtx2 = fn_80129280(0, 0);

    /* Copy battle state data (0x3BFA iterations) */
    /* This is a large memcpy-like block transfer in the original assembly,
     * copying the entire battle context state. Simplified here. */
    {
        u32* src = (u32*)((u8*)savedCtx - 4);
        u32* dst = (u32*)((u8*)effectCtx2 - 4);
        u32 count;
        for (count = 0; count < 0x3BFA; count++) {
            u32 val1 = src[1];
            u32 val2 = src[2];
            dst[1] = val1;
            dst[2] = val2;
            src += 2;
            dst += 2;
        }
    }

    /* Load floor/transition for credits */
    fn_80106D3C(2, 0x444C, 1, 0);

    /* Poll input: wait for button press or timeout */
    inputResult = fn_8001E074(0, 0x3C, 0xAA, 0);

    if (inputResult != 0) {
        goto cleanup;
    }

    /* Check for extended credits (special pokemon conditions) */
    {
        u32 battleMode;
        void* queryResult;

        battleMode = (u32)fn_801D0748(3, 2, 0);
        if (battleMode != 3) {
            goto postCheck;
        }

        queryResult = (void*)fn_80135168(effectCtx, 4);
        if (queryResult == NULL) {
            goto postCheck;
        }

        /* Check if special pokemon pair matches */
        {
            void* pairA;
            void* pairB;

            effectCtx2 = fn_80129280((u32)savedCtx, 2);
            pairA = (void*)fn_8012A5B0(2, 0, 0);
            pairB = (void*)fn_8012A5B0(0, 2, 0);

            if (pairA != pairB) {
                queryResult = (void*)fn_80135168(savedCtx, 4);
                if (queryResult != NULL) {
                    goto postCheck;
                }
            }

            /* Load special credits floor */
            fn_80106D3C(2, 0x3C02, 1, 0);
            inputResult = fn_8001E074(0, 0x3C, 0xAA, 1);
        }
    }

postCheck:
    /* Restore original battle context */
    effectCtx2 = fn_80129280(0, 0);
    {
        u32* src = (u32*)((u8*)savedCtx - 4);
        u32* dst = (u32*)((u8*)effectCtx2 - 4);
        u32 count;
        for (count = 0; count < 0x3BFA; count++) {
            u32 val1 = src[1];
            u32 val2 = src[2];
            dst[1] = val1;
            dst[2] = val2;
            src += 2;
            dst += 2;
        }
    }

openMovie:
    /* Open the staff roll THP movie */
    fn_801E189C(lbl_80267000, 0);

    /* Wait for staff roll to finish playing */
    movieWaitForFinish();

    /* Stop sound and restore state */
    fn_80165A20(1, 0, 0x7F);
    fn_80190528(0x08D0);
    fn_80113828(1, 0);
    fn_8011288C(0, 0x59608);
    return;

cleanup:
    /* Early exit cleanup path */
    fn_80165A20(1, 0, 0x7F);
    fn_80190528(0x08D0);
    fn_80113828(1, 0);
    fn_8011288C(0, 0x59608);
}

/* =======================================================================
 *  moviePlayGSLogo (call site near 0x80036568)
 *
 *  Opens "movie/gs_logo.thp" via THPPlayerOpen with loop=0.
 *  Called as part of the boot sequence logo display.
 * ======================================================================= */
void moviePlayGSLogo(void) {
    fn_801E189C(lbl_8026702C, 0);
}

/* =======================================================================
 *  moviePlayTPCLogo (call site near 0x80036668)
 *
 *  Opens "movie/tpc.thp" via THPPlayerOpen with loop=0.
 *  Called as part of the boot sequence logo display.
 * ======================================================================= */
void moviePlayTPCLogo(void) {
    fn_801E189C(lbl_80267040, 0);
}

/* =========================================================================
 * Stubs for remaining movie functions (0x800361C0-0x800366A4)
 * ========================================================================= */

/* 0x50 | fn_800361C0 | call_sequence */
void fn_800361C0(void) {
    fn_801C41C8(0, 0);
    fn_801C40F0(0);
    fn_801E189C(0, 0);
    fn_80165A20(0, 0, 0);
}

/* 0x80036210 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80036210(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)&lbl_8047BA30;
    r3 = 0x3;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    return;
}
#pragma pop

/* 0x80036240 | 0x120 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80036240(void) {
    extern u8 lbl_802E50E0[];
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_800FF58C();
    extern void fn_8016597C();
    extern void fn_80166A28();
    extern void fn_8017B1AC();
    extern void fn_801E16D0();
    extern void fn_801E1810();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    goto L_8003630C;
L_8003625C: ;
    fn_8017B1AC();
    if ((s32)r3 == (s32)0xb) goto L_80036270;
    if ((s32)r3 != (s32)0x5) goto L_80036278;
L_80036270: ;
    ((void(*)(void))fn_800F0308)();
    goto L_8003630C;
L_80036278: ;
    r3 = 0x1;
    fn_800F7AF0();
    r31 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & r31;
    r0 = r0 & 0x1300;
    if ((u32)r0 == (u32)0x0) goto L_800362A4;
    fn_801E1810();
    goto L_8003631C;
L_800362A4: ;
    r31 = r30;
    if ((u32)r30 < (u32)0x21) goto L_800362B4;
    goto L_80036304;
L_800362B4: ;
    fn_801E16D0();
    if ((s32)r3 >= (s32)0x0) goto L_800362C4;
    goto L_80036304;
L_800362C4: ;
    r5 = r30 * 0x6;
    r4 = (u32)lbl_802E50E0;
    r0 = (u32)lbl_802E50E0;
    r30 = r0 + r5;
    r0 = *(u16*)((u8*)r30 + 0x0);
    if ((s32)r3 < (s32)r0) goto L_80036304;
    r3 = *(u16*)((u8*)r30 + 0x2);
    if ((u32)r3 == (u32)0x0) goto L_800362F0;
    fn_80166A28();
L_800362F0: ;
    r3 = *(u16*)((u8*)r30 + 0x4);
    if ((u32)r3 == (u32)0x0) goto L_80036300;
    fn_80166A28();
L_80036300: ;
    r31 = r31 + 0x1;
L_80036304: ;
    r30 = r31;
    ((void(*)(void))fn_800F0308)();
L_8003630C: ;
    ((void(*)(void))fn_801E1874)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8003625C;
L_8003631C: ;
    r3 = 0x1;
    r4 = 0x3e8;
    r5 = 0x0;
    r6 = 0x7f;
    fn_8016597C();
    r3 = 0x384;
    fn_800FF58C();
    r4 = (0x596 << 16);
    r3 = 0x0;
    r4 = r4 + 0x8;
    ((void(*)(void))fn_8011288C)();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x50 | fn_80036360 | call_sequence */
void fn_80036360(void) {
    fn_801C41C8(0, 0);
    fn_801C40F0(0);
    fn_801E189C(0, 0);
    fn_80165A20(0, 0, 0);
}

/* 0x800363B0 | 0x4 -- nop */
void fn_800363B0(void) { }

/* 0x800363B4 | 0x4 -- nop */
void fn_800363B4(void) { }

/* 0x800363B8 | 0x4 -- nop */
void fn_800363B8(void) { }

/* 0x800363BC | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800363BC(void) {
    extern u8 lbl_803A3E58[];
    extern u8 lbl_8047A460[];
    extern void fn_800A19CC();
    extern void fn_800A1F94();
    extern void fn_800370E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r3 = (u32)lbl_803A3E58;
    f1 = *(f32*)&lbl_8047BA30;
    r31 = (u32)lbl_803A3E58;
    r3 = 0x3;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = *(u32*)lbl_8047A460;
    r7 = 0x3;
    r6 = r31 + 0x1640;
    *(u32*)((u8*)r31 + 0x1318) = r7;
    r9 = r3 + 0x1;
    r5 = r31 + 0x1318;
    r0 = 0x0;
    r8 = 0x7;
    r3 = (u32)fn_800370E0;
    *(u32*)lbl_8047A460 = r9;
    r4 = (u32)fn_800370E0;
    r6 = r6 + 0xffc;
    *(u32*)((u8*)r5 + 0x4) = r8;
    r3 = r31 + 0x1328;
    r7 = 0x1000;
    r8 = 0x10;
    *(u32*)((u8*)r5 + 0x8) = r0;
    r9 = 0x1;
    *(u32*)((u8*)r5 + 0xC) = r0;
    fn_800A19CC();
    r3 = r31 + 0x1328;
    fn_800A1F94();
    goto L_80036448;
L_80036444: ;
    ((void(*)(void))fn_800F0308)();
L_80036448: ;
    r0 = *(u32*)lbl_8047A460;
    if ((s32)r0 != (s32)0x0) goto L_80036444;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x60 | fn_80036468 | generic */
u32 fn_80036468(void) {
    /* refs: lbl_8047A468 */
    fn_800F0308();
    fn_801E1874();
    fn_800FF58C();
    fn_8011288C(0, 0);
    return 0;
}

/* 0x800364C8 | 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800364C8(void) {
    extern u8 lbl_803A3E58[];
    extern u8 lbl_8047A460[];
    extern u8 lbl_8047A468[];
    extern void fn_800A19CC();
    extern void fn_800A1F94();
    extern void fn_800370E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r10 = 0x0;
    r3 = (u32)lbl_803A3E58;
    r31 = (u32)lbl_803A3E58;
    *(u8*)lbl_8047A468 = r10;
    r3 = *(u32*)lbl_8047A460;
    r5 = r31 + 0x1318;
    r7 = 0x5;
    r8 = 0x4;
    r9 = r3 + 0x1;
    r0 = 0x6;
    r3 = (u32)fn_800370E0;
    r6 = r31 + 0x1640;
    *(u32*)lbl_8047A460 = r9;
    r4 = (u32)fn_800370E0;
    r3 = r31 + 0x1328;
    r6 = r6 + 0xffc;
    *(u32*)((u8*)r31 + 0x1318) = r7;
    r7 = 0x1000;
    r9 = 0x1;
    *(u32*)((u8*)r5 + 0x4) = r8;
    r8 = 0x10;
    *(u32*)((u8*)r5 + 0x8) = r0;
    *(u32*)((u8*)r5 + 0xC) = r10;
    fn_800A19CC();
    r3 = r31 + 0x1328;
    fn_800A1F94();
    goto L_80036548;
L_80036544: ;
    ((void(*)(void))fn_800F0308)();
L_80036548: ;
    r0 = *(u32*)lbl_8047A460;
    if ((s32)r0 != (s32)0x0) goto L_80036544;
    f1 = *(f32*)&lbl_8047BA30;
    r3 = 0x2;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = (u32)&lbl_8026702C;
    r4 = 0x0;
    r3 = (u32)&lbl_8026702C;
    ((void(*)(void))fn_801E189C)();
    r3 = 0x4d1;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_80165A20)();
    r3 = (0x8000 << 16);
    r4 = 0x0;
    r3 = r3 + 0x1805;
    r5 = 0x17fb;
    memset((void*)r3, (int)r4, (u32)r5);
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x800365B0 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800365B0(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)&lbl_8047BA30;
    r3 = 0x3;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    return;
}
#pragma pop

/* 0x60 | fn_800365E0 | generic */
u32 fn_800365E0(void) {
    /* refs: lbl_8047A468 */
    fn_80165F40();
    fn_800F0308();
    fn_801E1874();
    fn_800FF58C();
    fn_8011288C(0, 0);
    return 0;
}

/* 0x5C | fn_80036640 | call_sequence */
void fn_80036640(void) {
    fn_801C41C8(0, 0);
    fn_801C40F0(0);
    fn_801E189C(0, 0);
    memset();
}

/* 0x8003669C | 0x4 -- nop */
void fn_8003669C(void) { }

/* 0x800366A0 | 0x4 -- nop */
void fn_800366A0(void) { }

/* 0x800366A4 | 0x4 -- nop */
void fn_800366A4(void) { }
