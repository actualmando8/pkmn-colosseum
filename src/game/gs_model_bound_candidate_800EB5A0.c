/** Candidate-only owner for the residual model-bound range at 0x800EB5A0. */
#define GS_MODEL_BOUND_800EB464_SUFFIX_ACTIVE
#include "src/game/gs_model_bound.c"

typedef f32 GSmtx[3][4];

extern u32 lbl_8047ABA0;
extern const char lbl_8047CC40[7];
extern const char lbl_8047CC48[5];
extern void _modelIntpJObjAll__FP8_GSmodelP9_HSD_JObjP9_HSD_JObjP9_HSD_JObjff(
    GSmodel*, HSD_JObj*, HSD_JObj*, HSD_JObj*, f32);
extern void fn_800E0560(GSmtx, const GSvec*);
extern void GSmtxMakeXRotation(GSmtx, f32);
extern void GSmtxMakeYRotation(GSmtx, f32);
extern void GSmtxMakeZRotation(GSmtx, f32);
extern void fn_800E042C(GSmtx, const GSvec*);
extern void fn_800E0290(GSmtx, GSmtx, GSmtx);

void modelCalculateBlendModel__FP8_GSmodelf(GSmodel* model, f32 unused)
{
    HSDJObj* jobj;
    GSmtx translation;
    GSmtx rotate_x;
    GSmtx rotate_y;
    GSmtx rotate_z;
    GSmtx scale;

    (void)unused;
    lbl_8047ABA0 = 0;
    _modelIntpJObjAll__FP8_GSmodelP9_HSD_JObjP9_HSD_JObjP9_HSD_JObjff(
        model, model->blendJObjA, model->blendJObjB, model->blendJObj,
        model->blendFactor);
    jobj = (HSDJObj*)model->blendJObj;
    if (jobj == NULL) {
        __assert(lbl_8047CC40, 0x47C, lbl_8047CC48);
    }
    if (jobj != NULL && HSD_JObjMtxIsDirty(jobj)) {
        fn_8019D9DC(jobj);
    }
    fn_800E0560(translation, &model->position);
    GSmtxMakeXRotation(rotate_x, model->rotation.x);
    GSmtxMakeYRotation(rotate_y, model->rotation.y);
    GSmtxMakeZRotation(rotate_z, model->rotation.z);
    fn_800E042C(scale, &model->scale);
    fn_800E0290(jobj->matrix, jobj->matrix, scale);
    fn_800E0290(jobj->matrix, jobj->matrix, translation);
    fn_800E0290(jobj->matrix, jobj->matrix, rotate_x);
    fn_800E0290(jobj->matrix, jobj->matrix, rotate_y);
    fn_800E0290(jobj->matrix, jobj->matrix, rotate_z);
}
