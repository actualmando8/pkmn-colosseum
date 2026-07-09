#ifndef GAME_POKEMON_DATA_H
#define GAME_POKEMON_DATA_H

#include "dolphin/types.h"

typedef struct PokemonStatBlock {
    /* 0x00 */ u16 maxHp;
    /* 0x02 */ u16 phyAtk;
    /* 0x04 */ u16 phyDef;
    /* 0x06 */ u16 speAtk;
    /* 0x08 */ u16 speDef;
    /* 0x0A */ u16 nimbleness;
} PokemonStatBlock;

typedef struct PokemonDataEvolution {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 _pad01;
    /* 0x02 */ u16 buff;
    /* 0x04 */ u16 pokemonDataId;
} PokemonDataEvolution;

typedef struct PokemonDataWazaLearn {
    /* 0x00 */ u8 level;
    /* 0x01 */ u8 _pad01;
    /* 0x02 */ u16 dataId;
} PokemonDataWazaLearn;

typedef struct PokemonDataFace {
    /* 0x00 */ u8 color;
    /* 0x01 */ u8 _pad01;
    /* 0x02 */ u16 statusFaceMenuSpriteId;
    /* 0x04 */ u32 pokebodyId;
} PokemonDataFace;

typedef struct PokemonData {
    /* 0x000 */ u8 growDataId;
    /* 0x001 */ u8 get;
    /* 0x002 */ u8 sexRatio;
    /* 0x003 */ u8 unk_003;
    /* 0x004 */ u8 unk_004;
    /* 0x005 */ u8 _pad005;
    /* 0x006 */ u16 giveExp;
    /* 0x008 */ u16 initFriend;
    /* 0x00A */ u16 height;
    /* 0x00C */ u16 weight;
    /* 0x00E */ u16 voice;
    /* 0x010 */ u8 _pad010[2];
    /* 0x012 */ u16 numPokemon;
    /* 0x014 */ u16 numZukan;
    /* 0x016 */ u16 unk_016;
    /* 0x018 */ u32 name;
    /* 0x01C */ u32 typeName;
    /* 0x020 */ u32 doc;
    /* 0x024 */ u32 mitaFlag;
    /* 0x028 */ u32 tukamaetaFlag;
    /* 0x02C */ u32 pkxDataId;
    /* 0x030 */ u8 zokuseiDataId[2];
    /* 0x032 */ u8 tokuseiDataId[2];
    /* 0x034 */ u8 wazaMcn[0x3A];
    /* 0x06E */ u8 unk_06E[2];
    /* 0x070 */ u16 itemDataId[2];
    /* 0x074 */ u16 kowaza[8];
    /* 0x084 */ PokemonStatBlock basis;
    /* 0x090 */ PokemonStatBlock giveEffort;
    /* 0x09C */ PokemonDataEvolution sinka[5];
    /* 0x0BA */ PokemonDataWazaLearn getWaza[0x14];
    /* 0x10A */ u8 _pad10A[2];
    /* 0x10C */ PokemonDataFace face[2];
} PokemonData;

typedef struct PokemonDpFilterData {
    /* 0x00 */ u8 value;
} PokemonDpFilterData;

typedef struct PokemonFriendFilterData {
    /* 0x00 */ u8 value[3];
} PokemonFriendFilterData;

typedef struct PokemonWaza {
    /* 0x00 */ u16 dataId;
    /* 0x02 */ u8 pp;
    /* 0x03 */ u8 ppCount;
} PokemonWaza;

typedef struct PokemonBiosContestStats {
    /* 0x00 */ u8 style;
    /* 0x01 */ u8 beautiful;
    /* 0x02 */ u8 cute;
    /* 0x03 */ u8 clever;
    /* 0x04 */ u8 strong;
} PokemonBiosContestStats;

typedef struct PokemonBiosContestMedals {
    /* 0x00 */ u8 styleMedal;
    /* 0x01 */ u8 beautifulMedal;
    /* 0x02 */ u8 cuteMedal;
    /* 0x03 */ u8 cleverMedal;
    /* 0x04 */ u8 strongMedal;
} PokemonBiosContestMedals;

typedef struct PokemonBiosRibbons {
    /* 0x00 */ u8 champRibbon;
    /* 0x01 */ u8 winningRibbon;
    /* 0x02 */ u8 victoryRibbon;
    /* 0x03 */ u8 bromideRibbon;
    /* 0x04 */ u8 ganbaRibbon;
    /* 0x05 */ u8 marineRibbon;
    /* 0x06 */ u8 landRibbon;
    /* 0x07 */ u8 skyRibbon;
    /* 0x08 */ u8 countryRibbon;
    /* 0x09 */ u8 nationalRibbon;
    /* 0x0A */ u8 earthRibbon;
    /* 0x0B */ u8 worldRibbon;
    /* 0x0C */ u8 amariRibbon;
} PokemonBiosRibbons;

typedef struct PokemonBios {
    /* 0x000 */ u16 pokemonDataId;
    /* 0x002 */ u8 _pad002[2];
    /* 0x004 */ u32 rnd;
    /* 0x008 */ u8 attest[4];
    /* 0x00C */ u16 catchFloorId;
    /* 0x00E */ u8 catchLevel;
    /* 0x00F */ u8 catchBallId;
    /* 0x010 */ u8 catchTrainerSex;
    /* 0x011 */ u8 _pad011[3];
    /* 0x014 */ u32 catchTrainerRnd;
    /* 0x018 */ u8 catchTrainerName[0x16];
    /* 0x02E */ u8 nickname[0x16];
    /* 0x044 */ u8 nicknameOrg[0x18];
    /* 0x05C */ u32 exp;
    /* 0x060 */ u8 level;
    /* 0x061 */ u8 _pad061[3];
    /* 0x064 */ u8 condition[0x10];
    /* 0x074 */ u32 conditionAmari;
    /* 0x078 */ PokemonWaza waza[4];
    /* 0x088 */ u16 itemDataId;
    /* 0x08A */ u16 hp;
    /* 0x08C */ PokemonStatBlock status;
    /* 0x098 */ PokemonStatBlock effort;
    /* 0x0A4 */ PokemonStatBlock rndStat;
    /* 0x0B0 */ u16 friend;
    /* 0x0B2 */ u8 style;
    /* 0x0B3 */ u8 beautiful;
    /* 0x0B4 */ u8 cute;
    /* 0x0B5 */ u8 clever;
    /* 0x0B6 */ u8 strong;
    /* 0x0B7 */ u8 styleMedal;
    /* 0x0B8 */ u8 beautifulMedal;
    /* 0x0B9 */ u8 cuteMedal;
    /* 0x0BA */ u8 cleverMedal;
    /* 0x0BB */ u8 strongMedal;
    /* 0x0BC */ u8 fur;
    /* 0x0BD */ u8 champRibbon;
    /* 0x0BE */ u8 winningRibbon;
    /* 0x0BF */ u8 victoryRibbon;
    /* 0x0C0 */ u8 bromideRibbon;
    /* 0x0C1 */ u8 ganbaRibbon;
    /* 0x0C2 */ u8 marineRibbon;
    /* 0x0C3 */ u8 landRibbon;
    /* 0x0C4 */ u8 skyRibbon;
    /* 0x0C5 */ u8 countryRibbon;
    /* 0x0C6 */ u8 nationalRibbon;
    /* 0x0C7 */ u8 earthRibbon;
    /* 0x0C8 */ u8 worldRibbon;
    /* 0x0C9 */ u8 amariRibbon;
    /* 0x0CA */ u8 pokerus;
    /* 0x0CB */ u8 tamagoFlag;
    /* 0x0CC */ u8 tokuseiFlag;
    /* 0x0CD */ u8 fuseiFlag;
    /* 0x0CE */ u8 flagAmari;
    /* 0x0CF */ u8 pcboxMark;
    /* 0x0D0 */ u8 mailId;
    /* 0x0D1 */ u8 _pad0D1;
    /* 0x0D2 */ u16 para1Amari;
    /* 0x0D4 */ u16 amari;
    /* 0x0D6 */ u16 fightTrainerPokemonDataId;
    /* 0x0D8 */ u16 darkpokemonDataId;
    /* 0x0DA */ u8 _pad0DA[2];
    /* 0x0DC */ s32 dp;
    /* 0x0E0 */ u32 poolExp;
    /* 0x0E4 */ u16 poolFriend;
    /* 0x0E6 */ u8 _pad0E6[2];
    /* 0x0E8 */ u8 attest2[0x10];
    /* 0x0F8 */ u32 eventGetFlag;
} PokemonBios;

typedef struct PokemonTokuseiData {
    /* 0x00 */ u8 _pad00[4];
    /* 0x04 */ u32 name;
    /* 0x08 */ u32 doc;
} PokemonTokuseiData;

typedef struct PokemonSeikakuRateData {
    /* 0x00 */ u8 kake;
    /* 0x01 */ u8 waru;
} PokemonSeikakuRateData;

typedef struct PokemonSeikakuData {
    /* 0x00 */ u8 reliveFightout;
    /* 0x01 */ u8 reliveWalk;
    /* 0x02 */ u8 reliveCall;
    /* 0x03 */ u8 reliveSodateya;
    /* 0x04 */ u8 reliveNadenade;
    /* 0x05 */ u8 phyAtkRateDataId;
    /* 0x06 */ u8 phyDefRateDataId;
    /* 0x07 */ u8 speAtkRateDataId;
    /* 0x08 */ u8 speDefRateDataId;
    /* 0x09 */ u8 nimblenessRateDataId;
    /* 0x0A */ u8 _pad0A;
    /* 0x0B */ u8 unk_0B;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 _pad12[2];
    /* 0x14 */ u32 name;
    /* 0x18 */ u8 unk_18[7];
    /* 0x1F */ u8 unk_1F[7];
    /* 0x26 */ u8 _pad26[2];
} PokemonSeikakuData;

typedef struct PokemonGrowData {
    /* 0x000 */ u32 exp[0x65];
} PokemonGrowData;

typedef struct PokemonNakigoeData {
    /* 0x00 */ u8 _pad00[0x10];
    /* 0x10 */ u16 dataAddressIndex;
} PokemonNakigoeData;

#endif /* GAME_POKEMON_DATA_H */
