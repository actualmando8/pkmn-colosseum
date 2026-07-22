#include "dolphin/types.h"

extern u32 fn_80113F48(void);
extern void* fn_8018D998(u32 group, u32 resource);
extern void fn_8018B220(u32 group, u32 resource);
extern void* GSresGetResource(u32 group, u32 resource);
extern void GSmodelStopAnimation(void* model);

void fn_801CB7C4(u32 resource)
{
    u32 group = fn_80113F48();
    void* model;

    if (fn_8018D998(group, resource) != NULL) {
        fn_8018B220(group, resource);
    } else {
        model = GSresGetResource(group, resource);
        if (model != NULL) {
            GSmodelStopAnimation(model);
        }
    }
}
