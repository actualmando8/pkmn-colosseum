/**
 * @file gs_worldmap.c
 * @brief GSWorldmap -- World map UI and location navigation.
 *
 * Address range: 0x80026000 - 0x80030170 (~90 functions)
 *
 * This module implements the world map interface where the player selects
 * a destination to travel to. It handles:
 *   - Map rendering with location markers
 *   - Cursor movement and location selection
 *   - Location unlock/discovery tracking
 *   - Travel confirmation dialog
 *   - Camera panning and zoom on the map
 *   - Location name and description display
 *   - Transition animation to selected location
 *
 * The world map in Pokemon Colosseum features a 3D map of the Orre region
 * with selectable locations (Phenac City, Pyrite Town, The Under, etc.).
 * Locations are unlocked as the story progresses.
 *
 * Key functions (first cluster: 0x80026000-0x80027740):
 *   fn_80026370  GSmap_GetLocationCount      -- 0x20 bytes
 *   fn_80026390  GSmap_GetUnlockedCount      -- 0x20 bytes
 *   fn_800263B0  GSmap_IsLocationUnlocked    -- 0x6C bytes
 *   fn_8002641C  GSmap_UnlockLocation        -- 0x5C bytes
 *   fn_80026478  GSmap_GetLocationData       -- 0xA4 bytes, get name/position
 *   fn_8002651C  GSmap_GetLocationPosition   -- 0xA4 bytes, get 3D coordinates
 *   fn_800265C0  GSmap_SetCursorPos          -- 0x40 bytes
 *   fn_80026600  GSmap_GetCursorPos          -- 0x40 bytes
 *   fn_80026640  GSmap_GetSelectedLocation   -- 0x40 bytes
 *   fn_80026680  GSmap_SetSelectedLocation   -- 0x40 bytes
 *   fn_800266C0  GSmap_GetTravelState        -- 0x40 bytes
 *   fn_80026700  GSmap_SetTravelState        -- 0x40 bytes
 *   fn_80026740  GSmap_MoveCursor            -- 0x90 bytes, cursor with bounds
 *   fn_800267D0  GSmap_AnimateCursor         -- 0x90 bytes, cursor bob animation
 *   fn_80026860  GSmap_AnimateMarker         -- 0x90 bytes, location marker pulse
 *   fn_800268F0  GSmap_DrawLocation0         -- 0x254 bytes, draw location icon type 0
 *   fn_80026B44  GSmap_DrawLocation1         -- 0x254 bytes, draw location icon type 1
 *   fn_80026D98  GSmap_DrawLocation2         -- 0x254 bytes, draw location icon type 2
 *   fn_80026FEC  GSmap_DrawRoute0            -- 0x190 bytes, draw route line type 0
 *   fn_8002717C  GSmap_DrawRoute1            -- 0x190 bytes, draw route line type 1
 *   fn_8002730C  GSmap_DrawRoute2            -- 0x190 bytes, draw route line type 2
 *   fn_8002749C  GSmap_DrawMapBackground     -- 0x158 bytes, terrain/map texture
 *   fn_800275F4  GSmap_DrawLocationName      -- 0x14C bytes, text overlay
 *   fn_80027740  GSmap_GetRouteCount         -- 0x3C bytes
 *   fn_8002777C  GSmap_GetRouteData          -- 0x3C bytes
 *   fn_800277B8  GSmap_IsRouteUnlocked       -- 0x3C bytes
 *
 * Key functions (second cluster: 0x800277F4-0x8002A618):
 *   fn_800277F4  GSmap_ProcessInput          -- 0xB0 bytes, input handling
 *   fn_800278A4  GSmap_ConfirmTravel         -- 0xBC bytes, "Travel to X?" dialog
 *   fn_80027960  GSmap_StartTravel           -- 0x144 bytes, begin travel sequence
 *   fn_80027AA4  GSmap_TravelAnimation       -- 0x2B4 bytes, flight/drive animation
 *   fn_80027D58  GSmap_ArrivalSequence       -- 0x3A4 bytes, arrive at destination
 *   fn_800280FC  GSmap_UpdateCamera          -- 0xF4 bytes, map camera control
 *   fn_800281F0  GSmap_Nop                   -- 4 bytes, no-op
 *   fn_800281F4  GSmap_Init                  -- 0x250 bytes, full initialization
 *   fn_80028444  GSmap_DrawCityA             -- 0x50 bytes, specific city renderer
 *   fn_80028494  GSmap_DrawCityB             -- 0x50 bytes
 *   fn_800284E4  GSmap_DrawCityC             -- 0x50 bytes
 *   fn_80028534  GSmap_DrawCityD             -- 0x54 bytes
 *   fn_80028588  GSmap_DrawAllCities         -- 0x98 bytes
 *   fn_80028620  GSmap_DrawLandmarks         -- 0x108 bytes
 *   fn_80028728  GSmap_DrawPaths             -- 0x108 bytes
 *   fn_80028830  GSmap_DrawOverlays          -- 0x118 bytes
 *   fn_80028948  GSmap_MainRenderFrame       -- 0x674 bytes, main render function
 *   fn_80028FBC  GSmap_MainUpdate            -- 0x59C bytes, main update loop
 *   fn_80029558  GSmap_ExitMap               -- 0xE0 bytes, close map UI
 *   fn_80029638  GSmap_GetExitResult         -- 0x28 bytes
 *   fn_80029660  GSmap_DrawDescription       -- 0x100 bytes, location description text
 *   fn_80029760  GSmap_DrawMinimap           -- 0xF0 bytes, minimap overlay
 *   fn_80029850  GSmap_DrawCompass           -- 0x8C bytes, compass indicator
 *
 * Key functions (third cluster: 0x800298DC-0x8002FC58+):
 *   fn_800298DC  GSmap_SceneCallback0        -- 0x1EC bytes, scene-specific callback
 *   fn_80029AC8  GSmap_SceneCallback1        -- 0x1F8 bytes
 *   fn_80029CC0  GSmap_SceneCallback2        -- 0x234 bytes
 *   fn_80029EF4  GSmap_ValidateDestination   -- 0xB8 bytes
 *   fn_80029FAC  GSmap_FormatText0           -- 0x10C bytes, format location string
 *   fn_8002A0B8  GSmap_FormatText1           -- 0x10C bytes (this is also called externally)
 *   fn_8002A1C4  GSmap_FormatText2           -- 0x108 bytes
 *   fn_8002A2CC  GSmap_FormatText3           -- 0x108 bytes
 *   fn_8002A3D4  GSmap_GetFormatCount        -- 0x2C bytes
 *   fn_8002A400  GSmap_BuildLocationList     -- 0x8C bytes
 *   fn_8002A48C  GSmap_SortLocations         -- 0x124 bytes
 *   fn_8002A5B0  GSmap_GetNearestLocation    -- 0x68 bytes
 *   fn_8002A618  GSmap_ComputeRoutes         -- 0x450 bytes, pathfinding
 *   fn_8002AA68  GSmap_GetRouteLength        -- 0x98 bytes
 *   fn_8002AB00  GSmap_IsDirectRoute         -- 0x40 bytes
 *   fn_8002AB40  GSmap_InterpolateRoute      -- 0x178 bytes
 *   fn_8002ACB8  GSmap_DrawRoutePreview      -- 0x18C bytes
 *   fn_8002AE44  GSmap_SetZoom               -- 0x24 bytes
 *   fn_8002AE68  GSmap_GetZoom               -- 0x34 bytes
 *   fn_8002AE9C  GSmap_AnimateZoom           -- 0x5C bytes
 *   fn_8002AEF8  GSmap_CameraLookAt          -- 0x144 bytes
 *   fn_8002B03C  GSmap_GetCameraTarget       -- 0x4C bytes
 *   fn_8002B088  GSmap_SetCameraTarget       -- 0x34 bytes
 *   fn_8002B0BC  GSmap_CameraPan             -- 0x78 bytes
 *   fn_8002B134  GSmap_CameraRotate          -- 0x6C bytes
 *   fn_8002B1A0  GSmap_DrawWeatherOverlay    -- 0x26C bytes
 *   fn_8002B40C  GSmap_DrawTimeOverlay       -- 0x188 bytes
 *   fn_8002B594  GSmap_DrawPartyIcons        -- 0x2EC bytes
 *   fn_8002B880  GSmap_DrawInfoPanel         -- 0x468 bytes
 *   fn_8002BCE8  GSmap_DrawTravelProgress    -- 0x120 bytes
 *   fn_8002BE08  GSmap_DrawTransition        -- 0x20C bytes
 *   fn_8002C014  GSmap_FadeToBlack           -- 0xD0 bytes
 *   fn_8002C0E4  GSmap_FadeFromBlack         -- 0x1A0 bytes
 *   fn_8002C284  GSmap_ShowTravelDialog      -- 0x184 bytes
 *   fn_8002C408  GSmap_DialogStateMachine    -- 0xA64 bytes, dialog handler
 *   fn_8002CE6C  GSmap_ProcessChoice         -- 0x2E8 bytes
 *   fn_8002D154  GSmap_ConfirmSequence       -- 0x480 bytes
 *   fn_8002D5D4  GSmap_CancelTravel         -- 0x348 bytes
 *   fn_8002D91C  GSmap_ArrivalDialog         -- 0x350 bytes
 *   fn_8002DC6C  GSmap_SetStoryFlag          -- 0xB8 bytes
 *   fn_8002DD24  GSmap_CheckStoryState       -- 0x1EC bytes
 *   fn_8002DF10  GSmap_UpdateAvailability    -- 0x35C bytes
 *   fn_8002E26C  GSmap_RefreshDisplay        -- 0x1F4 bytes
 *   fn_8002E460  GSmap_DrawFullFrame         -- 0x5FC bytes
 *   fn_8002EA5C  GSmap_HandleSceneChange     -- 0x418 bytes
 *   fn_8002EE74  GSmap_TransitionToScene     -- 0x410 bytes
 *   fn_8002F284  GSmap_LoadDestination       -- 0x518 bytes
 *   fn_8002F79C  GSmap_PrepareArrival        -- 0x4BC bytes
 *   fn_8002FC58  GSmap_FinalizeTransition    -- 0x518 bytes
 *
 * SDA globals:
 *   lbl_8047A360-A3A0: Map state variables (cursor pos, zoom, camera, etc.)
 *   lbl_80478DD8: Location data table pointer
 *   lbl_80478898: Camera float parameter
 *
 * BSS globals:
 *   lbl_803A2058: Map camera position vector
 *   lbl_803A204C: Map camera target vector
 *
 * Sdata2 float constants:
 *   lbl_8047B8B8: 0x4330000080000000 (int-to-float conversion constant)
 *   lbl_8047B8E0: Map viewport center X (float)
 *   lbl_8047B9D4: Map scale factor (float)
 *   lbl_8047B9F0: Map Y offset (float)
 */

#include "dolphin/types.h"

/* ===== Phase 2 recovery stubs ===== */

/* fn_80026370 - 0x80026370 | size: 0x20 */
#if 0
asm void fn_80026370(void) {
#include "src/game/gs_worldmap_fn_80026370.inc"
}
#else
#pragma optimization_level 4
void fn_80026370(void* r3, u8* r4) {
    r4[0x64] = 0;
    r4[0x65] = 0x35;
    r4[0x66] = 0x3c;
}
#endif

/* fn_80026390 - 0x80026390 | size: 0x20 */
#if 0
asm void fn_80026390(void) {
#include "src/game/gs_worldmap_fn_80026390.inc"
}
#else
#pragma optimization_level 4
void fn_80026390(void* r3, u8* r4) {
    r4[0x64] = 0;
    r4[0x65] = 0x35;
    r4[0x66] = 0x3c;
}
#endif

/* fn_800263B0 - 0x800263B0 | size: 0x6c */
extern u8 lbl_80266E18[];
#if 0
asm void fn_800263B0(void) {
#include "src/game/gs_worldmap_fn_800263B0.inc"
}
#else
#pragma optimization_level 4
s32 fn_800263B0(void* r3, u8* r4) {
    void* ctx;
    s32 idx;
    u8 r0, r5, r6;
    ctx = *(void**)((u8*)(*(void**)((u8*)r3 + 0x60)) + 0x24);
    idx = *(s32*)ctx + 1;
    if (idx >= 2) idx -= 2;
    if (idx < 0 || idx >= 2) {
        r0 = 0xff; r5 = 0xff; r6 = 0xff;
    } else {
        r0 = lbl_80266E18[idx * 0x18 + 0];
        r5 = lbl_80266E18[idx * 0x18 + 1];
        r6 = lbl_80266E18[idx * 0x18 + 2];
    }
    r4[0x64] = r0;
    r4[0x65] = r5;
    r4[0x66] = r6;
    return 0;
}
#endif

/* fn_8002641C - 0x8002641C | size: 0x5c */
#pragma scheduling off
#if 0
asm void fn_8002641C(void) {
#include "src/game/gs_worldmap_fn_8002641C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002641C(void* r3, u8* r4) {
    void* ctx;
    s32 idx;
    u8 r0, r5, r6;
    ctx = *(void**)((u8*)(*(void**)((u8*)r3 + 0x60)) + 0x24);
    idx = *(s32*)ctx;
    if (idx < 0 || idx >= 2) {
        r0 = 0xff; r5 = 0xff; r6 = 0xff;
    } else {
        r0 = lbl_80266E18[idx * 0x18 + 0];
        r5 = lbl_80266E18[idx * 0x18 + 1];
        r6 = lbl_80266E18[idx * 0x18 + 2];
    }
    r4[0x64] = r0;
    r4[0x65] = r5;
    r4[0x66] = r6;
    return 0;
}
#endif

/* fn_80026478 - 0x80026478 | size: 0xa4 */
extern void* fn_8012A5B0(s32, s32, u32);
extern u8 fn_80123FBC(void);
extern u8 fn_801231A4(void*);
#if 0
asm void fn_80026478(void) {
#include "src/game/gs_worldmap_fn_80026478.inc"
}
#else
#pragma optimization_level 4
s32 fn_80026478(void* r3, u8* r4) {
    void* ctx;
    void* r31;
    u8 r30;
    ctx = *(void**)((u8*)r3 + 0x60);
    r30 = 0;
    if (*(s32*)((u8*)ctx + 0x1c) != 2) {
        r4[0x67] = 0;
        return 0;
    }
    r31 = fn_8012A5B0(0, 3, (u16)*(u32*)((u8*)ctx + 0x20));
    if ((fn_80123FBC() & 0xff) != 0) {
        if ((fn_801231A4(r31) & 0xff) == 1) {
            r30 = 0xff;
        }
    }
    r4[0x67] = r30;
    return 0;
}
#endif

/* fn_8002651C - 0x8002651C | size: 0xa4 */
#if 0
asm void fn_8002651C(void) {
#include "src/game/gs_worldmap_fn_8002651C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002651C(void* r3, u8* r4) {
    void* ctx;
    void* r31;
    u8 r30;
    ctx = *(void**)((u8*)r3 + 0x60);
    r30 = 0;
    if (*(s32*)((u8*)ctx + 0x1c) != 2) {
        r4[0x67] = 0;
        return 0;
    }
    r31 = fn_8012A5B0(0, 3, (u16)*(u32*)((u8*)ctx + 0x20));
    if ((fn_80123FBC() & 0xff) != 0) {
        if ((fn_801231A4(r31) & 0xff) == 0) {
            r30 = 0xff;
        }
    }
    r4[0x67] = r30;
    return 0;
}
#endif

/* fn_800265C0 - 0x800265C0 | size: 0x40 */
extern u8 lbl_80266DD8[];
#if 0
asm void fn_800265C0(void) {
#include "src/game/gs_worldmap_fn_800265C0.inc"
}
#else
#pragma optimization_level 4
s32 fn_800265C0(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 7) ? 0xff : 0;
    return 0;
}
#endif

/* fn_80026600 - 0x80026600 | size: 0x40 */
#if 0
asm void fn_80026600(void) {
#include "src/game/gs_worldmap_fn_80026600.inc"
}
#else
#pragma optimization_level 4
s32 fn_80026600(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 8) ? 0xff : 0;
    return 0;
}
#endif

/* fn_80026640 - 0x80026640 | size: 0x40 */
#if 0
asm void fn_80026640(void) {
#include "src/game/gs_worldmap_fn_80026640.inc"
}
#else
#pragma optimization_level 4
s32 fn_80026640(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 0xa) ? 0xff : 0;
    return 0;
}
#endif

/* fn_80026680 - 0x80026680 | size: 0x40 */
#if 0
asm void fn_80026680(void) {
#include "src/game/gs_worldmap_fn_80026680.inc"
}
#else
#pragma optimization_level 4
s32 fn_80026680(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 7) ? 0xff : 0;
    return 0;
}
#endif

/* fn_800266C0 - 0x800266C0 | size: 0x40 */
#if 0
asm void fn_800266C0(void) {
#include "src/game/gs_worldmap_fn_800266C0.inc"
}
#else
#pragma optimization_level 4
s32 fn_800266C0(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 8) ? 0xff : 0;
    return 0;
}
#endif

/* fn_80026700 - 0x80026700 | size: 0x40 */
#if 0
asm void fn_80026700(void) {
#include "src/game/gs_worldmap_fn_80026700.inc"
}
#else
#pragma optimization_level 4
s32 fn_80026700(void* r3, u8* r4) {
    void* ctx;
    u32* entry;
    ctx = *(void**)((u8*)r3 + 0x60);
    entry = (u32*)(lbl_80266DD8 + (*(s32*)((u8*)ctx + 0x1c) << 4));
    r4[0x67] = (entry[1] == 0xa) ? 0xff : 0;
    return 0;
}
#endif

/* fn_80026740 - 0x80026740 | size: 0x90 */
extern f32 lbl_8047B938;
extern f32 lbl_8047B934;
#if 1
asm void fn_80026740(void) {
#include "src/game/gs_worldmap_fn_80026740.inc"
}
#else
void fn_80026740(void) { /* TODO */ }
#endif

/* fn_800267D0 - 0x800267D0 | size: 0x90 */
extern f32 lbl_8047B938;
extern f32 lbl_8047B934;
#if 1
asm void fn_800267D0(void) {
#include "src/game/gs_worldmap_fn_800267D0.inc"
}
#else
void fn_800267D0(void) { /* TODO */ }
#endif

/* fn_80026860 - 0x80026860 | size: 0x90 */
extern f32 lbl_8047B938;
extern f32 lbl_8047B934;
#if 1
asm void fn_80026860(void) {
#include "src/game/gs_worldmap_fn_80026860.inc"
}
#else
void fn_80026860(void) { /* TODO */ }
#endif

/* fn_800268F0 - 0x800268F0 | size: 0x254 */
extern void fn_80132A38(s32, void*);
extern u32 fn_800FA444(u32);
extern void fn_800FB680(s32, s32, s32, u32);
extern s32 fn_800FA314(void*);
extern void* fn_800FA280(u32);
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_800268F0(void) {
#include "src/game/gs_worldmap_fn_800268F0.inc"
}
#else
void fn_800268F0(void) { /* TODO */ }
#endif

/* fn_80026B44 - 0x80026B44 | size: 0x254 */
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_80026B44(void) {
#include "src/game/gs_worldmap_fn_80026B44.inc"
}
#else
void fn_80026B44(void) { /* TODO */ }
#endif

/* fn_80026D98 - 0x80026D98 | size: 0x254 */
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_80026D98(void) {
#include "src/game/gs_worldmap_fn_80026D98.inc"
}
#else
void fn_80026D98(void) { /* TODO */ }
#endif

/* fn_80026FEC - 0x80026FEC | size: 0x190 */
extern u32 lbl_8047B928;
extern u32 lbl_8047B92C;
extern u8 lbl_802EF0A8[];
extern f64 lbl_8047B948;
extern f32 lbl_8047B93C;
extern f32 lbl_8047B940;
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_80026FEC(void) {
#include "src/game/gs_worldmap_fn_80026FEC.inc"
}
#else
void fn_80026FEC(void) { /* TODO */ }
#endif

/* fn_8002717C - 0x8002717C | size: 0x190 */
extern u32 lbl_8047B928;
extern u32 lbl_8047B92C;
extern f64 lbl_8047B948;
extern f32 lbl_8047B93C;
extern f32 lbl_8047B940;
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_8002717C(void) {
#include "src/game/gs_worldmap_fn_8002717C.inc"
}
#else
void fn_8002717C(void) { /* TODO */ }
#endif

/* fn_8002730C - 0x8002730C | size: 0x190 */
extern u32 lbl_8047B928;
extern u32 lbl_8047B92C;
extern f64 lbl_8047B948;
extern f32 lbl_8047B93C;
extern f32 lbl_8047B940;
extern f32 lbl_8047B934;
extern f32 lbl_8047B938;
#if 1
asm void fn_8002730C(void) {
#include "src/game/gs_worldmap_fn_8002730C.inc"
}
#else
void fn_8002730C(void) { /* TODO */ }
#endif

/* fn_8002749C - 0x8002749C | size: 0x158 */
extern f64 lbl_8047B948;
extern f32 lbl_8047B93C;
extern f32 lbl_8047B934;
extern f32 lbl_8047B940;
extern f32 lbl_8047B938;
#if 1
asm void fn_8002749C(void) {
#include "src/game/gs_worldmap_fn_8002749C.inc"
}
#else
void fn_8002749C(void) { /* TODO */ }
#endif

/* fn_800275F4 - 0x800275F4 | size: 0x14c */
#if 0
asm void fn_800275F4(void) {
#include "src/game/gs_worldmap_fn_800275F4.inc"
}
#else
#pragma optimization_level 4
s32 fn_800275F4(void* r3) {
    void* r22;
    u8* ctx;
    u8* r27;
    void* r29;
    s32 r23;
    s32 r24;
    s32 r25;
    s32 r28;
    s32 r30;
    s32 r31;
    void* r21;
    u8* arr;
    u16 buf[2];
    u16 r6;
    s32 r0;
    r22 = r3;
    ctx = *(u8**)((u8*)r22 + 0x60);
    r23 = *(s32*)(*(u8**)((u8*)ctx + 0x24));
    r24 = 0;
    r28 = 0;
    r27 = lbl_80266E18 + r23 * 0x18;
    while (r24 < 4) {
        r31 = 0;
        r29 = (void*)(r27 + 8);
        r30 = 0;
        r25 = 0;
        for (;;) {
            if (r23 < 0 || r23 >= 2) { r6 = 0; break; }
            if (r24 < 0 || r24 >= 4) { r6 = 0; break; }
            r21 = *(void**)r29;
            r0 = fn_800FA314(r21);
            if (r25 < 0 || r25 >= r0) { r6 = 0; break; }
            arr = (u8*)fn_800FA280((u32)r21); r6 = *(u16*)(arr + r30);
            if (r6 == 0) { break; }
            buf[0] = r6; buf[1] = 0;
            fn_80132A38(0x37, buf);
            r0 = (s32)(s16)(u16)((u32)fn_800FA444(0xce) >> 16);
            r0 = (0x1b - r0);
            r0 = (r0 + (r0 >> 31)) >> 1;
            fn_800FB680(r31 + r0, r28, (s32)((u8*)r22)[0x8b] | (s32)(-0x100), 0xce);
            r31 += 0x1b;
            r30 += 2;
            r25++;
        }
        r28 += 0x23;
        r27 += 4;
        r24++;
    }
    return 0;
}
#endif

/* fn_8002777C - 0x8002777C | size: 0x3c */
#if 0
asm void fn_8002777C(void) {
#include "src/game/gs_worldmap_fn_8002777C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002777C(void* r3) {
    fn_800FB680(0, 0, (s32)(((u8*)r3)[0x8b]) | (s32)(-0x100), 0x2ef3);
    return 0;
}
#endif

/* fn_800277B8 - 0x800277B8 | size: 0x3c */
#if 0
asm void fn_800277B8(void) {
#include "src/game/gs_worldmap_fn_800277B8.inc"
}
#else
#pragma optimization_level 4
s32 fn_800277B8(void* r3) {
    fn_800FB680(0, 0, (s32)(((u8*)r3)[0x8b]) | (s32)(-0x100), 0x2ef4);
    return 0;
}
#endif

/* fn_800278A4 - 0x800278A4 | size: 0xbc */
extern s32 fn_8011F5C8(void*);
#if 0
asm void fn_800278A4(void) {
#include "src/game/gs_worldmap_fn_800278A4.inc"
}
#else
#pragma optimization_level 4
s32 fn_800278A4(void* r3) {
    u8* r29;
    void* r31;
    void* r30;
    s32 r4;
    u32* entry;
    r29 = (u8*)r3;
    r31 = *(void**)((u8*)r3 + 0x60);
    if (*(s32*)r31 == 2) {
        r30 = fn_8012A5B0(0, 3, (u16)*(u32*)((u8*)r31 + 0x4));
        if ((fn_80123FBC() & 0xff) == 0) {
            r4 = (u16)fn_8011F5C8(r30);
        } else {
            r4 = 1;
        }
        fn_80132A38(0x4e, (void*)(u32)(u16)r4);
    }
    entry = (u32*)(lbl_80266DD8 + (*(s32*)r31 << 4));
    fn_800FB680(0, 0, (s32)r29[0x8b] | (s32)(-0x100), entry[0]);
    return 0;
}
#endif

/* fn_80027960 - 0x80027960 | size: 0x144 */
extern u8 lbl_8047B920[];
#if 1
asm void fn_80027960(void) {
#include "src/game/gs_worldmap_fn_80027960.inc"
}
#else
u16 fn_80027960_TODO(u16 r26, s32 r27) {
    u32* r29;
    u16* r5;
    s32 r28, r30, r31, r4;
    r28 = 0;
    r31 = 0;
    do {
        u32 ptr;
        r29 = (u32*)lbl_8047B920;
        ptr = r29[r28];
        if (ptr == 0) goto next;
        r30 = fn_800FA314((void*)ptr);
        r5 = (u16*)((u8*)fn_800FA280(r29[r28]) + 2);
        r4 = 1;
        if (r30 > 1) {
            s32 ctr = r30 >> 1;
            do {
                if (*r5 == r26) break;
                r4 += 2;
                r5 += 2;
            } while (--ctr);
        }
        if (r4 < r30) goto found;
    next:
        r28++;
        r31 += 4;
    } while (r28 < 2);

found:
    if (r28 >= 2) r28 = 0;
    if (r28 == r27) return r26;
    if (r28 == 1) {
        r26 = *(r5 - 1);
    }
    if (r27 == 0) return r26;
    r30 = r27 << 2;
    r29 = (u32*)lbl_8047B920;
    r31 = fn_800FA314((void*)r29[r27]);
    r5 = (u16*)fn_800FA280(r29[r27]);
    r4 = 0;
    if (r31 > 0) {
        s32 ctr = (r31 + 1) >> 1;
        do {
            if (*r5 == r26) break;
            r4 += 2;
            r5 += 2;
        } while (--ctr);
    }
    if (r4 < r31) return *(r5 + 1);
    return 0;
}
#endif

/* fn_80027AA4 - 0x80027AA4 | size: 0x2b4 */
extern void fn_80166A28(void);
#if 1
asm void fn_80027AA4(void) {
#include "src/game/gs_worldmap_fn_80027AA4.inc"
}
#else
void fn_80027AA4(void) { /* TODO */ }
#endif

/* fn_80027D58 - 0x80027D58 | size: 0x3a4 */
extern u16* fn_80105624(void);
#if 1
asm void fn_80027D58(void) {
#include "src/game/gs_worldmap_fn_80027D58.inc"
}
#else
void fn_80027D58(void) { /* TODO */ }
#endif

/* fn_800280FC - 0x800280FC | size: 0xf4 */
extern void fn_801080CC(void*, s32);
extern f32 lbl_8047B930;
extern f32 lbl_8047B950;
extern f32 lbl_8047B934;
#if 0
asm void fn_800280FC(void) {
#include "src/game/gs_worldmap_fn_800280FC.inc"
}
#else
#pragma optimization_level 4
#pragma peephole off
s32 fn_800280FC(void* r3) {
    u8* r30;
    u8* r31;
    f32* fptr;
    f32 f0;
    f32 f1;
    f32 f2;
    s32 state;
    s32 flag;
    u8 one;
    u8 stateByte;

    r30 = (u8*)r3;
    stateByte = *(volatile u8*)(r30 + 1);
    r31 = *(u8**)(r30 + 0x60);
    state = (s32)(s8)stateByte;
    switch (state) {
    case 0:
        flag = (s32)(s8)*(volatile u8*)(r30 + 2);
        if (flag == 0) {
            fn_801080CC(*(void**)(r30 + 4), 0x56);
            one = 1;
            **(f32**)(r31 + 0x30) = lbl_8047B930;
            r30[2] = one;
        }
        break;
    case 2:
        fptr = *(f32**)(r31 + 0x30);
        f0 = lbl_8047B950;
        f2 = *fptr;
        f1 = lbl_8047B934;
        f0 = f2 + f0;
        *fptr = f0;
        if (f0 >= f1) {
            fptr = *(f32**)(r31 + 0x30);
            *fptr = *fptr - f1;
        }
        break;
    case 3:
        flag = (s32)(s8)*(volatile u8*)(r30 + 2);
        if (flag == 0) {
            fn_801080CC(*(void**)(r30 + 4), 0x5a);
            r30[2] = 1;
        }
        break;
    }
    return 0;
}
#endif

/* fn_800281F0 - 0x800281F0 | size: 0x4 */
#if 0
asm void fn_800281F0(void) {
#include "src/game/gs_worldmap_fn_800281F0.inc"
}
#else
#pragma optimization_level 4
void fn_800281F0(void) { }
#endif

/* fn_800281F4 - 0x800281F4 | size: 0x250 */
extern void fn_800F9E70(void*, u8*);
extern void fn_801337A8(void);
extern void fn_801046B8(void);
extern void fn_801026A4(void);
extern void fn_80106D3C(void);
extern void fn_8001E074(void);
extern void fn_801069FC(s32);
extern void fn_80102510(void);
extern void fn_80102428(void);
extern f32 lbl_8047B930;
extern u8 lbl_8047A3D4[];
extern u8 lbl_8047A3D0[];
extern u8 lbl_8047A3CC[];
extern u8 lbl_8047A3C8[];
extern u8 lbl_8047A3C4[];
extern u8 lbl_8047A3C0[];
extern u8 lbl_8047A3BC[];
extern u8 lbl_8047A3B8[];
extern u8 lbl_8047A3B4[];
extern u8 lbl_8047A3B0[];
extern u8 lbl_803A2068[];
#if 1
asm void fn_800281F4(void) {
#include "src/game/gs_worldmap_fn_800281F4.inc"
}
#else
void fn_800281F4(void) { /* TODO */ }
#endif

/* fn_80028444 - 0x80028444 | size: 0x50 */
#if 0
asm void fn_80028444(void) {
#include "src/game/gs_worldmap_fn_80028444.inc"
}
#else
#pragma optimization_level 4
s32 fn_80028444(void* r3, u8* r4) {
    u8* r31;
    void* ctx;
    void* sub;
    void* p;
    r31 = r4;
    ctx = *(void**)((u8*)r3 + 0x60);
    sub = *(void**)ctx;
    p = *(void**)((u8*)sub + 0x8);
    fn_80132A38(0x37, fn_800FA280((u32)p));
    *(u32*)(r31 + 0x4c) = 0xcf;
    return 0;
}
#endif

/* fn_80028494 - 0x80028494 | size: 0x50 */
#if 0
asm void fn_80028494(void) {
#include "src/game/gs_worldmap_fn_80028494.inc"
}
#else
#pragma optimization_level 4
s32 fn_80028494(void* r3, u8* r4) {
    u8* r31;
    void* ctx;
    void* sub;
    void* p;
    r31 = r4;
    ctx = *(void**)((u8*)r3 + 0x60);
    sub = *(void**)ctx;
    p = *(void**)((u8*)sub + 0x4);
    fn_80132A38(0x37, fn_800FA280((u32)p));
    *(u32*)(r31 + 0x4c) = 0xcf;
    return 0;
}
#endif

/* fn_800284E4 - 0x800284E4 | size: 0x50 */
#if 0
asm void fn_800284E4(void) {
#include "src/game/gs_worldmap_fn_800284E4.inc"
}
#else
#pragma optimization_level 4
s32 fn_800284E4(void* r3, u8* r4) {
    u8* r31;
    void* ctx;
    void* sub;
    void* p;
    r31 = r4;
    ctx = *(void**)((u8*)r3 + 0x60);
    sub = *(void**)ctx;
    p = *(void**)sub;
    fn_80132A38(0x37, fn_800FA280((u32)p));
    *(u32*)(r31 + 0x4c) = 0xcf;
    return 0;
}
#endif

/* fn_80028534 - 0x80028534 | size: 0x54 */
#if 0
asm void fn_80028534(void) {
#include "src/game/gs_worldmap_fn_80028534.inc"
}
#else
#pragma optimization_level 4
void fn_80028534(void* r3) {
    u8* r31;
    u16* pad;
    r31 = (u8*)r3;
    pad = fn_80105624();
    if (!(pad[0] & 0x20)) {
        if (pad[2] & 0x10) {
            r31[0x98] = 1;
        }
    }
}
#endif

/* fn_80028588 - 0x80028588 | size: 0x98 */
#if 0
asm void fn_80028588(void) {
#include "src/game/gs_worldmap_fn_80028588.inc"
}
#else
#pragma optimization_level 4
s32 fn_80028588(void* r3) {
    u8* r31;
    s8 state;
    r31 = (u8*)r3;
    state = (s8)r31[1];
    if (state == 0) {
        if ((s8)r31[2] == 0) {
            fn_801080CC(*(void**)(r31 + 4), 0x56);
            r31[2] = 1;
        }
    } else if (state == 3) {
        if ((s8)r31[2] == 0) {
            fn_801080CC(*(void**)(r31 + 4), 0x5a);
            r31[2] = 1;
        }
    }
    return 0;
}
#endif

/* fn_80028620 - 0x80028620 | size: 0x108 */
extern void* fn_80109934(void*);
extern void fn_800D888C(s32);
extern void fn_800D88DC(s32);
extern void fn_800D7820(void*);
extern void fn_800D85D4(s32, void*);
extern void fn_800D6A00(s32);
extern void fn_800D67BC(s32);
extern void fn_800D61E4(s32, s32);
extern void fn_800D5CB8(s32, s32, s32, s32, s32);
extern void fn_800D59B8(s32, f32, f32);
extern void fn_800D6728(void);
extern u8 lbl_803A2094[];
extern u8 lbl_80314F98[];
extern f32 lbl_8047B930;
extern f32 lbl_8047B934;
#if 0
asm void fn_80028620(void) {
#include "src/game/gs_worldmap_fn_80028620.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
s32 fn_80028620(void* r3, u8* r4) {
    void* r31;
    u8* r30;
    r30 = r4;
    r3 = *(void**)((u8*)r3 + 0x60);
    if (*(s32*)r3 == 2) {
        return 0;
    }
    r31 = fn_80109934(lbl_803A2094);
    if (r31 != (void*)0) {
        fn_800D888C(4);
        fn_800D88DC(3);
        fn_800D7820(lbl_80314F98);
        fn_800D85D4(0, r31);
        fn_800D6A00(7);
        fn_800D67BC(2);
        fn_800D61E4(0, 0);
        fn_800D5CB8(0, 0xff, 0xff, 0xff, 0xff);
        fn_800D59B8(0, lbl_8047B930, lbl_8047B930);
        fn_800D61E4((s32)*(s16*)(r30 + 0x54), (s32)*(s16*)(r30 + 0x56));
        fn_800D5CB8(0, 0xff, 0xff, 0xff, 0xff);
        fn_800D59B8(0, lbl_8047B934, lbl_8047B934);
        fn_800D6728();
    }
    return 0;
}
#pragma peephole on
#endif

/* fn_80028728 - 0x80028728 | size: 0x108 */
extern f32 lbl_8047B930;
extern f32 lbl_8047B934;
#if 0
asm void fn_80028728(void) {
#include "src/game/gs_worldmap_fn_80028728.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
s32 fn_80028728(void* r3, u8* r4) {
    void* r31;
    u8* r30;
    r30 = r4;
    r3 = *(void**)((u8*)r3 + 0x60);
    if (*(s32*)r3 != 2) {
        return 0;
    }
    r31 = fn_80109934(lbl_803A2094);
    if (r31 != (void*)0) {
        fn_800D888C(4);
        fn_800D88DC(3);
        fn_800D7820(lbl_80314F98);
        fn_800D85D4(0, r31);
        fn_800D6A00(7);
        fn_800D67BC(2);
        fn_800D61E4(0, 0);
        fn_800D5CB8(0, 0xff, 0xff, 0xff, 0xff);
        fn_800D59B8(0, lbl_8047B930, lbl_8047B930);
        fn_800D61E4((s32)*(s16*)(r30 + 0x54), (s32)*(s16*)(r30 + 0x56));
        fn_800D5CB8(0, 0xff, 0xff, 0xff, 0xff);
        fn_800D59B8(0, lbl_8047B934, lbl_8047B934);
        fn_800D6728();
    }
    return 0;
}
#pragma peephole on
#endif

/* fn_80028830 - 0x80028830 | size: 0x118 */
extern void* fn_8005D858(s32);
extern void fn_80104160(s32, s32, s32, s32, u32, void*, s32, s32);
extern u8 lbl_803A20DC[];
extern f64 lbl_8047B948;
extern f32 lbl_8047B940;
typedef struct WorldMapOverlay {
    s32 active;
    f32 x;
    f32 y;
    f32 scale;
    f32 unused10;
    u32 color;
    u8 alpha;
    u8 pad19[3];
    f32 timer;
    f32 lifetime;
} WorldMapOverlay;
#if 0
asm void fn_80028830(void) {
#include "src/game/gs_worldmap_fn_80028830.inc"
}
#else
#pragma push
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole off
#pragma fp_contract on
s32 fn_80028830(void* r3) {
    void* r27;
    register s32 r29;
    register s32 r28;
    register WorldMapOverlay* r31;
    register s32 r30;
    f32 sx;
    f32 sy;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    r27 = r3;
    r29 = *(s16*)((u8*)fn_8005D858(0x98) + 0xc);
    r28 = *(s16*)((u8*)fn_8005D858(0x98) + 0xe);
    r31 = (WorldMapOverlay*)lbl_803A20DC;
    r30 = 0;
    while (r30 < 0x1e) {
        if (r31->active != 0) {
            sx = (f32)r29 * r31->scale;
            sy = (f32)r28 * r31->scale;
            x1 = (s32)(lbl_8047B940 + sx);
            x0 = (s32)(lbl_8047B940 + (r31->x - sx * lbl_8047B940));
            y1 = (s32)(lbl_8047B940 + sy);
            y0 = (s32)(lbl_8047B940 + (r31->y - sy * lbl_8047B940));
            fn_80104160(x0, y0, x1, y1, r31->color | r31->alpha, r27, 0x98, 0);
        }
        r31++;
        r30++;
    }
    return 0;
}
#pragma pop
#endif

/* fn_80028948 - 0x80028948 | size: 0x674 */
extern void fn_800E0BE4(void);
extern f32 lbl_8047B958;
extern f32 lbl_8047B95C;
extern f32 lbl_8047B960;
extern f32 lbl_8047B964;
extern f32 lbl_8047B968;
extern f32 lbl_8047B930;
extern f32 lbl_8047B970;
extern f32 lbl_8047B96C;
extern f32 lbl_8047B934;
extern f32 lbl_8047B954;
#if 1
asm void fn_80028948(void) {
#include "src/game/gs_worldmap_fn_80028948.inc"
}
#else
void fn_80028948(void) { /* TODO */ }
#endif

/* fn_80028FBC - 0x80028FBC | size: 0x59c */
extern void fn_8011F4F0(void);
extern void fn_80134A98(void);
extern void fn_8005D934(void);
extern void fn_8010A5BC(void);
extern void fn_8010A010(void);
extern void fn_8018F6F4(void);
extern void fn_8018F4C8(void);
extern void fn_80109894(void);
extern void fn_80109C88(void);
extern void fn_80109B90(void);
extern void fn_801C41C8(void);
extern void fn_801C40F0(void);
extern void fn_8010A420(void);
extern void fn_8012A450(void);
extern void fn_8011DEE4(void);
extern void fn_801349DC(void);
extern void fn_800F9EE4(void);
extern void fn_800FF660(void);
extern void fn_8011288C(s32, u32);
extern u32 lbl_804788A0;
extern u8 lbl_80266DC0[];
extern f32 lbl_8047B940;
#if 1
asm void fn_80028FBC(void) {
#include "src/game/gs_worldmap_fn_80028FBC.inc"
}
#else
void fn_80028FBC(void) { /* TODO */ }
#endif

/* fn_80029558 - 0x80029558 | size: 0xe0 */
extern u8 lbl_803A2068[];
#if 0
asm void fn_80029558(void) {
#include "src/game/gs_worldmap_fn_80029558.inc"
}
#else
#pragma optimization_level 4
s32 fn_80029558(s32 r3, s32 r4) {
    s32 r29;
    s32 r30;
    s32 r31;
    u8* ctx;
    r29 = r3;
    r30 = r4;
    r31 = 1;
    if (r29 == 2) {
        fn_8012A5B0(0, 3, (u16)r30);
        if ((fn_80123FBC() & 0xff) == 0) r31 = 0;
    } else if (r29 >= 3) {
        if (r29 < 4) {
            if (r30 < 0 || r30 >= 3) r31 = 0;
        } else {
            r31 = 0;
        }
    } else if (r29 < 0) {
        r31 = 0;
    }
    if (r31 == 0) return 0;
    ctx = lbl_803A2068;
    *(u16*)ctx = 0;
    *(s32*)(ctx + 0x18) = r29;
    *(s32*)(ctx + 0x1c) = r30;
    *(s32*)(ctx + 0x20) = 0;
    *(s32*)(ctx + 0x28) = 0;
    fn_80028FBC();
    return *(s32*)(ctx + 0x20);
}
#endif

/* fn_80029638 - 0x80029638 | size: 0x28 */
#if 0
asm void fn_80029638(void) {
#include "src/game/gs_worldmap_fn_80029638.inc"
}
#else
#pragma optimization_level 4
void fn_80029638(void* r3) {
    fn_800F9E70(r3, lbl_803A2068);
}
#endif

/* fn_80029660 - 0x80029660 | size: 0x100 */
extern void fn_800FF730(s32);
extern void fn_800F0308(void);
extern void fn_8011288C(s32, u32);
#if 0
asm void fn_80029660(void) {
#include "src/game/gs_worldmap_fn_80029660.inc"
}
#else
#pragma optimization_level 4
s32 fn_80029660(s32 r3, s32 r4) {
    s32 r29;
    s32 r30;
    s32 r31;
    u8* ctx;
    r29 = r3;
    r30 = r4;
    r31 = 1;
    if (r29 == 2) {
        fn_8012A5B0(0, 3, (u16)r30);
        if ((fn_80123FBC() & 0xff) == 0) r31 = 0;
    } else if (r29 >= 3) {
        if (r29 < 4) {
            if (r30 < 0 || r30 >= 3) r31 = 0;
        } else {
            r31 = 0;
        }
    } else if (r29 < 0) {
        r31 = 0;
    }
    if (r31 == 0) return 0;
    ctx = lbl_803A2068;
    *(u16*)ctx = 0;
    *(s32*)(ctx + 0x18) = r29;
    *(s32*)(ctx + 0x1c) = r30;
    *(s32*)(ctx + 0x20) = 0;
    *(s32*)(ctx + 0x24) = 0;
    *(s32*)(ctx + 0x28) = 1;
    fn_800FF730(0x390);
    fn_8011288C(0, 0x59600008);
    fn_800F0308();
    return *(s32*)(ctx + 0x20);
}
#endif

/* fn_80029760 - 0x80029760 | size: 0xf0 */
#if 0
asm void fn_80029760(void) {
#include "src/game/gs_worldmap_fn_80029760.inc"
}
#else
#pragma optimization_level 4
s32 fn_80029760(s32 r3, s32 r4) {
    s32 r29;
    s32 r30;
    s32 r31;
    u8* ctx;
    r29 = r3;
    r30 = r4;
    r31 = 1;
    if (r29 == 2) {
        fn_8012A5B0(0, 3, (u16)r30);
        if ((fn_80123FBC() & 0xff) == 0) r31 = 0;
    } else if (r29 >= 3) {
        if (r29 < 4) {
            if (r30 < 0 || r30 >= 3) r31 = 0;
        } else {
            r31 = 0;
        }
    } else if (r29 < 0) {
        r31 = 0;
    }
    if (r31 == 0) return 0;
    ctx = lbl_803A2068;
    *(u16*)ctx = 0;
    *(s32*)(ctx + 0x18) = r29;
    *(s32*)(ctx + 0x1c) = r30;
    *(s32*)(ctx + 0x20) = 0;
    *(s32*)(ctx + 0x24) = 1;
    *(s32*)(ctx + 0x28) = 1;
    fn_800FF730(0x390);
    fn_800F0308();
    return *(s32*)(ctx + 0x20);
}
#endif

/* fn_80029850 - 0x80029850 | size: 0x8c */
extern u16 fn_80143C68(void*);
extern u16 fn_80143C50(void*);
#if 0
asm void fn_80029850(void) {
#include "src/game/gs_worldmap_fn_80029850.inc"
}
#else
#pragma optimization_level 4
u16 fn_80029850(void** r3, u16 r4, u16 r5, u16 r6) {
    void** r26;
    u16 r28;
    u16 r29;
    u16 r30;
    u16 r31;
    u16 r27;
    r26 = r3;
    r30 = r6;
    r29 = r4;
    r28 = 0;
    r27 = 0;
    r31 = r5;
    while (r27 < r29) {
        u16 val = fn_80143C68(r26);
        if (val == r31) {
            u16 cur = fn_80143C50(r26);
            r28 = (u16)(r28 + (u16)(r30 - cur));
        } else if (val == 0) {
            r28 = (u16)(r28 + r30);
        }
        r27++;
        r26++;
    }
    return r28;
}
#endif

/* fn_80029AC8 - 0x80029AC8 | size: 0x1f8 */
extern void fn_80143B80(void*, u16);
extern void fn_80143B70(void*, u16);
#if 0
asm void fn_80029AC8(void) {
#include "src/game/gs_worldmap_fn_80029AC8.inc"
}
#else
#pragma optimization_level 4
void fn_80029AC8(s32 r3, u16 r4, u16 r5, void* r6) {
    s32 r29;
    u16 r30;
    u16 r26;
    s16 r28;
    s16 r27;
    u8* r31;
    u16 r24;
    u16 r25;
    r29 = r3;
    r30 = r4;
    r26 = r5;
    r31 = (u8*)r6;
    if (!r31) return;
    r28 = *(s16*)((u8*)r31 + 0x768);
    if (r28 > -1) {
        r24 = r26;
        r27 = 0;
        while (r27 < r28 && r24) {
            s16 i = r27;
            if (i >= 0 && i < r28) {
                void* slot = (void*)(r31 + ((s32)i << 2));
                u16 v = fn_80143C68(slot);
                if (v == r30 || v == 0) {
                    u16 cur;
                    u16 delta;
                    u16 give;
                    if (v == 0) {
                        fn_80143B80(slot, r30);
                        cur = 0;
                    } else {
                        cur = fn_80143C50(slot);
                    }
                    delta = (u16)(0x3e7 - cur);
                    give = (delta >= r24) ? r24 : delta;
                    fn_80143B70(slot, (u16)(cur + give));
                    r24 = (u16)(r24 - give);
                }
            }
            r27++;
        }
        r26 = r24;
    }
    if (r28 > -1) {
        r25 = r26;
        r27 = 0;
        while (r27 < r28 && r25) {
            s16 i = r27;
            if (i >= 0 && i < r28) {
                void* slot = (void*)((u8*)r31 + 0x3ac + ((s32)i << 2));
                u16 v = fn_80143C68(slot);
                if (v == r30 || v == 0) {
                    u16 cur;
                    u16 delta;
                    u16 give;
                    if (v == 0) {
                        fn_80143B80(slot, r30);
                        cur = 0;
                    } else {
                        cur = fn_80143C50(slot);
                    }
                    delta = (u16)(0x3e7 - cur);
                    give = (delta >= r25) ? r25 : delta;
                    fn_80143B70(slot, (u16)(cur + give));
                    r25 = (u16)(r25 - give);
                }
            }
            r27++;
        }
    }
    *(s32*)(r31 + 0x758) -= r29;
    *(u8*)(r31 + 0x760) = 1;
}
#endif

/* fn_80029CC0 - 0x80029CC0 | size: 0x234 */
extern void fn_80142A88(void*, s32);
extern s32 fn_800849B4(s32, s32, s32, void*);
typedef struct WorldMapEntry {
    u16 id;
    u16 qty;
} WorldMapEntry;
typedef struct WorldMapBuf {
    u32 a;
    u32 b;
    u32 c;
    u16 d;
    u16 count;
    WorldMapEntry items[48];
} WorldMapBuf;
#if 0
asm void fn_80029CC0(void) {
#include "src/game/gs_worldmap_fn_80029CC0.inc"
}
#else
#pragma push
#pragma optimization_level 4
#pragma peephole off
#pragma scheduling on
s32 fn_80029CC0(u8* r30) {
    WorldMapBuf buf;
    s32 i;
    s16 idx;
    u16 cnt;
    u16 id;
    u16 qty;
    void* slot;
    u16 v;
    u16 cur;
    u16 delta;
    u16 give;
    s32 j;
    s16 jj;

    fn_80142A88(r30, 0xeb);
    fn_80142A88(r30 + 0x3ac, 0xeb);
    *(u32*)(r30 + 0x758) = 0;
    *(u32*)(r30 + 0x75c) = 0;
    if (fn_800849B4(0, 0x40, 0, &buf) < 0) {
        return 0;
    }
    for (i = 0; i < buf.count; i++) {
        id = buf.items[i].id;
        if (id == 0) continue;
        qty = buf.items[i].qty;
        idx = (s16)i;
        cnt = buf.count;
        if (idx < -1) continue;
        if (idx >= (s32)cnt) continue;
        if (idx != -1) {
            if (idx >= 0 && idx < (s32)cnt) {
                slot = (void*)(r30 + ((s32)idx << 2));
                v = (u16)fn_80143C68(slot);
                if (v != id && v != 0) continue;
                if (v == 0) {
                    fn_80143B80(slot, id);
                    cur = 0;
                } else {
                    cur = (u16)fn_80143C50(slot);
                }
                delta = (u16)(0x3e7 - cur);
                give = (delta >= qty) ? qty : delta;
                fn_80143B70(slot, (u16)(cur + give));
            }
        } else {
            for (j = 0; j < (s32)cnt && qty != 0; j++) {
                jj = (s16)j;
                qty = (u16)qty;
                if (jj >= 0 && jj < (s32)cnt) {
                    slot = (void*)(r30 + ((s32)jj << 2));
                    v = (u16)fn_80143C68(slot);
                    if (v != id && v != 0) continue;
                    if (v == 0) {
                        fn_80143B80(slot, id);
                        cur = 0;
                    } else {
                        cur = (u16)fn_80143C50(slot);
                    }
                    delta = (u16)(0x3e7 - cur);
                    give = (delta >= qty) ? qty : delta;
                    fn_80143B70(slot, (u16)(cur + give));
                    qty = (u16)(qty - give);
                }
            }
        }
    }
    *(u32*)(r30 + 0x758) = *(u32*)((u8*)&buf + 0);
    *(u32*)(r30 + 0x75c) = *(u32*)((u8*)&buf + 4);
    *(u8*)(r30 + 0x760) = 0;
    *(u32*)(r30 + 0x764) = *(u32*)((u8*)&buf + 8);
    *(u16*)(r30 + 0x768) = buf.count;
    return 1;
}
#pragma pop
#endif

/* fn_80029EF4 - 0x80029EF4 | size: 0xb8 */
extern void fn_80129384(s32, void*);
extern void fn_8013467C(s32, s32, u16);
extern void fn_80129A78(s32, s32, u16, s32);
#if 0
asm void fn_80029EF4(void) {
#include "src/game/gs_worldmap_fn_80029EF4.inc"
}
#else
#pragma optimization_level 4
void fn_80029EF4(void* r3, s32 r4, s32 r5, u8 r6, void* r7) {
    s32 r29, r30;
    void* r31;
    r29 = r4; r30 = r5; r31 = r7;
    if ((u8)r6 == 3) {
        fn_80029AC8((s32)(u32)r3, (u16)r29, (u16)r30, r31);
    } else if ((u8)r6 == 2) {
        fn_80129384(0, r3);
        fn_8013467C(0, r29, (u16)r30);
        if (r31 != 0) { ((u8*)r31)[0x760] = 1; }
    } else {
        fn_80129384(0, r3);
        fn_80129A78(0, r29, (u16)r30, -1);
    }
}
#endif

/* fn_80029FAC - 0x80029FAC | size: 0x10c */
extern void* __va_arg(void*, s32);
extern u32 lbl_80478E54;
extern u32 lbl_80478E4C;
typedef struct WorldMapVaList {
    u8 gpr;
    u8 fpr;
    u16 padding;
    u32* overflow_arg_area;
    u32* reg_save_area;
} WorldMapVaList;
typedef WorldMapVaList WorldMapVaListArray[1];
#if 0
asm void fn_80029FAC(void) {
#include "src/game/gs_worldmap_fn_80029FAC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole off
u32 fn_80029FAC(u8* r3, s32 r4, s32 r5, s32 r6, ...) {
    WorldMapVaListArray list;
    s32 r31;
    s32 r30;
    s32 r29;
    u8* r28;
    u8* map;
    u8* table;
    s32 idx;
    s32 offset;

    *(u32*)list = 0x04000000;
    list[0].overflow_arg_area = (u32*)((u8*)list + 0x30);
    list[0].reg_save_area = (u32*)((u8*)list - 0x60);
    idx = r4 << 2;
    map = (u8*)lbl_80478E54;
    r31 = r5 << 2;
    table = (u8*)lbl_80478E4C;
    r30 = 1;
    offset = map[idx] * 0x4c;
    *r3 = table[offset];
    r28 = table + offset + 4;
    while (r6 >= 0) {
        if (r30 != 0) {
            r29 = r6;
            r30 = 0;
        } else {
            r30 = 1;
            fn_80132A38(r29, (void*)r6);
        }
        r6 = *(s32*)__va_arg(list, 1);
    }
    return *(u32*)(r28 + r31);
}
#endif

/* fn_8002A0B8 - 0x8002A0B8 | size: 0x10c */
extern u32 lbl_80478E54;
extern u32 lbl_80478E3C;
#if 0
asm void fn_8002A0B8(void) {
#include "src/game/gs_worldmap_fn_8002A0B8.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole off
u32 fn_8002A0B8(u8* r3, s32 r4, s32 r5, s32 r6, ...) {
    WorldMapVaListArray list;
    s32 r31;
    s32 r30;
    s32 r29;
    u8* r28;
    u8* map;
    u8* table;
    s32 idx;
    s32 offset;

    *(u32*)list = 0x04000000;
    list[0].overflow_arg_area = (u32*)((u8*)list + 0x30);
    list[0].reg_save_area = (u32*)((u8*)list - 0x60);
    idx = r4 << 2;
    map = (u8*)lbl_80478E54;
    r31 = r5 << 2;
    table = (u8*)lbl_80478E3C;
    r30 = 1;
    offset = map[idx] * 0x3c;
    *r3 = table[offset];
    r28 = table + offset + 4;
    while (r6 >= 0) {
        if (r30 != 0) {
            r29 = r6;
            r30 = 0;
        } else {
            r30 = 1;
            fn_80132A38(r29, (void*)r6);
        }
        r6 = *(s32*)__va_arg(list, 1);
    }
    return *(u32*)(r28 + r31);
}
#endif

/* fn_8002A1C4 - 0x8002A1C4 | size: 0x108 */
extern void fn_80106ADC(s32, u32, s32, s32, u8);
extern u32 lbl_80478E54;
extern u32 lbl_80478E4C;
#if 0
asm void fn_8002A1C4(void) {
#include "src/game/gs_worldmap_fn_8002A1C4.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole off
void fn_8002A1C4(s32 r3, s32 r4, s32 r5, ...) {
    WorldMapVaListArray list;
    s32 r31;
    s32 r30;
    s32 r29;
    u8* r28;
    u8* map;
    s32 idx;
    u8 r27;

    *(u32*)list = 0x03000000;
    list[0].overflow_arg_area = (u32*)((u8*)list + 0x30);
    list[0].reg_save_area = (u32*)((u8*)list - 0x60);
    idx = r3 << 2;
    map = (u8*)lbl_80478E54;
    r31 = r4 << 2;
    r28 = (u8*)lbl_80478E4C + map[idx] * 0x4c;
    r27 = r28[0];
    r28 += 4;
    r30 = 1;
    while (r5 >= 0) {
        if (r30 != 0) {
            r29 = r5;
            r30 = 0;
        } else {
            r30 = 1;
            fn_80132A38(r29, (void*)r5);
        }
        r5 = *(s32*)__va_arg(list, 1);
    }
    fn_80106ADC(2, *(u32*)(r28 + r31), 1, 0, r27);
    fn_801069FC(1);
}
#endif

/* fn_8002A2CC - 0x8002A2CC | size: 0x108 */
extern u32 lbl_80478E54;
extern u32 lbl_80478E3C;
#if 0
asm void fn_8002A2CC(void) {
#include "src/game/gs_worldmap_fn_8002A2CC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole off
void fn_8002A2CC(s32 r3, s32 r4, s32 r5, ...) {
    WorldMapVaListArray list;
    s32 r31;
    s32 r30;
    s32 r29;
    u8* r28;
    u8* map;
    s32 idx;
    u8 r27;

    *(u32*)list = 0x03000000;
    list[0].overflow_arg_area = (u32*)((u8*)list + 0x30);
    list[0].reg_save_area = (u32*)((u8*)list - 0x60);
    idx = r3 << 2;
    map = (u8*)lbl_80478E54;
    r31 = r4 << 2;
    r28 = (u8*)lbl_80478E3C + map[idx] * 0x3c;
    r27 = r28[0];
    r28 += 4;
    r30 = 1;
    while (r5 >= 0) {
        if (r30 != 0) {
            r29 = r5;
            r30 = 0;
        } else {
            r30 = 1;
            fn_80132A38(r29, (void*)r5);
        }
        r5 = *(s32*)__va_arg(list, 1);
    }
    fn_80106ADC(2, *(u32*)(r28 + r31), 1, 0, r27);
    fn_801069FC(1);
}
#endif

/* fn_8002A3D4 - 0x8002A3D4 | size: 0x2c */
#if 0
asm void fn_8002A3D4(void) {
#include "src/game/gs_worldmap_fn_8002A3D4.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002A3D4(void* r3, u8* r4) {
    void* ctx;
    ctx = *(void**)((u8*)r3 + 0x60);
    r4[0x64] = ((u8*)ctx)[0x10];
    r4[0x65] = ((u8*)ctx)[0x11];
    r4[0x66] = ((u8*)ctx)[0x12];
    r4[0x67] = 0xff;
    return 0;
}
#endif

/* fn_8002A400 - 0x8002A400 | size: 0x8c */
#if 0
asm void fn_8002A400(void) {
#include "src/game/gs_worldmap_fn_8002A400.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002A400(void* r3, u8* r4) {
    u8* r30;
    void* r31;
    u32 id;
    u32 ret;
    r30 = r4;
    r31 = *(void**)((u8*)r3 + 0x60);
    fn_80132A38(0x50, (void*)(*(s32*)((u8*)r31 + 0x8) * *(s32*)(*(u32*)((u8*)r31 + 0xc))));
    if (*(u32*)((u8*)r31 + 0x14) != 0) {
        id = 0x153;
    } else {
        id = 0x151;
    }
    ret = fn_800FA444(id);
    fn_800FB680((s32)(s16)*(u16*)(r30 + 0x54) - (s32)(ret >> 16), 0, -1, id);
    return 0;
}
#endif

/* fn_8002A48C - 0x8002A48C | size: 0x124 */
extern void fn_800FB8C8(s32, s32, s16, s16, s32, s32);
extern u8 lbl_80266E58[];
#if 0
asm void fn_8002A48C(void) {
#include "src/game/gs_worldmap_fn_8002A48C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002A48C(void* r3, u8* r4) {
    u8* r31;
    void* r6;
    s32 r5;
    s32 r8;
    s32 r7;
    s32 r4v;
    s32 r0;
    r31 = r4;
    r6 = *(void**)((u8*)r3 + 0x60);
    r5 = 0;
    if (*(s32*)lbl_80266E58 == (s32)(s16)*(u16*)(r31 + 0x6)) {
        r5 = 0;
    } else {
        r5 = 1;
        if (*(s32*)(lbl_80266E58 + 0xc) == (s32)(s16)*(u16*)(r31 + 0x6)) {
            r5 = 1;
        } else {
            r5 = 2;
        }
    }
    if (r5 >= 2) { return 0; }
    r5 = 1 - r5;
    r8 = 1;
    r7 = 0;
    if (r5 > 0) {
        if (r5 > 8) {
            r0 = (r5 - 8 + 7) >> 3;
            if (r5 - 8 > 0) {
                do { r8 = r8 * 100000000; r7 += 8; r0--; } while (r0 != 0);
            }
        }
        r0 = r5 - r7;
        if (r7 < r5) {
            do { r8 = r8 * 10; r0--; } while (r0 != 0);
        }
    }
    r4v = *(s32*)(*(u32*)((u8*)r6 + 0xc));
    r4v = r4v / r8;
    r0 = r4v / 10 * 10;
    r4v = r4v - r0;
    fn_80132A38(0x34, (void*)r4v);
    fn_800FB8C8(0, 0, (s16)*(u16*)(r31 + 0x54), (s16)*(u16*)(r31 + 0x56), -1, 0xc9);
    return 0;
}
#endif

/* fn_8002A5B0 - 0x8002A5B0 | size: 0x68 */
#if 0
asm void fn_8002A5B0(void) {
#include "src/game/gs_worldmap_fn_8002A5B0.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002A5B0(void* r3, u8* r4) {
    s8 idx;
    s32 val;
    idx = (s8)((u8*)r3)[0x95];
    if (idx < 0 || idx >= 2) { return 0; }
    val = (s16)(*(u16*)(r4 + 0x6));
    if (*(s32*)(lbl_80266E58 + (s32)idx * 0xc + 0x4) == val) {
        r4[0x67] = 0xff;
    } else if (*(s32*)(lbl_80266E58 + (s32)idx * 0xc + 0x8) == val) {
        r4[0x67] = 0xff;
    } else {
        r4[0x67] = 0;
    }
    return 0;
}
#endif

/* fn_8002AA68 - 0x8002AA68 | size: 0x98 */
#if 0
asm void fn_8002AA68(void) {
#include "src/game/gs_worldmap_fn_8002AA68.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AA68(void* r3) {
    u8* r31;
    s8 state;
    r31 = (u8*)r3;
    state = (s8)r31[1];
    if (state == 0) {
        if ((s8)r31[2] == 0) {
            fn_801080CC((void*)0x61, 0x7e);
            r31[2] = 1;
        }
    } else if (state == 3) {
        if ((s8)r31[2] == 0) {
            fn_801080CC((void*)0x61, 0x82);
            r31[2] = 1;
        }
    }
    return 0;
}
#endif

/* fn_8002AB00 - 0x8002AB00 | size: 0x40 */
extern u8 lbl_80266E70[];
#if 0
asm void fn_8002AB00(void) {
#include "src/game/gs_worldmap_fn_8002AB00.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AB00(void* r3, u8* r4) {
    void* ctx;
    u8* base;
    u8 v;
    ctx = *(void**)((u8*)r3 + 0x60);
    v = ((u8*)ctx)[0x1c];
    base = lbl_80266E70 + (u32)v * 3;
    r4[0x64] = base[0];
    r4[0x65] = base[1];
    r4[0x66] = base[2];
    r4[0x67] = 0xff;
    return 0;
}
#endif

/* fn_8002AB40 - 0x8002AB40 | size: 0x178 */
extern u8 lbl_80266E80[];
extern u32 lbl_804788F0;
extern u8 lbl_802E61D8[];
#if 1
asm void fn_8002AB40(void) {
#include "src/game/gs_worldmap_fn_8002AB40.inc"
}
#else
void fn_8002AB40(void) { /* TODO */ }
#endif

/* fn_8002ACB8 - 0x8002ACB8 | size: 0x18c */
extern u32 lbl_8047A660;
extern u32 lbl_8047A664;
#if 1
asm void fn_8002ACB8(void) {
#include "src/game/gs_worldmap_fn_8002ACB8.inc"
}
#else
void fn_8002ACB8(void) { /* TODO */ }
#endif

/* fn_8002AE44 - 0x8002AE44 | size: 0x24 */
#if 0
asm void fn_8002AE44(void) {
#include "src/game/gs_worldmap_fn_8002AE44.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AE44(void* r3, u8* r4) {
    void* ctx;
    ctx = *(void**)((u8*)r3 + 0x60);
    if (((u8*)ctx)[0x1d] & 1) {
        r4[0x67] = 0;
    }
    return 0;
}
#endif

/* fn_8002AE68 - 0x8002AE68 | size: 0x34 */
#if 0
asm void fn_8002AE68(void) {
#include "src/game/gs_worldmap_fn_8002AE68.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AE68(void* r3, u8* r4) {
    void* ctx;
    u8 v;
    ctx = *(void**)((u8*)r3 + 0x60);
    v = ((u8*)ctx)[0x1c];
    r4[0x67] = (v == 0 || v == 1) ? 0xcc : 0;
    return 0;
}
#endif

/* fn_8002AE9C - 0x8002AE9C | size: 0x5c */
extern u8 lbl_80266E70[];
#if 0
asm void fn_8002AE9C(void) {
#include "src/game/gs_worldmap_fn_8002AE9C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AE9C(void* r3, u8* r4) {
    void* ctx;
    u8 v;
    u8* base;
    ctx = *(void**)((u8*)r3 + 0x60);
    v = ((u8*)ctx)[0x1c];
    if (v == 0 || v == 1) {
        base = lbl_80266E70 + v * 3;
        r4[0x64] = base[0];
        r4[0x65] = base[1];
        r4[0x66] = base[2];
        r4[0x67] = 0xff;
    } else {
        r4[0x67] = 0;
    }
    return 0;
}
#endif

/* fn_8002AEF8 - 0x8002AEF8 | size: 0x144 */
extern void fn_801440A0(u32);
extern u32 fn_80144014(void);
extern u32 fn_80129BC8(s32, u32, u16*, s32, s32, s32, s32);
extern u32 fn_80142CF4(u32, s32, s32, s32);
#if 0
asm void fn_8002AEF8(void) {
#include "src/game/gs_worldmap_fn_8002AEF8.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002AEF8(void* r3, u8* r4) {
    void* r5;
    u32 r30;
    s32 r29;
    u32 r28;
    u32 r27;
    u32 r31;
    s32 idx;
    u16 stack;
    r5 = *(void**)((u8*)r3 + 0x60);
    if (((u8*)r5)[0x1c] != 0 && ((u8*)r5)[0x1c] != 1) { return 0; }
    idx = (s8)((u8*)r3)[0x95] + (s8)((u8*)r3)[0x94];
    if (idx < 0 || idx >= (s32)*(u32*)((u8*)r5 + 0x8)) {
        r30 = 0;
    } else {
        r30 = (u16)*(u16*)(*(u32*)((u8*)r5 + 0x4) + idx * 2);
    }
    if (r30 == 0) { return 0; }
    r29 = 0;
    r31 = r30;
    fn_801440A0(r30);
    r27 = fn_80129BC8(0, fn_80144014(), &stack, 0, 0, 0, 0);
    r28 = 0;
    while (r28 < (u32)stack) {
        if ((u16)fn_80142CF4(r27, 0, 0x1b, 0) == (u16)r31) {
            r29 += (s32)fn_80142CF4(r27, 0, 0x1c, 0);
        }
        r28++;
        r27 += 4;
    }
    fn_80132A38(0x2d, (void*)(u32)(u16)r30);
    fn_80132A38(0x34, (void*)r29);
    fn_800FB680(0, 0, -1, 0x2b2f);
    return 0;
}
#endif

/* fn_8002B03C - 0x8002B03C | size: 0x4c */
#if 0
asm void fn_8002B03C(void) {
#include "src/game/gs_worldmap_fn_8002B03C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002B03C(void* r3) {
    void* ctx;
    u8 v;
    ctx = *(void**)((u8*)r3 + 0x60);
    v = ((u8*)ctx)[0x1c];
    if (v == 0 || v == 1) {
        fn_800FB680(0, 0, -1, 0x2b2e);
    }
    return 0;
}
#endif

/* fn_8002B088 - 0x8002B088 | size: 0x34 */
extern u32 lbl_8047A3E4;
#if 0
asm void fn_8002B088(void) {
#include "src/game/gs_worldmap_fn_8002B088.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
s32 fn_8002B088(void) {
    fn_800FB680(0, 0, -1, lbl_8047A3E4);
    return 0;
}
#pragma peephole on
#endif

/* fn_8002B0BC - 0x8002B0BC | size: 0x78 */
extern f32 lbl_8047B97C;
extern f32 lbl_8047A3E8;
extern f32 lbl_8047B978;
#if 0
asm void fn_8002B0BC(void) {
#include "src/game/gs_worldmap_fn_8002B0BC.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
s32 fn_8002B0BC(void* r3, u8* r4) {
    u16 hv;
    u8* ctx;
    u8 pad[8];
    hv = *(u16*)((u8*)r3 + 0x94);
    ctx = *(u8**)((u8*)r3 + 0x60);
    *(u16*)pad = hv;
    if ((s8)pad[0] + 0xa < *(s32*)(ctx + 0x8) + 1) {
        if (*(u16*)(*(void**)ctx) == 0) {
            r4[0x67] = (lbl_8047B97C - lbl_8047A3E8) * lbl_8047B978;
            goto end;
        }
    }
    r4[0x67] = 0;
end:
    return 0;
}
#pragma peephole on
#endif

/* fn_8002B134 - 0x8002B134 | size: 0x6c */
#pragma scheduling off
extern f32 lbl_8047B97C;
extern f32 lbl_8047A3E8;
extern f32 lbl_8047B978;
#if 0
asm void fn_8002B134(void) {
#include "src/game/gs_worldmap_fn_8002B134.inc"
}
#else
#pragma peephole off
#pragma optimization_level 4
s32 fn_8002B134(void* r3, u8* r4) {
    u16 hv;
    void* ctx;
    u8 pad[8];
    hv = *(u16*)((u8*)r3 + 0x94);
    ctx = *(void**)((u8*)r3 + 0x60);
    *(u16*)pad = hv;
    if ((s8)pad[0] > 0) {
        if (*(u16*)(*(void**)ctx) == 0) {
            r4[0x67] = (lbl_8047B97C - lbl_8047A3E8) * lbl_8047B978;
            goto end;
        }
    }
    r4[0x67] = 0;
end:
    return 0;
}
#pragma peephole on
#endif

/* fn_8002B1A0 - 0x8002B1A0 | size: 0x26c */
extern void fn_800FE38C(void);
extern void fn_80144088(void);
extern void fn_80143FFC(void);
extern void fn_80143FE4(void);
extern void fn_800FE35C(void);
extern f32 lbl_8047B980;
#if 1
asm void fn_8002B1A0(void) {
#include "src/game/gs_worldmap_fn_8002B1A0.inc"
}
#else
void fn_8002B1A0(void) { /* TODO */ }
#endif

/* fn_8002B40C - 0x8002B40C | size: 0x188 */
extern u8 lbl_802E4F68[];
extern f64 lbl_8047B998;
extern f32 lbl_8047B984;
extern f32 lbl_8047B988;
extern f32 lbl_8047B98C;
extern f32 lbl_8047B990;
#if 1
asm void fn_8002B40C(void) {
#include "src/game/gs_worldmap_fn_8002B40C.inc"
}
#else
void fn_8002B40C(void) { /* TODO */ }
#endif

/* fn_8002B594 - 0x8002B594 | size: 0x2ec */
extern void fn_800CDBE0(void);
extern void fn_800CE148(void);
extern f32 lbl_8047B980;
extern f32 lbl_8047B97C;
extern f64 lbl_8047B998;
extern f32 lbl_8047B98C;
extern f32 lbl_8047B9A0;
extern f32 lbl_8047B9A4;
extern f32 lbl_8047B9A8;
#if 1
asm void fn_8002B594(void) {
#include "src/game/gs_worldmap_fn_8002B594.inc"
}
#else
void fn_8002B594(void) { /* TODO */ }
#endif

/* fn_8002B880 - 0x8002B880 | size: 0x468 */
extern void fn_800FE6D0(void);
extern void fn_800FE4D4(void);
extern f64 lbl_8047B998;
extern f32 lbl_8047B98C;
extern f32 lbl_8047B9A0;
extern f32 lbl_8047B9A4;
extern f32 lbl_8047A3F0;
extern f32 lbl_8047B978;
extern f32 lbl_8047B9AC;
extern f32 lbl_8047B97C;
extern f32 lbl_8047B9B0;
extern f32 lbl_8047B9B4;
#if 1
asm void fn_8002B880(void) {
#include "src/game/gs_worldmap_fn_8002B880.inc"
}
#else
void fn_8002B880(void) { /* TODO */ }
#endif

/* fn_8002BCE8 - 0x8002BCE8 | size: 0x120 */
extern u8 lbl_802E4F68[];
#if 0
asm void fn_8002BCE8(void) {
#include "src/game/gs_worldmap_fn_8002BCE8.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002BCE8(void* r3, u8* r4) {
    u16 sprite_id;
    s16 key;
    s32 r5;
    u8* entry;
    u8* tab;
    u8* ctx;
    s32 idx;
    s8 low_byte;
    sprite_id = *(u16*)((u8*)r3 + 0x94);
    ctx = (u8*)*(void**)((u8*)r3 + 0x60);
    tab = lbl_802E4F68;
    key = *(s16*)(r4 + 0x6);
    idx = 5;
    if (key == *(s32*)(tab + 0x0)) idx = 0;
    else if (key == *(s32*)(tab + 0x8)) idx = 1;
    else if (key == *(s32*)(tab + 0x10)) idx = 2;
    else if (key == *(s32*)(tab + 0x18)) idx = 3;
    else if (key == *(s32*)(tab + 0x20)) idx = 4;
    if (idx >= 5) return 0;
    entry = tab + (u32)idx * 8;
    low_byte = (s8)(sprite_id & 0xff);
    r5 = (s32)*(s16*)(entry + 4) + (s32)low_byte * 0x1f;
    if (*(u32*)(ctx + 0x14) != 0) {
        r5 += (s32)*(f32*)(*(u32*)(ctx + 0xc));
    }
    {
        u16 v = *(u16*)(*(u32*)ctx);
        u8 alpha = (v == 0) ? 0x72 : 0xff;
        *(s16*)(r4 + 0x52) = (s16)r5;
        r4[0x67] = alpha;
    }
    return 0;
}
#endif

/* fn_8002BE08 - 0x8002BE08 | size: 0x20c */
extern void fn_80143F84(void);
extern f32 lbl_8047B9B8;
extern f32 lbl_8047B9BC;
extern u32 lbl_8047A3E4;
#if 0
asm void fn_8002BE08(void) {
#include "src/game/gs_worldmap_fn_8002BE08.inc"
}
#else
#pragma push
#pragma peephole off
#pragma scheduling on
u32 fn_8002BE08(u8* arg0) {
    u8* ctx;
    u16* state;
    s32 sum;
    u32 r3val;
    s32 limit;

    ctx = *(u8**)(arg0 + 0x60);
    state = fn_80105624();
    if (lbl_8047B980 != *(f32*)(*(u32*)(ctx + 0xc))) {
        return 0;
    }
    limit = *(s32*)(ctx + 0x8) + 1;
    if ((state[2] | state[4]) & 0x2) {
        ++arg0[0x95];
        if ((s32)((s8)arg0[0x95] + (s8)arg0[0x94]) >= limit) {
            --arg0[0x95];
        } else {
            if ((s8)arg0[0x95] >= 0xa) {
                ++arg0[0x94];
                --arg0[0x95];
                *(s32*)(*(u32*)(ctx + 0x14)) = 1;
            } else {
                *(s32*)(*(u32*)(ctx + 0x14)) = 0;
            }
            *(f32*)(*(u32*)(ctx + 0xc)) = lbl_8047B9B8;
        }
    }
    if ((state[2] | state[4]) & 0x1) {
        if ((s8)arg0[0x95] > 0 || (s8)arg0[0x94] > 0) {
            s32 t = (s8)(arg0[0x95] - 1);
            arg0[0x95] = (u8)t;
            if (t < 0) {
                arg0[0x95] = 0;
                --arg0[0x94];
                *(s32*)(*(u32*)(ctx + 0x14)) = 1;
            } else {
                *(s32*)(*(u32*)(ctx + 0x14)) = 0;
            }
            *(f32*)(*(u32*)(ctx + 0xc)) = lbl_8047B9BC;
        }
    }
    sum = (s32)(s8)arg0[0x94] + (s32)(s8)arg0[0x95];
    if (sum < 0 || sum >= *(s32*)(ctx + 0x8)) {
        r3val = 0;
    } else {
        r3val = ((u16*)(*(u32*)(ctx + 0x4)))[sum];
    }
    if ((u16)r3val != 0) {
        fn_801440A0((u16)r3val);
        fn_80143F84();
    } else {
        u8 b = ctx[0x1c];
        if (b == 0 || b == 1) {
            r3val = 0x2b2d;
        } else if (ctx[0x1d] & 1) {
            r3val = 0x2b46;
        } else {
            r3val = 0x2b37;
        }
    }
    lbl_8047A3E4 = r3val;
    return 0;
}
#pragma pop
#endif

/* fn_8002C014 - 0x8002C014 | size: 0xd0 */
extern void fn_80102ED4(void*);
#if 0
asm void fn_8002C014(void) {
#include "src/game/gs_worldmap_fn_8002C014.inc"
}
#else
#pragma optimization_level 4
s32 fn_8002C014(void* r3) {
    u8* r31;
    u8* r30;
    u16* pad;
    s32 r4;
    u16 r3val;
    r31 = (u8*)r3;
    r30 = (u8*)*(void**)((u8*)r3 + 0x60);
    pad = fn_80105624();
    if (pad[2] & 0x10) {
        s32 a = (s8)r31[0x94];
        s32 b = (s8)r31[0x95];
        r4 = a + b;
        if (r4 < 0 || r4 >= (s32)*(u32*)(r30 + 0x8)) {
            r3val = 0;
        } else {
            r3val = ((u16*)(*(u32*)(r30 + 0x4)))[r4];
        }
        if ((r30[0x1d] & 1) && (r3val != 0)) {
            return 0;
        }
    } else {
        r3val = 0;
    }
    if (r3val != 0) {
        *(u16*)(*(u32*)r30) = r3val;
        fn_80102ED4(r31);
    }
    return 0;
}
#endif

/* fn_8002C0E4 - 0x8002C0E4 | size: 0x1a0 */
extern f32 lbl_8047B980;
extern f32 lbl_8047B9C0;
extern f32 lbl_8047B9C4;
extern f32 lbl_8047B97C;
extern f32 lbl_8047B9C8;
#if 1
asm void fn_8002C0E4(void) {
#include "src/game/gs_worldmap_fn_8002C0E4.inc"
}
#else
void fn_8002C0E4(void) { /* TODO */ }
#endif

/* fn_8002C284 - 0x8002C284 | size: 0x184 */
extern void fn_80102568(void);
extern u32 lbl_804788A8;
extern u8 lbl_8047A3F8[];
extern u32 lbl_80478E54;
extern u32 lbl_80478E44;
extern u8 lbl_8047A3F4[];
extern u8 lbl_8047A3EC[];
#if 1
asm void fn_8002C284(void) {
#include "src/game/gs_worldmap_fn_8002C284.inc"
}
#else
void fn_8002C284(void) { /* TODO */ }
#endif

/* fn_8002C408 - 0x8002C408 | size: 0xa64 */
extern void fn_80129280(void);
extern void fn_80134420(void);
extern void fn_801298B8(void);
extern void fn_80166AB8(void);
extern void fn_80093574(void);
extern void fn_80092C90(void);
extern void fn_80093610(void);
extern void fn_80093698(void);
extern void fn_801D0748(void);
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 n);
extern u32 lbl_8047A3DC;
extern u32 lbl_8047A3D8;
extern u32 lbl_8047A660;
extern u32 lbl_8047A664;
extern u32 lbl_80478E54;
extern u32 lbl_80478E44;
extern u32 lbl_804788A8;
extern u32 lbl_8047A3E4;
extern u8 lbl_8047A3E0[];
extern u32 lbl_80478E4C;
#if 1
asm void fn_8002C408(void) {
#include "src/game/gs_worldmap_fn_8002C408.inc"
}
#else
void fn_8002C408(void) { /* TODO */ }
#endif

/* fn_8002CE6C - 0x8002CE6C | size: 0x2e8 */
extern void fn_800D3088(void);
extern void fn_80129474(void);
extern u32 lbl_804788A8;
extern u32 lbl_80478E54;
extern u32 lbl_80478E44;
#if 1
asm void fn_8002CE6C(void) {
#include "src/game/gs_worldmap_fn_8002CE6C.inc"
}
#else
void fn_8002CE6C(void) { /* TODO */ }
#endif

/* fn_8002D154 - 0x8002D154 | size: 0x480 */
extern void fn_80129B2C(void);
extern u32 lbl_804788A8;
extern u32 lbl_80478E54;
extern u32 lbl_80478E44;
extern u32 lbl_8047A3E4;
#if 1
asm void fn_8002D154(void) {
#include "src/game/gs_worldmap_fn_8002D154.inc"
}
#else
void fn_8002D154(void) { /* TODO */ }
#endif

/* fn_8002D5D4 - 0x8002D5D4 | size: 0x348 */
extern void fn_80018F54(void);
extern void fn_8010264C(void);
extern void fn_800E3534(void);
extern void fn_800E27B0(void);
extern void fn_800E24B0(void);
extern void fn_800E209C(void);
extern u32 lbl_8047A3FC;
extern u32 lbl_80478E54;
extern u32 lbl_8047A3DC;
#if 1
asm void fn_8002D5D4(void) {
#include "src/game/gs_worldmap_fn_8002D5D4.inc"
}
#else
void fn_8002D5D4(void) { /* TODO */ }
#endif

/* fn_8002D91C - 0x8002D91C | size: 0x350 */
extern u32 lbl_80478E54;
extern u32 lbl_8047A3DC;
#if 1
asm void fn_8002D91C(void) {
#include "src/game/gs_worldmap_fn_8002D91C.inc"
}
#else
void fn_8002D91C(void) { /* TODO */ }
#endif

/* fn_8002DC6C - 0x8002DC6C | size: 0xb8 */
extern void fn_801D23C0(void);
extern u32 fn_800D37CC(void);
extern void fn_8010206C(f32);
extern void fn_8019075C(s32, s32);
extern void fn_80102038(f32);
extern f64 lbl_8047B998;
extern f32 lbl_8047B9CC;
#if 1
asm void fn_8002DC6C(void) {
#include "src/game/gs_worldmap_fn_8002DC6C.inc"
}
#else
void fn_8002DC6C(void) { /* TODO */ }
#endif

/* fn_8002DD24 - 0x8002DD24 | size: 0x1ec */
extern void fn_80089E20(void);
extern void fn_801D055C(void);
extern void fn_801D04D0(void);
extern void fn_80089D98(void);
extern void fn_801D046C(void);
extern void fn_801D04F4(void);
extern void fn_8008ABE4(void);
extern void fn_801D039C(void);
extern void fn_8001D7E4(void);
extern void fn_800E0C04(void);
extern u32 lbl_8047A424;
extern u8 lbl_803A2518[];
extern u32 lbl_8047A420;
extern u32 lbl_8047A40C;
extern u32 lbl_804788B0;
extern u32 lbl_8047A42C;
#if 1
asm void fn_8002DD24(void) {
#include "src/game/gs_worldmap_fn_8002DD24.inc"
}
#else
void fn_8002DD24(void) { /* TODO */ }
#endif

/* fn_8002DF10 - 0x8002DF10 | size: 0x35c */
extern void fn_80128A64(void);
extern void fn_801CB9D8(void);
extern void fn_80112260(void);
extern void fn_8012805C(void);
extern void fn_80176E0C(void);
extern void fn_80113F48(void);
extern void fn_801CBA0C(void);
extern void fn_800F9318(void);
extern void fn_80177A44(void);
extern void fn_800E4014(void);
extern u32 lbl_8047A424;
extern u32 lbl_8047A40C;
extern u32 lbl_8047A420;
extern f32 lbl_8047B9D0;
extern u32 lbl_8047A41C;
extern u32 lbl_8047A418;
extern f32 lbl_8047B9D4;
extern u32 lbl_8047A408;
extern u32 lbl_8047A414;
extern u32 lbl_8047A42C;
#if 1
asm void fn_8002DF10(void) {
#include "src/game/gs_worldmap_fn_8002DF10.inc"
}
#else
void fn_8002DF10(void) { /* TODO */ }
#endif

/* fn_8002E26C - 0x8002E26C | size: 0x1f4 */
extern void fn_80124A60(void);
extern void fn_8011F5FC(void);
extern void fn_801024E8(void);
extern void fn_801CB834(void);
extern void fn_80176B48(void);
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
extern u32 lbl_8047A414;
extern u32 lbl_8047A41C;
extern f32 lbl_8047B9D0;
extern f32 lbl_8047B9D8;
extern u32 lbl_8047A42C;
#if 1
asm void fn_8002E26C(void) {
#include "src/game/gs_worldmap_fn_8002E26C.inc"
}
#else
void fn_8002E26C(void) { /* TODO */ }
#endif

/* fn_8002E460 - 0x8002E460 | size: 0x5fc */
extern void fn_80104704(void);
extern void fn_801046C8(void);
extern void fn_80109220(void);
extern void fn_80073A44(void);
extern void fn_8017B1AC(void);
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
extern f32 lbl_8047B9D0;
extern u32 lbl_8047A42C;
extern u32 lbl_8047A428;
extern u32 lbl_8047A410;
#if 1
asm void fn_8002E460(void) {
#include "src/game/gs_worldmap_fn_8002E460.inc"
}
#else
void fn_8002E460(void) { /* TODO */ }
#endif

/* fn_8002EA5C - 0x8002EA5C | size: 0x418 */
extern void fn_8011F1A0(void);
extern void fn_80144064(void);
extern void fn_801021F8(void);
extern void fn_8012AC08(void);
extern void fn_8011E850(void);
extern void fn_80075FEC(void);
extern void fn_8011E8DC(void);
extern void fn_8012640C(void);
extern u32 lbl_8047A428;
extern u8 lbl_803A2688[];
extern f32 lbl_8047B9D4;
extern f64 lbl_8047B9E0;
extern f64 lbl_8047B9E8;
extern f32 lbl_8047B9DC;
extern u32 lbl_8047A42C;
extern u32 lbl_8047A420;
extern f32 lbl_8047B9D0;
#if 1
asm void fn_8002EA5C(void) {
#include "src/game/gs_worldmap_fn_8002EA5C.inc"
}
#else
void fn_8002EA5C(void) { /* TODO */ }
#endif

/* fn_8002EE74 - 0x8002EE74 | size: 0x410 */
extern void fn_80103CC0(void);
extern void fn_801045A8(void);
extern void fn_801043A4(void);
extern void fn_801023E4(void);
extern void fn_80102004(void);
extern u32 lbl_8047A428;
extern f32 lbl_8047B9D4;
extern f64 lbl_8047B9E0;
extern f64 lbl_8047B9E8;
extern f32 lbl_8047B9DC;
extern u32 lbl_8047A42C;
#if 1
asm void fn_8002EE74(void) {
#include "src/game/gs_worldmap_fn_8002EE74.inc"
}
#else
void fn_8002EE74(void) { /* TODO */ }
#endif

/* fn_8002F284 - 0x8002F284 | size: 0x518 */
extern void fn_8005D8F8(void);
extern void fn_80102138(void);
extern void fn_801022B8(void);
extern u32 lbl_8047A410;
extern u32 lbl_8047A42C;
extern u8 lbl_80266E90[];
extern u32 lbl_8047A428;
#if 1
asm void fn_8002F284(void) {
#include "src/game/gs_worldmap_fn_8002F284.inc"
}
#else
void fn_8002F284(void) { /* TODO */ }
#endif

/* fn_8002F79C - 0x8002F79C | size: 0x4bc */
extern void fn_8014402C(void);
extern void fn_8011ED68(void);
extern u32 lbl_8047A428;
extern f32 lbl_8047B9D4;
extern f64 lbl_8047B9E0;
extern f64 lbl_8047B9E8;
extern f32 lbl_8047B9DC;
extern u32 lbl_8047A42C;
extern u32 lbl_8047A410;
extern u32 lbl_8047A424;
#if 1
asm void fn_8002F79C(void) {
#include "src/game/gs_worldmap_fn_8002F79C.inc"
}
#else
void fn_8002F79C(void) { /* TODO */ }
#endif
