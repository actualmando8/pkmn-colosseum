/** Residual retail-only fn_800E584C island. */
#define GSMODEL_SUFFIX_ISOLATED
#include "src/game/gs_model_main_suffix_800E4AC0.c"
#include "game/gs_model_material_internal.h"

void** fn_800E584C(GSmodel* model, u32* count)
{
    *count = GSmodelAcquireMaterials(model);
    return (void**)model->materialList;
}
