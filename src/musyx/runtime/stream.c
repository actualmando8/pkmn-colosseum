/**
 * @file stream.c
 * @brief MusyX runtime streaming (musyx/runtime/stream.c),
 * 0x8014DDD8 - 0x80150C78.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/stream.c`. Boundary evidence: streamInit
 * confirmed at 0x8014DDD8 (simindex seq=1.0 vs MP4/Prime matched copies);
 * sndStreamDeactivate (0x80150564 + 0x714) is reference stream.c's last
 * function and ends exactly at dataInsertKeymap (0x80150C78), synthdata.c's
 * first. All functions asm-only until matched.
 *
 * Version: struct layout + call sites confirm MUSY_VERSION is 2.0.0 or
 * 2.0.1 (nextStreamHandle/hwStreamHandle/lastPSFromBuffer moved to the
 * front of STREAM_INFO per >=2.0.0, but no lpf fields / no SetupVolume /
 * no shadow _off/_len locals in sndStreamARAMUpdate, which are >=2.0.2).
 * STREAM_INFO verified 0x64 bytes/entry (streamInfo[64] = 0x1900 bytes at
 * lbl_80435FF8) via direct field-offset cross-check against the
 * disassembly (state@0xc, type@0xd, hwStreamHandle@0xe, lastPSFromBuffer@
 * 0xf, updateFunction@0x10, buffer@0x14, size@0x18, bytes@0x1c, last@0x20,
 * adpcmInfo@0x24 (0x28 bytes: numCoef u16@0x24, initialPS u8@0x26,
 * loopPS u8@0x27, loopY0/loopY1/coefTab), voice@0x4c, user@0x50, frq@0x54,
 * prio@0x58, vol@0x59, pan@0x5a, span@0x5b, auxa@0x5c, auxb@0x5d,
 * orgPan@0x5e, orgSPan@0x5f, studio@0x60).
 */
#include "dolphin/types.h"

/* ===== Cross-TU MusyX helpers (already ported in sibling runtime files) ===== */
extern void hwSetVolume(u32 v, u8 table, f32 vol, u32 pan, u32 span, f32 auxa, f32 auxb);
extern void hwSetPitch(u32 index, u16 value);
extern void hwStart(u32 index, u8 unused2);
extern void hwDisableIrq(void);
extern void hwEnableIrq(void);
extern void hwFlushStream(u8* dstBase, u32 srcOffset, u32 size, u32 streamIndex, u32 arg7, u32 arg8);
extern void voiceUnblock(u32 voice);

/* Not yet named in symbols.txt (still fn_XXXXXXXX); addresses/roles derived
 * from direct disassembly of this TU's call sites (register setup at each
 * `bl` matches the reference's argument list exactly):
 *   fn_80158328 = voiceBlock(u8 prio) -> voice index or -1
 *   fn_801628B4 = hwSetStreamLoopPS(u32 voice, u8 ps)
 *   fn_80162E14 = hwGetPos(u32 voice) -> current DSP playback position
 *   fn_80162F48 = hwInitStream(u32 len) -> u8 hwStreamHandle (0xFF = fail)
 *   fn_80162F68 = hwExitStream(u8 hwStreamHandle)
 *   fn_80162F88 = hwGetStreamPlayBuffer(u8 hwStreamHandle) -> void*
 *   hwInitSamplePlayback: signature pinned from the 8-register call at
 *     0x8014E03C (v, -1 as u16 pitch, &newsmp, 1, -1, synthVoice[v].id, 1, 1).
 */
extern u32 fn_80158328(u8 prio);
extern void fn_801628B4(u32 voice, u8 ps);
extern u32 fn_80162E14(u32 voice);
extern u8 fn_80162F48(u32 len);
extern void fn_80162F68(u8 hwStreamHandle);
extern void* fn_80162F88(u8 hwStreamHandle);
extern void hwInitSamplePlayback(u32 voice, u16 pitch, void* smp, u32 resetState, u32 unk1C,
                                  u32 unk18, u32 initFlags, u32 setupFlag);

/* ===== Shared engine globals (lbl_ addresses per symbols.txt; names per
 * reference MusyX source given in trailing comments, matching the
 * lbl_/comment convention already used in synth.c) ===== */
typedef struct SynthInfo {
  u32 mixFrq;      // 0x0
  u32 numSamples;  // 0x4
  u8 pad[0x210 - 8];
  u8 voiceNum;     // 0x210
  u8 maxMusic;     // 0x211
  u8 maxSFX;       // 0x212
  u8 studioNum;    // 0x213
} SynthInfo; // size 0x214

extern u8 lbl_80434C50[]; /* synthInfo */
extern u32 lbl_8047AF44;  /* synthFlags */
extern u8* lbl_8047AF48;  /* synthVoice (base ptr; stride 0x404, id field @0xf4) */

#define synthInfo ((SynthInfo*)lbl_80434C50)
#define synthFlags lbl_8047AF44

/* ===== Stream subsystem state (owned by this TU) ===== */
typedef struct SNDADPCMinfo {
  u16 numCoef;       // 0x0
  u8 initialPS;      // 0x2
  u8 loopPS;         // 0x3
  s16 loopY0;        // 0x4
  s16 loopY1;         // 0x6
  s16 coefTab[8][2]; // 0x8, size 0x20
} SNDADPCMinfo; // size 0x28

typedef struct SND_ADPCMSTREAM_INFO {
  s16 coefTab[8][2]; // 0x0, public-facing struct passed by callers - NOT the
                     // same layout as the internal SNDADPCMinfo embedded in
                     // STREAM_INFO (which has numCoef/initialPS/loopPS/loopY0/
                     // loopY1 before coefTab).
} SND_ADPCMSTREAM_INFO;

typedef struct SAMPLE_INFO {
  u32 info;        // 0x0
  void* addr;      // 0x4
  void* extraData; // 0x8
  u32 offset;      // 0xc
  u32 length;      // 0x10
  u32 loop;        // 0x14
  u32 loopLength;  // 0x18
  u8 compType;     // 0x1c
} SAMPLE_INFO; // size 0x20

typedef struct STREAM_INFO {
  u32 nextStreamHandle; // 0x0
  u32 stid;             // 0x4
  u32 flags;            // 0x8
  u8 state;             // 0xc
  u8 type;               // 0xd
  u8 hwStreamHandle;      // 0xe
  u8 lastPSFromBuffer;    // 0xf
  u32 (*updateFunction)(void* buffer1, u32 len1, void* buffer2, u32 len2, u32 user); // 0x10
  s16* buffer;          // 0x14
  u32 size;             // 0x18
  u32 bytes;            // 0x1c
  u32 last;             // 0x20
  SNDADPCMinfo adpcmInfo; // 0x24, size 0x28
  volatile u32 voice;   // 0x4c
  u32 user;             // 0x50
  u32 frq;              // 0x54
  u8 prio;              // 0x58
  u8 vol;               // 0x59
  u8 pan;               // 0x5a
  u8 span;              // 0x5b
  u8 auxa;              // 0x5c
  u8 auxb;              // 0x5d
  u8 orgPan;            // 0x5e
  u8 orgSPan;           // 0x5f
  u8 studio;            // 0x60
} STREAM_INFO; // size 0x64 (rounded up from 0x61)

STREAM_INFO lbl_80435FF8[64]; /* streamInfo */
u32 lbl_8047AF60;             /* nextPublicID */
u8 lbl_8047AF64;              /* streamCallDelay (declared before streamCallCnt in the
                                * <=2.0.2 branch of reference stream.c, hence lower address) */
u8 lbl_8047AF65;              /* streamCallCnt */

#define streamInfo lbl_80435FF8
#define nextPublicID lbl_8047AF60
#define streamCallDelay lbl_8047AF64
#define streamCallCnt lbl_8047AF65

/* Forward decls: called before their definition further down this TU,
 * in address order per the campaign method (compiler auto-inlines
 * same-TU callees; declarations just satisfy the C89 need-before-use
 * rule without affecting codegen order). */
u32 sndStreamAllocLength(u32 num, u32 flags);
u32 sndStreamActivate(u32 stid);
void sndStreamDeactivate(u32 stid);

/* Improved 2026-07-14 from ~59% to 91.17%: caching `synthInfo->voiceNum`
 * into a local `s32 n` (declared before the loop var `i`, also s32) up
 * front makes MWCC's unroller pick the SAME by-8 group-count preamble as
 * the target (cmpwi/subi/addi/srwi/mtctr chain matches exactly, and the
 * post-loop `subf`/`cmpw` remainder-tracking sequence now matches too).
 * Remaining gap is register-allocation only: target reloads
 * `synthInfo->voiceNum` a second time (fresh `lbz`) after the unrolled
 * block for the remainder-loop bound instead of reusing the cached
 * value, plus a handful of r3/r4/r5/r6/r7 swaps in the zero-store
 * unrolled body. Tried and reverted: u8 loop var (26.91%), scoped
 * `#pragma peephole off` (36.62%), pointer-bump body via STREAM_INFO*
 * si (58.3%), i++ vs ++i (no change). Don't re-grind past 91.17% without
 * a new lever for the compiler-forced double-load of a memory-resident
 * loop bound. */
void fn_8014DDD8(void) {
  s32 n;
  s32 i;
  streamCallCnt = 0;
  streamCallDelay = 3;
  n = synthInfo->voiceNum;
  for (i = 0; i < n; ++i) {
    streamInfo[i].state = 0;
  }
  nextPublicID = 0;
}

static void SetHWMix(const STREAM_INFO* si) {
  hwSetVolume(si->voice, 0, si->vol * (1 / 127.f), (si->pan << 16), (si->span << 16),
              si->auxa * (1 / 127.f), si->auxb * (1 / 127.f));
}

void fn_8014DF20(void) {
  u32 i;              // r28
  u32 cpos;           // r30 / r3
  u32 len;            // r3 (updateFunction result)
  SAMPLE_INFO newsmp; // stack
  STREAM_INFO* si;    // r28 (base, incremented)
  f32 f;
  u32 v;

  if (streamCallCnt != 0) {
    --streamCallCnt;
    return;
  }
  streamCallCnt = streamCallDelay;
  si = &streamInfo[0];
  for (i = 0; i < synthInfo->voiceNum; ++i, ++si) {
    switch (si->state) {
    case 1:
      newsmp.info = si->frq | 0x40000000;
      newsmp.addr = fn_80162F88(si->hwStreamHandle);
      newsmp.offset = 0;
      newsmp.length = si->size;
      newsmp.loop = 0;
      newsmp.loopLength = si->size;

      switch (si->type) {
      case 0:
        newsmp.compType = 2;
        break;
      case 1:
        newsmp.extraData = &si->adpcmInfo;
        newsmp.compType = 4;

        fn_801628B4(si->voice, si->lastPSFromBuffer);
        si->adpcmInfo.loopPS = si->adpcmInfo.initialPS = si->lastPSFromBuffer;
        break;
      }

      v = si->voice;
      hwInitSamplePlayback(v, -1, &newsmp, 1, -1, *(u32*)(lbl_8047AF48 + v * 0x404 + 0xF4), 1, 1);

      f = (si->frq / (f32)synthInfo->mixFrq);
      hwSetPitch(si->voice, (u16)(f * 4096.f));
      SetHWMix(si);

      hwStart(si->voice, si->studio);
      si->state = 2;
      if (!(si->flags & 0x20000)) {
        hwFlushStream((u8*)si->buffer, 0, si->bytes, si->hwStreamHandle, 0, 0);
      }
      break;
    case 2: {
      cpos = fn_80162E14(si->voice);

      if (si->type == 1) {
        cpos = (cpos / 14) * 14;
      }

      if (si->last != cpos) {
        if (si->last < cpos) {
          switch (si->type) {
          case 0: {
            if ((len = si->updateFunction(si->buffer + si->last, cpos - si->last, 0, 0,
                                           si->user)) != 0 &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & 0x20000)) {
                if (cpos != 0) {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (cpos - si->last) * 2,
                                si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (si->size - si->last) * 2,
                                si->hwStreamHandle, 0, 0);
                }
              }

              si->last = cpos;
            }
          } break;
          case 1: {
            u32 off = (si->last / 14) * 8;
            if ((len = si->updateFunction((u8*)si->buffer + off, cpos - si->last, 0, 0,
                                           si->user)) != 0 &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;

              if (!(si->flags & 0x20000)) {
                if (cpos != 0) {
                  hwFlushStream((u8*)si->buffer, off, ((cpos + 13) / 14) * 8 - off,
                                si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, off, si->bytes - off, si->hwStreamHandle, 0, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          }
        } else if (cpos == 0) {
          switch (si->type) {
          case 0:
            if ((len = si->updateFunction(si->buffer + si->last, si->size - si->last, 0, 0,
                                           si->user)) &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & 0x20000)) {
                if (cpos == 0) {
                  hwFlushStream((u8*)si->buffer, si->last * 2, si->bytes - (si->last * 2),
                                si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (cpos - si->last) * 2,
                                si->hwStreamHandle, 0, 0);
                }
              }
              si->last = cpos;
            }
            break;
          case 1: {
            u32 off = ((si->last / 14) * 8);
            if ((len = si->updateFunction((u8*)si->buffer + off, si->size - si->last, 0, 0,
                                           si->user)) &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;
              if (!(si->flags & 0x20000)) {
                if (cpos == 0) {
                  hwFlushStream((u8*)si->buffer, off, si->bytes - off, si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, off, ((cpos + 13) / 14) * 8 - off,
                                si->hwStreamHandle, 0, 0);
                }
              }
              si->last = cpos;
            }
          } break;
          }
        } else {
          switch (si->type) {
          case 0:
            if ((len = si->updateFunction(si->buffer + si->last, si->size - si->last,
                                           si->buffer, cpos, si->user)) &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;

              if (!(si->flags & 0x20000)) {
                if (len > si->size - si->last) {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (si->bytes - si->last * 2),
                                si->hwStreamHandle, 0, 0);
                  hwFlushStream((u8*)si->buffer, 0, cpos * 2, si->hwStreamHandle, 0, 0);

                } else if (cpos == 0) {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (si->bytes - si->last * 2),
                                si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, si->last * 2, (cpos - si->last) * 2,
                                si->hwStreamHandle, 0, 0);
                }
              }
              si->last = cpos;
            }
            break;
          case 1: {
            u32 off = (si->last / 14) * 8;
            if ((len = si->updateFunction((u8*)si->buffer + off, si->size - si->last, si->buffer,
                                           cpos, si->user)) &&
                si->state == 2) {
              cpos = (si->last + len) % si->size;

              if (!(si->flags & 0x20000)) {
                if (len > si->size - si->last) {
                  hwFlushStream((u8*)si->buffer, off, si->bytes - off, si->hwStreamHandle, 0, 0);
                  hwFlushStream((u8*)si->buffer, 0, (cpos / 14) << 3, si->hwStreamHandle, 0, 0);
                } else if (cpos == 0) {
                  hwFlushStream((u8*)si->buffer, off, si->bytes - off, si->hwStreamHandle, 0, 0);
                } else {
                  hwFlushStream((u8*)si->buffer, off, ((cpos + 13) / 14) * 8 - off,
                                si->hwStreamHandle, 0, 0);
                }
              }
              si->last = cpos;
            }

          } break;
          }
        }

        if (si->state == 2 && !(si->flags & 0x20000) && si->type == 1) {
          fn_801628B4(si->voice,
                      (si->lastPSFromBuffer = *(u32*)((u8*)si->buffer + 0x40000000) >> 24));
        }
      }
    } break;
    }
  }
}

void fn_8014E7CC(void) {}

void fn_8014E7D0(u32 voice) {
  STREAM_INFO* si;
  si = &streamInfo[voice];
  switch (si->state) {
  case 1:
  case 2:
    if (si->state == 2) {
      voiceUnblock(si->voice);
    }
    si->state = 3;
    si->updateFunction(0, 0, 0, 0, si->user);
    break;
  default:
    break;
  }
}

u32 GetPrivateIndex(u32 publicID) {
  u32 i;
  for (i = 0; i < 64; ++i) {
    if (streamInfo[i].state != 0 && publicID == streamInfo[i].stid) {
      return i;
    }
  }

  return -1;
}

static u32 GeneratePublicID(void) {
  u32 id;
  u32 i;

  do {
    if ((id = nextPublicID++) == (u32)-1) {
      id = nextPublicID;
      nextPublicID = id + 1;
    }
    for (i = 0; i < 64; ++i) {
      if (streamInfo[i].state != 0 && id == streamInfo[i].stid) {
        break;
      }
    }
  } while (i != 64);

  return id;
}

void fn_8014E9B4(u32 stid, u32 off1, u32 len1, u32 off2, u32 len2) {
  u32 i;

  hwDisableIrq();
  i = GetPrivateIndex(stid);
  if (i != (u32)-1) {
    switch (streamInfo[i].type) {
    case 0:
      off1 *= 2;
      len1 *= 2;
      off2 *= 2;
      len2 *= 2;
      break;
    case 1:
      off1 = (off1 / 14) * 8;
      len1 = ((len1 + 13) / 14) * 8;
      off2 = (off2 / 14) * 8;
      len2 = ((len2 + 13) / 14) * 8;
      break;
    }

    if (len1 != 0) {
      hwFlushStream((u8*)streamInfo[i].buffer, off1, len1, streamInfo[i].hwStreamHandle, 0, 0);
    }

    if (len2 != 0) {
      hwFlushStream((u8*)streamInfo[i].buffer, off2, len2, streamInfo[i].hwStreamHandle, 0, 0);
    }

    if (streamInfo[i].type == 1) {
      streamInfo[i].lastPSFromBuffer =
          (*(u32*)((u8*)streamInfo[i].buffer + 0x40000000)) >> 24;
      if (streamInfo[i].voice != (u32)-1) {
        fn_801628B4(streamInfo[i].voice, streamInfo[i].lastPSFromBuffer);
      }
    }
  }
  hwEnableIrq();
}

void CheckOutputMode(u8* pan, u8* span) {
  if (synthFlags & 1) {
    *pan = 64;
    *span = 0;
  } else if (!(synthFlags & 2)) {
    *span = 0;
  }
}

void fn_8014ECCC(STREAM_INFO* si, u8 vol, u8 pan, u8 span, u8 auxa, u8 auxb) {
  si->orgPan = pan;
  si->orgSPan = span;
  CheckOutputMode(&pan, &span);
  si->vol = vol;
  si->pan = pan;
  si->span = span;
  si->auxa = auxa;
  si->auxb = auxb;
}

void streamOutputModeChanged(void) {
  u32 i;

  hwDisableIrq();
  for (i = 0; i < synthInfo->voiceNum; ++i) {
    if (streamInfo[i].state != 0) {
      streamInfo[i].pan = streamInfo[i].orgPan;
      streamInfo[i].span = streamInfo[i].orgSPan;
      CheckOutputMode(&streamInfo[i].pan, &streamInfo[i].span);
      if (streamInfo[i].state != 3) {
        SetHWMix(&streamInfo[i]);
      }
    }
  }

  hwEnableIrq();
}

u32 fn_8014EE40(u8 prio, void* buffer, u32 samples, u32 frq, u8 vol, u8 pan, u8 span, u8 auxa,
                u8 auxb, u8 studio, u32 flags,
                u32 (*updateFunction)(void* buffer1, u32 len1, void* buffer2, u32 len2, u32 user),
                u32 user, SND_ADPCMSTREAM_INFO* adpcmInfo) {
  u32 stid;
  u32 i;
  u32 bytes;
  u32 j;

  hwDisableIrq();

  for (i = 0; i < 64; ++i) {
    if (streamInfo[i].state == 0) {
      break;
    }
  }

  if (i != 64) {
    stid = GeneratePublicID();
    streamInfo[i].stid = stid;
    streamInfo[i].flags = flags;
    bytes = sndStreamAllocLength(samples, flags);
    streamInfo[i].buffer = (s16*)buffer;
    streamInfo[i].size = samples;
    streamInfo[i].bytes = bytes;
    streamInfo[i].updateFunction = updateFunction;
    streamInfo[i].voice = (u32)-1;
    if (flags & 1) {
      if (adpcmInfo != 0) {
        for (j = 0; j < 8; j++) {
          streamInfo[i].adpcmInfo.coefTab[j][0] = adpcmInfo->coefTab[j][0];
          streamInfo[i].adpcmInfo.coefTab[j][1] = adpcmInfo->coefTab[j][1];
        }
        streamInfo[i].adpcmInfo.numCoef = 8;
      }
      streamInfo[i].type = 1;
    } else {
      streamInfo[i].type = 0;
    }

    streamInfo[i].frq = frq;
    streamInfo[i].studio = studio;
    streamInfo[i].prio = prio;
    fn_8014ECCC(&streamInfo[i], vol, pan, span, auxa, auxb);
    streamInfo[i].user = user;
    streamInfo[i].nextStreamHandle = (u32)-1;
    streamInfo[i].state = 3;
    if ((streamInfo[i].hwStreamHandle = fn_80162F48(bytes)) != 0xFF) {
      if (!(flags & 0x10000) && !sndStreamActivate(stid)) {
        stid = (u32)-1;
      }
    } else {
      stid = (u32)-1;
    }
    if (stid == (u32)-1) {
      streamInfo[i].state = 0;
    }
  } else {
    stid = (u32)-1;
  }

  hwEnableIrq();

  return stid;
}

u32 sndStreamAllocLength(u32 num, u32 flags) {
  if (flags & 1) {
    return (((num + 13) / 14) * 8 + 31) & ~31;
  }

  return (num * 2 + 31) & ~31;
}

void fn_8014F2DC(u32 stid, u8 vol, u8 pan, u8 span, u8 auxa, u8 auxb) {
  u32 i;
  hwDisableIrq();
  i = GetPrivateIndex(stid);
  if (i != (u32)-1) {
    fn_8014ECCC(&streamInfo[i], vol, pan, span, auxa, auxb);
    if (streamInfo[i].state == 2) {
      SetHWMix(&streamInfo[i]);
    }

    if (streamInfo[i].nextStreamHandle != (u32)-1) {
      fn_8014F2DC(streamInfo[i].nextStreamHandle, vol, pan, span, auxa, auxb);
    }
  }

  hwEnableIrq();
}

/* PARKED at 98.9% (2026-07-02): logic and control flow are exactly right
 * (MWCC's self-recursion partial-inlining depth matched via
 * `#pragma inline_depth(4)`, tried 1/2/3/5 -- 4 is the closest, giving
 * only 2 register-allocation swaps left, e.g. target picks r25 where we
 * pick r26 for one temp deep inside the 3rd unrolled recursion level.
 * No instruction/branch shape differs -- pure callee-saved-register
 * numbering preference by MWCC's allocator that source-level reordering
 * (locals, statement order) did not shift. */
#pragma push
#pragma inline_depth(4)
void sndStreamFree(u32 stid) {
  u32 i;
  hwDisableIrq();
  i = GetPrivateIndex(stid);
  if (i != (u32)-1) {
    sndStreamDeactivate(stid);
    fn_80162F68(streamInfo[i].hwStreamHandle);
    if (streamInfo[i].nextStreamHandle != (u32)-1) {
      sndStreamFree(streamInfo[i].nextStreamHandle);
    }

    streamInfo[i].state = 0;
  }

  hwEnableIrq();
}
#pragma pop

/* PARKED at 99.2% (2026-07-02): same story as sndStreamFree above --
 * logic/branch shape is exact, remaining diff is a handful of
 * register-allocation swaps (r30 vs r31 for the streamInfo base pointer,
 * chosen at the very first inlined GetPrivateIndex use and cascading
 * through the rest of the function). Tried: declaration-order swap of
 * i/ret, #pragma inline_depth(1..5) (no change vs default -inline auto).
 * No behavioral difference from target. */
u32 sndStreamActivate(u32 stid) {
  u32 i;
  u32 ret = 0;

  hwDisableIrq();
  i = GetPrivateIndex(stid);
  if (i != (u32)-1) {
    if (streamInfo[i].state == 3) {
      if ((streamInfo[i].voice = fn_80158328(streamInfo[i].prio)) == (u32)-1) {
        hwEnableIrq();
        return 0;
      }

      streamInfo[i].last = 0;
      streamInfo[i].state = 1;
    }

    if (streamInfo[i].nextStreamHandle != (u32)-1) {
      ret = sndStreamActivate(streamInfo[i].nextStreamHandle);
    } else {
      ret = 1;
    }
  }
  hwEnableIrq();
  return ret;
}

void sndStreamDeactivate(u32 stid) {
  u32 i;
  hwDisableIrq();
  i = GetPrivateIndex(stid);
  if (i != (u32)-1) {
    if (streamInfo[i].state == 1 || streamInfo[i].state == 2) {
      voiceUnblock(streamInfo[i].voice);
      streamInfo[i].state = 3;
    }

    if (streamInfo[i].nextStreamHandle != (u32)-1) {
      sndStreamDeactivate(streamInfo[i].nextStreamHandle);
    }
  }

  hwEnableIrq();
}
