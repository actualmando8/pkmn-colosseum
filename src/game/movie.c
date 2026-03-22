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

    } else {

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

/*
 * movieSetupFadeSpecial - Set fade mode 3 with speed 1.0 and enable.
 *
 * 0x80036210 | size: 0x30
 */
void fn_80036210(void) {
    fn_801C41C8(3, lbl_8047BA30);
    fn_801C40F0(1);
}

/*
 * moviePlayWithSubtitles - Play a THP movie with timed subtitle display.
 *
 * Polls the THP player state each frame. While playing, checks for
 * skip-button input and processes subtitle entries from the subtitle
 * table at lbl_802E50E0. Each entry is 6 bytes: 2-byte frame threshold,
 * 2-byte sound ID A, 2-byte sound ID B.
 *
 * 0x80036240 | size: 0x120
 */
void fn_80036240(void) {
    extern u8 lbl_802E50E0[];
    extern u32 fn_800F7AF0(u32 pad);
    extern u32 fn_800F7BC4(u32 pad);
    extern void fn_800FF58C(u32 flag);
    extern void fn_8016597C(u32 grp, u32 fade, u32 unk, u32 vol);
    extern void fn_80166A28(u32 sndId);
    extern s32 fn_8017B1AC(void);
    extern s32 fn_801E16D0(void);
    extern void fn_801E1810(void);
    u32 subtitleIdx = 0;
    u32 nextIdx;
    u8 state;
    s32 sysState;
    u32 btnA, btnB, combined;
    s32 framePos;
    u8* entry;
    u16 threshold, sndA, sndB;

    while (1) {
        state = fn_801E1874();
        if ((state & 0xFF) != 1) {
            break;
        }

        sysState = fn_8017B1AC();
        if (sysState == 0x0B || sysState == 5) {
            fn_800F0308();
            continue;
        }

        btnA = fn_800F7AF0(1);
        btnB = fn_800F7BC4(1);
        combined = btnA & btnB;
        if (combined & 0x1300) {
            fn_801E1810();
            break;
        }

        nextIdx = subtitleIdx;
        if (subtitleIdx < 0x21) {
            framePos = fn_801E16D0();
            if (framePos >= 0) {
                entry = lbl_802E50E0 + (subtitleIdx * 6);
                threshold = *(u16*)(entry + 0);
                if (framePos >= (s32)threshold) {
                    sndA = *(u16*)(entry + 2);
                    if (sndA != 0) {
                        fn_80166A28(sndA);
                    }
                    sndB = *(u16*)(entry + 4);
                    if (sndB != 0) {
                        fn_80166A28(sndB);
                    }
                    nextIdx = nextIdx + 1;
                }
            }
        }

        subtitleIdx = nextIdx;
        fn_800F0308();
    }

    fn_8016597C(1, 0x3E8, 0, 0x7F);
    fn_800FF58C(0x384);
    fn_8011288C(0, 0x59608);
}

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

/*
 * moviePlayTPCSequence - Set up TPC logo DMA load and wait for completion.
 *
 * Configures fade mode 3, sets up a DVD read task for logo data,
 * then yields until the task counter reaches zero.
 *
 * 0x800363BC | size: 0xAC
 */
void fn_800363BC(void) {
    extern u8 lbl_803A3E58[];
    extern u32 lbl_8047A460;
    extern void fn_800A19CC(u8* desc, void* callback, u8* buf, u32 size, u32 align, u32 async);
    extern void fn_800A1F94(u8* desc);
    extern void fn_800370E0(void);
    u8* base = lbl_803A3E58;
    u8* taskDesc = base + 0x1318;
    u8* buffer = base + 0x1640 + 0xFFC;

    /* Fade mode 3 (special), speed 1.0 */
    fn_801C41C8(3, lbl_8047BA30);
    fn_801C40F0(1);

    /* Set up task descriptor */
    *(u32*)(taskDesc + 0x00) = 3;   /* type */
    *(u32*)(taskDesc + 0x04) = 7;   /* priority */
    *(u32*)(taskDesc + 0x08) = 0;   /* offset */
    *(u32*)(taskDesc + 0x0C) = 0;   /* flags */

    lbl_8047A460 = lbl_8047A460 + 1;

    fn_800A19CC(base + 0x1328, (void*)fn_800370E0, buffer, 0x1000, 0x10, 1);
    fn_800A1F94(base + 0x1328);

    /* Wait for task to complete */
    while ((s32)lbl_8047A460 != 0) {
        fn_800F0308();
    }
}

/* 0x60 | fn_80036468 | generic */
u32 fn_80036468(void) {
    /* refs: lbl_8047A468 */
    fn_800F0308();
    fn_801E1874();
    fn_800FF58C();
    fn_8011288C(0, 0);
    return 0;
}

/*
 * moviePlayGSLogoSequence - Set up GS logo DMA read, wait, then play logo movie.
 *
 * Initializes state, configures a DVD read task, waits for completion,
 * then fades in and opens the GS logo movie with BGM 0x4D1.
 *
 * 0x800364C8 | size: 0xE8
 */
void fn_800364C8(void) {
    extern u8 lbl_803A3E58[];
    extern u32 lbl_8047A460;
    extern u8 lbl_8047A468;
    extern void fn_800A19CC(u8* desc, void* callback, u8* buf, u32 size, u32 align, u32 async);
    extern void fn_800A1F94(u8* desc);
    extern void fn_800370E0(void);
    u8* base = lbl_803A3E58;
    u8* taskDesc = base + 0x1318;
    u8* buffer = base + 0x1640 + 0xFFC;

    lbl_8047A468 = 0;

    /* Set up task descriptor */
    *(u32*)(taskDesc + 0x00) = 5;   /* type */
    *(u32*)(taskDesc + 0x04) = 4;   /* priority */
    *(u32*)(taskDesc + 0x08) = 6;   /* offset */
    *(u32*)(taskDesc + 0x0C) = 0;   /* flags */

    lbl_8047A460 = lbl_8047A460 + 1;

    fn_800A19CC(base + 0x1328, (void*)fn_800370E0, buffer, 0x1000, 0x10, 1);
    fn_800A1F94(base + 0x1328);

    /* Wait for task to complete */
    while ((s32)lbl_8047A460 != 0) {
        fn_800F0308();
    }

    /* Fade in and play GS logo movie */
    fn_801C41C8(2, lbl_8047BA30);
    fn_801C40F0(1);
    fn_801E189C(lbl_8026702C, 0);
    fn_80165A20(0x4D1, 0, 0x7F);

    /* Clear low memory area */
    memset((void*)0x80001805, 0, 0x17FB);
}

/*
 * movieSetupFadeSpecial2 - Set fade mode 3 with speed 1.0 and enable.
 *
 * 0x800365B0 | size: 0x30
 */
void fn_800365B0(void) {
    fn_801C41C8(3, lbl_8047BA30);
    fn_801C40F0(1);
}

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
