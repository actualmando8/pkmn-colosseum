/* Canonical HSD base-class lifecycle methods. */
#include "hsd/hsd_class.h"

extern void fn_801A6960(void* cls);

void _hsdClassDestroy(HSD_Class* cls)
{
    HSD_ClassInfo* info = cls->class_info;

    info->head.nb_exist -= 1;
    fn_801A6960(cls);
}

void _hsdClassRelease(HSD_Class* cls)
{
}

int _hsdClassInit(HSD_Class* cls)
{
    return 0;
}
