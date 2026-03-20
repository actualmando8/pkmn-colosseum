/**
 * @file movie.h
 * @brief THP movie playback system for Pokemon Colosseum.
 *
 * The movie system handles playback of THP (Nintendo's video format) files
 * for opening cinematics, logo screens, credits, auto-demo sequences, etc.
 *
 * THP files referenced in rodata:
 *   "movie/openingdemo.thp"  -- Opening cinematic
 *   "movie/staffroll.thp"    -- Credits / staff roll
 *   "movie/autodemo01.thp"   -- Auto-demo / attract mode
 *   "movie/gs_logo.thp"      -- Genius Sonority logo
 *   "movie/tpc.thp"          -- The Pokemon Company logo
 *
 * The movie playback functions sit in the early game code at approximately
 * 0x80035E04 - 0x800366D0. They call into the THP player library
 * (fn_801E1874 for THPPlayerGetState, fn_801E189C for THPPlayerOpen, etc.)
 * and coordinate with the sound system (fn_80165A20) and flag system
 * (fn_801902E0, fn_80190528) for proper sequencing.
 *
 * Address range: 0x80035E04 - 0x800366D0 (approx.)
 */
#ifndef GAME_MOVIE_H
#define GAME_MOVIE_H

#include "dolphin/types.h"

/* ===================================================================
 * Movie ID constants
 *
 * These are derived from the order in which the movie paths appear
 * in the rodata and from the calling conventions in the game code.
 * =================================================================== */
#define MOVIE_ID_OPENING_DEMO   0   /* movie/openingdemo.thp */
#define MOVIE_ID_STAFF_ROLL     1   /* movie/staffroll.thp */
#define MOVIE_ID_AUTO_DEMO      2   /* movie/autodemo01.thp */
#define MOVIE_ID_GS_LOGO        3   /* movie/gs_logo.thp */
#define MOVIE_ID_TPC_LOGO       4   /* movie/tpc.thp */

/* ===================================================================
 * THP player state constants (returned by THPPlayerGetState)
 * =================================================================== */
#define THP_STATE_IDLE       0   /* player not active */
#define THP_STATE_PLAYING    1   /* currently playing a movie */
#define THP_STATE_STOPPED    2   /* playback finished or stopped */
#define THP_STATE_ERROR      3   /* an error occurred */

/* ===================================================================
 * Public API
 * =================================================================== */

/**
 * moviePlayOpeningDemo -- Play the opening demo cinematic.
 *
 * Sets up fade parameters, opens "movie/openingdemo.thp" via the
 * THP player, and triggers the associated BGM (sound ID 0x0495).
 *
 * Corresponds to fn_80035EE4.
 */
void moviePlayOpeningDemo(void);

/**
 * moviePlayStaffRoll -- Play the staff roll / credits sequence.
 *
 * The staff roll has special logic:
 *   1. Waits for any current THP playback to finish
 *   2. Stops BGM and sounds
 *   3. Checks game flags to determine if the special post-game
 *      credits variant should play
 *   4. Opens "movie/staffroll.thp"
 *   5. Manages battle/floor transitions around the movie
 *
 * Corresponds to fn_80035F64.
 */
void moviePlayStaffRoll(void);

/**
 * moviePlayAutoDemo -- Set up and play the auto-demo / attract mode.
 *
 * Sets the fade mode and opens "movie/autodemo01.thp" for the
 * title screen attract sequence. Called from the title screen
 * idle timer.
 *
 * Corresponds to fn_80035F34 (sets up fade) and related functions.
 */
void moviePlayAutoDemo(void);

/**
 * moviePlayGSLogo -- Play the Genius Sonority logo movie.
 *
 * Opens "movie/gs_logo.thp" for the boot-up logo sequence.
 * Called during the initial boot screen sequence.
 *
 * Corresponds to a call site near 0x80036568.
 */
void moviePlayGSLogo(void);

/**
 * moviePlayTPCLogo -- Play The Pokemon Company logo movie.
 *
 * Opens "movie/tpc.thp" for the boot-up logo sequence.
 * Called during the initial boot screen sequence.
 *
 * Corresponds to a call site near 0x80036668.
 */
void moviePlayTPCLogo(void);

/**
 * movieWaitForFinish -- Spin-wait until the current THP playback ends.
 *
 * Polls THPPlayerGetState (fn_801E1874) in a loop, yielding via
 * fn_800F0308 (GStextureFlush / GSthread yield) each frame, until
 * the state is no longer THP_STATE_PLAYING.
 *
 * This pattern appears at multiple call sites in the movie code.
 *
 * Corresponds to the wait loops in fn_80035E04, fn_80035F64, etc.
 */
void movieWaitForFinish(void);

/**
 * movieStopAndCleanup -- Stop playback and release resources.
 *
 * Stops BGM (sound ID 1, volume 0, time 0x7F), cleans up any
 * floor/battle state that was modified for movie playback,
 * and restores normal game flow.
 *
 * Corresponds to the cleanup code in fn_80035E04 and fn_80035F64.
 */
void movieStopAndCleanup(void);

#endif /* GAME_MOVIE_H */
