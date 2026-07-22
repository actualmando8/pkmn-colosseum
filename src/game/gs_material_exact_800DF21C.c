#include "game/gs_material.h"
#include "hsd/hsd_mobj.h"

void GSmaterialSetAlpha(GSmaterialEntry* material, f32 alpha)
{
    HSD_MObjSetAlpha((HSD_MObj*) material->mobjPrimary, alpha);
}
