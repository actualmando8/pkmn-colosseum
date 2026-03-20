/**
 * @file gs_pokemon_summary.c
 * @brief GSpokeSummary -- Pokemon summary screen and status display.
 *
 * Address range: 0x80015000 - 0x800181C4 (~30 functions)
 *
 * This module implements the Pokemon summary/status screen that displays
 * detailed information about a single Pokemon. It handles:
 *   - Multi-page summary display (Info, Moves, Stats, Ribbons, etc.)
 *   - Move detail popup with type/power/accuracy
 *   - Contest stat visualization
 *   - Ribbon collection display
 *   - Shadow Pokemon purification gauge
 *
 * The summary screen operates as a sub-state of the party menu. When
 * the player selects "Summary" on a party Pokemon, this module takes
 * over rendering and input handling.
 *
 * Key functions:
 *   fn_8001501C  GSpokeSummary_DrawLabel      -- 0x34 bytes, render text label
 *   fn_80015050  GSpokeSummary_DrawHandler     -- 0x94 bytes, invoke page draw handler
 *   fn_800150E4  GSpokeSummary_DrawPage        -- 0x290 bytes, main page renderer
 *   fn_80015374  GSpokeSummary_DrawMovePage     -- 0x23C bytes, move list page
 *   fn_800155B0  GSpokeSummary_DrawStatsPage    -- 0x40C bytes, stat hexagon page
 *   fn_800159BC  GSpokeSummary_DrawRibbonPage   -- 0x480 bytes, ribbon collection
 *   fn_80015E3C  GSpokeSummary_ProcessInput     -- 0x374 bytes, input handler
 *   fn_800161B0  GSpokeSummary_PageTransition   -- 0x198 bytes, page flip animation
 *   fn_80016348  GSpokeSummary_UpdateCursor     -- 0x188 bytes, cursor movement
 *   fn_800164D0  GSpokeSummary_MoveDetail       -- 0x148 bytes, move detail popup
 *   fn_80016618  GSpokeSummary_GetPageCount     -- 0xA4 bytes, count available pages
 *   fn_800166BC  GSpokeSummary_SetupPage        -- 0x114 bytes, initialize page data
 *   fn_800167D0  GSpokeSummary_DrawTypeIcon     -- 0x2EC bytes, draw Pokemon type icon
 *   fn_80016ABC  GSpokeSummary_DrawStatBar      -- 0x458 bytes, draw stat bar graphic
 *   fn_80016F14  GSpokeSummary_DrawExpBar       -- 0x114 bytes, draw EXP bar
 *   fn_80017028  GSpokeSummary_DrawHPBar        -- 0x73C bytes, draw HP bar with color
 *   fn_80017764  GSpokeSummary_GetBarColor      -- 0x2C bytes, HP color threshold
 *   fn_80017790  GSpokeSummary_FormatLevel      -- 0xD8 bytes, format "Lv.XX" string
 *   fn_80017868  GSpokeSummary_FormatHP         -- 0x84 bytes, format "HP/MaxHP"
 *   fn_800178EC  GSpokeSummary_GetGenderChar    -- 0x28 bytes, get gender symbol
 *   fn_80017914  GSpokeSummary_GetShinyIcon     -- 0x28 bytes, get shiny star icon
 *   fn_8001793C  GSpokeSummary_DrawShadowGauge  -- 0x54 bytes, draw heart gauge
 *   fn_80017990  GSpokeSummary_AnimateGauge     -- 0x7C bytes, gauge fill animation
 *   fn_80017A0C  GSpokeSummary_DrawPageDots     -- 0x2AC bytes, page indicator dots
 *   fn_80017CB8  GSpokeSummary_DrawBackground   -- 0x1D4 bytes, background gradient
 *   fn_80017E8C  GSpokeSummary_Init             -- 0x338 bytes, full initialization
 *
 * The page drawing system uses a function pointer table at lbl_80266918.
 * Each page entry is 0x4C bytes and contains:
 *   +0x04: Pokemon index for data source (s32, -1 = use party default)
 *   +0x18: Draw handler function pointer
 *   +0x1C: Data source message ID (u16)
 *
 * fn_800150E4 (GSpokeSummary_DrawPage) iterates over the Pokemon's
 * data fields and renders them using fn_80129BC8 (get Pokemon field data)
 * and fn_801429E8 (check if field is valid/non-empty). It calculates
 * scroll positions using floating point arithmetic with stick input
 * values from lbl_8047A2D0 (analog stick deflection).
 *
 * SDA globals:
 *   lbl_8047A2BC: Current summary screen mode (s32)
 *   lbl_8047A2C8: Analog stick integration value (s32)
 *   lbl_8047A2D0: Stick deflection (float)
 *   lbl_8047A2D8: Pokemon data cache pointer
 *   lbl_8047A2DC: Current label text resource
 *   lbl_8047A2F8: Pokemon data table pointer
 *
 * Rodata:
 *   lbl_80266918: Page handler/data table (used by DrawHandler at +0x18)
 *   lbl_8047B748: Float constant 0.0f (stick deadzone)
 *   lbl_8047B8B8: Float constant for int-to-float conversion (0x43300000)
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Pokemon data access */
extern void* fn_80129BC8(void* pokeData, u8 fieldId, u16* outCount,
                          s32 p4, s32 p5, s32 p6);
extern void* fn_801297D8(void* pokeData, u16* outCount, s32 p3, s32 p4, s32 p5);
extern u8    fn_801429E8(void* fieldData);    /* Check field validity */
extern u16   fn_80143C50(void* fieldData);    /* Get field value */
extern u16   fn_801440A0(u16 speciesId);      /* Get species data */
extern u16   fn_80143FFC(void);               /* Get display field count */

/* Text rendering */
extern void  fn_80132A38(s32 paramId, s32 value);
extern u32   fn_800FA444(s32 resourceId);
extern void  fn_800FB680(s32 x, s32 y, s32 flags, u32 color, u16 resourceId);

/* Math/rendering helpers */
extern void  fn_800E0CA0(f32 angle);          /* Set camera rotation */
extern void  fn_800E090C(void* outVec, void* posA, void* posB); /* Vector subtract */
extern void  fn_80106ADC(s32 p1, void* data, s32 p3, s32 p4, u8 p5);

/* =========================================================================
 * SDA globals
 * ========================================================================= */

extern s32   gSummaryMode;        /* lbl_8047A2BC */
extern void* gPokeDataCache;      /* lbl_8047A2D8 */
extern void* gPokeDataTable;      /* lbl_8047A2F8 */
extern s32   gSummaryLabel;       /* lbl_8047A2DC */
extern f32   gStickDeflection;    /* lbl_8047A2D0 */
