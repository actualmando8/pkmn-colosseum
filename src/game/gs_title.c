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

extern u32 lbl_80478DDC;
extern u32 fn_801902E0(void*);

/* 0x4C | fn_80024438 | check_then_call */
void fn_80024438(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x80));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_80024484 | check_then_call */
/* fn_80024484 - 0x80024484 | size: 0x4c */
void fn_80024484(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x70));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_800244D0 | check_then_call */
/* fn_800244D0 - 0x800244D0 | size: 0x4c */
void fn_800244D0(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x60));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_8002451C | check_then_call */
/* fn_8002451C - 0x8002451C | size: 0x4c */
void fn_8002451C(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x50));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_80024568 | check_then_call */
/* fn_80024568 - 0x80024568 | size: 0x4c */
void fn_80024568(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x40));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_800245B4 | check_then_call */
/* fn_800245B4 - 0x800245B4 | size: 0x4c */
void fn_800245B4(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x30));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_80024600 | check_then_call */
/* fn_80024600 - 0x80024600 | size: 0x4c */
void fn_80024600(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x20));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x4C | fn_8002464C | check_then_call */
/* fn_8002464C - 0x8002464C | size: 0x4c */
void fn_8002464C(u32 arg1, u32 arg2) {
    void* ctx = *(void**)&lbl_80478DDC;
    u32 result = (u32)fn_801902E0(*(void**)((u8*)ctx + 0x10));
    if ((result & 0xFF) == 0) {
        fn_80109220(arg2, 0);
    }
}

/* 0x80024698 | 0x4 -- nop */
#if 0
asm void fn_80024698(void) {
#include "src/game/gs_title_fn_80024698.inc"
}
#else
#pragma optimization_level 4
void fn_80024698(void) { }
#endif

/* 0x8002469C | 0x30 */
extern void* fn_800FA280(u32);
extern void fn_80132A38(s32, void*);
#if 0
asm void fn_8002469C(void) {
#include "src/game/gs_title_fn_8002469C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
void fn_8002469C(void) {
    fn_80132A38(0x37, fn_800FA280(0x3cdf));
}
#endif

/* 0x800246CC | 0x30 */
#if 0
asm void fn_800246CC(void) {
#include "src/game/gs_title_fn_800246CC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
void fn_800246CC(void) {
    fn_80132A38(0x37, fn_800FA280(0x3ce4));
}
#endif

/* 0x800246FC | 0x330 */
extern void* fn_8005DA18(s32);
extern void* fn_8005D934(u32);
extern u32 lbl_80478DE4;
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern f32 lbl_8047B8D8;
extern u32 lbl_8047A368;
extern u32 lbl_80478DDC;
extern u32 lbl_80478DD8;
extern f32 lbl_8047B8DC;
extern u32 lbl_8047A36C;
extern u8 lbl_802E4F58[];
extern u32 lbl_80478DE0;
#if 0
asm void fn_800246FC(void) {
#include "src/game/gs_title_fn_800246FC.inc"
}
#else
void fn_800246FC(void) {
    extern u8 lbl_802E4F58[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_80478DE0;
    extern u32 lbl_80478DE4;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A36C;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047B8D8;
    extern f32 lbl_8047B8DC;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800FA280();
    extern void fn_80132A38();
    extern void fn_801902E0();
    u8 sp[0x30];
    u32 tmp = 0;
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

    r28 = r3;
    r30 = r4;
    r31 = 0x0;
    r27 = 0x0;
    while (1) {
        r3 = lbl_80478DE0;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r31 >= tmp) break;
        r3 = lbl_80478DE4;
        r3 = *(u32*)(r3 + r27);
        if (r3 != 0) {
            fn_801902E0();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
            }
            tmp = lbl_8047A370;
            if ((s32)tmp != 1) {

            } else {
                f1 = lbl_80478898;
                f0 = lbl_8047B8D8;
                if (f1 > f0) {
                    r29 = lbl_8047A368;
                    r3 = *(u32*)((u8*)r28 + 0x4);
                    fn_8005DA18();
                    r3 = *(s16*)((u8*)r3 + 0x4);
                    fn_8005D934();
                    r28 = 0x0;
                L_80024778:
                    tmp = *(u8*)((u8*)r3 + 0x0);
                    /* extrwi tmp, tmp, 1, 24 */;
                    if (tmp != 0) {
                        if ((s32)r29 == (s32)r28) {
                            r28 = r3;
                            goto L_800247BC;
                        }
                        r28 = r28 + 0x1;
                    }
                    tmp = *(u8*)((u8*)r3 + 0x0);
                    /* extrwi tmp, tmp, 1, 25 */;
                    if (tmp == 0) {
                        r3 = *(s16*)((u8*)r3 + 0x18);
                        fn_8005D934();
                        goto L_80024778;
                    }
                    r28 = 0x0;
                L_800247BC:
                    r29 = 0x0;
                    r27 = r29;
                    while (1) {
                        r3 = lbl_80478DD8;
                        tmp = *(u32*)((u8*)r3 + 0x0);
                        if (r29 >= tmp) break;
                        r3 = lbl_80478DDC;
                        tmp = r27 + 0x8;
                        r3 = *(u32*)(r3 + tmp);
                        fn_8005D934();
                        if (r28 == r3) {
                            break;
                        }
                        r27 = r27 + 0x10;
                        r29 = r29 + 0x1;

                    }
                    r29 = 0x0;

                    f0 = lbl_80478898;
                    f1 = lbl_8047B8D8;
                    f2 = lbl_8047B8DC;
                    f0 = f0 - f1;
                    f0 = f0 / f1;
                    f0 = f2 * f0;
                    f0 = (f64)(s32)f0;
                    goto L_80024994;
                }
                r29 = lbl_8047A36C;
                r3 = *(u32*)((u8*)r28 + 0x4);
                fn_8005DA18();
                r3 = *(s16*)((u8*)r3 + 0x4);
                fn_8005D934();
                r28 = 0x0;
            L_80024840:
                tmp = *(u8*)((u8*)r3 + 0x0);
                /* extrwi tmp, tmp, 1, 24 */;
                if (tmp != 0) {
                    if ((s32)r29 == (s32)r28) {
                        r28 = r3;
                        goto L_80024884;
                    }
                    r28 = r28 + 0x1;
                }
                tmp = *(u8*)((u8*)r3 + 0x0);
                /* extrwi tmp, tmp, 1, 25 */;
                if (tmp == 0) {
                    r3 = *(s16*)((u8*)r3 + 0x18);
                    fn_8005D934();
                    goto L_80024840;
                }
                r28 = 0x0;
            L_80024884:
                r29 = 0x0;
                r27 = r29;
                while (1) {
                    r3 = lbl_80478DD8;
                    tmp = *(u32*)((u8*)r3 + 0x0);
                    if (r29 >= tmp) break;
                    r3 = lbl_80478DDC;
                    tmp = r27 + 0x8;
                    r3 = *(u32*)(r3 + tmp);
                    fn_8005D934();
                    if (r28 == r3) {
                        break;
                    }
                    r27 = r27 + 0x10;
                    r29 = r29 + 0x1;

                }
                r29 = 0x0;

                f1 = lbl_8047B8D8;
                f0 = lbl_80478898;
                f2 = lbl_8047B8DC;
                f0 = f1 - f0;
                f0 = f0 / f1;
                f0 = f2 * f0;
                f0 = (f64)(s32)f0;
                goto L_80024994;
            }
            r29 = lbl_8047A368;
            r3 = *(u32*)((u8*)r28 + 0x4);
            fn_8005DA18();
            r3 = *(s16*)((u8*)r3 + 0x4);
            fn_8005D934();
            r28 = 0x0;
        L_80024908:
            tmp = *(u8*)((u8*)r3 + 0x0);
            /* extrwi tmp, tmp, 1, 24 */;
            if (tmp != 0) {
                if ((s32)r29 == (s32)r28) {
                    r28 = r3;
                    goto L_8002494C;
                }
                r28 = r28 + 0x1;
            }
            tmp = *(u8*)((u8*)r3 + 0x0);
            /* extrwi tmp, tmp, 1, 25 */;
            if (tmp == 0) {
                r3 = *(s16*)((u8*)r3 + 0x18);
                fn_8005D934();
                goto L_80024908;
            }
            r28 = 0x0;
        L_8002494C:
            r29 = 0x0;
            r27 = r29;
            while (1) {
                r3 = lbl_80478DD8;
                tmp = *(u32*)((u8*)r3 + 0x0);
                if (r29 >= tmp) break;
                r3 = lbl_80478DDC;
                tmp = r27 + 0x8;
                r3 = *(u32*)(r3 + tmp);
                fn_8005D934();
                if (r28 == r3) {
                    break;
                }
                r27 = r27 + 0x10;
                r29 = r29 + 0x1;

            }
            r29 = 0x0;

            tmp = 0xff;
        L_80024994:
            *(u8*)((u8*)r30 + 0x67) = tmp;
            r3 = lbl_80478DD8;
            tmp = *(u32*)((u8*)r3 + 0x0);
            if (r29 >= tmp) break;
            if (r29 > 9) {
                r29 = 0x0;
            }
            r3 = (u32)lbl_802E4F58;
            r5 = lbl_80478DE4;
            r4 = r31 * 0x28;
            r3 = (u32)lbl_802E4F58;
            tmp = *(u8*)(r3 + r29);
            r3 = r5 + r4;
            tmp = tmp << 2;
            r3 = r3 + tmp;
            r3 = *(u32*)((u8*)r3 + 0x4);
            fn_800FA280();
            r4 = r3;
            r3 = 0x37;
            fn_80132A38();
            return;
            }
        r27 = r27 + 0x28;
        r31 = r31 + 0x1;

    }

    r3 = 0x1;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();

    return;
}
#endif

/* 0x80024A2C | 0x178 */
extern u32 lbl_8047A368;
extern u32 lbl_80478DDC;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern f32 lbl_8047B8A8;
extern f32 lbl_8047B8DC;
#if 0
asm void fn_80024A2C(void) {
#include "src/game/gs_title_fn_80024A2C.inc"
}
#else
void fn_80024A2C(void) {
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047B8A8;
    extern f32 lbl_8047B8DC;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_801902E0();
    u8 sp[0x20];
    u32 tmp = 0;
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
    r30 = lbl_8047A368;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
    while (1) {
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r30 == (s32)r29) {
                r29 = r3;
                break;
            }
            r29 = r29 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp != 0) {
            r29 = 0x0;
            break;
        }
        r3 = *(s16*)((u8*)r3 + 0x18);
        fn_8005D934();
    }
    r30 = 0x0;
    r28 = r30;
    while (1) {
        r3 = lbl_80478DD8;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r30 >= tmp) { r30 = 0x0; break; }
        r3 = lbl_80478DDC;
        tmp = r28 + 0x8;
        r3 = *(u32*)(r3 + tmp);
        fn_8005D934();
        if (r29 == r3) break;
        r28 = r28 + 0x10;
        r30 = r30 + 0x1;
    }
    tmp = lbl_8047A370;
    if ((s32)tmp == 1) {
        f1 = lbl_80478898;
        f0 = lbl_8047B8A8;
        f2 = lbl_8047B8DC;
        f0 = f1 / f0;
        f0 = f2 * f0;
        f0 = (f64)(s32)f0;
        *(u8*)((u8*)r31 + 0x67) = tmp;
    } else {
        tmp = 0xff;
        *(u8*)((u8*)r31 + 0x67) = tmp;
    }
    r3 = lbl_80478DD8;
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (r30 >= tmp) return;
    tmp = lbl_80478DDC;
    r28 = r30 << 4;
    r3 = tmp + r28;
    tmp = *(u32*)((u8*)r3 + 0x4);
    if (tmp == 0x66) {
        r3 = 0x45d;
        fn_801902E0();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xC5F0000;
            tmp = r3 + 0x1200;
        } else {
            tmp = lbl_80478DDC;
            r3 = tmp + r28;
            tmp = *(u32*)((u8*)r3 + 0xC);
        }
    } else {
        tmp = lbl_80478DDC;
        r3 = tmp + r28;
        tmp = *(u32*)((u8*)r3 + 0xC);
    }
    *(u32*)((u8*)r31 + 0x58) = tmp;

    return;
}
#endif

/* 0x80024BA4 | 0x138 */
extern u32 lbl_8047A36C;
extern u32 lbl_80478DDC;
extern u32 lbl_80478DD8;
#if 0
asm void fn_80024BA4(void) {
#include "src/game/gs_title_fn_80024BA4.inc"
}
#else
void fn_80024BA4(void) {
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A36C;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_801902E0();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r30 = lbl_8047A36C;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_8005DA18();
    r3 = *(s16*)((u8*)r3 + 0x4);
    fn_8005D934();
    r29 = 0x0;
L_80024BDC:
    tmp = *(u8*)((u8*)r3 + 0x0);
    /* extrwi tmp, tmp, 1, 24 */;
    if (tmp != 0) {
        if ((s32)r30 == (s32)r29) {
            r29 = r3;
            goto L_80024C20;
        }
        r29 = r29 + 0x1;
    }
    tmp = *(u8*)((u8*)r3 + 0x0);
    /* extrwi tmp, tmp, 1, 25 */;
    if (tmp == 0) {
        r3 = *(s16*)((u8*)r3 + 0x18);
        fn_8005D934();
        goto L_80024BDC;
    }
    r29 = 0x0;
L_80024C20:
    r30 = 0x0;
    r28 = r30;
    while (1) {
        r3 = lbl_80478DD8;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r30 >= tmp) break;
        r3 = lbl_80478DDC;
        tmp = r28 + 0x8;
        r3 = *(u32*)(r3 + tmp);
        fn_8005D934();
        if (r29 == r3) {
            break;
        }
        r28 = r28 + 0x10;
        r30 = r30 + 0x1;

    }
    r30 = 0x0;

    r3 = lbl_80478DD8;
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (r30 >= tmp) return;
    tmp = lbl_80478DDC;
    r28 = r30 << 4;
    r3 = tmp + r28;
    tmp = *(u32*)((u8*)r3 + 0x4);
    do {
        if (tmp != 0x66) break;
        r3 = 0x45d;
        fn_801902E0();
        tmp = r3 & 0xFF;
        if (tmp == 0) break;
        r3 = 0xC5F0000;
        tmp = r3 + 0x1200;
        break;
    } while (0);

    tmp = lbl_80478DDC;
    r3 = tmp + r28;
    tmp = *(u32*)((u8*)r3 + 0xC);

    *(u32*)((u8*)r31 + 0x58) = tmp;

    return;
}
#endif

/* 0x80024CDC | 0xE0 */
extern u32 fn_800D3088(void);
extern u32 lbl_8047A390;
extern f64 lbl_8047B8D0;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047A37C;
#if 0
asm void fn_80024CDC(void) {
#include "src/game/gs_title_fn_80024CDC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
void fn_80024CDC(s32 r3, u8* r4) {
    u8* r30;
    s32 r31;
    f32 f0;
    f32 f3;

    r30 = r4;
    if (lbl_8047A390 != 0) {
        *(s16*)(r30 + 0x50) = *(s16*)((u8*)lbl_8047A390 + 2) + 8;
        *(s16*)(r30 + 0x52) = *(s16*)((u8*)lbl_8047A390 + 4) + 8;
    }

    r31 = *(u8*)(r30 + 0x67);
    r3 = fn_800D3088();
    f3 = lbl_8047A37C;
    f0 = (f32)((f64)r31 + f3 * (f64)r3);
    r31 = (s32)f0;

    if (r31 < 0x40) {
        f0 = -f3;
        r31 = 0x40;
        lbl_8047A37C = f0;
    } else if (r31 > 0xFF) {
        f0 = -f3;
        r31 = 0xFF;
        lbl_8047A37C = f0;
    }

    *(u8*)(r30 + 0x67) = r31;
}
#endif

/* 0x80024DBC | 0x170 */
extern f32 fn_800E0CA0(f32);
extern void fn_800E090C(void*, void*, void*, f32);
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u8 lbl_803A2058[];
extern u8 lbl_803A204C[];
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047A374;
extern f32 lbl_8047B8E0;
#if 0
asm void fn_80024DBC(void) {
#include "src/game/gs_title_fn_80024DBC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
void fn_80024DBC(u8* arg0, u8* arg1) {
    f32 pos[2];
    f32 fx;
    f32 fy;
    u8* node;
    u8 flags;
    u32 state;
    s32 r27;
    s32 r28;
    s32 r29;
    s32 r31;
    f32 t;

    state = lbl_8047A370;
    switch ((s32)state) {
    case 1:
        t = fn_800E0CA0(lbl_80478898);
        fn_800E090C(pos, lbl_803A2058, lbl_803A204C, t);
        fx = pos[0];
        fy = pos[1];
        r28 = (s32)fx;
        r31 = (s32)fy;
        break;
    default:
        r27 = lbl_8047A368;
        if ((u32)r27 < *(u32*)lbl_80478DD8) {
            node = fn_8005DA18(*(u32*)(arg0 + 4));
            node = fn_8005D934(*(s16*)(node + 4));
            r29 = 0;
            while (1) {
                flags = node[0];
                if ((((u32)flags >> 6) & 1) != 0) {
                    if (r27 == r29) {
                        break;
                    }
                    r29++;
                }
                if ((((u32)flags >> 5) & 1) == 0) {
                    node = fn_8005D934(*(s16*)(node + 0x18));
                } else {
                    node = 0;
                    break;
                }
            }
            if (node != 0) {
                r28 = *(s16*)(node + 2);
                r31 = *(s16*)(node + 4);
            }
        }
        break;
    }
    *(s16*)(arg1 + 0x50) = (s16)(s32)(lbl_8047B8E0 + ((f32)r28 + lbl_8047A374));
    *(s16*)(arg1 + 0x52) = (s16)(s32)(lbl_8047B8E0 + ((f32)r31 + lbl_8047A374));
}
#endif

/* 0x80024F2C | 0x170 */
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047A374;
extern f32 lbl_8047B8E0;
#if 0
asm void fn_80024F2C(void) {
#include "src/game/gs_title_fn_80024F2C.inc"
}
#else
void fn_80024F2C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047A374;
    extern f64 lbl_8047B8B8;
    extern f32 lbl_8047B8E0;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 tmp = 0;
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

    r30 = r4;
    tmp = lbl_8047A370;
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_80024F9C;
        goto L_80024F9C;
    }
    f1 = lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = (u32)sp + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    goto L_80025014;
L_80024F9C:
    r4 = lbl_80478DD8;
    r27 = lbl_8047A368;
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (r27 < tmp) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        fn_8005DA18();
        r3 = *(s16*)((u8*)r3 + 0x4);
        fn_8005D934();
        r29 = 0x0;
    L_80024FC4:
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r27 == (s32)r29) {
                goto L_80025004;
            }
            r29 = r29 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp == 0) {
            r3 = *(s16*)((u8*)r3 + 0x18);
            fn_8005D934();
            goto L_80024FC4;
        }
        r3 = 0x0;
    L_80025004:
        if (r3 != 0) {
            r28 = *(s16*)((u8*)r3 + 0x2);
            r31 = *(s16*)((u8*)r3 + 0x4);
        }
    }
L_80025014:
    tmp = (s16)r28;
    r3 = 0x43300000;
    tmp = (s16)r31;
    f3 = lbl_8047B8B8;
    f0 = lbl_8047A374;
    f4 = lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = tmp;
    f1 = f1 + f0;
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(u16*)((u8*)r30 + 0x50) = tmp;
    f0 = lbl_8047A374;
    f0 = f1 - f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r30 + 0x52) = tmp;
    return;
}
#endif

/* 0x8002509C | 0x170 */
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047A374;
extern f32 lbl_8047B8E0;
#if 0
asm void fn_8002509C(void) {
#include "src/game/gs_title_fn_8002509C.inc"
}
#else
void fn_8002509C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047A374;
    extern f64 lbl_8047B8B8;
    extern f32 lbl_8047B8E0;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 tmp = 0;
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

    r30 = r4;
    tmp = lbl_8047A370;
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_8002510C;
        goto L_8002510C;
    }
    f1 = lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = (u32)sp + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    goto L_80025184;
L_8002510C:
    r4 = lbl_80478DD8;
    r27 = lbl_8047A368;
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (r27 < tmp) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        fn_8005DA18();
        r3 = *(s16*)((u8*)r3 + 0x4);
        fn_8005D934();
        r29 = 0x0;
    L_80025134:
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r27 == (s32)r29) {
                goto L_80025174;
            }
            r29 = r29 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp == 0) {
            r3 = *(s16*)((u8*)r3 + 0x18);
            fn_8005D934();
            goto L_80025134;
        }
        r3 = 0x0;
    L_80025174:
        if (r3 != 0) {
            r28 = *(s16*)((u8*)r3 + 0x2);
            r31 = *(s16*)((u8*)r3 + 0x4);
        }
    }
L_80025184:
    tmp = (s16)r28;
    r3 = 0x43300000;
    tmp = (s16)r31;
    f3 = lbl_8047B8B8;
    f0 = lbl_8047A374;
    f4 = lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = tmp;
    f1 = f1 - f0;
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(u16*)((u8*)r30 + 0x50) = tmp;
    f0 = lbl_8047A374;
    f0 = f1 + f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r30 + 0x52) = tmp;
    return;
}
#endif

/* 0x8002520C | 0x170 */
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047A374;
extern f32 lbl_8047B8E0;
#if 0
asm void fn_8002520C(void) {
#include "src/game/gs_title_fn_8002520C.inc"
}
#else
void fn_8002520C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047A374;
    extern f64 lbl_8047B8B8;
    extern f32 lbl_8047B8E0;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x50];
    u32 tmp = 0;
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

    r30 = r4;
    tmp = lbl_8047A370;
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_8002527C;
        goto L_8002527C;
    }
    f1 = lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = (u32)sp + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    goto L_800252F4;
L_8002527C:
    r4 = lbl_80478DD8;
    r27 = lbl_8047A368;
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (r27 < tmp) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        fn_8005DA18();
        r3 = *(s16*)((u8*)r3 + 0x4);
        fn_8005D934();
        r29 = 0x0;
    L_800252A4:
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r27 == (s32)r29) {
                goto L_800252E4;
            }
            r29 = r29 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp == 0) {
            r3 = *(s16*)((u8*)r3 + 0x18);
            fn_8005D934();
            goto L_800252A4;
        }
        r3 = 0x0;
    L_800252E4:
        if (r3 != 0) {
            r28 = *(s16*)((u8*)r3 + 0x2);
            r31 = *(s16*)((u8*)r3 + 0x4);
        }
    }
L_800252F4:
    tmp = (s16)r28;
    r3 = 0x43300000;
    tmp = (s16)r31;
    f3 = lbl_8047B8B8;
    f0 = lbl_8047A374;
    f4 = lbl_8047B8E0;
    f1 = f1 - f3;
    *(u32*)(sp + 0x2C) = tmp;
    f1 = f1 - f0;
    f2 = f4 + f1;
    f1 = f0 - f3;
    f0 = (f64)(s32)f2;
    *(u16*)((u8*)r30 + 0x50) = tmp;
    f0 = lbl_8047A374;
    f0 = f1 - f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r30 + 0x52) = tmp;
    return;
}
#endif

/* 0x8002537C | 0x114 */
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
#if 0
asm void fn_8002537C(void) {
#include "src/game/gs_title_fn_8002537C.inc"
}
#else
void fn_8002537C(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x30];
    u32 tmp = 0;
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
    tmp = lbl_8047A370;
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_800253E8;
        goto L_800253E8;
    }
    f1 = lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = (u32)sp + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f0 = *(f32*)(sp + 0x8);
    f0 = (f64)(s32)f0;
    goto L_8002545C;
L_800253E8:
    r4 = lbl_80478DD8;
    r28 = lbl_8047A368;
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (r28 < tmp) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        fn_8005DA18();
        r3 = *(s16*)((u8*)r3 + 0x4);
        fn_8005D934();
        r30 = 0x0;
    L_80025410:
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r28 == (s32)r30) {
                goto L_80025450;
            }
            r30 = r30 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp == 0) {
            r3 = *(s16*)((u8*)r3 + 0x18);
            fn_8005D934();
            goto L_80025410;
        }
        r3 = 0x0;
    L_80025450:
        if (r3 != 0) {
            r29 = *(s16*)((u8*)r3 + 0x2);
        }
    }
L_8002545C:
    r3 = r29 + 0xf;
    tmp = 0x0;
    r3 = (s16)r3;
    *(u16*)((u8*)r31 + 0x50) = r3;
    *(u16*)((u8*)r31 + 0x52) = tmp;
    return;
}
#endif

/* 0x80025490 | 0x114 */
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern u32 lbl_80478DD8;
extern u32 lbl_8047A368;
#if 0
asm void fn_80025490(void) {
#include "src/game/gs_title_fn_80025490.inc"
}
#else
void fn_80025490(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A370;
    extern void fn_8005D934();
    extern void fn_8005DA18();
    extern void fn_800E090C();
    extern void fn_800E0CA0();
    u8 sp[0x30];
    u32 tmp = 0;
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
    tmp = lbl_8047A370;
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_800254FC;
        goto L_800254FC;
    }
    f1 = lbl_80478898;
    fn_800E0CA0();
    r3 = (u32)lbl_803A2058;
    r5 = (u32)lbl_803A204C;
    r4 = (u32)lbl_803A2058;
    r3 = (u32)sp + 0x8;
    r5 = (u32)lbl_803A204C;
    fn_800E090C();
    f0 = *(f32*)(sp + 0xC);
    f0 = (f64)(s32)f0;
    goto L_80025570;
L_800254FC:
    r4 = lbl_80478DD8;
    r28 = lbl_8047A368;
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (r28 < tmp) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        fn_8005DA18();
        r3 = *(s16*)((u8*)r3 + 0x4);
        fn_8005D934();
        r30 = 0x0;
    L_80025524:
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 24 */;
        if (tmp != 0) {
            if ((s32)r28 == (s32)r30) {
                goto L_80025564;
            }
            r30 = r30 + 0x1;
        }
        tmp = *(u8*)((u8*)r3 + 0x0);
        /* extrwi tmp, tmp, 1, 25 */;
        if (tmp == 0) {
            r3 = *(s16*)((u8*)r3 + 0x18);
            fn_8005D934();
            goto L_80025524;
        }
        r3 = 0x0;
    L_80025564:
        if (r3 != 0) {
            r29 = *(s16*)((u8*)r3 + 0x4);
        }
    }
L_80025570:
    r3 = 0x0;
    tmp = r29 + 0xf;
    *(u16*)((u8*)r31 + 0x50) = r3;
    tmp = (s16)tmp;
    *(u16*)((u8*)r31 + 0x52) = tmp;
    return;
}
#endif

/* 0x800255A4 | 0x18C */
extern void fn_801C41C8(f32, s32);
extern void fn_801657F8(void);
extern void fn_800D37CC(void);
extern void fn_801653C4(void);
extern void fn_801656F8(void);
extern void fn_801C40F0(s32);
extern void fn_80102620(void);
extern void fn_80102510(void);
extern void fn_800EF5A4(void);
extern u32 lbl_8047A384;
extern f32 lbl_8047B8E4;
extern f32 lbl_8047B8AC;
extern f64 lbl_8047B8B8;
extern f64 lbl_8047B8D0;
extern f32 lbl_8047B8B0;
extern f32 lbl_8047B8E8;
extern u32 lbl_8047A388;
extern u32 lbl_8047A38C;
#if 0
asm void fn_800255A4(void) {
#include "src/game/gs_title_fn_800255A4.inc"
}
#else
void fn_800255A4(void) {
    extern u32 lbl_8047A384;
    extern u32 lbl_8047A388;
    extern u32 lbl_8047A38C;
    extern f32 lbl_8047B8AC;
    extern f32 lbl_8047B8B0;
    extern f64 lbl_8047B8B8;
    extern f64 lbl_8047B8D0;
    extern f32 lbl_8047B8E4;
    extern f32 lbl_8047B8E8;
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
    u32 tmp = 0;
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

    tmp = lbl_8047A384;
    if (tmp != 0) {
        f1 = lbl_8047B8E4;
        r3 = 0x3;
        fn_801C41C8();
        r3 = 0x449;
        r4 = 0x0;
        fn_801657F8();
        f27 = lbl_8047B8AC;
        f28 = lbl_8047B8B8;
        r31 = 0x43300000;
        f30 = lbl_8047B8D0;
        f31 = lbl_8047B8B0;
        while (f27 < f31) {

            ((void(*)(void))fn_800F0308)();
            fn_800D37CC();
            *(u32*)(sp + 0xC) = tmp;
            f29 = f0 - f28;
            fn_800D3088();
            f0 = f0 - f30;
            f0 = f0 / f29;
            f27 = f27 + f0;

        }
        fn_801653C4();
        r4 = 0x7d0;
        r5 = 0x0;
        fn_801656F8();
    } else {

        f1 = lbl_8047B8E8;
        r3 = 0x3;
        fn_801C41C8();
        fn_801653C4();
        r4 = 0x1f4;
        r5 = 0x0;
        fn_801656F8();
    }
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0xbd;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp == 1) {
        r3 = 0xbd;
        fn_80102510();
    }
    r3 = 0xc3;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp == 1) {
        r3 = 0xc3;
        fn_80102510();
    }
    tmp = lbl_8047A384;
    if (tmp != 0) {
        r3 = lbl_8047A388;
        if (r3 != 0) {
            fn_800EF5A4();
        }
        r3 = lbl_8047A38C;
        if (r3 != 0) {
            fn_800EF5A4();
    }
    }
    return;
}
#endif

/* 0x80025730 | 0x280 */
extern void fn_8005D8F8(void);
extern void fn_801046B8(void);
extern void fn_801026A4(void);
extern void fn_8011394C(void);
extern void fn_800D3074(void);
extern void fn_800EF5FC(void);
extern void fn_800DC390(void);
extern void fn_8010264C(void);
extern u32 lbl_80478DDC;
extern u32 lbl_8047A368;
extern u32 lbl_8047A390;
extern u32 lbl_8047A3AC;
extern u32 lbl_80478DD8;
extern u8 lbl_8047A3A8;
extern u32 lbl_8047A388;
extern void fn_80025F84(void);
#if 1
asm void fn_80025730(void) {
#include "src/game/gs_title_fn_80025730.inc"
}
#else
void fn_80025730(void) {
    extern u32 lbl_80478DD8;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A388;
    extern u32 lbl_8047A390;
    extern u8 lbl_8047A3A8;
    extern u32 lbl_8047A3AC;
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
    u32 tmp = 0;
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

    tmp = 0x0;
    r3 = lbl_80478DDC;
    *(u32*)(sp + 0x8) = tmp;
    lbl_8047A368 = tmp;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_8005D934();
    r28 = 0x0;
    lbl_8047A390 = r3;
    r31 = r28 << 4;
    r29 = 0x0;
    while (1) {
        r3 = lbl_80478DD8;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r29 >= tmp) break;
        tmp = lbl_80478DDC;
        r3 = lbl_8047A3AC;
        r4 = tmp + r31;
        tmp = *(u32*)((u8*)r4 + 0x4);
        if (r3 == tmp) {
            lbl_8047A368 = r28;
            r3 = *(u32*)((u8*)r4 + 0x8);
            fn_8005D934();
            lbl_8047A390 = r3;
        }
        r3 = lbl_80478DDC;
        r30 = 0x1;
        r3 = *(u32*)(r3 + r31);
        if (r3 != 0) {
            fn_801902E0();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                r30 = 0x0;
        }
        }
        r3 = lbl_80478DDC;
        tmp = r31 + 0x8;
        r4 = r30;
        r3 = *(u32*)(r3 + tmp);
        fn_8005D8F8();
        tmp = r30 & 0xFF;
        if (tmp != 0) {
            r28 = r28 + 0x1;
        }
        r31 = r31 + 0x10;
        r29 = r29 + 0x1;

    }
    fn_801046B8();
    r4 = r3;
    r5 = (u32)sp + 0x8;
    r3 = 0xbd;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_801026A4();
    if ((s32)r3 < 0) {
        fn_8011394C();
        r30 = r3;

    } else {
        r4 = lbl_80478DD8;
        r7 = 0x0;
        r6 = lbl_80478DDC;
        r5 = *(u32*)((u8*)r4 + 0x0);
        r4 = r6;
        ctr_fn = (void(*)(void))r5;
        if (r5 > 0) {
            do {
                tmp = *(u32*)((u8*)r4 + 0x8);
                if (r3 == tmp) break;
                r4 = r4 + 0x10;
                r7 = r7 + 0x1;
            } while (--ctr != 0);
        }
        if (r7 >= r5) {
            r7 = 0x0;
        }
        r31 = r7 << 4;
        r3 = r6 + r31;
        tmp = *(u32*)((u8*)r3 + 0x4);
        do {
        if (tmp != 0x66) break;
            r3 = 0x45d;
            fn_801902E0();
            tmp = r3 & 0xFF;
            if (tmp == 0) break;
            r30 = 0x7b;
            break;
        } while (0);

        tmp = lbl_80478DDC;
        r3 = tmp + r31;
        r30 = *(u32*)((u8*)r3 + 0x4);
    }
    fn_8011394C();
    if (r30 != r3) {
        do {
            if (r30 != 0x7a) break;
            fn_8011394C();
            if (r3 != 0xf) break;
            tmp = 0x0;
            goto L_80025910;
        } while (0);

        do {
            if (r30 != 0xf) break;
            fn_8011394C();
            if (r3 != 0x7a) break;
            tmp = 0x0;
            break;
        } while (0);

        tmp = 0x1;
        goto L_80025910;
    }
    tmp = 0x0;
L_80025910:
    tmp = tmp & 0xFF;
    if (tmp == 1) {
        r3 = 0x2;
        fn_800D3074();
        tmp = 0x0;
        r3 = 0x0;
        lbl_8047A3A8 = tmp;
        r4 = 0x0;
        r5 = 0x44;
        r6 = 0x0;
        r7 = 0x0;
        fn_800EF5FC();
        r4 = (u32)fn_80025F74;
        lbl_8047A388 = r3;
        r4 = (u32)fn_80025F74;
        r5 = 0x0;
        fn_800DC390();
        while (1) {
            tmp = lbl_8047A3A8;
            if (tmp != 0) break;
            ((void(*)(void))fn_800F0308)();

        }
        r3 = 0xc3;
        r4 = 0x0;
        fn_8010264C();
        r3 = 0xbd;
        fn_80102510();
        fn_80025F84();
    }
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    return;
}
#endif

/* 0x800259B0 | 0xCC */
extern void fn_801CB954(void);
extern f32 lbl_8047B8A8;
extern f32 lbl_8047B8AC;
extern f32 lbl_8047B8E8;
extern f32 lbl_8047B8EC;
extern u32 lbl_8047A368;
extern u32 lbl_8047A36C;
extern u32 lbl_8047A370;
extern f32 lbl_80478898;
extern f32 lbl_8047A374;
extern f32 lbl_8047A378;
extern f32 lbl_8047A37C;
extern u8 lbl_8047A380;
extern u32 lbl_8047A384;
extern u32 lbl_8047A388;
extern u32 lbl_8047A390;
extern u32 lbl_8047A3AC;
#if 0
asm void fn_800259B0(void) {
#include "src/game/gs_title_fn_800259B0.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
void fn_800259B0(void) {
    extern u8 lbl_803A2058[];
    extern u8 lbl_803A204C[];
    extern void fn_801CB954(s32, s32);
    extern u32 fn_8011394C(void);
    f32* vec1 = (f32*)lbl_803A2058;
    f32* vec2 = (f32*)lbl_803A204C;
    u32 result;

    lbl_8047A368 = 0;
    lbl_8047A36C = 0;
    lbl_8047A370 = 0;
    lbl_80478898 = lbl_8047B8A8;
    vec1[0] = lbl_8047B8AC;
    vec1[1] = lbl_8047B8AC;
    vec1[2] = lbl_8047B8AC;
    vec2[0] = lbl_8047B8AC;
    vec2[1] = lbl_8047B8AC;
    vec2[2] = lbl_8047B8AC;
    lbl_8047A374 = lbl_8047B8AC;
    lbl_8047A378 = lbl_8047B8E8;
    lbl_8047A37C = lbl_8047B8EC;
    lbl_8047A380 = 0;
    lbl_8047A384 = 0;
    lbl_8047A388 = 0;
    lbl_8047A390 = 0;
    fn_801CB954(0x0C6A1000, 0);
    fn_801CB954(0x0C6A1001, 0);
    fn_801CB954(0x0C6A1002, 0);
    result = fn_8011394C();
    lbl_8047A3AC = result;
    if (result == 0x7b) {
        lbl_8047A3AC = 0x66;
    }
}
#endif

#if 0
void fn_800259B0_old(void) {
    extern u8 lbl_803A204C[];
    extern u8 lbl_803A2058[];
    extern f32 lbl_80478898;
    extern u32 lbl_8047A368;
    extern u32 lbl_8047A36C;
    extern u32 lbl_8047A370;
    extern f32 lbl_8047A374;
    extern f32 lbl_8047A378;
    extern f32 lbl_8047A37C;
    extern u8 lbl_8047A380;
    extern u32 lbl_8047A384;
    extern u32 lbl_8047A388;
    extern u32 lbl_8047A390;
    extern u32 lbl_8047A3AC;
    extern f32 lbl_8047B8A8;
    extern f32 lbl_8047B8AC;
    extern f32 lbl_8047B8E8;
    extern f32 lbl_8047B8EC;
    extern void fn_8011394C();
    extern void fn_801CB954();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    f3 = lbl_8047B8A8;
    r3 = (u32)lbl_803A2058;
    tmp = 0x0;
    f2 = lbl_8047B8AC;
    r6 = (u32)lbl_803A2058;
    f1 = lbl_8047B8E8;
    f0 = lbl_8047B8EC;
    r3 = (u32)lbl_803A204C;
    r5 = (u32)lbl_803A204C;
    r3 = 0xC6A0000;
    lbl_8047A368 = tmp;
    r3 = r3 + 0x1000;
    r4 = 0x0;
    lbl_8047A36C = tmp;
    lbl_8047A370 = tmp;
    lbl_80478898 = f3;
    *(f32*)((u8*)r6 + 0x0) = f2;
    *(f32*)((u8*)r6 + 0x4) = f2;
    *(f32*)((u8*)r6 + 0x8) = f2;
    *(f32*)((u8*)r5 + 0x0) = f2;
    *(f32*)((u8*)r5 + 0x4) = f2;
    *(f32*)((u8*)r5 + 0x8) = f2;
    lbl_8047A374 = f2;
    lbl_8047A378 = f1;
    lbl_8047A37C = f0;
    lbl_8047A380 = tmp;
    lbl_8047A384 = tmp;
    lbl_8047A388 = tmp;
    lbl_8047A390 = tmp;
    fn_801CB954();
    r3 = 0xC6A0000;
    r4 = 0x0;
    r3 = r3 + 0x1001;
    fn_801CB954();
    r3 = 0xC6A0000;
    r4 = 0x0;
    r3 = r3 + 0x1002;
    fn_801CB954();
    fn_8011394C();
    lbl_8047A3AC = r3;
    if (r3 == 0x7b) {
        tmp = 0x66;
        lbl_8047A3AC = tmp;
    }
    return;
}
#endif

/* 0x80025A7C | 0x4 -- nop */
#if 0
asm void fn_80025A7C(void) {
#include "src/game/gs_title_fn_80025A7C.inc"
}
#else
#pragma optimization_level 4
void fn_80025A7C(void) { }
#endif

/* 0x80025A80 | 0x19C */
extern void fn_800D9B58(void);
extern void fn_800DA4C4(void);
extern void fn_800DA2BC(void);
extern void fn_800DA1E8(void);
extern void fn_800DA100(void);
extern void fn_800DA028(void);
extern void fn_800D9ED8(void);
extern f32 lbl_8047B8AC;
extern f32 lbl_8047B8F8;
extern f32 lbl_8047B8FC;
extern f32 lbl_8047B8F0;
extern f32 lbl_8047B8F4;
extern f32 lbl_8047A3A0;
extern f32 lbl_8047B8B0;
extern f32 lbl_8047B904;
extern f32 lbl_8047B908;
extern f32 lbl_8047B8E0;
extern u32 lbl_8047A388;
extern f32 lbl_8047A3A4;
extern f32 lbl_8047B8E4;
extern f32 lbl_8047B8C8;
extern void fn_80025C1C(void);
#if 0
asm void fn_80025A80(void) {
#include "src/game/gs_title_fn_80025A80.inc"
}
#else
void fn_80025A80(void) {
    extern u8 lbl_803A204C[];
    extern u32 lbl_8047A388;
    extern f32 lbl_8047A3A0;
    extern f32 lbl_8047A3A4;
    extern f32 lbl_8047B8AC;
    extern f32 lbl_8047B8B0;
    extern f32 lbl_8047B8C8;
    extern f32 lbl_8047B8E0;
    extern f32 lbl_8047B8E4;
    extern f32 lbl_8047B8F0;
    extern f32 lbl_8047B8F4;
    extern f32 lbl_8047B8F8;
    extern f32 lbl_8047B8FC;
    extern f32 lbl_8047B904;
    extern f32 lbl_8047B908;
    extern void fn_80025C1C();
    extern void fn_800D9B58();
    extern void fn_800D9ED8();
    extern void fn_800DA028();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x20];
    u32 tmp = 0;
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
    f1 = lbl_8047B8AC;
    f3 = lbl_8047B8F8;
    f2 = f1;
    f4 = lbl_8047B8FC;
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
    f1 = lbl_8047B8F0;
    r3 = r31;
    f2 = lbl_8047B8F4;
    r6 = (u32)sp + 0x8;
    f0 = lbl_8047B8AC;
    r4 = 0x1;
    *(f32*)(sp + 0x8) = f1;
    r5 = 0x0;
    f1 = lbl_8047A3A0;
    *(f32*)(sp + 0xC) = f2;
    f2 = lbl_8047B8B0;
    *(f32*)(sp + 0x10) = f0;
    f3 = lbl_8047B904;
    fn_80025C1C();
    f2 = lbl_8047A3A0;
    f1 = lbl_8047B908;
    f0 = lbl_8047B8AC;
    f1 = f2 - f1;
    lbl_8047A3A0 = f1;
    if (f1 < f0) {
        lbl_8047A3A0 = f0;
    }
    r3 = (u32)lbl_803A204C;
    f0 = lbl_8047B8AC;
    r3 = (u32)lbl_803A204C;
    f4 = lbl_8047B8E0;
    f2 = *(f32*)((u8*)r3 + 0x0);
    r6 = (u32)sp + 0x8;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r4 = 0x1;
    f3 = f4 + f2;
    f5 = lbl_8047B8F8;
    f1 = f4 + f1;
    f2 = lbl_8047B8FC;
    *(f32*)(sp + 0x10) = f0;
    r5 = 0x1;
    f3 = f5 - f3;
    r3 = lbl_8047A388;
    f0 = f2 - f1;
    f1 = lbl_8047A3A4;
    f2 = lbl_8047B8B0;
    *(f32*)(sp + 0x8) = f3;
    f3 = lbl_8047B8E4;
    *(f32*)(sp + 0xC) = f0;
    fn_80025C1C();
    f2 = lbl_8047A3A4;
    f1 = lbl_8047B8C8;
    f0 = lbl_8047B8AC;
    f1 = f2 - f1;
    lbl_8047A3A4 = f1;
    if (f1 < f0) {
        lbl_8047A3A4 = f0;
    }
    f0 = lbl_8047A3A4;
    f1 = lbl_8047B8AC;
    /* cror eq, lt, eq */;
    if (f0 == f1) {
        f0 = lbl_8047A3A0;
        /* cror eq, lt, eq */;
        if (f0 == f1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}
#endif

/* 0x80025C1C | 0x358 */
extern void fn_800E042C(void);
extern void fn_800E03B4(void);
extern void fn_800D88DC(void);
extern void fn_800D888C(void);
extern void fn_800D6A00(void);
extern void fn_800D7820(void);
extern void fn_800D85D4(void);
extern void fn_800D67BC(void);
extern void fn_800DFF98(void);
extern void fn_800D6680(void);
extern void fn_800D5CB8(void);
extern void fn_800D59B8(void);
extern void fn_800D6728(void);
extern f32 lbl_8047B8AC;
extern f32 lbl_8047B8DC;
extern f32 lbl_8047B8B0;
extern f32 lbl_8047B8F0;
extern f32 lbl_8047B8F4;
extern u8 lbl_80314AE8[];
extern f32 lbl_8047B90C;
extern f32 lbl_8047B910;
#if 0
asm void fn_80025C1C(void) {
#include "src/game/gs_title_fn_80025C1C.inc"
}
#else
void fn_80025C1C(void) {
    extern u8 lbl_80314AE8[];
    extern f32 lbl_8047B8AC;
    extern f32 lbl_8047B8B0;
    extern f32 lbl_8047B8DC;
    extern f32 lbl_8047B8F0;
    extern f32 lbl_8047B8F4;
    extern f32 lbl_8047B90C;
    extern f32 lbl_8047B910;
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
    u32 tmp = 0;
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

    f30 = f1;
    r29 = r3;
    f31 = f3;
    r31 = r5;
    r30 = r6;
    tmp = r4 & 0xFF;
    if (tmp == 1) {
        f0 = lbl_8047B8AC;
        *(f32*)(sp + 0x14) = f2;
        *(f32*)(sp + 0x18) = f2;
        *(f32*)(sp + 0x1C) = f2;
        *(f32*)(sp + 0x8) = f0;
        *(f32*)(sp + 0xC) = f0;
        *(f32*)(sp + 0x10) = f0;
    } else {

        f0 = lbl_8047B8AC;
        *(f32*)(sp + 0x8) = f2;
        *(f32*)(sp + 0x14) = f0;
        *(f32*)(sp + 0x18) = f0;
        *(f32*)(sp + 0x1C) = f0;
        *(f32*)(sp + 0xC) = f2;
        *(f32*)(sp + 0x10) = f2;
    }
    f1 = f30;
    fn_800E0CA0();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0x8;
    fn_800E090C();
    f1 = lbl_8047B8DC;
    tmp = r31 & 0xFF;
    f0 = *(f32*)(sp + 0x20);
    f0 = f1 * f0;
    f0 = (f64)(s32)f0;
    if (tmp == 1) {
        f0 = lbl_8047B8B0;
        *(f32*)(sp + 0x8) = f31;
        *(f32*)(sp + 0x14) = f0;
        *(f32*)(sp + 0x18) = f0;
        *(f32*)(sp + 0x1C) = f0;
        *(f32*)(sp + 0xC) = f31;
        *(f32*)(sp + 0x10) = f0;
    } else {

        f0 = lbl_8047B8B0;
        *(f32*)(sp + 0x14) = f31;
        *(f32*)(sp + 0x18) = f31;
        *(f32*)(sp + 0x1C) = f0;
        *(f32*)(sp + 0x8) = f0;
        *(f32*)(sp + 0xC) = f0;
        *(f32*)(sp + 0x10) = f0;
    }
    f1 = f30;
    fn_800E0CA0();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0x8;
    fn_800E090C();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x20;
    fn_800E042C();
    f3 = lbl_8047B8F0;
    f1 = f30;
    f0 = lbl_8047B8F4;
    f2 = lbl_8047B8AC;
    *(f32*)(sp + 0x14) = f3;
    *(f32*)(sp + 0x18) = f0;
    *(f32*)(sp + 0x1C) = f2;
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    f0 = *(f32*)((u8*)r30 + 0x4);
    *(f32*)(sp + 0xC) = f0;
    *(f32*)(sp + 0x10) = f2;
    fn_800E0CA0();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0x8;
    fn_800E090C();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x20;
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
    f2 = lbl_8047B90C;
    r3 = (u32)sp + 0x8;
    f1 = lbl_8047B910;
    r4 = (u32)sp + 0x2c;
    f0 = lbl_8047B8AC;
    r5 = (u32)sp + 0x14;
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
    f1 = lbl_8047B8AC;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    f2 = lbl_8047B8F0;
    r3 = (u32)sp + 0x8;
    f1 = lbl_8047B910;
    r4 = (u32)sp + 0x2c;
    f0 = lbl_8047B8AC;
    r5 = (u32)sp + 0x14;
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
    f1 = lbl_8047B8B0;
    r3 = 0x0;
    f2 = lbl_8047B8AC;
    fn_800D59B8();
    f2 = lbl_8047B90C;
    r3 = (u32)sp + 0x8;
    f1 = lbl_8047B8F4;
    r4 = (u32)sp + 0x2c;
    f0 = lbl_8047B8AC;
    r5 = (u32)sp + 0x14;
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
    f1 = lbl_8047B8AC;
    r3 = 0x0;
    f2 = lbl_8047B8B0;
    fn_800D59B8();
    f2 = lbl_8047B8F0;
    r3 = (u32)sp + 0x8;
    f1 = lbl_8047B8F4;
    r4 = (u32)sp + 0x2c;
    f0 = lbl_8047B8AC;
    r5 = (u32)sp + 0x14;
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
    f1 = lbl_8047B8B0;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    fn_800D6728();
    return;
}
#endif

/* 0x80025F74 | 0x10 */
extern u8 lbl_8047A3A8;
#if 0
asm void fn_80025F74(void) {
#include "src/game/gs_title_fn_80025F74.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
s32 fn_80025F74(void) {
    lbl_8047A3A8 = 1;
    return 0;
}
#endif

/* 0x80025F84 | 0x3EC */
extern void fn_801CBA0C(void);
extern void fn_80113F48(void);
extern void fn_800F9318(void);
extern void fn_800E3C94(void);
extern void fn_800E9108(void);
extern void fn_800E8FE8(void);
extern void fn_800E900C(void);
extern void fn_800E8FA0(void);
extern void fn_800E3C08(void);
extern void fn_800E3C00(void);
extern void fn_801CB61C(void);
extern void fn_801CB834(void);
extern void fn_80165A20(void);
extern void fn_80176E0C(void);
extern void fn_800C46B0(void);
extern u8 lbl_8047A380;
extern u32 lbl_8047A384;
extern u32 lbl_8047A394;
extern u32 lbl_80478DEC;
extern u32 lbl_8047A398;
extern f32 lbl_8047B8A8;
extern f32 lbl_8047A3A4;
extern f32 lbl_8047A3A0;
extern u32 lbl_8047A38C;
extern f32 lbl_8047B914;
extern f32 lbl_8047B918;
extern u8 lbl_803A2040[];
extern f32 lbl_8047B8F0;
extern f32 lbl_8047B8F4;
extern f32 lbl_8047B8AC;
extern u32 lbl_8047A39C;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047B8E4;
extern f64 lbl_8047B8D0;
extern f32 lbl_8047B900;
#if 0
asm void fn_80025F84(void) {
#include "src/game/gs_title_fn_80025F84.inc"
}
#else
void fn_80025F84(void) {
    extern u8 lbl_803A2040[];
    extern u32 lbl_80478DEC;
    extern u8 lbl_8047A380;
    extern u32 lbl_8047A384;
    extern u32 lbl_8047A38C;
    extern u32 lbl_8047A394;
    extern u32 lbl_8047A398;
    extern u32 lbl_8047A39C;
    extern f32 lbl_8047A3A0;
    extern f32 lbl_8047A3A4;
    extern f32 lbl_8047B8A8;
    extern f32 lbl_8047B8AC;
    extern f64 lbl_8047B8B8;
    extern f64 lbl_8047B8D0;
    extern f32 lbl_8047B8E4;
    extern f32 lbl_8047B8F0;
    extern f32 lbl_8047B8F4;
    extern f32 lbl_8047B900;
    extern f32 lbl_8047B914;
    extern f32 lbl_8047B918;
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
    u32 tmp = 0;
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

    r3 = 0xC6A0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    fn_801CB954();
    r3 = 0xC6A0000;
    r4 = 0x1;
    r3 = r3 + 0x1001;
    fn_801CB954();
    r3 = 0xC6A0000;
    r4 = 0x1;
    r3 = r3 + 0x1002;
    fn_801CB954();
    r3 = 0x3e5;
    fn_801902E0();
    tmp = r3 & 0xFF;
    if (tmp == 1) {
        r3 = 0xC6B0000;
        r3 = r3 + 0x1000;
        fn_801CBA0C();
        r4 = 0x0;
        r28 = r3;
        fn_801CB954();
        r29 = 0x0;
    } else {

        r3 = 0xC6C0000;
        r3 = r3 + 0x1000;
        fn_801CBA0C();
        r4 = 0x0;
        r28 = r3;
        fn_801CB954();
        r29 = 0x0;
    }
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
    r4 = 0xC6A0000;
    r4 = r4 + 0x1002;
    fn_800F9318();
    r31 = r3;
    fn_80113F48();
    r4 = 0xC6A0000;
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
    r5 = (u32)sp + 0x8;
    r4 = 0x1;
    fn_800E900C();
    r3 = 0x280;
    r4 = 0x1e0;
    fn_800E8FA0();
    fn_80113F48();
    r4 = 0xC830000;
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
    r4 = 0xC6A0000;
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
    tmp = lbl_8047A380;
    lbl_8047A384 = r28;
    if (tmp == 0) {
        r3 = lbl_8047A394;
        r5 = lbl_80478DEC;
        r4 = r3 << 4;
        tmp = r3 + 0x1;
        r3 = r5 + r4;
        r29 = *(u32*)((u8*)r3 + 0xA4);
        r28 = *(u32*)((u8*)r3 + 0xA8);
        lbl_8047A394 = tmp;
        if ((s32)tmp > 9) {
            tmp = 0x0;
            lbl_8047A394 = tmp;
        }

    } else {
        r3 = lbl_8047A398;
        r5 = lbl_80478DEC;
        r4 = r3 << 4;
        tmp = r3 + 0x1;
        r3 = r5 + r4;
        r29 = *(u32*)((u8*)r3 + 0x4);
        r28 = *(u32*)((u8*)r3 + 0x8);
        lbl_8047A398 = tmp;
        if ((s32)tmp > 9) {
            tmp = 0x0;
            lbl_8047A398 = tmp;
        }
    }
    f0 = lbl_8047B8A8;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x44;
    lbl_8047A3A4 = f0;
    r6 = 0x0;
    r7 = 0x0;
    lbl_8047A3A0 = f0;
    fn_800EF5FC();
    r4 = (u32)fn_80025A80;
    lbl_8047A38C = r3;
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
    if ((s32)r3 == 0x32) {
        f1 = lbl_8047B914;
        fn_800C46B0();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r28 = 0x0;
    while (r28 < r30) {

        ((void(*)(void))fn_800F0308)();
        fn_800D3088();
        r28 = r28 + r3;

    }
    r3 = 0xC6A0000;
    r4 = r29;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0xae;
    fn_800D37CC();
    if ((s32)r3 == 0x32) {
        f1 = lbl_8047B918;
        fn_800C46B0();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r28 = 0x0;
    while (r28 < r29) {

        ((void(*)(void))fn_800F0308)();
        fn_800D3088();
        r28 = r28 + r3;

    }
    r3 = (u32)lbl_803A2040;
    f2 = lbl_8047B8F0;
    r3 = (u32)lbl_803A2040;
    f1 = lbl_8047B8F4;
    f0 = lbl_8047B8AC;
    *(f32*)((u8*)r3 + 0x0) = f2;
    *(f32*)((u8*)r3 + 0x4) = f1;
    *(f32*)((u8*)r3 + 0x8) = f0;
    fn_80113F48();
    r4 = lbl_8047A384;
    fn_800F9318();
    f0 = lbl_8047B8A8;
    lbl_8047A39C = r3;
    lbl_8047A3A4 = f0;
    fn_800D37CC();
    tmp = 0x43300000;
    f2 = lbl_8047B8B8;
    *(u32*)(sp + 0x10) = tmp;
    f0 = lbl_8047B8E4;
    f1 = f1 - f2;
    f1 = f0 * f1;
    fn_800C46B0();
    r29 = 0x78 - r3;
    if (r29 != 0) {
        fn_800D37CC();
        if ((s32)r3 == 0x32) {
            tmp = 0x43300000;
            f2 = lbl_8047B8D0;
            *(u32*)(sp + 0x10) = tmp;
            f0 = lbl_8047B900;
            f1 = f1 - f2;
            f1 = f1 / f0;
            fn_800C46B0();
            r29 = r3;
            if (r29 < 1) {
                r29 = 0x1;
    }
    }
    }
    r28 = 0x0;
    while (r28 < r29) {

        ((void(*)(void))fn_800F0308)();
        fn_800D3088();
        r28 = r28 + r3;

    }
    return;
}
#endif

/* ===== Phase 2 recovery stubs ===== */

/* fn_8002058C - 0x8002058C | size: 0x2c */
extern u8 lbl_8047A32C;
#if 0
asm void fn_8002058C(void) {
#include "src/game/gs_title_fn_8002058C.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
void fn_8002058C(void) {
    lbl_8047A32C = 0;
    fn_801EF644(-1);
    lbl_8047A32C = 1;
    for (;;) {
        fn_800F0308();
    }
}
#endif

/* fn_800205C8 - 0x800205C8 | size: 0x44 */
extern void fn_800FB680(u32 a, u32 b, s32 c, u32 d);
extern u32 lbl_8047A350;
extern u32 lbl_80478880;
#if 1
asm void fn_800205C8(void) {
#include "src/game/gs_title_fn_800205C8.inc"
}
#else
void fn_800205C8(u8* obj) {
    fn_800FB680(0, 0, obj[0x8b] | (s32)-0x100, (&lbl_80478880)[lbl_8047A350]);
}
#endif

/* fn_8002060C - 0x8002060C | size: 0xc */
extern u32 lbl_8047A350;
#if 0
asm void fn_8002060C(void) {
#include "src/game/gs_title_fn_8002060C.inc"
}
#else
#pragma optimization_level 4
void fn_8002060C(void) {
    lbl_8047A350 = 0;
}
#endif

/* fn_80020618 - 0x80020618 | size: 0x304 */
extern void fn_80105624(void);
extern s32 fn_80135168(s32, s32);
extern f64 fn_800CE148(f32);
extern void fn_80166CC0(void);
extern void fn_800F78A4(void);
extern void fn_80135030(void);
extern u32 lbl_8047B878;
extern u8 lbl_803A1FC8[];
extern u32 lbl_8047B880;
extern u32 lbl_8047B868;
extern u32 lbl_8047B86C;
extern u32 lbl_8047B870;
extern f32 lbl_8047B874;
#if 1
asm void fn_80020618(void) {
#include "src/game/gs_title_fn_80020618.inc"
}
#else
void fn_80020618(void) { /* TODO */ }
#endif

/* fn_8002091C - 0x8002091C | size: 0x10 */
#if 0
asm void fn_8002091C(void) {
#include "src/game/gs_title_fn_8002091C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002091C(void) {
    return *(s32*)lbl_803A1FC8;
}
#endif

/* fn_8002092C - 0x8002092C | size: 0x90 */
extern f32 lbl_8047B874;
extern f32 lbl_8047B88C;
extern f32 lbl_8047B888;
#if 0
asm void fn_8002092C(void) {
#include "src/game/gs_title_fn_8002092C.inc"
}
#else
#pragma optimization_level 4
void fn_8002092C(void* r3, u8* r4) {
    f32 f2;
    f32 f0;
    f2 = (f32)fn_800CE148(lbl_8047B874 * *(f32*)(lbl_803A1FC8 + 0x24));
    f0 = lbl_8047B88C * f2 + lbl_8047B888;
    *(f32*)(r4 + 0x6c) = f0;
    *(f32*)(r4 + 0x68) = f0;
    if (*(s32*)lbl_803A1FC8 == 2) {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) | 2;
    } else {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) & ~2;
    }
}
#endif

/* fn_800209BC - 0x800209BC | size: 0x90 */
extern f32 lbl_8047B874;
extern f32 lbl_8047B88C;
extern f32 lbl_8047B888;
#if 0
asm void fn_800209BC(void) {
#include "src/game/gs_title_fn_800209BC.inc"
}
#else
#pragma optimization_level 4
void fn_800209BC(void* r3, u8* r4) {
    f32 f2;
    f32 f0;
    f2 = (f32)fn_800CE148(lbl_8047B874 * *(f32*)(lbl_803A1FC8 + 0x24));
    f0 = lbl_8047B88C * f2 + lbl_8047B888;
    *(f32*)(r4 + 0x6c) = f0;
    *(f32*)(r4 + 0x68) = f0;
    if (*(s32*)lbl_803A1FC8 == 1) {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) | 2;
    } else {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) & ~2;
    }
}
#endif

/* fn_80020A4C - 0x80020A4C | size: 0x90 */
extern f32 lbl_8047B874;
extern f32 lbl_8047B88C;
extern f32 lbl_8047B888;
#if 0
asm void fn_80020A4C(void) {
#include "src/game/gs_title_fn_80020A4C.inc"
}
#else
#pragma optimization_level 4
void fn_80020A4C(void* r3, u8* r4) {
    f32 f2;
    f32 f0;
    f2 = (f32)fn_800CE148(lbl_8047B874 * *(f32*)(lbl_803A1FC8 + 0x24));
    f0 = lbl_8047B88C * f2 + lbl_8047B888;
    *(f32*)(r4 + 0x6c) = f0;
    *(f32*)(r4 + 0x68) = f0;
    if (*(s32*)lbl_803A1FC8 == 0) {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) | 2;
    } else {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) & ~2;
    }
}
#endif

/* fn_80020ADC - 0x80020ADC | size: 0x58 */
extern u32 fn_80166C74(void);
#if 0
asm void fn_80020ADC(void) {
#include "src/game/gs_title_fn_80020ADC.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
void fn_80020ADC(void* r3, u8* r4) {
    if (fn_80166C74() != 1) {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) | 2;
    } else {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) & ~2;
    }
}
#endif

/* fn_80020B34 - 0x80020B34 | size: 0x58 */
#if 0
asm void fn_80020B34(void) {
#include "src/game/gs_title_fn_80020B34.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
void fn_80020B34(void* r3, u8* r4) {
    if (fn_80166C74() == 1) {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) | 2;
    } else {
        *(s8*)(r4 + 4) = *(s8*)(r4 + 4) & ~2;
    }
}
#endif

/* fn_80020B8C - 0x80020B8C | size: 0x14 */
#if 0
asm void fn_80020B8C(void) {
#include "src/game/gs_title_fn_80020B8C.inc"
}
#else
#pragma optimization_level 4
void fn_80020B8C(void* r3, u8* r4) {
    *(f32*)(r4 + 0x70) = *(f32*)(lbl_803A1FC8 + 0x10);
}
#endif

/* fn_80020BA0 - 0x80020BA0 | size: 0xfc */
extern void fn_801040F0(s32, s32, void*, s32, s32);
extern s32 fn_80135168(s32, s32);
extern u8 lbl_802EF0A8[];
#if 0
asm void fn_80020BA0(void) {
#include "src/game/gs_title_fn_80020BA0.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
void fn_80020BA0(void* arg0, u8* arg1) {
    s16* ptr;

    if (fn_80166C74() == 0) {
        ptr = (s16*)(lbl_802EF0A8 + 0x8FF0);
    } else {
        ptr = (s16*)(lbl_802EF0A8 + 0x900C);
    }
    fn_801040F0((s16)(ptr[1] - *(s16*)(arg1 + 0x50)), (s16)(ptr[2] - *(s16*)(arg1 + 0x52)), arg0, 0x192, 0);

    if (fn_80135168(0, 9) == 1) {
        ptr = (s16*)(lbl_802EF0A8 + 0x8FD4);
    } else {
        ptr = (s16*)(lbl_802EF0A8 + 0x8FB8);
    }
    fn_801040F0((s16)(ptr[1] - *(s16*)(arg1 + 0x50)), (s16)(ptr[2] - *(s16*)(arg1 + 0x52)), arg0, 0x192, 0);
}
#endif

/* fn_80020C9C - 0x80020C9C | size: 0x200 */
extern void fn_801070F4(void);
extern void fn_801D04E8(void);
extern void fn_80106D3C(s32, s32, s32, s32);
extern void fn_8001E074(void);
extern void fn_801D0748(void);
extern void fn_801069FC(s32);
extern void fn_80102568(s32, s32, s32);
extern u32 lbl_8047B86C;
extern u32 lbl_8047B890;
extern u32 lbl_8047B870;
#if 1
asm void fn_80020C9C(void) {
#include "src/game/gs_title_fn_80020C9C.inc"
}
#else
void fn_80020C9C(void) { /* TODO */ }
#endif

/* fn_80020EA4 - 0x80020EA4 | size: 0xb0 */
extern u32 lbl_8047A360;
extern u32 lbl_802E4EF0[];
extern u32 lbl_802E4ED8[];
#if 1
asm void fn_80020EA4(void) {
#include "src/game/gs_title_fn_80020EA4.inc"
}
#else
#pragma optimization_level 4
void fn_80020EA4(u8* r3, u8* r4) {
    u8* r31;
    s32 idx;
    r31 = r3;
    if ((s32)(&lbl_8047A360)[1] == 4) {
        if (fn_8002091C() >= 0) {
            idx = fn_8002091C();
            fn_800FB680(0, -8, (s32)(r31[0x8b] | -0x100), lbl_802E4EF0[idx]);
        }
    } else if ((s32)(&lbl_8047A360)[1] != 5) {
        if ((s32)lbl_8047A360 >= 0) {
            fn_800FB680(0, -8, (s32)(r31[0x8b] | -0x100), lbl_802E4ED8[(s32)lbl_8047A360]);
        }
    }
}
#endif

/* fn_80020F54 - 0x80020F54 | size: 0x19c */
extern void* fn_80109934(void*);
extern void fn_800D61E4(void);
extern s32 fn_801EF214(void);
extern void fn_801021F8(u32, u32);
extern u8 lbl_803A1FF8[];
extern u8 lbl_80314F98[];
extern u32 lbl_8047B898;
extern u32 lbl_8047B89C;
#if 1
asm void fn_80020F54(void) {
#include "src/game/gs_title_fn_80020F54.inc"
}
#else
#pragma optimization_level 4
void fn_80020F54(u8* arg0, u8* arg1) {
    void* model;
    s16 code;

    code = *(s16*)(arg1 + 6);
    if (code == 0x2D2) {
        model = fn_80109934(lbl_803A1FF8);
        if (model != 0) {
            ((void (*)(u32))fn_800D88DC)(3);
            ((void (*)(u32))fn_800D888C)(4);
            ((void (*)(u32))fn_800D6A00)(7);
            ((void (*)(void*))fn_800D7820)(lbl_80314F98);
            ((void (*)(u32, void*))fn_800D85D4)(0, model);
            ((void (*)(u32))fn_800D67BC)(2);
            ((void (*)(s32, s32))fn_800D61E4)(0, 0);
            ((void (*)(u32, u32, u32, u32, u32))fn_800D5CB8)(0, 0xFF, 0xFF, 0xFF, 0xFF);
            ((void (*)(u32, f32, f32))fn_800D59B8)(0, *(f32*)&lbl_8047B898, *(f32*)&lbl_8047B898);
            ((void (*)(s32, s32))fn_800D61E4)(*(s16*)(arg1 + 0x54), *(s16*)(arg1 + 0x56));
            ((void (*)(u32, u32, u32, u32, u32))fn_800D5CB8)(0, 0xFF, 0xFF, 0xFF, 0xFF);
            ((void (*)(u32, f32, f32))fn_800D59B8)(0, *(f32*)&lbl_8047B89C, *(f32*)&lbl_8047B89C);
            ((void (*)(void))fn_800D6728)();
        }
    } else if (code == 0x90E) {
        if ((s32)lbl_8047A360 >= 0) {
            lbl_8047A360 = (s32)(s8)arg0[0x95];
        }
    } else if ((s32)code >= 0x912 && (s32)code < 0x918) {
        if ((s32)(&lbl_8047A360)[1] == 5) {
            fn_801021F8(0xAA, 0);
        } else {
            fn_801021F8(0xAA, 1);
        }
    }

    if (code == 0x2CD || code == 0x2D0 || code == 0x2D1 || code == 0x2D2) {
        if (fn_801EF214() == 0) {
            arg1[4] = (u8)(s8)(arg1[4] & ~2);
        }
    }
}
#endif

/* fn_800210F0 - 0x800210F0 | size: 0x4d4 */
extern void fn_8005CEE8(void);
extern void fn_800FF58C(void);
extern void fn_8006A718(void);
extern void fn_801CB9D8(u32);
extern void fn_8010A420(void*);
extern void fn_80029760(void);
extern void fn_80029638(void);
extern void fn_80128E38(void);
extern void fn_800056E4(void);
extern void fn_800056EC(void);
extern void fn_80130054(void);
extern void fn_8010A5BC(void);
extern void fn_8010A010(void);
extern void fn_8018F6F4(void);
extern void fn_8018F4C8(void);
extern void fn_80109894(void);
extern void fn_80005748(void);
extern void fn_801EF274(void);
extern void fn_80113FE8(void);
extern u32 lbl_8047A35C;
extern u32 lbl_8047B898;
extern u32 lbl_8047A358;
#if 1
asm void fn_800210F0(void) {
#include "src/game/gs_title_fn_800210F0.inc"
}
#else
void fn_800210F0(void) { /* TODO */ }
#endif

/* fn_800215C4 - 0x800215C4 | size: 0x60 */
extern u32 lbl_8047A35C;
#if 0
asm void fn_800215C4(void) {
#include "src/game/gs_title_fn_800215C4.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
void fn_800215C4(void) {
    fn_80102568(0xaa, 0, 1);
    fn_80102568(0x7a, 0, 1);
    fn_80102568(0x7f, 0, 1);
    fn_801CB9D8(lbl_8047A35C);
    fn_8010A420(lbl_803A1FF8);
}
#pragma peephole on
#endif

/* fn_80021624 - 0x80021624 | size: 0x20 */
#if 0
asm void fn_80021624(void) {
#include "src/game/gs_title_fn_80021624.inc"
}
#else
#pragma optimization_level 4
void fn_80021624(void) {
    fn_800210F0();
}
#endif

/* fn_80021644 - 0x80021644 | size: 0x9c */
extern void fn_80177A44(void);
extern u32 lbl_8047A35C;
extern u32 lbl_8047A358;
#if 0
asm void fn_80021644(void) {
#include "src/game/gs_title_fn_80021644.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
void fn_80021644(void) {
    extern void fn_80165A20(s32, s32, s32);
    extern void* fn_80113F48(void);
    extern u32 fn_801CBA0C(s32);
    extern void fn_800F9318(void*, u32);
    extern void fn_80176E0C(s32, s32, s32, s32);
    extern void fn_80177A44(s32);
    void* handle;

    if (fn_800FF548() == 0) {
        fn_80165A20(0x446, 0, 0xff);
        handle = fn_80113F48();
        lbl_8047A35C = fn_801CBA0C(0x0FFE1000);
        fn_800F9318(handle, lbl_8047A35C);
        fn_80176E0C(0x632, 0x0FFF1800, 0, 1);
        fn_80177A44(4);
        lbl_8047A360 = lbl_8047A358;
        (&lbl_8047A360)[1] = 0;
    }
}
#pragma peephole on
#endif

/* fn_800216E8 - 0x800216E8 | size: 0x1d4 */
extern void fn_800F96E4(void);
extern void* fn_8011F4F0(s32);
extern u8 lbl_80266C7C[];
#if 1
asm void fn_800216E8(void) {
#include "src/game/gs_title_fn_800216E8.inc"
}
#else
#pragma optimization_level 4
void fn_800216E8(void* arg0, s32 arg1, u8* arg2, s16 arg3, s32 arg4) {
    struct TitleNameCand {
        s32 code;
        s32 msg;
        s32 link;
    } table[21];
    s32 i;
    s32 j;
    s32 msg;
    s32 link;
    s16 x;
    s16 y;

    for (i = 0; i < 21; i++) {
        table[i] = ((struct TitleNameCand*)lbl_80266C7C)[i];
    }

    if (arg3 <= 0) {
        fn_800F96E4(arg0, arg1 + 1, (void*)0x4261);
        return;
    }

    for (i = 0; i < 21; i++) {
        for (j = 0; j < arg3; j++) {
            if (table[i].code == *(s32*)(arg2 + j * 8)) {
                break;
            }
        }
        if (j >= arg3) {
            break;
        }
    }

    if (i >= 21) {
        fn_800F96E4(arg0, arg1 + 1, (void*)0x4261);
        return;
    }

    link = table[i].link;
    msg = table[i].msg;
    x = *(s16*)(arg2 + j * 8 + 4);
    y = *(s16*)(arg2 + j * 8 + 6);

    if (link >= 0) {
        for (i = i + 1; i < 21; i++) {
            if (table[i].link >= 0 && table[i].link != link) {
                for (j = 0; j < arg3; j++) {
                    if (table[i].code == *(s32*)(arg2 + j * 8)) {
                        break;
                    }
                }
                if (j >= arg3) {
                    msg = 0x4201;
                    break;
                }
            }
        }
    }

    fn_80132A38(0x32, (void*)fn_8011F4F0(arg4));
    fn_80132A38(0x2F, (void*)(s32)x);
    fn_80132A38(0x30, (void*)(s32)y);
    fn_800F96E4(arg0, arg1 + 1, (void*)msg);
}
#endif

/* fn_800218BC - 0x800218BC | size: 0x1e0 */
extern void fn_80014118(s32, void*, void*);
extern s32 fn_80123FBC(s32);
extern u8 fn_8011FC74(s32);
extern void fn_8011FC14(void);
extern s32 fn_800141BC(void*, s32);
extern void fn_80014198(s32);
extern u8 lbl_803A1B90[];
extern u32 lbl_8047B8A0;
#if 1
asm void fn_800218BC(void) {
#include "src/game/gs_title_fn_800218BC.inc"
}
#else
void fn_800218BC(void) { /* TODO */ }
#endif

/* fn_80021A9C - 0x80021A9C | size: 0x78 */
#if 0
asm void fn_80021A9C(void) {
#include "src/game/gs_title_fn_80021A9C.inc"
}
#else
#pragma optimization_level 4
s32 fn_80021A9C(u32 r3, u32* r4) {
    u8* r5;
    s32 count;

    r5 = lbl_803A1B90;
    count = *(s32*)(lbl_803A1B90 + 0x40);
    for (; count > 0; count--) {
        if (r3 == (u32)*(u16*)r5) {
            *r4 = 0;
            return 2;
        }
        r5 += 8;
    }
    fn_80106D3C(2, 0x426a, 1, 0);
    fn_801069FC(1);
    return 1;
}
#endif

/* fn_80021B14 - 0x80021B14 | size: 0x53c */
extern void fn_80142EF8(void);
extern void fn_801431AC(void);
extern void fn_80014110(void);
extern s32 fn_80121ADC(s32, s32);
extern s16 fn_80144574(void*, s32, s32, u16, s32);
extern void fn_80166A50(s32, s32, s32, s32);
extern void fn_8001D378(void);
extern u8 lbl_80478890[];
extern u8 lbl_80266DB0[];
extern u8 lbl_80266D78[];
extern void fn_80023968(void);
#if 1
asm void fn_80021B14(void) {
#include "src/game/gs_title_fn_80021B14.inc"
}
#else
void fn_80021B14(void) { /* TODO */ }
#endif

/* fn_80022050 - 0x80022050 | size: 0x12c */
extern s32 fn_801347E0(void);
extern s32 fn_801347E8(s32, s8);
extern void fn_800140FC(s32*, s32*);
extern s32 fn_801F7EF0(s32);
extern void fn_80023968(void);
#if 0
asm void fn_80022050(void) {
#include "src/game/gs_title_fn_80022050.inc"
}
#else
#pragma optimization_level 4
#pragma peephole off
s32 fn_80022050(s32 arg0, s32* arg1) {
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 valid_count;
    s32 i;

    valid_count = 0;
    i = 0;
    while (i < 6) {
        fn_80014118(i, &spC, &sp8);
        if ((u8)fn_80123FBC(spC) != 0) {
            valid_count++;
        }
        i++;
    }
    if (valid_count >= 6) {
        valid_count = (s8)fn_801347E0();
        i = 0;
        while (i < valid_count) {
            if ((s8)fn_801347E8(0, (s8)i) > 0) {
                break;
            }
            i++;
        }
        if (i >= valid_count) {
            fn_80106D3C(2, 0x4416, 1, 0);
            fn_801069FC(1);
            return 1;
        }
    }
    fn_800140FC(&sp14, &sp10);
    if ((u8)fn_801F7EF0(sp10) != 0) {
        fn_80106D3C(2, 0x426d, 1, 0);
        fn_801069FC(1);
        return 1;
    }
    *arg1 = 0;
    return 0;
}
#endif

/* fn_8002217C - 0x8002217C | size: 0x2fc */
#if 1
asm void fn_8002217C(void) {
#include "src/game/gs_title_fn_8002217C.inc"
}
#else
void fn_8002217C(void) { /* TODO */ }
#endif

/* fn_80022478 - 0x80022478 | size: 0x2a8 */
extern void jumptable_802E4F00();
extern u8 lbl_80478888[];
extern void fn_80023068(void);
extern void fn_800232F0(void);
extern void fn_80023760(void);
#if 1
asm void fn_80022478(void) {
#include "src/game/gs_title_fn_80022478.inc"
}
#else
#pragma optimization_level 4
s32 fn_80022478(u32 arg0, u32* arg1) {
    u8 state_buf[0x110];
    u8 text_buf[0x100];
    u8 name_buf[0x40];
    s32 sel;
    s32 slot;
    s32 effect;
    s32 sc;
    s32 sd;
    s32 sound_id;

    fn_80142EF8(state_buf, lbl_80478888);
    sel = fn_801431AC(state_buf);
    if ((u32)sel > 0x15) {
        fn_80106D3C(2, 0x426A, 1, 0);
        fn_801069FC(1);
        return 1;
    }

    switch (sel) {
    case 1:
        return ((s32 (*)(u32, u32*))fn_80023760)(arg0, arg1);
    case 2:
        return ((s32 (*)(u32, u32*))fn_800232F0)(arg0, arg1);
    case 19:
    case 20:
        return ((s32 (*)(u32, u32*))fn_80023068)(arg0, arg1);
    case 21:
        return ((s32 (*)(u32, u32*))fn_80023968)(arg0, arg1);
    case 0:
    case 8:
    case 9:
    case 18:
        fn_80106D3C(2, 0x426A, 1, 0);
        fn_801069FC(1);
        return 1;
    default:
        slot = fn_800141BC((void*)arg0, 1);
        if (slot >= 0) {
            fn_80014118(slot, &sc, &sd);
            if ((u8)fn_80121ADC(sc, 0x3E) == 0) {
                effect = fn_80144574(text_buf, sc, sd, (u16)arg0, 0);
                if ((s16)effect > 0) {
                    if (arg0 == *(u16*)(lbl_80266DB0 + 0x0) ||
                        arg0 == *(u16*)(lbl_80266DB0 + 0x2) ||
                        arg0 == *(u16*)(lbl_80266DB0 + 0x4) ||
                        arg0 == *(u16*)(lbl_80266DB0 + 0x6) ||
                        arg0 == *(u16*)(lbl_80266DB0 + 0x8)) {
                        sound_id = 0x466;
                    } else {
                        sound_id = 0x465;
                    }
                    fn_80166A50(sound_id, 0, 0xFF, 0);
                    fn_8001D378();
                }
                fn_800216E8(name_buf, 0x40, (u8*)sd, effect, sc);
                fn_80132A38(0x4D, name_buf);
                fn_80106D3C(2, 0xE0, 1, 0);
                fn_801069FC(1);
            } else {
                fn_80132A38(0x32, (void*)fn_8011F4F0(sc));
                fn_80106D3C(2, 0x424D, 1, 0);
                fn_801069FC(1);
                effect = 0;
            }
        } else {
            effect = 0;
        }

        fn_80014198(slot);
        if (slot >= 0 && (s16)effect > 0) {
            if (arg0 >= 0x27 && arg0 < 0x2C) {
                *arg1 = 0;
            } else {
                *arg1 = 1;
            }
            return 0;
        }
        return 1;
    }
}
#endif

/* fn_80022720 - 0x80022720 | size: 0x114 */
extern u8 lbl_80266C54[];
extern void fn_80023068(void);
extern void fn_800232F0(void);
#if 0
asm void fn_80022720(void) {
#include "src/game/gs_title_fn_80022720.inc"
}
#else
#pragma optimization_level 4
void fn_80022720(u32 r3, u32* r4) {
    u32* r31;
    u8* r30;
    s32 r29;
    u32 buf[10];
    r31 = r4;
    r30 = lbl_80266C54;
    buf[0] = *(u32*)(r30 + 0x00); buf[1] = *(u32*)(r30 + 0x04);
    buf[2] = *(u32*)(r30 + 0x08); buf[3] = *(u32*)(r30 + 0x0c);
    buf[4] = *(u32*)(r30 + 0x10); buf[5] = *(u32*)(r30 + 0x14);
    buf[6] = *(u32*)(r30 + 0x18); buf[7] = *(u32*)(r30 + 0x1c);
    buf[8] = *(u32*)(r30 + 0x20); buf[9] = *(u32*)(r30 + 0x24);
    r29 = 5;
    if (r3 == (u32)*(u16*)((u8*)buf + 0x00)) { r29 = 0; }
    else if (r3 == (u32)*(u16*)((u8*)buf + 0x08)) { r29 = 1; }
    else if (r3 == (u32)*(u16*)((u8*)buf + 0x10)) { r29 = 2; }
    else if (r3 == (u32)*(u16*)((u8*)buf + 0x18)) { r29 = 3; }
    else if (r3 == (u32)*(u16*)((u8*)buf + 0x20)) { r29 = 4; }
    fn_80106D3C(2, (s32)*(u32*)((u8*)buf + r29 * 8 + 4), 1, 0);
    fn_801069FC(1);
    *r31 = 0;
}
#endif

/* fn_80022834 - 0x80022834 | size: 0x308 */
extern void fn_800232F0(void);
extern void fn_801440A0(void);
extern void fn_80143F24(void);
extern void fn_80143EF0(void);
extern void fn_80143E88(void);
extern void fn_8011F228(void);
extern void fn_8011F5C8(void);
extern void fn_8011E778(void);
extern void fn_8011E2AC(void);
extern void fn_802600E4(void);
extern void fn_80123D58(void);
extern void fn_80123090(void);
extern void fn_80122370(void);
#if 1
asm void fn_80022834(void) {
#include "src/game/gs_title_fn_80022834.inc"
}
#else
#pragma optimization_level 4
s32 fn_80022834(u32 arg0, u32* arg1) {
    s32 slot;
    s32 sc;
    s32 sd;
    void* handle;
    u32 status;
    u32 value16;
    s32 state;
    s32 i;
    s32 result;
    u8 type_byte;

    handle = ((void* (*)(u32))fn_801440A0)((u16)arg0);
    status = (u8)((u32 (*)(void*))fn_80143F24)(handle);
    value16 = (u16)((u32 (*)(u32))fn_80143EF0)(status);

    ((void (*)(u32, u32, u32, u32))fn_80166A50)(0x4CB, 0, 0xFF, 0);
    type_byte = (u8)((u32 (*)(void*))fn_80143E88)(handle);
    if (type_byte != 0xFF) {
        result = 0x4260;
    } else {
        result = 0x4265;
    }

    fn_80132A38(0x39, (void*)(u16)value16);
    fn_80106D3C(2, result, 1, 0);
    fn_801069FC(1);
    fn_80106D3C(2, 0x426B, 1, 0);

    state = (s8)fn_8001E074(0, -1, -1, 0);
    fn_801069FC(1);
    if (state == 0 || state == 1) {
        return 1;
    }

    slot = fn_800141BC((void*)arg0, 1);
    if (slot >= 0) {
        fn_80014118(slot, &sc, &sd);
        if ((u8)((u32 (*)(u32))fn_8011FC74)(sc) != 0) {
            fn_80106D3C(2, 0x424C, 1, 0);
            fn_801069FC(1);
            result = 0;
        } else {
            value16 = (u16)((u32 (*)(u32))fn_80143EF0)(status);
            for (i = 0; i < 4; i++) {
                if (value16 == (u16)((u32 (*)(u32, u32))fn_8011F228)(sc, (u16)i)) {
                    break;
                }
            }
            if (i < 4) {
                fn_80132A38(0x32, (void*)((u32 (*)(u32))fn_8011F4F0)(sc));
                fn_80132A38(0x39, (void*)(u16)value16);
                fn_80106D3C(2, 0x4244, 1, 0);
                fn_801069FC(1);
                result = 0;
            } else {
                ((void (*)(u32))fn_8011F5C8)(sc);
                ((void (*)(void))fn_8011E778)();
                if ((u8)((u32 (*)(u32, u32))fn_8011E2AC)(status, sc) == 0) {
                    fn_80132A38(0x32, (void*)((u32 (*)(u32))fn_8011F4F0)(sc));
                    fn_80132A38(0x39, (void*)(u16)value16);
                    fn_80106D3C(2, 0x423F, 1, 0);
                    fn_801069FC(1);
                    result = 0;
                } else {
                    result = ((s32 (*)(u32, u32, void*, u32, void*, u32))fn_802600E4)(sc, (u16)value16, &type_byte, 1, fn_80023274, 0);
                    if (result != 0) {
                        ((void (*)(u32, u32, u8))fn_80123D58)(sc, (u16)value16, type_byte);
                        ((void (*)(u32))fn_80123090)(sc);
                        ((void (*)(u32, u32, u32))fn_80122370)(sc, ((u32 (*)(u32))fn_80123090)(sc), 4);
                    }
                }
            }
        }
    } else {
        result = 0;
    }

    fn_80014198(slot);
    if (slot >= 0 && result != 0) {
        if (type_byte != 0xFF) {
            *arg1 = 0;
        } else {
            *arg1 = 1;
        }
        return 0;
    }
    return 1;
}
#endif

/* fn_80022B3C - 0x80022B3C | size: 0x318 */
extern void fn_8011E15C(void);
extern void fn_80166A28(void);
extern void fn_801666BC(void);
extern void fn_80121B4C(void);
extern void fn_8011F910(void);
extern u8 lbl_80266C30[];
extern u32 lbl_8047B8A0;
#if 1
asm void fn_80022B3C(void) {
#include "src/game/gs_title_fn_80022B3C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
s32 fn_80022B3C(s32 arg0, s32 arg1) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 temp_r28;
    s32 temp_r29;
    s32 temp_r30;
    s32 temp_r31;
    s32 result;
    s32 i;
    s32 count;
    f32 f0;
    void* data_ptr;

    data_ptr = &lbl_80266C30;
    temp_r30 = arg0;
    
    /* Copy data from structure */
    sp10 = *(s32*)data_ptr;
    sp14 = *(s32*)((u8*)data_ptr + 4);
    sp18 = *(s32*)((u8*)data_ptr + 8);
    sp1C = *(s32*)((u8*)data_ptr + 0xC);
    sp20 = *(s32*)((u8*)data_ptr + 0x10);
    sp24 = *(s32*)((u8*)data_ptr + 0x14);
    sp28 = *(s32*)((u8*)data_ptr + 0x18);
    sp2C = *(s32*)((u8*)data_ptr + 0x1C);
    
    fn_80014118(&spC, &sp8);
    result = fn_80123FBC(spC);
    
    if ((result & 0xFF000000) == 0) {
        return 1;
    }
    
    /* Determine particle type */
    temp_r29 = 0;
    result = temp_r30 & 0xFFFF;
    
    if (result == (u16)sp10) {
        temp_r29 = 0;
    } else if (result == (u16)sp1C) {
        temp_r29 = 1;
    } else if (result == (u16)sp28) {
        temp_r29 = 2;
    } else {
        temp_r29 = 3;
    }
    
    if (temp_r29 >= 3) {
        return 1;
    }
    
    /* Play particle sound effects */
    fn_8011F4F0(spC);
    fn_80132A38(0x32, result);
    fn_80132A38(0x2D, temp_r30);
    
    result = *(s32*)((u8*)(&sp10) + temp_r29 * 0xC);
    fn_80106D3C(2, result, 1, 0);
    fn_801069FC(1);
    
    /* Check if particle system is available */
    if (fn_80135168(0, 9) == 0) {
        /* Wait for particle system with timeout */
        for (i = 0; i < 2; i++) {
            fn_800F78A4(1, 0, 0xFF, 0x15, 0);
            count = 0;
            
            while (count < 0x3C) {
                fn_800F0308();
                count += fn_800D3088();
            }
        }
        
        /* Additional wait */
        fn_800F78A4(1, 0, 0xFF, 0x30, 0);
        count = 0;
        
        while (count < 0x60) {
            fn_800F0308();
            count += fn_800D3088();
        }
    }
    
    /* Validate and process particle */
    temp_r29 = spC;
    if (temp_r29 == 0) {
        return 0;
    }
    
    result = fn_80123FBC(temp_r29);
    if ((result & 0xFF000000) == 0) {
        return 0;
    }
    
    fn_8011F5C8(temp_r29);
    fn_8011E778();
    
    result = fn_8011E15C();
    if (result == 0) {
        temp_r29 = 0;
    } else {
        temp_r29 = result & 0xFFFF;
        fn_80166A28(temp_r29);
    }
    
    /* Wait for particle completion */
    while (fn_801666BC(temp_r29) == 2) {
        fn_800F0308();
    }
    
    /* Update particle state */
    fn_8011F4F0(spC);
    result = fn_80121ADC(spC, 0x3E);
    
    if ((result & 0xFF000000) != 0) {
        fn_80121B4C(spC, 0x3E);
        fn_80106D3C(2, 0x4277, 1, 0);
        fn_801069FC(1);
    }
    
    /* Final particle processing */
    fn_8011FC14(spC);
    f0 = lbl_8047B8A0;
    
    if (f1 > f0) {
        fn_8011F910(spC, temp_r30 & 0xFFFF, 4);
        fn_8011F4F0(spC);
        result = fn_80121ADC(spC, 0x3E);
        
        if ((result & 0xFF000000) != 0) {
            result = *(s32*)((u8*)(&sp10) + temp_r29 * 0xC);
            fn_80106D3C(2, result, 1, 0);
            fn_801069FC(1);
        }
    }
    
    return 1;
}
#endif

/* fn_80022E54 - 0x80022E54 | size: 0x90 */
extern void fn_800190D8(void*, s32);
#if 0
asm void fn_80022E54(void) {
#include "src/game/gs_title_fn_80022E54.inc"
}
#else
#pragma optimization_level 4
s32 fn_80022E54(void* r3, u32* r4) {
    void* r29;
    u32* r30;
    s32 r31;
    u32 sc, sd;
    r29 = r3;
    r30 = r4;
    r31 = fn_800141BC(r29, 1);
    if (r31 >= 0) {
        fn_80014118(r31, &sc, &sd);
    }
    fn_80014198(r31);
    if (r31 >= 0) {
        fn_800190D8(r29, 0x59610);
        *r30 = 1;
        return 2;
    }
    return 1;
}
#endif

/* fn_80022EE4 - 0x80022EE4 | size: 0x184 */
extern s32 fn_80128A64(s32, s32, u16, void*, void*);
extern void fn_801096F8(s32);
extern void fn_8012805C(s32, s32, u16, void*, s32, s32, s32, s32);
extern f32 lbl_8047B8A4;
#if 0
asm void fn_80022EE4(void) {
#include "src/game/gs_title_fn_80022EE4.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
s32 fn_80022EE4(u32 arg0, u32* arg1) {
    extern s32 fn_80144574(void*, s32, s32, u16, s32);
    u8 text_buf[0x100];
    s32 slot;
    s32 effect;
    s32 sc;
    s32 sd;
    u16 sp8;
    u32 spC;

    slot = fn_800141BC((void*)arg0, 1);
    if (slot >= 0) {
        fn_80014118(slot, &sc, &sd);
        if ((u8)fn_8011FC74(sc) == 0) {
            effect = fn_80144574(text_buf, sc, sd, (u16)arg0, 0);
            if ((s16)effect <= 0) {
                fn_80106D3C(2, 0x4261, 1, 0);
                fn_801069FC(1);
            }
        } else {
            fn_80106D3C(2, 0x424C, 1, 0);
            fn_801069FC(1);
        }
    }

    fn_80014198(slot);

    if (slot >= 0 && (s16)effect > 0) {
        effect = fn_80128A64(sc, 1, (u16)arg0, &sp8, &spC);
        fn_801C41C8(lbl_8047B8A4, 3);
        fn_801C40F0(1);
        fn_801096F8(0);
        fn_8012805C(sc, effect, sp8, &spC, 0, 1, 0, 0);
        fn_801096F8(1);
        fn_801C41C8(lbl_8047B8A4, 2);
        fn_801C40F0(1);
        *arg1 = 1;
        return 0;
    }

    return 1;
}
#endif

/* fn_80023068 - 0x80023068 | size: 0x20c */
extern s32 fn_80019B48(s32);
extern void fn_80019B1C(void);
#if 1
asm void fn_80023068(void) {
#include "src/game/gs_title_fn_80023068.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
s32 fn_80023068(u32 arg0, u32* arg1) {
    extern s32 fn_80144574(void*, s32, s32, u16, s32);
    extern void fn_800216E8(void*, s32, void*, s32, s32);
    u8 name_buf[0x84];
    u8 text_buf[0x108];
    s32 blocked;
    s32 mode;
    s32 slot;
    s32 sc;
    s32 sd;
    s32 effect;

    blocked = 0;
    for (;;) {
        slot = fn_800141BC((void*)arg0, 1);
        if (slot < 0) {
            break;
        }
        fn_80014118(slot, &sc, &sd);
        if ((u8)fn_80121ADC(sc, 0x3E) != 0) {
            blocked = 1;
            break;
        }
        mode = fn_80019B48((s8)slot);
        fn_80019B1C();
        if (mode >= 0) {
            break;
        }
    }

    if (slot >= 0) {
        if (blocked != 0) {
            fn_80132A38(0x32, fn_8011F4F0(sc));
            fn_80106D3C(2, 0x424D, 1, 0);
            fn_801069FC(1);
            effect = 0;
        } else {
            effect = fn_80144574(text_buf, sc, sd, (u16)arg0, (u8)mode);
            if ((s16)effect > 0) {
                s32 sound_id;
                if (arg0 == *(u16*)(lbl_80266DB0 + 0x0) ||
                    arg0 == *(u16*)(lbl_80266DB0 + 0x2) ||
                    arg0 == *(u16*)(lbl_80266DB0 + 0x4) ||
                    arg0 == *(u16*)(lbl_80266DB0 + 0x6) ||
                    arg0 == *(u16*)(lbl_80266DB0 + 0x8)) {
                    sound_id = 0x466;
                } else {
                    sound_id = 0x465;
                }
                fn_80166A50(sound_id, 0, 0xFF, 0);
                fn_8001D378();
            }
            fn_800216E8(name_buf, 0x40, text_buf, effect, sc);
            fn_80132A38(0x4D, name_buf);
            fn_80106D3C(2, 0xE0, 1, 0);
            fn_801069FC(1);
        }
    }

    fn_80014198(slot);
    if (slot >= 0 && (s16)effect > 0) {
        *arg1 = 1;
        return 0;
    }
    return 1;
}
#endif

/* fn_80023274 - 0x80023274 | size: 0x7c */
extern s32 fn_80097B04(s32, s32);
extern f32 lbl_8047B8A4;
#if 0
asm void fn_80023274(void) {
#include "src/game/gs_title_fn_80023274.inc"
}
#else
#pragma scheduling on
#pragma optimization_level 4
s32 fn_80023274(s32 r3, s32 r4) {
    s32 result;

    fn_801C41C8(lbl_8047B8A4, 3);
    fn_801C40F0(1);
    result = fn_80097B04(r3, r4);
    if (result >= 4) {
        result = -1;
    }
    fn_801C41C8(lbl_8047B8A4, 2);
    fn_801C40F0(1);
    return (s8)result;
}
#pragma peephole on
#endif

/* fn_800232F0 - 0x800232F0 | size: 0x470 */
extern void fn_8012640C(void);
extern void fn_80165668(void);
extern void fn_8011F4A8(void);
extern void fn_80105D48(void);
extern void fn_80105C68(void);
extern void fn_80123B5C(void);
extern void fn_801236F8(void);
extern f32 lbl_8047B8A4;
#if 1
asm void fn_800232F0(void) {
#include "src/game/gs_title_fn_800232F0.inc"
}
#else
void fn_800232F0(void) { /* TODO */ }
#endif

/* fn_80023760 - 0x80023760 | size: 0x208 */
#if 1
asm void fn_80023760(void) {
#include "src/game/gs_title_fn_80023760.inc"
}
#else
#pragma optimization_level 4
s32 fn_80023760(u32 arg0, u32* arg1) {
    u8 buf[0x80];
    u16 entries[5];
    s32 total;
    s32 slot;
    s32 count;
    s32 effect;
    u32 sc;
    u32 sd;
    u32 species;
    u32 msg;

    total = 0;
    fn_800141BC((void*)arg0, 0);
    for (slot = 0; slot < 6; slot++) {
        fn_80014118(slot, &sc, &sd);
        if ((u8)fn_80123FBC(sc) != 0) {
            if ((s32)fn_8012640C(sc, 0, 0x83, 0) <= 0) {
                if ((u8)fn_80121ADC(sc, 0x3E) == 0) {
                    effect = fn_80144574(buf, sc, sd, (u16)arg0, 0);
                    if ((s16)effect > 0) {
                        memcpy(entries, lbl_80266DB0, sizeof(entries));
                        count = 0;
                        if (arg0 == entries[0]) {
                            count = 0;
                        } else if (arg0 == entries[1]) {
                            count = 1;
                        } else if (arg0 == entries[2]) {
                            count = 2;
                        } else if (arg0 == entries[3]) {
                            count = 3;
                        } else if (arg0 == entries[4]) {
                            count = 4;
                        } else {
                            count = 5;
                        }
                        if (count < 5) {
                            species = 0x466;
                        } else {
                            species = 0x465;
                        }
                        fn_80166A50(species, 0, 0xFF, 0);
                        fn_8001D378();
                        fn_800216E8(buf, 0x40, (u8*)sd, effect, sc);
                        fn_80132A38(0x4D, buf);
                        fn_80106D3C(2, 0xE0, 1, 0);
                        fn_801069FC(1);
                        total = (u16)(total + effect);
                    }
                }
            }
        }
    }
    if ((u16)total == 0) {
        fn_80106D3C(2, 0x4261, 1, 0);
        fn_801069FC(1);
        effect = -1;
    } else {
        effect = 1;
    }
    fn_80014198(-1);
    if (effect < 0) {
        return 1;
    }
    *arg1 = 1;
    return 0;
}
#endif

/* fn_80023968 - 0x80023968 | size: 0x234 */
extern void fn_80143DFC(void);
extern void fn_80143A94(void);
extern void fn_801437B8(void);
#if 1
asm void fn_80023968(void) {
#include "src/game/gs_title_fn_80023968.inc"
}
#else
void fn_80023968(void) { /* TODO */ }
#endif

/* fn_80023B9C - 0x80023B9C | size: 0x20c */
#if 1
asm void fn_80023B9C(void) {
#include "src/game/gs_title_fn_80023B9C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
s32 fn_80023B9C(u32 arg0, u32* arg1) {
    u8 state_buf[0x110];
    u8 text_buf[0x100];
    u8 name_buf[0x40];
    s32 result;
    s32 slot;
    s32 effect;
    s32 sc;
    s32 sd;
    u32 species;
    u32 msg;
    s32 r28 = 0;
    
    result = fn_800141BC((void*)arg0, 1);
    result = fn_80014118(&state_buf[12], &text_buf[8]);
    
    if (result >= 0) {
        if (fn_80121ADC(result, 0x3E) == 0) {
            species = (u32)(u16)arg0;
            slot = fn_80144574(&state_buf[28], &text_buf[0xA0], species, 0, 0);
            
            if (slot > 0) {
                u16* bgm_table = (u16*)lbl_80266DB0;
                s32 match_index = 5;
                
                for (s32 i = 0; i < 5; i++) {
                    if (species == bgm_table[i]) {
                        match_index = i;
                        break;
                    }
                }
                
                if (match_index < 5) {
                    msg = 0x466;
                } else {
                    msg = 0x465;
                }
                
                fn_80166A50(0x466, 0, 0xFF, 0);
                fn_8001D378();
            }
            
            fn_800216E8(&state_buf[28], slot, &text_buf[0xA0], 0x40);
            fn_80132A38(0x4D, &state_buf[28]);
            fn_80106D3C(2, 0x4D, 1, 0);
            fn_801069FC(1);
        } else {
            result = fn_8011F4F0(result);
            fn_80132A38(0x32, result);
            fn_80106D3C(2, 0x424D, 1, 0);
            fn_801069FC(1);
            r28 = 0;
        }
    }
    
    result = fn_80014198(result);
    
    if (result >= 0 && r28 > 0) {
        if (arg0 < 0x2C) {
            if (arg0 >= 0x27) {
                arg1[0] = 0;
            } else {
                arg1[0] = 1;
            }
        } else {
            arg1[0] = 1;
        }
        return 0;
    }
    
    return 1;
}
#endif

/* fn_80023DA8 - 0x80023DA8 | size: 0x3c */
#if 0
asm void fn_80023DA8(void) {
#include "src/game/gs_title_fn_80023DA8.inc"
}
#else
#pragma optimization_level 4
s32 fn_80023DA8(void) {
    fn_80106D3C(2, 0x44c6, 1, 0);
    fn_801069FC(1);
    return 1;
}
#endif

/* fn_80023DE4 - 0x80023DE4 | size: 0x3c */
#if 0
asm void fn_80023DE4(void) {
#include "src/game/gs_title_fn_80023DE4.inc"
}
#else
#pragma optimization_level 4
s32 fn_80023DE4(void) {
    fn_80106D3C(2, 0x4261, 1, 0);
    fn_801069FC(1);
    return 1;
}
#endif

/* fn_80023E20 - 0x80023E20 | size: 0x3c */
#if 0
asm void fn_80023E20(void) {
#include "src/game/gs_title_fn_80023E20.inc"
}
#else
#pragma optimization_level 4
s32 fn_80023E20(void) {
    fn_80106D3C(2, 0x426a, 1, 0);
    fn_801069FC(1);
    return 1;
}
#endif

/* fn_80023E5C - 0x80023E5C | size: 0x4 */
#if 0
asm void fn_80023E5C(void) {
#include "src/game/gs_title_fn_80023E5C.inc"
}
#else
#pragma optimization_level 4
void fn_80023E5C(void) { }
#endif

/* fn_80023E60 - 0x80023E60 | size: 0x300 */
extern void fn_80104318(void);
extern void fn_800E0060(void);
extern void fn_800E0000(void);
extern u32 lbl_8047A370;
extern u32 lbl_8047A368;
extern u32 lbl_8047A36C;
extern f32 lbl_8047B8A8;
extern f32 lbl_80478898;
extern f64 lbl_8047B8B8;
extern f32 lbl_8047B8AC;
extern u32 lbl_8047A390;
extern f32 lbl_8047B8B0;
extern u8 lbl_8047A380;
extern void fn_80024160(void);
#if 1
asm void fn_80023E60(void) {
#include "src/game/gs_title_fn_80023E60.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling off
s32 fn_80023E60(void* arg0) {
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 result;
    s32 temp_r29;
    s32 temp_r30;
    void* temp_r3;
    void* temp_r4;
    void* temp_r5;
    f64 f2;
    f32 f0;
    f32 f1;
    f32 f4;
    s32 i;
    s32 found;

    if ((s32)lbl_8047A370 == 1) {
        return 0;
    }
    
    temp_r3 = *(void**)((u8*)arg0 + 4);
    temp_r29 = fn_8005DA18(temp_r3);
    temp_r30 = fn_80104318(arg0);
    
    if (temp_r30 == 0) {
        ((u8*)arg0)[0x95] = 0;
        return 0;
    }
    
    fn_80024160(arg0, temp_r30, temp_r29);
    
    result = (s8)((u8*)arg0)[0x95] + (s8)((u8*)arg0)[0x94];
    temp_r29 = (s32)lbl_8047A368;
    lbl_8047A36C = result;
    
    if (temp_r29 != result) {
        f0 = lbl_8047B8A8;
        lbl_8047A370 = 1;
        lbl_80478898 = f0;
        
        temp_r3 = *(void**)((u8*)arg0 + 4);
        temp_r3 = (void*)fn_8005DA18(temp_r3);
        temp_r3 = (void*)fn_8005D934(*(s16*)((u8*)temp_r3 + 4));
        
        found = 0;
        while (temp_r3 != 0) {
            if ((((u8*)temp_r3)[0] >> 7) & 1) {
                if (temp_r29 == found) {
                    break;
                }
                found++;
            }
            
            if ((((u8*)temp_r3)[0] >> 6) & 1) {
                break;
            }
            
            temp_r3 = (void*)fn_8005D934(*(s16*)((u8*)temp_r3 + 0x18));
        }
        
        if (temp_r3 != 0) {
            sp2C = *(s16*)((u8*)temp_r3 + 2) ^ 0x8000;
            sp28 = *(s16*)((u8*)temp_r3 + 4) ^ 0x8000;
            
            f2 = lbl_8047B8B8;
            f0 = lbl_8047B8AC;
            *(f32*)&sp20 = (f32)(sp2C - f2);
            *(f32*)&sp24 = (f32)(sp28 - f2);
            
            temp_r4 = (void*)((u32)lbl_803A2058 & 0xFFFF);
            *(f32*)((u8*)temp_r4 + 0) = *(f32*)&sp20;
            *(f32*)((u8*)temp_r4 + 4) = *(f32*)&sp24;
            *(f32*)((u8*)temp_r4 + 8) = f0;
        }
    }
    
    result = (s32)lbl_8047A36C;
    temp_r3 = *(void**)((u8*)arg0 + 4);
    temp_r3 = (void*)fn_8005DA18(temp_r3);
    temp_r3 = (void*)fn_8005D934(*(s16*)((u8*)temp_r3 + 4));
    
    found = 0;
    while (temp_r3 != 0) {
        if ((((u8*)temp_r3)[0] >> 7) & 1) {
            if (result == found) {
                break;
            }
            found++;
        }
        
        if ((((u8*)temp_r3)[0] >> 6) & 1) {
            break;
        }
        
        temp_r3 = (void*)fn_8005D934(*(s16*)((u8*)temp_r3 + 0x18));
    }
    
    if (temp_r3 != 0) {
        sp2C = *(s16*)((u8*)temp_r3 + 2) ^ 0x8000;
        sp28 = *(s16*)((u8*)temp_r3 + 4) ^ 0x8000;
        
        f2 = lbl_8047B8B8;
        f0 = lbl_8047B8AC;
        *(f32*)&sp20 = (f32)(sp2C - f2);
        *(f32*)&sp24 = (f32)(sp28 - f2);
        
        temp_r4 = (void*)((u32)lbl_803A204C & 0xFFFF);
        *(f32*)((u8*)temp_r4 + 0) = *(f32*)&sp20;
        *(f32*)((u8*)temp_r4 + 4) = *(f32*)&sp24;
        *(f32*)((u8*)temp_r4 + 8) = f0;
    }
    
    temp_r5 = *(void**)lbl_8047A390;
    sp2C = *(s16*)((u8*)temp_r5 + 2) ^ 0x8000;
    
    f2 = lbl_8047B8B8;
    f4 = *(f32*)((u32)lbl_803A204C & 0xFFFF);
    f0 = lbl_8047B8AC;
    f1 = (f32)(sp2C - f2) - f4;
    *(f32*)&sp20 = f1;
    
    if (f0 == f1) {
        f1 = *(f32*)((u8*)temp_r5 + 4);
        f1 = (f32)((s32)(f1 - f2) - *(s32*)((u32)lbl_803A204C & 0xFFFF));
        *(f32*)&sp24 = f1;
        
        if (f0 == f1) {
            fn_800E0060(&sp24, &sp20);
            
            f0 = lbl_8047B8AC;
            f1 = lbl_8047B8B0;
            *(f32*)&sp20 = f0;
            *(f32*)&sp24 = f1;
            *(f32*)&sp28 = f0;
            
            fn_800E0000(&sp28, &sp20);
            
            if (f1 < f0) {
                lbl_8047A380 = 1;
            } else {
                lbl_8047A380 = 0;
            }
        }
    }
    
    fn_80165A20(0x464, 0, 0xFF);
    return 0;
}
#endif

/* fn_80024160 - 0x80024160 | size: 0x1a8 */
extern u32 lbl_8047A368;
extern u32 lbl_80478DF4;
extern u32 lbl_80478DF0;
#if 1
asm void fn_80024160(void) {
#include "src/game/gs_title_fn_80024160.inc"
}
#else
#pragma optimization_level 4
void fn_80024160(u8* arg0, void* arg1, u16* arg2, u8* arg3) {
    u8 mask;
    u8* current;
    u8* active;
    u8* candidate;
    u8* entry;
    s32 active_index;
    s32 entry_index;
    s32 chain_index;

    (void)arg1;
    mask = 0;
    if ((*arg2 & 0x0001) != 0) {
        mask |= 0x01;
    }
    if ((*arg2 & 0x0002) != 0) {
        mask |= 0x04;
    }
    if ((*arg2 & 0x0004) != 0) {
        mask |= 0x08;
    }
    if ((*arg2 & 0x0008) != 0) {
        mask |= 0x02;
    }

    if (mask != 0) {
        active_index = lbl_8047A368;
        current = fn_8005DA18(*(u32*)(arg0 + 4));
        current = fn_8005D934(*(s16*)(current + 4));
        chain_index = 0;
        while (1) {
            if ((current[0] & 0x80) != 0) {
                if (active_index == chain_index) {
                    active = current;
                    break;
                }
                chain_index++;
            }
            if ((current[0] & 0x40) == 0) {
                current = fn_8005D934(*(s16*)(current + 0x18));
            } else {
                active = 0;
                break;
            }
        }

        entry_index = 0;
        chain_index = 0;
        while ((u32)entry_index < *(u32*)lbl_80478DF0) {
            candidate = fn_8005D934(*(s16*)(arg3 + 4));
            chain_index = 0;
            while (candidate != 0) {
                if ((candidate[0] & 0x80) != 0) {
                    entry = (u8*)(lbl_80478DF4 + chain_index);
                    if ((entry[0] & mask) == mask) {
                        if (fn_8005D934(*(u32*)(entry + 4)) == active) {
                            if (fn_8005D934(*(u32*)(entry + 8)) == candidate) {
                                arg0[0x95] = (u8)chain_index;
                                return;
                            }
                        }
                    }
                    chain_index++;
                }
                if ((candidate[0] & 0x40) == 0) {
                    candidate = fn_8005D934(*(s16*)(candidate + 0x18));
                } else {
                    break;
                }
            }
            entry_index++;
            chain_index += 0x0C;
        }
    }
}
#endif

/* fn_80024308 - 0x80024308 | size: 0x130 */
extern u32 lbl_8047A370;
extern f64 lbl_8047B8D0;
extern f32 lbl_8047A378;
extern f32 lbl_8047A374;
extern u32 lbl_8047B8C0;
extern u32 lbl_8047B8C4;
extern f32 lbl_8047B8C8;
extern f32 lbl_80478898;
extern f32 lbl_8047B8AC;
extern u32 lbl_8047A36C;
extern u32 lbl_8047A368;
#if 1
asm void fn_80024308(void) {
#include "src/game/gs_title_fn_80024308.inc"
}
#else
#pragma optimization_level 4
s32 fn_80024308(u8* arg0) {
    u8* ctx;
    f32 f0;
    f32 f1;
    f32 f2;
    f32 f3;

    ctx = ((u8* (*)(void))fn_80105624)();
    if (arg0 != 0) {
        if ((s32)lbl_8047A370 != 1) {
            if ((*(u16*)(ctx + 4) & 0x10) != 0) {
                arg0[0x98] = 1;
            }
        }
        if ((*(u16*)(ctx + 4) & 0x20) != 0) {
            arg0[0x98] = 1;
            arg0[0x99] = 1;
        }
    }

    f3 = lbl_8047A378;
    f2 = (f32)(u32)fn_800D3088();
    f0 = f3 * f2 + lbl_8047A374;
    lbl_8047A374 = f0;
    f1 = *(f32*)&lbl_8047B8C0;
    if (f0 < f1) {
        lbl_8047A374 = f1;
        lbl_8047A378 = -f3;
    } else {
        f1 = *(f32*)&lbl_8047B8C4;
        if (f0 > f1) {
            lbl_8047A374 = f1;
            lbl_8047A378 = -f3;
        }
    }

    if ((s32)lbl_8047A370 == 1) {
        f1 = lbl_80478898 - lbl_8047B8C8 * (f32)(u32)fn_800D3088();
        lbl_80478898 = f1;
        if (f1 < lbl_8047B8AC) {
            lbl_80478898 = lbl_8047B8AC;
            lbl_8047A370 = 0;
            lbl_8047A368 = lbl_8047A36C;
        }
    }
    return 0;
}
#endif
