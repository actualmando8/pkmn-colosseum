#ifndef GAME_PEOPLE_PEOPLE_DATA_H
#define GAME_PEOPLE_PEOPLE_DATA_H

#include "dolphin/types.h"

/** Packed 16-byte item-effect parameter record. */
typedef struct ItemParamData {
    /* 0x00 */ u8 meromeroFlag : 1;
    /*      */ u8 field_00_bit6 : 1;
    /*      */ u8 criticalFlag : 1;
    /*      */ u8 attackUp : 4;
    /*      */ u8 field_00_bit0 : 1;
    /* 0x01 */ u8 defenceUp : 4;
    /*      */ u8 quickUp : 4;
    /* 0x02 */ u8 hitUp : 4;
    /*      */ u8 spAttackUp : 4;
    /* 0x03 */ u8 guardFlag : 1;
    /*      */ u8 levelUpFlag : 1;
    /*      */ u8 sleepFlag : 1;
    /*      */ u8 poisonFlag : 1;
    /*      */ u8 burnFlag : 1;
    /*      */ u8 freezeFlag : 1;
    /*      */ u8 paralyzeFlag : 1;
    /*      */ u8 confuseFlag : 1;
    /* 0x04 */ u8 ppMaxUpFlag : 1;
    /*      */ u8 reviveFlag : 1;
    /*      */ u8 ppSelectFlag : 1;
    /*      */ u8 evolutionFlag : 1;
    /*      */ u8 ppMaxFullFlag : 1;
    /*      */ u8 field_04_bits0_2 : 3;
    /* 0x05 */ s8 friend1Up;
    /* 0x06 */ s8 friend2Up;
    /* 0x07 */ s8 friend3Up;
    /* 0x08 */ u8 hpEffortUp;
    /* 0x09 */ u8 attackEffortUp;
    /* 0x0A */ u8 hpUp;
    /* 0x0B */ u8 ppUp;
    /* 0x0C */ u8 defenceEffortUp;
    /* 0x0D */ u8 quickEffortUp;
    /* 0x0E */ u8 spDefenceEffortUp;
    /* 0x0F */ u8 spAttackEffortUp;
} ItemParamData;

/*
 * String storage retained by the active data splits. The definitions live in
 * game/data/rodata_80270008.c and game/data/data_8036C52C.c respectively.
 */
typedef char PeopleMoveCheckForceEndMessage[384];
typedef char PeopleMoveCheckName[16];

extern const PeopleMoveCheckForceEndMessage lbl_80274078;
extern PeopleMoveCheckName lbl_8036C52C;

#endif /* GAME_PEOPLE_PEOPLE_DATA_H */
