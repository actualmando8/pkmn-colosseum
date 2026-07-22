#include "dolphin/types.h"

typedef struct FieldCameraList {
    u32* entries;
    u32 count;
} FieldCameraList;

extern void* fn_800FF56C(void);
extern void* floorDataBiosGetPtr(u32 key);
extern void* floorDataBiosGetCharInfo(void* floorData, u32 index);
extern FieldCameraList* floorDataBiosGetFieldCameraListPtr(void);
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern u8 lbl_8047AD70;
extern u8 lbl_8047AD71;
extern f32 lbl_8047AD74;
extern f32 lbl_8047AD78;
extern f32 lbl_8047AD7C;
extern f32 lbl_8047CFD0;

void* fn_8011711C(u32 index)
{
    return floorDataBiosGetCharInfo(
        floorDataBiosGetPtr((u32)fn_800FF56C()), index);
}

void fn_80117154(void)
{
    lbl_8047AD68 = 0;
    lbl_8047AD6C = 0;
}

void fn_80117164(void)
{
    FieldCameraList* list;

    list = floorDataBiosGetFieldCameraListPtr();
    lbl_8047AD68 = 0;
    lbl_8047AD6C = 0;
    lbl_8047AD70 = 0;
    lbl_8047AD71 = 1;
    lbl_8047AD74 = lbl_8047CFD0;
    lbl_8047AD78 = lbl_8047CFD0;
    lbl_8047AD7C = lbl_8047CFD0;
    if (list != 0) {
        lbl_8047AD68 = *list->entries;
        lbl_8047AD6C = list->count;
    }
}
