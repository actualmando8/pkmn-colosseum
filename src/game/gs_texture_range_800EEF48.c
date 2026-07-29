/**
 * @file gs_texture_range_800EEF48.c
 * @brief GStexture (head function stranded by splitter; rest is existing unit game/gs_texture.c)
 *
 * Split from gs_range_800E202C.c (0x800EEF48-0x800EF098) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct GStextureRange {
    u16 width;
    u16 height;
    u8 pad_04[4];
    s32 format;
    u8 pad_0C[0x1C];
    u16* data;
    u8 pad_2C[0x20];
    u32 dataSize;
    u16 convertCount;
} GStextureRange;

extern u16 _toolentryAlloc__FUl(u32 size);
extern void* fn_800E27B0(u16 handle);
extern void fn_800E24B0(u16 handle);
extern void fn_800E209C(u16 handle);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void DCFlushRange(void* address, u32 size);
extern void GXInvalidateTexAll(void);

void GStextureConvertToHW(GStextureRange* texture)
{
    u16 handle;
    u16* source;
    u16* converted;
    u16 width;
    u16 pixelCount;
    u32 byteCount;
    u16 blocksPerRow;
    u16 i;

    if (texture->format != 0x44) {
        return;
    }
    texture->convertCount++;
    source = texture->data;
    if (source == NULL) {
        return;
    }

    width = texture->width;
    pixelCount = width * texture->height;
    byteCount = pixelCount * sizeof(u16);
    handle = _toolentryAlloc__FUl(byteCount);
    if (handle == 0) {
        return;
    }

    converted = fn_800E27B0(handle);
    blocksPerRow = width >> 2;
    for (i = 0; i < pixelCount; i++) {
        u16 tile = (i >> 4) / blocksPerRow;
        u16 tileColumn = (i >> 4) - tile * blocksPerRow;
        u16 row = (i & 0xF) >> 2;
        u16 column = i & 3;
        u16 sourceIndex =
            (tile * width + tileColumn) * 4 + row * width + column;
        converted[i] = source[sourceIndex];
    }

    memcpy(source, converted, byteCount);
    DCFlushRange(texture->data, texture->dataSize);
    GXInvalidateTexAll();
    texture->convertCount--;
    fn_800E24B0(handle);
    fn_800E209C(handle);
}
