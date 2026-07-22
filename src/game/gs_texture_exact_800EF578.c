#include "game/gs_texture.h"

void GStextureSetFilter(GStextureHandle* texture, u32 minFilter,
                        u32 magFilter, u32 lodClamp)
{
    texture->minFilter = minFilter;
    texture->magFilter = magFilter;
    texture->lodClamp = lodClamp;
    texture->dirty = 1;
}

void GStextureSetWrap(GStextureHandle* texture, u32 wrapS, u32 wrapT)
{
    texture->wrapS = wrapS;
    texture->wrapT = wrapT;
    texture->dirty = 1;
}
