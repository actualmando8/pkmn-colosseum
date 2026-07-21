#include "dolphin/types.h"

u32 floorReadMakeFogResID(u32 value) {
    return (value & 0x7FFF0000U) | 0x1A00;
}

u32 floorReadMakeCameraResID(u32 value) {
    return (value & 0x7FFF0000U) | 0x1800;
}

u32 floorReadMakeLightResID(u32 value) {
    return (value & 0x7FFF0000U) | 0x1600;
}

u32 floorReadMakeModelResID(u32 value) {
    return (value & 0x7FFF0000U) | 0x1000;
}
