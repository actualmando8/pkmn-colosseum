#include "dolphin/types.h"

extern f32 lbl_8047CFF0;
extern f32 lbl_8047CFF4;
extern f32 lbl_8047D018;

extern s32 pokemonGetStatus(
    u8* object, u32 data_id, u32 status, u32 index);
extern void pokemonSetStatus(
    u8* object, u32 data_id, u32 status, u32 index, u32 value);
extern u8* pokemonSeikakuDataBiosGetPtr(u8 index);
extern u8* pokemonDpFilterDataBiosGetPtr(u16 index);
extern s32 pokemonDpFilterDataBiosGetValue(u8* data);
extern u8* itemDataBiosGetPtr(u16 item_data_id);
extern u8 itemDataBiosGetKind(u8* data);
extern u32 itemDataBiosGetBuff(u8* data);
extern s32 pokemonSeikakuDataBiosGetReliveFightout(u8* data);
extern s32 pokemonSeikakuDataBiosGetReliveWalk(u8* data);
extern s32 pokemonSeikakuDataBiosGetReliveCall(u8* data);
extern s32 pokemonSeikakuDataBiosGetReliveSodateya(u8* data);
extern s32 pokemonSeikakuDataBiosGetReliveNadenade(u8* data);
extern u8* pokemonSeikakuRateDataBiosGetPtr(u8 index);
extern u8 pokemonSeikakuRateDataBiosGetKake(u8* data);
extern u8 pokemonSeikakuRateDataBiosGetWaru(u8* data);

void pokemonAddDpFormPokemonDpFilterId(
    u8* ptr, u16 item_data_id, u16 filter_id)
{
    u8* seikaku_data;
    u8* rate_data;
    u8* item_data;
    s32 rate_data_id;
    u8 kake;
    u8 waru;
    f32 dp;
    f32 current_dp;
    f32 new_dp;
    s32 raw_dp;

    if ((u8)pokemonGetStatus(ptr, 0, 0xc2, 0) == 0) {
        return;
    }

    seikaku_data = pokemonSeikakuDataBiosGetPtr(
        (u8)pokemonGetStatus(ptr, 0, 0xbf, 0));
    if (seikaku_data == NULL) {
        return;
    }

    dp = (f32)(s8)pokemonDpFilterDataBiosGetValue(
        pokemonDpFilterDataBiosGetPtr(filter_id));

    if (filter_id == 4) {
        item_data = itemDataBiosGetPtr(item_data_id);
        if (item_data == NULL) {
            return;
        }
        if (itemDataBiosGetKind(item_data) != 6) {
            return;
        }
        dp *= (f32)itemDataBiosGetBuff(item_data);
    }

    if (filter_id == 5) {
        if (ptr == NULL) {
            current_dp = lbl_8047CFF0;
        } else {
            raw_dp = pokemonGetStatus(ptr, 0, 0xc5, 0);
            current_dp = (f32)raw_dp / lbl_8047CFF4;
        }
        dp = lbl_8047D018 * current_dp;
    }

    if (filter_id == 0) {
        rate_data_id =
            pokemonSeikakuDataBiosGetReliveFightout(seikaku_data);
    } else if (filter_id == 1) {
        rate_data_id = pokemonSeikakuDataBiosGetReliveWalk(seikaku_data);
    } else if (filter_id == 2) {
        rate_data_id = pokemonSeikakuDataBiosGetReliveCall(seikaku_data);
    } else if (filter_id == 3) {
        rate_data_id = pokemonSeikakuDataBiosGetReliveSodateya(seikaku_data);
    } else if (filter_id == 4) {
        rate_data_id = pokemonSeikakuDataBiosGetReliveNadenade(seikaku_data);
    }

    rate_data = pokemonSeikakuRateDataBiosGetPtr((u8)rate_data_id);
    if (rate_data == NULL) {
        return;
    }

    kake = pokemonSeikakuRateDataBiosGetKake(rate_data);
    waru = pokemonSeikakuRateDataBiosGetWaru(rate_data);
    if (waru != 0) {
        dp *= (f32)kake;
        dp /= (f32)waru;
    } else {
        return;
    }

    if (ptr != NULL) {
        if (ptr == NULL) {
            current_dp = lbl_8047CFF0;
        } else {
            raw_dp = pokemonGetStatus(ptr, 0, 0xc5, 0);
            current_dp = (f32)raw_dp / lbl_8047CFF4;
        }
        new_dp = current_dp + dp;
        if (new_dp < lbl_8047CFF0) {
            new_dp = lbl_8047CFF0;
        }
        if (ptr != NULL) {
            pokemonSetStatus(
                ptr,
                0,
                0xc5,
                0,
                (u32)(s32)(lbl_8047CFF4 * new_dp));
        }
    }
}
