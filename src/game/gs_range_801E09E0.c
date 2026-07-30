/**
 * @file gs_range_801E09E0.c
 * @brief gs-engine, 0x801E09E0 - 0x801E1B54.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

void etctoolSetPokemonNakigoe(void)
{
    void *pokemonData;
    u16 voice;

    extern void *pokemonDataBiosGetPtr(void);
    extern u16 pokemonDataBiosGetVoice(void *);
    extern void fn_80166AB8(u32, u32, u32);

    pokemonData = pokemonDataBiosGetPtr();
    if (pokemonData != NULL) {
        voice = pokemonDataBiosGetVoice(pokemonData);
        fn_80166AB8((u32)voice, 0, 0);
    }
}

void fn_801E0FB4(s32 flags, u32 setupCamera, u32 resetQueue)
{
    extern u32 lbl_80467CF8[];
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern s32 lbl_8047B428;
    extern u32 lbl_8047B42C;
    extern u32 lbl_8047B430;
    extern u8 lbl_8047B43C;
    extern void GSgappBlock(u32 taskId);
    extern void GSgappUnblock(u32 taskId);
    extern void GSthreadExecuteGroup(u32 group);
    extern void fn_800D3410(s32 mode, s32 enabled);
    extern void fn_800D3FA4(s32 flags, u32 setupCamera, u32 resetQueue);
    extern void fn_800D3190(void);
    u32 i;
    s32 enabled;

    enabled = 0;
    if (lbl_8047B420 == 0) {
        enabled = 1;
    } else {
        switch (lbl_8047B428) {
        case 0:
            enabled = 1;
            break;
        case 1:
            lbl_8047B428 = 2;
            for (i = 0; i < lbl_8047B42C; i++) {
                if (lbl_80467CF8[i] != 0) {
                    GSgappBlock(lbl_80467CF8[i]);
                }
            }
            enabled = 1;
            break;
        case 2:
        case 3:
            lbl_8047B428 = 0;
            for (i = 0; i < lbl_8047B42C; i++) {
                if (lbl_80467CF8[i] != 0) {
                    GSgappUnblock(lbl_80467CF8[i]);
                }
            }
            enabled = 0;
            break;
        }
    }

    switch (lbl_8047B424) {
    case 3:
        if (lbl_8047B428 != 2) {
            lbl_8047B428 = 1;
        }
        break;
    case 4:
        if (lbl_8047B428 == 0) {
            lbl_8047B428 = 1;
        } else {
            lbl_8047B428 = 2;
        }
        lbl_8047B430++;
        if (lbl_8047B430 >= 5) {
            lbl_8047B428 = 3;
            lbl_8047B430 = 0;
        }
        break;
    }

    if (lbl_8047B428 == 2) {
        GSthreadExecuteGroup(0xE38F910B);
    }
    if (lbl_8047B43C != 0) {
        enabled = 0;
    }
    fn_800D3410(0, enabled);
    fn_800D3FA4(flags, setupCamera, resetQueue);
    fn_800D3190();
}

void fn_801E1170(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;
    extern u32 lbl_8047B430;

    lbl_8047B424 = 4;
    lbl_8047B428 = 3;
    lbl_8047B430 = 0;
}

void fn_801E118C(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B424 = 3;
    lbl_8047B428 = 3;
}

void fn_801E119C(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B424 = 2;
    lbl_8047B428 = 3;
}

void fn_801E11B0(void)
{
    extern s32 lbl_8047B424;
    extern s32 lbl_8047B428;

    lbl_8047B424 = 1;
    if (lbl_8047B428 == 2) {
        return;
    }

    lbl_8047B428 = 1;
}

u32 fn_801E11CC(void)
{
    extern u8 lbl_8047B434;

    return lbl_8047B434;
}

void fn_801E11D4(u32 a, u8 b)
{
    extern u8 lbl_8047B434;
    extern u8 lbl_8047B435;

    lbl_8047B434 = a;
    lbl_8047B435 = b;
}

s32 fn_801E11E0(void)
{
    extern s32 lbl_8047B424;

    return lbl_8047B424;
}

u8 fn_801E11E8(void)
{
    extern u8 lbl_8047B420;

    return lbl_8047B420;
}

void fn_801E11F0(void)
{
    extern u32 lbl_80467CF8[];
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B42C;
    extern void GSgappUnblock(u32 taskId);
    u32 i;

    lbl_8047B420 = lbl_8047B424 = i = 0;
    while (i < lbl_8047B42C) {
        if (lbl_80467CF8[i] != 0) {
            GSgappUnblock(lbl_80467CF8[i]);
        }
        i++;
    }
}

void fn_801E1258(void)
{
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B420 = 1;
    lbl_8047B424 = 2;
    lbl_8047B428 = 3;
}

void GSvtrLoadTexture(void)
{
    extern u32 lbl_8047B438;
    extern u32 fn_800F92D4(u32);

    lbl_8047B438 = fn_800F92D4(0x0b521200);
}

void _vtrTexDispFunc__Fv(void)
{
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;
    extern u8 lbl_8047B434;
    extern u8 lbl_8047B435;
    extern void* lbl_8047B438;
    extern f32 lbl_8047E428;
    extern f32 lbl_8047E42C;
    extern f32 lbl_8047E430;
    extern f32 lbl_8047E434;
    extern f32 lbl_8047E438;
    extern f32 lbl_8047E43C;
    extern f32 lbl_8047E440;
    extern f32 lbl_8047E444;
    extern f32 lbl_8047E448;
    extern f32 lbl_8047E44C;
    extern f32 lbl_8047E450;
    extern f32 lbl_8047E454;
    extern char lbl_8047E458[];
    extern char lbl_8047E45C[];
    extern char lbl_8047E464[];
    extern char lbl_8047E46C[];
    extern char lbl_8047E474[];
    extern u8 lbl_80314958[];
    extern u8 lbl_80314C78[];
    extern void fn_800D88DC(s32 mode);
    extern void fn_800D888C(s32 mode);
    extern void fn_800D9B58(f32, f32, f32, f32);
    extern void fn_800DA4C4(s32, s32, s32);
    extern void fn_800DA2BC(s32, s32, s32);
    extern void fn_800DA1E8(s32, s32, s32);
    extern void fn_800DA028(s32);
    extern void fn_800D9ED8(s32);
    extern void fn_800D6A00(s32 primitive);
    extern void fn_800D7820(void* format);
    extern void fn_800D67BC(s32 count);
    extern void fn_800D6680(f32 x, f32 y, f32 z);
    extern void fn_800D5C18(s32 index, s32 red, s32 green, s32 blue);
    extern void fn_800D85D4(s32 index, void* texture);
    extern void fn_800D59B8(s32 index, f32 u, f32 v);
    extern void fn_800D6728(void);
    extern void fn_800FAEF8(s32 x, s32 y, s32 color, const char* text,
                            ...);
    const char* message;

    if (!lbl_8047B420 || !lbl_8047B434 || lbl_8047B438 == NULL) {
        return;
    }

    fn_800D88DC(1);
    fn_800D888C(4);
    fn_800D9B58(lbl_8047E428, lbl_8047E428, lbl_8047E42C,
                lbl_8047E430);
    fn_800DA4C4(0, 1, 1);
    fn_800DA2BC(1, 1, 0);
    fn_800DA1E8(0, 1, 1);
    fn_800DA028(0);
    fn_800D9ED8(1);
    fn_800D6A00(4);

    if (lbl_8047B435) {
        fn_800D888C(2);
        fn_800D7820(lbl_80314958);
        fn_800D67BC(4);
        fn_800D6680(lbl_8047E434, lbl_8047E438, lbl_8047E428);
        fn_800D5C18(0, 0xFF, 0, 0);
        fn_800D6680(lbl_8047E43C, lbl_8047E438, lbl_8047E428);
        fn_800D5C18(0, 0xFF, 0, 0);
        fn_800D6680(lbl_8047E434, lbl_8047E440, lbl_8047E428);
        fn_800D5C18(0, 0xFF, 0, 0);
        fn_800D6680(lbl_8047E43C, lbl_8047E440, lbl_8047E428);
        fn_800D5C18(0, 0xFF, 0, 0);
        fn_800D6728();
    }

    fn_800D88DC(2);
    fn_800D7820(lbl_80314C78);
    fn_800D85D4(0, lbl_8047B438);
    fn_800D67BC(4);
    fn_800D6680(lbl_8047E444, lbl_8047E448, lbl_8047E428);
    fn_800D5C18(0, 0xFF, 0xFF, 0xFF);
    fn_800D59B8(0, lbl_8047E428, lbl_8047E428);
    fn_800D6680(lbl_8047E44C, lbl_8047E448, lbl_8047E428);
    fn_800D5C18(0, 0xFF, 0xFF, 0xFF);
    fn_800D59B8(0, lbl_8047E450, lbl_8047E428);
    fn_800D6680(lbl_8047E444, lbl_8047E454, lbl_8047E428);
    fn_800D5C18(0, 0xFF, 0xFF, 0xFF);
    fn_800D59B8(0, lbl_8047E428, lbl_8047E450);
    fn_800D6680(lbl_8047E44C, lbl_8047E454, lbl_8047E428);
    fn_800D5C18(0, 0xFF, 0xFF, 0xFF);
    fn_800D59B8(0, lbl_8047E450, lbl_8047E450);
    fn_800D6728();

    message = NULL;
    switch (lbl_8047B424) {
    case 0:
        message = lbl_8047E458;
        break;
    case 1:
        message = lbl_8047E45C;
        break;
    case 2:
        message = lbl_8047E464;
        break;
    case 3:
        if (lbl_8047B428 < 2) {
            message = lbl_8047E46C;
        } else if (lbl_8047B428 == 2 || lbl_8047B428 == 3) {
            message = lbl_8047E45C;
        }
        break;
    case 4:
        message = lbl_8047E474;
        break;
    }
    if (message != NULL) {
        fn_800FAEF8(0x230, 0x2C, -1, message);
    }
    fn_800D9ED8(0);
}

s32 GSvtrRegisterGSgapp(u32 taskId)
{
    extern u32 lbl_80467CF8[];
    extern u32 lbl_8047B42C;
    u32 *entry;
    u32 i;

    if (lbl_8047B42C + 1 >= 4) {
        return 0;
    }

    entry = lbl_80467CF8;
    for (i = 0; i < 4; i++, entry++) {
        if (*entry == 0) {
            *entry = taskId;
            lbl_8047B42C++;
            return 1;
        }
    }

    return 0;
}

void fn_801E1300(void)
{
    extern u32 lbl_80467CF8[];
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B42C;
    extern u8 lbl_8047B434;
    extern u32 lbl_8047B438;
    extern u8 lbl_8047B43C;
    extern void *memset(void *dst, int value, u32 size);
    extern u32 GSgappCreate(s32 state, u8 priority, u32 param, void *func);
    extern void _vtrTexDispFunc(void);

    lbl_8047B420 = 0;
    lbl_8047B424 = 0;
    lbl_8047B42C = 0;
    lbl_8047B438 = 0;
    lbl_8047B434 = 1;
    lbl_8047B43C = 0;
    memset(lbl_80467CF8, 0, 0x10);
    GSgappCreate(1, 0xFD, 10, _vtrTexDispFunc);
}

s32 fn_801E16D0(void)
{
    extern s32 fn_801E25C8(void);

    return fn_801E25C8();
}

void fn_801E16F0(void)
{
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;
    extern u32 lbl_8047B450;
    extern void fn_801E386C(void);
    extern s32 fn_801E38D8(void);
    extern void fn_801E3F54(void);
    extern void fn_801E4724(void);
    extern u32 fn_800E202C(u32);
    extern void fn_800E24B0(u32);
    extern void fn_800E209C(u32);
    extern void GSscratchSetValid(void);
    u32 handle;
    u8 active;

    if (lbl_8047B440 == 0 || lbl_8047B441 == 0) {
        active = 0;
    } else {
        active = 1;
    }

    if (active != 0) {
        fn_801E386C();
        switch (fn_801E38D8()) {
        case 3:
        case 5:
            if (lbl_8047B441 != 0) {
                fn_801E3F54();
                fn_801E4724();
                handle = fn_800E202C(lbl_8047B450);
                if ((u16)handle != 0) {
                    fn_800E24B0(handle);
                    fn_800E209C(handle);
                }
                lbl_8047B441 = 0;
                GSscratchSetValid();
            }
            break;
        }
    }
}

void fn_801E17A8(void)
{
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;
    extern s32 lbl_8047B454;
    extern s32 lbl_8047B458;
    extern s32 lbl_8047B45C;
    extern s32 lbl_80469030[];
    extern s32 fn_801E3978(s32, s32, s32, s32, s32);
    u8 active;

    if (lbl_8047B440 == 0 || lbl_8047B441 == 0) {
        active = 0;
    } else {
        active = 1;
    }

    if (active != 0) {
        fn_801E3978(lbl_8047B45C, lbl_8047B458, lbl_8047B454,
                    lbl_80469030[0], lbl_80469030[1]);
    }
}

typedef struct GsVtrStartArgs {
    u32 resourceId;
    u32 mode;
} GsVtrStartArgs;

typedef struct GsVtrDimensions {
    u16 pad_00;
    u16 pad_02;
    u16 width;
    u16 height;
} GsVtrDimensions;

typedef struct GsVtrOrigin {
    u32 x;
    u32 y;
} GsVtrOrigin;

s32 fn_801E1924(GsVtrStartArgs* args)
{
    extern const char lbl_80279A68[];
    extern GsVtrDimensions lbl_80466BC0;
    extern u32 lbl_80469020[];
    extern GsVtrOrigin lbl_80469030;
    extern u32 lbl_8047B444;
    extern void* lbl_8047B450;
    extern u32 lbl_8047B454;
    extern u32 lbl_8047B458;
    extern void* lbl_8047B45C;
    extern void GSscratchSetInvalid(void);
    extern void GSscratchSetValid(void);
    extern s32 fn_801E4778(u32 resourceId, u8 mode);
    extern void GSlogWrite(const char* format, ...);
    extern void fn_801E3930(GsVtrOrigin* origin);
    extern void fn_801E38E8(u32* state);
    extern u32 fn_801E4650(void);
    extern u16 fn_800E2B00(u32 size, u32 alignment);
    extern void* fn_800E27B0(u16 handle);
    extern void fn_801E449C(void);
    extern u32 OSGetTick(void);
    extern s32 fn_801E40F8(u32, u32, u32);
    extern u16 fn_800E202C(void* pointer);
    extern void fn_800E24B0(u16 handle);
    extern void fn_800E209C(u16 handle);
    extern void fn_801E3858(s32* first, s32* second);
    extern void fn_8014F2DC(s32 id, u32, u32, u32, u32, u32);
    extern void fn_801E4058(void);
    extern u8 lbl_8047B441;
    u32 resourceId;
    u8 mode;
    u16 handle;
    u32 remainder;
    s32 first;
    s32 second;

    resourceId = args->resourceId;
    mode = args->mode;
    GSscratchSetInvalid();
    if (!fn_801E4778(resourceId, mode)) {
        GSlogWrite(lbl_80279A68, resourceId);
        GSscratchSetValid();
        lbl_8047B444 = 1;
        return 0;
    }

    fn_801E3930(&lbl_80469030);
    fn_801E38E8(lbl_80469020);
    lbl_8047B45C = &lbl_80466BC0;
    lbl_8047B458 = (lbl_80466BC0.width - lbl_80469030.x) >> 1;
    lbl_8047B454 = (lbl_80466BC0.height - lbl_80469030.y) >> 1;

    handle = fn_800E2B00(fn_801E4650(), 0x20);
    if (handle != 0) {
        lbl_8047B450 = fn_800E27B0(handle);
    } else {
        lbl_8047B450 = NULL;
    }
    if (lbl_8047B450 == NULL) {
        GSlogWrite(lbl_80279A68 + 0x30);
        GSscratchSetValid();
        lbl_8047B444 = 1;
        return 0;
    }

    fn_801E449C();
    if (lbl_80469020[3] != 1) {
        remainder = OSGetTick() % lbl_80469020[3];
    } else {
        remainder = 0;
    }
    if (!fn_801E40F8(0, 0, remainder)) {
        GSlogWrite(lbl_80279A68 + 0x5C);
        handle = fn_800E202C(lbl_8047B450);
        if (handle != 0) {
            fn_800E24B0(handle);
            fn_800E209C(handle);
        }
        GSscratchSetValid();
        lbl_8047B444 = 1;
        return 0;
    }

    fn_801E3858(&first, &second);
    if (first != -1) {
        fn_8014F2DC(first, 0, 0, 0, 0x7F, 0);
    }
    if (second != -1) {
        fn_8014F2DC(second, 0, 0x7F, 0, 0x7F, 0);
    }
    fn_801E4058();
    lbl_8047B441 = 1;
    lbl_8047B444 = 1;
    return 0;
}

void fn_801E1810(void)
{
    extern u8 lbl_8047B441;
    extern u32 lbl_8047B450;
    extern void fn_801E3F54(void);
    extern void fn_801E4724(void);
    extern u32 fn_800E202C(u32);
    extern void fn_800E24B0(u32);
    extern void fn_800E209C(u32);
    extern void GSscratchSetValid(void);
    u32 handle;

    if (lbl_8047B441 != 0) {
        fn_801E3F54();
        fn_801E4724();
        handle = fn_800E202C(lbl_8047B450);
        if ((u16)handle != 0) {
            fn_800E24B0(handle);
            fn_800E209C(handle);
        }
        lbl_8047B441 = 0;
        GSscratchSetValid();
    }
}

s32 fn_801E1874(void)
{
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;

    if (lbl_8047B440 == 0 || lbl_8047B441 == 0) {
        return 0;
    }
    return 1;
}

void fn_801E189C(char *path, u8 flag)
{
    struct ThreadArgs {
        char *path;
        u32 flag;
    };
    extern u8 lbl_80467D08[];
    extern u8 lbl_80468020[];
    extern s32 lbl_8047B444;
    extern struct ThreadArgs lbl_8047B448;
    extern void *fn_801E1924(void *);
    extern s32 OSCreateThread(void *, void *(*)(void *), void *, void *, u32,
                              s32, u16);
    extern s32 OSResumeThread(void *);
    extern void _threadSwitch(void);

    lbl_8047B448.flag = flag;
    lbl_8047B444 = 0;
    lbl_8047B448.path = path;
    OSCreateThread(lbl_80467D08, fn_801E1924, &lbl_8047B448,
                   lbl_80468020 + 0xFFC, 0x1000, 0x10, 1);
    OSResumeThread(lbl_80467D08);
    while (lbl_8047B444 == 0) {
        _threadSwitch();
    }
}

void fn_801E1B2C(void)
{
    extern void fn_801E4A6C(void);
    extern u8 lbl_8047B440;

    fn_801E4A6C();
    lbl_8047B440 = 1;
}
