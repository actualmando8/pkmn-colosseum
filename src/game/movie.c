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
extern void fn_8011394C(void);                         /* floor state query */
extern void* fn_80129280(u32 a, u32 b);               /* battle/effect state setup */
extern void fn_80135030(void* ctx, u32 a, u32 b);     /* effect parameter set */
extern void fn_80135168(void* ctx, u32 a);             /* effect query */
extern void fn_8012A450(u32 a, u32 b, u32 c);         /* effect system control */
extern void fn_8012A5B0(u32 a, u32 b, u32 c);         /* effect system query */
extern void fn_80106D3C(u32 a, u32 b, u32 c, u32 d);  /* floor transition trigger */
extern s8   fn_8001E074(u32 a, u32 b, u32 c, u32 d);  /* input poll / wait */
extern void fn_801D036C(void);                         /* battle state query */
extern void fn_801D0748(u32 a, u32 b, u32 c);         /* battle mode set */

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

            effectCtx2 = fn_80129280(savedCtx, 2);
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
