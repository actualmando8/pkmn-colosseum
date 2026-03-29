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
#if 1
asm void fn_80026370(void) {
#include "src/game/gs_worldmap_fn_80026370.inc"
}
#else
void fn_80026370(void) { /* TODO */ }
#endif

/* fn_80026390 - 0x80026390 | size: 0x20 */
#if 1
asm void fn_80026390(void) {
#include "src/game/gs_worldmap_fn_80026390.inc"
}
#else
void fn_80026390(void) { /* TODO */ }
#endif

/* fn_800263B0 - 0x800263B0 | size: 0x6c */
extern u8 lbl_80266E18[];
#if 1
asm void fn_800263B0(void) {
#include "src/game/gs_worldmap_fn_800263B0.inc"
}
#else
void fn_800263B0(void) { /* TODO */ }
#endif

/* fn_8002641C - 0x8002641C | size: 0x5c */
#if 1
asm void fn_8002641C(void) {
#include "src/game/gs_worldmap_fn_8002641C.inc"
}
#else
void fn_8002641C(void) { /* TODO */ }
#endif

/* fn_80026478 - 0x80026478 | size: 0xa4 */
extern void fn_8012A5B0(void);
extern void fn_80123FBC(void);
extern void fn_801231A4(void);
#if 1
asm void fn_80026478(void) {
#include "src/game/gs_worldmap_fn_80026478.inc"
}
#else
void fn_80026478(void) { /* TODO */ }
#endif

/* fn_8002651C - 0x8002651C | size: 0xa4 */
#if 1
asm void fn_8002651C(void) {
#include "src/game/gs_worldmap_fn_8002651C.inc"
}
#else
void fn_8002651C(void) { /* TODO */ }
#endif

/* fn_800265C0 - 0x800265C0 | size: 0x40 */
extern u8 lbl_80266DD8[];
#if 1
asm void fn_800265C0(void) {
#include "src/game/gs_worldmap_fn_800265C0.inc"
}
#else
void fn_800265C0(void) { /* TODO */ }
#endif

/* fn_80026600 - 0x80026600 | size: 0x40 */
#if 1
asm void fn_80026600(void) {
#include "src/game/gs_worldmap_fn_80026600.inc"
}
#else
void fn_80026600(void) { /* TODO */ }
#endif

/* fn_80026640 - 0x80026640 | size: 0x40 */
#if 1
asm void fn_80026640(void) {
#include "src/game/gs_worldmap_fn_80026640.inc"
}
#else
void fn_80026640(void) { /* TODO */ }
#endif

/* fn_80026680 - 0x80026680 | size: 0x40 */
#if 1
asm void fn_80026680(void) {
#include "src/game/gs_worldmap_fn_80026680.inc"
}
#else
void fn_80026680(void) { /* TODO */ }
#endif

/* fn_800266C0 - 0x800266C0 | size: 0x40 */
#if 1
asm void fn_800266C0(void) {
#include "src/game/gs_worldmap_fn_800266C0.inc"
}
#else
void fn_800266C0(void) { /* TODO */ }
#endif

/* fn_80026700 - 0x80026700 | size: 0x40 */
#if 1
asm void fn_80026700(void) {
#include "src/game/gs_worldmap_fn_80026700.inc"
}
#else
void fn_80026700(void) { /* TODO */ }
#endif

/* fn_80026740 - 0x80026740 | size: 0x90 */
extern u32 lbl_8047B938;
extern u32 lbl_8047B934;
#if 1
asm void fn_80026740(void) {
#include "src/game/gs_worldmap_fn_80026740.inc"
}
#else
void fn_80026740(void) { /* TODO */ }
#endif

/* fn_800267D0 - 0x800267D0 | size: 0x90 */
extern u32 lbl_8047B938;
extern u32 lbl_8047B934;
#if 1
asm void fn_800267D0(void) {
#include "src/game/gs_worldmap_fn_800267D0.inc"
}
#else
void fn_800267D0(void) { /* TODO */ }
#endif

/* fn_80026860 - 0x80026860 | size: 0x90 */
extern u32 lbl_8047B938;
extern u32 lbl_8047B934;
#if 1
asm void fn_80026860(void) {
#include "src/game/gs_worldmap_fn_80026860.inc"
}
#else
void fn_80026860(void) { /* TODO */ }
#endif

/* fn_800268F0 - 0x800268F0 | size: 0x254 */
extern void fn_80132A38(void);
extern void fn_800FA444(void);
extern void fn_800FB680(void);
extern void fn_800FA314(void);
extern void fn_800FA280(void);
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
#if 1
asm void fn_800268F0(void) {
#include "src/game/gs_worldmap_fn_800268F0.inc"
}
#else
void fn_800268F0(void) { /* TODO */ }
#endif

/* fn_80026B44 - 0x80026B44 | size: 0x254 */
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
#if 1
asm void fn_80026B44(void) {
#include "src/game/gs_worldmap_fn_80026B44.inc"
}
#else
void fn_80026B44(void) { /* TODO */ }
#endif

/* fn_80026D98 - 0x80026D98 | size: 0x254 */
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
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
extern u32 lbl_8047B948;
extern u32 lbl_8047B93C;
extern u32 lbl_8047B940;
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
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
extern u32 lbl_8047B948;
extern u32 lbl_8047B93C;
extern u32 lbl_8047B940;
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
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
extern u32 lbl_8047B948;
extern u32 lbl_8047B93C;
extern u32 lbl_8047B940;
extern u32 lbl_8047B934;
extern u32 lbl_8047B938;
#if 1
asm void fn_8002730C(void) {
#include "src/game/gs_worldmap_fn_8002730C.inc"
}
#else
void fn_8002730C(void) { /* TODO */ }
#endif

/* fn_8002749C - 0x8002749C | size: 0x158 */
extern u32 lbl_8047B948;
extern u32 lbl_8047B93C;
extern u32 lbl_8047B934;
extern u32 lbl_8047B940;
extern u32 lbl_8047B938;
#if 1
asm void fn_8002749C(void) {
#include "src/game/gs_worldmap_fn_8002749C.inc"
}
#else
void fn_8002749C(void) { /* TODO */ }
#endif

/* fn_800275F4 - 0x800275F4 | size: 0x14c */
#if 1
asm void fn_800275F4(void) {
#include "src/game/gs_worldmap_fn_800275F4.inc"
}
#else
void fn_800275F4(void) { /* TODO */ }
#endif

/* fn_8002777C - 0x8002777C | size: 0x3c */
#if 1
asm void fn_8002777C(void) {
#include "src/game/gs_worldmap_fn_8002777C.inc"
}
#else
void fn_8002777C(void) { /* TODO */ }
#endif

/* fn_800277B8 - 0x800277B8 | size: 0x3c */
#if 1
asm void fn_800277B8(void) {
#include "src/game/gs_worldmap_fn_800277B8.inc"
}
#else
void fn_800277B8(void) { /* TODO */ }
#endif

/* fn_800278A4 - 0x800278A4 | size: 0xbc */
extern void fn_8011F5C8(void);
#if 1
asm void fn_800278A4(void) {
#include "src/game/gs_worldmap_fn_800278A4.inc"
}
#else
void fn_800278A4(void) { /* TODO */ }
#endif

/* fn_80027960 - 0x80027960 | size: 0x144 */
extern u8 lbl_8047B920[];
#if 1
asm void fn_80027960(void) {
#include "src/game/gs_worldmap_fn_80027960.inc"
}
#else
void fn_80027960(void) { /* TODO */ }
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
extern void fn_80105624(void);
#if 1
asm void fn_80027D58(void) {
#include "src/game/gs_worldmap_fn_80027D58.inc"
}
#else
void fn_80027D58(void) { /* TODO */ }
#endif

/* fn_800280FC - 0x800280FC | size: 0xf4 */
extern void fn_801080CC(void);
extern u32 lbl_8047B930;
extern u32 lbl_8047B950;
extern u32 lbl_8047B934;
#if 1
asm void fn_800280FC(void) {
#include "src/game/gs_worldmap_fn_800280FC.inc"
}
#else
void fn_800280FC(void) { /* TODO */ }
#endif

/* fn_800281F0 - 0x800281F0 | size: 0x4 */
#if 1
asm void fn_800281F0(void) {
#include "src/game/gs_worldmap_fn_800281F0.inc"
}
#else
void fn_800281F0(void) { /* TODO */ }
#endif

/* fn_800281F4 - 0x800281F4 | size: 0x250 */
extern void fn_800F9E70(void);
extern void fn_801337A8(void);
extern void fn_801046B8(void);
extern void fn_801026A4(void);
extern void fn_80106D3C(void);
extern void fn_8001E074(void);
extern void fn_801069FC(void);
extern void fn_80102510(void);
extern void fn_80102428(void);
extern u32 lbl_8047B930;
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
#if 1
asm void fn_80028444(void) {
#include "src/game/gs_worldmap_fn_80028444.inc"
}
#else
void fn_80028444(void) { /* TODO */ }
#endif

/* fn_80028494 - 0x80028494 | size: 0x50 */
#if 1
asm void fn_80028494(void) {
#include "src/game/gs_worldmap_fn_80028494.inc"
}
#else
void fn_80028494(void) { /* TODO */ }
#endif

/* fn_800284E4 - 0x800284E4 | size: 0x50 */
#if 1
asm void fn_800284E4(void) {
#include "src/game/gs_worldmap_fn_800284E4.inc"
}
#else
void fn_800284E4(void) { /* TODO */ }
#endif

/* fn_80028534 - 0x80028534 | size: 0x54 */
#if 1
asm void fn_80028534(void) {
#include "src/game/gs_worldmap_fn_80028534.inc"
}
#else
void fn_80028534(void) { /* TODO */ }
#endif

/* fn_80028588 - 0x80028588 | size: 0x98 */
#if 1
asm void fn_80028588(void) {
#include "src/game/gs_worldmap_fn_80028588.inc"
}
#else
void fn_80028588(void) { /* TODO */ }
#endif

/* fn_80028620 - 0x80028620 | size: 0x108 */
extern void fn_80109934(void);
extern void fn_800D888C(void);
extern void fn_800D88DC(void);
extern void fn_800D7820(void);
extern void fn_800D85D4(void);
extern void fn_800D6A00(void);
extern void fn_800D67BC(void);
extern void fn_800D61E4(void);
extern void fn_800D5CB8(void);
extern void fn_800D59B8(void);
extern void fn_800D6728(void);
extern u8 lbl_803A2094[];
extern u8 lbl_80314F98[];
extern u32 lbl_8047B930;
extern u32 lbl_8047B934;
#if 1
asm void fn_80028620(void) {
#include "src/game/gs_worldmap_fn_80028620.inc"
}
#else
void fn_80028620(void) { /* TODO */ }
#endif

/* fn_80028728 - 0x80028728 | size: 0x108 */
extern u32 lbl_8047B930;
extern u32 lbl_8047B934;
#if 1
asm void fn_80028728(void) {
#include "src/game/gs_worldmap_fn_80028728.inc"
}
#else
void fn_80028728(void) { /* TODO */ }
#endif

/* fn_80028830 - 0x80028830 | size: 0x118 */
extern void fn_8005D858(void);
extern void fn_80104160(void);
extern u8 lbl_803A20DC[];
extern u32 lbl_8047B948;
extern u32 lbl_8047B940;
#if 1
asm void fn_80028830(void) {
#include "src/game/gs_worldmap_fn_80028830.inc"
}
#else
void fn_80028830(void) { /* TODO */ }
#endif

/* fn_80028948 - 0x80028948 | size: 0x674 */
extern void fn_800E0BE4(void);
extern u32 lbl_8047B958;
extern u32 lbl_8047B95C;
extern u32 lbl_8047B960;
extern u32 lbl_8047B964;
extern u32 lbl_8047B968;
extern u32 lbl_8047B930;
extern u32 lbl_8047B970;
extern u32 lbl_8047B96C;
extern u32 lbl_8047B934;
extern u32 lbl_8047B954;
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
extern void fn_8011288C(void);
extern u32 lbl_804788A0;
extern u8 lbl_80266DC0[];
extern u32 lbl_8047B940;
#if 1
asm void fn_80028FBC(void) {
#include "src/game/gs_worldmap_fn_80028FBC.inc"
}
#else
void fn_80028FBC(void) { /* TODO */ }
#endif

/* fn_80029558 - 0x80029558 | size: 0xe0 */
#if 1
asm void fn_80029558(void) {
#include "src/game/gs_worldmap_fn_80029558.inc"
}
#else
void fn_80029558(void) { /* TODO */ }
#endif

/* fn_80029638 - 0x80029638 | size: 0x28 */
#if 1
asm void fn_80029638(void) {
#include "src/game/gs_worldmap_fn_80029638.inc"
}
#else
void fn_80029638(void) { /* TODO */ }
#endif

/* fn_80029660 - 0x80029660 | size: 0x100 */
extern void fn_800FF730(void);
extern void fn_800F0308(void);
#if 1
asm void fn_80029660(void) {
#include "src/game/gs_worldmap_fn_80029660.inc"
}
#else
void fn_80029660(void) { /* TODO */ }
#endif

/* fn_80029760 - 0x80029760 | size: 0xf0 */
#if 1
asm void fn_80029760(void) {
#include "src/game/gs_worldmap_fn_80029760.inc"
}
#else
void fn_80029760(void) { /* TODO */ }
#endif

/* fn_80029850 - 0x80029850 | size: 0x8c */
extern void fn_80143C68(void);
extern void fn_80143C50(void);
#if 1
asm void fn_80029850(void) {
#include "src/game/gs_worldmap_fn_80029850.inc"
}
#else
void fn_80029850(void) { /* TODO */ }
#endif

/* fn_80029AC8 - 0x80029AC8 | size: 0x1f8 */
extern void fn_80143B80(void);
extern void fn_80143B70(void);
#if 1
asm void fn_80029AC8(void) {
#include "src/game/gs_worldmap_fn_80029AC8.inc"
}
#else
void fn_80029AC8(void) { /* TODO */ }
#endif

/* fn_80029CC0 - 0x80029CC0 | size: 0x234 */
extern void fn_80142A88(void);
extern void fn_800849B4(void);
#if 1
asm void fn_80029CC0(void) {
#include "src/game/gs_worldmap_fn_80029CC0.inc"
}
#else
void fn_80029CC0(void) { /* TODO */ }
#endif

/* fn_80029EF4 - 0x80029EF4 | size: 0xb8 */
extern void fn_80129384(void);
extern void fn_8013467C(void);
extern void fn_80129A78(void);
#if 1
asm void fn_80029EF4(void) {
#include "src/game/gs_worldmap_fn_80029EF4.inc"
}
#else
void fn_80029EF4(void) { /* TODO */ }
#endif

/* fn_80029FAC - 0x80029FAC | size: 0x10c */
extern void __va_arg();
extern u32 lbl_80478E54;
extern u32 lbl_80478E4C;
#if 1
asm void fn_80029FAC(void) {
#include "src/game/gs_worldmap_fn_80029FAC.inc"
}
#else
void fn_80029FAC(void) { /* TODO */ }
#endif

/* fn_8002A0B8 - 0x8002A0B8 | size: 0x10c */
extern u32 lbl_80478E54;
extern u32 lbl_80478E3C;
#if 1
asm void fn_8002A0B8(void) {
#include "src/game/gs_worldmap_fn_8002A0B8.inc"
}
#else
void fn_8002A0B8(void) { /* TODO */ }
#endif

/* fn_8002A1C4 - 0x8002A1C4 | size: 0x108 */
extern void fn_80106ADC(void);
extern u32 lbl_80478E54;
extern u32 lbl_80478E4C;
#if 1
asm void fn_8002A1C4(void) {
#include "src/game/gs_worldmap_fn_8002A1C4.inc"
}
#else
void fn_8002A1C4(void) { /* TODO */ }
#endif

/* fn_8002A2CC - 0x8002A2CC | size: 0x108 */
extern u32 lbl_80478E54;
extern u32 lbl_80478E3C;
#if 1
asm void fn_8002A2CC(void) {
#include "src/game/gs_worldmap_fn_8002A2CC.inc"
}
#else
void fn_8002A2CC(void) { /* TODO */ }
#endif

/* fn_8002A3D4 - 0x8002A3D4 | size: 0x2c */
#if 1
asm void fn_8002A3D4(void) {
#include "src/game/gs_worldmap_fn_8002A3D4.inc"
}
#else
void fn_8002A3D4(void) { /* TODO */ }
#endif

/* fn_8002A400 - 0x8002A400 | size: 0x8c */
#if 1
asm void fn_8002A400(void) {
#include "src/game/gs_worldmap_fn_8002A400.inc"
}
#else
void fn_8002A400(void) { /* TODO */ }
#endif

/* fn_8002A48C - 0x8002A48C | size: 0x124 */
extern void fn_800FB8C8(void);
extern u8 lbl_80266E58[];
#if 1
asm void fn_8002A48C(void) {
#include "src/game/gs_worldmap_fn_8002A48C.inc"
}
#else
void fn_8002A48C(void) { /* TODO */ }
#endif

/* fn_8002A5B0 - 0x8002A5B0 | size: 0x68 */
#if 1
asm void fn_8002A5B0(void) {
#include "src/game/gs_worldmap_fn_8002A5B0.inc"
}
#else
void fn_8002A5B0(void) { /* TODO */ }
#endif

/* fn_8002AA68 - 0x8002AA68 | size: 0x98 */
#if 1
asm void fn_8002AA68(void) {
#include "src/game/gs_worldmap_fn_8002AA68.inc"
}
#else
void fn_8002AA68(void) { /* TODO */ }
#endif

/* fn_8002AB00 - 0x8002AB00 | size: 0x40 */
extern u8 lbl_80266E70[];
#if 1
asm void fn_8002AB00(void) {
#include "src/game/gs_worldmap_fn_8002AB00.inc"
}
#else
void fn_8002AB00(void) { /* TODO */ }
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
#if 1
asm void fn_8002AE44(void) {
#include "src/game/gs_worldmap_fn_8002AE44.inc"
}
#else
void fn_8002AE44(void) { /* TODO */ }
#endif

/* fn_8002AE68 - 0x8002AE68 | size: 0x34 */
#if 1
asm void fn_8002AE68(void) {
#include "src/game/gs_worldmap_fn_8002AE68.inc"
}
#else
void fn_8002AE68(void) { /* TODO */ }
#endif

/* fn_8002AE9C - 0x8002AE9C | size: 0x5c */
#if 1
asm void fn_8002AE9C(void) {
#include "src/game/gs_worldmap_fn_8002AE9C.inc"
}
#else
void fn_8002AE9C(void) { /* TODO */ }
#endif

/* fn_8002AEF8 - 0x8002AEF8 | size: 0x144 */
extern void fn_801440A0(void);
extern void fn_80144014(void);
extern void fn_80129BC8(void);
extern void fn_80142CF4(void);
#if 1
asm void fn_8002AEF8(void) {
#include "src/game/gs_worldmap_fn_8002AEF8.inc"
}
#else
void fn_8002AEF8(void) { /* TODO */ }
#endif

/* fn_8002B03C - 0x8002B03C | size: 0x4c */
#if 1
asm void fn_8002B03C(void) {
#include "src/game/gs_worldmap_fn_8002B03C.inc"
}
#else
void fn_8002B03C(void) { /* TODO */ }
#endif

/* fn_8002B088 - 0x8002B088 | size: 0x34 */
extern u32 lbl_8047A3E4;
#if 1
asm void fn_8002B088(void) {
#include "src/game/gs_worldmap_fn_8002B088.inc"
}
#else
void fn_8002B088(void) { /* TODO */ }
#endif

/* fn_8002B0BC - 0x8002B0BC | size: 0x78 */
extern u32 lbl_8047B97C;
extern u32 lbl_8047A3E8;
extern u32 lbl_8047B978;
#if 1
asm void fn_8002B0BC(void) {
#include "src/game/gs_worldmap_fn_8002B0BC.inc"
}
#else
void fn_8002B0BC(void) { /* TODO */ }
#endif

/* fn_8002B134 - 0x8002B134 | size: 0x6c */
extern u32 lbl_8047B97C;
extern u32 lbl_8047A3E8;
extern u32 lbl_8047B978;
#if 1
asm void fn_8002B134(void) {
#include "src/game/gs_worldmap_fn_8002B134.inc"
}
#else
void fn_8002B134(void) { /* TODO */ }
#endif

/* fn_8002B1A0 - 0x8002B1A0 | size: 0x26c */
extern void fn_800FE38C(void);
extern void fn_80144088(void);
extern void fn_80143FFC(void);
extern void fn_80143FE4(void);
extern void fn_800FE35C(void);
extern u32 lbl_8047B980;
#if 1
asm void fn_8002B1A0(void) {
#include "src/game/gs_worldmap_fn_8002B1A0.inc"
}
#else
void fn_8002B1A0(void) { /* TODO */ }
#endif

/* fn_8002B40C - 0x8002B40C | size: 0x188 */
extern u8 lbl_802E4F68[];
extern u32 lbl_8047B998;
extern u32 lbl_8047B984;
extern u32 lbl_8047B988;
extern u32 lbl_8047B98C;
extern u32 lbl_8047B990;
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
extern u32 lbl_8047B980;
extern u32 lbl_8047B97C;
extern u32 lbl_8047B998;
extern u32 lbl_8047B98C;
extern u32 lbl_8047B9A0;
extern u32 lbl_8047B9A4;
extern u32 lbl_8047B9A8;
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
extern u32 lbl_8047B998;
extern u32 lbl_8047B98C;
extern u32 lbl_8047B9A0;
extern u32 lbl_8047B9A4;
extern u32 lbl_8047A3F0;
extern u32 lbl_8047B978;
extern u32 lbl_8047B9AC;
extern u32 lbl_8047B97C;
extern u32 lbl_8047B9B0;
extern u32 lbl_8047B9B4;
#if 1
asm void fn_8002B880(void) {
#include "src/game/gs_worldmap_fn_8002B880.inc"
}
#else
void fn_8002B880(void) { /* TODO */ }
#endif

/* fn_8002BCE8 - 0x8002BCE8 | size: 0x120 */
#if 1
asm void fn_8002BCE8(void) {
#include "src/game/gs_worldmap_fn_8002BCE8.inc"
}
#else
void fn_8002BCE8(void) { /* TODO */ }
#endif

/* fn_8002BE08 - 0x8002BE08 | size: 0x20c */
extern void fn_80143F84(void);
extern u32 lbl_8047B980;
extern u32 lbl_8047B9B8;
extern u32 lbl_8047B9BC;
extern u32 lbl_8047A3E4;
#if 1
asm void fn_8002BE08(void) {
#include "src/game/gs_worldmap_fn_8002BE08.inc"
}
#else
void fn_8002BE08(void) { /* TODO */ }
#endif

/* fn_8002C014 - 0x8002C014 | size: 0xd0 */
extern void fn_80102ED4(void);
#if 1
asm void fn_8002C014(void) {
#include "src/game/gs_worldmap_fn_8002C014.inc"
}
#else
void fn_8002C014(void) { /* TODO */ }
#endif

/* fn_8002C0E4 - 0x8002C0E4 | size: 0x1a0 */
extern u32 lbl_8047B980;
extern u32 lbl_8047B9C0;
extern u32 lbl_8047B9C4;
extern u32 lbl_8047B97C;
extern u32 lbl_8047B9C8;
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
extern u8 lbl_8047A3FC[];
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
extern void fn_800D37CC(void);
extern void fn_8010206C(void);
extern void fn_8019075C(void);
extern void fn_80102038(void);
extern u32 lbl_8047B998;
extern u32 lbl_8047B9CC;
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
extern u32 lbl_8047B9D0;
extern u32 lbl_8047A41C;
extern u32 lbl_8047A418;
extern u32 lbl_8047B9D4;
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
extern u32 lbl_8047B9D0;
extern u32 lbl_8047B9D8;
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
extern u32 lbl_8047B9D0;
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
extern u32 lbl_8047B9D4;
extern u32 lbl_8047B9E0;
extern u32 lbl_8047B9E8;
extern u32 lbl_8047B9DC;
extern u32 lbl_8047A42C;
extern u32 lbl_8047A420;
extern u32 lbl_8047B9D0;
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
extern u32 lbl_8047B9D4;
extern u32 lbl_8047B9E0;
extern u32 lbl_8047B9E8;
extern u32 lbl_8047B9DC;
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
extern u32 lbl_8047B9D4;
extern u32 lbl_8047B9E0;
extern u32 lbl_8047B9E8;
extern u32 lbl_8047B9DC;
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

