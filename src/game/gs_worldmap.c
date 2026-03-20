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
