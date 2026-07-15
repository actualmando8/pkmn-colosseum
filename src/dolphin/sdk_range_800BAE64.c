/**
 * @file sdk_range_800BAE64.c
 * @brief dolphin-sdk code, 0x800BAE64 - 0x800BB2E4 (7 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct GXTexObj_800BAFFC {
    u32 data[8];
} GXTexObj_800BAFFC;

typedef struct GXTexRegion_800BAFFC GXTexRegion_800BAFFC;
typedef struct GXTlutRegion_800BB098 GXTlutRegion_800BB098;

typedef struct GXData_800BAFFC {
    u8 pad_000[0x410];
    GXTexRegion_800BAFFC* (*texRegionCallback)(GXTexObj_800BAFFC*, u32);
    GXTlutRegion_800BB098* (*tlutRegionCallback)(u32);
} GXData_800BAFFC;

void GXLoadTexObj(GXTexObj_800BAFFC* obj, u32 mapID) {
    extern GXData_800BAFFC* gx;
    extern void fn_800BAE64(GXTexObj_800BAFFC*, GXTexRegion_800BAFFC*, u32);
    GXTexRegion_800BAFFC* region = gx->texRegionCallback(obj, mapID);

    fn_800BAE64(obj, region, mapID);
}

typedef struct GXTlutObj_800BB050 {
    u32 format;
    u32 loadTlut;
    u16 numEntries;
} GXTlutObj_800BB050;

void fn_800BB050(GXTlutObj_800BB050* obj, void* data, u32 format,
                 u16 numEntries) {
    obj->format = 0;
    obj->format = (obj->format & ~0xC00) | (format << 10);
    obj->loadTlut =
        (obj->loadTlut & 0xFFE00000) | (((u32)data & 0x3FFFFFFF) >> 5);
    obj->loadTlut = (obj->loadTlut & 0x00FFFFFF) | 0x64000000;
    obj->numEntries = numEntries;
}

struct GXTlutRegion_800BB098 {
    u32 tmemAddrConf;
    GXTlutObj_800BB050 tlutObj;
};

void fn_800BB098(GXTlutObj_800BB050* obj, u32 tlutName) {
    extern GXData_800BAFFC* gx;
    extern void __GXFlushTextureState(void);
    GXTlutRegion_800BB098* region = gx->tlutRegionCallback(tlutName);
    u32 scratch[2];

    __GXFlushTextureState();
    *(volatile u8*)0xCC008000 = 0x61;
    *(volatile u32*)0xCC008000 = obj->loadTlut;
    *(volatile u8*)0xCC008000 = 0x61;
    *(volatile u32*)0xCC008000 = region->tmemAddrConf;
    __GXFlushTextureState();

    obj->format = (obj->format & ~0x3FF) | (region->tmemAddrConf & 0x3FF);
    region->tlutObj = *obj;
}

void GXInitTlutRegion(GXTlutRegion_800BB098* region, u32 tmemAddr,
                      u32 tlutSize) {
    region->tmemAddrConf = 0;
    region->tmemAddrConf =
        (region->tmemAddrConf & ~0x3FF) | ((tmemAddr - 0x80000) >> 9);
    region->tmemAddrConf =
        (region->tmemAddrConf & 0xFFE003FF) | (tlutSize << 10);
    region->tmemAddrConf =
        (region->tmemAddrConf & 0x00FFFFFF) | 0x65000000;
}

void GXInvalidateTexAll(void) {
    extern void __GXFlushTextureState(void);

    __GXFlushTextureState();
    *(volatile u8*)0xCC008000 = 0x61;
    *(volatile u32*)0xCC008000 = 0x66001000;
    *(volatile u8*)0xCC008000 = 0x61;
    *(volatile u32*)0xCC008000 = 0x66001100;
    __GXFlushTextureState();
}
