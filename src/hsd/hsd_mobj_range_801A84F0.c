#include "dolphin/types.h"

extern void HSD_ObjFree(void* list, void* data);
extern void* HSD_ObjAlloc(void* list);
extern void __assert(const char* file, u32 line, const char* msg);

/* BSS vtx desc globals */
extern u8 lbl_80465620[];
extern u8 lbl_8046564C[];

/* SDA2 string constants */
extern const char lbl_8047DC48[6];
extern const char lbl_8047DC50[4];
extern const char lbl_8047DC54[4];

/* Address: 0x801A84F0 | Size: 0x34 */
/* Free matrix allocation data. */
void HSD_MtxFree(void* data) {
    if (data != NULL) {
        HSD_ObjFree(lbl_80465620, data);
    }
}

/* Address: 0x801A8524 | Size: 0x4C */
/* Allocate matrix data. */
void* HSD_MtxAlloc(void) {
    void* result;
    result = HSD_ObjAlloc(lbl_80465620);
    if (result == NULL) {
        __assert(lbl_8047DC48, 0x396, lbl_8047DC50);
    }
    return result;
}

/* Address: 0x801A8570 | Size: 0x34 */
/* Set vertex attribute format for alpha channel */
void HSD_VecFree(void* data) {
    if (data != NULL) {
        HSD_ObjFree(lbl_8046564C, data);
    }
}

/* Address: 0x801A85A4 | Size: 0x4C */
/* Allocate/init alpha channel vtx desc with assert */
void* HSD_VecAlloc(void) {
    void* result;
    result = HSD_ObjAlloc(lbl_8046564C);
    if (result == NULL) {
        __assert(lbl_8047DC48, 0x377, lbl_8047DC54);
    }
    return result;
}
