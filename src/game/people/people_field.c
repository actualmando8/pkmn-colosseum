/**
 * @file people_field.c
 * @brief MISNAMED UNIT -- actually MusyX audio runtime + item-use logic.
 *
 * Audit 2026-07-01 (docs/fable5_audit_pass1_musyx_discovery.md,
 * docs/fable5_audit_pass2_symbol_name_audit.md): this TU's address range
 * 0x80144574 - 0x801652DC contains NO NPC/people code. Actual contents:
 *
 *   0x80144574 - ~0x80146E88: item-use-on-Pokemon logic (calls
 *     hpRecover__FP20ITEMUSE2POKEMON_LOG1PsP7PokemonUcbUsP12FightPokemon).
 *   ~0x80146E88 - 0x80165400: the MusyX audio runtime (seq, synth, synthdata,
 *     synthmacros/mcmd*, voice, adsr, vsamples, sal, snd_midictrl/inp*,
 *     hardware/hw*, aram, ReverbHI). Reference source: AxioDL/musyx
 *     (musyx/runtime). The 0x404-stride array at lbl_8047AF48 is the MusyX
 *     SYNTH_VOICE work array allocated by synthInit (0x8014D000), NOT a
 *     "PeopleFieldWork" NPC struct.
 *
 * The real people/NPC system appears to live at ~0x80181478 - 0x8018FC50
 * (orphaned people_fn_*.inc extracts in this directory; currently unassigned
 * in splits.txt). The unit/file name is kept until a splits.txt rename pass.
 *
 * Historical per-function commentary below may still use the old fictional
 * peopleField* naming in places; trust symbols.txt and the audit docs over
 * comments here.
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void  DCFlushRange(void* ptr, u32 size);
extern u32   OSDisableInterrupts(void);
extern void  OSRestoreInterrupts(u32 level);

/* renamed symbols referenced by asm incs (symbolmap port) */
extern void ARQPostRequest();
extern void InitStreamBuffers();
extern void aramQueueCallback();
extern void aramUploadData();
extern u32 inpGetMidiCtrl(u32 ctrl, u32 bank, u32 channel);
extern void salCalcVolume(u32 volumeArg, f32* out, u32 voiceIndex, f32 a, f32 b, f32 c, u32 hasPan, u32 studioFlag);
extern void salCallback();
extern u8 jumptable_80369CB0[];
extern u8 jumptable_80369CD4[];
extern u8 jumptable_80369CF8[];
extern u8 lbl_80273448[];
extern u8 lbl_8036944C[];
extern u8 lbl_8036BF00[];
extern u8 lbl_80434C50[];
extern f32 lbl_8047D4D8;
extern f32 lbl_8047D4DC;
extern f32 lbl_8047D4E0;
extern f64 lbl_8047D4E8;
extern f32 lbl_8047D4F0;
extern f32 lbl_8047D4F4;
extern f32 lbl_8047D4F8;
extern f32 lbl_8047D4FC;
extern f32 lbl_8047D500;
extern f32 lbl_8047D504;
extern f64 lbl_8047D508;
extern f32 lbl_8047D510;
extern f32 lbl_8047D514;
extern f64 lbl_8047D518;
extern f32 lbl_8047D520;
extern f64 lbl_8047D528;
extern f32 lbl_8047D530;
extern f32 lbl_8047D534;
extern u32 lbl_8047B070;
extern u32 lbl_8047B078;
extern u32 lbl_8047B07C;

/* GSmem allocator */
extern u16   _toolentryAlloc__FUl(u32 size);
extern void* fn_800E27B0(u16 handle);

/* External functions referenced from asm wrappers */
extern u32 sndAuxCallbackUpdateSettingsReverbHI(u8* ptr);

/* Model system */
extern void  fn_800EE150(void* model, u32 param);
extern void  fn_800EE828(void* model, u32 param);
extern void  fn_800E24B0(void* model, u32 param);
extern void  fn_800E209C(void* model, u32 param);
extern void  fn_800E01F4(void* dst, void* src);
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800E019C(void* model, void* param);
extern void  fn_800E0BA0(void* param);
extern void  fn_800E0BE4(void* param);
extern void  fn_800E013C(void* param);
extern u32   __cvt_fp2unsigned(f64 val);

/* Floor/field system */
extern void* fn_800F9318(u16 group, u16 model, u16 param);

/* GX rendering */
extern void  GSmodelSetVisibility(void* param);

/* People data layer (people_data.c) */
extern void* fn_801440A0(u16 index);   /* peopleFieldGetByIndex */
extern void* fn_80142CF4(u32 a, u32 b, u32 c, u32 d);  /* peopleFieldAlloc */
extern void  fn_801429E8(void* entry);  /* peopleFieldGetEntry */
extern void  fn_80142984(u32 id);       /* peopleFieldGetByID */

/* Script system */
extern void fn_801621BC(u32* ptr);  /* peopleFieldUtilDispatch - same-TU asm wrapper */

/* ===================================================================
 * All functions in this module are listed above in the MODULE MAP.
 * The asm files in build/GC6E01/asm/ remain the authoritative
 * implementation until each function is individually decompiled.
 * =================================================================== */

/* ===================================================================
 * MusyX runtime: seq.c (sequencer instance / playback control).
 * Reference: AxioDL/musyx `musyx/runtime/seq.c`, cross-verified against
 * byte-exact matched copies in Mario Party 4, Metroid Prime and Mario
 * Strikers (GC/1.3.2). SEQ_INSTANCE / SND_CROSSFADE layouts below are
 * copied field-for-field from Metroid Prime's musyx/seq.h and confirmed
 * byte-identical against this TU's own disassembly (defVGroup @0xEB0,
 * syncCrossInfo @0xEB4, trackVolGroup @0x324, noteUsed/noteKeyOff
 * @0xE64/0xE6C all match exactly). One proven version delta: PRG_STATE
 * here is the pre-2.0.1 4-byte variant (no `program` field) even though
 * this build otherwise sits close to XD 2.0.3 -- proven by the
 * prgState[16] gap arithmetic (0xEB0-0xE70 = 0x40 = 16*4). Struct
 * internals not touched by the functions implemented so far (track[],
 * pattern[], prgState[], event[], section[]) are kept as opaque padding
 * until a later pass needs them.
 * =================================================================== */

typedef struct NOTE {
    struct NOTE* next; // 0x0
    struct NOTE* prev; // 0x4
    u32 id;            // 0x8
    s32 endTime;        // 0xC
    u8 section;          // 0x10
    u8 timeIndex;         // 0x11
    u8 reserved[2];        // 0x12
} NOTE; // size 0x14

typedef struct {
    u32 seqId1;         // 0x0
    u16 time1;           // 0x4
    u16 pad_6;            // 0x6
    u32 seqId2;            // 0x8
    u16 time2;              // 0xC
    u16 pad_E;               // 0xE
    void* arr2;               // 0x10
    u16 gid2;                  // 0x14
    u16 sid2;                    // 0x16
    u8 vol2;                      // 0x18
    u8 studio2;                     // 0x19
    u16 pad_1A;                      // 0x1A
    u32 trackMute2[2];                // 0x1C
    u16 speed2;                        // 0x24
    u8 flags;                            // 0x26
    u8 pad_27;                            // 0x27
} SND_CROSSFADE; // size 0x28

typedef struct {
    volatile u32 time; // 0x0
    u32 bpm;            // 0x4
} MTRACK_DATA;           // size 0x8

typedef struct {
    MTRACK_DATA* base; // 0x0
    MTRACK_DATA* addr; // 0x4
} MTRACK;               // size 0x8

typedef struct {
    u32 low;  // 0x0
    s32 high; // 0x4
} TICKS;      // size 0x8

typedef struct {
    u32 time;       // 0x0
    u8 prgChange;    // 0x4
    u8 velocity;      // 0x5
    u8 res[2];         // 0x6
    u16 pattern;        // 0x8
    s8 transpose;        // 0xA
    s8 velocityAdd;       // 0xB
} TENTRY; // size 0xC

typedef struct {
    TENTRY* base; // 0x0
    TENTRY* addr; // 0x4
} TRACK;          // size 0x8

typedef struct {
    u16 time;     // 0x0
    u8 key;        // 0x2
    u8 velocity;    // 0x3
    u16 length;      // 0x4
} NOTE_DATA;          // size 0x6

typedef struct {
    u8* nextAddr;    // 0x0
    u16 value;        // 0x4
    s16 nextDelta;     // 0x6
    u32 nextTime;       // 0x8
} SEQ_STREAM;            // size 0xC

typedef struct {
    u32 lTime;                // 0x0
    u32 baseTime;              // 0x4
    NOTE_DATA* addr;            // 0x8
    TENTRY* patternInfo;         // 0xC
    SEQ_STREAM pitchBend;          // 0x10
    SEQ_STREAM modulation;           // 0x1C
    u8 midi;                          // 0x28
    u8 pad_29[3];                      // 0x29
} CPAT;                                 // size 0x2C

typedef struct SEQ_EVENT {
    struct SEQ_EVENT* next; // 0x0
    struct SEQ_EVENT* prev;  // 0x4
    u32 time;                 // 0x8
    union {
        TENTRY* trackAddr;
        struct {
            NOTE_DATA* addr; // 0x0
            CPAT* base;       // 0x4
        } pattern;
    } info;               // 0xC
    u8 type;               // 0x14
    u8 trackId;              // 0x15
    u8 pad_16[2];             // 0x16
} SEQ_EVENT;                   // size 0x18

typedef struct {
    u16 macId;     // 0x0
    u8 priority;    // 0x2
    u8 maxVoices;    // 0x3
} PRG_STATE;          // size 0x4 (pre-2.0.1 variant, no `program` field --
                       // proven by prgState[16] gap == 0x40 == 16*4)

typedef struct {
    u16 macro;    // 0x0
    u8 prio;       // 0x2
    u8 maxVoices;   // 0x3
    u8 index;        // 0x4
    u8 reserved;      // 0x5
} PAGE;                // size 0x6

typedef struct {
    u32 tTab;          // 0x0
    u32 pTab;           // 0x4
    u32 tmTab;           // 0x8
    u32 mTrack;           // 0xC
    u32 info;              // 0x10
    u32 loopPoint[16];      // 0x14
    u32 tsTab;               // 0x54
} ARR;                        // size 0x58

#define ARR_GET(arr, offset) ((void*)((offset) + (u32)(arr)))
#define ARR_GET_TYPE(arr, offset, ty) ((ty)ARR_GET(arr, offset))

typedef struct {
    MTRACK mTrack;              // 0x0
    u32 bpm;                     // 0x8
    TICKS tickDelta[2];           // 0xC
    SEQ_EVENT* globalEventRoot;     // 0x1C
    TICKS time[2];                    // 0x20
    u8 timeIndex;                       // 0x30
    u8 pad_31;                            // 0x31
    u16 speed;                              // 0x32
    u16 loopCnt;                              // 0x34
    u8 loopDisable;                             // 0x36
    u8 pad_37;                                    // 0x37
} SEQ_SECTION; // size 0x38

typedef struct SEQ_INSTANCE {
    struct SEQ_INSTANCE* next; // 0x0
    struct SEQ_INSTANCE* prev; // 0x4
    u8 state;                   // 0x8
    u8 index;                    // 0x9
    u16 groupID;                  // 0xA
    u32 publicId;                   // 0xC
    PAGE* normtab;                    // 0x10
    u8 normTrans[128];                  // 0x14
    PAGE* drumtab;                        // 0x94
    u8 drumTrans[128];                     // 0x98
    ARR* arrbase;                            // 0x118
    u32 trackMute[2];                          // 0x11C
    TRACK track[64];                             // 0x124
    u8 trackVolGroup[64];                          // 0x324
    CPAT pattern[64];                                // 0x364
    NOTE* noteUsed[2];                                 // 0xE64
    NOTE* noteKeyOff;                                    // 0xE6C
    PRG_STATE prgState[16];                                // 0xE70
    u8 defVGroup;                                            // 0xEB0
    u8 pad_EB1[3];                                             // 0xEB1
    SND_CROSSFADE syncCrossInfo;                                 // 0xEB4
    u32* syncSeqIdPtr;                                             // 0xEDC
    u8 syncActive;                                                   // 0xEE0
    u8 defStudio;                                                     // 0xEE1
    u8 keyOffCheck;                                                    // 0xEE2
    u8 pad_EE3;                                                         // 0xEE3
    SEQ_EVENT event[64];                                                 // 0xEE4
    u8* trackSectionTab;                                                   // 0x14E4
    SEQ_SECTION section[16];                                                // 0x14E8
} SEQ_INSTANCE; // size 0x1868

extern SEQ_INSTANCE lbl_804285D0[8]; /* seqInstance */
extern NOTE* lbl_8047AF04;           /* noteFree */
extern SEQ_INSTANCE* lbl_8047AF08;   /* cseq (current sequence being processed) */
extern SEQ_INSTANCE* lbl_8047AF0C;   /* seqFreeRoot */
extern SEQ_INSTANCE* lbl_8047AF10;   /* seqPausedRoot */
extern SEQ_INSTANCE* lbl_8047AF14;   /* seqActiveRoot */
extern u32 lbl_8047AF00;             /* curSeqId */

extern void synthSetBpm(u32 bpm, u8 seqId, u8 secIndex);
extern f64 fn_800CE318(f64 a, f64 b); /* fmod (wraps __ieee754_fmod) */
extern f64 floor(f64 x);
extern SEQ_EVENT* fn_801485FC(SEQ_EVENT* event, u8 secIndex, u32* loopFlag); /* HandleEvent */
extern void synthSendKeyOff(u32 id);
extern u8 synthIsFadeOutActive(u8 volGroup);
extern u32 fn_8014D880(u32 id); /* sndFXCheck */
extern u8 lbl_8047AEFC;         /* curFadeOutState */
extern u32 lbl_8047AEF8;        /* seq_next_id */
extern NOTE lbl_804271D0[256];  /* seqNote */
extern u16 lbl_80434910[8][16]; /* seqMIDIPriority */

extern void voiceKillSound(u32 id);
extern void synthVolume(u8 volume, u16 time, u8 volGroup, u8 mode, u32 pubId);

#define SND_SEQ_ERROR_ID 0xFFFFFFFFU
#define SND_SEQ_CROSSFADE_ID 0x80000000U
#define SND_CROSSFADE_SYNC 4
#define SND_CROSSFADE_PAUSENEW 8
#define SND_CROSSFADE_TRACKMUTE 16
#define SND_CROSSFADE_SPEED 32
#define SND_CROSSFADE_MUTE 64
#define SND_CROSSFADE_MUTENEW 128
#define SND_SEQVOL_CONTINUE 0
#define SND_SEQVOL_STOP 1
#define SND_SEQVOL_PAUSE 2
#define SND_SEQVOL_MUTE 3
#define SND_SEQVOL_MODEMASK 0xF

u32 seqGetPrivateId(u32 seqId) {
    SEQ_INSTANCE* si;
    for (si = lbl_8047AF14; si != NULL; si = si->next) {
        if (si->publicId == (seqId & ~SND_SEQ_CROSSFADE_ID)) {
            return si->index | (seqId & SND_SEQ_CROSSFADE_ID);
        }
    }
    for (si = lbl_8047AF10; si != NULL; si = si->next) {
        if (si->publicId == (seqId & ~SND_SEQ_CROSSFADE_ID)) {
            return si->index | (seqId & SND_SEQ_CROSSFADE_ID);
        }
    }
    return SND_SEQ_ERROR_ID;
}

static void StartPause(SEQ_INSTANCE* si);

static void KillNotes(SEQ_INSTANCE* seq) {
    NOTE* n;
    u32 i;

    for (i = 0; i < 2; i++) {
        for (n = seq->noteUsed[i]; n != NULL; n = n->next) {
            voiceKillSound(n->id);
        }
    }

    for (n = seq->noteKeyOff; n != NULL; n = n->next) {
        voiceKillSound(n->id);
    }
}

static void ResetNotes(SEQ_INSTANCE* seq) {
    NOTE* n;
    u32 i;

    for (i = 0; i < 2; i++) {
        n = seq->noteUsed[i];
        if (n != NULL) {
            for (; n->next != NULL; n = n->next) {
            }

            if (lbl_8047AF04 != NULL) {
                n->next = lbl_8047AF04;
                lbl_8047AF04->prev = n;
            }

            lbl_8047AF04 = seq->noteUsed[i];
            seq->noteUsed[i] = NULL;
        }
    }

    n = seq->noteKeyOff;
    if (n != NULL) {
        for (; n->next != NULL; n = n->next) {
        }

        if (lbl_8047AF04 != NULL) {
            n->next = lbl_8047AF04;
            lbl_8047AF04->prev = n;
        }

        lbl_8047AF04 = seq->noteKeyOff;
        seq->noteKeyOff = NULL;
    }
}

static void StartPause(SEQ_INSTANCE* si) {
    if (si->prev != NULL) {
        si->prev->next = si->next;
    } else {
        lbl_8047AF14 = si->next;
    }

    if (si->next != NULL) {
        si->next->prev = si->prev;
    }

    if ((si->next = lbl_8047AF10) != NULL) {
        lbl_8047AF10->prev = si;
    }

    si->prev = NULL;
    lbl_8047AF10 = si;
    si->state = 2;
}

void seqPause(u32 seqId) {
    SEQ_INSTANCE* si;
    seqId = seqGetPrivateId(seqId);

    if (seqId == SND_SEQ_ERROR_ID) {
        return;
    }

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        si = &lbl_804285D0[seqId];
        if (si->state == 1) {
            StartPause(si);
            KillNotes(si);
            ResetNotes(si);
        }
    } else {
        si = &lbl_804285D0[seqId & ~SND_SEQ_CROSSFADE_ID];
        if (si->state != 0) {
            si->syncCrossInfo.flags |= SND_CROSSFADE_PAUSENEW;
        }
    }
}

void seqStop(u32 seqId) {
    SEQ_INSTANCE* si;

    if ((seqId = seqGetPrivateId(seqId)) == SND_SEQ_ERROR_ID) {
        return;
    }

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        si = &lbl_804285D0[seqId];
        switch (si->state) {
        case 1:
            if (si->prev != NULL) {
                si->prev->next = si->next;
            } else {
                lbl_8047AF14 = si->next;
            }

            KillNotes(&lbl_804285D0[seqId]);
            ResetNotes(&lbl_804285D0[seqId]);
            break;
        case 2:
            if (si->prev != NULL) {
                si->prev->next = si->next;
            } else {
                lbl_8047AF10 = si->next;
            }
            break;
        }

        if (si->next != NULL) {
            si->next->prev = si->prev;
        }
        si->state = 0;
        if (lbl_8047AF0C != NULL) {
            lbl_8047AF0C->prev = si;
        }
        si->next = lbl_8047AF0C;
        si->prev = NULL;
        lbl_8047AF0C = si;
    } else {
        si = &lbl_804285D0[seqId & ~SND_SEQ_CROSSFADE_ID];
        if (si->state != 0) {
            si->syncSeqIdPtr = NULL;
        }
    }
}

void seqSpeed(u32 seqId, u16 speed) {
    u32 i;

    seqId = seqGetPrivateId(seqId);

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        for (i = 0; i < 16; i++) {
            lbl_804285D0[seqId].section[i].speed = speed;
        }
    } else {
        seqId &= ~SND_SEQ_CROSSFADE_ID;
        lbl_804285D0[seqId].syncCrossInfo.flags |= SND_CROSSFADE_SPEED;
        lbl_804285D0[seqId].syncCrossInfo.speed2 = speed;
    }
}

void seqContinue(u32 seqId) {
    SEQ_INSTANCE* si;

    seqId = seqGetPrivateId(seqId);

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        si = &lbl_804285D0[seqId];

        if (si->state == 2) {
            if (si->prev != NULL) {
                si->prev->next = si->next;
            } else {
                lbl_8047AF10 = si->next;
            }

            if (si->next != NULL) {
                si->next->prev = si->prev;
            }

            if ((si->next = lbl_8047AF14) != NULL) {
                lbl_8047AF14->prev = si;
            }

            si->prev = NULL;
            lbl_8047AF14 = si;
            si->state = 1;
        }
    } else {
        lbl_804285D0[seqId & ~SND_SEQ_CROSSFADE_ID].syncCrossInfo.flags &= ~SND_CROSSFADE_PAUSENEW;
    }
}

void seqMute(u32 seqId, u32 mask1, u32 mask2) {
    seqId = seqGetPrivateId(seqId);
    if (seqId == SND_SEQ_ERROR_ID) {
        return;
    }

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        lbl_804285D0[seqId].trackMute[0] = mask1;
        lbl_804285D0[seqId].trackMute[1] = mask2;
    } else {
        seqId &= ~SND_SEQ_CROSSFADE_ID;
        lbl_804285D0[seqId].syncCrossInfo.flags |= SND_CROSSFADE_TRACKMUTE;
        lbl_804285D0[seqId].syncCrossInfo.trackMute2[0] = mask1;
        lbl_804285D0[seqId].syncCrossInfo.trackMute2[1] = mask2;
    }
}

void seqVolume(u8 volume, u16 time, u32 seqId, u8 mode) {
    u32 i;
    u32 pub_id;

    pub_id = seqId;
    seqId = seqGetPrivateId(seqId);
    if (seqId == SND_SEQ_ERROR_ID) {
        return;
    }

    if ((seqId & SND_SEQ_CROSSFADE_ID) == 0) {
        synthVolume(volume, time, lbl_804285D0[seqId].defVGroup, mode, pub_id);
        for (i = 0; i < 64; i++) {
            if (lbl_804285D0[seqId].trackVolGroup[i] != lbl_804285D0[seqId].defVGroup) {
                synthVolume(volume, time, lbl_804285D0[seqId].trackVolGroup[i], SND_SEQVOL_CONTINUE,
                            SND_SEQ_ERROR_ID);
            }
        }
    } else {
        seqId &= ~SND_SEQ_CROSSFADE_ID;
        switch (mode & SND_SEQVOL_MODEMASK) {
        case SND_SEQVOL_CONTINUE:
            lbl_804285D0[seqId].syncCrossInfo.vol2 = volume;
            break;
        case SND_SEQVOL_STOP:
            lbl_804285D0[seqId].syncSeqIdPtr = NULL;
            break;
        case SND_SEQVOL_PAUSE:
            lbl_804285D0[seqId].syncCrossInfo.flags |= SND_CROSSFADE_PAUSENEW;
            lbl_804285D0[seqId].syncCrossInfo.vol2 = volume;
            break;
        case SND_SEQVOL_MUTE:
            lbl_804285D0[seqId].syncCrossInfo.flags |= SND_CROSSFADE_MUTENEW;
            lbl_804285D0[seqId].syncCrossInfo.vol2 = volume;
            break;
        default:
            break;
        }
    }
}

#define SND_CROSSFADE_STOP 0
#define SND_CROSSFADE_PAUSE 1
#define SND_CROSSFADE_CONTINUE 2
#define SND_PLAYPARA_TRACKMUTE 0x1
#define SND_PLAYPARA_SPEED 0x2
#define SND_PLAYPARA_VOLUME 0x4
#define SND_PLAYPARA_SEQVOLDEF 0x8
#define SND_PLAYPARA_PAUSE 0x10

typedef struct {
    u32 flags;         // 0x0
    u32 trackMute[2];   // 0x4
    u16 speed;            // 0xC
    u16 volTime;           // 0xE
    u8 volTarget;            // 0x10
    u8 numSeqVolDef;          // 0x11
    u8 pad_12[2];
    void* seqVolDef;            // 0x14
    u8 numFaded;                  // 0x18
    u8 pad_19[3];
    u8* faded;                       // 0x1C
} SND_PLAYPARA; // size 0x20

extern void sndSeqVolume(u8 volume, u16 time, u32 seqId, u8 mode);
extern void sndSeqMute(u32 seqId, u32 mask1, u32 mask2);
extern void sndSeqSpeed(u32 seqId, u16 speed);
extern void fn_8014D648(u32 seqId); /* sndSeqContinue */
extern u32 fn_8015A21C(u16 sgid, u16 sid, void* arrfile, SND_PLAYPARA* para, u32 irqCall,
                        u8 studio); /* seqPlaySong */
extern u32 fn_8015A368(u16 sgid, u16 sid, void* arrfile, SND_PLAYPARA* para,
                        u8 studio); /* sndSeqPlayEx */

void seqCrossFade(SND_CROSSFADE* ci, u32* new_seqId, u8 irq_call) {
    SND_PLAYPARA pp;
    u32 seqId;
    u16 time;

    seqId = seqGetPrivateId(ci->seqId1);

    if ((ci->flags & SND_CROSSFADE_SYNC) != 0) {
        lbl_804285D0[seqId].syncCrossInfo = *ci;
        lbl_804285D0[seqId].syncActive = TRUE;
        lbl_804285D0[seqId].syncSeqIdPtr = new_seqId;
        lbl_804285D0[seqId].syncCrossInfo.flags &= ~SND_CROSSFADE_SYNC;
        *new_seqId = ci->seqId1 | SND_SEQ_CROSSFADE_ID;
        return;
    }

    if (irq_call) {
        time = ci->time1 < 5 ? 5 : ci->time1;
        if ((ci->flags & SND_CROSSFADE_PAUSE) != 0) {
            seqVolume(0, time, ci->seqId1, SND_SEQVOL_PAUSE);
        } else if ((ci->flags & SND_CROSSFADE_MUTE) != 0) {
            seqVolume(0, time, ci->seqId1, SND_SEQVOL_MUTE);
        } else {
            seqVolume(0, time, ci->seqId1, SND_SEQVOL_STOP);
        }
    } else {
        if ((ci->flags & SND_CROSSFADE_PAUSE) != 0) {
            sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_PAUSE);
        } else if ((ci->flags & SND_CROSSFADE_MUTE) != 0) {
            sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_MUTE);
        } else {
            sndSeqVolume(0, ci->time1, ci->seqId1, SND_SEQVOL_STOP);
        }
    }

    if (new_seqId == NULL) {
        return;
    }

    if ((ci->flags & SND_CROSSFADE_CONTINUE) != 0) {
        if (seqGetPrivateId(ci->seqId2) != SND_SEQ_ERROR_ID) {
            if (irq_call) {
                seqContinue(ci->seqId2);
                seqVolume(ci->vol2, ci->time2, ci->seqId2, SND_SEQVOL_CONTINUE);
                if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
                    seqMute(ci->seqId2, ci->trackMute2[0], ci->trackMute2[1]);
                }
                if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
                    seqSpeed(ci->seqId2, ci->speed2);
                }
            } else {
                fn_8014D648(ci->seqId2);
                sndSeqVolume(ci->vol2, ci->time2, ci->seqId2, SND_SEQVOL_CONTINUE);
                if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
                    sndSeqMute(ci->seqId2, ci->trackMute2[0], ci->trackMute2[1]);
                }
                if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
                    sndSeqSpeed(ci->seqId2, ci->speed2);
                }
            }
            *new_seqId = ci->seqId2;
        } else {
            *new_seqId = SND_SEQ_ERROR_ID;
        }
    } else {
        pp.flags = SND_PLAYPARA_VOLUME;
        if ((ci->flags & SND_CROSSFADE_PAUSENEW) != 0) {
            pp.flags |= SND_PLAYPARA_PAUSE;
        }
        if ((ci->flags & SND_CROSSFADE_SPEED) != 0) {
            pp.flags |= SND_PLAYPARA_SPEED;
            pp.speed = ci->speed2;
        }
        if ((ci->flags & SND_CROSSFADE_TRACKMUTE) != 0) {
            pp.flags |= SND_PLAYPARA_TRACKMUTE;
            pp.trackMute[0] = ci->trackMute2[0];
            pp.trackMute[1] = ci->trackMute2[1];
        }
        pp.volTime = ci->time2;
        pp.volTarget = ci->vol2;
        pp.numFaded = 0;
        if (irq_call != 0) {
            if ((*new_seqId = fn_8015A21C(ci->gid2, ci->sid2, ci->arr2, &pp, TRUE, ci->studio2)) !=
                    SND_SEQ_ERROR_ID &&
                (ci->flags & SND_CROSSFADE_MUTENEW) != 0) {
                seqMute(*new_seqId, 0, 0);
            }
        } else {
            if ((*new_seqId = fn_8015A368(ci->gid2, ci->sid2, ci->arr2, &pp, ci->studio2)) !=
                    SND_SEQ_ERROR_ID &&
                (ci->flags & SND_CROSSFADE_MUTENEW) != 0) {
                sndSeqMute(*new_seqId, 0, 0);
            }
        }
    }
}

static SEQ_EVENT* GenerateNextTrackEvent(u8 trackId) {
    TRACK* track;
    CPAT* pattern;
    SEQ_EVENT* ev;
    u32 patternTime;
    u32 pitchTime;
    u32 modTime;

    track = &lbl_8047AF08->track[trackId];
    pattern = &lbl_8047AF08->pattern[trackId];

    if (track->addr != NULL) {
        ev = &lbl_8047AF08->event[trackId];
        ev->trackId = trackId;
        ev->info.pattern.base = pattern;

        if (pattern->addr == NULL) {
        null_pattern_addr:
            if (track->addr->pattern == 0xffff) {
                track->addr = NULL;
                return NULL;
            }

            if (track->addr->pattern == 0xfffe) {
                if (lbl_8047AF08->trackSectionTab == NULL) {
                    if (lbl_8047AF08->section[0].loopDisable) {
                        track->addr = NULL;
                        return NULL;
                    }
                } else if (lbl_8047AF08->section[lbl_8047AF08->trackSectionTab[trackId]].loopDisable) {
                    track->addr = NULL;
                    return NULL;
                }

                ev->type = 3;
                ev->time = track->addr->time;
                track->addr = &track->base[*((u16*)&track->addr->transpose)];
                return ev;
            }

            ev->type = 4;
            ev->time = track->addr->time;
            ev->info.trackAddr = track->addr;
            ++track->addr;
            return ev;
        }

        pitchTime = pattern->pitchBend.nextTime;
        modTime = pattern->modulation.nextTime;

    loop:
        patternTime = pattern->addr->time + pattern->lTime;
        if (patternTime >= pitchTime) {
            goto use_pitch_time;
        }
        if (patternTime >= modTime) {
            goto use_mod_time;
        }
        if (pattern->addr->key == 0xff && pattern->addr->velocity == 0xff) {
            pattern->addr = NULL;
            goto null_pattern_addr;
        }

        ev->info.trackAddr = (TENTRY*)pattern->addr;
        pattern->lTime = patternTime;

        if ((pattern->addr->key & 0x80) != 0) {
            pattern->addr = (NOTE_DATA*)((u8*)pattern->addr + 4);
            goto use_pattern_time;
        }
        if ((pattern->addr->key | pattern->addr->velocity) == 0) {
            pattern->addr = (NOTE_DATA*)((u8*)pattern->addr + 4);
            goto loop;
        }
        ++pattern->addr;

    use_pattern_time:
        ev->type = 0;
        ev->time = patternTime + pattern->baseTime;
        goto end;

    use_pitch_time:
        if (pitchTime < modTime) {
            ev->time = pitchTime + pattern->baseTime;
            ev->type = 2;
            goto end;
        }

    use_mod_time:
        ev->time = modTime + pattern->baseTime;
        ev->type = 1;

    end:
        return ev;
    }

    return NULL;
}

static void InsertGlobalEvent(SEQ_SECTION* section, SEQ_EVENT* event) {
    SEQ_EVENT* el;
    SEQ_EVENT* last_el;

    last_el = NULL;
    el = section->globalEventRoot;
    for (; el != NULL; last_el = el, el = el->next) {
        if (el->time > event->time) {
            event->next = el;
            event->prev = last_el;
            if (last_el != NULL) {
                last_el->next = event;
            } else {
                section->globalEventRoot = event;
            }
            el->prev = event;
            return;
        }
    }

    event->prev = last_el;
    if (last_el != NULL) {
        last_el->next = event;
    } else {
        section->globalEventRoot = event;
    }
    event->next = NULL;
}

static void InitTrackEvents(void) {
    u32 i;
    SEQ_EVENT* ev;

    if (lbl_8047AF08->trackSectionTab == NULL) {
        for (i = 0; i < 0x40; i += 1) {
            if ((ev = GenerateNextTrackEvent(i)) != NULL) {
                InsertGlobalEvent(lbl_8047AF08->section, ev);
            }
        }
    } else {
        for (i = 0; i < 0x40; i += 1) {
            if ((ev = GenerateNextTrackEvent(i)) != NULL) {
                InsertGlobalEvent(lbl_8047AF08->section + lbl_8047AF08->trackSectionTab[i], ev);
            }
        }
    }
}

static void InitTrackEventsSection(u8 secIndex) {
    u32 i;
    SEQ_EVENT* ev;

    if (lbl_8047AF08->trackSectionTab == NULL) {
        for (i = 0; i < 64; i += 1) {
            if ((ev = GenerateNextTrackEvent(i)) != NULL) {
                InsertGlobalEvent(lbl_8047AF08->section, ev);
            }
        }
    } else {
        for (i = 0; i < 64; i += 1) {
            if (secIndex == lbl_8047AF08->trackSectionTab[i] &&
                (ev = GenerateNextTrackEvent(i)) != NULL) {
                InsertGlobalEvent(lbl_8047AF08->section + secIndex, ev);
            }
        }
    }
}

static u32 GetNextEventTime(SEQ_SECTION* section) {
    if (section->globalEventRoot == NULL) {
        return 0;
    }
    return section->globalEventRoot->time;
}

static SEQ_EVENT* GetGlobalEvent(SEQ_SECTION* section) {
    SEQ_EVENT* ev;
    ev = section->globalEventRoot;
    if (ev != NULL && (section->globalEventRoot = ev->next) != NULL) {
        section->globalEventRoot->prev = NULL;
    }
    return ev;
}

static void HandleMasterTrack(u8 secIndex) {
    SEQ_SECTION* section;

    section = &lbl_8047AF08->section[secIndex];
    if (section->mTrack.base != NULL) {
        while (section->mTrack.addr->time != -1) {
            if (section->mTrack.addr->time > section->time[section->timeIndex].high) {
                break;
            }

            if ((lbl_8047AF08->arrbase->info & 0x40000000) != 0) {
                synthSetBpm((section->bpm = section->mTrack.addr->bpm) >> 10, lbl_8047AF00,
                            secIndex);
            } else {
                synthSetBpm(section->mTrack.addr->bpm, lbl_8047AF00, secIndex);
                section->bpm = section->mTrack.addr->bpm << 10;
            }

            ++section->mTrack.addr;
        }
    }
}

static void SetTickDelta(SEQ_SECTION* section, u32 deltaTime) {
    f32 tickDelta = (f32)section->bpm * (f32)deltaTime * (1.f / 40960000.f);
    tickDelta *= (f32)section->speed * (1.f / 256.f);

    section->tickDelta[section->timeIndex].low =
        (u32)(f32)fn_800CE318(tickDelta * 65536.f, 65536.f);
    section->tickDelta[section->timeIndex].high = (s32)(f32)floor(tickDelta);
}

static void RewindMTrack(u8 secIndex, u32 deltaTime) {
    if (lbl_8047AF08->section[secIndex].mTrack.base == NULL) {
        return;
    }
    lbl_8047AF08->section[secIndex].mTrack.addr = lbl_8047AF08->section[secIndex].mTrack.base;
    HandleMasterTrack(secIndex);
    SetTickDelta(lbl_8047AF08->section + secIndex, deltaTime);
}

static u32 HandleTrackEvents(u8 secIndex, u32 deltaTime) {
    SEQ_EVENT* ev;
    u32 loopFlag;
    SEQ_SECTION* section;

    section = &lbl_8047AF08->section[secIndex];
    loopFlag = FALSE;

    while (GetNextEventTime(section) <= section->time[section->timeIndex].high) {
        if ((ev = GetGlobalEvent(section)) == NULL) {
            if (!loopFlag) {
                return FALSE;
            }

            loopFlag = FALSE;
            section->timeIndex ^= 1;
            section->time[section->timeIndex].high = lbl_8047AF08->arrbase->loopPoint[secIndex];
            section->time[section->timeIndex].low = section->time[section->timeIndex ^ 1].low;
            RewindMTrack(secIndex, deltaTime);
            section->loopCnt += 1;
            InitTrackEventsSection(secIndex);
            continue;
        }

        if ((ev = fn_801485FC(ev, secIndex, &loopFlag)) != NULL) {
            InsertGlobalEvent(section, ev);
        }
    }

    return TRUE;
}

static u32 HandleNotes(void) {
    NOTE* note;
    u32 i;

    for (i = 0; i < 2; i++) {
        note = lbl_8047AF08->noteUsed[i];
        if (note != NULL) {
            while (note->endTime <= lbl_8047AF08->section[note->section].time[i].high) {
                synthSendKeyOff(note->id);

                if ((lbl_8047AF08->noteUsed[i] = note->next) != NULL) {
                    lbl_8047AF08->noteUsed[i]->prev = NULL;
                }

                if ((note->next = lbl_8047AF08->noteKeyOff) != NULL) {
                    lbl_8047AF08->noteKeyOff->prev = note;
                }
                lbl_8047AF08->noteKeyOff = note;
                note = lbl_8047AF08->noteUsed[i];

                if (note == NULL) {
                    break;
                }
            }
        }
    }

    return lbl_8047AF08->noteUsed[0] != NULL || lbl_8047AF08->noteUsed[1] != NULL;
}

static void seqFreeKeyOffNote(NOTE* n) {
    if (n->next != NULL) {
        n->next->prev = n->prev;
    }

    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        lbl_8047AF08->noteKeyOff = n->next;
    }

    if ((n->next = lbl_8047AF04) != NULL) {
        lbl_8047AF04->prev = n;
    }

    n->prev = NULL;
    lbl_8047AF04 = n;
}

static void HandleKeyOffNotes(void) {
    NOTE* n;
    NOTE* nn;

    if (!lbl_8047AF08->keyOffCheck) {
        n = lbl_8047AF08->noteKeyOff;
        while (n != NULL) {
            nn = n->next;
            if (n->id != SND_SEQ_ERROR_ID && fn_8014D880(n->id) == SND_SEQ_ERROR_ID) {
                seqFreeKeyOffNote(n);
            }
            n = nn;
        }
    }

    lbl_8047AF08->keyOffCheck = (lbl_8047AF08->keyOffCheck + 1) % 5;
}

void fn_801496A0(u32 deltaTime) {
    u32 i;
    u32 j;
    u32 x;
    u32 eventsActive;
    u32 notesActive;
    SEQ_INSTANCE* si;
    SEQ_INSTANCE* nextSi;

    if (deltaTime == 0) {
        return;
    }

    si = lbl_8047AF14;
    while (si != NULL) {
        nextSi = si->next;
        lbl_8047AF08 = si;
        lbl_8047AF00 = si->index;
        lbl_8047AEFC = synthIsFadeOutActive(si->defVGroup);

        if (lbl_8047AF08->trackSectionTab == NULL) {
            HandleMasterTrack(0);
            SetTickDelta(lbl_8047AF08->section, deltaTime);
            eventsActive = HandleTrackEvents(0, deltaTime);
            notesActive = HandleNotes();
            HandleKeyOffNotes();

            for (i = 0; i < 2; i++) {
                x = lbl_8047AF08->section[0].time[i].low + lbl_8047AF08->section[0].tickDelta[i].low;
                lbl_8047AF08->section[0].time[i].low = x & 0xffff;
                x >>= 16;
                lbl_8047AF08->section[0].time[i].high += x + lbl_8047AF08->section[0].tickDelta[i].high;
            }
        } else {
            eventsActive = 0;
            for (i = 0; i < 16; i++) {
                HandleMasterTrack(i);
                SetTickDelta(&lbl_8047AF08->section[i], deltaTime);
                eventsActive |= HandleTrackEvents(i, deltaTime);
            }
            notesActive = HandleNotes();
            HandleKeyOffNotes();

            for (i = 0; i < 16; i++) {
                for (j = 0; j < 2; j++) {
                    x = lbl_8047AF08->section[i].time[j].low + lbl_8047AF08->section[i].tickDelta[j].low;
                    lbl_8047AF08->section[i].time[j].low = x & 0xffff;
                    x >>= 16;
                    lbl_8047AF08->section[i].time[j].high +=
                        x + lbl_8047AF08->section[i].tickDelta[j].high;
                }
            }
        }

        if (eventsActive == 0 && notesActive == 0) {
            if (si->prev != NULL) {
                si->prev->next = nextSi;
            } else {
                lbl_8047AF14 = nextSi;
            }
            if (nextSi != NULL) {
                nextSi->prev = si->prev;
            }
            ResetNotes(si);
            si->state = 0;
            si->prev = NULL;
            if ((si->next = lbl_8047AF0C) != NULL) {
                lbl_8047AF0C->prev = si;
            }
            lbl_8047AF0C = si;
        }
        si = nextSi;
    }
}

static void ClearNotes(void) {
    NOTE* ln;
    s32 i;

    ln = NULL;
    lbl_8047AF04 = &lbl_804271D0[0];
    for (i = 0; i < 256; i++) {
        lbl_804271D0[i].prev = ln;
        if (ln != NULL) {
            ln->next = &lbl_804271D0[i];
        }
        ln = &lbl_804271D0[i];
    }

    ln->next = NULL;
}

static void InitPublicIds(void) { lbl_8047AEF8 = 0; }

void seqInit(void) {
    u32 i;
    u32 j;

    lbl_8047AF14 = NULL;
    lbl_8047AF10 = NULL;

    for (i = 0; i < 8; i++) {
        if (i == 0) {
            lbl_8047AF0C = &lbl_804285D0[i];
            lbl_804285D0[i].prev = NULL;
        } else {
            lbl_804285D0[i - 1].next = &lbl_804285D0[i];
            lbl_804285D0[i].prev = &lbl_804285D0[i - 1];
        }
        lbl_804285D0[i].index = i;
        lbl_804285D0[i].state = 0;

        for (j = 0; j < 0x10; j++) {
            lbl_80434910[i][j] = 0xffff;
        }
    }
    lbl_804285D0[i - 1].next = NULL;

    ClearNotes();
    InitPublicIds();
}

extern u32 lbl_80434A10[9][16]; /* synthTicksPerSecond */

void synthSetBpm(u32 bpm, u8 seqId, u8 secIndex) {
    if (seqId == 0xff) {
        seqId = 8;
    }
    lbl_80434A10[seqId][secIndex] = ((bpm << 3) * 1536) / 240;
}

s32 maccmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
typedef s32 (*PeopleCmpFn)(u8* a, u8* b);
extern void* sndBSearch(u8* key, u8* base, s32 count, u32 size, PeopleCmpFn cmp);
extern u8 lbl_8043D6F8[];
extern u32 lbl_8047AF98;
extern u8 lbl_8047AF90[8];  /* true .sbss size 0x8 -> @sda21 (was unsized [] => band mis-measured 96.57%) */
extern u8 lbl_8043DEF8[];
extern u32 lbl_8047AF9C;
extern u32 lbl_8047AF8C;
/* Early asm includes predate the symbol-map rename at 0x80162118. */
#define fn_80162118 sndBSearch
#if 0
asm void dataGetMacro(void) {
#include "src/game/people/people_field_fn_8015211C.inc"
}
#else
/* WIP decomp (jun17): functionally-correct REAL C, byte-match tops out at 70.43%.
 * Entire 2nd half (sth/slwi/sndBSearch/epilogue) matches once lbl_8047AF90 is
 * sized [8] (-> @sda21) and `sub` is u32 (-> slwi not clrlslwi). Residual WALL:
 * target loads `count` via indexed `lhzx r5,r4,r6` (base+offset separate); CW
 * here CSEs `lbl_8043D6F8 + idx*4` into one pointer -> `add`+plain `lhz`. The
 * lhzx-vs-add+lhz addressing choice resisted: precomputed-ptr, inline-twice, and
 * named-offset forms all CSE to the same code. Real C is active for coverage;
 * wrapper parked until the lhzx form is cracked. */
u32 dataGetMacro(u32 key) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    u8* p;
    u16 count;
    u32 sub;

    lbl_8047AF98 = (key >> 6) & 0x3FF;
    count = *(u16*)(lbl_8043D6F8 + ((key >> 6) & 0x3FF) * 4);
    if (count == 0) return 0;
    p = lbl_8043D6F8 + ((key >> 6) & 0x3FF) * 4;
    sub = *(u16*)(p + 2);
    *(u16*)(lbl_8047AF90 + 4) = (u16)key;
    lbl_8047AF9C = sub;
    result = sndBSearch(lbl_8047AF90, lbl_8043DEF8 + sub * 8, count, 8, maccmp);
    lbl_8047AF8C = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
#if 0
asm void fn_801521A8(void) {
#include "src/game/people/people_field_fn_801521A8.inc"
}
#else
s32 fn_801521A8(u16* a, u16* b) {
    return (s32)(a[0]) - (s32)(b[0]);
}
#endif
extern void _savegpr_20(void);
extern void _restgpr_20(void);
extern void _savegpr_23(void);
extern void _restgpr_23(void);
extern void _savegpr_24(void);
extern void _restgpr_24(void);
extern void _savegpr_25(void);
extern void _restgpr_25(void);
extern void _savegpr_27(void);
extern void _restgpr_27(void);
extern u8 lbl_80445EF8[];
extern u8 lbl_8043CCF8[];
extern u32 lbl_8047AF88;
extern u32 lbl_8047AF84;
extern u16 lbl_8047AFAA;
#if 0
asm void fn_801521B8(void) {
#include "src/game/people/people_field_fn_801521B8.inc"
}
#else
u32 fn_801521B8(u16 key, u32* out) {
    void* result;
    u8* header;
    u8* table;
    u32 i;

    *(u16*)lbl_80445EF8 = key;
    for (i = 0; i < lbl_8047AFAA; i++) {
        table = lbl_8043CCF8 + i * 0xC;
        result = sndBSearch(lbl_80445EF8, *(u8**)table, *(u16*)(table + 8), 0x20, (PeopleCmpFn)fn_801521A8);
        lbl_8047AF88 = (u32)result;
        if (result != NULL && *(u16*)((u8*)result + 2) != 0xFFFF) {
            header = (u8*)result + 0xC;
            lbl_8047AF84 = (u32)header;
            out[0] = *(u32*)header;
            out[1] = *(u32*)((u8*)result + 8);
            out[3] = 0;
            out[5] = *(u32*)((u8*)result + 0x14);
            out[4] = *(u32*)((u8*)result + 0x10) & 0x00FFFFFF;
            *(u8*)((u8*)out + 0x1C) = (u8)(*(u32*)((u8*)result + 0x10) >> 24);
            if (*(u32*)((u8*)result + 0x1C) != 0) {
                out[2] = *(u32*)table + *(u32*)((u8*)result + 0x1C);
            }
            return 0;
        }
    }
    return (u32)-1;
}
#endif
#if 0
asm void curvecmp(void) {
#include "src/game/people/people_field_curvecmp.inc"
}
#else
s32 curvecmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80438CF8[];
extern u8 lbl_8047AF7C[8];
extern u16 lbl_8047AFA8;
extern u32 lbl_8047AF78;
#if 0
asm void dataGetCurve(void) {
#include "src/game/people/people_field_dataGetCurve.inc"
}
#else
u32 dataGetCurve(u16 arg) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF7C + 4) = arg;
    result = sndBSearch(lbl_8047AF7C, lbl_80438CF8, lbl_8047AFA8, 8, curvecmp);
    lbl_8047AF78 = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
extern u8 lbl_804378F8[];
extern u8 lbl_8047AF70[8];
extern u16 lbl_8047AFA6;
extern u32 lbl_8047AF6C;
#if 0
asm void dataGetKeymap(void) {
#include "src/game/people/people_field_dataGetKeymap.inc"
}
#else
u32 dataGetKeymap(u16 arg) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF70 + 4) = arg;
    result = sndBSearch(lbl_8047AF70, lbl_804378F8, lbl_8047AFA6, 8, curvecmp);
    lbl_8047AF6C = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
#if 0
asm void layercmp(void) {
#include "src/game/people/people_field_layercmp.inc"
}
#else
s32 layercmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80445F18[];
extern u8 lbl_804380F8[];
extern u16 lbl_8047AFA4;
extern u32 lbl_8047AF68;
#if 0
asm void dataGetLayer(void) {
#include "src/game/people/people_field_fn_801523B8.inc"
}
#else
u32 dataGetLayer(u16 arg, u16* out) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_80445F18 + 4) = arg;
    result = sndBSearch(lbl_80445F18, lbl_804380F8, lbl_8047AFA4, 0xc, layercmp);
    lbl_8047AF68 = (u32)result;
    if (result != NULL) {
        *out = *(u16*)((u8*)result + 6);
        return *(u32*)(u32)lbl_8047AF68;
    }
    return 0;
}
#endif

extern u16 lbl_8047AFA0;
extern u16 lbl_8047AFA2;
extern u8 lbl_8043D2F8[];
extern u8 lbl_80445F24[];
#if 0
asm void fxcmp(void) {
#include "src/game/people/people_field_fn_80152434.inc"
}
#else
s32 fxcmp(u16* a, u16* b) {
    return (s32)(a[0]) - (s32)(b[0]);
}
#endif
#if 0
asm void dataGetFX(void) {
#include "src/game/people/people_field_fn_80152444.inc"
}
#else
u32 dataGetFX(u16 key) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    u8* table;
    s32 i;

    *(u16*)lbl_80445F24 = key;
    for (i = 0; i < lbl_8047AFA0; i++) {
        table = lbl_8043D2F8 + i * 8;
        result = sndBSearch(lbl_80445F24, *(u8**)(table + 4), *(u16*)(table + 2), 0xA, fxcmp);
        if (result != NULL) { return (u32)result; }
    }
    return 0;
}
#endif
#if 0
asm void dataInit(void) {
#include "src/game/people/people_field_dataInit.inc"
}
#else
typedef struct { u16 num; u16 subTabIndex; } DataMacMainEntry;

void dataInit(u32 smpBase, u32 smpLength) {
    extern void fn_8016300C(u32 a, u32 b);
    s32 i;

    lbl_8047AFAA = 0;
    lbl_8047AFA8 = 0;
    lbl_8047AFA6 = 0;
    lbl_8047AFA4 = 0;
    lbl_8047AFA0 = 0;
    lbl_8047AFA2 = 0;
    for (i = 0; i < 0x200; i++) {
        ((DataMacMainEntry*)lbl_8043D6F8)[i].num = 0;
        ((DataMacMainEntry*)lbl_8043D6F8)[i].subTabIndex = 0;
    }
    fn_8016300C(smpBase, smpLength);
}
#endif

#undef fn_80162118
extern void inpAddCtrl(void* dst, u32 lowByte, s32 value, u32 repeat, u32 hasUpperByte);
static void MotionSetterCommon(u8* ctx, u32* cmd, u64 initMask, u32 dataOffset, u32 doneMask) {
    u32 comb;
    s32 scale;
    u8 upperByte;

    if (!(*(u64*)(ctx + 0x114) & initMask)) {
        comb = 0;
        *(u64*)(ctx + 0x114) |= initMask;
    } else {
        comb = cmd[1] & 0xFF;
    }
    scale = ((s16)(cmd[0] >> 16) << 16) / 100;
    if (scale < 0) {
        scale -= ((s8)(cmd[1] >> 0x10) << 8) / 100;
    } else {
        scale += ((s8)(cmd[1] >> 0x10) << 8) / 100;
    }
    upperByte = (cmd[1] >> 8) & 0xFF;
    inpAddCtrl(ctx + dataOffset, (cmd[0] >> 8) & 0xFF, scale, comb, upperByte != 0);
    *(u32*)(ctx + 0x214) |= doneMask;
}
#define PF_DEFINE_MOTION_SETTER(name, initMask, dataOffset, doneMask) \
void name(u8* ctx, u32* cmd) { MotionSetterCommon(ctx, cmd, (initMask), (dataOffset), (doneMask)); }
#if 0
asm void fn_80153FEC(void) {
#include "src/game/people/people_field_fn_80153FEC.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80153FEC, 0x00100000ULL, 0x23C, 0x0002u)
#endif
#if 0
asm void fn_801540F0(void) {
#include "src/game/people/people_field_fn_801540F0.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801540F0, 0x00200000ULL, 0x284, 0x0008u)
#endif
#if 0
asm void fn_801541F4(void) {
#include "src/game/people/people_field_fn_801541F4.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801541F4, 0x00400000ULL, 0x2CC, 0x0020u)
#endif
#if 0
asm void fn_801542F8(void) {
#include "src/game/people/people_field_fn_801542F8.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801542F8, 0x02000000ULL, 0x2F0, 0x0040u)
#endif
#if 0
asm void fn_801543FC(void) {
#include "src/game/people/people_field_fn_801543FC.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801543FC, 0x01000000ULL, 0x314, 0x0080u)
#endif
#if 0
asm void fn_80154500(void) {
#include "src/game/people/people_field_fn_80154500.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154500, 0x00800000ULL, 0x35C, 0x0200u)
#endif
#if 0
asm void fn_80154604(void) {
#include "src/game/people/people_field_fn_80154604.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154604, 0x20000000ULL, 0x338, 0x0100u)
#endif
#if 0
asm void fn_80154708(void) {
#include "src/game/people/people_field_fn_80154708.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154708, 0x40000000ULL, 0x380, 0x0400u)
#endif
#if 0
asm void fn_8015480C(void) {
#include "src/game/people/people_field_fn_8015480C.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_8015480C, 0x80000000ULL, 0x3A4, 0x0800u)
#endif
#if 0
asm void fn_80154910(void) {
#include "src/game/people/people_field_fn_80154910.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154910, 0x04000000ULL, 0x260, 0x0004u)
#endif
#if 0
asm void fn_80154A14(void) {
#include "src/game/people/people_field_fn_80154A14.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154A14, 0x08000000ULL, 0x2A8, 0x0010u)
#endif
#undef PF_DEFINE_MOTION_SETTER

extern u32 _GetInputValue(u8* obj, u8* motionBase, u32 p1, u32 p2);
#if 0
asm void inpGetPanning(void) {
#include "src/game/people/people_field_inpGetPanning.inc"
}
#else
/* If flags bit 30 (0x2) is CLEAR: return halfword at 0x25c.
 * If SET: clear bit 30, call _GetInputValue with motion data, return its result. */
u32 inpGetPanning(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x2)) {
        return *(u16*)(obj + 0x25c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x2u;
    return _GetInputValue(obj, obj + 0x23c, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetSurroundPanning(void) {
#include "src/game/people/people_field_inpGetSurroundPanning.inc"
}
#else
/* bit 29 (0x4), offset 0x280, motion at 0x260 */
u32 inpGetSurroundPanning(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x4)) {
        return *(u16*)(obj + 0x280);
    }
    *(u32*)(obj + 0x214) = flags & ~0x4u;
    return _GetInputValue(obj, obj + 0x260, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetPitchBend(void) {
#include "src/game/people/people_field_inpGetPitchBend.inc"
}
#else
/* bit 28 (0x8), offset 0x2a4, motion at 0x284 */
u32 inpGetPitchBend(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x8)) {
        return *(u16*)(obj + 0x2a4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x8u;
    return _GetInputValue(obj, obj + 0x284, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetDoppler(void) {
#include "src/game/people/people_field_inpGetDoppler.inc"
}
#else
/* bit 27 (0x10), offset 0x2c8, motion at 0x2a8 */
u32 inpGetDoppler(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x10)) {
        return *(u16*)(obj + 0x2c8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x10u;
    return _GetInputValue(obj, obj + 0x2a8, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetModulation(void) {
#include "src/game/people/people_field_inpGetModulation.inc"
}
#else
/* bit 26 (0x20), offset 0x2ec, motion at 0x2cc */
u32 inpGetModulation(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x20)) {
        return *(u16*)(obj + 0x2ec);
    }
    *(u32*)(obj + 0x214) = flags & ~0x20u;
    return _GetInputValue(obj, obj + 0x2cc, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetPedal(void) {
#include "src/game/people/people_field_inpGetPedal.inc"
}
#else
/* bit 25 (0x40), offset 0x310, motion at 0x2f0 */
u32 inpGetPedal(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x40)) {
        return *(u16*)(obj + 0x310);
    }
    *(u32*)(obj + 0x214) = flags & ~0x40u;
    return _GetInputValue(obj, obj + 0x2f0, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetPreAuxA(void) {
#include "src/game/people/people_field_inpGetPreAuxA.inc"
}
#else
/* bit 23 (0x100), offset 0x358, motion at 0x338 */
u32 inpGetPreAuxA(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x100)) {
        return *(u16*)(obj + 0x358);
    }
    *(u32*)(obj + 0x214) = flags & ~0x100u;
    return _GetInputValue(obj, obj + 0x338, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetReverb(void) {
#include "src/game/people/people_field_inpGetReverb.inc"
}
#else
/* bit 22 (0x200), offset 0x37c, motion at 0x35c */
u32 inpGetReverb(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x200)) {
        return *(u16*)(obj + 0x37c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x200u;
    return _GetInputValue(obj, obj + 0x35c, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetPreAuxB(void) {
#include "src/game/people/people_field_inpGetPreAuxB.inc"
}
#else
/* bit 21 (0x400), offset 0x3a0, motion at 0x380 */
u32 inpGetPreAuxB(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x400)) {
        return *(u16*)(obj + 0x3a0);
    }
    *(u32*)(obj + 0x214) = flags & ~0x400u;
    return _GetInputValue(obj, obj + 0x380, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetPostAuxB(void) {
#include "src/game/people/people_field_inpGetPostAuxB.inc"
}
#else
/* bit 20 (0x800), offset 0x3c4, motion at 0x3a4 */
u32 inpGetPostAuxB(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x800)) {
        return *(u16*)(obj + 0x3c4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x800u;
    return _GetInputValue(obj, obj + 0x3a4, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void inpGetTremolo(void) {
#include "src/game/people/people_field_inpGetTremolo.inc"
}
#else
/* bit 19 (0x1000), offset 0x3e8, motion at 0x3c8 */
u32 inpGetTremolo(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x1000)) {
        return *(u16*)(obj + 0x3e8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x1000u;
    return _GetInputValue(obj, obj + 0x3c8, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801619E8(void) {
#include "src/game/people/people_field_fn_801619E8.inc"
}
#else
extern u32 lbl_80449390[];
extern u32 lbl_80369CA0[];
extern u8  lbl_804356F4[];
u32 fn_801619E8(u8 idx, u8 r4, u32 r5, u32 r6) {
    u32* row = lbl_80449390 + (u8)r6 * 16;
    u32 t2 = lbl_80369CA0[r4];
    u32 t1 = row[(u8)r5];
    u32 mask = t2 & t1;
    u32 nonzero = ((-mask | mask) >> 31);
    if (nonzero) {
        row[(u8)r5] = t1 & ~t2;
    }
    if (nonzero) {
        return _GetInputValue(NULL, lbl_804356F4 + (u32)idx * 0x90 + (u32)r4 * 0x24, r5, r6);
    } else {
        return *(u16*)(lbl_804356F4 + (u32)idx * 0x90 + (u32)r4 * 0x24 + 0x20);
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void inpTranslateExCtrl(void) {
#include "src/game/people/people_field_fn_80161D20.inc"
}
#else
/* WALL (w_sg2 2026-06-18, measured 99.64% @ opt_level 4): only residual is anonymous
   jumptable @174@ha/l vs named jumptable_80369CB0@ha/l (numeric-vs-named reloc artifact,
   not C-controllable). Real C is active for coverage. NOTE: needs opt_level 4 (opt 0 = 58%). */
u32 inpTranslateExCtrl(u32 r3) {
    u32 key = r3 & 0xFF;
    switch (key) {
    case 0x80: return 0x80;
    case 0x81: return 0x82;
    case 0x82: return 0xa0;
    case 0x83: return 0xa1;
    case 0x84: return 0x83;
    case 0x85: return 0x84;
    case 0x86: return 0xa2;
    case 0x87: return 0xa3;
    case 0x88: return 0xa4;
    default:   return r3;
    }
}
#endif
#pragma pop
extern void fn_80160BDC(void);
extern void fn_801603C0(u32 ctrl, u32 bank, u32 channel, u32 value);
extern u32  salInitDspCtrl(u8 a, u8 b, u8 c);
extern void salExitDspCtrl(void);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80161D90(void) {
#include "src/game/people/people_field_fn_80161D90.inc"
}
#else
u32 fn_80161D90(u8* obj, u32 ctrl) {
    u32 code;
    s32 value;

    code = inpTranslateExCtrl(ctrl) & 0xFF;
    if (code == 0xA0) {
        value = *(s16*)(obj + 0x1C4);
        return (u16)((value * 2) + 0x2000);
    }
    if (code == 0xA1) {
        value = *(s16*)(obj + 0x1D0);
        return (u16)((value * 2) + 0x2000);
    }
    if (obj[0x121] != 0xFF) {
        return inpGetMidiCtrl(ctrl, obj[0x121], obj[0x122]) & 0xFFFF;
    }
    return 0;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80161E8C(void) {
#include "src/game/people/people_field_fn_80161E8C.inc"
}
#else
void fn_80161E8C(u8* obj, u32 ctrl, s32 value) {
    u32 code;
    u32 raw;
    u32 base;
    u32 high;
    u32 low;

    if (value < 0) {
        value = 0;
    }
    if (value > 0x3FFF) {
        value = 0x3FFF;
    }
    raw = ctrl & 0xFF;
    code = inpTranslateExCtrl(ctrl) & 0xFF;
    if (code == 0xA0 || code == 0xA1) {
        return;
    }
    if (obj[0x121] == 0xFF || obj[0x122] == 0xFF) {
        return;
    }

    high = ((u32)value >> 7) & 0xFF;
    low = (u32)value & 0x7F;
    if (raw < 0x40) {
        base = raw & 0x1F;
        fn_801603C0(base, obj[0x121], obj[0x122], high);
        fn_801603C0((base + 0x20) & 0xFF, obj[0x121], obj[0x122], low);
    } else if (raw == 0x80 || raw == 0x81 || raw == 0x84 || raw == 0x85) {
        base = raw & 0xFE;
        fn_801603C0(base, obj[0x121], obj[0x122], high);
        fn_801603C0((base + 1) & 0xFF, obj[0x121], obj[0x122], low);
    } else {
        fn_801603C0(raw, obj[0x121], obj[0x122], high);
    }
}
#endif
#pragma pop
extern u32 lbl_80478BF0;
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162070(void) {
#include "src/game/people/people_field_fn_80162070.inc"
}
#else
u32 fn_80162070(void) {
    u32 temp;

    temp = lbl_80478BF0 * 0xA8351D63;
    lbl_80478BF0 = temp;
    return (temp >> 6) & 0xFFFF;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void sndSin(void) {
#include "src/game/people/people_field_sndSin.inc"
}
#else
extern s16 lbl_80369D20[];
s32 sndSin(u32 angle) {
    u32 a = angle & 0xFFF;
    if (a < 0x400) {
        return lbl_80369D20[(a & 0xFFFF) * 2 / 2];
    }
    if (a < 0x800) {
        u32 idx = 0x3FF - (a & 0x3FF);
        return lbl_80369D20[idx];
    }
    if (a < 0xC00) {
        u32 idx = (a & 0x3FF) * 2 / 2;
        s16 v = lbl_80369D20[idx];
        return (s16)(-v);
    }
    {
        u32 idx = 0x3FF - (a & 0x3FF);
        s16 v = lbl_80369D20[idx];
        return (s16)(-v);
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void sndBSearch(void) {
#include "src/game/people/people_field_fn_80162118.inc"
}
#else
void* sndBSearch(u8* key, u8* base, s32 count, u32 size, PeopleCmpFn cmp) {
    s32 lo, hi, mid;
    s32 r;
    u8* elem;

    if (count != 0) {
        lo = 1;
        hi = count;
        do {
            mid = (lo + hi) >> 1;
            elem = base + size * (mid - 1);
            r = cmp(key, elem);
            if (r == 0) {
                return elem;
            }
            if (r < 0) {
                hi = mid - 1;
            }
            if (((u32)r >> 31) == 0) {
                lo = mid + 1;
            }
        } while (lo <= hi);
    }
    return 0;
}
#endif
#pragma pop
extern u32  OSEnableInterrupts(void);
extern u32  OSGetTick(void);
extern void fn_80098034(void);
extern void fn_800AC0F8(void);
extern void fn_800AC110(void);
extern u32  fn_800AE794(void);
extern void fn_800AE7CC(u32 a);
extern void DSPInit(void);
extern void fn_800AE8A4(void);
extern void fn_800AE8EC(void);
extern u32  fn_800AE92C(void);
extern void DSPAddTask(u8* ptr);
extern void fn_800CE358(void);
extern void fn_8015B250(u32, u32);
extern u32 ReverbHICreate(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
extern u32 ReverbHIModify(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
extern void ReverbHICallback(u32 a, u32 b, u32 c, u8* d);
extern void DCStoreRange(void* addr, u32 nBytes);
extern void fn_800AC02C(u32 a);
extern void AIInitDMA(u8* ptr, u32 size);
extern u32  fn_800ACB44(void);
extern u32  fn_800ACB4C(void);
extern void fn_800AE630(void);
extern u32  fn_800AE78C(void);
extern void synthGetTicksPerSecond(void);
extern void adsrConvertTimeCents(void);
extern void salActivateStudio(void);
extern void fn_8015AAA0(void);
extern void salActivateVoice(u8* ptr, u8 unused2);
extern void salDeactivateVoice(void);
extern void fn_8015D54C(u8* ptr);
extern void fn_8015D5F4(u8* ptr);
extern u8 lbl_80447E60[];
extern void fn_8015D7D0(void);
extern void fn_801629A4(u32 index, u8 value);
extern void fn_801629D0(u32 index, u8 value);
extern void hwSetITDMode(u32 index, u8 flag);
extern void fn_801632B4(u8* dst, u8* src, u32 size, u32 priority, u32 callbackArg0, u32 callbackArg1);
extern void fn_80163490(void);
extern void fn_801634A8(u32 size);
extern void fn_80163794(void);
extern void aramSetUploadCallback(u8* ptr, u32 size);
extern u32  fn_80163810(u32 ptr, u32 size);
extern void fn_80163BCC(u8* a, u32 b);
extern void fn_80163BE4(void);
extern u8   fn_80163CA8();
extern u32  aramGetStreamBufferAddress(u32 idx, u32 *out);
extern void aramFreeStreamBuffer();
extern u32  salInitAi(u32(*fnptr)(void), u32 d, u32 a);
extern void fn_801640C4(void);
extern u32  salExitAi(void);
extern u32  fn_80164148(u32 d);
extern u32  fn_80164204(void);
extern void fn_80164324(void);
extern void hwEnableIrq(void);
extern void hwDisableIrq(void);
extern void hwInitIrq(void);
extern u32 salGetStartDelay(void);
extern u32  fn_801643D8(u32 size);
extern void fn_80164400(u32 a);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801621BC(void) {
#include "src/game/people/people_field_fn_801621BC.inc"
}
#else
void fn_801621BC(u32* ptr) { *ptr <<= 8; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void sndConvertTicks(void) {
#include "src/game/people/people_field_sndConvertTicks.inc"
}
#else
void sndConvertTicks(u32* ptr, u32 divisor) {
    extern u32 synthGetTicksPerSecond(u32 a);
    u32 result = synthGetTicksPerSecond(divisor);
    *ptr = ((*ptr << 16) / result * 0x3E8) >> 5;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwInit(void) {
#include "src/game/people/people_field_fn_80162370.inc"
}
#else
extern u8  lbl_8047B05E;
extern u8  lbl_8047B05F;
extern u32 snd_handle_irq(void);
u32 hwInit(u32 a, u16 b, u32 c, u32 d) {
    extern u32 lbl_8047B028;
    hwInitIrq();
    lbl_8047B05F = 0;
    lbl_8047B05E = 0;
    lbl_8047B028 = 0;
    if (salInitAi(snd_handle_irq, d, a) != 0
     && salInitDspCtrl(b, c, (u32)(d & 1)) != 0
     && fn_80164148(d) != 0) {
        hwEnableIrq();
        fn_801640C4();
        return 0;
    }
    return -1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void hwExit(void) {
#include "src/game/people/people_field_hwExit.inc"
}
#else
void hwExit(void) {
    hwDisableIrq();
    fn_80164204();
    salExitDspCtrl();
    salExitAi();
    hwEnableIrq();
    fn_80164324();
}
#endif
extern u8  lbl_8047B050;
extern u32 lbl_8047B028;
extern u32 lbl_8047B024;
typedef struct PeopleFieldMoveSlot {
    u8 pad_00[0x1C];       /* 0x00 */
    u32 field_1C;          /* 0x1C */
    u8 pad_20[0x4];        /* 0x20 */
    u32 flags_24[0x13];    /* 0x24 */
    u16 field_70;          /* 0x70 */
    u8 pad_72[0x1E];       /* 0x72 */
    u8 field_90;           /* 0x90 */
    u8 pad_91[0x3];        /* 0x91 */
    u32 field_94;          /* 0x94 */
    u32 field_98;          /* 0x98 */
    u8 field_9C;           /* 0x9C */
    u8 pad_9D[0x3];        /* 0x9D */
    u8 field_A0;           /* 0xA0 */
    u8 pad_A1[0x2B];       /* 0xA1 */
    u16 field_CC;          /* 0xCC */
    u16 field_CE;          /* 0xCE */
    u16 field_D0;          /* 0xD0 */
    u16 field_D2;          /* 0xD2 */
    u8 field_D4;           /* 0xD4 */
    u8 pad_D5[0x13];       /* 0xD5 */
    u32 field_E8;          /* 0xE8 */
    u8 active;             /* 0xEC */
    u8 field_ED;           /* 0xED */
    u8 field_EE;           /* 0xEE */
    u8 pad_EF;             /* 0xEF */
    u32 field_F0;          /* 0xF0 */
} PeopleFieldMoveSlot;

#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8016245C(void) {
#include "src/game/people/people_field_fn_8016245C.inc"
}
#else
void fn_8016245C(u8 val) { lbl_8047B050 = val; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162464(void) {
#include "src/game/people/people_field_fn_80162464.inc"
}
#else
u8 fn_80162464(void) { return lbl_8047B050; }
#endif
#pragma pop
extern u32 lbl_8047B024;
u32 fn_8016246C(u32 index) {
    PeopleFieldMoveSlot* entries = (PeopleFieldMoveSlot*)lbl_8047B024;

    return entries[index].active != 0;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8016248C(void) {
#include "src/game/people/people_field_fn_8016248C.inc"
}
#else
void fn_8016248C(u32 val) { lbl_8047B028 = val; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162494(void) {
#include "src/game/people/people_field_fn_80162494.inc"
}
#else
void fn_80162494(u32 index, u32 val) {
    PeopleFieldMoveSlot* entries = (PeopleFieldMoveSlot*)lbl_8047B024;

    entries[index].field_1C = val;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801624A8(void) {
#include "src/game/people/people_field_fn_801624A8.inc"
}
#else
#pragma push
#pragma peephole off
void fn_801624A8(u32 index, u16 value70, void* words74, u32 resetState, u32 value1C, u32 value18, u32 initFlags, u32 setupFlag, u32 unused) {
    typedef struct {
        u8 pad_00[0x18];      /* 0x00 */
        u32 field_18;         /* 0x18 */
        u32 field_1C;         /* 0x1C */
        u8 pad_20[0x4];       /* 0x20 */
        u32 flags_24[0x13];   /* 0x24 */
        u16 field_70;         /* 0x70 */
        u8 pad_72[0x2];       /* 0x72 */
        u32 words_74[0x8];    /* 0x74 */
        u32 field_94;         /* 0x94 */
        u32 field_98;         /* 0x98 */
        u8 field_9C;          /* 0x9C */
        u8 pad_9D[0x3];       /* 0x9D */
        u8 field_A0;          /* 0xA0 */
        u8 pad_A1[0x3];       /* 0xA1 */
        u8 field_A4;          /* 0xA4 */
        u8 pad_A5[0x13];      /* 0xA5 */
        u32 field_B8;         /* 0xB8 */
        u32 field_BC;         /* 0xBC */
        u16 field_C0;         /* 0xC0 */
        u8 pad_C2[0x2];       /* 0xC2 */
        u32 field_C4;         /* 0xC4 */
        u8 pad_C8[0x1C];      /* 0xC8 */
        u8 bytes_E4[4];       /* 0xE4 */
        u8 pad_E8[0x8];       /* 0xE8 */
        u32 field_F0;         /* 0xF0 */
    } PeopleFieldState;
    extern u32 lbl_8047B024;
    extern u8 lbl_8047B050;
    PeopleFieldState* entries = (*(PeopleFieldState* volatile*)&lbl_8047B024);
    PeopleFieldState* entry = &entries[index];
    u32 i;
    u32 flags = 0;
    u32* src = (u32*)words74;

    for (i = 0; i < lbl_8047B050; i++) {
        flags |= entries[0].flags_24[i] & 0x20;
        entries[0].flags_24[i] = 0;
    }

    entry->flags_24[0] = flags;
    entry->field_1C = value1C;
    entry->field_18 = value18;
    entry->field_F0 = 0;
    entry->field_70 = value70;

    entry->words_74[0] = src[0];
    entry->words_74[1] = src[1];
    entry->words_74[2] = src[2];
    entry->words_74[3] = src[3];
    entry->words_74[4] = src[4];
    entry->words_74[5] = src[5];
    entry->words_74[6] = src[6];
    entry->words_74[7] = src[7];

    if (resetState == 0) {
        entry->field_A4 = 0;
        entry->field_B8 = 0;
        entry->field_BC = 0;
        entry->field_C0 = 0x7FFF;
        entry->field_C4 = 0;
    }

    entry->bytes_E4[0] = 0xFF;
    entry->bytes_E4[1] = 0xFF;
    entry->bytes_E4[2] = 0xFF;
    entry->bytes_E4[3] = 0xFF;

    if (initFlags != 0) {
        fn_801629A4(index, 0);
        fn_801629D0(index, 1);
    }

    hwSetITDMode(index, setupFlag);
}
#pragma pop
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwBreak(void) {
#include "src/game/people/people_field_fn_8016265C.inc"
}
#else
void hwBreak(u32 index) {
    extern u32 lbl_8047B024;
    extern u8 lbl_8047B050;
    u32 offset = index * 0xF4;
    PeopleFieldMoveSlot* entry;
    u8* p;

    entry = (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
    if (entry->active == 1 && lbl_8047B050 == 0) {
        entry->field_EE = 1;
    }
    p = (u8*)lbl_8047B024 + offset;
    p += (u32)lbl_8047B050 * 4;
    *(u32*)(p + 0x24) |= 0x20;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801626AC(void) {
#include "src/game/people/people_field_fn_801626AC.inc"
}
#else
#pragma push
#pragma peephole off
void fn_801626AC(u32 index, void* ptr, u32 mode) {
    typedef struct {
        u8 pad_00[0x18];      /* 0x00 */
        u32 field_18;         /* 0x18 */
        u32 field_1C;         /* 0x1C */
        u8 pad_20[0x4];       /* 0x20 */
        u32 flags_24[0x13];   /* 0x24 */
        u16 field_70;         /* 0x70 */
        u8 pad_72[0x2];       /* 0x72 */
        u32 words_74[0x8];    /* 0x74 */
        u32 field_94;         /* 0x94 */
        u32 field_98;         /* 0x98 */
        u8 field_9C;          /* 0x9C */
        u8 pad_9D[0x3];       /* 0x9D */
        u8 field_A0;          /* 0xA0 */
        u8 pad_A1[0x3];       /* 0xA1 */
        u8 field_A4;          /* 0xA4 */
        u8 pad_A5[0x13];      /* 0xA5 */
        u32 field_B8;         /* 0xB8 */
        u32 field_BC;         /* 0xBC */
        u16 field_C0;         /* 0xC0 */
        u8 pad_C2[0x2];       /* 0xC2 */
        u32 field_C4;         /* 0xC4 */
        u8 pad_C8[0x2];       /* 0xC8 */
        u8 field_CA;          /* 0xCA */
        u8 pad_CB[0x19];      /* 0xCB */
        u8 bytes_E4[4];       /* 0xE4 */
        u8 pad_E8[0x8];       /* 0xE8 */
        u32 field_F0;         /* 0xF0 */
    } PeopleFieldState;
    typedef struct {
        u16 field_00;         /* 0x00 */
        u16 field_02;         /* 0x02 */
        u16 field_04;         /* 0x04 */
        u16 field_06;         /* 0x06 */
    } PeopleFieldMode0Args;
    typedef struct {
        u32 field_00;         /* 0x00 */
        u32 field_04;         /* 0x04 */
        u16 field_08;         /* 0x08 */
        u16 field_0A;         /* 0x0A */
    } PeopleFieldMode12Args;
    extern u32 lbl_8047B024;
    extern u8 lbl_8036944C[];
    extern u32 adsrConvertTimeCents(u32);
    PeopleFieldState* entries = (*(PeopleFieldState* volatile*)&lbl_8047B024);
    PeopleFieldState* entry = &entries[index];
    u8 m = (u8)mode;

    switch (m) {
    case 0: {
        PeopleFieldMode0Args* args = ptr;
        u32 v;
        entry->field_A4 = 0;
        entry->field_B8 = args->field_00;
        entry->field_BC = args->field_02;
        v = args->field_04 << 3;
        if (v > 0x7FFF) {
            v = 0x7FFF;
        }
        entry->field_C0 = (u16)v;
        entry->field_C4 = args->field_06;
        break;
    }
    case 1:
    case 2:
        {
        PeopleFieldMode12Args* args = ptr;
        entry->field_A4 = 1;
        entry->field_CA = 0;
        if (m == 1) {
            entry->field_B8 = (u16)adsrConvertTimeCents(args->field_00);
            entry->field_BC = (u16)adsrConvertTimeCents(args->field_04);
            {
                s32 idx = args->field_08 >> 2;
                if ((u32)idx > 0x3FF) {
                    idx = 0x3FF;
                }
                entry->field_C0 = (u16)(0xC1 - lbl_8036944C[idx]);
            }
        } else {
            entry->field_B8 = (u16)args->field_00;
            entry->field_BC = (u16)args->field_04;
            entry->field_C0 = args->field_08;
        }
        entry->field_C4 = args->field_0A;
        }
    }

    entry->flags_24[0] |= 0x10;
}
#pragma pop
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162858(void) {
#include "src/game/people/people_field_fn_80162858.inc"
}
#else
void fn_80162858(u32 index, u32 val1, u32 val2) {
    extern u32 lbl_8047B024;
    u32 offset = index * 0xF4;
    {
        PeopleFieldMoveSlot* entry1 = (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
        entry1->field_94 = val1;
    }
    {
        PeopleFieldMoveSlot* entry2 = (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
        entry2->field_98 = val2;
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162878(void) {
#include "src/game/people/people_field_fn_80162878.inc"
}
#else
u8 fn_80162878(u32 index) {
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    return entries[index].field_9C;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_8016288C(void) {
#include "src/game/people/people_field_fn_8016288C.inc"
}
#else
u8 fn_8016288C(u32 index) {
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    return entries[index].field_90;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801628A0(void) {
#include "src/game/people/people_field_fn_801628A0.inc"
}
#else
u16 fn_801628A0(u32 index) {
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    return entries[index].field_70;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801628B4(void) {
#include "src/game/people/people_field_fn_801628B4.inc"
}
#else
void fn_801628B4(u32 index, u8 val) {
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    entries[index].field_A0 = val;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwStart(void) {
#include "src/game/people/people_field_fn_801628C8.inc"
}
#else
void hwStart(u32 index, u8 unused2) {
#define PF (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024)
    PF[index].field_D4 = lbl_8047B050;
    salActivateVoice((u8*)&PF[index], unused2);
#undef PF
}
#endif
#pragma pop
extern u32 lbl_8047B024;
extern u8 lbl_8047B050;
void hwKeyOff(u32 index) {
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    entries[index].flags_24[lbl_8047B050] |= 0x40;
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwSetPitch(void) {
#include "src/game/people/people_field_fn_8016292C.inc"
}
#else
void hwSetPitch(u32 index, u16 value) {
    typedef struct {
        u8 pad_00[0x24];      /* 0x00 */
        u32 words_24[0x30];   /* 0x24 */
        u8 activeWordIndex;   /* 0xE4 */
        u8 pad_E5[0x0F];      /* 0xE5 */
    } PeopleFieldState;
    extern u32 lbl_8047B024;
    extern u8 lbl_8047B050;
    PeopleFieldState* entries = (*(PeopleFieldState* volatile*)&lbl_8047B024);
    PeopleFieldState* entry = &entries[index];
    u32 scaledValue;

    if ((u16)value >= 0x4000) {
        value = 0x3FFF;
    }

    if (entry->activeWordIndex != 0xFF) {
        scaledValue = (u16)value << 4;
        if (entry->words_24[5 + entry->activeWordIndex] == scaledValue) {
            return;
        }
    }

    scaledValue = (u16)value << 4;
    entry->words_24[5 + lbl_8047B050] = scaledValue;
    entry->words_24[lbl_8047B050] |= 8;
    entry->activeWordIndex = lbl_8047B050;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801629A4(void) {
#include "src/game/people/people_field_fn_801629A4.inc"
}
#else
void fn_801629A4(u32 index, u8 value) {
    extern u16 lbl_80478BF8;
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    entries[index].field_CC = (&lbl_80478BF8)[(u8)value];
    entries[index].flags_24[0] |= 0x100;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801629D0(void) {
#include "src/game/people/people_field_fn_801629D0.inc"
}
#else
void fn_801629D0(u32 index, u8 value) {
    extern u16 lbl_80478C00;
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    entries[index].field_CE = (&lbl_80478C00)[(u8)value];
    entries[index].flags_24[0] |= 0x80;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwSetITDMode(void) {
#include "src/game/people/people_field_fn_801629FC.inc"
}
#else
/* peopleFieldMemSetup: flag==0 marks NPC slot active (set hi bit of +0xF0, +0xD0/+0xD2=0x10);
 * else clears the hi bit. Base array lbl_8047B024 (stride 0xF4, same as fn_801629A4/D0) is
 * re-read per access (volatile reinterpret) to match the target. byte-match verified 22/22. */
void hwSetITDMode(u32 index, u8 flag) {
    extern u32 lbl_8047B024;
#define PF (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024)
    if (flag == 0) {
        PF[index].field_F0 |= 0x80000000;
        PF[index].field_D0 = 0x10;
        PF[index].field_D2 = 0x10;
    } else {
        PF[index].field_F0 &= ~0x80000000u;
    }
#undef PF
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162A58(void) {
#include "src/game/people/people_field_fn_80162A58.inc"
}
#else
void fn_80162A58(u32 index, u32 volumeArg, u32 voiceIndex, f32 a, f32 b, f32 c) {
    u8* entry;
    f32 work[9];
    u32 studioFlag;
    u32 hasPan;
    u32 i;
    u16* panTable;

    entry = (u8*)lbl_8047B024 + index * 0xF4;
    if (a < 0.0f) {
        a = 0.0f;
    }
    if (b < 0.0f) {
        b = 0.0f;
    }
    if (c < 0.0f) {
        c = 0.0f;
    }

    studioFlag = (*(u32*)(lbl_80447E60 + (u32)entry[0xEF] * 0xBC + 0x54) == 1);
    hasPan = (*(u32*)(entry + 0xF0) >> 31) & 1;
    salCalcVolume(volumeArg, work, voiceIndex, a, b, c, hasPan, studioFlag);

    for (i = 0; i < 3; i++) {
        u16* dst;
        u8* dirtyByte;
        u32 dirtyBit;
        s32 v0;
        s32 v1;
        s32 v2;

        dst = (u16*)(entry + 0x4C + i * 6);
        dirtyByte = entry + 0xE5 + i;
        dirtyBit = 1u << i;
        v0 = (s32)(work[i * 3 + 0] * lbl_8047D4DC);
        v1 = (s32)(work[i * 3 + 1] * lbl_8047D4DC);
        v2 = (s32)(work[i * 3 + 2] * lbl_8047D4DC);
        if (*dirtyByte == 0xFF || dst[0] != (u16)v0 || dst[1] != (u16)v1 || dst[2] != (u16)v2) {
            dst[0] = (u16)v0;
            dst[1] = (u16)v1;
            dst[2] = (u16)v2;
            *(u32*)(entry + 0x24) |= dirtyBit;
            *dirtyByte = 0;
        }
    }

    if ((*(u32*)(entry + 0xF0) & 0x7FFFFFFF) != 0) {
        panTable = (u16*)lbl_80273448;
        *(u16*)(entry + 0xD0) = panTable[voiceIndex];
        *(u16*)(entry + 0xD2) = 0x20 - panTable[voiceIndex];
        *(u32*)(entry + 0x24) |= 0x200;
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D18(void) {
#include "src/game/people/people_field_fn_80162D18.inc"
}
#else
void fn_80162D18(u32 index) {
    extern u32 lbl_8047B024;
    extern void salDeactivateVoice(u8* ptr);
    PeopleFieldMoveSlot* entries = (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);

    salDeactivateVoice((u8*)&entries[index]);
}
#endif
#pragma pop
typedef struct PeopleStudioState {
    u8 pad_00[0xAC];       /* 0x00 */
    u32 field_AC;          /* 0xAC */
    u32 field_B0;          /* 0xB0 */
    u32 field_B4;          /* 0xB4 */
    u32 field_B8;          /* 0xB8 */
} PeopleStudioState;

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwSetAUXProcessingCallbacks(void) {
#include "src/game/people/people_field_fn_80162D44.inc"
}
#else
void hwSetAUXProcessingCallbacks(u8 index, u32 a, u32 b, u32 c, u32 d) {
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    entries[(u8)index].field_AC = a;
    entries[(u8)index].field_B4 = b;
    entries[(u8)index].field_B0 = c;
    entries[(u8)index].field_B8 = d;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D6C(void) {
#include "src/game/people/people_field_fn_80162D6C.inc"
}
#else
void fn_80162D6C(void) { salActivateStudio(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D8C(void) {
#include "src/game/people/people_field_fn_80162D8C.inc"
}
#else
void fn_80162D8C(void) { fn_8015AAA0(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162DAC(void) {
#include "src/game/people/people_field_fn_80162DAC.inc"
}
#else
void fn_80162DAC(u8 index, u32 arg1) {
    extern void fn_8015D54C(u8*, u32);
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    fn_8015D54C((u8*)&entries[(u8)index], arg1);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162DE0(void) {
#include "src/game/people/people_field_fn_80162DE0.inc"
}
#else
void fn_80162DE0(u8 index, u32 arg1) {
    extern void fn_8015D5F4(u8*, u32);
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    fn_8015D5F4((u8*)&entries[(u8)index], arg1);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162E14(void) {
#include "src/game/people/people_field_fn_80162E14.inc"
}
#else
/* STAGED SEED (Claude Opus 2026-06-16) — logic verified vs target; NOT 100%.
 * At #pragma optimization_level 4 the prologue is gone (volatile-only leaf)
 * and it reaches ~58-60%. Residual is compute-block (case 0/1/4/5) reg-alloc:
 *   (1) target recomputes the entry base into r3 (`add r3,r6,r5`) then loads
 *       dim_78/dim_20 via r3, freeing r4 for `lo`; ours loads via the saved r4.
 *   (2) target emits `cmplwi r4,2; mulli r3,r0,0xe; bltlr` (return m if lo<2);
 *       ours inverts to `mulli r5,...; ... bgelr` (different reg + polarity).
 * Cracking needs permuter/band lever search on operand order + the base-recompute
 * binding. The TU default for this block was opt 0 (prologue/spill => 18.5%);
 * opt 4 is correct. */
#pragma optimization_level 4
u32 fn_80162E14(u32 idx) {
    /* Local view of the 0xf4-stride people-field entry; only the fields
     * touched here are named. */
    typedef struct PeopleFieldEntry_E14 {
        u8  _00[0x20];   /* 0x00 */
        u32 dim_20;      /* 0x20 */
        u8  _24[0x54];   /* 0x24 */
        u32 dim_78;      /* 0x78 */
        u8  _7C[0x14];   /* 0x7C */
        u8  kind_90;     /* 0x90 */
        u8  _91[0x5B];   /* 0x91 */
        u8  flag_ec;     /* 0xEC */
        u8  _ED[0x07];   /* 0xED ... 0xF4 */
    } PeopleFieldEntry_E14;
    extern u32 lbl_8047B024;
    PeopleFieldEntry_E14* entries = (*(PeopleFieldEntry_E14* volatile*)&lbl_8047B024);
    PeopleFieldEntry_E14* e = &entries[idx];

    if (e->flag_ec != 2) {
        return 0;
    }
    switch (e->kind_90) {
    case 0:
    case 1:
    case 4:
    case 5: {
        u32 big = e->dim_20;
        u32 small = e->dim_78;
        u32 v = (big - (small << 1)) >> 4;
        u32 lo = big & 0xF;
        if (lo < 2) {
            return v * 0xe;
        }
        return lo + v * 0xe - 2;
    }
    case 2:
        return e->dim_20 - (e->dim_78 >> 1);
    case 3:
        return e->dim_20 - e->dim_78;
    default:
        return idx;
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwFlushStream(void) {
#include "src/game/people/people_field_fn_80162EB8.inc"
}
#else
void hwFlushStream(u8* dstBase, u32 srcOffset, u32 size, u32 streamIndex, u32 arg7, u32 arg8) {
    u32 unusedOut;
    u8* srcBase = (u8*)aramGetStreamBufferAddress(streamIndex, &unusedOut);
    u8* dst;

    size += srcOffset & 0x1F;
    srcOffset &= ~0x1F;
    dst = dstBase + srcOffset;
    size = (size + 0x1F) & ~0x1F;

    DCStoreRange(dst, size);
    aramUploadData(dst, srcBase + srcOffset, size, 1, arg7, arg8);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F48(void) {
#include "src/game/people/people_field_fn_80162F48.inc"
}
#else
void fn_80162F48(void) { fn_80163CA8(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F68(void) {
#include "src/game/people/people_field_fn_80162F68.inc"
}
#else
void fn_80162F68(void) { aramFreeStreamBuffer(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F88(void) {
#include "src/game/people/people_field_fn_80162F88.inc"
}
#else
void fn_80162F88(void* a) {
    aramGetStreamBufferAddress((u32)a, 0);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162FAC(void) {
#include "src/game/people/people_field_fn_80162FAC.inc"
}
#else
void fn_80162FAC(void) {}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwFrq2Pitch(void) {
#include "src/game/people/people_field_fn_80162FB0.inc"
}
#else
typedef struct PeopleFieldMoveScale {
    u32 divisor; /* 0x00 */
} PeopleFieldMoveScale;

u32 hwFrq2Pitch(u32 value) {
    PeopleFieldMoveScale* scale = (PeopleFieldMoveScale*)lbl_80434C50;

    return __cvt_fp2unsigned((lbl_8047D4E0 * (f32)value) / (f32)scale->divisor);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8016300C(void) {
#include "src/game/people/people_field_fn_8016300C.inc"
}
#else
void fn_8016300C(u32 a, u32 b) {
    extern void fn_801634A8(u32 x);
    fn_801634A8(b);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163030(void) {
#include "src/game/people/people_field_fn_80163030.inc"
}
#else
void fn_80163030(void) { fn_80163794(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163050(void) {
#include "src/game/people/people_field_fn_80163050.inc"
}
#else
typedef struct PeopleFieldMoveCommand {
    u32 field_00;          /* 0x00 */
    u32 packedSizeWord;    /* 0x04: high byte type, low 24-bit payload */
} PeopleFieldMoveCommand;

void fn_80163050(u32** src, u32* out) {
    extern u32 fn_80163810(u32 a, u32 b);
    PeopleFieldMoveCommand* command = (PeopleFieldMoveCommand*)*src;
    u32 val = command->packedSizeWord;
    u32 type = val >> 24;
    u32 payload = val & 0xFFFFFF;
    switch (type) {
        case 0: case 1: case 4: case 5:
            payload = (payload + 13) / 7 * 4 & ~7u;
            break;
        case 2:
            payload = payload << 1;
            break;
    }
    *out = fn_80163810(*out, payload);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801630E4(void) {
#include "src/game/people/people_field_fn_801630E4.inc"
}
#else
void fn_801630E4(u8* ptr, u32 size) { aramSetUploadCallback(ptr, size); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163104(void) {
#include "src/game/people/people_field_fn_80163104.inc"
}
#else
void fn_80163104(u8* src, u8* dest) {
    extern void fn_80163BCC(u8* a, u32 b);
    PeopleFieldMoveCommand* command = (PeopleFieldMoveCommand*)src;
    u32 val = command->packedSizeWord;
    u32 type = val >> 24;
    u32 payload = val & 0xFFFFFF;
    switch (type) {
        case 0: case 1: case 4: case 5:
            payload = (payload + 13) / 7 * 4 & ~7u;
            break;
        case 2:
            payload = payload << 1;
            break;
    }
    fn_80163BCC(dest, payload);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163188(void) {
#include "src/game/people/people_field_fn_80163188.inc"
}
#else
void fn_80163188(void) { fn_80163490(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801631A8(void) {
#include "src/game/people/people_field_fn_801631A8.inc"
}
#else
void fn_801631A8(void) {}
#endif
#pragma pop
extern u32 lbl_8047B054;
extern u32 lbl_8047B058;
extern u32 lbl_8047B014;
#pragma push
typedef struct { u32 a; u32 b; } fn_801631AC_Pair;

void fn_801631AC(fn_801631AC_Pair* src) {
    *(fn_801631AC_Pair*)&lbl_8047B054 = *src;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801631C0(void) {
#include "src/game/people/people_field_fn_801631C0.inc"
}
#else
void fn_801631C0(void) { lbl_8047B014 = 0; }
#endif
#pragma pop
extern u32 lbl_8047B024;

u32 hwGetVirtualSampleID(u32 index) {
    PeopleFieldMoveSlot* entries = (PeopleFieldMoveSlot*)lbl_8047B024;
    PeopleFieldMoveSlot* entry = &entries[index];

    if (entry->active == 0) {
        return -1;
    } else {
        return entry->field_E8;
    }
}
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801631F4(void) {
#include "src/game/people/people_field_fn_801631F4.inc"
}
#else
u32 fn_801631F4(u32 index) {
    PeopleFieldMoveSlot* entries = (PeopleFieldMoveSlot*)lbl_8047B024;
    u8 v = entries[index].active;
    u32 diff = 1 - v;
    return (u32)(diff == 0);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163214(void) {
#include "src/game/people/people_field_fn_80163214.inc"
}
#else
extern u8 lbl_8044FB90[];
extern u8 lbl_8044FE14[];
typedef struct PFAramQueueEntry {
    u32 request;       /* 0x00 */
    u32 command;       /* 0x04 */
    u32 zero;          /* 0x08 */
    u32 priority;      /* 0x0C */
    u8* dst;           /* 0x10 */
    u8* src;           /* 0x14 */
    u32 size;          /* 0x18 */
    void (*callback)(void*); /* 0x1C */
    u32 callbackArg0;  /* 0x20 */
    u32 callbackArg1;  /* 0x24 */
} PFAramQueueEntry;

typedef struct PFAramQueue {
    PFAramQueueEntry entries[16]; /* 0x000 */
    u8 writeIndex;                /* 0x280 */
    u8 count;                     /* 0x281 */
} PFAramQueue;

void fn_80163214(void *arg) {
    u8 *tbl;
    u32 i;
    if (*(u32*)((u8*)arg + 0xc) == 1) {
        tbl = lbl_8044FE14;
    } else {
        tbl = lbl_8044FB90;
    }
    for (i = 0; i < 16; i++) {
        u8 *entry = tbl + i * 0x28;
        if (arg == (void*)entry) {
            void (*fn)(void*) = *(void(**)(void*))(entry + 0x20);
            if (fn != 0) {
                fn(*(void**)(entry + 0x24));
            }
        }
    }
    tbl[0x281]--;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801632B4(void) {
#include "src/game/people/people_field_fn_801632B4.inc"
}
#else
void fn_801632B4(u8* dst, u8* src, u32 size, u32 priority, u32 callbackArg0, u32 callbackArg1) {
    PFAramQueue* queue;
    PFAramQueueEntry* entry;
    u32 level;

    if (priority != 0) {
        queue = (PFAramQueue*)lbl_8044FE14;
    } else {
        queue = (PFAramQueue*)lbl_8044FB90;
    }

    for (;;) {
        level = OSDisableInterrupts();
        if (queue->count < 16) {
            break;
        }
        OSRestoreInterrupts(level);
    }

    entry = &queue->entries[queue->writeIndex];
    entry->command = 0x2A;
    entry->zero = 0;
    entry->priority = priority != 0;
    entry->dst = dst;
    entry->src = src;
    entry->size = size;
    entry->callback = (void (*)(void*))aramQueueCallback;
    entry->callbackArg0 = callbackArg0;
    entry->callbackArg1 = callbackArg1;
    ARQPostRequest(entry, entry->command, entry->zero, entry->priority, entry->dst, entry->src, entry->size, entry->callback);
    queue->count++;
    queue->writeIndex = (queue->writeIndex + 1) & 0xF;
    OSRestoreInterrupts(level);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern u8 lbl_8044FB90[];
#if 0
asm void fn_80163490(void) {
#include "src/game/people/people_field_fn_80163490.inc"
}
#else
void fn_80163490(void) {
    u8* ptr;

    ptr = lbl_8044FB90;
    while (*(volatile u8*)(ptr + 0x281) != 0) {
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801634A8(void) {
#include "src/game/people/people_field_fn_801634A8.inc"
}
#else
void fn_801634A8(u32 size) {
    u32 base;
    u32 end;
    u8* clearBuf;
    PFAramQueue* lowQueue;
    PFAramQueue* highQueue;

    base = fn_800ACB44();
    clearBuf = (u8*)fn_801643D8(0x500);
    if (clearBuf != NULL) {
        memset(clearBuf, 0, 0x500);
        DCFlushRange(clearBuf, 0x500);
    }

    lowQueue = (PFAramQueue*)lbl_8044FB90;
    highQueue = (PFAramQueue*)lbl_8044FE14;
    lowQueue->writeIndex = 0;
    lowQueue->count = 0;
    highQueue->writeIndex = 0;
    highQueue->count = 0;

    if (clearBuf != NULL) {
        fn_801632B4(clearBuf, (u8*)base, 0x500, 0, 0, 0);
        fn_80163490();
        fn_80164400((u32)clearBuf);
    }

    lbl_8047B07C = base + size;
    end = fn_800ACB4C();
    if (lbl_8047B07C > end) {
        lbl_8047B07C = end;
    }
    lbl_8047B078 = base + 0x500;
    lbl_8047B070 = 0;
    InitStreamBuffers();
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80163794(void) {
#include "src/game/people/people_field_fn_80163794.inc"
}
#else
void fn_80163794(void) {}
#endif
#pragma pop
extern u32 fn_800ACB44(void);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80163798(void) {
#include "src/game/people/people_field_fn_80163798.inc"
}
#else
void fn_80163798(void) { fn_800ACB44(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern u32 lbl_8047B06C;
extern u32 lbl_8047B070;
#if 0
asm void aramSetUploadCallback(void) {
#include "src/game/people/people_field_fn_801637B8.inc"
}
#else
void aramSetUploadCallback(u8* ptr, u32 size) {
    u32 aligned;
    u32 avail;
    if (ptr) {
        aligned = (size + 0x1f) & ~0x1fu;
        avail = fn_800AE78C();
        lbl_8047B06C = aligned < avail ? avail : aligned;
    }
    lbl_8047B070 = (u32)ptr;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80163810(void) {
#include "src/game/people/people_field_fn_80163810.inc"
}
#else
u32 fn_80163810(u32 ptr, u32 size) {
    u8* src;
    u8* uploadPtr;
    u8* (*copyProc)(u8*, u32);
    u32 aligned;
    u32 chunk;
    u32 start;

    src = (u8*)ptr;
    aligned = (size + 0x1F) & ~0x1Fu;
    start = lbl_8047B078;
    if (lbl_8047B070 == 0) {
        DCFlushRange(src, aligned);
        fn_801632B4((u8*)lbl_8047B078, src, aligned, 0, 0, 0);
        lbl_8047B078 += aligned;
        return start;
    }

    copyProc = (u8* (*)(u8*, u32))lbl_8047B070;
    while (aligned != 0) {
        if (aligned > lbl_8047B06C) {
            chunk = lbl_8047B06C;
        } else {
            chunk = aligned;
        }
        uploadPtr = copyProc(src, chunk);
        DCFlushRange(uploadPtr, chunk);
        fn_801632B4((u8*)lbl_8047B078, uploadPtr, chunk, 0, 0, 0);
        lbl_8047B078 += chunk;
        src += chunk;
        aligned -= chunk;
    }
    return start;
}
#endif
#pragma pop
extern u32 lbl_8047B078;
void fn_80163BCC(u8* unused, u32 size) {
    lbl_8047B078 -= (size + 0x1F) & ~0x1F;
}
/* shared free-list node (stride 0x10) + heads; used by 80163BE4/CA8/DB0/DE8 */
typedef struct PFNode { struct PFNode* next; u32 f4; u32 f8; u32 fc; } PFNode;
extern PFNode lbl_80450098[];
extern PFNode *lbl_8047B060, *lbl_8047B064, *lbl_8047B068;
extern u32 lbl_8047B074, lbl_8047B07C;
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163BE4(void) {
#include "src/game/people/people_field_fn_80163BE4.inc"
}
#else
/* peopleFieldMoveSetState: init the 0x40-entry free list (node[i].next=&node[i+1],
 * tail NULL; head ptrs reset). byte-match verified via objdiff. */
void fn_80163BE4(void) {
    u32 i;
    lbl_8047B068 = 0;
    lbl_8047B064 = 0;
    lbl_8047B060 = &lbl_80450098[0];
    for (i = 1; i < 0x40; i++) {
        lbl_80450098[i - 1].next = &lbl_80450098[i];
    }
    lbl_80450098[i - 1].next = 0;
    lbl_8047B074 = lbl_8047B07C;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163CA8(void) {
#include "src/game/people/people_field_fn_80163CA8.inc"
}
#else
/* peopleFieldMoveApplyForce: best-fit allocator over the free list; returns the slot
 * index (u8) or 0xFF. byte-match verified via objdiff. */
u8 fn_80163CA8(u32 size) {
    PFNode* node;
    PFNode* prev;
    PFNode* best;
    u32 bestExcess;
    u32 aligned;

    aligned = (size + 0x1f) & ~0x1fu;
    node = lbl_8047B064;
    best = 0;
    prev = 0;
    bestExcess = (u32)-1;

    while (node != 0) {
        if (node->fc == aligned) {
            best = node;
            goto have_best;
        }
        if (node->fc > aligned) {
            if (bestExcess > node->fc) {
                best = node;
                bestExcess = node->fc;
            }
        }
        prev = node;
        node = node->next;
    }
have_best:

    if (best == 0) {
        PFNode* fh = lbl_8047B060;
        if (fh != 0) {
            if ((u32)(lbl_8047B074 - aligned) >= lbl_8047B078) {
                lbl_8047B060 = fh->next;
                best = fh;
                fh->fc = aligned;
                fh->f8 = aligned;
                lbl_8047B074 = lbl_8047B074 - aligned;
                fh->f4 = lbl_8047B074;
                fh->next = lbl_8047B068;
                lbl_8047B068 = fh;
            }
        }
    } else {
        if (prev != 0) {
            prev->next = best->next;
        } else {
            lbl_8047B064 = best->next;
        }
        best->f8 = aligned;
        best->next = lbl_8047B068;
        lbl_8047B068 = best;
    }

    if (best == 0) {
        return 0xff;
    }
    return (u8)(best - lbl_80450098);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void aramGetStreamBufferAddress(void) {
#include "src/game/people/people_field_fn_80163DB0.inc"
}
#else
u32 aramGetStreamBufferAddress(u32 idx, u32 *out) {
    if (out != 0) {
        *out = lbl_80450098[(u8)idx].f8;
    }
    return lbl_80450098[(u8)idx].f4;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void aramFreeStreamBuffer(void) {
#include "src/game/people/people_field_fn_80163DE8.inc"
}
#else
/* peopleFieldMoveCalcForce: unlink a block, and if it was the active one, recompute the
 * min and recoalesce; else push to the other free list. byte-match verified via objdiff. */
void aramFreeStreamBuffer(u32 idx) {
    PFNode* blk;
    PFNode* cur;
    PFNode* prev;

    blk = &lbl_80450098[(u8)idx];
    cur = lbl_8047B068;
    prev = 0;

    while (cur != 0) {
        if (cur == blk) {
            if (prev != 0) {
                prev->next = blk->next;
            } else {
                lbl_8047B068 = blk->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    if (blk->f4 == lbl_8047B074) {
        u32 mn;
        PFNode* scan;

        blk->next = lbl_8047B060;
        lbl_8047B060 = blk;

        mn = (u32)-1;
        scan = lbl_8047B068;
        while (scan != 0) {
            if (scan->f4 <= mn) {
                mn = scan->f4;
            }
            scan = scan->next;
        }

        scan = lbl_8047B064;
        while (scan != 0) {
            PFNode* nx = scan->next;
            if (scan->f4 < mn) {
                lbl_8047B064 = scan->next;
                scan->next = lbl_8047B060;
                lbl_8047B060 = scan;
            }
            scan = nx;
        }

        lbl_8047B074 = (mn != (u32)-1) ? mn : lbl_8047B07C;
        return;
    }

    blk->next = lbl_8047B064;
    lbl_8047B064 = blk;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163EE0(void) {
#include "src/game/people/people_field_fn_80163EE0.inc"
}
#else
extern u8  lbl_8047B0A0;
extern u32 lbl_8047B09C;
extern u32 lbl_8047B08C;
extern u32 lbl_8047B098;
extern u32 lbl_8047B094;
extern u32 lbl_8047B090;
extern u32 lbl_8047B0A4;
void fn_80163EE0(void) {
    int counter = ((int)lbl_8047B0A0 + 1) % 4;
    u8* ptr = (u8*)(lbl_8047B09C + 0x80000000u) + (u8)counter * 0x280;
    lbl_8047B0A0 = counter;
    AIInitDMA(ptr, 0x280);
    lbl_8047B08C = OSGetTick();
    if (lbl_8047B098 != 0) {
        if (lbl_8047B090 == 0) {
            lbl_8047B090 = 1;
            OSEnableInterrupts();
            ((void(*)(void))lbl_8047B0A4)();
            OSDisableInterrupts();
            lbl_8047B090 = 0;
        }
    } else {
        lbl_8047B094 = 1;
    }
}
#endif
#pragma pop
extern u32 lbl_8047B098;
extern u32 lbl_8047B088;
void fn_80163F88(void) {
    lbl_8047B098 = 1;
    lbl_8047B088 = 1;
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void dspResumeCallback(void) {
#include "src/game/people/people_field_fn_80163F98.inc"
}
#else
void dspResumeCallback(void) {
    lbl_8047B098 = 1;
    if (lbl_8047B094 != 0) {
        lbl_8047B094 = 0;
        if (lbl_8047B090 == 0) {
            lbl_8047B090 = 1;
            OSEnableInterrupts();
            ((void(*)(void))lbl_8047B0A4)();
            OSDisableInterrupts();
            lbl_8047B090 = 0;
        }
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm u32 salInitAi(u32(*fnptr)(void), u32 d, u32 a) {
#include "src/game/people/people_field_fn_80163FFC.inc"
}
#else
/* WALL (Sonnet 5 2026-07-01, measured 99.6%): guard-clause restructure
 * (`if (ptr != 0) { ...; return 1; } return 0;`) plus opt_level 4 fixed the
 * branch polarity/layout and register spill vs base/prior attempt. Residual
 * is register-letter only: target loads the 0/1 immediates into r0/r4 in the
 * opposite pairing from ours (li r4,1/li r0,0 vs our li r4,0/li r0,1), which
 * flips the two adjacent `stw` operand registers. 3 statement-order variants
 * tried (naive, swapped store order, local one/zero temps); none changed the
 * pairing further. Known wall class, not C-controllable. */
u32 salInitAi(u32(*fnptr)(void), u32 d, u32 a) {
    lbl_8047B09C = fn_801643D8(0xA00);
    if (lbl_8047B09C != 0) {
        memset((void*)lbl_8047B09C, 0, 0xA00);
        DCFlushRange((void*)lbl_8047B09C, 0xA00);
        lbl_8047B098 = 1;
        lbl_8047B094 = 0;
        lbl_8047B0A0 = 1;
        lbl_8047B090 = 0;
        lbl_8047B0A4 = (u32)fnptr;
        fn_800AC02C((u32)salCallback);
        AIInitDMA((u8*)(lbl_8047B09C + 0x80000000u) + (u32)lbl_8047B0A0 * 0x280, 0x280);
        ((u32*)lbl_80434C50)[1] = 0x20;
        *(u32*)a = 0x7D00;
        return 1;
    }
    return 0;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801640C4(void) {
#include "src/game/people/people_field_fn_801640C4.inc"
}
#else
void fn_801640C4(void) { fn_800AC0F8(); }
#endif
#pragma pop
u32 salExitAi(void) {
    extern u32 lbl_8047B09C;
    extern void fn_80164400(u32 a);
    fn_800AC02C(0);
    fn_800AC110();
    fn_80164400(lbl_8047B09C);
    return 1;
}
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
#if 0
asm void salAiGetDest(void) {
#include "src/game/people/people_field_fn_80164118.inc"
}
#else
u8* salAiGetDest(void) {
    return (u8*)lbl_8047B09C + (u8)((lbl_8047B0A0 + 2) % 4) * 0x280;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164148(void) {
#include "src/game/people/people_field_fn_80164148.inc"
}
#else
extern u8  lbl_804504A0[];
extern u8  lbl_80450500[];
extern u8  lbl_8036A520[];
extern u16 lbl_80478C08;
u32 fn_80164148(u32 d) {
    u8* s = lbl_804504A0;
    *(u32*)(s + 0x0C) = (u32)lbl_8036A520;
    *(u32*)(s + 0x10) = lbl_80478C08;
    *(u32*)(s + 0x14) = 0;
    *(u32*)(s + 0x18) = (u32)lbl_80450500;
    *(u32*)(s + 0x1C) = 0x2000;
    *(u32*)(s + 0x20) = 0;
    *(u16*)(s + 0x24) = 0x10;
    *(u16*)(s + 0x26) = 0x30;
    *(u32*)(s + 0x28) = (u32)fn_80163F88;
    *(u32*)(s + 0x2C) = (u32)dspResumeCallback;
    *(u32*)(s + 0x30) = 0;
    *(u32*)(s + 0x34) = 0;
    *(u32*)(s + 0x04) = 0;
    DSPInit();
    DSPAddTask(lbl_804504A0);
    lbl_8047B088 = 0;
    hwEnableIrq();
    while (*(volatile u32*)&lbl_8047B088 == 0) {
    }
    hwDisableIrq();
    return 1;
}
#endif
#pragma pop
u32 fn_80164204(void) {
    fn_800AE8EC();
    while (fn_800AE92C() != 0) {}
    fn_800AE8A4();
    return 1;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void salCtrlDsp(void) {
#include "src/game/people/people_field_fn_80164238.inc"
}
#else
extern u32 lbl_8047B010;
extern u16 lbl_8047B00C;
void salCtrlDsp(u32 arg) {
    extern u32 lbl_8047B098;
    extern u32 salGetStartDelay(void);
    fn_8015B250(arg, salGetStartDelay());
    arg = lbl_8047B010;
    lbl_8047B098 = 0;
    fn_80098034();
    fn_800AE7CC((u32)lbl_8047B00C | 0xBABE0000);
    while (fn_800AE794() != 0) {}
    fn_800AE7CC(arg);
    while (fn_800AE794() != 0) {}
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void salGetStartDelay(void) {
#include "src/game/people/people_field_salGetStartDelay.inc"
}
#else
u32 salGetStartDelay(void) {
    extern u32 OSGetTick(void);
    extern u32 lbl_8047B08C;
    u32 tick;
    u32 divisor;
    u32 prev;
    tick = OSGetTick();
    divisor = *(volatile u32*)0x800000F8;
    prev = lbl_8047B08C;
    divisor = __mulhwu(0x431BDE83u, divisor >> 2);
    tick = tick - prev;
    tick = tick << 3;
    return tick / (divisor >> 15);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void hwInitIrq(void) {
#include "src/game/people/people_field_hwInitIrq.inc"
}
#else
void hwInitIrq(void) {
    extern u32 OSDisableInterrupts(void);
    extern u32 lbl_8047B080;
    extern u16 lbl_8047B084;
    lbl_8047B080 = OSDisableInterrupts();
    lbl_8047B084 = 1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80164324(void) {
#include "src/game/people/people_field_fn_80164324.inc"
}
#else
void fn_80164324(void) {}
#endif
#pragma pop
void hwEnableIrq(void) {
    extern u16 lbl_8047B084;
    extern u32 lbl_8047B080;
    lbl_8047B084 = lbl_8047B084 - 1;
    if (lbl_8047B084 == 0) {
        OSRestoreInterrupts(lbl_8047B080);
    }
}
#pragma push
#pragma optimization_level 2
#pragma optimizewithasm off
void hwDisableIrq(void) {
    extern u16 lbl_8047B084;
    extern u32 lbl_8047B080;
    u16 v = lbl_8047B084++;
    if (v == 0) {
        lbl_8047B080 = OSDisableInterrupts();
    }
}
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164398(void) {
#include "src/game/people/people_field_fn_80164398.inc"
}
#else
u32 fn_80164398(void) { return OSDisableInterrupts(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801643B8(void) {
#include "src/game/people/people_field_fn_801643B8.inc"
}
#else
u32 fn_801643B8(void) { return OSEnableInterrupts(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801643D8(void) {
#include "src/game/people/people_field_fn_801643D8.inc"
}
#else
u32 fn_801643D8(u32 size) {
    extern u32 lbl_8047B054;
    return ((u32(*)(u32))lbl_8047B054)(size);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164400(void) {
#include "src/game/people/people_field_fn_80164400.inc"
}
#else
void fn_80164400(u32 a) {
    extern u32 lbl_8047B054;
    u32* base = &lbl_8047B054;
    ((void(*)(u32))base[1])(a);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#pragma scheduling on
#if 0
asm void sndAuxCallbackReverbHI(void) {
#include "src/game/people/people_field_sndAuxCallbackReverbHI.inc"
}
#else
void sndAuxCallbackReverbHI(u8 type, u32* data, u8* obj) {
    switch (type) {
        case 0:
            if (obj[0x1C4] != 0) { break; }
            ReverbHICallback(data[0], data[1], data[2], obj);
            break;
        case 1:
            break;
    }
}
#endif
#pragma pop
u32 sndAuxCallbackUpdateSettingsReverbHI(u8* ptr) {
    extern u32 ReverbHIModify(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
    ptr[0x1C4] = 1;
    ReverbHIModify(
        ptr,
        *(f32*)(ptr + 0x1C8),
        *(f32*)(ptr + 0x1D0),
        *(f32*)(ptr + 0x1CC),
        *(f32*)(ptr + 0x1D4),
        *(f32*)(ptr + 0x1D8),
        *(f32*)(ptr + 0x1DC)
    );
    ptr[0x1C4] = 0;
    return 1;
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void sndAuxCallbackPrepareReverbHI(void) {
#include "src/game/people/people_field_sndAuxCallbackPrepareReverbHI.inc"
}
#else
void sndAuxCallbackPrepareReverbHI(u8* ptr) {
    extern u32 ReverbHICreate(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
    ptr[0x1C4] = 0;
    ReverbHICreate(
        ptr,
        *(f32*)(ptr + 0x1C8),
        *(f32*)(ptr + 0x1D0),
        *(f32*)(ptr + 0x1CC),
        *(f32*)(ptr + 0x1D4),
        *(f32*)(ptr + 0x1D8),
        *(f32*)(ptr + 0x1DC)
    );
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void ReverbHICreate(void) {
#include "src/game/people/people_field_fn_80164520.inc"
}
#else
u32 ReverbHICreate(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6) {
    u32 i;
    u32 bytes;
    u32 ptr;
    u32 delaySamples;
    u32* table;
    f32 damping;

    if (obj == NULL) {
        return 0;
    }
    if (f1 < lbl_8047D4F0 || f1 > lbl_8047D4F4) {
        return 0;
    }
    if (f2 < lbl_8047D4F8 || f2 > lbl_8047D4FC) {
        return 0;
    }
    if (f3 < lbl_8047D4F0 || f3 > lbl_8047D4F4) {
        return 0;
    }
    if (f4 < lbl_8047D4F0 || f4 > lbl_8047D4F4) {
        return 0;
    }
    if (f5 < lbl_8047D4F0 || f5 > lbl_8047D500) {
        return 0;
    }
    if (f6 < lbl_8047D4F0 || f6 > lbl_8047D4F4) {
        return 0;
    }

    memset(obj, 0, 0x1C4);
    table = (u32*)lbl_8036BF00;
    for (i = 0; i < 9; i++) {
        bytes = table[i & 7] * 4;
        if (bytes == 0) {
            bytes = 4;
        }
        ptr = fn_801643D8(bytes);
        if (ptr == 0) {
            return 0;
        }
        memset((void*)ptr, 0, bytes);
        *(u32*)(obj + 0x0C + i * 0x14) = ptr;

        ptr = fn_801643D8(bytes);
        if (ptr == 0) {
            return 0;
        }
        memset((void*)ptr, 0, bytes);
        *(u32*)(obj + 0xC0 + i * 0x14) = ptr;
    }

    *(f32*)(obj + 0x168) = f1;
    *(f32*)(obj + 0x16C) = f2;
    *(f32*)(obj + 0x19C) = f3;
    *(f32*)(obj + 0x1A8) = f6;
    damping = f4;
    if (damping < lbl_8047D510) {
        damping = lbl_8047D510;
    }
    *(f32*)(obj + 0x1A0) = lbl_8047D4F4 - (lbl_8047D510 + lbl_8047D514 * damping);

    if (f5 != lbl_8047D4F0) {
        delaySamples = (u32)(lbl_8047D504 * f5);
        if (delaySamples == 0) {
            delaySamples = 1;
        }
        *(u32*)(obj + 0x1A4) = delaySamples;
        bytes = delaySamples * 4;
        for (i = 0; i < 3; i++) {
            ptr = fn_801643D8(bytes);
            if (ptr == 0) {
                return 0;
            }
            memset((void*)ptr, 0, bytes);
            *(u32*)(obj + 0x1AC + i * 4) = ptr;
            *(u32*)(obj + 0x1B8 + i * 4) = ptr;
        }
    }
    return 1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
/* ReverbHIModify's preserved asm include predates the symbol-map rename. */
#define fn_80164520 ReverbHICreate
#if 0
asm void ReverbHIModify(void) {
#include "src/game/people/people_field_fn_80164A2C.inc"
}
#else
u32 ReverbHIModify(u8* obj, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6) {
    u32 i;
    u32 ptr;

    if (obj == NULL) {
        return 0;
    }
    if (f1 < lbl_8047D4F0 || f1 > lbl_8047D4F4) {
        return 0;
    }
    if (f2 < lbl_8047D4F8 || f2 > lbl_8047D4FC) {
        return 0;
    }
    if (f3 < lbl_8047D4F0 || f3 > lbl_8047D4F4) {
        return 0;
    }
    if (f4 < lbl_8047D4F0 || f4 > lbl_8047D4F4) {
        return 0;
    }
    if (f5 < lbl_8047D4F0 || f5 > lbl_8047D520) {
        return 0;
    }
    if (f6 < lbl_8047D4F0 || f6 > lbl_8047D4F4) {
        return 0;
    }

    for (i = 0; i < 9; i++) {
        ptr = *(u32*)(obj + 0x0C + i * 0x14);
        if (ptr != 0) {
            fn_80164400(ptr);
        }
        ptr = *(u32*)(obj + 0xC0 + i * 0x14);
        if (ptr != 0) {
            fn_80164400(ptr);
        }
    }
    if (*(u32*)(obj + 0x1A4) != 0) {
        for (i = 0; i < 3; i++) {
            ptr = *(u32*)(obj + 0x1AC + i * 4);
            if (ptr != 0) {
                fn_80164400(ptr);
            }
        }
    }
    return ReverbHICreate(obj, f1, f2, f3, f4, f5, f6);
}
#endif
#undef fn_80164520
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80164C40(void) {
#include "src/game/people/people_field_fn_80164C40.inc"
}
#else
void fn_80164C40(s32* a, s32* b, f32 scaleA, f32 scaleB) {
    u32 i;

    if (a == NULL || b == NULL) {
        return;
    }
    for (i = 0; i < 160; i++) {
        a[i] = (s32)((f32)a[i] * scaleA);
        b[i] = (s32)((f32)b[i] * scaleB);
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80164DD0(void) {
#include "src/game/people/people_field_fn_80164DD0.inc"
}
#else
void fn_80164DD0(s32* samples, u8* obj, u32 channel) {
    u32 i;
    f32 wet;
    f32 feedback;
    f32 dry;
    f32 prev;
    f32 value;
    u32 stateOffset;

    if (samples == NULL || obj == NULL) {
        return;
    }
    if (channel > 2) {
        channel = 0;
    }
    wet = *(f32*)(obj + 0x168);
    feedback = *(f32*)(obj + 0x1A0);
    dry = lbl_8047D4F4 - wet;
    stateOffset = 0x18C + channel * 4;
    prev = (f32)*(s32*)(obj + stateOffset);
    for (i = 0; i < 160; i++) {
        value = (f32)samples[i] * dry + prev * wet * feedback;
        samples[i] = (s32)value;
        prev = value;
    }
    *(s32*)(obj + stateOffset) = (s32)prev;
}
#endif
#pragma pop
