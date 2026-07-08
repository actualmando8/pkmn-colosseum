#include "dolphin/types.h"

#pragma section ".data"

extern u8 lbl_802E4B98[];
extern void* jumptable_802E4BB8[];
extern void* jumptable_802E4C20[];
extern void* jumptable_802E4C80[];
extern void* jumptable_802E4CA8[];
extern void* jumptable_802E4CD8[];
extern void* jumptable_802E4D2C[];

extern u8 menuFightDrawSecretPokemonStatus[];
extern u8 menuFightDrawSecretPokemon[];
extern u8 menuFightDrawSecretSelect[];
extern u8 menuFightDrawSecretWazaDoc[];
extern u8 menuFightDrawSecretWazaSelect[];
extern u8 menuFightDrawBall[];

/* Auto-carved .data unit 0x802E4B98..0x802E4D8C (7 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 lbl_802E4B98[32] = {
    0x00, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35,
    0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x41, 0x00, 0x42,
    0x00, 0x43, 0x00, 0x44, 0x00, 0x45, 0x00, 0x46,
};

void* jumptable_802E4BB8[26] = {
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x100),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x18C),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x1FC),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x1B4),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x260),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x260),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x260),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x260),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x2F8),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x2F8),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x2F8),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x2F8),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x40C),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x40C),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x40C),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x40C),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x4B8),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x504),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x5F0),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x654),
    (void*)((u8*)menuFightDrawSecretPokemonStatus + 0x72C),
};

void* jumptable_802E4C20[24] = {
    (void*)((u8*)menuFightDrawSecretPokemon + 0x80),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x78),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x70),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x68),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x60),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x58),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x80),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x78),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x70),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x68),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x60),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x58),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x80),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x78),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x70),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x68),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x60),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x58),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x80),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x78),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x70),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x68),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x60),
    (void*)((u8*)menuFightDrawSecretPokemon + 0x58),
};

void* jumptable_802E4C80[10] = {
    (void*)((u8*)menuFightDrawSecretSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretSelect + 0x70),
    (void*)((u8*)menuFightDrawSecretSelect + 0x8C),
    (void*)((u8*)menuFightDrawSecretSelect + 0x8C),
    (void*)((u8*)menuFightDrawSecretSelect + 0x8C),
    (void*)((u8*)menuFightDrawSecretSelect + 0x70),
    (void*)((u8*)menuFightDrawSecretSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretSelect + 0x8C),
    (void*)((u8*)menuFightDrawSecretSelect + 0x70),
    (void*)((u8*)menuFightDrawSecretSelect + 0x80),
};

void* jumptable_802E4CA8[12] = {
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0xBC),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0xBC),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0xBC),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0xBC),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x128),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x150),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x184),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x354),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x1EC),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x354),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x284),
    (void*)((u8*)menuFightDrawSecretWazaDoc + 0x254),
};

void* jumptable_802E4CD8[21] = {
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x88),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x90),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x78),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x94),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x88),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x90),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x78),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x88),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x90),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x78),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x88),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x90),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x78),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x80),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x88),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x90),
    (void*)((u8*)menuFightDrawSecretWazaSelect + 0x78),
};

void* jumptable_802E4D2C[24] = {
    (void*)((u8*)menuFightDrawBall + 0x58),
    (void*)((u8*)menuFightDrawBall + 0x60),
    (void*)((u8*)menuFightDrawBall + 0x68),
    (void*)((u8*)menuFightDrawBall + 0x70),
    (void*)((u8*)menuFightDrawBall + 0x78),
    (void*)((u8*)menuFightDrawBall + 0x80),
    (void*)((u8*)menuFightDrawBall + 0x58),
    (void*)((u8*)menuFightDrawBall + 0x60),
    (void*)((u8*)menuFightDrawBall + 0x68),
    (void*)((u8*)menuFightDrawBall + 0x70),
    (void*)((u8*)menuFightDrawBall + 0x78),
    (void*)((u8*)menuFightDrawBall + 0x80),
    (void*)((u8*)menuFightDrawBall + 0x58),
    (void*)((u8*)menuFightDrawBall + 0x60),
    (void*)((u8*)menuFightDrawBall + 0x68),
    (void*)((u8*)menuFightDrawBall + 0x70),
    (void*)((u8*)menuFightDrawBall + 0x78),
    (void*)((u8*)menuFightDrawBall + 0x80),
    (void*)((u8*)menuFightDrawBall + 0x58),
    (void*)((u8*)menuFightDrawBall + 0x60),
    (void*)((u8*)menuFightDrawBall + 0x68),
    (void*)((u8*)menuFightDrawBall + 0x70),
    (void*)((u8*)menuFightDrawBall + 0x78),
    (void*)((u8*)menuFightDrawBall + 0x80),
};

