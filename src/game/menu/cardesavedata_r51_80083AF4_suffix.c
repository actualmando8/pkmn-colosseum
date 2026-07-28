/* Score instrumentation only; not evidence of a retail TU boundary. */
#include "src/game/menu/cardesavedata_candidate_80082A88.c"

s32 fn_80083ECC(u8* destination, void* source);

s32 fn_80083D30(void* hero, u8* destination)
{
    extern void* fn_8012AC08(void*, u16);
    extern void* fn_8011F228(void*, u16);
    u32 partyIndex;
    u32 moveIndex;
    void* pokemon;

    for (partyIndex = 0; partyIndex < 6; partyIndex++) {
        pokemon = fn_8012AC08(hero, (u16)partyIndex);
        for (moveIndex = 0; moveIndex < 4; moveIndex++) {
            fn_80083ECC(destination,
                        fn_8011F228(pokemon, (u16)moveIndex));
            destination += 0x50;
        }
    }
    return 0;
}

s32 fn_80083ECC(u8* destination, void* source)
{
    extern void* fn_8011CA34(void*);
    extern u32 fn_8011C7C0(void*);
    extern const u16* fn_800FA280(u32);
    extern u32 fn_80135938(s32, s32);
    extern u32 fn_800F9AEC(u8*, const u16*, u32);
    const u16* text;
    u16 first[0x52];
    u16 second[0x52];
    u32 count;
    u32 written;
    void* entry;

    entry = fn_8011CA34(source);
    memset(destination, 0, 0x50);
    if (entry == NULL) {
        return 0;
    }

    text = fn_800FA280(fn_8011C7C0(entry));
    count = 0;
    while (*text != 0 && *text != 0xFFFF) {
        if (count < 0x50) {
            first[count++] = *text;
        }
        text++;
    }
    first[count] = 0;
    written = fn_800F9AEC(destination, first, fn_80135938(0, 5));
    destination += written;

    if (*text == 0xFFFF) {
        *destination++ = 0xFE;
        text = (const u16*)((const u8*)text + 3);
        count = 0;
        while (*text != 0 && *text != 0xFFFF) {
            if (count < 0x50) {
                second[count++] = *text;
            }
            text++;
        }
        second[count] = 0;
        destination += fn_800F9AEC(destination, second, fn_80135938(0, 5));
    }
    *destination = 0xFF;
    return 0;
}
