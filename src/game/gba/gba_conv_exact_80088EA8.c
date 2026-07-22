#include "dolphin/types.h"
#include "game/gba/gba_conv.h"

extern void heroMoveGetHeroRot(f32* rotation);
extern void heroMoveGetHeroPos(f32* position);
extern u32 fn_800FF56C(void);
extern u32 floorGetPrevFloorID(void);
extern u32 fn_801906A0(s32 flag);

void fn_80088EA8(u8* output)
{
    GbaConvPlayerMemo* memo = (GbaConvPlayerMemo*)output;
    f32 position[3];
    f32 rotation[3];

    heroMoveGetHeroPos(position);
    heroMoveGetHeroRot(rotation);
    memo->posX = position[0];
    memo->posY = position[1];
    memo->posZ = position[2];
    memo->rotX = rotation[0];
    memo->rotY = rotation[1];
    memo->rotZ = rotation[2];
    memo->unk04 = fn_800FF56C();
    memo->prevFloorId = floorGetPrevFloorID();
    memo->valid = 1;
    memo->flagAfc = fn_801906A0(0xafc);
    memo->flagAfd = fn_801906A0(0xafd);
    memo->flagB11 = fn_801906A0(0xb11);
    memo->flagDe1 = fn_801906A0(0xde1);
}
