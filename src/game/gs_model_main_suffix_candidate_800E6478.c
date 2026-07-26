/** Residual retail-only GSmodelSetPEdescr island at 0x800E6478. */
#define GSMODEL_SUFFIX_ISOLATED
#include "src/game/gs_model_main_suffix_800E4AC0.c"
#include "game/gs_model_material_internal.h"

void GSmaterialSetPEdescr(void* material, void* descriptor);

void GSmodelSetPEdescr(GSmodel* model, void* descriptor)
{
    u16 count = GSmodelAcquireMaterials(model);
    void** materials = (void**)model->materialList;
    s32 i;

    for (i = 0; i < count; i++, materials++) {
        if (*materials != NULL) {
            GSmaterialSetPEdescr(*materials, descriptor);
        }
    }
}
