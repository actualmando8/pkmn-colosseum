/**
 * @file sound.h
 * @brief Sound system structures and API for Pokemon Colosseum.
 *
 * The sound system wraps Nintendo's JAudio2 middleware and provides:
 *   - BGM (background music) playback with fade in/out
 *   - SE (sound effect) playback with 3D positioning
 *   - Streaming audio from disc (WAVE files via FSYS archives)
 *   - Master volume control with per-channel adjustment
 *   - Sound work tracking for BGM and SE slots
 *
 * The system uses two playback categories:
 *   - BGM: category flag 0x406 (streaming), one active at a time
 *   - SE:  category flag 0x407 (one-shot/looping), multiple simultaneous
 *
 * Global state is stored in SDA-accessible variables (sbss section).
 * Per-sound metadata is kept in SndWork entries (0x0C bytes each).
 *
 * Source file: sound.c
 * Address range: 0x801652DC - 0x80167040
 *
 * Debug strings:
 *   "ERROR: can't open WAVE ID = %d"
 *   "ERROR: can't open WAVE File = %s"
 *   "soundStop: Warning! BGM cannot be stopped.(snd_id=%d)"
 *   "ERROR(sound.c): invalid file_info number snd_id=%d"
 *   "ERROR(sound.c): Can't Play Sound. snd_id=%d"
 *   "ERROR: Can't Read Group(%d)"
 *   "GSsndGetStatus:Forced termination SE=%d"
 *   "GSsndGetStatus:Forced termination BGM=%d"
 *   "_sndCheckSndWorkALL:Start"
 *   "_sndCheckSndWorkALL:End"
 */
#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "dolphin/types.h"

/* -----------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------- */

/** JAudio category IDs used by the sound system */
#define SND_CATEGORY_BGM    0x0406  /* streaming BGM playback */
#define SND_CATEGORY_SE     0x0407  /* sound effect playback  */
#define SND_CATEGORY_STREAM 0x0408  /* streaming wave data    */

/** Buffer size for streaming audio from disc */
#define SND_STREAM_BUFFER_SIZE  0x10000  /* 64 KB */

/** Default master volume (0x7F = max for JAudio) */
#define SND_VOLUME_MAX      0x7F
#define SND_VOLUME_DEFAULT  0x7F

/** Fade time in milliseconds for BGM transitions */
#define SND_FADE_DEFAULT    50   /* 0x32 -- ~50ms quick crossfade */
#define SND_FADE_INSTANT    0

/** Maximum number of queued volume changes per sound work entry */
#define SND_VOLQUEUE_MAX    3

/** Invalid sound handle sentinel (addis r0, handle, 1; cmplwi 0xFFFF) */
#define SND_HANDLE_INVALID  ((u32)0xFFFF0000u)

/** Master volume command codes for SetMasterVolume */
#define SND_MASTER_ALL      0xFF  /* apply to both BGM and SE */
#define SND_MASTER_BGM_ONLY 0xFD  /* apply to BGM channels only */
#define SND_MASTER_SE_ONLY  0xFE  /* apply to SE channels only */

/* -----------------------------------------------------------------------
 * Sound work entry -- per-sound metadata (0x0C bytes)
 *
 * Stored in a flat array at g_sndWorkTable (lbl_80478FAC).
 * The count of entries is in g_sndWorkCount (lbl_8047B0E8).
 * ----------------------------------------------------------------------- */
typedef struct SndWork {
    /* 0x00 */ u8  flags;
    /*         bit 0 (mask 0x02, extrwi r,1,24): isBGM
     *         bit 2 (mask 0x08, extrwi r,1,26): isActive
     *         bit 3 (mask 0x10, extrwi r,1,27): isFading
     */
    /* 0x01 */ u8  pad_01;
    /* 0x02 */ u8  groupIndex;
    /* 0x03 */ u8  fileInfoIndex; /* index into file_info table */
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u16 pad_06;
    /* 0x08 */ void* soundData;  /* pointer to sound resource data */
} SndWork;

/* -----------------------------------------------------------------------
 * Sound resource data -- pointed to by SndWork::soundData (variable size)
 * ----------------------------------------------------------------------- */
typedef struct SndResData {
    /* 0x00 */ u8  field_00;
    /* 0x01 */ u8  maxVolume;      /* maximum volume for this resource */
    /* 0x02 */ u8  pad_02;
    /* 0x03 */ u8  pad_03;
    /* 0x04 */ u8  volQueueCount;  /* number of queued volume entries */
    /* 0x05 */ u8  volQueue[3];    /* queued volume values */
    /* 0x08 */ u32 handle;         /* JAudio playback handle */
} SndResData;

/* -----------------------------------------------------------------------
 * 3D sound position vector (matches GS engine Vec3 layout)
 * ----------------------------------------------------------------------- */
typedef struct SndVec {
    f32 x, y, z;
} SndVec;

/* -----------------------------------------------------------------------
 * Sound listener parameters for 3D audio
 * (0x4C bytes, allocated per-listener)
 * ----------------------------------------------------------------------- */
typedef struct SndListener {
    /* 0x00 */ u8  field_00;
    /* 0x01 */ u8  volume;       /* listener local volume */
    /* 0x02 */ u8  field_02;
    /* 0x03 */ u8  pad_03;
    /* 0x04 */ u32 sndWorkIndex; /* which SndWork this is bound to */
    /* 0x08 */ f32 falloff;      /* distance attenuation factor */
    /* 0x0C */ f32 maxDist;      /* max audible distance */
    /* 0x10 */ SndVec pos;       /* listener position */
    /* 0x1C */ SndVec dir;       /* listener facing direction */
    /* 0x28 */ u8  jAudioParams[0x24]; /* JAudio-specific state */
} SndListener;

/* -----------------------------------------------------------------------
 * Global state variables (in sbss, SDA-accessible)
 * ----------------------------------------------------------------------- */

/* lbl_80478E30: pointer to file_info count structure */
extern void* g_sndFileInfoCount;
/* lbl_80478E34: pointer to file_info data array (8 bytes per entry) */
extern void* g_sndFileInfoData;

/* lbl_80478FA4: (unused/reserved) */
/* lbl_80478FA8: pointer to sound system master state */
extern void* g_sndMasterState;
/* lbl_80478FAC: pointer to SndWork array base */
extern SndWork* g_sndWorkTable;
/* lbl_80478FB0: pointer to WAVE resource count */
extern void* g_sndWaveCount;
/* lbl_80478FB4: pointer to WAVE resource data array (0x18 bytes per entry) */
extern void* g_sndWaveData;

/* lbl_8047B0A0 - lbl_8047B0A4: (reserved) */
/* lbl_8047B0A8: current BGM sound ID (SE category) */
extern u32 g_currentBgmId;
/* lbl_8047B0AC: current BGM sound ID (streaming category) */
extern u32 g_currentStreamBgmId;
/* lbl_8047B0B0: streaming buffer size */
extern u32 g_streamBufferSize;
/* lbl_8047B0B4: streaming buffer pointer */
extern void* g_streamBuffer;

/* lbl_8047B0B8 - lbl_8047B0BC: (reserved) */
/* lbl_8047B0C0: total sound buffer allocation size */
extern u32 g_sndTotalAllocSize;
/* lbl_8047B0C4: 3D listener pool pointer */
extern void* g_sndListenerPool;
/* lbl_8047B0C8: max 3D listener count */
extern u32 g_sndListenerMax;
/* lbl_8047B0CC: SE work pool pointer */
extern void* g_sndSePool;
/* lbl_8047B0D0: max SE count */
extern u32 g_sndSeMax;
/* lbl_8047B0D4: BGM work pool pointer */
extern void* g_sndBgmPool;
/* lbl_8047B0D8: max BGM count */
extern u32 g_sndBgmMax;
/* lbl_8047B0DC: master allocation block pointer */
extern void* g_sndMasterAlloc;
/* lbl_8047B0E0: total sound work entry allocation */
extern u32 g_sndWorkAllocSize;
/* lbl_8047B0E4: current group ID (-1 = none) */
extern s32 g_sndCurrentGroup;
/* lbl_8047B0E8: number of SndWork entries */
extern u32 g_sndWorkCount;

/* -----------------------------------------------------------------------
 * Sound system core API (sound.c)
 * ----------------------------------------------------------------------- */

/**
 * Initialize the sound system.
 *
 * Allocates all sound work tables, sets up JAudio, configures the
 * master volume, and registers the sound update callback.
 *
 * @param numBgm     Number of BGM slots to allocate.
 * @param numSe      Number of SE work entries.
 * @param numBgmRes  Number of BGM resource slots.
 * @param numSe3d    Number of 3D SE listener slots.
 * @param numStreams  Number of streaming channels.
 * @return TRUE on success, FALSE on failure.
 *
 * Address: 0x80166E88 (fn_80166E88), size 0x1B8
 */
BOOL sndInit(u32 numBgm, u32 numSe, u32 numBgmRes, u32 numSe3d, u32 numStreams);

/**
 * Shut down the sound system.
 *
 * Stops all playing sounds, releases JAudio resources, frees all
 * allocated sound work tables and streaming buffers.
 *
 * Address: 0x80165F84 (fn_80165F84), size 0x58
 */
void sndShutdown(void);

/**
 * Open a WAVE resource by numeric ID.
 *
 * Looks up the wave data in the WAVE resource table and prepares it
 * for playback. Prints "ERROR: can't open WAVE ID = %d" on failure.
 *
 * @param waveId  Index into the WAVE resource table.
 *
 * Address: 0x801655D4 (fn_801655D4), size 0x94
 */
void sndWaveOpen(u32 waveId);

/**
 * Open a WAVE resource by filename.
 *
 * Searches the FSYS archive for the named file, reads it from disc,
 * and prepares it for streaming playback.
 * Prints "ERROR: can't open WAVE File = %s" on failure.
 *
 * @param filename  Path to the WAVE file within the FSYS archive.
 * @param buffer    Destination buffer for the wave data.
 * @param bufSize   Size of the destination buffer.
 *
 * Address: 0x80165DEC (fn_80165DEC), size 0xF4
 */
void sndWaveOpenByName(const char* filename, void* buffer, u32 bufSize);

/**
 * Load streaming wave data into the provided buffer.
 *
 * Initializes a memset(0xE0) on the buffer, stores it as the active
 * streaming buffer, and triggers an FSYS load request.
 *
 * Address: 0x80165EE0 (fn_80165EE0), size 0x60
 */
void sndStreamLoad(void* waveData, void* buffer, u32 bufSize);

/* -----------------------------------------------------------------------
 * BGM playback API (sound_bgm.c)
 * ----------------------------------------------------------------------- */

/**
 * Play a sound by ID. Handles both BGM and SE based on the file_info
 * entry's type flag.
 *
 * For BGM: stops the current BGM with a crossfade, then starts the new one.
 * For SE: allocates a new SE slot and begins playback.
 *
 * @param sndId     Sound ID to play.
 * @param fadeTime  Fade-in time (ignored for SE).
 * @param volume    Playback volume (0-127).
 * @param isBgm     If nonzero, treat as streaming BGM.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x80165A44 (fn_80165A44), size 0x22C
 */
u32 sndCheckFileInfo(u32 sndId, u32 fadeTime, u32 volume, u32 isBgm);

/**
 * Play BGM by ID (non-streaming category).
 *
 * Wrapper: calls sndCheckFileInfo with isBgm=0 for non-streaming BGM.
 *
 * Address: 0x801659FC (fn_801659FC), size 0x24
 */
u32 sndPlayBgm(u32 sndId, u32 fadeTime, u32 volume);

/**
 * Play BGM by ID (streaming category).
 *
 * Wrapper: calls sndCheckFileInfo with isBgm=1 for streaming BGM.
 *
 * Address: 0x80165A20 (fn_80165A20), size 0x24
 */
u32 sndPlayBgmStream(u32 sndId, u32 fadeTime, u32 volume);

/**
 * Play a sound with crossfade from the current BGM.
 *
 * Stops the current BGM (non-streaming), then starts the new sound.
 *
 * @param sndId     Sound ID to play.
 * @param crossfade Crossfade BGM ID.
 * @param fadeTime  Fade time.
 * @param volume    Volume level.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x801658FC (fn_801658FC), size 0x80
 */
u32 sndPlayWithCrossfade(u32 sndId, u32 crossfade, u32 fadeTime, u32 volume);

/**
 * Play a sound with crossfade from the current streaming BGM.
 *
 * Same as sndPlayWithCrossfade but for the streaming category.
 *
 * Address: 0x8016597C (fn_8016597C), size 0x80
 */
u32 sndPlayStreamWithCrossfade(u32 sndId, u32 crossfade, u32 fadeTime, u32 volume);

/**
 * Stop a sound by its handle.
 *
 * If the sound is a BGM that cannot be stopped immediately, prints
 * "soundStop: Warning! BGM cannot be stopped.(snd_id=%d)".
 * Handles cleanup of the current BGM ID tracking.
 *
 * @param handle  Sound handle (from g_currentBgmId or similar).
 * @param sndId   Sound ID for the warning message.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x801657F8 (soundStop), size 0x104
 */
u32 sndStop(u32 handle, u32 sndId);

/**
 * Stop a sound by ID, using the current BGM handle.
 *
 * Convenience wrapper: loads g_currentBgmId, calls sndStop.
 *
 * Address: 0x801657D0 (fn_801657D0), size 0x28
 */
void sndStopBgm(u32 sndId);

/**
 * Get the current non-streaming BGM sound ID.
 *
 * Address: 0x801653BC (fn_801653BC), size 0x8
 */
u32 sndGetCurrentBgmId(void);

/**
 * Get the current streaming BGM sound ID.
 *
 * Address: 0x801653C4 (fn_801653C4), size 0x8
 */
u32 sndGetCurrentStreamBgmId(void);

/**
 * Get the playback status of a sound.
 *
 * Checks the JAudio handle state. If a sound is stuck, forces
 * termination and prints the appropriate warning message.
 *
 * @param sndId  Sound work index to query.
 * @return Status code: 0=stopped, 1=initializing, 2=playing, 3=fading.
 *
 * Address: 0x801666BC (fn_801666BC), size 0x11C
 */
s32 GSsndGetStatus(u32 sndId);

/* -----------------------------------------------------------------------
 * Sound effect playback API (sound_se.c)
 * ----------------------------------------------------------------------- */

/**
 * Play a sound effect at the default listener position.
 *
 * Allocates a listener, sets up default 3D parameters, and starts
 * playback through JAudio.
 *
 * @param listenerParams  3D audio listener parameters.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x801664F0 (fn_801664F0), size 0x88
 */
s32 sndPlaySe(void* listenerParams);

/**
 * Play a sound effect at a specific 3D position.
 *
 * @param sndId           Sound work index.
 * @param listenerParams  3D audio listener parameters.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x80166308 (fn_80166308), size 0x68
 */
s32 sndPlaySe3D(u32 sndId, void* listenerParams);

/**
 * Play a sound effect with full 3D parameters.
 *
 * @param sndId      Sound work index.
 * @param pos        3D position.
 * @param dir        3D direction.
 * @param innerAngle Inner cone angle.
 * @param outerAngle Outer cone angle (0 = omnidirectional).
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x80166370 (fn_80166370), size 0xE8
 */
s32 sndPlaySe3DFull(u32 sndId, void* pos, void* dir, u8 volume, u8 pan);

/**
 * Play a sound effect with full 4-point 3D parameters.
 *
 * @param sndId       Sound work index.
 * @param pos         Position vector.
 * @param fwd         Forward direction.
 * @param up          Up direction.
 * @param right       Right direction.
 * @param volume      Volume level.
 * @return Nonzero on success, zero on failure.
 *
 * Address: 0x80166578 (fn_80166578), size 0xF8
 */
s32 sndPlaySe3D4Point(u32 sndId, void* pos, void* fwd, void* up, void* right, u8 volume);

/**
 * Update a playing sound effect's 3D position.
 *
 * @param listener  Listener to update.
 * @param pos       New 3D position.
 * @return TRUE if update succeeded.
 *
 * Address: 0x80166458 (fn_80166458), size 0x98
 */
BOOL sndUpdateSe3D(SndListener* listener, void* pos);

/**
 * Play a BGM-triggered sound effect (used when entering new areas).
 *
 * Iterates all active sound work entries and triggers queued volume
 * changes from the SndResData volume queue.
 *
 * @param volume    New volume level.
 * @param playSe    If TRUE, trigger SE sounds; if FALSE, trigger BGM.
 * @param playBgm   If TRUE, also trigger BGM sounds.
 *
 * Address: 0x801660D8 (fn_801660D8), size 0x90
 */
void sndTriggerVolumeChange(u32 volume, BOOL playSe, BOOL playBgm);

/**
 * Queue a volume change for a specific sound work entry.
 *
 * @param sndId   Sound work index.
 * @param volume  New volume level.
 * @param playSe  SE flag.
 * @param playBgm BGM flag.
 *
 * Address: 0x801661D0 (fn_801661D0), size 0x98
 */
void sndQueueVolumeChange(u32 volume, u32 fadeTime, u32 playSe, u32 playBgm);

/* -----------------------------------------------------------------------
 * Volume control API
 * ----------------------------------------------------------------------- */

/**
 * Set the master volume for all sound channels.
 *
 * @param volume    Volume level (0-127).
 * @param panValue  Pan value (0-0xFFFF).
 * @param applyBgm  Apply to BGM (TRUE/FALSE).
 * @param applySe   Apply to SE (TRUE/FALSE).
 *
 * Address: 0x80166D48 (fn_80166D48), size 0xFC
 */
void sndSetMasterVolume(u32 volume, u32 panValue, BOOL applyBgm, BOOL applySe);

/**
 * Set the master volume with BGM and SE combined.
 *
 * Convenience wrapper: calls sndSetMasterVolume with apply flags set to
 * both BGM=1 and SE=0 (quick BGM-only volume set).
 *
 * Address: 0x80165718 (fn_80165718), size 0x2C
 */
BOOL sndSetMasterVolumeBgm(u32 volume, u32 panValue);

/**
 * Check if audio output is available (disc is readable).
 *
 * @return TRUE if audio output is active, FALSE otherwise.
 *
 * Address: 0x80166C74 (GSsndGetOutputMode), size 0x4C
 */
BOOL sndIsAudioAvailable(void);

/**
 * Configure surround sound mode (mono/stereo/surround).
 *
 * @param mode  0=mono, 1=stereo, 2=surround.
 *
 * Address: 0x80166CC0 (fn_80166CC0), size 0x58
 */
void sndSetSurroundMode(s32 mode);

/**
 * Set DSP (Digital Signal Processor) mix parameters.
 *
 * @param chorus  Chorus effect level.
 * @param reverb  Reverb send level.
 * @param delay   Delay effect level.
 * @param wet     Wet/dry mix ratio.
 *
 * Address: 0x80166D18 (fn_80166D18), size 0x30
 */
void sndSetDspMix(u8 chorus, u16 reverb, u8 delay, u8 wet);

/**
 * Auto-apply master volume on disc activity pause.
 *
 * Called when the disc drive pauses (e.g., during load screens).
 * Temporarily adjusts volume to prevent audio glitches.
 *
 * Address: 0x80166E44 (fn_80166E44), size 0x44
 */
void sndAutoVolumeOnDiscPause(void);

/* -----------------------------------------------------------------------
 * Internal helpers (not part of public API)
 * ----------------------------------------------------------------------- */

/**
 * Fade the currently playing BGM to a target volume over time.
 *
 * @param fadeTimeMs  Fade duration in milliseconds.
 *
 * Address: 0x80165D0C (fn_80165D0C), size 0xE0
 */
void _sndFadeBgm(u32 fadeTimeMs);

/**
 * Find the currently playing BGM handle for a given category.
 *
 * @param channel    Channel index (0 = default).
 * @param category   JAudio category.
 * @return Sound handle, or SND_HANDLE_INVALID if none playing.
 *
 * Address: 0x801662E8 (fn_801662E8), size 0x20
 */
u32 _sndFindCurrentHandle(u32 channel, u32 category);

/**
 * Stop and clean up a BGM crossfade transition.
 *
 * @param sndId     Sound ID being crossfaded out.
 * @param fadeTime  Fade-out time in frames.
 * @param wait      If TRUE, block until fade completes.
 *
 * Address: 0x80165C70 (fn_80165C70), size 0x9C
 */
void _sndCrossfadeOut(u32 sndId, u32 fadeTime, BOOL wait);

/**
 * Wait for a sound group to finish loading from disc.
 *
 * Polls the FSYS load status in a loop, printing
 * "ERROR: Can't Read Group(%d)" on failure.
 *
 * @param groupId  Sound group ID.
 *
 * Address: 0x80165FDC (fn_80165FDC), size 0x70
 */
void _sndWaitGroupLoad(u32 groupId);

/**
 * Check all sound work entries and clean up finished sounds.
 *
 * Prints "_sndCheckSndWorkALL:Start" / "End" debug markers.
 *
 * Address: 0x80166670 (fn_80166670), size 0x4C
 */
u32 _sndCheckSndWorkAll(u32 sndId, u32 fadeTime, u32 flags);

/**
 * Mark a sound work entry as complete and release resources.
 *
 * @param sndId  Sound work index.
 *
 * Address: 0x80166B18 (fn_80166B18), size 0x24
 */
void _sndReleaseWork(u32 sndId);

/**
 * Check if a sound work entry is flagged as BGM.
 *
 * @param sndId  Sound work index.
 * @return TRUE if the entry is a BGM type.
 *
 * Address: 0x80166084 (fn_80166084), size 0x14
 */
BOOL _sndIsBgm(u32 sndId);

/**
 * Get the volume level of a sound work entry's resource data.
 *
 * @param sndId  Sound work index.
 * @return Volume level (0-127), or 0 if not active.
 *
 * Address: 0x80166098 (fn_80166098), size 0x40
 */
u32 _sndGetWorkVolume(u32 sndId);

/**
 * Prepare a sound for playback by loading its associated
 * streaming data into memory.
 *
 * @param sndId    Sound work index.
 * @param fadeTime Fade-in time.
 * @param volume   Target volume.
 * @return TRUE if the sound began loading, FALSE on error.
 *
 * Address: 0x801653CC (fn_801653CC), size 0x114
 */
BOOL _sndPrepareStream(u32 sndId, u32 fadeTime, u32 volume);

/**
 * Stop the currently playing non-streaming BGM and wait for it
 * to fully stop (crossfade out with 50ms fade).
 *
 * @return The old BGM handle (SND_HANDLE_INVALID if nothing was playing).
 *
 * Address: 0x8016557C (fn_8016557C), size 0x58
 */
u32 _sndStopCurrentBgm(void);

/**
 * Helper to play a sound with a combined start/seek operation
 * after stopping the previous BGM.
 *
 * @param sndId    Sound work index.
 * @param fadeTime Fade-in time.
 * @param volume   Target volume.
 *
 * Address: 0x80165668 (fn_80165668), size 0x70
 */
u32 _sndPlayAfterStop(u32 sndId, u32 fadeTime, u32 volume);

/**
 * Stop all sounds (both BGM and SE), used during scene transitions.
 *
 * Address: 0x801656D8 (fn_801656D8), size 0x20
 */
void sndStopAll(void);

/**
 * Stop all SE-category sounds (keep BGM playing).
 *
 * Address: 0x801656F8 (fn_801656F8), size 0x20
 */
void sndStopAllSe(void);

/**
 * Initialize streaming BGM playback buffers and start an FSYS load.
 *
 * Address: 0x80165F40 (fn_80165F40), size 0x44
 */
void _sndInitStreamBuffers(void);

/**
 * Allocate a memory block from the sound heap.
 *
 * @param size  Requested size in bytes.
 * @return Pointer to the allocated block, or NULL on failure.
 *
 * Address: 0x8016604C (fn_8016604C), size 0x38
 */
void* _sndAllocBlock(u32 size);

/**
 * Perform a BGM key change (transpose) or re-trigger.
 *
 * @param sndId      Sound work index.
 * @param transpose  Half-step transposition value.
 * @return TRUE on success.
 *
 * Address: 0x801667D8 (fn_801667D8), size 0x104
 */
BOOL _sndBgmTranspose(u32 sndId, u16 transpose);

/**
 * Perform a SE key change (transpose) or re-trigger.
 *
 * @param sndId      Sound work index.
 * @param transpose  Half-step transposition value.
 * @param volume     New volume.
 * @return TRUE on success.
 *
 * Address: 0x801668DC (fn_801668DC), size 0xE0
 */
BOOL _sndSeTranspose(u32 sndId, u32 transpose, u32 volume);

/**
 * Change the group used for sound loading.
 *
 * @param groupId  New group ID (0 = no change).
 *
 * Address: 0x80166C34 (fn_80166C34), size 0x40
 */
void _sndChangeGroup(u32 groupId);

/**
 * Update volume/pan for all active sound work entries.
 *
 * Called per-frame as part of the sound system update tick.
 *
 * Address: 0x801652DC (fn_801652DC), size 0xE0
 */
void _sndUpdateAllVolumes(u32 bgmHandle, u32 seHandle, u32 streamHandle, void* params);

#endif /* GAME_SOUND_H */
