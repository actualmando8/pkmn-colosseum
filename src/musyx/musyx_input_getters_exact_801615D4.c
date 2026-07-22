/**
 * @file musyx_input_getters_exact_801615D4.c
 * @brief Exact natural-C MusyX cached input getters.
 */

#include "dolphin/types.h"

extern u32 _GetInputValue(u8* obj, u8* motionBase, u8 midi, u8 midiSet);

u32 inpGetVolume(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x1)) {
        return *(u16*)(obj + 0x238);
    }
    *(u32*)(obj + 0x214) = flags & ~0x1u;
    return _GetInputValue(obj, obj + 0x218, obj[0x121], obj[0x122]);
}

u32 inpGetPanning(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x2)) {
        return *(u16*)(obj + 0x25c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x2u;
    return _GetInputValue(obj, obj + 0x23c, obj[0x121], obj[0x122]);
}

u32 inpGetSurroundPanning(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x4)) {
        return *(u16*)(obj + 0x280);
    }
    *(u32*)(obj + 0x214) = flags & ~0x4u;
    return _GetInputValue(obj, obj + 0x260, obj[0x121], obj[0x122]);
}

u32 inpGetPitchBend(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x8)) {
        return *(u16*)(obj + 0x2a4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x8u;
    return _GetInputValue(obj, obj + 0x284, obj[0x121], obj[0x122]);
}

u32 inpGetDoppler(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x10)) {
        return *(u16*)(obj + 0x2c8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x10u;
    return _GetInputValue(obj, obj + 0x2a8, obj[0x121], obj[0x122]);
}

u32 inpGetModulation(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x20)) {
        return *(u16*)(obj + 0x2ec);
    }
    *(u32*)(obj + 0x214) = flags & ~0x20u;
    return _GetInputValue(obj, obj + 0x2cc, obj[0x121], obj[0x122]);
}

u32 inpGetPedal(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x40)) {
        return *(u16*)(obj + 0x310);
    }
    *(u32*)(obj + 0x214) = flags & ~0x40u;
    return _GetInputValue(obj, obj + 0x2f0, obj[0x121], obj[0x122]);
}

u32 inpGetPreAuxA(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x100)) {
        return *(u16*)(obj + 0x358);
    }
    *(u32*)(obj + 0x214) = flags & ~0x100u;
    return _GetInputValue(obj, obj + 0x338, obj[0x121], obj[0x122]);
}

u32 inpGetReverb(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x200)) {
        return *(u16*)(obj + 0x37c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x200u;
    return _GetInputValue(obj, obj + 0x35c, obj[0x121], obj[0x122]);
}

u32 inpGetPreAuxB(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x400)) {
        return *(u16*)(obj + 0x3a0);
    }
    *(u32*)(obj + 0x214) = flags & ~0x400u;
    return _GetInputValue(obj, obj + 0x380, obj[0x121], obj[0x122]);
}

u32 inpGetPostAuxB(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x800)) {
        return *(u16*)(obj + 0x3c4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x800u;
    return _GetInputValue(obj, obj + 0x3a4, obj[0x121], obj[0x122]);
}

u32 inpGetTremolo(u8* obj)
{
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x1000)) {
        return *(u16*)(obj + 0x3e8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x1000u;
    return _GetInputValue(obj, obj + 0x3c8, obj[0x121], obj[0x122]);
}
