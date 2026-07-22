/** Exact GS model DObj traversal dispatcher, 0x800EA7E4 - 0x800EA820. */
#include "dolphin/types.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_pobj.h"

typedef void (*GSModelPObjDisp)(HSD_PObj* pobj, f32 vmtx[3][4],
                               f32 pmtx[3][4], f32 smtx[3][4], void* arg);

extern void _modelParseJObjDispDObj__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
    HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg);

void _modelParseJObjDisp__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv_800EA7E4(
    HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg)
{
    if (jobj != NULL && union_type_dobj(jobj)) {
        _modelParseJObjDispDObj__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
            jobj, obj_mtx, trsp_mask, is_visible, disp, arg);
    }
}
