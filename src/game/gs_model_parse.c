/**
 * @file gs_model_parse.c
 * @brief GSmodel parse (Colosseum source literally named parse.c)
 *
 * Split from gs_range_800E202C.c (0x800E9E34-0x800EB268) - one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"
#define HSD_JObjMtxIsDirty HSD_JObjMtxIsDirty_inline
#include "hsd/hsd_jobj.h"
#undef HSD_JObjMtxIsDirty

typedef void (*GSModelPObjDisp)(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                               f32 smtx[3][4], void* arg);

typedef struct GSmodel GSmodel;

void GSmodelParse(GSmodel* model, BOOL is_visible, GSModelPObjDisp disp,
                  void* arg)
{
    extern HSD_JObj* modelGetRenderJObj(GSmodel* model);
    extern void _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
        HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
        GSModelPObjDisp disp, void* arg);

    _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
        modelGetRenderJObj(model), NULL, HSD_TRSP_ALL, is_visible, disp, arg);
}

BOOL HSD_JObjMtxIsDirty(HSD_JObj* jobj)
{
    extern const char lbl_8047CC00[7];
    extern const char lbl_8047CC08[5];
    BOOL result;

    if (jobj == NULL) {
        __assert(lbl_8047CC00, 0x25D, lbl_8047CC08);
    }
    result = FALSE;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX) &&
        (jobj->flags & JOBJ_MTX_DIRTY))
    {
        result = TRUE;
    }
    return result;
}

void HSD_JObjSetupMatrix_800EA664(HSD_JObj* jobj)
{
    extern void fn_8019D9DC(HSD_JObj* jobj);

    if (jobj == NULL || !HSD_JObjMtxIsDirty(jobj)) {
        return;
    }
    fn_8019D9DC(jobj);
}

void _modelParseJObjDispDObj__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
    HSD_JObj* jobj, f32 *obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg);

void _modelParseJObjDisp__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv_800EA7E4(
    HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg)
{
    if ((jobj != NULL) && union_type_dobj(jobj)) {
        _modelParseJObjDispDObj__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
            jobj, obj_mtx, trsp_mask, is_visible, disp, arg);
    }
}
