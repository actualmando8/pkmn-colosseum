#include "dolphin/types.h"

typedef struct GStexture GStexture;

extern u8 lbl_8047B3A8;

void* myBackFB__FP9GStextureUlPv(GStexture* texture, u32 size, void* userData)
{
    lbl_8047B3A8 = 1;
    return NULL;
}
