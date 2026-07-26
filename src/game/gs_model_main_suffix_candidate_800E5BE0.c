/** Residual retail-only GSmodelEnableModulation island at 0x800E5BE0. */
#define GSMODEL_SUFFIX_ISOLATED
#include "src/game/gs_model_main_suffix_800E4AC0.c"
#include "game/gs_model_material_internal.h"

void fn_800DF1E4(void* material, void* modulation);
void fn_800DF384(void* material, s32 mode);

void GSmodelEnableModulation(GSmodel* model, void* modulation)
{
    u16 count = GSmodelAcquireMaterials(model);
    void** materials = (void**)model->materialList;
    s32 i;

    for (i = 0; i < count; i++, materials++) {
        if (*materials != NULL) {
            fn_800DF1E4(*materials, modulation);
            fn_800DF384(*materials, 1);
        }
    }
}
