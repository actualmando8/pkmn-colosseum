#ifndef GAME_PS_H
#define GAME_PS_H

#include "dolphin/types.h"

typedef struct PSGeneratorState PSGeneratorState;
struct PSParticle;

void psSetRandomVelocityScaling(PSGeneratorState* generator, u8 enabled);
void psSetNodeScaling(PSGeneratorState* generator, u8 enabled);
void psSetTornadoScaling(PSGeneratorState* generator, u8 enabledA, u8 enabledB);
void psSetParticleTexScaling(PSGeneratorState* generator, u8 enabled);
void psSetOffsetRotationInLocal(PSGeneratorState* generator, u8 enabledA,
                                u8 enabledB);
void psSetVelocityRotationInLocal(PSGeneratorState* generator, u8 enabled);
void psUnlinkChildGensFromJObj(PSGeneratorState* generator);
void psLinkChildGensToJObj(PSGeneratorState* generator, void* jobj);
u32 psGetParticleChildCount(PSGeneratorState* generator);
u32 psGetGeneratorChildMaxLife(PSGeneratorState* generator);
void psKillGeneratorChild(PSGeneratorState* generator);
void psKillFamily(s32 familyId, s32 linkNo);
void psDeletePntJObjwithParticle(struct PSParticle* particle);
void psSetPointJObjNodup(void* jobj, s32 index);
void psSetPointJObj(s32 index, void* jobj);

#endif
