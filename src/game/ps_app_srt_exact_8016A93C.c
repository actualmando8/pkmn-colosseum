#include "game/ps_types.h"
#include "game/script/script.h"

s32 psAttachGeneratorAppSRT(PSGeneratorState* generator, PSAppSRT* appSRT)
{
    u16 refCount;

    if (appSRT == NULL || generator == NULL || generator->appSRT != NULL) {
        return -1;
    }

    generator->appSRT = appSRT;
    refCount = appSRT->refCount + 1;
    appSRT->refCount = refCount;
    return refCount;
}

s32 psAttachParticleAppSRT(PSParticle* particle, PSAppSRT* appSRT)
{
    u16 refCount;

    if (appSRT == NULL || particle == NULL || particle->parentObj != NULL) {
        return -1;
    }

    particle->parentObj = appSRT;
    refCount = appSRT->refCount + 1;
    appSRT->refCount = refCount;
    return refCount;
}
