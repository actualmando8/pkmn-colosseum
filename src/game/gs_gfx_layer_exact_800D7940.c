#include "dolphin/types.h"

extern void fn_800D67BC(u16 count);
extern void fn_800D6028(u16 vertex);
extern void fn_800D5FA4(u8 vertex);
extern void fn_800D5DD0(u16 vertex);
extern void fn_800D5D6C(u8 vertex);
extern void fn_800D5AB0(u32 layer, u16 vertex);
extern void fn_800D5A38(u32 layer, u8 vertex);
extern void fn_800D579C(u32 layer, u16 vertex);
extern void fn_800D5724(u32 layer, u8 vertex);
extern void fn_800D6728(void);

void fn_800D7940(u8* object, u16 vertexCount)
{
    u16 vertex;
    s32 i;
    u32 layer;
    u8* entry;

    fn_800D67BC(vertexCount);
    for (vertex = 0; vertex < vertexCount; vertex++) {
        if (*(s32*)(object + 0x28) == 2) {
            fn_800D6028(vertex);
        } else {
            fn_800D5FA4(vertex);
        }

        if (*(u8*)(object + 0x40) == 1) {
            if (*(s32*)(object + 0x44) == 2) {
                fn_800D5DD0(vertex);
            } else {
                fn_800D5D6C(vertex);
            }
        }

        for (i = 4; i <= 5; i++) {
            entry = object + i * 0x1c;
            if (entry[0x8] == 1) {
                layer = i - 4;
                if (*(s32*)(entry + 0xc) == 2) {
                    fn_800D5AB0(layer, vertex);
                } else {
                    fn_800D5A38(layer, vertex);
                }
            }
        }

        for (i = 6; i <= 13; i++) {
            entry = object + i * 0x1c;
            if (entry[0x8] == 1) {
                layer = i - 6;
                if (*(s32*)(entry + 0xc) == 2) {
                    fn_800D579C(layer, vertex);
                } else {
                    fn_800D5724(layer, vertex);
                }
            }
        }
    }
    fn_800D6728();
}
