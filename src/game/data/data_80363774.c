#include "dolphin/types.h"

#pragma section ".data"

extern u8 pad_05_80363774_data[];
extern void* msgctrlcode[];
extern void* jumptable_80363A70[];
extern void* jumptable_80363A9C[];
extern void* jumptable_80363AC8[];
extern void* jumptable_80363AF0[];
extern u8 lbl_80363B18[];
extern u8 lbl_80363B78[];
extern u8 lbl_80363B88[];
extern u8 lbl_80363C00[];
extern void* jumptable_80363C70[];

extern u8 fn_80135030[];
extern u8 fn_80135168[];
extern u8 fn_801364A8[];
extern u8 msgctrlAlign[];
extern u8 msgctrlAttackMons[];
extern u8 msgctrlBaseLineBias[];
extern u8 msgctrlCR[];
extern u8 msgctrlClientMos[];
extern u8 msgctrlClientnowork[];
extern u8 msgctrlColor[];
extern u8 msgctrlDeffenceMons[];
extern u8 msgctrlDigit[];
extern u8 msgctrlDigit2[];
extern u8 msgctrlEnemyMons[];
extern u8 msgctrlEnemyMons2[];
extern u8 msgctrlEnemyTmons[];
extern u8 msgctrlEnemyTmons2[];
extern u8 msgctrlEvStrBuf0[];
extern u8 msgctrlEvStrBuf1[];
extern u8 msgctrlEvStrBuf2[];
extern u8 msgctrlFont[];
extern u8 msgctrlHero[];
extern u8 msgctrlHizuki[];
extern u8 msgctrlIndentOff[];
extern u8 msgctrlItem[];
extern u8 msgctrlItem2[];
extern u8 msgctrlItemName[];
extern u8 msgctrlKeyEnd[];
extern u8 msgctrlKeyWait[];
extern u8 msgctrlLineSpace[];
extern u8 msgctrlMenuDigit[];
extern u8 msgctrlMenuDigit2[];
extern u8 msgctrlMenuFullDigit[];
extern u8 msgctrlMenuHex[];
extern u8 msgctrlMenuHex2[];
extern u8 msgctrlMenuMoney[];
extern u8 msgctrlMenuMsg[];
extern u8 msgctrlMenuMsg2[];
extern u8 msgctrlMenuMsgID[];
extern u8 msgctrlMenuMsgID2[];
extern u8 msgctrlMenuPokemon[];
extern u8 msgctrlMenuUDigit[];
extern u8 msgctrlMenuUDigit2[];
extern u8 msgctrlMenuZDigit[];
extern u8 msgctrlMenuZDigit2[];
extern u8 msgctrlMoney[];
extern u8 msgctrlMsgID[];
extern u8 msgctrlMyMons[];
extern u8 msgctrlMyMons2[];
extern u8 msgctrlMyName[];
extern u8 msgctrlNpc[];
extern u8 msgctrlPalette[];
extern u8 msgctrlPasoName[];
extern u8 msgctrlPokemon[];
extern u8 msgctrlPokemon2[];
extern u8 msgctrlPokemonID[];
extern u8 msgctrlRubyEnd[];
extern u8 msgctrlRubyStart[];
extern u8 msgctrlRubyTop[];
extern u8 msgctrlSeOff[];
extern u8 msgctrlSeOn[];
extern u8 msgctrlShadow[];
extern u8 msgctrlSideAttackNameha[];
extern u8 msgctrlSideAttackNameno[];
extern u8 msgctrlSideAttackNamewo[];
extern u8 msgctrlSideDefenceNameha[];
extern u8 msgctrlSideDefenceNameno[];
extern u8 msgctrlSideDefenceNamewo[];
extern u8 msgctrlSndPlay[];
extern u8 msgctrlSndWait[];
extern u8 msgctrlSpeabiNamea[];
extern u8 msgctrlSpeabiNamec[];
extern u8 msgctrlSpeabiNamed[];
extern u8 msgctrlSpeabiNamet[];
extern u8 msgctrlString[];
extern u8 msgctrlString2[];
extern u8 msgctrlTalkSE[];
extern u8 msgctrlTime[];
extern u8 msgctrlTrainerClientno[];
extern u8 msgctrlTrainerEnename[];
extern u8 msgctrlTrainerEnename2[];
extern u8 msgctrlTrainerLose[];
extern u8 msgctrlTrainerName[];
extern u8 msgctrlTrainerType[];
extern u8 msgctrlTribe[];
extern u8 msgctrlTsuikaMons[];
extern u8 msgctrlWait[];
extern u8 msgctrlWaza[];
extern u8 msgctrlWazaName[];
extern u8 statusGetStatus[];
extern u8 statusSetStatus[];

/* Auto-carved .data unit 0x80363774..0x80363CA4 (11 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 pad_05_80363774_data[4] = {
    0x00, 0x00, 0x00, 0x00,
};

void* msgctrlcode[190] = {
    (void*)0x08000000,
    (void*)((u8*)msgctrlCR),
    (void*)0x08000000,
    (void*)((u8*)msgctrlCR),
    (void*)0x30000000,
    (void*)((u8*)msgctrlKeyEnd),
    (void*)0x38000000,
    (void*)((u8*)msgctrlKeyWait),
    (void*)0x18000000,
    (void*)((u8*)msgctrlRubyStart),
    (void*)0x18000000,
    (void*)((u8*)msgctrlRubyTop),
    (void*)0x18000000,
    (void*)((u8*)msgctrlRubyEnd),
    (void*)0x18000000,
    (void*)((u8*)msgctrlFont),
    (void*)0x18000000,
    (void*)((u8*)msgctrlColor),
    (void*)0x38000000,
    (void*)((u8*)msgctrlWait),
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x58000000,
    (void*)((u8*)msgctrlEvStrBuf0),
    (void*)0x58000000,
    (void*)((u8*)msgctrlEvStrBuf1),
    (void*)0x58000000,
    (void*)((u8*)msgctrlAttackMons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlDeffenceMons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlClientMos),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTsuikaMons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMyName),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMyMons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMyMons2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlEnemyMons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlEnemyMons2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlEnemyTmons),
    (void*)0x58000000,
    (void*)((u8*)msgctrlEnemyTmons2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSpeabiNamea),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSpeabiNamed),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSpeabiNamec),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSpeabiNamet),
    (void*)0x58000000,
    (void*)((u8*)msgctrlClientnowork),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideAttackNameha),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideAttackNamewo),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideAttackNameno),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerType),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerName),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerLose),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerEnename),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerEnename2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTrainerClientno),
    (void*)0x58000000,
    (void*)((u8*)msgctrlWazaName),
    (void*)0x58000000,
    (void*)((u8*)msgctrlItemName),
    (void*)0x58000000,
    (void*)((u8*)msgctrlPasoName),
    (void*)0x58000000,
    (void*)((u8*)msgctrlHero),
    (void*)0x58000000,
    (void*)((u8*)msgctrlHizuki),
    (void*)0x98000000,
    (void*)((u8*)msgctrlItem),
    (void*)0x98000000,
    (void*)((u8*)msgctrlItem2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlDigit),
    (void*)0x58000000,
    (void*)((u8*)msgctrlDigit2),
    (void*)0x98000000,
    (void*)((u8*)msgctrlMsgID),
    (void*)0x58000000,
    (void*)((u8*)msgctrlPokemon),
    (void*)0x58000000,
    (void*)((u8*)msgctrlPokemon2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuDigit),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuDigit2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuPokemon),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuMsg),
    (void*)0x18000000,
    (void*)((u8*)msgctrlPalette),
    (void*)0x98000000,
    (void*)((u8*)msgctrlWaza),
    (void*)0x98000000,
    (void*)0x00000000,
    (void*)0x98000000,
    (void*)0x00000000,
    (void*)0x58000000,
    (void*)0x00000000,
    (void*)0x10000000,
    (void*)((u8*)msgctrlSeOff),
    (void*)0x10000000,
    (void*)((u8*)msgctrlSeOn),
    (void*)0x18000000,
    (void*)0x00000000,
    (void*)0x18000000,
    (void*)0x00000000,
    (void*)0x58000000,
    (void*)((u8*)msgctrlEvStrBuf2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideDefenceNameha),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideDefenceNamewo),
    (void*)0x58000000,
    (void*)((u8*)msgctrlSideDefenceNameno),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuUDigit),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuUDigit2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuHex),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuHex2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuZDigit),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuZDigit2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMoney),
    (void*)0x58000000,
    (void*)((u8*)msgctrlTime),
    (void*)0x58000000,
    (void*)((u8*)msgctrlString),
    (void*)0x98000000,
    (void*)((u8*)msgctrlPokemonID),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuFullDigit),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuMoney),
    (void*)0x58000000,
    (void*)((u8*)msgctrlMenuMsg2),
    (void*)0x08000000,
    (void*)((u8*)msgctrlShadow),
    (void*)0x08000000,
    (void*)((u8*)msgctrlAlign),
    (void*)0x10000000,
    (void*)((u8*)msgctrlTalkSE),
    (void*)0x98000000,
    (void*)((u8*)msgctrlMenuMsgID),
    (void*)0x98000000,
    (void*)((u8*)msgctrlMenuMsgID2),
    (void*)0x58000000,
    (void*)((u8*)msgctrlString2),
    (void*)0x98000000,
    (void*)((u8*)msgctrlTribe),
    (void*)0x98000000,
    (void*)((u8*)msgctrlNpc),
    (void*)0x10000000,
    (void*)((u8*)msgctrlIndentOff),
    (void*)0x18000000,
    (void*)((u8*)msgctrlLineSpace),
    (void*)0x18000000,
    (void*)((u8*)msgctrlBaseLineBias),
    (void*)0x10000000,
    (void*)((u8*)msgctrlSndPlay),
    (void*)0x30000000,
    (void*)((u8*)msgctrlSndWait),
};

void* jumptable_80363A70[11] = {
    (void*)((u8*)fn_80135030 + 0x11C),
    (void*)((u8*)fn_80135030 + 0x98),
    (void*)((u8*)fn_80135030 + 0xA8),
    (void*)((u8*)fn_80135030 + 0x11C),
    (void*)((u8*)fn_80135030 + 0xB4),
    (void*)((u8*)fn_80135030 + 0xC0),
    (void*)((u8*)fn_80135030 + 0xCC),
    (void*)((u8*)fn_80135030 + 0xF0),
    (void*)((u8*)fn_80135030 + 0xFC),
    (void*)((u8*)fn_80135030 + 0x108),
    (void*)((u8*)fn_80135030 + 0x114),
};

void* jumptable_80363A9C[11] = {
    (void*)((u8*)fn_80135168 + 0x108),
    (void*)((u8*)fn_80135168 + 0xA8),
    (void*)((u8*)fn_80135168 + 0xB0),
    (void*)((u8*)fn_80135168 + 0x108),
    (void*)((u8*)fn_80135168 + 0xB8),
    (void*)((u8*)fn_80135168 + 0xC0),
    (void*)((u8*)fn_80135168 + 0xC8),
    (void*)((u8*)fn_80135168 + 0xDC),
    (void*)((u8*)fn_80135168 + 0xE4),
    (void*)((u8*)fn_80135168 + 0xF0),
    (void*)((u8*)fn_80135168 + 0xFC),
};

void* jumptable_80363AC8[10] = {
    (void*)((u8*)statusSetStatus + 0x11C),
    (void*)((u8*)statusSetStatus + 0x38),
    (void*)((u8*)statusSetStatus + 0x4C),
    (void*)((u8*)statusSetStatus + 0x60),
    (void*)((u8*)statusSetStatus + 0x74),
    (void*)((u8*)statusSetStatus + 0x90),
    (void*)((u8*)statusSetStatus + 0xAC),
    (void*)((u8*)statusSetStatus + 0xC8),
    (void*)((u8*)statusSetStatus + 0xE8),
    (void*)((u8*)statusSetStatus + 0x104),
};

void* jumptable_80363AF0[10] = {
    (void*)((u8*)statusGetStatus + 0x30),
    (void*)((u8*)statusGetStatus + 0x38),
    (void*)((u8*)statusGetStatus + 0x48),
    (void*)((u8*)statusGetStatus + 0x5C),
    (void*)((u8*)statusGetStatus + 0x70),
    (void*)((u8*)statusGetStatus + 0x88),
    (void*)((u8*)statusGetStatus + 0xA0),
    (void*)((u8*)statusGetStatus + 0xB8),
    (void*)((u8*)statusGetStatus + 0xD0),
    (void*)((u8*)statusGetStatus + 0xE8),
};

u8 lbl_80363B18[96] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x02,
    0x00, 0x83, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x07, 0x00, 0x00, 0x0A,
    0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00,
    0x00, 0x50, 0x00, 0x00, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x4F,
    0x00, 0x00, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x07, 0x00,
    0x00, 0x0A, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x07, 0x00, 0x00, 0x0A,
    0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80363B78[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80363B88[120] = {
    0x01, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x0D, 0x5D, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x76, 0x52, 0x00, 0x00, 0x76, 0x53, 0x00, 0x00, 0x76, 0x54,
    0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x0D, 0x5E, 0x00, 0x00, 0x76, 0xAA,
    0x00, 0x00, 0x76, 0x4A, 0x00, 0x00, 0x76, 0x4C, 0x00, 0x00, 0x76, 0x4E,
    0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x0D, 0x5F, 0x00, 0x00, 0x76, 0xAB,
    0x00, 0x00, 0x76, 0x4F, 0x00, 0x00, 0x76, 0x50, 0x00, 0x00, 0x76, 0x51,
    0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x0D, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x76, 0x55, 0x00, 0x00, 0x76, 0x56, 0x00, 0x00, 0x76, 0x57,
};

u8 lbl_80363C00[112] = {
    0x0C, 0x00, 0x00, 0x02, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x49,
    0x0C, 0x00, 0x00, 0x01, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4A,
    0x04, 0x00, 0x00, 0x1B, 0x00, 0x59, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4B,
    0x0B, 0x00, 0x00, 0x17, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4C,
    0x0B, 0x00, 0x00, 0x16, 0x00, 0x39, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4D,
    0x0B, 0x00, 0x00, 0x18, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4E,
    0x05, 0x00, 0x00, 0x07, 0x00, 0x9D, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x4F,
    0x05, 0x00, 0x00, 0x08, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x50,
    0x00, 0x00, 0x00, 0x05, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x51,
    0x00, 0x00, 0x00, 0x00,
};

void* jumptable_80363C70[13] = {
    (void*)((u8*)fn_801364A8 + 0xBC),
    (void*)((u8*)fn_801364A8 + 0xD4),
    (void*)((u8*)fn_801364A8 + 0x24C),
    (void*)((u8*)fn_801364A8 + 0x344),
    (void*)((u8*)fn_801364A8 + 0x35C),
    (void*)((u8*)fn_801364A8 + 0x4A4),
    (void*)((u8*)fn_801364A8 + 0x5F0),
    (void*)((u8*)fn_801364A8 + 0x608),
    (void*)((u8*)fn_801364A8 + 0x768),
    (void*)((u8*)fn_801364A8 + 0x8D8),
    (void*)((u8*)fn_801364A8 + 0x8F0),
    (void*)((u8*)fn_801364A8 + 0xA0C),
    (void*)((u8*)fn_801364A8 + 0xB04),
};

