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
