#include "dolphin/types.h"

extern f32 lbl_8047CFF0;
extern f32 lbl_8047CFF4;

extern s32 pokemonGetStatus(
    u8* object, u32 data_id, u32 status, u32 index);

f32 pokemonGetDp(u8* ptr)
{
    s32 value;

    if (ptr == NULL) {
        return lbl_8047CFF0;
    }
    value = pokemonGetStatus(ptr, 0, 0xc5, 0);
    return (f32)value / lbl_8047CFF4;
}
