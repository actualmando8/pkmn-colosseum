#include "dolphin/types.h"

extern u32 lbl_8047A2EC;
extern void menuPokemonClose(void);

void fn_80014198(u32 value)
{
    lbl_8047A2EC = value;
    menuPokemonClose();
}
