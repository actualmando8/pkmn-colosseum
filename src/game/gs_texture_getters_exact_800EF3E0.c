/**
 * @file gs_texture_getters_exact_800EF3E0.c
 * @brief Exact GS texture format and dimension accessors.
 *
 * Address range: 0x800EF3E0 - 0x800EF504.
 */

#include "dolphin/types.h"
#include "game/gs_texture.h"

s32 GStextureGetGXformat(GStextureHandle* tex, u8 alpha)
{
    u32 format = tex->format;

    switch (format) {
    case 0x00:
        return 0x08;
    case 0x01:
        return 0x09;
    case 0x30:
        return 0x0A;
    case 0x40:
        return 0x00;
    case 0x41:
        return 0x02;
    case 0x42:
        return 0x01;
    case 0x43:
        return 0x03;
    case 0x44:
        return 0x04;
    case 0x45:
        return 0x06;
    case 0x90:
        return 0x05;
    case 0xB0:
        return 0x0E;
    case 0xA0:
        if (alpha != 0) {
            return 0x01;
        }
        return 0x27;
    default:
        return -1;
    }
}

u32 GStextureGetTlutFormat(GStextureHandle* tex)
{
    return tex->tlutFormat;
}

u32 GStextureGetFormat(GStextureHandle* tex)
{
    return tex->format;
}

u8 GStextureGetMiplevels(GStextureHandle* tex)
{
    return (u8)(tex->mipLevels - 1);
}

u16 GStextureGetYsize(GStextureHandle* tex)
{
    return tex->height;
}

u16 GStextureGetXsize(GStextureHandle* tex)
{
    return tex->width;
}
