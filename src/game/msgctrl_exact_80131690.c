/**
 * @file msgctrl_exact_80131690.c
 * @brief Strict message-control helpers, 0x80131690 - 0x80131714.
 */
#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"

u32 msgctrlIndentOff(EffectUtilCommandObj* obj)
{
    obj->field_41 = 1;
    return 0;
}

u16 msgctrlNpc(void)
{
    return lbl_8047AEA2;
}

void msgctrlTribe(void)
{
    fightTrainerKindDataBiosGetPtr(lbl_8047AEA0);
    fightTrainerKindDataBiosGetPrefixName();
}

u32 msgctrlString2(void)
{
    return lbl_8047AE8C;
}

u32 msgctrlMenuMsgID2(void)
{
    return lbl_8047AE9C;
}

u32 msgctrlMenuMsgID(void)
{
    return lbl_8047AE98;
}

u32 msgctrlTalkSE(EffectUtilCommandObj* obj)
{
    u8* stream;

    if (obj->activeFlag == 0) {
        stream = obj->stream;
        obj->field_03 = *stream;
    }
    stream = obj->stream;
    obj->stream = stream + 1;
    return 0;
}
