/** Residual retail-only GSmodelEnableEnvMap island at 0x800E61BC. */
#define GSMODEL_SUFFIX_ISOLATED
#include "src/game/gs_model_main_suffix_800E4AC0.c"
#include "game/gs_model_material_internal.h"

s32 fn_800DF240(void* material);
void fn_800DF1D0(void* material, f64 blend, void* texture, void* matrix,
                 void* light);
void fn_800DF384(void* material, s32 mode);

void GSmodelEnableEnvMap(GSmodel* model, void* texture, void* matrix,
                         void* light, f32 blend)
{
    u16 count;
    void** materials;
    s32 i;

    if (model->materialCount != 0) {
        void* first = model->materialList->materials[0];
        if (first != NULL && (fn_800DF240(first) & 4) != 0) {
            return;
        }
    }

    count = GSmodelAcquireMaterials(model);
    materials = (void**)model->materialList;
    for (i = 0; i < count; i++, materials++) {
        if (*materials != NULL) {
            fn_800DF1D0(*materials, blend, texture, matrix, light);
            fn_800DF384(*materials, 4);
        }
    }
}
