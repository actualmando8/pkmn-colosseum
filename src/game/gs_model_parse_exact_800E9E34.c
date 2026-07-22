/** Exact GS model traversal entry point, 0x800E9E34 - 0x800E9E90. */
#include "dolphin/types.h"
#include "hsd/hsd_pobj.h"

typedef void (*GSModelPObjDisp)(HSD_PObj* pobj, f32 vmtx[3][4],
                               f32 pmtx[3][4], f32 smtx[3][4], void* arg);

typedef struct GSmodel GSmodel;

extern void* modelGetRenderJObj(GSmodel* model);
extern void _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
    HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg);

void GSmodelParse(GSmodel* model, BOOL is_visible, GSModelPObjDisp disp,
                  void* arg)
{
    _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
        modelGetRenderJObj(model), NULL, 7, is_visible, disp, arg);
}
