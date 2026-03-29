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

/* 0x80190E60 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800E053C(void);
extern void fn_800E0518(void);
extern void fn_800E04F4(void);
extern void fn_800E048C(void);
extern void fn_800E0560(void);
extern void fn_800E0290(void);
extern void fn_800DFF98(void);
extern void fn_800D2248(void);
extern void fn_800D88DC(void);
extern void fn_800D888C(void);
extern void fn_800DA028(void);
extern void fn_800DA4C4(void);
extern void fn_800D7820(void);
extern void fn_800D6A00(void);
extern void fn_800D67BC(void);
extern void fn_800D6680(void);
extern void fn_800D5CB8(void);
extern void fn_800D6728(void);
#if 1
asm void fn_80190E60(void) {
#include "src/hsd/hsd_object_fn_80190E60.inc"
}
#else
void fn_80190E60(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191118 | 0x240 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800D2F34(void);
#if 1
asm void fn_80191118(void) {
#include "src/hsd/hsd_object_fn_80191118.inc"
}
#else
void fn_80191118(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191358 | 0x108 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800E01F4(void);
extern void fn_800E0168(void);
#if 1
asm void fn_80191358(void) {
#include "src/hsd/hsd_object_fn_80191358.inc"
}
#else
void fn_80191358(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191460 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80191460(void) {
#include "src/hsd/hsd_object_fn_80191460.inc"
}
#else
void fn_80191460(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019146C | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8019146C(void) {
#include "src/hsd/hsd_object_fn_8019146C.inc"
}
#else
void fn_8019146C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191474 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80191474(void) {
#include "src/hsd/hsd_object_fn_80191474.inc"
}
#else
void fn_80191474(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019147C | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8019147C(void) {
#include "src/hsd/hsd_object_fn_8019147C.inc"
}
#else
void fn_8019147C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191484 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800E3534(void);
extern void fn_800E27B0(void);
#if 1
asm void fn_80191484(void) {
#include "src/hsd/hsd_object_fn_80191484.inc"
}
#else
void fn_80191484(void) { /* TODO */ }
#endif
#pragma pop
