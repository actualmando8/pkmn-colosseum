/**
 * @file gs_title.c
 * @brief GStitle -- Title screen, autodemo, and intro sequence.
 *
 * Address range: 0x80020328 - 0x80026000 (~80 functions)
 *
 * This module manages the title screen and pre-game sequences:
 *   - Title logo display and animation
 *   - "Press Start" prompt with blink animation
 *   - Autodemo playback timer and trigger
 *   - New Game / Continue selection
 *   - Intro cutscene orchestration
 *   - Camera fly-through for title background
 *   - Sound effect scheduling for title events
 *
 * Key functions:
 *   fn_80020328  GStitle_Cleanup              -- 0x8C bytes, clean up title resources
 *   fn_800203B4  GStitle_MainLoop             -- 0xE8 bytes, title screen main loop
 *   fn_8002049C  GStitle_Init                 -- 0xF0 bytes, initialize title screen
 *   fn_8002058C  GStitle_ThreadEntry          -- 0x2C bytes, title thread entry point
 *   fn_800205B8  GStitle_SetMode              -- 8 bytes, set title mode (stw + blr)
 *   fn_800205C0  GStitle_SetDemoPtr           -- 8 bytes, set autodemo pointer
 *   fn_800205C8  GStitle_CheckAutodemo        -- 0x44 bytes, check autodemo timer
 *   fn_8002060C  GStitle_ResetTimer           -- 0xC bytes, reset autodemo countdown
 *   fn_80020618  GStitle_RenderFrame          -- 0x304 bytes, render one title frame
 *   fn_8002091C  GStitle_GetState             -- 0x10 bytes, return title state
 *   fn_8002092C  GStitle_AnimateLogo          -- 0x90 bytes, logo bounce/spin
 *   fn_800209BC  GStitle_AnimatePrompt        -- 0x90 bytes, "Press Start" blink
 *   fn_80020A4C  GStitle_AnimateBackground    -- 0x90 bytes, background scroll
 *   fn_80020ADC  GStitle_FadeIn               -- 0x58 bytes, fade from black
 *   fn_80020B34  GStitle_FadeOut              -- 0x58 bytes, fade to black
 *   fn_80020B8C  GStitle_GetFadeState         -- 0x14 bytes, return fade progress
 *   fn_80020BA0  GStitle_HandleInput          -- 0xFC bytes, Start/A button handler
 *   fn_80020C9C  GStitle_MenuSelect           -- 0x200 bytes, New/Continue/Options menu
 *   fn_80020E9C  GStitle_ReturnZero           -- 8 bytes, stub
 *   fn_80020EA4  GStitle_OptionsMenu          -- 0xB0 bytes, options sub-menu
 *   fn_80020F54  GStitle_SaveFileSelect       -- 0x19C bytes, save file selection
 *   fn_800210F0  GStitle_LoadSaveFile         -- 0x4D4 bytes, load save from memcard
 *   fn_800215C4  GStitle_NewGameSetup         -- 0x60 bytes, initialize new game
 *   fn_80021624  GStitle_SetDifficulty        -- 0x20 bytes, set game difficulty
 *   fn_80021644  GStitle_IntroSequence        -- 0x9C bytes, start intro sequence
 *   fn_800216E0  GStitle_SkipIntro            -- 8 bytes, set skip flag
 *   fn_800216E8  GStitle_NameEntry            -- 0x1D4 bytes, player name input screen
 *   fn_800218BC  GStitle_NameEntryInput       -- 0x1E0 bytes, keyboard input handler
 *   fn_80021A9C  GStitle_NameEntryConfirm     -- 0x78 bytes, confirm name
 *   fn_80021B14  GStitle_NameEntryDraw        -- 0x53C bytes, render keyboard UI
 *   fn_80022050  GStitle_DrawKeyboard         -- 0x12C bytes, keyboard grid
 *   fn_8002217C  GStitle_DrawNamePreview      -- 0x2FC bytes, live name preview
 *   fn_80022478  GStitle_DrawCaseSwitch       -- 0x2A8 bytes, upper/lower case switch
 *   fn_80022720  GStitle_CameraFlythrough     -- 0x114 bytes, title camera movement
 *   fn_80022834  GStitle_CameraInterpolate    -- 0x308 bytes, camera spline interp
 *   fn_80022B3C  GStitle_ParticleEffects      -- 0x318 bytes, title particle system
 *   fn_80022E54  GStitle_GetEffectCount       -- 0x90 bytes, count active particles
 *   fn_80022EE4  GStitle_SpawnEffect          -- 0x184 bytes, spawn new particle
 *   fn_80023068  GStitle_UpdateEffects        -- 0x20C bytes, update particle positions
 *   fn_80023274  GStitle_DestroyEffect        -- 0x7C bytes, remove expired particle
 *   fn_800232F0  GStitle_SoundScheduler       -- 0x470 bytes, title BGM/SE scheduling
 *   fn_80023760  GStitle_CrossfadeBGM         -- 0x208 bytes, BGM crossfade
 *   fn_80023968  GStitle_PlayTitleBGM         -- 0x234 bytes, play title music
 *   fn_80023B9C  GStitle_StopTitleBGM         -- 0x20C bytes, fade out title music
 *   fn_80023DA8  GStitle_GetBGMState          -- 0x3C bytes
 *   fn_80023DE4  GStitle_GetSEState           -- 0x3C bytes
 *   fn_80023E20  GStitle_GetFadeState2        -- 0x3C bytes
 *   fn_80023E5C  GStitle_Nop                  -- 4 bytes, nop
 *   fn_80023E60  GStitle_AutodemoPlayback     -- 0x300 bytes, replay input recording
 *   fn_80024160  GStitle_AutodemoRecord       -- 0x1A8 bytes, record input (debug)
 *   fn_80024308  GStitle_AutodemoSetup        -- 0x130 bytes, setup autodemo
 *
 * fn_800203B4 (GStitle_MainLoop) structure:
 *   while (1) {
 *       if (gTitleState == 0x28) {  // TITLE_STATE_AUTODEMO_TIMEOUT
 *           fn_80113828(0x39C, 0);  // load autodemo camera
 *           gTitleState = 0x3E8;   // TITLE_STATE_PLAY_AUTODEMO
 *           continue;
 *       }
 *       if (!gIsInitialized) continue;
 *       if (gDemoPtr == NULL) continue;
 *       // Accumulate camera float position
 *       gCamAngle += gCamDelta;
 *       if (gCamPhase < 2 && !gPaused) {
 *           // Check distance threshold for camera movement
 *           if (dist >= threshold[gCamPhase]) {
 *               if (gCamPhase == 0)
 *                   fn_80166AB8(0x46E, 0, 0);  // Play whoosh SE
 *               else
 *                   fn_801669E4(0x46E, 0, 0);  // Play landing SE
 *               gCamPhase++;
 *           }
 *       }
 *       fn_800F0308();  // Frame advance
 *   }
 *
 * fn_8002049C (GStitle_Init):
 *   - Clears stale state flags
 *   - Creates the title thread via fn_800F07A8 (priority 0x14, stack 0x2000)
 *   - Initializes camera position from lbl_803A1F88 float constants
 *   - Checks save file existence via fn_800FF548
 *   - Calls fn_8002060C if no save found
 *
 * SDA globals:
 *   lbl_8047A310: Title mode (s32)
 *   lbl_8047A314: Frame counter for autodemo timer
 *   lbl_8047A31C: Current title state (s32)
 *   lbl_8047A320: Initialized flag
 *   lbl_8047A324: Demo data pointer
 *   lbl_8047A328: Pause flag
 *   lbl_8047A32C: Thread completion flag (u8)
 *   lbl_8047A330: Title thread handle
 *   lbl_8047A33C: Camera phase counter
 *   lbl_8047A340: Camera angle accumulator (float)
 *   lbl_8047A344: Camera delta per frame (float)
 *   lbl_8047A350: Autodemo playback pointer
 *   lbl_80478878: Frame cycle counter (wraps at 4)
 *
 * BSS globals:
 *   lbl_803A1F88: Camera position array (4 entries, 4 floats each)
 *
 * Rodata (sdata2 float constants):
 *   lbl_8047B814: 0.0f (zero)
 *   lbl_8047B84C: Camera X start position
 *   lbl_8047B850: Camera Y start position
 *   lbl_8047B854: Camera Z position
 *   lbl_8047B858: Camera target X
 *   lbl_8047B85C: Camera target Y
 *   lbl_8047B860: Camera target Z
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Thread management */
extern void  fn_800F07A8(s32 priority, void* stack, s32 stackSize,
                          s32 flags, s32 p5, void* entry);
extern void  fn_800F0308(void);
extern void  fn_800F05A0(void* threadCtx);

/* Scene/camera */
extern void  fn_80113828(s32 cameraId, s32 mode);
extern u8    fn_800FF548(void);
extern void  fn_800FF56C(s32 floorId);

/* Sound */
extern void  fn_80166AB8(s32 soundId, s32 p2, s32 p3);
extern void  fn_801669E4(s32 soundId, s32 p2, s32 p3);

/* Save system */
extern void  fn_8011288C(s32 p1, s32 p2);

/* Input */
extern void  fn_801EF644(s32 result);

/* Math */
extern f32   fn_800EC53C(void);               /* Get distance */

/* =========================================================================
 * SDA globals
 * ========================================================================= */

extern s32   gTitleMode;         /* lbl_8047A310 */
extern s32   gTitleState;        /* lbl_8047A31C */
extern s32   gTitleInitFlag;     /* lbl_8047A320 */
extern void* gAutodemoPtr;       /* lbl_8047A324 */
extern s32   gTitlePaused;       /* lbl_8047A328 */
extern u8    gTitleThreadDone;   /* lbl_8047A32C */
extern void* gTitleThreadHandle; /* lbl_8047A330 */
extern s32   gCameraPhase;      /* lbl_8047A33C */
extern f32   gCameraAngle;      /* lbl_8047A340 */
extern f32   gCameraDelta;      /* lbl_8047A344 */
extern void* gDemoPlaybackPtr;   /* lbl_8047A350 */
extern s32   gFrameCycleCount;   /* lbl_80478878 */

/* =========================================================================
 * Function: GStitle_Init
 * Address:  0x8002049C
 * Size:     0xF0
 * ========================================================================= */

/* =========================================================================
 * Function: GStitle_MainLoop
 * Address:  0x800203B4
 * Size:     0xE8
 * ========================================================================= */

/* =========================================================================
 * Function: GStitle_Cleanup
 * Address:  0x80020328
 * Size:     0x8C
 *
 * Cancels active events on slots 0x13, 0x15, 0x16, closes any open
 * message boxes, then waits for the title thread to complete.
 * ========================================================================= */

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 4 functions matched
 * =================================================================== */

extern u32 lbl_8047A310;
extern u32 lbl_8047A350;
extern u32 lbl_8047A358;

/* Address: 0x800205B8 | Size: 0x8 | Pattern: sda_setter */
void fn_800205B8(u32 val) {
    lbl_8047A310 = val;
}

/* Address: 0x800205C0 | Size: 0x8 | Pattern: sda_setter */
void fn_800205C0(u32 val) {
    lbl_8047A350 = val;
}

/* Address: 0x80020E9C | Size: 0x8 | Pattern: return_constant */
u32 fn_80020E9C(void) { return 0; }

/* Address: 0x800216E0 | Size: 0x8 | Pattern: sda_setter */
void fn_800216E0(u32 val) {
    lbl_8047A358 = val;
}

/* =========================================================================
 * Stubs for remaining GStitle functions (0x80024438-0x80025F84)
 * ========================================================================= */

/* 0x4C | fn_80024438 | check_then_call */
void fn_80024438(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_80024484 | check_then_call */
void fn_80024484(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_800244D0 | check_then_call */
void fn_800244D0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_8002451C | check_then_call */
void fn_8002451C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_80024568 | check_then_call */
void fn_80024568(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_800245B4 | check_then_call */
void fn_800245B4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_80024600 | check_then_call */
void fn_80024600(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x4C | fn_8002464C | check_then_call */
void fn_8002464C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    { fn_801902E0(); return; }
    fn_80109220();
}

/* 0x80024698 | 0x4 -- nop */
void fn_80024698(void) { }

/* 0x8002469C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8002469C(void) {
    extern void fn_800FA280();
    extern void fn_80132A38();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x3cdf;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    return;
}
#pragma pop

/* 0x800246CC | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800246CC(void) {
    extern void fn_800FA280();
    extern void fn_80132A38();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x3ce4;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    return;
}
#pragma pop

/* 0x800246FC | 0x330 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800246FC(void) {
    extern u8 lbl_802E4F58[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_80478DDC[];
    extern u8 lbl_80478DE0[];
    extern u8 lbl_80478DE4[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A36C[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047B8D8[];
    extern u8 lbl_8047B8DC[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800FA280();
    extern void fn_80132A38();
    extern void fn_801902E0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* stmw r27, 0x1c(r1) */;
    r28 = r3;
    r30 = r4;
    r31 = 0x0;
    r27 = 0x0;
    goto L_800249F4;
L_80024720: ;
    r3 = *(u32*)lbl_80478DE4;
    r3 = *(u32*)(r3 + r27);
    if ((u32)r3 == (u32)0x0) goto L_80024740;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800249EC;
L_80024740: ;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_80024750;
    goto L_800248F0;
L_80024750: ;
    f1 = *(f32*)lbl_80478898;
    f0 = *(f32*)lbl_8047B8D8;
    if (f1 <= f0) goto L_80024828;
    r29 = *(u32*)lbl_8047A368;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r28 = 0x0;
L_80024778: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_8002479C;
    if ((s32)r29 != (s32)r28) goto L_80024798;
    r28 = r3;
    goto L_800247BC;
L_80024798: ;
    r28 = r28 + 0x1;
L_8002479C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_800247B8;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024778;
L_800247B8: ;
    r28 = 0x0;
L_800247BC: ;
    r29 = 0x0;
    r27 = r29;
    goto L_800247EC;
L_800247C8: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r27 + 0x8;
    r3 = *(u32*)(r3 + r0);
    fn_8005D934();
    if ((u32)r28 != (u32)r3) goto L_800247E4;
    goto L_80024800;
L_800247E4: ;
    r27 = r27 + 0x10;
    r29 = r29 + 0x1;
L_800247EC: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 < (u32)r0) goto L_800247C8;
    r29 = 0x0;
L_80024800: ;
    f0 = *(f32*)lbl_80478898;
    f1 = *(f32*)lbl_8047B8D8;
    f2 = *(f32*)lbl_8047B8DC;
    f0 = f0 - f1;
    f0 = f0 / f1;
    f0 = f2 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    r0 = *(u32*)(sp + 0xC);
    goto L_80024994;
L_80024828: ;
    r29 = *(u32*)lbl_8047A36C;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r28 = 0x0;
L_80024840: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80024864;
    if ((s32)r29 != (s32)r28) goto L_80024860;
    r28 = r3;
    goto L_80024884;
L_80024860: ;
    r28 = r28 + 0x1;
L_80024864: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80024880;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024840;
L_80024880: ;
    r28 = 0x0;
L_80024884: ;
    r29 = 0x0;
    r27 = r29;
    goto L_800248B4;
L_80024890: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r27 + 0x8;
    r3 = *(u32*)(r3 + r0);
    fn_8005D934();
    if ((u32)r28 != (u32)r3) goto L_800248AC;
    goto L_800248C8;
L_800248AC: ;
    r27 = r27 + 0x10;
    r29 = r29 + 0x1;
L_800248B4: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 < (u32)r0) goto L_80024890;
    r29 = 0x0;
L_800248C8: ;
    f1 = *(f32*)lbl_8047B8D8;
    f0 = *(f32*)lbl_80478898;
    f2 = *(f32*)lbl_8047B8DC;
    f0 = f1 - f0;
    f0 = f0 / f1;
    f0 = f2 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    r0 = *(u32*)(sp + 0xC);
    goto L_80024994;
L_800248F0: ;
    r29 = *(u32*)lbl_8047A368;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r28 = 0x0;
L_80024908: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_8002492C;
    if ((s32)r29 != (s32)r28) goto L_80024928;
    r28 = r3;
    goto L_8002494C;
L_80024928: ;
    r28 = r28 + 0x1;
L_8002492C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80024948;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024908;
L_80024948: ;
    r28 = 0x0;
L_8002494C: ;
    r29 = 0x0;
    r27 = r29;
    goto L_8002497C;
L_80024958: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r27 + 0x8;
    r3 = *(u32*)(r3 + r0);
    fn_8005D934();
    if ((u32)r28 != (u32)r3) goto L_80024974;
    goto L_80024990;
L_80024974: ;
    r27 = r27 + 0x10;
    r29 = r29 + 0x1;
L_8002497C: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 < (u32)r0) goto L_80024958;
    r29 = 0x0;
L_80024990: ;
    r0 = 0xff;
L_80024994: ;
    *(u8*)((u8*)r30 + 0x67) = r0;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 >= (u32)r0) goto L_80024A04;
    if ((u32)r29 <= (u32)0x9) goto L_800249B4;
    r29 = 0x0;
L_800249B4: ;
    r3 = (u32)lbl_802E4F58;
    r5 = *(u32*)lbl_80478DE4;
    r4 = r31 * 0x28;
    r3 = (u32)lbl_802E4F58;
    r0 = *(u8*)(r3 + r29);
    r3 = r5 + r4;
    r0 = r0 << 2;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    goto L_80024A18;
L_800249EC: ;
    r27 = r27 + 0x28;
    r31 = r31 + 0x1;
L_800249F4: ;
    r3 = *(u32*)lbl_80478DE0;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r31 < (u32)r0) goto L_80024720;
L_80024A04: ;
    r3 = 0x1;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
L_80024A18: ;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x80024A2C | 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80024A2C(void) {
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_80478DDC[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047B8A8[];
    extern u8 lbl_8047B8DC[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_801902E0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r31 = r4;
    r30 = *(u32*)lbl_8047A368;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80024A64: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80024A88;
    if ((s32)r30 != (s32)r29) goto L_80024A84;
    r29 = r3;
    goto L_80024AA8;
L_80024A84: ;
    r29 = r29 + 0x1;
L_80024A88: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80024AA4;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024A64;
L_80024AA4: ;
    r29 = 0x0;
L_80024AA8: ;
    r30 = 0x0;
    r28 = r30;
    goto L_80024AD8;
L_80024AB4: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r28 + 0x8;
    r3 = *(u32*)(r3 + r0);
    fn_8005D934();
    if ((u32)r29 != (u32)r3) goto L_80024AD0;
    goto L_80024AEC;
L_80024AD0: ;
    r28 = r28 + 0x10;
    r30 = r30 + 0x1;
L_80024AD8: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r30 < (u32)r0) goto L_80024AB4;
    r30 = 0x0;
L_80024AEC: ;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_80024AFC;
    goto L_80024B24;
L_80024AFC: ;
    f1 = *(f32*)lbl_80478898;
    f0 = *(f32*)lbl_8047B8A8;
    f2 = *(f32*)lbl_8047B8DC;
    f0 = f1 / f0;
    f0 = f2 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    r0 = *(u32*)(sp + 0xC);
    *(u8*)((u8*)r31 + 0x67) = r0;
    goto L_80024B2C;
L_80024B24: ;
    r0 = 0xff;
    *(u8*)((u8*)r31 + 0x67) = r0;
L_80024B2C: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r30 >= (u32)r0) goto L_80024B84;
    r0 = *(u32*)lbl_80478DDC;
    r28 = r30 << 4;
    r3 = r0 + r28;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)0x66) goto L_80024B74;
    r3 = 0x45d;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80024B74;
    r3 = (0xc5f << 16);
    r0 = r3 + 0x1200;
    goto L_80024B80;
L_80024B74: ;
    r0 = *(u32*)lbl_80478DDC;
    r3 = r0 + r28;
    r0 = *(u32*)((u8*)r3 + 0xC);
L_80024B80: ;
    *(u32*)((u8*)r31 + 0x58) = r0;
L_80024B84: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80024BA4 | 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80024BA4(void) {
    extern u8 lbl_80478DD8[];
    extern u8 lbl_80478DDC[];
    extern u8 lbl_8047A36C[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_801902E0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r30 = *(u32*)lbl_8047A36C;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80024BDC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80024C00;
    if ((s32)r30 != (s32)r29) goto L_80024BFC;
    r29 = r3;
    goto L_80024C20;
L_80024BFC: ;
    r29 = r29 + 0x1;
L_80024C00: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80024C1C;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024BDC;
L_80024C1C: ;
    r29 = 0x0;
L_80024C20: ;
    r30 = 0x0;
    r28 = r30;
    goto L_80024C50;
L_80024C2C: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r28 + 0x8;
    r3 = *(u32*)(r3 + r0);
    fn_8005D934();
    if ((u32)r29 != (u32)r3) goto L_80024C48;
    goto L_80024C64;
L_80024C48: ;
    r28 = r28 + 0x10;
    r30 = r30 + 0x1;
L_80024C50: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r30 < (u32)r0) goto L_80024C2C;
    r30 = 0x0;
L_80024C64: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r30 >= (u32)r0) goto L_80024CBC;
    r0 = *(u32*)lbl_80478DDC;
    r28 = r30 << 4;
    r3 = r0 + r28;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)0x66) goto L_80024CAC;
    r3 = 0x45d;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80024CAC;
    r3 = (0xc5f << 16);
    r0 = r3 + 0x1200;
    goto L_80024CB8;
L_80024CAC: ;
    r0 = *(u32*)lbl_80478DDC;
    r3 = r0 + r28;
    r0 = *(u32*)((u8*)r3 + 0xC);
L_80024CB8: ;
    *(u32*)((u8*)r31 + 0x58) = r0;
L_80024CBC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80024CDC | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80024CDC(void) {
    extern u8 lbl_8047A37C[];
    extern u8 lbl_8047A390[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8D0[];
    extern void fn_800D3088();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r30 = r4;
    r3 = *(u32*)lbl_8047A390;
    if ((u32)r3 == (u32)0x0) goto L_80024D24;
    r3 = *(s16*)((u8*)r3 + 0x2);
    r0 = r3 + 0x8;
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x50) = r0;
    r3 = *(u32*)lbl_8047A390;
    r3 = *(s16*)((u8*)r3 + 0x4);
    r0 = r3 + 0x8;
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x52) = r0;
L_80024D24: ;
    r31 = *(u8*)((u8*)r30 + 0x67);
    fn_800D3088();
    r4 = (0x4330 << 16);
    /* xoris r0, r31, 0x8000 */;
    f2 = *(f64*)lbl_8047B8D0;
    f1 = *(f64*)lbl_8047B8B8;
    f0 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f2 = f0 - f2;
    f3 = *(f32*)lbl_8047A37C;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f0 = f3 * f2 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r31 = *(u32*)(sp + 0x1C);
    if ((s32)r31 >= (s32)0x40) goto L_80024D88;
    f0 = -f3;
    r31 = 0x40;
    *(f32*)lbl_8047A37C = f0;
    goto L_80024D9C;
L_80024D88: ;
    if ((s32)r31 <= (s32)0xff) goto L_80024D9C;
    f0 = -f3;
    r31 = 0xff;
    *(f32*)lbl_8047A37C = f0;
L_80024D9C: ;
    r0 = r31 & 0xFF;
    *(u8*)((u8*)r30 + 0x67) = r0;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    return;
}
#pragma pop

/* 0x80024DBC | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80024DBC(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047A374[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8E0[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    /* stmw r27, 0x3c(r1) */;
    r30 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_80024DE8;
    if ((s32)r0 >= (s32)0x1) goto L_80024E2C;
    goto L_80024E2C;
L_80024DE8: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f1;
    *(f64*)(sp + 0x20) = f0;
    r28 = *(u32*)(sp + 0x1C);
    r31 = *(u32*)(sp + 0x24);
    goto L_80024EA4;
L_80024E2C: ;
    r4 = *(u32*)lbl_80478DD8;
    r27 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r27 >= (u32)r0) goto L_80024EA4;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80024E54: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80024E74;
    if ((s32)r27 != (s32)r29) goto L_80024E70;
    goto L_80024E94;
L_80024E70: ;
    r29 = r29 + 0x1;
L_80024E74: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80024E90;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024E54;
L_80024E90: ;
    r3 = 0x0;
L_80024E94: ;
    if ((u32)r3 == (u32)0x0) goto L_80024EA4;
    r28 = *(s16*)((u8*)r3 + 0x2);
    r31 = *(s16*)((u8*)r3 + 0x4);
L_80024EA4: ;
    r0 = (s16)r28;
    r3 = (0x4330 << 16);
    /* xoris r4, r0, 0x8000 */;
    r0 = (s16)r31;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047B8B8;
    f0 = *(f32*)lbl_8047A374;
    f1 = *(f64*)(sp + 0x20);
    f4 = *(f32*)lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = r0;
    f1 = f1 + f0;
    f0 = *(f64*)(sp + 0x28);
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u16*)((u8*)r30 + 0x50) = r0;
    f0 = *(f32*)lbl_8047A374;
    f0 = f1 + f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r30 + 0x52) = r0;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x80024F2C | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80024F2C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047A374[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8E0[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    /* stmw r27, 0x3c(r1) */;
    r30 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_80024F58;
    if ((s32)r0 >= (s32)0x1) goto L_80024F9C;
    goto L_80024F9C;
L_80024F58: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f1;
    *(f64*)(sp + 0x20) = f0;
    r28 = *(u32*)(sp + 0x1C);
    r31 = *(u32*)(sp + 0x24);
    goto L_80025014;
L_80024F9C: ;
    r4 = *(u32*)lbl_80478DD8;
    r27 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r27 >= (u32)r0) goto L_80025014;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80024FC4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80024FE4;
    if ((s32)r27 != (s32)r29) goto L_80024FE0;
    goto L_80025004;
L_80024FE0: ;
    r29 = r29 + 0x1;
L_80024FE4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80025000;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80024FC4;
L_80025000: ;
    r3 = 0x0;
L_80025004: ;
    if ((u32)r3 == (u32)0x0) goto L_80025014;
    r28 = *(s16*)((u8*)r3 + 0x2);
    r31 = *(s16*)((u8*)r3 + 0x4);
L_80025014: ;
    r0 = (s16)r28;
    r3 = (0x4330 << 16);
    /* xoris r4, r0, 0x8000 */;
    r0 = (s16)r31;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047B8B8;
    f0 = *(f32*)lbl_8047A374;
    f1 = *(f64*)(sp + 0x20);
    f4 = *(f32*)lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = r0;
    f1 = f1 + f0;
    f0 = *(f64*)(sp + 0x28);
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u16*)((u8*)r30 + 0x50) = r0;
    f0 = *(f32*)lbl_8047A374;
    f0 = f1 - f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r30 + 0x52) = r0;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x8002509C | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8002509C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047A374[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8E0[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    /* stmw r27, 0x3c(r1) */;
    r30 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_800250C8;
    if ((s32)r0 >= (s32)0x1) goto L_8002510C;
    goto L_8002510C;
L_800250C8: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f1;
    *(f64*)(sp + 0x20) = f0;
    r28 = *(u32*)(sp + 0x1C);
    r31 = *(u32*)(sp + 0x24);
    goto L_80025184;
L_8002510C: ;
    r4 = *(u32*)lbl_80478DD8;
    r27 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r27 >= (u32)r0) goto L_80025184;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80025134: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80025154;
    if ((s32)r27 != (s32)r29) goto L_80025150;
    goto L_80025174;
L_80025150: ;
    r29 = r29 + 0x1;
L_80025154: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80025170;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80025134;
L_80025170: ;
    r3 = 0x0;
L_80025174: ;
    if ((u32)r3 == (u32)0x0) goto L_80025184;
    r28 = *(s16*)((u8*)r3 + 0x2);
    r31 = *(s16*)((u8*)r3 + 0x4);
L_80025184: ;
    r0 = (s16)r28;
    r3 = (0x4330 << 16);
    /* xoris r4, r0, 0x8000 */;
    r0 = (s16)r31;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047B8B8;
    f0 = *(f32*)lbl_8047A374;
    f1 = *(f64*)(sp + 0x20);
    f4 = *(f32*)lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = r0;
    f1 = f1 - f0;
    f0 = *(f64*)(sp + 0x28);
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u16*)((u8*)r30 + 0x50) = r0;
    f0 = *(f32*)lbl_8047A374;
    f0 = f1 + f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r30 + 0x52) = r0;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x8002520C | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8002520C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047A374[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8E0[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    /* stmw r27, 0x3c(r1) */;
    r30 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_80025238;
    if ((s32)r0 >= (s32)0x1) goto L_8002527C;
    goto L_8002527C;
L_80025238: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f1;
    *(f64*)(sp + 0x20) = f0;
    r28 = *(u32*)(sp + 0x1C);
    r31 = *(u32*)(sp + 0x24);
    goto L_800252F4;
L_8002527C: ;
    r4 = *(u32*)lbl_80478DD8;
    r27 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r27 >= (u32)r0) goto L_800252F4;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_800252A4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_800252C4;
    if ((s32)r27 != (s32)r29) goto L_800252C0;
    goto L_800252E4;
L_800252C0: ;
    r29 = r29 + 0x1;
L_800252C4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_800252E0;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_800252A4;
L_800252E0: ;
    r3 = 0x0;
L_800252E4: ;
    if ((u32)r3 == (u32)0x0) goto L_800252F4;
    r28 = *(s16*)((u8*)r3 + 0x2);
    r31 = *(s16*)((u8*)r3 + 0x4);
L_800252F4: ;
    r0 = (s16)r28;
    r3 = (0x4330 << 16);
    /* xoris r4, r0, 0x8000 */;
    r0 = (s16)r31;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047B8B8;
    f0 = *(f32*)lbl_8047A374;
    f1 = *(f64*)(sp + 0x20);
    f4 = *(f32*)lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = r0;
    f1 = f1 - f0;
    f0 = *(f64*)(sp + 0x28);
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u16*)((u8*)r30 + 0x50) = r0;
    f0 = *(f32*)lbl_8047A374;
    f0 = f1 - f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r30 + 0x52) = r0;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x8002537C | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8002537C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_800253B4;
    if ((s32)r0 >= (s32)0x1) goto L_800253E8;
    goto L_800253E8;
L_800253B4: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f0 = *(f32*)(sp + 0x8);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r29 = *(u32*)(sp + 0x1C);
    goto L_8002545C;
L_800253E8: ;
    r4 = *(u32*)lbl_80478DD8;
    r28 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r28 >= (u32)r0) goto L_8002545C;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r30 = 0x0;
L_80025410: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80025430;
    if ((s32)r28 != (s32)r30) goto L_8002542C;
    goto L_80025450;
L_8002542C: ;
    r30 = r30 + 0x1;
L_80025430: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_8002544C;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80025410;
L_8002544C: ;
    r3 = 0x0;
L_80025450: ;
    if ((u32)r3 == (u32)0x0) goto L_8002545C;
    r29 = *(s16*)((u8*)r3 + 0x2);
L_8002545C: ;
    r3 = r29 + 0xf;
    r0 = 0x0;
    r3 = (s16)r3;
    *(u16*)((u8*)r31 + 0x50) = r3;
    *(u16*)((u8*)r31 + 0x52) = r0;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}
#pragma pop

/* 0x80025490 | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025490(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_80478DD8[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A370[];
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r4;
    r0 = *(u32*)lbl_8047A370;
    if ((s32)r0 == (s32)0x1) goto L_800254C8;
    if ((s32)r0 >= (s32)0x1) goto L_800254FC;
    goto L_800254FC;
L_800254C8: ;
    f1 = *(f32*)lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f0 = *(f32*)(sp + 0xC);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r29 = *(u32*)(sp + 0x1C);
    goto L_80025570;
L_800254FC: ;
    r4 = *(u32*)lbl_80478DD8;
    r28 = *(u32*)lbl_8047A368;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r28 >= (u32)r0) goto L_80025570;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r30 = 0x0;
L_80025524: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80025544;
    if ((s32)r28 != (s32)r30) goto L_80025540;
    goto L_80025564;
L_80025540: ;
    r30 = r30 + 0x1;
L_80025544: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80025560;
    r3 = *(s16*)((u8*)r3 + 0x18);
    fn_8005D934();
    goto L_80025524;
L_80025560: ;
    r3 = 0x0;
L_80025564: ;
    if ((u32)r3 == (u32)0x0) goto L_80025570;
    r29 = *(s16*)((u8*)r3 + 0x4);
L_80025570: ;
    r3 = 0x0;
    r0 = r29 + 0xf;
    *(u16*)((u8*)r31 + 0x50) = r3;
    r0 = (s16)r0;
    *(u16*)((u8*)r31 + 0x52) = r0;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}
#pragma pop

/* 0x800255A4 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800255A4(void) {
    extern u8 lbl_8047A384[];
    extern u8 lbl_8047A388[];
    extern u8 lbl_8047A38C[];
    extern u8 lbl_8047B8AC[];
    extern u8 lbl_8047B8B0[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8D0[];
    extern u8 lbl_8047B8E4[];
    extern u8 lbl_8047B8E8[];
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800EF5A4();
    extern void fn_80102510();
    extern void fn_80102620();
    extern void fn_801653C4();
    extern void fn_801656F8();
    extern void fn_801657F8();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f4 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x60) = f31;
    /* psq_st f31, 0x68(r1), 0, qr0 */;
    *(f64*)(sp + 0x50) = f30;
    /* psq_st f30, 0x58(r1), 0, qr0 */;
    *(f64*)(sp + 0x40) = f29;
    /* psq_st f29, 0x48(r1), 0, qr0 */;
    *(f64*)(sp + 0x30) = f28;
    /* psq_st f28, 0x38(r1), 0, qr0 */;
    *(f64*)(sp + 0x20) = f27;
    /* psq_st f27, 0x28(r1), 0, qr0 */;
    r0 = *(u32*)lbl_8047A384;
    if ((u32)r0 == (u32)0x0) goto L_8002566C;
    f1 = *(f32*)lbl_8047B8E4;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x449;
    r4 = 0x0;
    fn_801657F8();
    f27 = *(f32*)lbl_8047B8AC;
    f28 = *(f64*)lbl_8047B8B8;
    r31 = (0x4330 << 16);
    f30 = *(f64*)lbl_8047B8D0;
    f31 = *(f32*)lbl_8047B8B0;
    goto L_80025650;
L_80025618: ;
    ((void(*)(void))fn_800F0308)();
    fn_800D37CC();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0xC) = r0;
    f0 = *(f64*)(sp + 0x8);
    f29 = f0 - f28;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_80025650: ;
    if (f27 < f31) goto L_80025618;
    fn_801653C4();
    r4 = 0x7d0;
    r5 = 0x0;
    fn_801656F8();
    goto L_80025688;
L_8002566C: ;
    f1 = *(f32*)lbl_8047B8E8;
    r3 = 0x3;
    fn_801C41C8();
    fn_801653C4();
    r4 = 0x1f4;
    r5 = 0x0;
    fn_801656F8();
L_80025688: ;
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0xbd;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800256AC;
    r3 = 0xbd;
    fn_80102510();
L_800256AC: ;
    r3 = 0xc3;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800256C8;
    r3 = 0xc3;
    fn_80102510();
L_800256C8: ;
    r0 = *(u32*)lbl_8047A384;
    if ((u32)r0 == (u32)0x0) goto L_800256F4;
    r3 = *(u32*)lbl_8047A388;
    if ((u32)r3 == (u32)0x0) goto L_800256E4;
    fn_800EF5A4();
L_800256E4: ;
    r3 = *(u32*)lbl_8047A38C;
    if ((u32)r3 == (u32)0x0) goto L_800256F4;
    fn_800EF5A4();
L_800256F4: ;
    /* psq_l f31, 0x68(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x60);
    /* psq_l f30, 0x58(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x50);
    /* psq_l f29, 0x48(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x40);
    /* psq_l f28, 0x38(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x30);
    /* psq_l f27, 0x28(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x80025730 | 0x280 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025730(void) {
    extern u8 lbl_80478DD8[];
    extern u8 lbl_80478DDC[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A388[];
    extern u8 lbl_8047A390[];
    extern u8 lbl_8047A3A8[];
    extern u8 lbl_8047A3AC[];
    extern void fn_80025F84();
    extern void fn_8005D8F8();
    extern void fn_8005D934();
    extern void fn_800D3074();
    extern void fn_800DC390();
    extern void fn_800EF5FC();
    extern void fn_80102510();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern void fn_8011394C();
    extern void fn_801902E0();
    extern void fn_80025F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = 0x0;
    r3 = *(u32*)lbl_80478DDC;
    *(u32*)(sp + 0x8) = r0;
    *(u32*)lbl_8047A368 = r0;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_8005D934();
    r28 = 0x0;
    *(u32*)lbl_8047A390 = r3;
    r31 = r28 << 4;
    r29 = 0x0;
    goto L_800257F8;
L_80025778: ;
    r0 = *(u32*)lbl_80478DDC;
    r3 = *(u32*)lbl_8047A3AC;
    r4 = r0 + r31;
    r0 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r3 != (u32)r0) goto L_800257A4;
    *(u32*)lbl_8047A368 = r28;
    r3 = *(u32*)((u8*)r4 + 0x8);
    fn_8005D934();
    *(u32*)lbl_8047A390 = r3;
L_800257A4: ;
    r3 = *(u32*)lbl_80478DDC;
    r30 = 0x1;
    r3 = *(u32*)(r3 + r31);
    if ((u32)r3 == (u32)0x0) goto L_800257CC;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800257CC;
    r30 = 0x0;
L_800257CC: ;
    r3 = *(u32*)lbl_80478DDC;
    r0 = r31 + 0x8;
    r4 = r30;
    r3 = *(u32*)(r3 + r0);
    fn_8005D8F8();
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800257F0;
    r28 = r28 + 0x1;
L_800257F0: ;
    r31 = r31 + 0x10;
    r29 = r29 + 0x1;
L_800257F8: ;
    r3 = *(u32*)lbl_80478DD8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 < (u32)r0) goto L_80025778;
    fn_801046B8();
    r4 = r3;
    r5 = r1 + 0x8;
    r3 = 0xbd;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    if ((s32)r3 >= (s32)0x0) goto L_80025840;
    fn_8011394C();
    r30 = r3;
    goto L_800258C0;
L_80025840: ;
    r4 = *(u32*)lbl_80478DD8;
    r7 = 0x0;
    r6 = *(u32*)lbl_80478DDC;
    r5 = *(u32*)((u8*)r4 + 0x0);
    r4 = r6;
    ctr_fn = (void(*)(void))r5;
    if ((u32)r5 <= (u32)0x0) goto L_80025878;
L_80025860: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_80025878;
    r4 = r4 + 0x10;
    r7 = r7 + 0x1;
    if (--ctr != 0) goto L_80025860;
L_80025878: ;
    if ((u32)r7 < (u32)r5) goto L_80025884;
    r7 = 0x0;
L_80025884: ;
    r31 = r7 << 4;
    r3 = r6 + r31;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)0x66) goto L_800258B4;
    r3 = 0x45d;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800258B4;
    r30 = 0x7b;
    goto L_800258C0;
L_800258B4: ;
    r0 = *(u32*)lbl_80478DDC;
    r3 = r0 + r31;
    r30 = *(u32*)((u8*)r3 + 0x4);
L_800258C0: ;
    fn_8011394C();
    if ((u32)r30 == (u32)r3) goto L_8002590C;
    if ((u32)r30 != (u32)0x7a) goto L_800258E8;
    fn_8011394C();
    if ((u32)r3 != (u32)0xf) goto L_800258E8;
    r0 = 0x0;
    goto L_80025910;
L_800258E8: ;
    if ((u32)r30 != (u32)0xf) goto L_80025904;
    fn_8011394C();
    if ((u32)r3 != (u32)0x7a) goto L_80025904;
    r0 = 0x0;
    goto L_80025910;
L_80025904: ;
    r0 = 0x1;
    goto L_80025910;
L_8002590C: ;
    r0 = 0x0;
L_80025910: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80025984;
    r3 = 0x2;
    fn_800D3074();
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)lbl_8047A3A8 = r0;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    r7 = 0x0;
    fn_800EF5FC();
    r4 = (u32)fn_80025F74;
    *(u32*)lbl_8047A388 = r3;
    r4 = (u32)fn_80025F74;
    r5 = 0x0;
    fn_800DC390();
    goto L_80025960;
L_8002595C: ;
    ((void(*)(void))fn_800F0308)();
L_80025960: ;
    r0 = *(u8*)lbl_8047A3A8;
    if ((u32)r0 == (u32)0x0) goto L_8002595C;
    r3 = 0xc3;
    r4 = 0x0;
    fn_8010264C();
    r3 = 0xbd;
    fn_80102510();
    fn_80025F84();
L_80025984: ;
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x800259B0 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800259B0(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern u8 lbl_80478898[];
    extern u8 lbl_8047A368[];
    extern u8 lbl_8047A36C[];
    extern u8 lbl_8047A370[];
    extern u8 lbl_8047A374[];
    extern u8 lbl_8047A378[];
    extern u8 lbl_8047A37C[];
    extern u8 lbl_8047A380[];
    extern u8 lbl_8047A384[];
    extern u8 lbl_8047A388[];
    extern u8 lbl_8047A390[];
    extern u8 lbl_8047A3AC[];
    extern u8 lbl_8047B8A8[];
    extern u8 lbl_8047B8AC[];
    extern u8 lbl_8047B8E8[];
    extern u8 lbl_8047B8EC[];
    extern void fn_8011394C();
    extern void fn_801CB954();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    f3 = *(f32*)lbl_8047B8A8;
    r3 = (u32)lbl_803A2058;
    r0 = 0x0;
    f2 = *(f32*)lbl_8047B8AC;
    r6 = (u32)lbl_803A2058;
    f1 = *(f32*)lbl_8047B8E8;
    f0 = *(f32*)lbl_8047B8EC;
    r3 = (u32)lbl_803A204C;
    r5 = (u32)lbl_803A204C;
    r3 = (0xc6a << 16);
    *(u32*)lbl_8047A368 = r0;
    r3 = r3 + 0x1000;
    r4 = 0x0;
    *(u32*)lbl_8047A36C = r0;
    *(u32*)lbl_8047A370 = r0;
    *(f32*)lbl_80478898 = f3;
    *(f32*)((u8*)r6 + 0x0) = f2;
    *(f32*)((u8*)r6 + 0x4) = f2;
    *(f32*)((u8*)r6 + 0x8) = f2;
    *(f32*)((u8*)r5 + 0x0) = f2;
    *(f32*)((u8*)r5 + 0x4) = f2;
    *(f32*)((u8*)r5 + 0x8) = f2;
    *(f32*)lbl_8047A374 = f2;
    *(f32*)lbl_8047A378 = f1;
    *(f32*)lbl_8047A37C = f0;
    *(u8*)lbl_8047A380 = r0;
    *(u32*)lbl_8047A384 = r0;
    *(u32*)lbl_8047A388 = r0;
    *(u32*)lbl_8047A390 = r0;
    fn_801CB954();
    r3 = (0xc6a << 16);
    r4 = 0x0;
    r3 = r3 + 0x1001;
    fn_801CB954();
    r3 = (0xc6a << 16);
    r4 = 0x0;
    r3 = r3 + 0x1002;
    fn_801CB954();
    fn_8011394C();
    *(u32*)lbl_8047A3AC = r3;
    if ((u32)r3 != (u32)0x7b) goto L_80025A6C;
    r0 = 0x66;
    *(u32*)lbl_8047A3AC = r0;
L_80025A6C: ;
    return;
}
#pragma pop

/* 0x80025A7C | 0x4 -- nop */
void fn_80025A7C(void) { }

/* 0x80025A80 | 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025A80(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_8047A388[];
    extern u8 lbl_8047A3A0[];
    extern u8 lbl_8047A3A4[];
    extern u8 lbl_8047B8AC[];
    extern u8 lbl_8047B8B0[];
    extern u8 lbl_8047B8C8[];
    extern u8 lbl_8047B8E0[];
    extern u8 lbl_8047B8E4[];
    extern u8 lbl_8047B8F0[];
    extern u8 lbl_8047B8F4[];
    extern u8 lbl_8047B8F8[];
    extern u8 lbl_8047B8FC[];
    extern u8 lbl_8047B904[];
    extern u8 lbl_8047B908[];
    extern void fn_80025C1C();
    extern void fn_800D9B58();
    extern void fn_800D9ED8();
    extern void fn_800DA028();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;

    r31 = r3;
    f1 = *(f32*)lbl_8047B8AC;
    f3 = *(f32*)lbl_8047B8F8;
    f2 = f1;
    f4 = *(f32*)lbl_8047B8FC;
    fn_800D9B58();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x7;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x1;
    fn_800D9ED8();
    f1 = *(f32*)lbl_8047B8F0;
    r3 = r31;
    f2 = *(f32*)lbl_8047B8F4;
    r6 = r1 + 0x8;
    f0 = *(f32*)lbl_8047B8AC;
    r4 = 0x1;
    *(f32*)(sp + 0x8) = f1;
    r5 = 0x0;
    f1 = *(f32*)lbl_8047A3A0;
    *(f32*)(sp + 0xC) = f2;
    f2 = *(f32*)lbl_8047B8B0;
    *(f32*)(sp + 0x10) = f0;
    f3 = *(f32*)lbl_8047B904;
    fn_80025C1C();
    f2 = *(f32*)lbl_8047A3A0;
    f1 = *(f32*)lbl_8047B908;
    f0 = *(f32*)lbl_8047B8AC;
    f1 = f2 - f1;
    *(f32*)lbl_8047A3A0 = f1;
    if (f1 >= f0) goto L_80025B5C;
    *(f32*)lbl_8047A3A0 = f0;
L_80025B5C: ;
    r3 = (u32)lbl_803A204C;
    f0 = *(f32*)lbl_8047B8AC;
    r3 = (u32)lbl_803A204C;
    f4 = *(f32*)lbl_8047B8E0;
    f2 = *(f32*)((u8*)r3 + 0x0);
    r6 = r1 + 0x8;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r4 = 0x1;
    f3 = f4 + f2;
    f5 = *(f32*)lbl_8047B8F8;
    f1 = f4 + f1;
    f2 = *(f32*)lbl_8047B8FC;
    *(f32*)(sp + 0x10) = f0;
    r5 = 0x1;
    f3 = f5 - f3;
    r3 = *(u32*)lbl_8047A388;
    f0 = f2 - f1;
    f1 = *(f32*)lbl_8047A3A4;
    f2 = *(f32*)lbl_8047B8B0;
    *(f32*)(sp + 0x8) = f3;
    f3 = *(f32*)lbl_8047B8E4;
    *(f32*)(sp + 0xC) = f0;
    fn_80025C1C();
    f2 = *(f32*)lbl_8047A3A4;
    f1 = *(f32*)lbl_8047B8C8;
    f0 = *(f32*)lbl_8047B8AC;
    f1 = f2 - f1;
    *(f32*)lbl_8047A3A4 = f1;
    if (f1 >= f0) goto L_80025BD8;
    *(f32*)lbl_8047A3A4 = f0;
L_80025BD8: ;
    f0 = *(f32*)lbl_8047A3A4;
    f1 = *(f32*)lbl_8047B8AC;
    /* cror eq, lt, eq */;
    if (f0 != f1) goto L_80025C04;
    f0 = *(f32*)lbl_8047A3A0;
    /* cror eq, lt, eq */;
    if (f0 != f1) goto L_80025C04;
    r3 = 0x0;
    goto L_80025C08;
L_80025C04: ;
    r3 = 0x1;
L_80025C08: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x80025C1C | 0x358 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025C1C(void) {
    extern u8 lbl_80314AE8[];
    extern u8 lbl_8047B8AC[];
    extern u8 lbl_8047B8B0[];
    extern u8 lbl_8047B8DC[];
    extern u8 lbl_8047B8F0[];
    extern u8 lbl_8047B8F4[];
    extern u8 lbl_8047B90C[];
    extern u8 lbl_8047B910[];
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DFF98();
    extern void fn_800E03B4();
    extern void fn_800E042C();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98(r1), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88(r1), 0, qr0 */;
    f30 = f1;
    r29 = r3;
    f31 = f3;
    r31 = r5;
    r30 = r6;
    r0 = r4 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80025C84;
    f0 = *(f32*)lbl_8047B8AC;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f2;
    *(f32*)(sp + 0x1C) = f2;
    *(f32*)(sp + 0x8) = f0;
    *(f32*)(sp + 0xC) = f0;
    *(f32*)(sp + 0x10) = f0;
    goto L_80025CA0;
L_80025C84: ;
    f0 = *(f32*)lbl_8047B8AC;
    *(f32*)(sp + 0x8) = f2;
    *(f32*)(sp + 0x14) = f0;
    *(f32*)(sp + 0x18) = f0;
    *(f32*)(sp + 0x1C) = f0;
    *(f32*)(sp + 0xC) = f2;
    *(f32*)(sp + 0x10) = f2;
L_80025CA0: ;
    f1 = f30;
    fn_800E0CA0();
    r3 = r1 + 0x20;
    r4 = r1 + 0x14;
    r5 = r1 + 0x8;
    fn_800E090C();
    f1 = *(f32*)lbl_8047B8DC;
    r0 = r31 & 0xFF;
    f0 = *(f32*)(sp + 0x20);
    f0 = f1 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x60) = f0;
    r31 = *(u32*)(sp + 0x64);
    if ((u32)r0 != (u32)0x1) goto L_80025CFC;
    f0 = *(f32*)lbl_8047B8B0;
    *(f32*)(sp + 0x8) = f31;
    *(f32*)(sp + 0x14) = f0;
    *(f32*)(sp + 0x18) = f0;
    *(f32*)(sp + 0x1C) = f0;
    *(f32*)(sp + 0xC) = f31;
    *(f32*)(sp + 0x10) = f0;
    goto L_80025D18;
L_80025CFC: ;
    f0 = *(f32*)lbl_8047B8B0;
    *(f32*)(sp + 0x14) = f31;
    *(f32*)(sp + 0x18) = f31;
    *(f32*)(sp + 0x1C) = f0;
    *(f32*)(sp + 0x8) = f0;
    *(f32*)(sp + 0xC) = f0;
    *(f32*)(sp + 0x10) = f0;
L_80025D18: ;
    f1 = f30;
    fn_800E0CA0();
    r3 = r1 + 0x20;
    r4 = r1 + 0x14;
    r5 = r1 + 0x8;
    fn_800E090C();
    r3 = r1 + 0x2c;
    r4 = r1 + 0x20;
    fn_800E042C();
    f3 = *(f32*)lbl_8047B8F0;
    f1 = f30;
    f0 = *(f32*)lbl_8047B8F4;
    f2 = *(f32*)lbl_8047B8AC;
    *(f32*)(sp + 0x14) = f3;
    *(f32*)(sp + 0x18) = f0;
    *(f32*)(sp + 0x1C) = f2;
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    f0 = *(f32*)((u8*)r30 + 0x4);
    *(f32*)(sp + 0xC) = f0;
    *(f32*)(sp + 0x10) = f2;
    fn_800E0CA0();
    r3 = r1 + 0x20;
    r4 = r1 + 0x14;
    r5 = r1 + 0x8;
    fn_800E090C();
    r3 = r1 + 0x2c;
    r4 = r1 + 0x20;
    fn_800E03B4();
    r3 = 0x3;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    r3 = 0x4;
    fn_800D6A00();
    r3 = (u32)lbl_80314AE8;
    r3 = (u32)lbl_80314AE8;
    fn_800D7820();
    r4 = r29;
    r3 = 0x0;
    fn_800D85D4();
    r3 = 0x4;
    fn_800D67BC();
    f2 = *(f32*)lbl_8047B90C;
    r3 = r1 + 0x8;
    f1 = *(f32*)lbl_8047B910;
    r4 = r1 + 0x2c;
    f0 = *(f32*)lbl_8047B8AC;
    r5 = r1 + 0x14;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
    fn_800DFF98();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r7 = r31 & 0xFF;
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047B8AC;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    f2 = *(f32*)lbl_8047B8F0;
    r3 = r1 + 0x8;
    f1 = *(f32*)lbl_8047B910;
    r4 = r1 + 0x2c;
    f0 = *(f32*)lbl_8047B8AC;
    r5 = r1 + 0x14;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
    fn_800DFF98();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r7 = r31 & 0xFF;
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047B8B0;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047B8AC;
    fn_800D59B8();
    f2 = *(f32*)lbl_8047B90C;
    r3 = r1 + 0x8;
    f1 = *(f32*)lbl_8047B8F4;
    r4 = r1 + 0x2c;
    f0 = *(f32*)lbl_8047B8AC;
    r5 = r1 + 0x14;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
    fn_800DFF98();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r7 = r31 & 0xFF;
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047B8AC;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047B8B0;
    fn_800D59B8();
    f2 = *(f32*)lbl_8047B8F0;
    r3 = r1 + 0x8;
    f1 = *(f32*)lbl_8047B8F4;
    r4 = r1 + 0x2c;
    f0 = *(f32*)lbl_8047B8AC;
    r5 = r1 + 0x14;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
    fn_800DFF98();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r7 = r31 & 0xFF;
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047B8B0;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    fn_800D6728();
    /* psq_l f31, 0x98(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    r31 = *(u32*)(sp + 0x7C);
    r30 = *(u32*)(sp + 0x78);
    r29 = *(u32*)(sp + 0x74);
    return;
}
#pragma pop

/* 0x80025F74 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025F74(void) {
    extern u8 lbl_8047A3A8[];
    u32 r0 = 0;
    u32 r3 = 0;

    r0 = 0x1;
    r3 = 0x0;
    *(u8*)lbl_8047A3A8 = r0;
    return;
}
#pragma pop

/* 0x80025F84 | 0x3EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80025F84(void) {
    extern u8 lbl_803A2040[];
    extern u8 lbl_80478DEC[];
    extern u8 lbl_8047A380[];
    extern u8 lbl_8047A384[];
    extern u8 lbl_8047A38C[];
    extern u8 lbl_8047A394[];
    extern u8 lbl_8047A398[];
    extern u8 lbl_8047A39C[];
    extern u8 lbl_8047A3A0[];
    extern u8 lbl_8047A3A4[];
    extern u8 lbl_8047B8A8[];
    extern u8 lbl_8047B8AC[];
    extern u8 lbl_8047B8B8[];
    extern u8 lbl_8047B8D0[];
    extern u8 lbl_8047B8E4[];
    extern u8 lbl_8047B8F0[];
    extern u8 lbl_8047B8F4[];
    extern u8 lbl_8047B900[];
    extern u8 lbl_8047B914[];
    extern u8 lbl_8047B918[];
    extern void fn_800C46B0();
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800DC390();
    extern void fn_800E3C00();
    extern void fn_800E3C08();
    extern void fn_800E3C94();
    extern void fn_800E8FA0();
    extern void fn_800E8FE8();
    extern void fn_800E900C();
    extern void fn_800E9108();
    extern void fn_800EF5FC();
    extern void fn_800F9318();
    extern void fn_80113F48();
    extern void fn_80165A20();
    extern void fn_80176E0C();
    extern void fn_801902E0();
    extern void fn_801CB61C();
    extern void fn_801CB834();
    extern void fn_801CB954();
    extern void fn_801CBA0C();
    extern void fn_80025A80();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = (0xc6a << 16);
    r4 = 0x1;
    r3 = r3 + 0x1000;
    fn_801CB954();
    r3 = (0xc6a << 16);
    r4 = 0x1;
    r3 = r3 + 0x1001;
    fn_801CB954();
    r3 = (0xc6a << 16);
    r4 = 0x1;
    r3 = r3 + 0x1002;
    fn_801CB954();
    r3 = 0x3e5;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80026004;
    r3 = (0xc6b << 16);
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0x0;
    r28 = r3;
    fn_801CB954();
    r29 = 0x0;
    goto L_80026020;
L_80026004: ;
    r3 = (0xc6c << 16);
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0x0;
    r28 = r3;
    fn_801CB954();
    r29 = 0x0;
L_80026020: ;
    fn_80113F48();
    r4 = r28;
    fn_800F9318();
    r4 = 0x0;
    fn_800E3C94();
    fn_80113F48();
    r4 = r28;
    fn_800F9318();
    r30 = r3;
    fn_80113F48();
    r4 = (0xc6a << 16);
    r4 = r4 + 0x1002;
    fn_800F9318();
    r31 = r3;
    fn_80113F48();
    r4 = (0xc6a << 16);
    r4 = r4 + 0x1603;
    fn_800F9318();
    r31 = r3;
    r3 = r30;
    r4 = 0x2;
    fn_800E9108();
    r3 = r30;
    r4 = r31;
    fn_800E8FE8();
    r3 = r30;
    r5 = r1 + 0x8;
    r4 = 0x1;
    fn_800E900C();
    r3 = 0x280;
    r4 = 0x1e0;
    fn_800E8FA0();
    fn_80113F48();
    r4 = (0xc83 << 16);
    r4 = r4 + 0x1400;
    fn_800F9318();
    r31 = r3;
    fn_80113F48();
    r4 = r28;
    fn_800F9318();
    r4 = r31;
    fn_800E3C08();
    fn_80113F48();
    r4 = r28;
    fn_800F9318();
    r4 = 0x4;
    fn_800E3C00();
    r3 = r28;
    r4 = 0x1;
    fn_801CB954();
    r4 = (0xc6a << 16);
    r3 = r28;
    r4 = r4 + 0x1000;
    r5 = 0x0;
    fn_801CB61C();
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x449;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
    r0 = *(u8*)lbl_8047A380;
    *(u32*)lbl_8047A384 = r28;
    if ((u32)r0 != (u32)0x0) goto L_80026168;
    r3 = *(u32*)lbl_8047A394;
    r5 = *(u32*)lbl_80478DEC;
    r4 = r3 << 4;
    r0 = r3 + 0x1;
    r3 = r5 + r4;
    r29 = *(u32*)((u8*)r3 + 0xA4);
    r28 = *(u32*)((u8*)r3 + 0xA8);
    *(u32*)lbl_8047A394 = r0;
    if ((s32)r0 <= (s32)0x9) goto L_80026198;
    r0 = 0x0;
    *(u32*)lbl_8047A394 = r0;
    goto L_80026198;
L_80026168: ;
    r3 = *(u32*)lbl_8047A398;
    r5 = *(u32*)lbl_80478DEC;
    r4 = r3 << 4;
    r0 = r3 + 0x1;
    r3 = r5 + r4;
    r29 = *(u32*)((u8*)r3 + 0x4);
    r28 = *(u32*)((u8*)r3 + 0x8);
    *(u32*)lbl_8047A398 = r0;
    if ((s32)r0 <= (s32)0x9) goto L_80026198;
    r0 = 0x0;
    *(u32*)lbl_8047A398 = r0;
L_80026198: ;
    f0 = *(f32*)lbl_8047B8A8;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x44;
    *(f32*)lbl_8047A3A4 = f0;
    r6 = 0x0;
    r7 = 0x0;
    *(f32*)lbl_8047A3A0 = f0;
    fn_800EF5FC();
    r4 = (u32)fn_80025A80;
    *(u32*)lbl_8047A38C = r3;
    r4 = (u32)fn_80025A80;
    r5 = 0x0;
    fn_800DC390();
    fn_80113F48();
    r4 = r28;
    r5 = 0x0;
    r6 = 0x0;
    fn_80176E0C();
    r30 = 0x1;
    fn_800D37CC();
    if ((s32)r3 != (s32)0x32) goto L_8002620C;
    f1 = *(f32*)lbl_8047B914;
    fn_800C46B0();
    r30 = r3;
    if ((u32)r30 >= (u32)0x1) goto L_8002620C;
    r30 = 0x1;
L_8002620C: ;
    r28 = 0x0;
    goto L_80026220;
L_80026214: ;
    ((void(*)(void))fn_800F0308)();
    fn_800D3088();
    r28 = r28 + r3;
L_80026220: ;
    if ((u32)r28 < (u32)r30) goto L_80026214;
    r3 = (0xc6a << 16);
    r4 = r29;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0xae;
    fn_800D37CC();
    if ((s32)r3 != (s32)0x32) goto L_80026268;
    f1 = *(f32*)lbl_8047B918;
    fn_800C46B0();
    r29 = r3;
    if ((u32)r29 >= (u32)0x1) goto L_80026268;
    r29 = 0x1;
L_80026268: ;
    r28 = 0x0;
    goto L_8002627C;
L_80026270: ;
    ((void(*)(void))fn_800F0308)();
    fn_800D3088();
    r28 = r28 + r3;
L_8002627C: ;
    if ((u32)r28 < (u32)r29) goto L_80026270;
    r3 = (u32)lbl_803A2040;
    f2 = *(f32*)lbl_8047B8F0;
    r3 = (u32)lbl_803A2040;
    f1 = *(f32*)lbl_8047B8F4;
    f0 = *(f32*)lbl_8047B8AC;
    *(f32*)((u8*)r3 + 0x0) = f2;
    *(f32*)((u8*)r3 + 0x4) = f1;
    *(f32*)((u8*)r3 + 0x8) = f0;
    fn_80113F48();
    r4 = *(u32*)lbl_8047A384;
    fn_800F9318();
    f0 = *(f32*)lbl_8047B8A8;
    *(u32*)lbl_8047A39C = r3;
    *(f32*)lbl_8047A3A4 = f0;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f2 = *(f64*)lbl_8047B8B8;
    *(u32*)(sp + 0x10) = r0;
    f0 = *(f32*)lbl_8047B8E4;
    f1 = *(f64*)(sp + 0x10);
    f1 = f1 - f2;
    f1 = f0 * f1;
    fn_800C46B0();
    r29 = 0x78 - r3;
    if ((u32)r29 == (u32)0x0) goto L_80026334;
    fn_800D37CC();
    if ((s32)r3 != (s32)0x32) goto L_80026334;
    r0 = (0x4330 << 16);
    f2 = *(f64*)lbl_8047B8D0;
    *(u32*)(sp + 0x10) = r0;
    f0 = *(f32*)lbl_8047B900;
    f1 = *(f64*)(sp + 0x10);
    f1 = f1 - f2;
    f1 = f1 / f0;
    fn_800C46B0();
    r29 = r3;
    if ((u32)r29 >= (u32)0x1) goto L_80026334;
    r29 = 0x1;
L_80026334: ;
    r28 = 0x0;
    goto L_80026348;
L_8002633C: ;
    ((void(*)(void))fn_800F0308)();
    fn_800D3088();
    r28 = r28 + r3;
L_80026348: ;
    if ((u32)r28 < (u32)r29) goto L_8002633C;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}
#pragma pop
