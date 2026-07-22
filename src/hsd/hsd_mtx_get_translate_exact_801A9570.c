#include "dolphin/mtx.h"

void HSD_MtxGetTranslate(Mtx matrix, Vec* translation)
{
    translation->x = matrix[0][3];
    translation->y = matrix[1][3];
    translation->z = matrix[2][3];
}
