#include "hsd/hsd_cobj.h"
#include "hsd/hsd_class.h"

extern HSD_CObjInfo lbl_8036C678;
extern char lbl_80274628[];
extern char lbl_80274640[];

extern int CObjInit(HSD_CObj* cobj);
extern void CObjRelease(HSD_CObj* cobj);
extern void CObjAmnesia(HSD_ClassInfo* info);
extern int CObjLoad(HSD_CObj* cobj, HSD_CObjDesc* desc);
extern void CObjUpdateFunc(HSD_CObj* cobj, u32 type, f32* value);

void fn_80193C24(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&lbl_8036C678), &hsdObj, lbl_80274628,
                     lbl_80274640, sizeof(HSD_CObjInfo), sizeof(HSD_CObj));
    HSD_CLASS_INFO(&lbl_8036C678)->init =
        (int (*)(HSD_Class*)) CObjInit;
    HSD_CLASS_INFO(&lbl_8036C678)->release =
        (void (*)(HSD_Class*)) CObjRelease;
    HSD_CLASS_INFO(&lbl_8036C678)->amnesia = CObjAmnesia;
    HSD_COBJ_INFO(&lbl_8036C678)->load = CObjLoad;
    HSD_COBJ_INFO(&lbl_8036C678)->update =
        (void (*)(HSD_CObj*, u32, void*)) CObjUpdateFunc;
}
