/** Candidate-only owner for 0x800E9E90 - 0x800EA60C. */
#include "src/game/gs_model_parse.c"

extern void _modelParseSetupInstanceMtx__FP5GSmtxP9_HSD_JObjP5GSmtx(
    f32 parent[3][4], HSD_JObj* jobj, f32 dst[3][4]);

void _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
    HSD_JObj* jobj, f32* obj_mtx, HSD_TrspMask trsp_mask, BOOL is_visible,
    GSModelPObjDisp disp, void* arg)
{
    HSD_JObj* child;

    if (jobj == NULL) {
        return;
    }

    if (jobj->flags & 0x1000) {
        f32 instance_mtx[3][4];

        if (jobj->flags & 0x10) {
            return;
        }
        _modelParseSetupInstanceMtx__FP5GSmtxP9_HSD_JObjP5GSmtx(
            (f32(*)[4])obj_mtx, jobj, instance_mtx);
        _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
            jobj->child, (f32*)instance_mtx, trsp_mask, is_visible, disp, arg);
        return;
    }

    if (jobj->flags & ((u32)trsp_mask << 18)) {
        _modelParseJObjDisp__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv_800EA7E4(
            jobj, obj_mtx, trsp_mask, is_visible, disp, arg);
    }
    if (jobj->flags & ((u32)trsp_mask << 28)) {
        for (child = jobj->child; child != NULL; child = child->next) {
            _modelParseJObjDispAll__FP9_HSD_JObjP5GSmtx12HSD_TrspMaskbPFP9_HSD_PObjP5GSmtxP5GSmtxP5GSmtxPv_vPv(
                child, obj_mtx, trsp_mask, is_visible, disp, arg);
        }
    }
}
