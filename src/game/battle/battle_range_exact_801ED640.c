/**
 * @file battle_range_exact_801ED640.c
 * @brief Exact battle resource state and texture-loading helpers.
 *
 * Address range: 0x801ED640 - 0x801ED780.
 */

#include "dolphin/types.h"

typedef struct BattleRangeVec {
    f32 x;
    f32 y;
    f32 z;
} BattleRangeVec;

typedef struct BattleRangeTextureEntry {
    u16 id;
    u16 pad02;
    void* data;
} BattleRangeTextureEntry;

typedef struct BattleRangeResource {
    u32 magic;
    u16 textureCount;
    u16 textureOffset;
    u8 pad08[0x14];
    u8 data[];
} BattleRangeResource;

extern u8 lbl_8047B5C0;
extern u8 lbl_8047B5C1;
extern BattleRangeResource* lbl_8047B5C4;
extern u8* lbl_8047B5C8;
extern BattleRangeVec lbl_80375230;
extern void* lbl_8046D630[];

extern void GSvecCopy(BattleRangeVec* dst, BattleRangeVec* src);
extern void* GStextureLoad(void* data);
extern u32 GSgappCreate(s32 state, u8 priority, u32 param,
                       void (*callback)(void));

void fn_801ED780(void);

void fn_801ED640(u8 value)
{
    lbl_8047B5C1 = value;
}

void fn_801ED648(BattleRangeVec* value)
{
    GSvecCopy(&lbl_80375230, value);
}

void fn_801ED674(void)
{
    lbl_8047B5C0 = 0;
}

void fn_801ED680(BattleRangeResource* resource)
{
    s32 i;
    BattleRangeTextureEntry* entry;
    void* texture;
    u8* data;

    lbl_8047B5C4 = resource;
    if (resource->magic == 0x7B1EE3F0) {
        data = resource->data;
        lbl_8047B5C8 = data;
        entry = (BattleRangeTextureEntry*)(data + resource->textureOffset * 12);

        for (i = 0; i < lbl_8047B5C4->textureCount; i++) {
            entry->data = (void*)((u32)entry->data + (u32)resource);
            texture = GStextureLoad(entry->data);
            if (texture == NULL) {
                break;
            }
            lbl_8046D630[entry->id] = texture;
            entry++;
        }
    }
    lbl_8047B5C0 = 1;
}

void fn_801ED740(void)
{
    lbl_8047B5C0 = 0;
    lbl_8047B5C1 = 0;
    GSgappCreate(1, 0xF0, 0xA, fn_801ED780);
}
