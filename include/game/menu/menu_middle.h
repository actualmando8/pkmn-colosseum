#ifndef GAME_MENU_MIDDLE_H
#define GAME_MENU_MIDDLE_H

#include "dolphin/types.h"

typedef struct MenuMiddleTrainerSlot {
    /* 0x0000 */ u16 trainerDataId;
    /* 0x0002 */ u16 controllerId;
    /* 0x0004 */ u32 trainerKind;
    /* 0x0008 */ u8 _pad0008[0x1C];
    /* 0x0024 */ u32 inputDevice;
    /* 0x0028 */ u32 unk_0028;
    /* 0x002C */ u8 hero[0xB18];
    /* 0x0B44 */ u8 heroCopy[0xB19];
    /* 0x165D */ u8 selectFlag;
    /* 0x165E */ u8 _pad165E[2];
} MenuMiddleTrainerSlot;

typedef struct MenuMiddleWork {
    /* 0x0000 */ u32 ruleMode;
    /* 0x0004 */ u32 battleMode;
    /* 0x0008 */ u32 controllerIndex;
    /* 0x000C */ u32 floorId;
    /* 0x0010 */ u32 randomTableIndex;
    /* 0x0014 */ u32 trainerIndex;
    /* 0x0018 */ u8 _pad0018[4];
    /* 0x001C */ u8 readyFlag;
    /* 0x001D */ u8 _pad001D[3];
    /* 0x0020 */ u32 result;
    /* 0x0024 */ MenuMiddleTrainerSlot battleSlots[4];
    /* 0x59A4 */ s32 selectedPartyIndex;
    /* 0x59A8 */ MenuMiddleTrainerSlot partySlots[4];
    /* 0xB328 */ MenuMiddleTrainerSlot backupSlot;
    /* 0xC988 */ u8 backupValid;
    /* 0xC989 */ u8 backupFlag1;
    /* 0xC98A */ u8 backupFlag2;
    /* 0xC98B */ u8 backupDirty;
    /* 0xC98C */ u8 _padC98C[0x152];
    /* 0xCADE */ u16 ruleCountA;
    /* 0xCAE0 */ u8 _padCAE0[0x52];
    /* 0xCB32 */ u16 ruleCountB;
    /* 0xCB34 */ u8 _padCB34[0x52];
    /* 0xCB86 */ u16 ruleCountC;
    /* 0xCB88 */ u8 _padCB88[0x4C];
    /* 0xCBD4 */ u8 ruleEnabled[7];
    /* 0xCBDB */ u8 ruleSlotEnabled[7][2];
} MenuMiddleWork;

#define MENU_MIDDLE_WORK(ptr) ((MenuMiddleWork*)(ptr))
#define MENU_MIDDLE_WORK_FROM_HIGH(ptr) ((MenuMiddleWork*)((u8*)(ptr) - 0x10000))

/* Typed one-field overlays for byte-exact offset accesses that are not yet tied to a single semantic owner. */
typedef struct MenuMiddleS16At0000 {
    /* 0x0000 */ s16 unk_0000;
} MenuMiddleS16At0000;
#define MENU_MIDDLE_S16_0000(ptr) ((MenuMiddleS16At0000*)(ptr))

typedef struct MenuMiddleU16At0000 {
    /* 0x0000 */ u16 unk_0000;
} MenuMiddleU16At0000;
#define MENU_MIDDLE_U16_0000(ptr) ((MenuMiddleU16At0000*)(ptr))

typedef struct MenuMiddleU32At0000 {
    /* 0x0000 */ u32 unk_0000;
} MenuMiddleU32At0000;
#define MENU_MIDDLE_U32_0000(ptr) ((MenuMiddleU32At0000*)(ptr))

typedef struct MenuMiddleU8At0000 {
    /* 0x0000 */ u8 unk_0000;
} MenuMiddleU8At0000;
#define MENU_MIDDLE_U8_0000(ptr) ((MenuMiddleU8At0000*)(ptr))

typedef struct MenuMiddleU8At0001 {
    /* 0x0000 */ u8 _pad0000[0x1];
    /* 0x0001 */ u8 unk_0001;
} MenuMiddleU8At0001;
#define MENU_MIDDLE_U8_0001(ptr) ((MenuMiddleU8At0001*)(ptr))

typedef struct MenuMiddleS16At0002 {
    /* 0x0000 */ u8 _pad0000[0x2];
    /* 0x0002 */ s16 unk_0002;
} MenuMiddleS16At0002;
#define MENU_MIDDLE_S16_0002(ptr) ((MenuMiddleS16At0002*)(ptr))

typedef struct MenuMiddleU16At0002 {
    /* 0x0000 */ u8 _pad0000[0x2];
    /* 0x0002 */ u16 unk_0002;
} MenuMiddleU16At0002;
#define MENU_MIDDLE_U16_0002(ptr) ((MenuMiddleU16At0002*)(ptr))

typedef struct MenuMiddleU8At0002 {
    /* 0x0000 */ u8 _pad0000[0x2];
    /* 0x0002 */ u8 unk_0002;
} MenuMiddleU8At0002;
#define MENU_MIDDLE_U8_0002(ptr) ((MenuMiddleU8At0002*)(ptr))

typedef struct MenuMiddleS16At0004 {
    /* 0x0000 */ u8 _pad0000[0x4];
    /* 0x0004 */ s16 unk_0004;
} MenuMiddleS16At0004;
#define MENU_MIDDLE_S16_0004(ptr) ((MenuMiddleS16At0004*)(ptr))

typedef struct MenuMiddleU16At0004 {
    /* 0x0000 */ u8 _pad0000[0x4];
    /* 0x0004 */ u16 unk_0004;
} MenuMiddleU16At0004;
#define MENU_MIDDLE_U16_0004(ptr) ((MenuMiddleU16At0004*)(ptr))

typedef struct MenuMiddleU32At0004 {
    /* 0x0000 */ u8 _pad0000[0x4];
    /* 0x0004 */ u32 unk_0004;
} MenuMiddleU32At0004;
#define MENU_MIDDLE_U32_0004(ptr) ((MenuMiddleU32At0004*)(ptr))

typedef struct MenuMiddleS16At0006 {
    /* 0x0000 */ u8 _pad0000[0x6];
    /* 0x0006 */ s16 unk_0006;
} MenuMiddleS16At0006;
#define MENU_MIDDLE_S16_0006(ptr) ((MenuMiddleS16At0006*)(ptr))

typedef struct MenuMiddleU16At0006 {
    /* 0x0000 */ u8 _pad0000[0x6];
    /* 0x0006 */ u16 unk_0006;
} MenuMiddleU16At0006;
#define MENU_MIDDLE_U16_0006(ptr) ((MenuMiddleU16At0006*)(ptr))

typedef struct MenuMiddleU16At0008 {
    /* 0x0000 */ u8 _pad0000[0x8];
    /* 0x0008 */ u16 unk_0008;
} MenuMiddleU16At0008;
#define MENU_MIDDLE_U16_0008(ptr) ((MenuMiddleU16At0008*)(ptr))

typedef struct MenuMiddleU32At0008 {
    /* 0x0000 */ u8 _pad0000[0x8];
    /* 0x0008 */ u32 unk_0008;
} MenuMiddleU32At0008;
#define MENU_MIDDLE_U32_0008(ptr) ((MenuMiddleU32At0008*)(ptr))

typedef struct MenuMiddleU8At000A {
    /* 0x0000 */ u8 _pad0000[0xA];
    /* 0x000A */ u8 unk_000A;
} MenuMiddleU8At000A;
#define MENU_MIDDLE_U8_000A(ptr) ((MenuMiddleU8At000A*)(ptr))

typedef struct MenuMiddleU16At000C {
    /* 0x0000 */ u8 _pad0000[0xC];
    /* 0x000C */ u16 unk_000C;
} MenuMiddleU16At000C;
#define MENU_MIDDLE_U16_000C(ptr) ((MenuMiddleU16At000C*)(ptr))

typedef struct MenuMiddleU32At000C {
    /* 0x0000 */ u8 _pad0000[0xC];
    /* 0x000C */ u32 unk_000C;
} MenuMiddleU32At000C;
#define MENU_MIDDLE_U32_000C(ptr) ((MenuMiddleU32At000C*)(ptr))

typedef struct MenuMiddleU8At000C {
    /* 0x0000 */ u8 _pad0000[0xC];
    /* 0x000C */ u8 unk_000C;
} MenuMiddleU8At000C;
#define MENU_MIDDLE_U8_000C(ptr) ((MenuMiddleU8At000C*)(ptr))

typedef struct MenuMiddleU8At000D {
    /* 0x0000 */ u8 _pad0000[0xD];
    /* 0x000D */ u8 unk_000D;
} MenuMiddleU8At000D;
#define MENU_MIDDLE_U8_000D(ptr) ((MenuMiddleU8At000D*)(ptr))

typedef struct MenuMiddleU8At000E {
    /* 0x0000 */ u8 _pad0000[0xE];
    /* 0x000E */ u8 unk_000E;
} MenuMiddleU8At000E;
#define MENU_MIDDLE_U8_000E(ptr) ((MenuMiddleU8At000E*)(ptr))

typedef struct MenuMiddleU8At000F {
    /* 0x0000 */ u8 _pad0000[0xF];
    /* 0x000F */ u8 unk_000F;
} MenuMiddleU8At000F;
#define MENU_MIDDLE_U8_000F(ptr) ((MenuMiddleU8At000F*)(ptr))

typedef struct MenuMiddleU32At0010 {
    /* 0x0000 */ u8 _pad0000[0x10];
    /* 0x0010 */ u32 unk_0010;
} MenuMiddleU32At0010;
#define MENU_MIDDLE_U32_0010(ptr) ((MenuMiddleU32At0010*)(ptr))

typedef struct MenuMiddleU8At0010 {
    /* 0x0000 */ u8 _pad0000[0x10];
    /* 0x0010 */ u8 unk_0010;
} MenuMiddleU8At0010;
#define MENU_MIDDLE_U8_0010(ptr) ((MenuMiddleU8At0010*)(ptr))

typedef struct MenuMiddleU8At0011 {
    /* 0x0000 */ u8 _pad0000[0x11];
    /* 0x0011 */ u8 unk_0011;
} MenuMiddleU8At0011;
#define MENU_MIDDLE_U8_0011(ptr) ((MenuMiddleU8At0011*)(ptr))

typedef struct MenuMiddleU8At0012 {
    /* 0x0000 */ u8 _pad0000[0x12];
    /* 0x0012 */ u8 unk_0012;
} MenuMiddleU8At0012;
#define MENU_MIDDLE_U8_0012(ptr) ((MenuMiddleU8At0012*)(ptr))

typedef struct MenuMiddleU8At0013 {
    /* 0x0000 */ u8 _pad0000[0x13];
    /* 0x0013 */ u8 unk_0013;
} MenuMiddleU8At0013;
#define MENU_MIDDLE_U8_0013(ptr) ((MenuMiddleU8At0013*)(ptr))

typedef struct MenuMiddleS16At0014 {
    /* 0x0000 */ u8 _pad0000[0x14];
    /* 0x0014 */ s16 unk_0014;
} MenuMiddleS16At0014;
#define MENU_MIDDLE_S16_0014(ptr) ((MenuMiddleS16At0014*)(ptr))

typedef struct MenuMiddleU16At0014 {
    /* 0x0000 */ u8 _pad0000[0x14];
    /* 0x0014 */ u16 unk_0014;
} MenuMiddleU16At0014;
#define MENU_MIDDLE_U16_0014(ptr) ((MenuMiddleU16At0014*)(ptr))

typedef struct MenuMiddleU32At0014 {
    /* 0x0000 */ u8 _pad0000[0x14];
    /* 0x0014 */ u32 unk_0014;
} MenuMiddleU32At0014;
#define MENU_MIDDLE_U32_0014(ptr) ((MenuMiddleU32At0014*)(ptr))

typedef struct MenuMiddleS16At0016 {
    /* 0x0000 */ u8 _pad0000[0x16];
    /* 0x0016 */ s16 unk_0016;
} MenuMiddleS16At0016;
#define MENU_MIDDLE_S16_0016(ptr) ((MenuMiddleS16At0016*)(ptr))

typedef struct MenuMiddleU16At0016 {
    /* 0x0000 */ u8 _pad0000[0x16];
    /* 0x0016 */ u16 unk_0016;
} MenuMiddleU16At0016;
#define MENU_MIDDLE_U16_0016(ptr) ((MenuMiddleU16At0016*)(ptr))

typedef struct MenuMiddleU16At0018 {
    /* 0x0000 */ u8 _pad0000[0x18];
    /* 0x0018 */ u16 unk_0018;
} MenuMiddleU16At0018;
#define MENU_MIDDLE_U16_0018(ptr) ((MenuMiddleU16At0018*)(ptr))

typedef struct MenuMiddleU32At0018 {
    /* 0x0000 */ u8 _pad0000[0x18];
    /* 0x0018 */ u32 unk_0018;
} MenuMiddleU32At0018;
#define MENU_MIDDLE_U32_0018(ptr) ((MenuMiddleU32At0018*)(ptr))

typedef struct MenuMiddleU16At001A {
    /* 0x0000 */ u8 _pad0000[0x1A];
    /* 0x001A */ u16 unk_001A;
} MenuMiddleU16At001A;
#define MENU_MIDDLE_U16_001A(ptr) ((MenuMiddleU16At001A*)(ptr))

typedef struct MenuMiddleU32At001C {
    /* 0x0000 */ u8 _pad0000[0x1C];
    /* 0x001C */ u32 unk_001C;
} MenuMiddleU32At001C;
#define MENU_MIDDLE_U32_001C(ptr) ((MenuMiddleU32At001C*)(ptr))

typedef struct MenuMiddleU8At001C {
    /* 0x0000 */ u8 _pad0000[0x1C];
    /* 0x001C */ u8 unk_001C;
} MenuMiddleU8At001C;
#define MENU_MIDDLE_U8_001C(ptr) ((MenuMiddleU8At001C*)(ptr))

typedef struct MenuMiddleU32At0020 {
    /* 0x0000 */ u8 _pad0000[0x20];
    /* 0x0020 */ u32 unk_0020;
} MenuMiddleU32At0020;
#define MENU_MIDDLE_U32_0020(ptr) ((MenuMiddleU32At0020*)(ptr))

typedef struct MenuMiddleU16At0024 {
    /* 0x0000 */ u8 _pad0000[0x24];
    /* 0x0024 */ u16 unk_0024;
} MenuMiddleU16At0024;
#define MENU_MIDDLE_U16_0024(ptr) ((MenuMiddleU16At0024*)(ptr))

typedef struct MenuMiddleU32At0024 {
    /* 0x0000 */ u8 _pad0000[0x24];
    /* 0x0024 */ u32 unk_0024;
} MenuMiddleU32At0024;
#define MENU_MIDDLE_U32_0024(ptr) ((MenuMiddleU32At0024*)(ptr))

typedef struct MenuMiddleU16At0026 {
    /* 0x0000 */ u8 _pad0000[0x26];
    /* 0x0026 */ u16 unk_0026;
} MenuMiddleU16At0026;
#define MENU_MIDDLE_U16_0026(ptr) ((MenuMiddleU16At0026*)(ptr))

typedef struct MenuMiddleU8At0046 {
    /* 0x0000 */ u8 _pad0000[0x46];
    /* 0x0046 */ u8 unk_0046;
} MenuMiddleU8At0046;
#define MENU_MIDDLE_U8_0046(ptr) ((MenuMiddleU8At0046*)(ptr))

typedef struct MenuMiddleU32At0048 {
    /* 0x0000 */ u8 _pad0000[0x48];
    /* 0x0048 */ u32 unk_0048;
} MenuMiddleU32At0048;
#define MENU_MIDDLE_U32_0048(ptr) ((MenuMiddleU32At0048*)(ptr))

typedef struct MenuMiddleU32At004C {
    /* 0x0000 */ u8 _pad0000[0x4C];
    /* 0x004C */ u32 unk_004C;
} MenuMiddleU32At004C;
#define MENU_MIDDLE_U32_004C(ptr) ((MenuMiddleU32At004C*)(ptr))

typedef struct MenuMiddleS16At0050 {
    /* 0x0000 */ u8 _pad0000[0x50];
    /* 0x0050 */ s16 unk_0050;
} MenuMiddleS16At0050;
#define MENU_MIDDLE_S16_0050(ptr) ((MenuMiddleS16At0050*)(ptr))

typedef struct MenuMiddleU16At0050 {
    /* 0x0000 */ u8 _pad0000[0x50];
    /* 0x0050 */ u16 unk_0050;
} MenuMiddleU16At0050;
#define MENU_MIDDLE_U16_0050(ptr) ((MenuMiddleU16At0050*)(ptr))

typedef struct MenuMiddleS16At0052 {
    /* 0x0000 */ u8 _pad0000[0x52];
    /* 0x0052 */ s16 unk_0052;
} MenuMiddleS16At0052;
#define MENU_MIDDLE_S16_0052(ptr) ((MenuMiddleS16At0052*)(ptr))

typedef struct MenuMiddleS16At0054 {
    /* 0x0000 */ u8 _pad0000[0x54];
    /* 0x0054 */ s16 unk_0054;
} MenuMiddleS16At0054;
#define MENU_MIDDLE_S16_0054(ptr) ((MenuMiddleS16At0054*)(ptr))

typedef struct MenuMiddleU16At0054 {
    /* 0x0000 */ u8 _pad0000[0x54];
    /* 0x0054 */ u16 unk_0054;
} MenuMiddleU16At0054;
#define MENU_MIDDLE_U16_0054(ptr) ((MenuMiddleU16At0054*)(ptr))

typedef struct MenuMiddleS16At0056 {
    /* 0x0000 */ u8 _pad0000[0x56];
    /* 0x0056 */ s16 unk_0056;
} MenuMiddleS16At0056;
#define MENU_MIDDLE_S16_0056(ptr) ((MenuMiddleS16At0056*)(ptr))

typedef struct MenuMiddleU32At0064 {
    /* 0x0000 */ u8 _pad0000[0x64];
    /* 0x0064 */ u32 unk_0064;
} MenuMiddleU32At0064;
#define MENU_MIDDLE_U32_0064(ptr) ((MenuMiddleU32At0064*)(ptr))

typedef struct MenuMiddleU8At0067 {
    /* 0x0000 */ u8 _pad0000[0x67];
    /* 0x0067 */ u8 unk_0067;
} MenuMiddleU8At0067;
#define MENU_MIDDLE_U8_0067(ptr) ((MenuMiddleU8At0067*)(ptr))

typedef struct MenuMiddleF32At0070 {
    /* 0x0000 */ u8 _pad0000[0x70];
    /* 0x0070 */ f32 unk_0070;
} MenuMiddleF32At0070;
#define MENU_MIDDLE_F32_0070(ptr) ((MenuMiddleF32At0070*)(ptr))

typedef struct MenuMiddleU16At0084 {
    /* 0x0000 */ u8 _pad0000[0x84];
    /* 0x0084 */ u16 unk_0084;
} MenuMiddleU16At0084;
#define MENU_MIDDLE_U16_0084(ptr) ((MenuMiddleU16At0084*)(ptr))

typedef struct MenuMiddleU16At0094 {
    /* 0x0000 */ u8 _pad0000[0x94];
    /* 0x0094 */ u16 unk_0094;
} MenuMiddleU16At0094;
#define MENU_MIDDLE_U16_0094(ptr) ((MenuMiddleU16At0094*)(ptr))

typedef struct MenuMiddleU8At0094 {
    /* 0x0000 */ u8 _pad0000[0x94];
    /* 0x0094 */ u8 unk_0094;
} MenuMiddleU8At0094;
#define MENU_MIDDLE_U8_0094(ptr) ((MenuMiddleU8At0094*)(ptr))

typedef struct MenuMiddleU8At0095 {
    /* 0x0000 */ u8 _pad0000[0x95];
    /* 0x0095 */ u8 unk_0095;
} MenuMiddleU8At0095;
#define MENU_MIDDLE_U8_0095(ptr) ((MenuMiddleU8At0095*)(ptr))

typedef struct MenuMiddleU8At0098 {
    /* 0x0000 */ u8 _pad0000[0x98];
    /* 0x0098 */ u8 unk_0098;
} MenuMiddleU8At0098;
#define MENU_MIDDLE_U8_0098(ptr) ((MenuMiddleU8At0098*)(ptr))

typedef struct MenuMiddleU8At0099 {
    /* 0x0000 */ u8 _pad0000[0x99];
    /* 0x0099 */ u8 unk_0099;
} MenuMiddleU8At0099;
#define MENU_MIDDLE_U8_0099(ptr) ((MenuMiddleU8At0099*)(ptr))

typedef struct MenuMiddleU16At1684 {
    /* 0x0000 */ u8 _pad0000[0x1684];
    /* 0x1684 */ u16 unk_1684;
} MenuMiddleU16At1684;
#define MENU_MIDDLE_U16_1684(ptr) ((MenuMiddleU16At1684*)(ptr))

typedef struct MenuMiddleU16At1686 {
    /* 0x0000 */ u8 _pad0000[0x1686];
    /* 0x1686 */ u16 unk_1686;
} MenuMiddleU16At1686;
#define MENU_MIDDLE_U16_1686(ptr) ((MenuMiddleU16At1686*)(ptr))

typedef struct MenuMiddleU32At16A8 {
    /* 0x0000 */ u8 _pad0000[0x16A8];
    /* 0x16A8 */ u32 unk_16A8;
} MenuMiddleU32At16A8;
#define MENU_MIDDLE_U32_16A8(ptr) ((MenuMiddleU32At16A8*)(ptr))

typedef struct MenuMiddleU16At2CE4 {
    /* 0x0000 */ u8 _pad0000[0x2CE4];
    /* 0x2CE4 */ u16 unk_2CE4;
} MenuMiddleU16At2CE4;
#define MENU_MIDDLE_U16_2CE4(ptr) ((MenuMiddleU16At2CE4*)(ptr))

typedef struct MenuMiddleU32At2D08 {
    /* 0x0000 */ u8 _pad0000[0x2D08];
    /* 0x2D08 */ u32 unk_2D08;
} MenuMiddleU32At2D08;
#define MENU_MIDDLE_U32_2D08(ptr) ((MenuMiddleU32At2D08*)(ptr))

typedef struct MenuMiddleU16At4344 {
    /* 0x0000 */ u8 _pad0000[0x4344];
    /* 0x4344 */ u16 unk_4344;
} MenuMiddleU16At4344;
#define MENU_MIDDLE_U16_4344(ptr) ((MenuMiddleU16At4344*)(ptr))

typedef struct MenuMiddleU32At4368 {
    /* 0x0000 */ u8 _pad0000[0x4368];
    /* 0x4368 */ u32 unk_4368;
} MenuMiddleU32At4368;
#define MENU_MIDDLE_U32_4368(ptr) ((MenuMiddleU32At4368*)(ptr))

typedef struct MenuMiddleU16At59AA {
    /* 0x0000 */ u8 _pad0000[0x59AA];
    /* 0x59AA */ u16 unk_59AA;
} MenuMiddleU16At59AA;
#define MENU_MIDDLE_U16_59AA(ptr) ((MenuMiddleU16At59AA*)(ptr))

typedef struct MenuMiddleU32At59CC {
    /* 0x0000 */ u8 _pad0000[0x59CC];
    /* 0x59CC */ u32 unk_59CC;
} MenuMiddleU32At59CC;
#define MENU_MIDDLE_U32_59CC(ptr) ((MenuMiddleU32At59CC*)(ptr))

typedef struct MenuMiddleU32At59D0 {
    /* 0x0000 */ u8 _pad0000[0x59D0];
    /* 0x59D0 */ u32 unk_59D0;
} MenuMiddleU32At59D0;
#define MENU_MIDDLE_U32_59D0(ptr) ((MenuMiddleU32At59D0*)(ptr))

typedef struct MenuMiddleU16At700A {
    /* 0x0000 */ u8 _pad0000[0x700A];
    /* 0x700A */ u16 unk_700A;
} MenuMiddleU16At700A;
#define MENU_MIDDLE_U16_700A(ptr) ((MenuMiddleU16At700A*)(ptr))

typedef struct MenuMiddleU32At7030 {
    /* 0x0000 */ u8 _pad0000[0x7030];
    /* 0x7030 */ u32 unk_7030;
} MenuMiddleU32At7030;
#define MENU_MIDDLE_U32_7030(ptr) ((MenuMiddleU32At7030*)(ptr))

typedef struct MenuMiddleNegU32At8690 {
    /* 0x8690 */ u32 unk_8690;
} MenuMiddleNegU32At8690;
#define MENU_MIDDLE_NEG_U32_8690(ptr) ((MenuMiddleNegU32At8690*)((u8*)(ptr) - 0x7970))

typedef struct MenuMiddleNegU32At9CF0 {
    /* 0x9CF0 */ u32 unk_9CF0;
} MenuMiddleNegU32At9CF0;
#define MENU_MIDDLE_NEG_U32_9CF0(ptr) ((MenuMiddleNegU32At9CF0*)((u8*)(ptr) - 0x6310))

typedef struct MenuMiddleNegU16AtB32A {
    /* 0xB32A */ u16 unk_B32A;
} MenuMiddleNegU16AtB32A;
#define MENU_MIDDLE_NEG_U16_B32A(ptr) ((MenuMiddleNegU16AtB32A*)((u8*)(ptr) - 0x4CD6))

typedef struct MenuMiddleNegU32AtB34C {
    /* 0xB34C */ u32 unk_B34C;
} MenuMiddleNegU32AtB34C;
#define MENU_MIDDLE_NEG_U32_B34C(ptr) ((MenuMiddleNegU32AtB34C*)((u8*)(ptr) - 0x4CB4))

typedef struct MenuMiddleNegU8AtC988 {
    /* 0xC988 */ u8 unk_C988;
} MenuMiddleNegU8AtC988;
#define MENU_MIDDLE_NEG_U8_C988(ptr) ((MenuMiddleNegU8AtC988*)((u8*)(ptr) - 0x3678))

typedef struct MenuMiddleNegU8AtC98B {
    /* 0xC98B */ u8 unk_C98B;
} MenuMiddleNegU8AtC98B;
#define MENU_MIDDLE_NEG_U8_C98B(ptr) ((MenuMiddleNegU8AtC98B*)((u8*)(ptr) - 0x3675))

typedef struct MenuMiddleNegU16AtCADE {
    /* 0xCADE */ u16 unk_CADE;
} MenuMiddleNegU16AtCADE;
#define MENU_MIDDLE_NEG_U16_CADE(ptr) ((MenuMiddleNegU16AtCADE*)((u8*)(ptr) - 0x3522))

typedef struct MenuMiddleNegU16AtCB32 {
    /* 0xCB32 */ u16 unk_CB32;
} MenuMiddleNegU16AtCB32;
#define MENU_MIDDLE_NEG_U16_CB32(ptr) ((MenuMiddleNegU16AtCB32*)((u8*)(ptr) - 0x34CE))

typedef struct MenuMiddleNegU16AtCB86 {
    /* 0xCB86 */ u16 unk_CB86;
} MenuMiddleNegU16AtCB86;
#define MENU_MIDDLE_NEG_U16_CB86(ptr) ((MenuMiddleNegU16AtCB86*)((u8*)(ptr) - 0x347A))

typedef struct MenuMiddleNegU8AtCBD4 {
    /* 0xCBD4 */ u8 unk_CBD4;
} MenuMiddleNegU8AtCBD4;
#define MENU_MIDDLE_NEG_U8_CBD4(ptr) ((MenuMiddleNegU8AtCBD4*)((u8*)(ptr) - 0x342C))

typedef struct MenuMiddleNegU8AtCBD5 {
    /* 0xCBD5 */ u8 unk_CBD5;
} MenuMiddleNegU8AtCBD5;
#define MENU_MIDDLE_NEG_U8_CBD5(ptr) ((MenuMiddleNegU8AtCBD5*)((u8*)(ptr) - 0x342B))

typedef struct MenuMiddleNegU8AtCBD6 {
    /* 0xCBD6 */ u8 unk_CBD6;
} MenuMiddleNegU8AtCBD6;
#define MENU_MIDDLE_NEG_U8_CBD6(ptr) ((MenuMiddleNegU8AtCBD6*)((u8*)(ptr) - 0x342A))

typedef struct MenuMiddleNegU8AtCBD7 {
    /* 0xCBD7 */ u8 unk_CBD7;
} MenuMiddleNegU8AtCBD7;
#define MENU_MIDDLE_NEG_U8_CBD7(ptr) ((MenuMiddleNegU8AtCBD7*)((u8*)(ptr) - 0x3429))

typedef struct MenuMiddleNegU8AtCBD8 {
    /* 0xCBD8 */ u8 unk_CBD8;
} MenuMiddleNegU8AtCBD8;
#define MENU_MIDDLE_NEG_U8_CBD8(ptr) ((MenuMiddleNegU8AtCBD8*)((u8*)(ptr) - 0x3428))

typedef struct MenuMiddleNegU8AtCBD9 {
    /* 0xCBD9 */ u8 unk_CBD9;
} MenuMiddleNegU8AtCBD9;
#define MENU_MIDDLE_NEG_U8_CBD9(ptr) ((MenuMiddleNegU8AtCBD9*)((u8*)(ptr) - 0x3427))

typedef struct MenuMiddleNegU8AtCBDB {
    /* 0xCBDB */ u8 unk_CBDB;
} MenuMiddleNegU8AtCBDB;
#define MENU_MIDDLE_NEG_U8_CBDB(ptr) ((MenuMiddleNegU8AtCBDB*)((u8*)(ptr) - 0x3425))

typedef struct MenuMiddleNegU8AtCBDC {
    /* 0xCBDC */ u8 unk_CBDC;
} MenuMiddleNegU8AtCBDC;
#define MENU_MIDDLE_NEG_U8_CBDC(ptr) ((MenuMiddleNegU8AtCBDC*)((u8*)(ptr) - 0x3424))

#endif /* GAME_MENU_MIDDLE_H */
