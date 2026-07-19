#include "dolphin/types.h"

extern f32 lbl_8047CFF4;

extern void pokemonSetStatus(
    u8* object, u32 data_id, u32 status, u32 index, u32 value);

void pokemonSetDp(u8* ptr, f32 dp)
{
    if (ptr == NULL) {
        return;
    }
    pokemonSetStatus(
        ptr, 0, 0xc5, 0, (u32)(s32)(lbl_8047CFF4 * dp));
}
