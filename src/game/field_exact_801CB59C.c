#include "dolphin/types.h"

extern u32 fn_80113F48(void);
extern void* fn_8018D998(u32 group, u32 resource);
extern void fn_80184470(u32 group, u32 resource);
extern void* GSresGetResource(u32 group, u32 resource);
extern void GSmodelDetachFromGSpart(void* model, s32 arg);

s32 fn_801CB59C(u32 resource)
{
    u32 group = fn_80113F48();
    void* model;

    if (fn_8018D998(group, resource) != NULL) {
        fn_80184470(group, resource);
    } else {
        model = GSresGetResource(group, resource);
        if (model == NULL) {
            return 0;
        }
        GSmodelDetachFromGSpart(model, 1);
    }
    return 1;
}
