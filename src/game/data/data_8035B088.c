#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_8035B088[];
extern void* jumptable_8035B0F8[];
extern void* jumptable_8035B120[];
extern void* jumptable_8035B148[];
extern void* jumptable_8035B170[];
extern void* jumptable_8035B198[];
extern void* jumptable_8035B1C0[];
extern void* jumptable_8035B1E8[];
extern void* jumptable_8035B210[];
extern void* jumptable_8035B238[];
extern void* jumptable_8035B260[];
extern void* jumptable_8035B288[];
extern void* jumptable_8035B2B0[];
extern void* jumptable_8035B2D8[];
extern void* jumptable_8035B300[];
extern void* jumptable_8035B328[];
extern void* jumptable_8035B350[];
extern void* jumptable_8035B378[];
extern void* jumptable_8035B3A0[];
extern void* jumptable_8035B3C8[];

extern u8 fn_80105634[];
extern u8 winMsgCloseLevelUpStatus[];
extern u8 winMsgOpenLevelUpFiledStatus[];
extern u8 winMsgOpenLevelUpStatus[];
extern u8 winMsgCloseFight[];
extern u8 winMsgCloseCheckFight[];
extern u8 winMsgOpenFightNoWait[];
extern u8 winMsgOpenFight[];
extern u8 winMsgCheckField[];
extern u8 winMsgCloseField[];
extern u8 winMsgOpenFieldWithSE[];
extern u8 winMsgOpenField[];
extern u8 winMsgCheck[];
extern u8 winMsgClose[];
extern u8 winMsgOpenWithSE[];
extern u8 winMsgOpen[];

/* Auto-carved .data unit 0x8035B088..0x8035B3F0 (20 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_8035B088[28] = {
    (void*)((u8*)fn_80105634 + 0x9C),
    (void*)((u8*)fn_80105634 + 0x250),
    (void*)((u8*)fn_80105634 + 0x250),
    (void*)((u8*)fn_80105634 + 0x19C),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x9C),
    (void*)((u8*)fn_80105634 + 0x250),
    (void*)((u8*)fn_80105634 + 0x250),
    (void*)((u8*)fn_80105634 + 0x19C),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x274),
    (void*)((u8*)fn_80105634 + 0x9C),
    (void*)((u8*)fn_80105634 + 0x19C),
};

void* jumptable_8035B0F8[10] = {
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x50),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x58),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x60),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x68),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x70),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x78),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x80),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x88),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x90),
    (void*)((u8*)winMsgCloseLevelUpStatus + 0x98),
};

void* jumptable_8035B120[10] = {
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x6C),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x74),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x7C),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x84),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x8C),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x94),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0x9C),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0xA4),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0xAC),
    (void*)((u8*)winMsgOpenLevelUpFiledStatus + 0xB4),
};

void* jumptable_8035B148[10] = {
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x6C),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x74),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x7C),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x84),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x8C),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x94),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0x9C),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0xA4),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0xAC),
    (void*)((u8*)winMsgOpenLevelUpStatus + 0xB4),
};

void* jumptable_8035B170[10] = {
    (void*)((u8*)winMsgCloseFight + 0x50),
    (void*)((u8*)winMsgCloseFight + 0x58),
    (void*)((u8*)winMsgCloseFight + 0x60),
    (void*)((u8*)winMsgCloseFight + 0x68),
    (void*)((u8*)winMsgCloseFight + 0x70),
    (void*)((u8*)winMsgCloseFight + 0x78),
    (void*)((u8*)winMsgCloseFight + 0x80),
    (void*)((u8*)winMsgCloseFight + 0x88),
    (void*)((u8*)winMsgCloseFight + 0x90),
    (void*)((u8*)winMsgCloseFight + 0x98),
};

void* jumptable_8035B198[10] = {
    (void*)((u8*)winMsgCloseCheckFight + 0x38),
    (void*)((u8*)winMsgCloseCheckFight + 0x40),
    (void*)((u8*)winMsgCloseCheckFight + 0x48),
    (void*)((u8*)winMsgCloseCheckFight + 0x50),
    (void*)((u8*)winMsgCloseCheckFight + 0x58),
    (void*)((u8*)winMsgCloseCheckFight + 0x60),
    (void*)((u8*)winMsgCloseCheckFight + 0x68),
    (void*)((u8*)winMsgCloseCheckFight + 0x70),
    (void*)((u8*)winMsgCloseCheckFight + 0x78),
    (void*)((u8*)winMsgCloseCheckFight + 0x80),
};

void* jumptable_8035B1C0[10] = {
    (void*)((u8*)winMsgOpenFightNoWait + 0x70),
    (void*)((u8*)winMsgOpenFightNoWait + 0x78),
    (void*)((u8*)winMsgOpenFightNoWait + 0x80),
    (void*)((u8*)winMsgOpenFightNoWait + 0x88),
    (void*)((u8*)winMsgOpenFightNoWait + 0x90),
    (void*)((u8*)winMsgOpenFightNoWait + 0x98),
    (void*)((u8*)winMsgOpenFightNoWait + 0xA0),
    (void*)((u8*)winMsgOpenFightNoWait + 0xA8),
    (void*)((u8*)winMsgOpenFightNoWait + 0xB0),
    (void*)((u8*)winMsgOpenFightNoWait + 0xB8),
};

void* jumptable_8035B1E8[10] = {
    (void*)((u8*)winMsgOpenFight + 0x70),
    (void*)((u8*)winMsgOpenFight + 0x78),
    (void*)((u8*)winMsgOpenFight + 0x80),
    (void*)((u8*)winMsgOpenFight + 0x88),
    (void*)((u8*)winMsgOpenFight + 0x90),
    (void*)((u8*)winMsgOpenFight + 0x98),
    (void*)((u8*)winMsgOpenFight + 0xA0),
    (void*)((u8*)winMsgOpenFight + 0xA8),
    (void*)((u8*)winMsgOpenFight + 0xB0),
    (void*)((u8*)winMsgOpenFight + 0xB8),
};

void* jumptable_8035B210[10] = {
    (void*)((u8*)winMsgCheckField + 0x38),
    (void*)((u8*)winMsgCheckField + 0x40),
    (void*)((u8*)winMsgCheckField + 0x48),
    (void*)((u8*)winMsgCheckField + 0x50),
    (void*)((u8*)winMsgCheckField + 0x58),
    (void*)((u8*)winMsgCheckField + 0x60),
    (void*)((u8*)winMsgCheckField + 0x68),
    (void*)((u8*)winMsgCheckField + 0x70),
    (void*)((u8*)winMsgCheckField + 0x78),
    (void*)((u8*)winMsgCheckField + 0x80),
};

void* jumptable_8035B238[10] = {
    (void*)((u8*)winMsgCloseField + 0x50),
    (void*)((u8*)winMsgCloseField + 0x58),
    (void*)((u8*)winMsgCloseField + 0x60),
    (void*)((u8*)winMsgCloseField + 0x68),
    (void*)((u8*)winMsgCloseField + 0x70),
    (void*)((u8*)winMsgCloseField + 0x78),
    (void*)((u8*)winMsgCloseField + 0x80),
    (void*)((u8*)winMsgCloseField + 0x88),
    (void*)((u8*)winMsgCloseField + 0x90),
    (void*)((u8*)winMsgCloseField + 0x98),
};

void* jumptable_8035B260[10] = {
    (void*)((u8*)winMsgOpenFieldWithSE + 0x74),
    (void*)((u8*)winMsgOpenFieldWithSE + 0x7C),
    (void*)((u8*)winMsgOpenFieldWithSE + 0x84),
    (void*)((u8*)winMsgOpenFieldWithSE + 0x8C),
    (void*)((u8*)winMsgOpenFieldWithSE + 0x94),
    (void*)((u8*)winMsgOpenFieldWithSE + 0x9C),
    (void*)((u8*)winMsgOpenFieldWithSE + 0xA4),
    (void*)((u8*)winMsgOpenFieldWithSE + 0xAC),
    (void*)((u8*)winMsgOpenFieldWithSE + 0xB4),
    (void*)((u8*)winMsgOpenFieldWithSE + 0xBC),
};

void* jumptable_8035B288[10] = {
    (void*)((u8*)winMsgOpenField + 0x70),
    (void*)((u8*)winMsgOpenField + 0x78),
    (void*)((u8*)winMsgOpenField + 0x80),
    (void*)((u8*)winMsgOpenField + 0x88),
    (void*)((u8*)winMsgOpenField + 0x90),
    (void*)((u8*)winMsgOpenField + 0x98),
    (void*)((u8*)winMsgOpenField + 0xA0),
    (void*)((u8*)winMsgOpenField + 0xA8),
    (void*)((u8*)winMsgOpenField + 0xB0),
    (void*)((u8*)winMsgOpenField + 0xB8),
};

void* jumptable_8035B2B0[10] = {
    (void*)((u8*)winMsgCheck + 0x38),
    (void*)((u8*)winMsgCheck + 0x40),
    (void*)((u8*)winMsgCheck + 0x48),
    (void*)((u8*)winMsgCheck + 0x50),
    (void*)((u8*)winMsgCheck + 0x58),
    (void*)((u8*)winMsgCheck + 0x60),
    (void*)((u8*)winMsgCheck + 0x68),
    (void*)((u8*)winMsgCheck + 0x70),
    (void*)((u8*)winMsgCheck + 0x78),
    (void*)((u8*)winMsgCheck + 0x80),
};

void* jumptable_8035B2D8[10] = {
    (void*)((u8*)winMsgClose + 0x50),
    (void*)((u8*)winMsgClose + 0x58),
    (void*)((u8*)winMsgClose + 0x60),
    (void*)((u8*)winMsgClose + 0x68),
    (void*)((u8*)winMsgClose + 0x70),
    (void*)((u8*)winMsgClose + 0x78),
    (void*)((u8*)winMsgClose + 0x80),
    (void*)((u8*)winMsgClose + 0x88),
    (void*)((u8*)winMsgClose + 0x90),
    (void*)((u8*)winMsgClose + 0x98),
};

void* jumptable_8035B300[10] = {
    (void*)((u8*)winMsgOpenWithSE + 0x1D8),
    (void*)((u8*)winMsgOpenWithSE + 0x1E0),
    (void*)((u8*)winMsgOpenWithSE + 0x1E8),
    (void*)((u8*)winMsgOpenWithSE + 0x1F0),
    (void*)((u8*)winMsgOpenWithSE + 0x1F8),
    (void*)((u8*)winMsgOpenWithSE + 0x200),
    (void*)((u8*)winMsgOpenWithSE + 0x208),
    (void*)((u8*)winMsgOpenWithSE + 0x210),
    (void*)((u8*)winMsgOpenWithSE + 0x218),
    (void*)((u8*)winMsgOpenWithSE + 0x220),
};

void* jumptable_8035B328[10] = {
    (void*)((u8*)winMsgOpenWithSE + 0xFC),
    (void*)((u8*)winMsgOpenWithSE + 0x104),
    (void*)((u8*)winMsgOpenWithSE + 0x10C),
    (void*)((u8*)winMsgOpenWithSE + 0x114),
    (void*)((u8*)winMsgOpenWithSE + 0x11C),
    (void*)((u8*)winMsgOpenWithSE + 0x124),
    (void*)((u8*)winMsgOpenWithSE + 0x12C),
    (void*)((u8*)winMsgOpenWithSE + 0x134),
    (void*)((u8*)winMsgOpenWithSE + 0x13C),
    (void*)((u8*)winMsgOpenWithSE + 0x144),
};

void* jumptable_8035B350[10] = {
    (void*)((u8*)winMsgOpenWithSE + 0x54),
    (void*)((u8*)winMsgOpenWithSE + 0x5C),
    (void*)((u8*)winMsgOpenWithSE + 0x64),
    (void*)((u8*)winMsgOpenWithSE + 0x6C),
    (void*)((u8*)winMsgOpenWithSE + 0x74),
    (void*)((u8*)winMsgOpenWithSE + 0x7C),
    (void*)((u8*)winMsgOpenWithSE + 0x84),
    (void*)((u8*)winMsgOpenWithSE + 0x8C),
    (void*)((u8*)winMsgOpenWithSE + 0x94),
    (void*)((u8*)winMsgOpenWithSE + 0x9C),
};

void* jumptable_8035B378[10] = {
    (void*)((u8*)winMsgOpen + 0x1D4),
    (void*)((u8*)winMsgOpen + 0x1DC),
    (void*)((u8*)winMsgOpen + 0x1E4),
    (void*)((u8*)winMsgOpen + 0x1EC),
    (void*)((u8*)winMsgOpen + 0x1F4),
    (void*)((u8*)winMsgOpen + 0x1FC),
    (void*)((u8*)winMsgOpen + 0x204),
    (void*)((u8*)winMsgOpen + 0x20C),
    (void*)((u8*)winMsgOpen + 0x214),
    (void*)((u8*)winMsgOpen + 0x21C),
};

void* jumptable_8035B3A0[10] = {
    (void*)((u8*)winMsgOpen + 0xF8),
    (void*)((u8*)winMsgOpen + 0x100),
    (void*)((u8*)winMsgOpen + 0x108),
    (void*)((u8*)winMsgOpen + 0x110),
    (void*)((u8*)winMsgOpen + 0x118),
    (void*)((u8*)winMsgOpen + 0x120),
    (void*)((u8*)winMsgOpen + 0x128),
    (void*)((u8*)winMsgOpen + 0x130),
    (void*)((u8*)winMsgOpen + 0x138),
    (void*)((u8*)winMsgOpen + 0x140),
};

void* jumptable_8035B3C8[10] = {
    (void*)((u8*)winMsgOpen + 0x50),
    (void*)((u8*)winMsgOpen + 0x58),
    (void*)((u8*)winMsgOpen + 0x60),
    (void*)((u8*)winMsgOpen + 0x68),
    (void*)((u8*)winMsgOpen + 0x70),
    (void*)((u8*)winMsgOpen + 0x78),
    (void*)((u8*)winMsgOpen + 0x80),
    (void*)((u8*)winMsgOpen + 0x88),
    (void*)((u8*)winMsgOpen + 0x90),
    (void*)((u8*)winMsgOpen + 0x98),
};

