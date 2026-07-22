#include "hsd/hsd_lobj.h"

typedef struct GSlight {
    u8 _00[0xC];
    HSD_LObj* lobj;
} GSlight;

void GSlightSetTarget(GSlight* light, Vec* target)
{
    HSD_LObjSetInterest(light->lobj, target);
}

void GSlightSetPosition(GSlight* light, Vec* position)
{
    HSD_LObjSetPosition(light->lobj, position);
}
