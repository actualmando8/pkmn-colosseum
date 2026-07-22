#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

typedef struct HSD_ShadowData {
    u8 pad_00[4];
    u16 width;
    u16 height;
} HSD_ShadowData;

typedef struct HSD_ShadowObject {
    u8 pad_00[0x58];
    HSD_ShadowData* shadow;
} HSD_ShadowObject;

typedef struct HSD_ShadowOwner {
    u8 pad_00[8];
    HSD_ShadowObject* object;
} HSD_ShadowOwner;

extern char lbl_802752C0[];
extern char lbl_8047DDCC;
extern void fn_800B962C(u32 left, u32 top, u32 width, u32 height);
extern void fn_800B96F8(u32 width, u32 height, u32 format, u32 mipmap);

void HSD_ShadowInit(HSD_ShadowOwner* owner)
{
    HSD_ShadowObject* object;
    HSD_ShadowData* shadow;

    if (owner == NULL) {
        __assert(lbl_802752C0, 0x10C, &lbl_8047DDCC);
    }
    object = owner->object;
    shadow = object->shadow;
    fn_800B962C(0, 0, shadow->width, shadow->height);
    fn_800B96F8(shadow->width, shadow->height, 0x20, 0);
}
