#include "dolphin/types.h"

extern u32 lbl_80478A1C;
extern u32 lbl_80478A18;
extern u32 __PADSpec;
extern void SPEC0_MakeStatus(void);
extern void SPEC1_MakeStatus(void);
extern void SPEC2_MakeStatus(void);

void PADSetSpec(s32 spec) {
    __PADSpec = 0;
    switch (spec) {
    case 0:
        lbl_80478A1C = (u32)SPEC0_MakeStatus;
        break;
    case 1:
        lbl_80478A1C = (u32)SPEC1_MakeStatus;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        lbl_80478A1C = (u32)SPEC2_MakeStatus;
        break;
    }
    lbl_80478A18 = spec;
}
