#ifndef MUSYX_SYNTHDATA_H
#define MUSYX_SYNTHDATA_H

#include "dolphin/types.h"

typedef struct MusyxLayer {
    u16 id;
    u8 keyLow;
    u8 keyHigh;
    s8 transpose;
    u8 volume;
    s16 priorityOffset;
    u8 panning;
    u8 reserved[3];
} MusyxLayer;

typedef struct MusyxKeymap {
    u16 id;
    s8 transpose;
    u8 panning;
    s16 priorityOffset;
    u8 reserved[2];
} MusyxKeymap;

typedef struct MusyxMacroStep {
    u32 parameter[2];
} MusyxMacroStep;

typedef struct MusyxPoolEntry {
    u32 nextOffset;
    u16 id;
    u16 reserved;
    union {
        struct {
            u32 count;
            MusyxLayer entries[1];
        } layer;
        MusyxKeymap keymaps[128];
        u8 curve[1];
        MusyxMacroStep macros[1][2];
    } data;
} MusyxPoolEntry;

typedef struct MusyxPoolData {
    u32 macroOffset;
    u32 curveOffset;
    u32 keymapOffset;
    u32 layerOffset;
} MusyxPoolData;

s32 dataInsertMacro(u16 id, void* data);
s32 dataRemoveMacro(u16 id);
s32 dataInsertKeymap(u16 id, void* data);
s32 dataRemoveKeymap(u16 id);
s32 dataInsertLayer(u16 id, void* data, u16 count);
s32 dataRemoveLayer(u16 id);
s32 dataInsertCurve(u16 id, void* data);
s32 dataRemoveCurve(u16 id);
s32 dataAddSampleReference(u16 id);
s32 dataRemoveSampleReference(u16 id);

#endif /* MUSYX_SYNTHDATA_H */
