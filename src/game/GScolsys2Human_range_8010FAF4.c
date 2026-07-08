/**
 * @file GScolsys2Human_range_8010FAF4.c
 * @brief GScolsys2Human (tail) -- human/character collision queries.
 *
 * Second of six translation units recovered from the former
 * game/gs_field_colquery.c CodeCandidate bucket (0x8010F6A0-0x801140DC).
 * Only the tail of the XD GScolsys2Human.cpp TU falls in our address
 * range (the head is elsewhere); named with a _range_ suffix per
 * convention since the exact internal split boundary vs. the next
 * unit (GScolsys2Thru) is lower-confidence.
 *
 * Address range: 0x8010FAF4 - 0x80110084
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
#include "game/gs_field_colquery_types.h"

/* 0x8010FAF4 | 0x304 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010FAF4(void) {
    /* TODO: match -- 772 bytes at 0x8010FAF4 */
}
#pragma pop

/* 0x8010FDF8 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GScolsys2HumanCollision(void) {
    /* TODO: match -- 460 bytes at 0x8010FDF8 */
}
#pragma pop

/* 0x8010FFC4 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GScolsys2HumanEnable(s32 index, s32 flag) {
#pragma optimization_level 4
    extern u8* GScolsys2GetCurFloor(void);
    u8* table;
    u8* entry;
    u8* p;
    s32 result;

    if (index < 0 || index >= 0x30) {
        result = 4;
    } else {
        table = GScolsys2GetCurFloor();
        if (table == NULL) {
            result = 1;
        } else {
            p = table + index * 0x14 + 0xA00;
            if ((*(u16*)(p + 0x10) & 1) == 0) {
                result = 4;
            } else {
                entry = p;
                result = 0;
            }
        }
    }
    if (result != 0) {
        return result;
    }
    if (flag != 0) {
        *(u16*)(entry + 0x10) &= ~0x2;
    } else {
        *(u16*)(entry + 0x10) |= 0x2;
    }
    return 0;
}
#pragma pop
