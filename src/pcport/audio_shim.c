/**
 * @file audio_shim.c
 * @brief JAudio2 replacement -- SDL2_mixer audio backend stubs.
 *
 * References:
 *   - docs/pc_port_design.md Section 9 (Audio Replacement)
 *   - audio_shim.h for full API documentation
 *
 * Phase 3 PC port scaffolding -- skeleton only.
 */

#include "audio_shim.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO: Include SDL2 headers when build system is ready
 * #include <SDL2/SDL.h>
 * #include <SDL2/SDL_mixer.h>
 */

/* =========================================================================
 * Internal state
 * ========================================================================= */

/** Per-channel state tracking */
static AudioChannel g_channels[AUDIO_MAX_CHANNELS];

/** Number of allocated channels */
static s32 g_numAllocated = 0;

/** Global per-frame update callback (from the game's sound system) */
static void (*g_updateCallback)(void) = 0;

/** Audio system initialized flag */
static BOOL g_audioInitialized = 0;

/* =========================================================================
 * Initialization / Shutdown
 * ========================================================================= */

BOOL JAudio_Init(void) {
    /* TODO: Phase 3g -- Initialize SDL2 audio subsystem
     *
     * if (SDL_Init(SDL_INIT_AUDIO) < 0) {
     *     printf("[audio_shim] SDL_Init(AUDIO) failed: %s\n", SDL_GetError());
     *     return 0;
     * }
     *
     * if (Mix_OpenAudio(AUDIO_SAMPLE_RATE, AUDIO_FORMAT,
     *                   AUDIO_CHANNELS, AUDIO_CHUNK_SIZE) < 0) {
     *     printf("[audio_shim] Mix_OpenAudio failed: %s\n", Mix_GetError());
     *     return 0;
     * }
     *
     * Mix_AllocateChannels(AUDIO_MAX_CHANNELS);
     *
     * // Register a global channel-finished callback
     * Mix_ChannelFinished(audio_channel_finished_callback);
     */

    memset(g_channels, 0, sizeof(g_channels));
    for (s32 i = 0; i < AUDIO_MAX_CHANNELS; i++) {
        g_channels[i].index = i;
        g_channels[i].sdlChannel = -1;
        g_channels[i].state = 0;
    }
    g_numAllocated = 0;
    g_audioInitialized = 1;

    printf("[audio_shim] JAudio_Init stub -- SDL2 audio init goes here\n");
    return 1;
}

void JAudio_Shutdown(void) {
    /* TODO: Phase 3g -- Clean up SDL2 audio
     *
     * // Stop all channels
     * Mix_HaltChannel(-1);
     * Mix_HaltMusic();
     *
     * // Free all allocated chunks
     * for (int i = 0; i < AUDIO_MAX_CHANNELS; i++) {
     *     if (g_channels[i].mixChunk) {
     *         Mix_FreeChunk((Mix_Chunk*)g_channels[i].mixChunk);
     *         g_channels[i].mixChunk = NULL;
     *     }
     * }
     *
     * Mix_CloseAudio();
     * SDL_QuitSubSystem(SDL_INIT_AUDIO);
     */

    memset(g_channels, 0, sizeof(g_channels));
    g_numAllocated = 0;
    g_audioInitialized = 0;

    printf("[audio_shim] JAudio_Shutdown stub\n");
}

/* =========================================================================
 * Channel management
 * ========================================================================= */

s32 JAudio_AllocChannel(u16 category) {
    /* TODO: Phase 3g -- Find a free SDL_mixer channel
     *
     * for (int i = 0; i < AUDIO_MAX_CHANNELS; i++) {
     *     if (g_channels[i].state == 0) {
     *         g_channels[i].category = category;
     *         g_channels[i].state = 0; // allocated but not playing
     *         g_channels[i].sdlChannel = i; // direct 1:1 mapping
     *         g_numAllocated++;
     *         return i;
     *     }
     * }
     * return -1; // no free channels
     */

    (void)category;
    for (s32 i = 0; i < AUDIO_MAX_CHANNELS; i++) {
        if (g_channels[i].state == 0 && g_channels[i].sdlChannel == -1) {
            g_channels[i].category = category;
            g_channels[i].sdlChannel = i;
            g_numAllocated++;
            return i;
        }
    }
    return -1;
}

void JAudio_FreeChannel(s32 channel) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    /* TODO: Phase 3g -- Stop and release SDL channel
     *
     * Mix_HaltChannel(g_channels[channel].sdlChannel);
     * if (g_channels[channel].mixChunk) {
     *     Mix_FreeChunk((Mix_Chunk*)g_channels[channel].mixChunk);
     *     g_channels[channel].mixChunk = NULL;
     * }
     */

    g_channels[channel].state = 0;
    g_channels[channel].sdlChannel = -1;
    g_channels[channel].volume = 0;
    g_channels[channel].callback = 0;
    g_channels[channel].mixChunk = 0;
    if (g_numAllocated > 0) g_numAllocated--;
}

void JAudio_ReleaseChannel(s32 channel) {
    /* TODO: Phase 3g -- Release channel without freeing chunk data */
    JAudio_FreeChannel(channel);
}

/* =========================================================================
 * Playback control
 * ========================================================================= */

s32 JAudio_StartPlayback(s32 channel, void* data, u32 dataSize, BOOL loop) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return -1;

    (void)data; (void)dataSize;

    /* TODO: Phase 3g -- Start playback via SDL_mixer
     *
     * 1. The 'data' pointer points to raw GCN ADPCM audio.
     *    Decode it to PCM16 using JAudio_DecodeADPCM.
     *
     * 2. Create an SDL_mixer chunk:
     *    Mix_Chunk* chunk = JAudio_CreateChunk(pcmData, numSamples,
     *                                          32000, 1);
     *    g_channels[channel].mixChunk = chunk;
     *
     * 3. Play the chunk:
     *    int loops = loop ? -1 : 0;
     *    Mix_PlayChannel(g_channels[channel].sdlChannel, chunk, loops);
     *
     * 4. Set volume:
     *    Mix_Volume(g_channels[channel].sdlChannel,
     *               g_channels[channel].volume * 128 / 127);
     */

    g_channels[channel].state = 1; /* playing */
    g_channels[channel].looping = loop ? 1 : 0;
    return 0;
}

void JAudio_Stop(s32 channel) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    /* TODO: Phase 3g -- Stop playback
     *
     * Mix_HaltChannel(g_channels[channel].sdlChannel);
     */

    g_channels[channel].state = 0;
}

BOOL JAudio_IsPlaying(s32 channel) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return 0;

    /* TODO: Phase 3g -- Check playing state
     *
     * return Mix_Playing(g_channels[channel].sdlChannel);
     */

    return g_channels[channel].state == 1;
}

void JAudio_Pause(s32 channel) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    /* TODO: Phase 3g -- Pause channel
     *
     * Mix_Pause(g_channels[channel].sdlChannel);
     */

    if (g_channels[channel].state == 1)
        g_channels[channel].state = 2; /* paused */
}

void JAudio_Resume(s32 channel) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    /* TODO: Phase 3g -- Resume channel
     *
     * Mix_Resume(g_channels[channel].sdlChannel);
     */

    if (g_channels[channel].state == 2)
        g_channels[channel].state = 1; /* playing */
}

void JAudio_SetVolume(s32 channel, u8 volume) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    g_channels[channel].volume = volume;

    /* TODO: Phase 3g -- Set SDL_mixer volume
     *
     * // SDL_mixer volume range is 0-128 (MIX_MAX_VOLUME)
     * // JAudio volume range is 0-127
     * int sdlVol = (int)volume * 128 / 127;
     * Mix_Volume(g_channels[channel].sdlChannel, sdlVol);
     */
}

void JAudio_SetCallback(s32 channel, void (*callback)(s32)) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    g_channels[channel].callback = callback;

    /* TODO: Phase 3g -- Per-channel finish callback
     *
     * SDL_mixer only supports a single global ChannelFinished callback.
     * Use the global callback to dispatch to per-channel callbacks:
     *
     * static void audio_channel_finished(int ch) {
     *     if (g_channels[ch].callback)
     *         g_channels[ch].callback(ch);
     * }
     * Mix_ChannelFinished(audio_channel_finished);
     */
}

void JAudio_SetParams(s32 channel, u8 volume, u8 pan, u16 pitch) {
    if (channel < 0 || channel >= AUDIO_MAX_CHANNELS) return;

    g_channels[channel].volume = volume;
    g_channels[channel].pan = pan;

    (void)pitch;

    /* TODO: Phase 3g -- Set volume, pan, and pitch
     *
     * // Volume
     * Mix_Volume(g_channels[channel].sdlChannel, volume * 128 / 127);
     *
     * // Pan (SDL_mixer panning: 0=left, 127=center, 255=right)
     * // GCN pan: 0=left, 64=center, 127=right (needs scaling)
     * u8 sdlPan = pan * 2;  // approximate
     * Mix_SetPanning(g_channels[channel].sdlChannel,
     *                255 - sdlPan, sdlPan);
     *
     * // Pitch: SDL_mixer does not natively support pitch shifting.
     * // Options:
     * // a) Ignore pitch for initial implementation
     * // b) Resample the audio data at a different rate
     * // c) Use SDL_mixer effects chain for pitch shift
     */
}

void JAudio_SetDSPMix(u8 chorus, u16 reverb, u8 delay, u8 wet) {
    (void)chorus; (void)reverb; (void)delay; (void)wet;

    /* TODO: Phase 3g -- DSP effects
     *
     * Low priority for first playable build. Options:
     *
     * 1. No-op: Skip DSP effects entirely (acceptable for initial port)
     * 2. Simple reverb: Use Mix_RegisterEffect with a delay-based
     *    reverb implementation
     * 3. Full DSP chain: Implement chorus/reverb/delay as SDL_mixer
     *    effect callbacks
     *
     * The game calls sndSetDspMix(chorus, reverb, delay, wet) which
     * maps to this function.
     */
}

void JAudio_Flush(void) {
    /* TODO: Phase 3g -- Flush pending operations
     *
     * No-op for SDL_mixer -- operations are immediate.
     * Or use Mix_ExpireChannel to cancel timed-out channels.
     */
}

void JAudio_RegisterUpdate(void (*callback)(void)) {
    g_updateCallback = callback;

    /* TODO: Phase 3g -- Register per-frame update
     *
     * The game's sound system calls _sndUpdateAllVolumes per frame.
     * On PC, this should be called from the main game loop:
     *
     *   void pcport_audio_update(void) {
     *       if (g_updateCallback)
     *           g_updateCallback();
     *   }
     */
}

/* =========================================================================
 * ADPCM decode
 * ========================================================================= */

s32 JAudio_DecodeADPCM(const void* adpcmData, u32 adpcmSize,
                       const s16 coeffs[16],
                       s16* outPCM, u32 outSize) {
    (void)adpcmData; (void)adpcmSize; (void)coeffs;
    (void)outPCM; (void)outSize;

    /* TODO: Phase 3g -- Implement DSP-ADPCM decoder
     *
     * GCN DSP-ADPCM format:
     * - 4 bits per sample, packed in frames of 14 samples + 2 header bytes
     * - Each frame is 8 bytes: 1 byte header (predictor + scale), 7 bytes data
     * - Uses 2 previous samples (yn1, yn2) for prediction
     * - 8 coefficient pairs (16 values) for the predictor
     *
     * Algorithm (per frame):
     *   u8 header = adpcmData[frameOffset]
     *   int predictor = (header >> 4) & 0x7
     *   int scale = 1 << (header & 0xF)
     *   s16 coef1 = coeffs[predictor * 2]
     *   s16 coef2 = coeffs[predictor * 2 + 1]
     *
     *   for each nibble in the frame:
     *     s32 sample = sign_extend_4bit(nibble)
     *     sample = (sample * scale) << 11
     *     sample += coef1 * yn1 + coef2 * yn2
     *     sample = clamp16(sample >> 11)
     *     yn2 = yn1
     *     yn1 = sample
     *     *outPCM++ = (s16)sample
     *
     * Reference implementation: vgmstream's dsp_decoder.c
     */

    return -1; /* Not implemented */
}

void* JAudio_CreateChunk(const s16* pcmData, u32 numSamples,
                         u32 sampleRate, u8 channels) {
    (void)pcmData; (void)numSamples; (void)sampleRate; (void)channels;

    /* TODO: Phase 3g -- Create SDL_mixer chunk from PCM data
     *
     * // If sample rate differs from output rate, resample
     * if (sampleRate != AUDIO_SAMPLE_RATE) {
     *     // Use SDL_AudioCVT for sample rate conversion
     *     SDL_AudioCVT cvt;
     *     SDL_BuildAudioCVT(&cvt, AUDIO_S16SYS, channels, sampleRate,
     *                       AUDIO_S16SYS, AUDIO_CHANNELS, AUDIO_SAMPLE_RATE);
     *     cvt.len = numSamples * channels * 2;
     *     cvt.buf = malloc(cvt.len * cvt.len_mult);
     *     memcpy(cvt.buf, pcmData, cvt.len);
     *     SDL_ConvertAudio(&cvt);
     *     // Create chunk from converted data
     * }
     *
     * Mix_Chunk* chunk = Mix_QuickLoad_RAW((Uint8*)pcmData,
     *                                      numSamples * channels * 2);
     * return chunk;
     */

    return (void*)0;
}

void JAudio_FreeChunk(void* chunk) {
    (void)chunk;

    /* TODO: Phase 3g -- Free SDL_mixer chunk
     *
     * if (chunk) Mix_FreeChunk((Mix_Chunk*)chunk);
     */
}

/* =========================================================================
 * Streaming BGM
 * ========================================================================= */

s32 JAudio_StartStream(const char* filePath, BOOL loop) {
    (void)filePath; (void)loop;

    /* TODO: Phase 3g -- Stream BGM from file
     *
     * The game's streaming BGM system loads from FSYS archives.
     * For the PC port, extracted audio files (WAV/OGG) are loaded
     * from disk:
     *
     * Mix_Music* music = Mix_LoadMUS(filePath);
     * if (!music) {
     *     printf("[audio_shim] Failed to load BGM: %s\n", Mix_GetError());
     *     return -1;
     * }
     * Mix_PlayMusic(music, loop ? -1 : 1);
     * return 0;
     */

    printf("[audio_shim] JAudio_StartStream stub: %s (loop=%d)\n",
           filePath ? filePath : "(null)", loop);
    return -1;
}

void JAudio_StopStream(void) {
    /* TODO: Phase 3g -- Stop streaming BGM
     *
     * Mix_HaltMusic();
     */
}

void JAudio_SetStreamVolume(u8 volume) {
    (void)volume;

    /* TODO: Phase 3g -- Set music volume
     *
     * Mix_VolumeMusic(volume * 128 / 127);
     */
}

BOOL JAudio_IsStreamPlaying(void) {
    /* TODO: Phase 3g -- Check if streaming
     *
     * return Mix_PlayingMusic();
     */
    return 0;
}
