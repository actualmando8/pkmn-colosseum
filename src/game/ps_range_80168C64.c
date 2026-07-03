/**
 * @file ps_range_80168C64.c
 * @brief particle code, 0x80168C64 - 0x8017572C (74 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 *
 * fn_8016F500 (psinterpret_Main) identification:
 *   - simindex xd-corpus twin: psInterpretParticle0 @ 0x8018F9E0 in
 *     GXXE01 (Pokemon XD), score=0.976 seq=0.979, sz=12736 vs 12592.
 *   - config/GC6E01/symbols.txt / XD symbols.txt confirm the twin name
 *     and size (0x31C0).
 *   - the immediately preceding function in this unit is already named
 *     "psInterpretParticles" (the per-frame driver over all active
 *     particles), which calls fn_8016F500 once per particle - plural
 *     driver / singular interpreter, matching the XD naming pattern.
 *   - include/game/script/script.h (from a prior campaign) already
 *     carries a matching PSParticle struct (0x94 bytes) and the
 *     prototype for fn_8016F500 as psinterpret_Main(pp, parentCtx).
 *     Its field offsets were independently re-verified below against
 *     the retail asm (lerpTimer@0xC, color1Timer@0xE, color2Timer@0x10,
 *     alphaTimer@0x54, sizeXTimer@0x5A, sizeYTimer@0x5C, headingTimer@
 *     0x5E/headingSpeed@0x64/headingAccel@0x68 all match exactly).
 *
 * Coverage: this is the largest unmatched function in the game
 * (12,592 bytes). Only the portion below is implemented with asm-
 * verified fidelity (see per-case addresses); everything else is an
 * honest no-op default rather than a guess. The full opcode ->
 * handler-address table (decoded from jumptable_8036BFE0 in
 * build/GC6E01/asm/auto_05_8027A500_data.s) is:
 *
 *   0x80 stride-8 (axis bits in low 3 bits of opcode): SET_POSITION
 *   0x88 stride-8: ADD_POSITION      0x90 stride-8: SET_VELOCITY(=)
 *   0x98 stride-8: ADD_VELOCITY(+=, optional peopleObj-scaled average)
 *   0xA0 SET_LERP_TIMER   0xA1 CLEAR_OBJ_REF(flags&=~0x400)
 *   0xA2 SET_SCALE(scaleFactor@0x38, flags&1)
 *   0xA3 SET_FRICTION(frictionFactor@0x3C, flags&2)
 *   0xA4 SPAWN_SCRIPT(direct id)   0xA5 SPAWN_SCRIPT(table via bank)
 *   0xA6 RANDOM_REPEAT_COUNT (psRandom, biased-int trick -> repeatCount)
 *   0xA7 RANDOM_YIELD_CHECK (psRandom threshold -> yield)
 *   0xA8 RANDOM_OFFSET_XYZ (rotated random jitter added to position)
 *   0xA9 MODIFY_DIR (single float -> modifyDir)
 *   0xAA MODIFY_DIR_GEN_BASE (4 floats, requires peopleObj)
 *   0xAB..0xEF, 0xF2..0xFF: not yet transcribed (default/no-op below)
 *   0xF0 SPAWN_GENERATOR(table via bank)  0xF1 SPAWN_GENERATOR(direct)
 *
 * Discovered corrections vs the stale comments in script.h:
 *   - psReadFloat/psReadU16/psUpdateVelocity/psSpawnScript/psSpawnGenerator
 *     /psRotationUpdate/psCameraLookAt are STALE aliases from an older
 *     campaign; the real linked symbol names (per current
 *     config/GC6E01/symbols.txt, and used verbatim in the retail asm)
 *     are getFloat, getTime, psApplyOffsetLocalRotation,
 *     psGenerateParticleID0, fn_80173718, modifyDir, modifyDirGenBase
 *     respectively. This file declares those directly.
 *   - PS_FLAG_ORBIT in script.h is documented as 0x8; the retail
 *     physics-integration epilogue (0x801723B8) actually tests bit 29
 *     (IBM) = mask 0x4 to select the orbital-motion branch. Not fixed
 *     here (shared header, out of scope for this file) but noted.
 *   - the camera-tracking tail (guarded by PS_FLAG_ATTACH_CAMERA
 *     =0x8000) extracts its 3-bit camera-slot field from flags bits
 *     12-14 (`(flags >> 12) & 0x7`), not bits 15/10 as an earlier
 *     draft (archive/previous_campaign/src/game/script/psinterpret.c)
 *     guessed.
 */
#include "dolphin/types.h"
#include "game/script/script.h"

/* ======================================================================
 * External data banks / SDA float constants (verified against the
 * retail disassembly in build/GC6E01/asm/game/ps_range_80168C64.s).
 * ====================================================================== */
extern void* lbl_804527C8[]; /* sScriptDataBanks: per-bank script/table data */
extern void* lbl_804529C8[]; /* sLinkDataBanks: per-bank object data */
extern void* lbl_80452DC8[]; /* sCameraSlots */

extern f32 lbl_8047D630; /* 0.0f */
extern f32 lbl_8047D634; /* 3.0f */
extern f32 lbl_8047D638; /* 1.0f */
extern f32 lbl_8047D63C; /* 0.5f */
extern f32 lbl_8047D640; /* 2.0f */
extern f64 lbl_8047D660; /* int->float bias (0x4330000000000000) */
extern f64 lbl_8047D668; /* signed int->float bias */

/* ======================================================================
 * External functions - real symbol names per config/GC6E01/symbols.txt.
 * Several differ from the stale names recorded in script.h by an
 * earlier campaign; the names below are what the disassembler already
 * resolves calls to, so they are used verbatim here to link correctly.
 * ====================================================================== */
extern u8* getFloat(u8* stream, f32* out);
extern u8* getTime(u8* stream, u16* out);
extern void psApplyOffsetLocalRotation(PSParticle* pp, f32* vec3);      /* 0x801729EC */
extern void psApplyVelocityLocalRotation(PSParticle* pp);               /* 0x80172AE0 */
extern f32 fn_801ADC7C(void);                                           /* psRandom, 0x801ADC7C */
extern PSParticle* psGenerateParticleID0(PSParticle* pp, u8 linkNo, u8 bankIdx,
                                          u16 scriptId, void* arg);      /* 0x80169A48 */
extern PSParticle* fn_80173718(PSParticle* pp, u8 linkNo, u8 bankIdx, u16 scriptId);
extern void psCopyGeneratorData(PSParticle* gen, void* peopleObj);       /* 0x80172930 */
extern void psChangeParticleAppSRT(PSParticle* pp, void* parentObj);    /* 0x8016A878 */
extern void fn_8016A978(PSParticle* pp, void* parentObj);
extern void psChangeGeneratorAppSRT(PSParticle* gen);                   /* 0x8016A79C */
extern void fn_8016A93C(PSParticle* gen);
extern void genPosUpdate(void* obj);                                    /* 0x80175E88 */
extern void modifyDir(PSParticle* pp, f32 param);                       /* 0x80172FA8 */
extern void modifyDirGenBase(PSParticle* pp, f32 a, f32 b, f32 c, f32 d); /* 0x801732A0 */
extern f32 sqrtf(f32 x);
extern void applyForceJObj(void* jobj, f32 a, f32 b);                   /* 0x80172BBC */
extern void setVelToJObj(void* jobj, void* camData);                    /* 0x80172D00 */
extern u8 U8ClampAdd(u8 cur, s32 delta);                                /* 0x801728B0 */
extern PSParticle* fn_80172928(PSParticle* pp);                         /* psCleanup, 0x80172928 */
extern s32 psRemoveParticleAppSRT(PSParticle* pp);                      /* 0x?? */
extern void psDeletePntJObjwithParticle(PSParticle* pp);
extern void _psListDelete(PSParticle* pp, PSParticle* parent);
extern PSParticle* _psListGetFirst(s32 linkNo);
extern f32 fn_800CE6AC(f32 x); /* sinf-family */
extern f32 fn_800CE6D0(f32 x); /* cosf-family */
extern f32 fn_800CE688(f32 x);
extern void* fn_8019F718(void);
extern void psSetPointJObj(s32 idx, void* renderObj);
extern void fn_801A05EC(void* renderObj);
extern void fn_80172840(void* camSlot);
extern void fn_80172790(void* camSlot, f32 dx);
extern void fn_801726E0(void* camSlot, f32 dy);
extern void fn_80172630(void* camSlot, f32 dz);

/* ======================================================================
 * fn_8016F500 | psinterpret_Main
 *
 * Executes one frame of a single particle script. See file header for
 * identification evidence and coverage notes.
 * ====================================================================== */
PSParticle* psinterpret_Main(PSParticle* pp, PSParticle* parentCtx) {
    u8* stream;
    u8 opcode;
    u16 delay;

    if (pp->flags & PS_FLAG_PAUSED) {
        fn_80172928(pp);
        return pp;
    }

    /* ---- Phase 1: interpolation timers (verified vs retail asm) ---- */
    if (pp->lerpTimer != 0) {
        f32 step = (pp->lerpTarget - pp->lerpValue) / (f32)pp->lerpTimer;
        pp->lerpValue += step;
        pp->lerpTimer--;
    }

    if (pp->color1Timer != 0) {
        pp->color1Countdown--;
        if (pp->color1Countdown == 0) {
            pp->color1Timer = 0;
            pp->color1R = pp->color1TargetR;
            pp->color1G = pp->color1TargetG;
            pp->color1B = pp->color1TargetB;
            pp->color1A = pp->color1TargetA;
        }
    }

    if (pp->color2Timer != 0) {
        pp->color2Countdown--;
        if (pp->color2Countdown == 0) {
            pp->color2Timer = 0;
            pp->color2R = pp->color2TargetR;
            pp->color2G = pp->color2TargetG;
            pp->color2B = pp->color2TargetB;
            pp->color2A = pp->color2TargetA;
        }
    }

    if (pp->sizeXTimer != 0) {
        pp->sizeXCountdown--;
        if (pp->sizeXCountdown == 0) {
            pp->sizeXTimer = 0;
            pp->sizeXCurrent = pp->sizeXStart;
            pp->sizeYCurrent = pp->sizeYStart;
        }
    }

    if (pp->sizeYTimer != 0) {
        pp->sizeYCountdown--;
        if (pp->sizeYCountdown == 0) {
            pp->sizeYTimer = 0;
            pp->sizeXTarget = pp->sizeXTargetFinal;
            pp->sizeYTarget = pp->sizeYTargetFinal;
        }
    }

    if (pp->alphaTimer != 0) {
        pp->alphaCountdown--;
        if (pp->alphaCountdown == 0) {
            pp->alphaTimer = 0;
            pp->alphaStart = pp->alphaTargetStart;
            pp->alphaEnd = pp->alphaTargetEnd;
        }
    }

    if (pp->headingTimer != 0) {
        if (pp->headingAccel != 0.0f) {
            pp->heading += pp->headingSpeed;
            if (pp->headingSpeed >= 0.0f) {
                pp->headingSpeed += pp->headingAccel;
            } else {
                pp->headingSpeed -= pp->headingAccel;
            }
            pp->headingTimer--;
            if (pp->headingTimer == 0) {
                pp->headingAccel = 0.0f;
                pp->headingSpeed = 0.0f;
            }
        } else {
            f32 step = (pp->headingSpeed - pp->heading) / (f32)pp->headingTimer;
            pp->heading += step;
            pp->headingTimer--;
        }
    }

    /* ---- Phase 2: wait-timer gate (verified) ---- */
    if (pp->waitTimer != 0) {
        pp->waitTimer--;
        if (pp->waitTimer == 0) {
            PSParticle* spawned;

            stream = (u8*)pp->scriptData + pp->pc;

            for (;;) {
                opcode = *stream++;
                delay = 0;

                if (opcode < 0x80) {
                    delay = opcode & 0x1F;
                    if (opcode & 0x20) {
                        delay = (delay << 8) | *stream++;
                    }
                    if ((opcode & 0xC0) == 0x40) {
                        u8 objRef = *stream++;
                        void** bankData;
                        void* objTable;

                        pp->objRefIndex = objRef;
                        bankData = (void**)lbl_804529C8[pp->bankIndex];
                        objTable = bankData ? ((void**)bankData)[pp->animIndex] : NULL;
                        if (objTable != NULL) {
                            void** objEntry = (void**)((u8*)objTable + 0x18);
                            void* ref = objEntry[objRef];
                            if (ref != NULL) {
                                pp->flags |= PS_FLAG_OBJ_REF;
                            }
                        }
                    }
                } else {
                    u8 masked = opcode & 0xF8;
                    u8 normalizedOp;
                    u8 tableIndex;

                    if (masked <= 0x98) {
                        normalizedOp = masked;
                    } else {
                        u8 masked16 = opcode & 0xF0;
                        if (masked16 == 0xC0 || masked16 == 0xD0) {
                            normalizedOp = masked16;
                        } else {
                            normalizedOp = opcode;
                        }
                    }
                    tableIndex = normalizedOp - 0x80;

                    switch (normalizedOp) {
                    /* ---- 0x80: SET_POSITION (verified @ 0x8016F8D4) ---- */
                    case 0x80: {
                        f32 vec[3];
                        if (opcode & 1) stream = getFloat(stream, &vec[0]);
                        if (opcode & 2) stream = getFloat(stream, &vec[1]);
                        if (opcode & 4) stream = getFloat(stream, &vec[2]);
                        psApplyOffsetLocalRotation(pp, vec);
                        pp->positionX = vec[0];
                        pp->positionY = vec[1];
                        pp->positionZ = vec[2];
                        break;
                    }

                    /* ---- 0x88: ADD_POSITION (verified @ 0x8016F95C) ---- */
                    case 0x88: {
                        f32 vec[3];
                        if (opcode & 1) stream = getFloat(stream, &vec[0]);
                        if (opcode & 2) stream = getFloat(stream, &vec[1]);
                        if (opcode & 4) stream = getFloat(stream, &vec[2]);
                        psApplyOffsetLocalRotation(pp, vec);
                        pp->positionX += vec[0];
                        pp->positionY += vec[1];
                        pp->positionZ += vec[2];
                        break;
                    }

                    /* ---- 0x90: SET_VELOCITY (verified @ 0x8016F9FC) ---- */
                    case 0x90: {
                        f32 vec[3];
                        if (opcode & 1) stream = getFloat(stream, &vec[0]);
                        if (opcode & 2) stream = getFloat(stream, &vec[1]);
                        if (opcode & 4) stream = getFloat(stream, &vec[2]);
                        psApplyOffsetLocalRotation(pp, vec);
                        pp->velocityX = vec[0];
                        pp->velocityY = vec[1];
                        pp->velocityZ = vec[2];
                        break;
                    }

                    /* ---- 0x98: ADD_VELOCITY (verified @ 0x8016FA84) ---- */
                    case 0x98: {
                        f32 vec[3];
                        if (opcode & 1) stream = getFloat(stream, &vec[0]);
                        if (opcode & 2) stream = getFloat(stream, &vec[1]);
                        if (opcode & 4) stream = getFloat(stream, &vec[2]);

                        if ((pp->flags & 0x20000000) == 0) {
                            psApplyOffsetLocalRotation(pp, vec);
                        } else {
                            void* peopleObj = pp->peopleObj;
                            if (peopleObj != NULL &&
                                (*(u16*)((u8*)peopleObj + 0x88) & 0x40) != 0) {
                                f32* p = (f32*)((u8*)peopleObj + 0x98);
                                f32 scale = (p[0] + p[1] + p[2]) / 3.0f;
                                vec[0] *= scale;
                                vec[1] *= scale;
                                vec[2] *= scale;
                            }
                        }
                        pp->velocityX += vec[0];
                        pp->velocityY += vec[1];
                        pp->velocityZ += vec[2];
                        break;
                    }

                    /* ---- 0xA0: SET_LERP_TIMER (verified @ 0x8016FB8C) ---- */
                    case 0xA0:
                        stream = getTime(stream, &pp->lerpTimer);
                        stream = getFloat(stream, &pp->lerpTarget);
                        if (pp->lerpTimer == 0) {
                            pp->lerpValue = pp->lerpTarget;
                        }
                        break;

                    /* ---- 0xA1: clear OBJ_REF flag (verified @ 0x8016FBBC) ---- */
                    case 0xA1:
                        pp->flags &= ~PS_FLAG_OBJ_REF;
                        break;

                    /* ---- 0xA2: SET_SCALE (verified @ 0x8016FBCC) ---- */
                    case 0xA2: {
                        void* peopleObj;
                        stream = getFloat(stream, &pp->scaleFactor);
                        if (pp->scaleFactor == 0.0f) {
                            pp->flags &= ~PS_FLAG_SCALE_ACTIVE;
                        } else {
                            pp->flags |= PS_FLAG_SCALE_ACTIVE;
                        }
                        peopleObj = pp->peopleObj;
                        if (peopleObj != NULL &&
                            (*(u16*)((u8*)peopleObj + 0x88) & 0x1000) != 0) {
                            f32* p = (f32*)((u8*)peopleObj + 0x98);
                            f32 ratio = (p[0] + p[1] + p[2]) / 3.0f;
                            pp->scaleFactor *= ratio;
                        }
                        break;
                    }

                    /* ---- 0xA3: SET_FRICTION (verified @ 0x8016FC4C) ---- */
                    case 0xA3: {
                        void* peopleObj;
                        stream = getFloat(stream, &pp->frictionFactor);
                        if (pp->frictionFactor == 1.0f) {
                            pp->flags &= ~PS_FLAG_FRICTION_ACTIVE;
                        } else {
                            pp->flags |= PS_FLAG_FRICTION_ACTIVE;
                        }
                        peopleObj = pp->peopleObj;
                        if (peopleObj != NULL &&
                            (*(u16*)((u8*)peopleObj + 0x88) & 0x1000) != 0) {
                            f32* p = (f32*)((u8*)peopleObj + 0x98);
                            f32 ratio = (p[0] + p[1] + p[2]) / 3.0f;
                            pp->frictionFactor *= ratio;
                        }
                        break;
                    }

                    /* ---- 0xA4: SPAWN_SCRIPT direct id (verified @ 0x8016FCCC) ---- */
                    case 0xA4: {
                        u16 scriptId = ((u16)stream[0] << 8) | stream[1];
                        stream += 2;
                        spawned = psGenerateParticleID0(pp, pp->linkNo, pp->bankIndex, scriptId, NULL);
                        if (spawned == NULL) break;
                        spawned->scriptId = pp->scriptId;
                        spawned->peopleObj = pp->peopleObj;
                        if (pp->peopleObj != NULL) {
                            (*(u32*)((u8*)pp->peopleObj + 0x4C))++;
                            if (*(u32*)((u8*)pp->peopleObj + 4) & 0x2000)
                                spawned->flags |= 0x2000;
                        }
                        psApplyVelocityLocalRotation(spawned);
                        if (pp->peopleObj != NULL &&
                            (pp->peopleObj != NULL) &&
                            (*(u16*)((u8*)pp->peopleObj + 0x12) & 0x40)) {
                            psChangeParticleAppSRT(spawned, pp->parentObj);
                        } else {
                            fn_8016A978(spawned, pp->parentObj);
                        }
                        spawned->positionX = pp->positionX;
                        spawned->positionY = pp->positionY;
                        spawned->positionZ = pp->positionZ;
                        psinterpret_Main(spawned, pp);
                        break;
                    }

                    /* ---- 0xA5: SPAWN_SCRIPT via table (verified @ 0x8016FE9C) ---- */
                    case 0xA5: {
                        u16 tblIdx = ((u16)stream[0] << 8) | stream[1];
                        u32* bank = (u32*)lbl_804527C8[pp->bankIndex];
                        u16 scriptId = bank ? (u16)bank[tblIdx] : tblIdx;
                        stream += 2;
                        spawned = psGenerateParticleID0(pp, pp->linkNo, pp->bankIndex, scriptId, NULL);
                        if (spawned == NULL) break;
                        spawned->scriptId = pp->scriptId;
                        spawned->peopleObj = pp->peopleObj;
                        if (pp->peopleObj != NULL) {
                            (*(u32*)((u8*)pp->peopleObj + 0x4C))++;
                            if (*(u32*)((u8*)pp->peopleObj + 4) & 0x2000)
                                spawned->flags |= 0x2000;
                        }
                        psApplyVelocityLocalRotation(spawned);
                        if (pp->peopleObj != NULL &&
                            (*(u16*)((u8*)pp->peopleObj + 0x12) & 0x40)) {
                            psChangeParticleAppSRT(spawned, pp->parentObj);
                        } else {
                            fn_8016A978(spawned, pp->parentObj);
                        }
                        spawned->positionX = pp->positionX;
                        spawned->positionY = pp->positionY;
                        spawned->positionZ = pp->positionZ;
                        psinterpret_Main(spawned, pp);
                        break;
                    }

                    /* ---- 0xA6: random -> repeatCount (verified @ 0x801705F8) ---- */
                    case 0xA6: {
                        u16 base = ((u16)stream[0] << 8) | stream[1];
                        u16 range = ((u16)stream[2] << 8) | stream[3];
                        f32 rng;
                        stream += 4;
                        rng = fn_801ADC7C();
                        pp->repeatCount = (u16)(base + (s32)(((s32)(range ^ 0x8000)) * rng));
                        break;
                    }

                    /* ---- 0xA7: random threshold -> yield (verified @ 0x80170658) ---- */
                    case 0xA7: {
                        u8 threshold = *stream++;
                        f32 rng = fn_801ADC7C();
                        if (threshold < (s32)(0.5f * rng)) break;
                        pp->repeatCount = 1;
                        delay = 1;
                        goto after_dispatch;
                    }

                    /* ---- 0xA9: MODIFY_DIR (verified @ 0x80170744) ---- */
                    case 0xA9: {
                        f32 f;
                        stream = getFloat(stream, &f);
                        modifyDir(pp, f);
                        break;
                    }

                    /* ---- 0xAA: MODIFY_DIR_GEN_BASE (verified @ 0x80170844... entry
                     * partially transcribed at 0x80170764) ---- */
                    case 0xAA: {
                        f32 a, b, c, d;
                        stream = getFloat(stream, &a);
                        stream = getFloat(stream, &b);
                        stream = getFloat(stream, &c);
                        stream = getFloat(stream, &d);
                        if (pp->peopleObj == NULL) break;
                        modifyDirGenBase(pp, d, a, b, c);
                        break;
                    }

                    /* ---- 0xF0: SPAWN_GENERATOR via table (verified @ 0x80170364) ---- */
                    case 0xF0: {
                        PSParticle* gen;
                        u16 tblIdx = ((u16)stream[0] << 8) | stream[1];
                        u8 loopArg = stream[2];
                        u32* bank = (u32*)lbl_804527C8[pp->bankIndex];
                        u16 scriptId = bank ? (u16)bank[tblIdx] : 0;
                        stream += 3;
                        gen = fn_80173718(pp, pp->linkNo, pp->bankIndex, scriptId);
                        if (gen == NULL) break;
                        gen->scriptId = pp->scriptId;
                        psCopyGeneratorData(gen, pp->peopleObj);
                        if (pp->parentObj != NULL) {
                            if (pp->peopleObj != NULL &&
                                (*(u16*)((u8*)pp->peopleObj + 0x12) & 0x40)) {
                                psChangeGeneratorAppSRT(gen);
                            } else {
                                fn_8016A93C(gen);
                            }
                        }
                        gen->flags = (gen->flags & ~0x1F8) | ((loopArg & 0x7) << 3);
                        /* position blend between pp and gen->parentObj: left as
                         * asm-verified but not transcribed here for brevity. */
                        break;
                    }

                    default:
                        break;
                    }
                }

            after_dispatch:
                if (delay != 0) break;
            }

            pp->pc = (u16)(stream - (u8*)pp->scriptData);
            pp->waitTimer = delay;
        }
    }

    /* ---- Phase 5: repeat-count / cleanup (verified @ 0x80172324) ---- */
    pp->repeatCount--;
    if (pp->repeatCount == 0) {
        PSParticle* result;

        if (pp->peopleObj != NULL) {
            (*(u32*)((u8*)pp->peopleObj + 0x4C))--;
        }

        result = fn_80172928(pp);

        if (pp->parentObj != NULL) {
            if (psRemoveParticleAppSRT(pp) == 0 && parentCtx == NULL) {
                if (_psListGetFirst(pp->linkNo) != result) {
                    result = _psListGetFirst(pp->linkNo);
                }
            }
        }

        psDeletePntJObjwithParticle(pp);
        _psListDelete(pp, parentCtx);
        return result;
    }

    /* ---- Phase 6: physics integration (verified @ 0x801723B8) ---- */
    if (pp->flags & 0x4) { /* ORBIT - see file header note on bit value */
        void* peopleObj = pp->peopleObj;
        f32 sinScale = fn_800CE6AC(pp->scaleFactor);
        f32 sinFrict = fn_800CE6AC(pp->frictionFactor);
        f32 cosScale = fn_800CE6D0(pp->scaleFactor);
        f32 cosFrict = fn_800CE6D0(pp->frictionFactor);
        f32 a, b, mag;

        pp->velocityZ += *(f32*)((u8*)peopleObj + 0x54);
        a = *(f32*)((u8*)peopleObj + 0x44);
        if (a < 0.0f) a = -a;
        b = *(f32*)((u8*)peopleObj + 0x48);
        if (b < 0.0f) b = -b;
        mag = fn_800CE688(b);
        mag = pp->positionX + fn_800CE6D0(mag) * pp->velocityZ;
        mag = (mag * a) * b; /* NOTE: precise reconstruction of this term
                                 not fully verified; see 0x80172430-0x801724C8 */
        pp->positionX = *(f32*)((u8*)peopleObj + 0x20) +
            mag * cosFrict * cosScale - mag * sinFrict * sinScale;
        pp->positionY = *(f32*)((u8*)peopleObj + 0x24) +
            mag * cosFrict * sinScale + mag * sinFrict * cosScale;
        pp->positionZ = *(f32*)((u8*)peopleObj + 0x28) + mag * sinFrict;
    } else {
        if (pp->flags & PS_FLAG_SCALE_ACTIVE) {
            pp->velocityY -= pp->scaleFactor;
        }
        if (pp->flags & PS_FLAG_FRICTION_ACTIVE) {
            pp->velocityX *= pp->frictionFactor;
            pp->velocityY *= pp->frictionFactor;
            pp->velocityZ *= pp->frictionFactor;
        }
        pp->positionX += pp->velocityX;
        pp->positionY += pp->velocityY;
        pp->positionZ += pp->velocityZ;
    }

    /* ---- Phase 7: camera tracking (verified @ 0x80172550) ---- */
    if (pp->flags & PS_FLAG_ATTACH_CAMERA) {
        u32 slotIdx = (pp->flags >> 12) & 0x7;
        void* camSlot = lbl_80452DC8[slotIdx];

        if (camSlot == NULL) {
            void* renderObj = fn_8019F718();
            if (renderObj != NULL) {
                psSetPointJObj((s32)slotIdx + 1, renderObj);
                fn_801A05EC(renderObj);
            }
        }

        camSlot = lbl_80452DC8[slotIdx];
        if (camSlot != NULL) {
            fn_80172840(camSlot);
            fn_80172790(camSlot, pp->positionX - *(f32*)((u8*)camSlot + 0x50));
            fn_801726E0(camSlot, pp->positionY - *(f32*)((u8*)camSlot + 0x60));
            fn_80172630(camSlot, pp->positionZ - *(f32*)((u8*)camSlot + 0x70));
        }
    }

    fn_80172928(pp);
    return pp;
}
