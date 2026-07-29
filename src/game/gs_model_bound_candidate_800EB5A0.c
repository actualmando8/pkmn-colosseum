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

typedef struct ModelIntpJObj {
    u8 pad_00[8];
    struct ModelIntpJObj* next;
    struct ModelIntpJObj* parent;
    struct ModelIntpJObj* child;
    u32 flags;
    f32 rotation[4];
    GSvec scale;
    GSvec translation;
} ModelIntpJObj;

void _modelIntpJObjAll__FP8_GSmodelP9_HSD_JObjP9_HSD_JObjP9_HSD_JObjff(
    GSmodel* model, HSD_JObj* out, HSD_JObj* from, HSD_JObj* to, f32 blend)
{
    extern void fn_800EB904(GSmodel*, HSD_JObj*, HSD_JObj*, HSD_JObj*, f32);
    ModelIntpJObj* outNode;
    ModelIntpJObj* fromNode;
    ModelIntpJObj* toNode;

    if (out == NULL || from == NULL || to == NULL) {
        return;
    }

    fn_800EB904(model, out, from, to, blend);
    outNode = (ModelIntpJObj*)out;
    fromNode = (ModelIntpJObj*)from;
    toNode = (ModelIntpJObj*)to;
    if (!(outNode->flags & 0x1000)) {
        outNode = outNode->child;
        fromNode = fromNode->child;
        toNode = toNode->child;
        while (outNode != NULL) {
            _modelIntpJObjAll__FP8_GSmodelP9_HSD_JObjP9_HSD_JObjP9_HSD_JObjff(
                model, (HSD_JObj*)outNode, (HSD_JObj*)fromNode,
                (HSD_JObj*)toNode, blend);
            outNode = outNode->next;
            fromNode = fromNode->next;
            toNode = toNode->next;
        }
    }
}

void fn_800EB904(GSmodel* model, HSD_JObj* out_arg, HSD_JObj* from_arg,
                 HSD_JObj* to_arg, f32 blend)
{
    extern void fn_8019D620(HSD_JObj*);
    extern void fn_801AD7CC(f32* from, f32* to, f32* out, f32 blend);
    ModelIntpJObj* out = (ModelIntpJObj*)out_arg;
    ModelIntpJObj* from = (ModelIntpJObj*)from_arg;
    ModelIntpJObj* to = (ModelIntpJObj*)to_arg;
    f32 inverse = 1.0f - blend;

    (void)model;
    if (out == NULL || from == NULL || to == NULL) {
        return;
    }

    out->translation.x = from->translation.x * inverse +
                         to->translation.x * blend;
    out->translation.y = from->translation.y * inverse +
                         to->translation.y * blend;
    out->translation.z = from->translation.z * inverse +
                         to->translation.z * blend;
    if (!(out->flags & 0x02000000)) {
        fn_8019D620(out_arg);
    }

    out->scale.x = from->scale.x * inverse + to->scale.x * blend;
    out->scale.y = from->scale.y * inverse + to->scale.y * blend;
    out->scale.z = from->scale.z * inverse + to->scale.z * blend;
    if (!(out->flags & 0x02000000)) {
        fn_8019D620(out_arg);
    }

    fn_801AD7CC(from->rotation, to->rotation, out->rotation, blend);
    if (!(out->flags & 0x02000000)) {
        fn_8019D620(out_arg);
    }
    lbl_8047ABA0++;
}

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
