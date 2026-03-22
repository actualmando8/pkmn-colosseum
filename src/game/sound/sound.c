/**
 * @file sound.c
 * @brief Core sound system initialization, shutdown, and wave loading.
 *
 * This module handles the fundamental lifecycle of the sound system:
 *   - sndInit: allocates work tables, sets up JAudio, registers callbacks
 *   - sndShutdown: stops all sounds, frees resources
 *   - sndWaveOpen / sndWaveOpenByName: load wave resources from disc
 *   - sndStreamLoad: prepare streaming buffers for disc-based audio
 *
 * The sound system uses two SDA-resident global pointers:
 *   g_sndMasterState  (lbl_80478FA8) -- points to the master JAudio state
 *   g_sndWorkTable    (lbl_80478FAC) -- base of the SndWork array
 *
 * Each SndWork entry is 0x0C bytes and tracks one sound resource:
 *   byte 0: flags (isBGM, isActive, isFading)
 *   byte 3: file_info index (into the wave bank)
 *   word 8: pointer to loaded sound data
 *
 * Address range: 0x801652DC - 0x80167040
 * Source file:   sound.c
 */

#include "game/sound/sound.h"

/* =========================================================================
 * External engine / SDK functions used by this module.
 * Names prefixed with fn_ are auto-generated from the decomp symbols.
 * =========================================================================
 */

/* GS debug print (OSReport-like) */
extern void fn_800DD970(const char* fmt, ...);

/* GS engine timing: wait one frame */
extern void fn_800F0308(void);

/* GS engine tick timers (for fade timing) */
extern s32 fn_800D37CC(void);
extern s32 fn_800D3088(void);

/* GSmem: allocate block from engine heap */
extern void* fn_800E2C04(u32 size, u32 align);
extern void* fn_800E27B0(void* block); /* get usable address from alloc handle */

/* GS Vec3 init / copy */
extern void fn_800E01F4(void* dst, f32 x, f32 y, f32 z);
extern void fn_800E01D0(void* dst, void* src);
extern void fn_800E0168(void* dst, void* src, void* offset);

/* JAudio: allocate channel */
extern u32 fn_800F9318(u32 channel, u32 category);
/* JAudio: release channel */
extern void fn_800F9378(u32 handle, u32 unused, u32 category, u32 flags);
/* JAudio: free channel resources */
extern void fn_800F9210(u32 channel, u32 category);
/* JAudio: start playback */
extern BOOL fn_800F9418(u32 bufSize, u32 align, u32 unused, u32 category,
                        u32 flags);

/* JAudio: low-level controls */
extern void fn_8014D598(u32 handle);
extern BOOL fn_8014D5C8(u32 handle);
extern void fn_8014D648(u32 handle);
extern void fn_8014D6D8(u32 channel, u16 value, u32 handle, u32 flags);
extern void fn_8014D880(u32 handle);
extern void fn_8014D8C0(void* callback);
extern void fn_8014D8C8(u8 volume, u16 pan, u8 code);
extern void fn_8014D928(u8 chorus, u16 reverb, u8 delay, u8 wet);
extern void fn_8014D9BC(void);
extern void fn_8014DAA8(u32 channel, void* callback, void* userData,
                        u32 volume, u32 pan, u32 r8, u32 r9, u32 r10);

/* libc */
extern void* memset(void* dst, int val, u32 size);

/* FSYS: load by ID / read */
extern void fn_8017AF6C(u32 fsysId, void* buffer);
extern s32  fn_8017B2CC(u32 fsysId);
extern void fn_8017B370(u32 fsysId);

/* Disc drive status */
extern s32 fn_800A0E34(void);
extern void fn_800A0EB4(u32 enable);

/* Sound 3D / listener layer (in the 0x8015E-0x80160 range) */
extern void fn_801631AC(void* params);
extern s32  fn_8015FE88(u32 maxVoices, u32 priority, u32 maxStreams,
                        u32 flag, u32 r7, u32 stackSize);
extern BOOL fn_8015FFA0(void);
extern BOOL fn_8015FFD4(void);
extern void fn_8015ECA8(void* dst, void* pos, void* dir, u32 innerAngle,
                        u16 pad, u8 volume, u32 pan, u32 flags);
extern BOOL fn_8015E890(void* jAudioParams);
extern void fn_8015ED00(void* dst, void* pos, void* srcPos,
                        void* srcDir, void* srcUp, u8 volume, u32 flags);
extern void fn_8015EF04(void* dst, void* pos, void* fwd, void* up,
                        void* right, u8 isOmni, u8 volume, u32 pan, u32 flags);

/* Sound internal helpers (in later part of sound.c / adjacent modules) */
extern void fn_80166B3C(u32 sndId, u32 unused, u32 category); /* _sndBindWork */
extern BOOL fn_80166BE0(u32 sndId, u32 flag, void* name,
                        u32 r6, u32 r7, u32 r8, u32 r9); /* _sndOpenWaveInternal */
extern void fn_80166670(u32 sndId, u32 fadeTime, u32 flags); /* _sndCheckSndWorkAll */
extern void fn_80166084(u32 sndId);  /* _sndIsBgm (returns bool in r3) */
extern void fn_80166098(u32 sndId);  /* _sndGetWorkVolume */
extern void fn_801662E8(u32 channel, u32 category); /* _sndFindCurrentHandle */
extern void fn_80166A50(u32 sndId, u32 fadeTime, u32 volume, u32 flags); /* _sndStartPlayback */
extern void fn_80166AB8(u32 sndId, u32 fadeTime, u32 volume); /* _sndStartPlaybackInternal */
extern void fn_80166B18(u32 sndId);  /* _sndReleaseWork */
extern void fn_801669E4(u32 sndId, u32 fadeTime, u32 volume); /* _sndSetFadeTarget */
extern void fn_801669BC(u32 sndId);  /* _sndSetFadeTargetDefault */
extern void fn_80166A28(u32 sndId);  /* _sndSetFadeTargetDefault2 */
extern void fn_801666BC(u32 sndId);  /* GSsndGetStatus */
extern void fn_801667D8(u32 sndId, u16 transpose); /* _sndBgmTranspose */
extern void fn_801668DC(u32 sndId, u32 transpose, u32 volume); /* _sndSeTranspose */
extern void fn_80166168(u32 sndId, u32 volume); /* _sndTriggerVolumeEntry */
extern void fn_80166268(u32 sndId, u32 volume, u32 fadeTime); /* _sndQueueVolumeEntry */
extern s32 fn_80166308(u32 sndId, void* pos); /* sndPlaySe3D */
extern void fn_80166370(u32 sndId, void* pos, void* dir,
                        u8 volume, u8 pan); /* sndPlaySe3DFull */
extern s32 fn_801664F0(void* listenerParams); /* sndPlaySe */
extern void fn_80166578(u32 sndId, void* pos, void* fwd,
                        void* up, void* right, u8 volume); /* sndPlaySe3D4Point */

/* Sound internal memory management */
extern void* fn_80167BB0(u32 size);  /* _sndAllocFromMaster */
extern void  fn_80167A6C(void);      /* _sndInitBgmPool */
extern void  fn_80167A44(void);      /* _sndInitSePool */
extern void  fn_80167A14(void);      /* _sndInitListenerPool */
extern void  fn_80167A9C(u32 groupId); /* _sndLoadGroup */
extern void  fn_801644E0(void* buffer); /* _sndSetTableBuffer */
extern void  fn_80164488(void* buffer); /* _sndLoadTableFromBuffer */

/* Sound internal: group/work management */
extern void  fn_80167070(u32 sndId, u32 mode); /* _sndCleanupWork */
extern void  fn_80167118(u32 sndId, u32 flag, u32 name,
                         u32 r6, u32 r7, u32 r8, u32 r9,
                         u32 r10); /* _sndOpenWaveWorker */
extern void  fn_80167318(void);     /* _sndFlushAllWork */
extern s32   fn_80167408(u32 sndId, u32 volume); /* _sndSetVolume */
extern void  fn_80167490(u32 sndId, u32 fadeTime, u32 volume); /* _sndFadeBgmInternal */
extern void  fn_80167508(u32 sndId, u32 fadeTime, u32 volume); /* _sndFadeSeInternal */
extern void  fn_8016758C(u32 sndId, u32 r4); /* _sndFadeBgmApply */
extern void  fn_8016761C(u32 sndId, u32 fadeTime, u32 volume); /* _sndFadeSeApply */
extern void  fn_80167768(u32 channel, u32 category); /* _sndFindHandleInternal */
extern void  fn_801677BC(void* listener); /* _sndReleaseListener */
extern void  fn_801677F4(void* listener); /* _sndReleaseListener2 */
extern void  fn_80167864(void);          /* _sndAllocListener */
extern void  fn_801678E4(void);          /* _sndAllocListener4Point */
extern void  fn_80167964(void);          /* _sndAllocSeSlot */
extern void  fn_801679E4(void);          /* _sndInitListenerDefaults */
extern void  fn_80167AF0(void* work, void* slot); /* _sndBindSlotToWork */
extern u32   fn_80167E5C(void* data);   /* _sndGetStreamLength */
extern void  fn_80167E64(void* data);   /* _sndReleaseStreamHandle */
extern s32   fn_80167ED0(void* handle, void* buffer, u32 size,
                         u32 offset);   /* _sndReadStreamData */
extern void* fn_80167F28(void);         /* _sndOpenStreamFile */

/* Sound update callback (registered with JAudio) */
extern void fn_80167040(void); /* _sndUpdateCallback */

/* Volume update function */
extern void fn_80164C40(u32 handle, u32 seHandle, f32 f1, f32 f2);
extern void fn_80164DD0(u32 handle, void* params, u32 channel);

/* =========================================================================
 * Rodata string references (used for error/debug prints)
 * =========================================================================
 */
/* lbl_80273548: "ERROR: can't open WAVE ID = %d\n" */
/* lbl_80273568: "ERROR: can't open WAVE File = %s\n" */
/* lbl_8027358C: "soundStop: Warning! BGM cannot be stopped.(snd_id=%d)\n" */
/* lbl_802735C4: "ERROR(sound.c): invalid file_info number snd_id=%d\n" */
/* lbl_802735F8: "ERROR(sound.c): Can't Play Sound. snd_id=%d\n"
 *               "ERROR: Unable to open '%s'\n"
 *               "ERROR: Zero length file '%s'\n"
 *               "ERROR: Unable to allocate buffer\n"
 *               "ERROR: Failed to read data from '%s'\n"
 *               "ERROR: Over Sound Buffer\n"                       */
/* lbl_802736CC: "ERROR: Can't Read Group(%d)\n\n" */
/* lbl_802736F0: "GSsndGetStatus:Forced termination SE=%d\n" */
/* lbl_8027371C: "GSsndGetStatus:Forced termination BGM=%d\n" */
/* lbl_80273748: "_sndCheckSndWorkALL:Start\n" */
/* lbl_80273764: "_sndCheckSndWorkALL:End\n" */

/* Data table (in .data section) */
/* lbl_80452500: sound table buffer (used for BGM index / sound bank) */

/* =========================================================================
 * SDA-resident global variables (in .sbss / .sbss2)
 * =========================================================================
 */

/* lbl_80478E30 */ void*    g_sndFileInfoCount;     /* -> { u32 count; } */
/* lbl_80478E34 */ void*    g_sndFileInfoData;      /* -> wave data ptrs, 8 bytes each */

/* lbl_80478FA8 */ void*    g_sndMasterState;       /* -> { u32 maxSndId; ... } */
/* lbl_80478FAC */ SndWork* g_sndWorkTable;         /* base of SndWork[g_sndWorkCount] */
/* lbl_80478FB0 */ void*    g_sndWaveCount;         /* -> { u32 count; } */
/* lbl_80478FB4 */ void*    g_sndWaveData;          /* -> WaveRes[count], 0x18 each */

/* lbl_8047B0A8 */ u32      g_currentBgmId;         /* active non-stream BGM snd_id */
/* lbl_8047B0AC */ u32      g_currentStreamBgmId;   /* active streaming BGM snd_id */
/* lbl_8047B0B0 */ u32      g_streamBufferSize;     /* streaming buffer capacity */
/* lbl_8047B0B4 */ void*    g_streamBuffer;          /* streaming buffer pointer */

/* lbl_8047B0C0 */ u32      g_sndTotalAllocSize;
/* lbl_8047B0C4 */ void*    g_sndListenerPool;
/* lbl_8047B0C8 */ u32      g_sndListenerMax;
/* lbl_8047B0CC */ void*    g_sndSePool;
/* lbl_8047B0D0 */ u32      g_sndSeMax;
/* lbl_8047B0D4 */ void*    g_sndBgmPool;
/* lbl_8047B0D8 */ u32      g_sndBgmMax;
/* lbl_8047B0DC */ void*    g_sndMasterAlloc;
/* lbl_8047B0E0 */ u32      g_sndWorkAllocSize;
/* lbl_8047B0E4 */ s32      g_sndCurrentGroup;
/* lbl_8047B0E8 */ u32      g_sndWorkCount;

/* =========================================================================
 * fn_801655D4 -- sndWaveOpen
 *
 * Opens a wave resource by numeric ID from the wave data table.
 * Each wave entry is 0x18 bytes. The wave name string is at offset 0x14.
 *
 * r3 = waveId
 * =========================================================================
 */
void sndWaveOpen(u32 waveId) {
    void* waveCountPtr;
    u32 maxWaves;
    u8* waveEntry;

    waveCountPtr = g_sndWaveCount; /* lbl_80478FB0 */
    maxWaves = *(u32*)waveCountPtr;

    if (waveId >= maxWaves) {
        fn_800DD970("ERROR: can't open WAVE ID = %d\n", waveId);
    }

    /* Compute pointer to wave entry: base + waveId * 0x18 */
    waveEntry = (u8*)g_sndWaveData + (waveId * 0x18);

    /* Call internal open with the wave's name string and parameters */
    if (!fn_80166BE0(waveId, 1, (void*)*(u32*)(waveEntry + 0x14),
                     *(u32*)(waveEntry + 0x04),
                     *(u32*)(waveEntry + 0x08),
                     *(u32*)(waveEntry + 0x0C),
                     *(u32*)(waveEntry + 0x10))) {
        fn_800DD970("ERROR: can't open WAVE File = %s\n",
                    (void*)*(u32*)(waveEntry + 0x14));
    }
}

/* =========================================================================
 * fn_80165668 -- _sndPlayAfterStop
 *
 * Stops the currently playing BGM, prepares a new sound for streaming,
 * then seeks to the beginning.
 *
 * r3 = sndId, r4 = fadeTime, r5 = volume
 * =========================================================================
 */
u32 _sndPlayAfterStop(u32 sndId, u32 fadeTime, u32 volume) {
    u32 oldHandle;

    oldHandle = _sndStopCurrentBgm();  /* fn_8016557C */
    _sndPrepareStream(sndId, fadeTime, volume); /* fn_801653CC */

    /* Seek the old handle to ensure clean stop */
    if (oldHandle + 0x10000 != 0xFFFF) {
        _sndSeTranspose(oldHandle, 0x3E8, 0); /* fn_801668DC: 1000ms transpose */
    }

    return oldHandle;
}

/* =========================================================================
 * fn_801656D8 -- sndStopAll
 *
 * Stops all sounds (both BGM and SE).
 * Thin wrapper around fn_80166098.
 * =========================================================================
 */
void sndStopAll(void) {
    fn_80166098(0); /* stop all categories */
}

/* =========================================================================
 * fn_801656F8 -- sndStopAllSe
 *
 * Stops all SE-category sounds, leaving BGM playing.
 * Thin wrapper around fn_80166670.
 * =========================================================================
 */
void sndStopAllSe(void) {
    fn_80166670(0, 0, 0); /* stop SE only */
}

/* =========================================================================
 * fn_80165718 -- sndSetMasterVolumeBgm
 *
 * Sets the master volume with BGM apply flag.
 * Returns TRUE always.
 *
 * r3 = volume, r4 = panValue
 * =========================================================================
 */
BOOL sndSetMasterVolumeBgm(u32 volume, u32 panValue) {
    sndSetMasterVolume(volume, panValue, TRUE, FALSE);
    return TRUE;
}

/* =========================================================================
 * fn_80165744 -- sndPlaySe (default position)
 *
 * Plays a sound effect at the origin (0,0,0) with default listener params.
 *
 * r3 = listenerParams (output struct on stack)
 * =========================================================================
 */
s32 sndPlaySe(void* listenerParams) {
    SndVec defaultPos;
    s32 result;

    /* f1 = lbl_8047D540 (1.0f) */
    fn_800E01F4(&defaultPos, 1.0f, 1.0f, 1.0f);
    result = fn_801664F0(&defaultPos);

    /* Convert: result = (result == 0) ? 1 : 0 -- negate and shift sign bit */
    return (result | (-result)) < 0 ? 0 : 1;
}

/* =========================================================================
 * fn_80165788 -- sndPlaySe3D
 *
 * Plays a sound effect at a specific 3D position.
 *
 * r3 = sndId, r4 = listenerParams
 * =========================================================================
 */
s32 sndPlaySe3D(u32 sndId, void* listenerParams) {
    SndVec pos;
    s32 result;

    fn_800E01F4(&pos, 1.0f, 1.0f, 1.0f);
    result = fn_80166308(sndId, &pos);

    return (result | (-result)) < 0 ? 0 : 1;
}

/* =========================================================================
 * fn_801657D0 -- sndStopBgm
 *
 * Stops a BGM by ID, loading the current BGM handle first.
 *
 * r3 = sndId
 * =========================================================================
 */
void sndStopBgm(u32 sndId) {
    sndStop(g_currentBgmId, sndId);
}

/* =========================================================================
 * fn_801657F8 -- sndStop
 *
 * Stops a sound by its handle. Handles BGM/SE distinction, cleanup
 * of the global current BGM tracking, and prints warnings when a BGM
 * cannot be stopped (e.g., during a fade).
 *
 * r3 = handle, r4 = sndId
 * =========================================================================
 */
u32 sndStop(u32 handle, u32 sndId) {
    void* masterState;
    u32 maxSndId;
    u32 wasBgm;

    /* Null-check and bounds-check the handle */
    if (handle == 0) {
        return 0;
    }
    masterState = g_sndMasterState; /* lbl_80478FA8 */
    maxSndId = *(u32*)masterState;
    if (handle >= maxSndId) {
        return 0;
    }

    /* Check if this handle matches the current streaming BGM */
    wasBgm = FALSE;
    if (g_currentStreamBgmId == handle) {
        g_currentStreamBgmId = 0;
        wasBgm = TRUE;
    }

    /* Check if this handle matches the current non-streaming BGM */
    if (g_currentBgmId == handle) {
        g_currentBgmId = 0;
        wasBgm = FALSE; /* non-streaming BGM gets priority in cleanup */
    }

    /* Check if the sound is flagged as BGM */
    if (_sndIsBgm(handle)) {
        /* Sound IS a BGM -- do a crossfade stop */
        _sndCrossfadeOut(sndId, wasBgm, TRUE); /* fn_80165C70 */

        if (wasBgm) {
            /* This was the streaming BGM -- warn and restart */
            fn_800DD970(
                "soundStop: Warning! BGM cannot be stopped.(snd_id=%d)\n",
                handle);
            sndCheckFileInfo(1, 0, 0xFF, wasBgm); /* restart with full volume */
            return 0;
        }

        /* Non-streaming BGM: just release the work entry */
        _sndReleaseWork(handle); /* fn_80166B18 */
    } else {
        /* Sound is an SE -- set fade target and release */
        fn_801669E4(handle, sndId, FALSE);
        _sndReleaseWork(handle);
    }

    return 0;
}

/* =========================================================================
 * fn_801658FC -- sndPlayWithCrossfade (non-streaming)
 *
 * r3 = sndId, r4 = crossfade, r5 = fadeTime, r6 = volume
 * =========================================================================
 */
u32 sndPlayWithCrossfade(u32 sndId, u32 crossfade, u32 fadeTime, u32 volume) {
    void* masterState;
    u32 maxSndId;

    if (sndId == 0) {
        return 0;
    }
    masterState = g_sndMasterState;
    maxSndId = *(u32*)masterState;
    if (sndId >= maxSndId) {
        return 0;
    }

    /* Stop current non-streaming BGM with crossfade */
    _sndCrossfadeOut(crossfade, FALSE, TRUE); /* fn_80165C70 */

    /* Start new sound (non-streaming) */
    sndCheckFileInfo(sndId, fadeTime, volume, FALSE); /* fn_80165A44 */

    return 0;
}

/* =========================================================================
 * fn_8016597C -- sndPlayStreamWithCrossfade (streaming)
 *
 * r3 = sndId, r4 = crossfade, r5 = fadeTime, r6 = volume
 * =========================================================================
 */
u32 sndPlayStreamWithCrossfade(u32 sndId, u32 crossfade, u32 fadeTime,
                               u32 volume) {
    void* masterState;
    u32 maxSndId;

    if (sndId == 0) {
        return 0;
    }
    masterState = g_sndMasterState;
    maxSndId = *(u32*)masterState;
    if (sndId >= maxSndId) {
        return 0;
    }

    /* Stop current streaming BGM with crossfade */
    _sndCrossfadeOut(crossfade, TRUE, TRUE); /* fn_80165C70 */

    /* Start new sound (streaming) */
    sndCheckFileInfo(sndId, fadeTime, volume, TRUE); /* fn_80165A44 */

    return 0;
}

/* =========================================================================
 * fn_801659FC -- sndPlayBgm (non-streaming wrapper)
 * =========================================================================
 */
u32 sndPlayBgm(u32 sndId, u32 fadeTime, u32 volume) {
    return sndCheckFileInfo(sndId, fadeTime, volume, FALSE);
}

/* =========================================================================
 * fn_80165A20 -- sndPlayBgmStream (streaming wrapper)
 * =========================================================================
 */
u32 sndPlayBgmStream(u32 sndId, u32 fadeTime, u32 volume) {
    return sndCheckFileInfo(sndId, fadeTime, volume, TRUE);
}

/* =========================================================================
 * fn_80165A44 -- sndCheckFileInfo
 *
 * The main sound playback dispatcher. Looks up the file_info entry for
 * the given sndId, determines if it's BGM or SE, and routes to the
 * appropriate playback path.
 *
 * r3 = sndId, r4 = fadeTime, r5 = volume, r6 = isBgm
 * =========================================================================
 */
u32 sndCheckFileInfo(u32 sndId, u32 fadeTime, u32 volume, u32 isBgm) {
    void* masterState;
    u32 maxSndId;
    u8* workEntry;
    u8 entryFlags;
    u32 category;
    u32 bufSize;
    u8 fileInfoIdx;
    u32 fileInfoCount;
    u32 curHandle;
    void* fileInfoData;
    void* wavePtr;
    void* streamName;

    /* Bounds check */
    if (sndId == 0) {
        return 0;
    }
    masterState = g_sndMasterState;
    maxSndId = *(u32*)masterState;
    if (sndId >= maxSndId) {
        return 0;
    }

    /* Look up SndWork entry: base + sndId * 0x0C */
    workEntry = (u8*)g_sndWorkTable + (sndId * 0x0C);
    entryFlags = workEntry[0];

    /* Check if sound is already flagged as BGM (bit 0, extrwi r,1,24) */
    if ((entryFlags >> 1) & 1) {
        /* Sound is already a BGM -- update it in place */
        if (isBgm == 0) {
            /* Check status and handle transitions */
            s32 status = GSsndGetStatus(sndId);
            if (status > 0 && status < 4) {
                /* Fade out old BGM before replacing */
                _sndCheckSndWorkAll(sndId, SND_FADE_DEFAULT, 0);
                _sndFadeBgm(SND_FADE_DEFAULT);
                _sndReleaseWork(sndId);
            }
            g_currentBgmId = sndId;
        }

        /* Start playback */
        fn_80166A50(sndId, fadeTime, volume, FALSE);
        return 0;
    }

    /* Determine category and buffer size based on isBgm flag */
    if (isBgm) {
        category = SND_CATEGORY_BGM;  /* 0x406 */
        bufSize = 0x10000;            /* 64KB streaming buffer */
    } else {
        category = SND_CATEGORY_SE;   /* 0x407 */
        bufSize = 0x2000;             /* 8KB SE buffer */
    }

    /* Validate file_info index */
    fileInfoIdx = workEntry[3];
    fileInfoCount = *(u32*)g_sndFileInfoCount;
    if (fileInfoIdx >= fileInfoCount) {
        fn_800DD970("ERROR(sound.c): invalid file_info number snd_id=%d\n",
                    sndId);
        return 0;
    }

    /* Check if another sound is playing in this category */
    curHandle = _sndFindCurrentHandle(0, category);

    /* If the same sound is already playing, skip re-triggering */
    if (curHandle == sndId) {

    } else {

        /* If another sound IS playing, stop it first */
        if ((curHandle + 0x10000) != 0xFFFF) {
            s32 status = GSsndGetStatus(sndId);
            if (status > 0 && status < 4) {
                _sndCheckSndWorkAll(curHandle, SND_FADE_DEFAULT, 0);
                _sndFadeBgm(SND_FADE_DEFAULT);
                _sndReleaseWork(curHandle);
            }
        }

        /* Look up wave data pointer from the file_info table */
        fileInfoData = g_sndFileInfoData;
        wavePtr = *(void**)((u8*)fileInfoData + (fileInfoIdx * 8));

        if (wavePtr != NULL) {
            /* Wave data already loaded -- use direct playback */
            void* jAudioBuf = (void*)fn_800F9318(0, category);
            sndStreamLoad(wavePtr, jAudioBuf, bufSize); /* fn_80165EE0 */
        } else {
            /* Try stream name from second pointer in file_info entry */
            streamName = *(void**)((u8*)fileInfoData + (fileInfoIdx * 8) + 4);
            if (streamName != NULL) {
                void* jAudioBuf = (void*)fn_800F9318(0, category);
                sndWaveOpenByName(streamName, jAudioBuf, bufSize); /* fn_80165DEC */
            } else {
                /* No data available */
                fn_800DD970("ERROR(sound.c): Can't Play Sound. snd_id=%d\n",
                            sndId);
                return 0;
            }
        }

        /* Bind this sound to a work entry */
        fn_80166B3C(sndId, 0, category);

    }
    /* Track as current BGM */
    if (isBgm) {
        g_currentStreamBgmId = sndId;
    }
    if (!isBgm) {
        g_currentBgmId = sndId;
    }

    /* Start playback with requested volume and fade */
    fn_80166A50(sndId, fadeTime, volume, FALSE);

    return 0;
}

/* =========================================================================
 * fn_80165C70 -- _sndCrossfadeOut
 *
 * Stops the current BGM with a crossfade, optionally waiting.
 *
 * r3 = sndId, r4 = isBgm, r5 = wait
 * =========================================================================
 */
void _sndCrossfadeOut(u32 sndId, u32 isBgm, BOOL wait) {
    u32 category;
    u32 curHandle;

    /* Determine category based on isBgm (neg/or/srawi trick for select) */
    category = isBgm ? SND_CATEGORY_BGM : SND_CATEGORY_SE;

    curHandle = _sndFindCurrentHandle(0, category);

    if ((curHandle + 0x10000) != 0xFFFF) {
        s32 status = GSsndGetStatus(curHandle);
        if (status > 0 && status < 4) {
            _sndCheckSndWorkAll(curHandle, sndId, 0);
            if (wait) {
                _sndFadeBgm(sndId);
            }
        }
    }
}

/* =========================================================================
 * fn_80165D0C -- _sndFadeBgm
 *
 * Fades the BGM volume over a specified time by spinning in a loop,
 * accumulating time via the engine's frame timing functions.
 *
 * r3 = fadeTimeMs (converted to float target)
 * =========================================================================
 */
void _sndFadeBgm(u32 fadeTimeMs) {
    f32 target;
    f32 elapsed;
    f32 deltaTime;
    s32 startTick;
    s32 endTick;

    /* Convert integer fadeTimeMs to float via u32->double->float trick */
    target = (f32)fadeTimeMs;
    elapsed = 0.0f;

    while (elapsed < target) {
        fn_800F0308();          /* wait one frame */
        startTick = fn_800D37CC(); /* get start tick (fn_800D37CC) */
        endTick = fn_800D3088();   /* get end tick */
        deltaTime = (f32)(endTick - startTick);
        elapsed += deltaTime;
    }
}

/* =========================================================================
 * fn_80165DEC -- sndWaveOpenByName
 *
 * Opens a wave file by name from the FSYS archive.
 * Reads the data into the provided buffer after validation.
 *
 * r3 = filename, r4 = buffer, r5 = bufSize
 * =========================================================================
 */
void sndWaveOpenByName(const char* filename, void* buffer, u32 bufSize) {
    void* fileHandle;
    u32 fileSize;
    u32 alignedSize;

    /* Open the file from the archive */
    fileHandle = fn_80167F28(); /* _sndOpenStreamFile */

    if (fileHandle == NULL) {
        fn_800DD970("ERROR: Unable to open '%s'\n", filename);

    } else {

        /* Get the file size */
        fileSize = (u32)fn_80167E5C(fileHandle);

        if (fileSize == 0) {
            fn_800DD970("ERROR: Zero length file '%s'\n", filename);

        } else {

            /* Align size to 32-byte boundary */
            alignedSize = (fileSize + 0x1F) & ~0x1F;

            if (alignedSize >= bufSize) {
                fn_800DD970("ERROR: Over Sound Buffer\n");
            } else if (buffer == NULL) {
                fn_800DD970("ERROR: Unable to allocate buffer\n");
            } else {
                memset(buffer, 0xE0, alignedSize); /* fill with silence pattern */

                if (fn_80167ED0(fileHandle, buffer, alignedSize, 0) <= 0) {
                    fn_800DD970("ERROR: Failed to read data from '%s'\n", filename);
                } else {
                    /* success path */
                    fn_80167E64(fileHandle);
                    return;
                }
            }

        }
        fn_80167E64(fileHandle); /* release stream handle */
        return;

    }
    fn_80167E64(fileHandle);
}

/* =========================================================================
 * fn_80165EE0 -- sndStreamLoad
 *
 * Initializes a streaming buffer and triggers an FSYS archive load.
 *
 * r3 = waveData, r4 = buffer, r5 = bufSize
 * =========================================================================
 */
void sndStreamLoad(void* waveData, void* buffer, u32 bufSize) {
    memset(buffer, 0xE0, bufSize); /* fill with silence pattern */

    g_streamBufferSize = bufSize;   /* lbl_8047B0B0 */
    g_streamBuffer = buffer;        /* lbl_8047B0B4 */

    /* Trigger FSYS load for sound resource 0x99 */
    fn_8017AF6C(0x99, waveData);
}

/* =========================================================================
 * fn_80165F40 -- _sndInitStreamBuffers
 *
 * Allocates a JAudio channel for BGM streaming and loads the initial
 * sound data from the FSYS archive.
 * =========================================================================
 */
void _sndInitStreamBuffers(void) {
    void* jAudioBuf;

    jAudioBuf = (void*)fn_800F9318(0, SND_CATEGORY_BGM);
    g_streamBuffer = jAudioBuf;
    g_streamBufferSize = 0x10000; /* 64KB */

    fn_8017B370(0x99); /* load FSYS resource 0x99 */
    _sndWaitGroupLoad(0x99); /* wait for load to complete */
}

/* =========================================================================
 * fn_80165F84 -- sndShutdown
 *
 * Stops all sounds, releases JAudio channels, and clears global state.
 * =========================================================================
 */
void sndShutdown(void) {
    void* bgmBuf;
    void* seBuf;

    /* Allocate and free BGM channel */
    bgmBuf = _sndAllocBlock(0x10000);
    fn_800F9378((u32)bgmBuf, 0, SND_CATEGORY_BGM, 0);

    /* Allocate and free SE channel */
    seBuf = _sndAllocBlock(0x2000);
    fn_800F9378((u32)seBuf, 0, SND_CATEGORY_SE, 0);

    /* Clear BGM tracking state */
    g_currentStreamBgmId = 0;
    g_currentBgmId = 0;
}

/* =========================================================================
 * fn_80165FDC -- _sndWaitGroupLoad
 *
 * Polls FSYS load status in a loop until complete.
 * Prints "ERROR: Can't Read Group(%d)" if the load fails.
 *
 * r3 = groupId
 * =========================================================================
 */
void _sndWaitGroupLoad(u32 groupId) {
    s32 status;

    do {
        status = fn_8017B2CC(groupId);
        if (status < 0) {
            fn_800DD970("ERROR: Can't Read Group(%d)\n\n", groupId);
        }
        if (status != 0) {
            fn_800F0308(); /* wait one frame */
        }
    } while (status != 0);
}

/* =========================================================================
 * fn_8016604C -- _sndAllocBlock
 *
 * Allocates a memory block from the GSmem heap with 32-byte alignment.
 *
 * r3 = size
 * =========================================================================
 */
void* _sndAllocBlock(u32 size) {
    u32 handle;

    handle = (u32)fn_800E2C04(size, 0x20);
    if ((handle & 0xFFFF) != 0) {
        return fn_800E27B0((void*)handle);
    }
    return NULL;
}

/* =========================================================================
 * fn_80166084 -- _sndIsBgm
 *
 * Checks the isBGM flag (bit 1) of a SndWork entry.
 *
 * r3 = sndId
 * =========================================================================
 */
BOOL _sndIsBgm(u32 sndId) {
    u8* entry;
    u8 flags;

    entry = (u8*)g_sndWorkTable + (sndId * 0x0C);
    flags = entry[0];

    /* Extract bit 1 (extrwi r0, r0, 1, 24 -- bit position 1) */
    return (flags >> 1) & 1;
}

/* =========================================================================
 * fn_80166098 -- _sndGetWorkVolume
 *
 * Returns the current volume from a SndWork entry's resource data.
 *
 * r3 = sndId
 * =========================================================================
 */
u32 _sndGetWorkVolume(u32 sndId) {
    u8* entry;
    u8 flags;
    SndResData* resData;

    entry = (u8*)g_sndWorkTable + (sndId * 0x0C);
    flags = entry[0];

    /* Check isActive flag (bit 2, extrwi r0, r0, 1, 26) */
    if (!((flags >> 3) & 1)) {
        return 0;
    }

    /* Get resource data pointer from offset 0x08 */
    resData = *(SndResData**)(entry + 0x08);
    if (resData == NULL) {
        return 0;
    }

    return resData->maxVolume;
}

/* =========================================================================
 * fn_801653BC -- sndGetCurrentBgmId
 * =========================================================================
 */
u32 sndGetCurrentBgmId(void) {
    return g_currentBgmId;
}

/* =========================================================================
 * fn_801653C4 -- sndGetCurrentStreamBgmId
 * =========================================================================
 */
u32 sndGetCurrentStreamBgmId(void) {
    return g_currentStreamBgmId;
}

/* =========================================================================
 * fn_8016557C -- _sndStopCurrentBgm
 *
 * Stops the currently playing non-streaming BGM with a quick fade.
 * Returns the old BGM handle.
 * =========================================================================
 */
u32 _sndStopCurrentBgm(void) {
    u32 oldHandle;

    /* Find the current BGM in the non-streaming category */
    oldHandle = _sndFindCurrentHandle(0, SND_CATEGORY_BGM);

    if ((oldHandle + 0x10000) != 0xFFFF) {
        /* Transpose/re-trigger with 50ms fade */
        _sndSeTranspose(oldHandle, SND_FADE_DEFAULT, 0);
        _sndFadeBgm(SND_FADE_DEFAULT);
    }

    return oldHandle;
}

/* =========================================================================
 * fn_801653CC -- _sndPrepareStream
 *
 * Loads streaming data for a sound by allocating a JAudio channel and
 * triggering an FSYS load. Polls until the data is ready.
 *
 * r3 = sndId, r4 = fadeTime, r5 = volume
 * =========================================================================
 */
BOOL _sndPrepareStream(u32 sndId, u32 fadeTime, u32 volume) {
    void* masterState;
    u32 maxSndId;
    u8* workEntry;
    u8 fileInfoIdx;
    u32 fileInfoCount;
    void* waveData;
    void* jAudioBuf;

    if (sndId == 0) {
        return FALSE;
    }
    masterState = g_sndMasterState;
    maxSndId = *(u32*)masterState;
    if (sndId >= maxSndId) {
        return FALSE;
    }

    /* Look up file_info index from SndWork entry */
    workEntry = (u8*)g_sndWorkTable + (sndId * 0x0C);
    fileInfoIdx = workEntry[3];

    fileInfoCount = *(u32*)g_sndFileInfoCount;
    if (fileInfoIdx >= fileInfoCount) {
        return FALSE; /* invalid index, silently fail */
    }

    /* Allocate JAudio channel for streaming */
    jAudioBuf = (void*)fn_800F9418(0x10000, 0x20, 0,
                                   SND_CATEGORY_STREAM, 0);
    if (jAudioBuf == NULL) {
        return FALSE;
    }

    /* Look up wave data from file_info table */
    waveData = *(void**)((u8*)g_sndFileInfoData + (fileInfoIdx * 8));
    if (waveData != NULL) {
        sndStreamLoad(waveData, jAudioBuf, 0x10000);
    }

    /* Bind work entry */
    fn_80166B3C(sndId, 0, SND_CATEGORY_STREAM);

    /* Start playback with fade */
    fn_80166A50(sndId, fadeTime, volume, FALSE);

    /* Poll until playback status is ready */
    while (GSsndGetStatus(sndId) == 2) {
        fn_800F0308(); /* wait one frame */
    }

    /* Release the work entry */
    _sndReleaseWork(sndId);

    /* Free the JAudio channel */
    fn_800F9210(0, SND_CATEGORY_STREAM);

    return TRUE;
}

/* =========================================================================
 * fn_801654E0 -- (unnamed) sndGetWaveData
 *
 * Retrieves the wave data pointer for a given sndId and loads it
 * into the streaming system if available.
 *
 * r3 = sndId, r4 = buffer, r5 = bufSize
 * =========================================================================
 */
static BOOL sndGetWaveData(u32 sndId, void* buffer, u32 bufSize) {
    u8* workEntry;
    u8 fileInfoIdx;
    u32 fileInfoCount;
    void* waveData;

    workEntry = (u8*)g_sndWorkTable + (sndId * 0x0C);
    fileInfoIdx = workEntry[3];

    fileInfoCount = *(u32*)g_sndFileInfoCount;
    if (fileInfoIdx >= fileInfoCount) {
        return FALSE;
    }

    waveData = *(void**)((u8*)g_sndFileInfoData + (fileInfoIdx * 8));
    if (waveData == NULL) {
        return FALSE;
    }

    sndStreamLoad(waveData, buffer, bufSize);
    return TRUE;
}

/* =========================================================================
 * fn_80165548 -- (unnamed) _sndSeekOldBgm
 *
 * If the given handle is valid, does a transpose/seek to ensure clean
 * crossfade completion.
 *
 * r3 = handle
 * =========================================================================
 */
static void _sndSeekOldBgm(u32 handle) {
    if ((handle + 0x10000) != 0xFFFF) {
        _sndBgmTranspose(handle, 0x3E8); /* 1000ms seek */
    }
}

/* =========================================================================
 * fn_80166E88 -- sndInit
 *
 * Main sound system initialization. Called during GameInit.
 *
 * Allocates memory pools for BGM slots, SE slots, and 3D listeners.
 * Sets up JAudio, loads the sound bank table, and configures the
 * initial master volume (BGM=0x7F, SE=0x64).
 *
 * Parameters (from disassembly analysis):
 *   r3 = numBgm      (base BGM count)
 *   r4 = numSe        (SE count multiplier for work table size)
 *   r5 = numBgmRes    (added to numBgm for total work alloc)
 *   r6 = numSe3d      (number of 3D SE listener slots)
 *   r7 = numStreams    (number of streaming channels)
 *
 * The function:
 *   1. Computes total work allocation: (numBgm + numBgmRes) * 0x14
 *   2. Stores sound count from the master state
 *   3. Allocates the master block (fn_80167BB0)
 *   4. Allocates BGM pool: numSe * fn_80167BB0
 *   5. Allocates SE pool: numSe3d * 0xD0 * fn_80167BB0
 *   6. Allocates listener pool: numStreams * 0x78 * fn_80167BB0
 *   7. Initializes JAudio (fn_801631AC, fn_8015FE88)
 *   8. Loads sound table from lbl_80452500
 *   9. Registers the sound update callback (fn_80167040)
 *   10. Sets initial volume: BGM=0x7F, SE=0x64
 *
 * Address: 0x80166E88, size 0x1B8
 * =========================================================================
 */
BOOL sndInit(u32 numBgm, u32 numSe, u32 numBgmRes, u32 numSe3d,
             u32 numStreams) {
    u32 totalWorkSize;
    u32 sndCount;
    void* masterBlock;
    void* bgmPool;
    void* sePool;
    void* listenerPool;
    u8 jAudioParams[8]; /* local struct for JAudio init params */
    s32 initResult;

    /* Compute total work allocation */
    totalWorkSize = (numBgm + numBgmRes) * 0x14;

    /* Store counts and init tracking */
    g_sndWorkCount = *(u32*)g_sndMasterState;
    g_sndCurrentGroup = -1;
    g_sndWorkAllocSize = totalWorkSize;

    /* Step 1: Allocate master work block */
    masterBlock = fn_80167BB0(totalWorkSize);
    g_sndMasterAlloc = masterBlock;
    if (masterBlock == NULL) {
        return FALSE;
    }
    fn_80167A6C(); /* init BGM pool defaults */

    /* Step 2: Allocate BGM pool */
    g_sndBgmMax = numSe;
    bgmPool = fn_80167BB0(numSe);
    g_sndBgmPool = bgmPool;
    if (bgmPool == NULL) {
        return FALSE;
    }
    fn_80167A44(); /* init SE pool defaults */

    /* Step 3: Allocate SE pool */
    g_sndSeMax = numSe3d;
    sePool = fn_80167BB0(numSe3d * 0xD0);
    g_sndSePool = sePool;
    if (sePool == NULL) {
        return FALSE;
    }
    fn_80167A14(); /* init listener pool defaults */

    /* Step 4: Allocate 3D listener pool */
    g_sndListenerMax = numStreams;
    listenerPool = fn_80167BB0(numStreams * 0x78);
    g_sndListenerPool = listenerPool;
    if (listenerPool == NULL) {
        return FALSE;
    }
    fn_801679E4(); /* init listener defaults */

    /* Step 5: Initialize JAudio */
    fn_801631AC(jAudioParams);
    initResult = fn_8015FE88(
        0x40,       /* maxVoices = 64 */
        0x30,       /* priority = 48 */
        0x10,       /* maxStreams = 16 */
        TRUE,       /* enableDSP */
        FALSE,      /* unused */
        0x009FC000  /* stackSize (lis r4, 0xA0; subi r8, r4, 0x4000) */
    );
    if (initResult != 0) {
        return FALSE;
    }

    /* Step 6: Load sound bank table from lbl_80452500 */
    fn_80167A9C(0); /* load group 0 */
    fn_801644E0((void*)0x80452500); /* set table buffer */

    /* Step 7: Register the per-frame update callback */
    fn_8014DAA8(
        0,                  /* channel */
        (void*)fn_80167040, /* callback: _sndUpdateCallback */
        (void*)0x80452500,  /* userData */
        0xFF,               /* volume */
        0,                  /* pan */
        0,                  /* unused */
        0,                  /* unused */
        0xFF                /* priority */
    );

    /* Step 8: Set initial master volumes */
    /* BGM: volume=0x7F, pan=0, applyBgm=TRUE, applySe=FALSE */
    sndSetMasterVolume(SND_VOLUME_MAX, 0, TRUE, FALSE);
    /* SE: volume=0x64, pan=0, applyBgm=FALSE, applySe=TRUE */
    sndSetMasterVolume(0x64, 0, FALSE, TRUE);

    /* Step 9: Check audio availability and set surround mode */
    sndIsAudioAvailable();
    sndSetSurroundMode(0); /* default to stereo */

    /* Step 10: Register the JAudio tick callback */
    fn_8014D8C0((void*)fn_80167040);

    return TRUE;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

extern u32 lbl_8047B0A8;
extern u32 lbl_8047B0AC;

/* Address: 0x801653BC | Size: 0x8 | Pattern: sda_getter */
u32 fn_801653BC(void) {
    return lbl_8047B0A8;
}

/* Address: 0x801653C4 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801653C4(void) {
    return lbl_8047B0AC;
}
