/**
 * @file hsd_object.c
 * @brief HSD base object class initialization.
 *
 * Colosseum address: part of HSD library section (0x80190E34+)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/object.c
 */

#include "hsd/hsd_object.h"
#include "hsd/hsd_class.h"

HSD_ClassInfo hsdObj = { ObjInfoInit };
extern HSD_ClassInfo hsdClass;

void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj",
                     sizeof(HSD_ObjInfo), sizeof(HSD_Obj));
}
