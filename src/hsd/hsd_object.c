/**
 * @file hsd_object.c
 * @brief HSD base object class initialization + GObj/flag internals.
 *
 * Colosseum address range: 0x80190E34 - 0x80191484
 * Adapted from doldecomp/melee src/sysdolphin/baselib/object.c
 */

#include "hsd/hsd_object.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"

HSD_ClassInfo hsdObj = { ObjInfoInit };
extern HSD_ClassInfo hsdClass;

/* ========================================================================= */
/*  Functions in range 0x80190E34 - 0x80191484                               */
/* ========================================================================= */

/* 0x80190E34 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80190E34(void) {
    /* TODO: match -- 0x2C bytes at 0x80190E34 */
}
#pragma pop

/* 0x80190E60 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80190E60(void) {
    /* TODO: match -- 0x2B8 bytes at 0x80190E60 */
}
#pragma pop

/* 0x80191118 | 0x240 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191118(void) {
    /* TODO: match -- 0x240 bytes at 0x80191118 */
}
#pragma pop

/* 0x80191358 | 0x108 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191358(void) {
    /* TODO: match -- 0x108 bytes at 0x80191358 */
}
#pragma pop

/* 0x80191460 | 0xC -- small: ObjInfoInit */
void fn_80191460(void) {
    /* ObjInfoInit wrapper / trampoline */
}

void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj",
                     sizeof(HSD_ObjInfo), sizeof(HSD_Obj));
}

/* ===================================================================
 * Accessor functions in this range
 * =================================================================== */

/* Address: 0x8019146C | Size: 0x8 | Pattern: simple_setter */
void fn_8019146C(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0xC) = val;
}

/* Address: 0x80191474 | Size: 0x8 | Pattern: simple_setter */
void fn_80191474(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x8) = val;
}

/* Address: 0x8019147C | Size: 0x8 | Pattern: simple_setter */
void fn_8019147C(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x4) = val;
}

/* 0x70 | fn_80191484 | generic */
void fn_80191484(void) {
    /* refs: lbl_8047B208, lbl_8047B20C, lbl_8047B210 */
    fn_800E3534();
    fn_800E27B0();
}
