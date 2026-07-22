#include "game/gs_texture.h"

void* GStextureLockImage(GStextureHandle* texture, u8 level)
{
    if (level >= 8) {
        return NULL;
    }
    texture->refCount++;
    return texture->mipData[level];
}
