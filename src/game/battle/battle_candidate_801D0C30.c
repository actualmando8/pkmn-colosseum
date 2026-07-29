/** Candidate-only owner for 0x801D0C30 - 0x801D1338. */
#include "src/game/battle/battle_range_801D0AA0.c"

void fn_801D0DB0(s32 peopleGroup, s32 peopleId)
{
    extern u32 fn_80113F48(void);
    extern void* fn_800F92D4(u32 id);
    extern void* fn_8018D998(s32 group, s32 id);
    extern void* peopleSearchID(void*);
    extern void peopleMoveCheck(s32 group, s32 id, s32 wait);
    extern void* floorOpenObject(u32 id);
    extern void GSmodelSetAnimIndex(void* model, s32 index);
    extern void GSmodelSetAnimFrame(void* model, f32 frame);
    extern void GSmodelSetAnimRate(void* model, f32 rate);
    extern void GSmodelSetAnimType(void* model, s32 type);
    extern void GSmodelStartAnimation(void* model);
    extern u8 GSmodelIsAnimating(void* model);
    extern void GSmodelSetVisibility(void* model, s32 visible);
    extern void GSmodelFree(void* model);
    extern void fn_801653CC(s32 id, s32 fade, s32 volume);
    extern void fn_80166AB8(s32 id, s32 pan, s32 volume);
    extern void _threadSwitch(void);
    extern f32 lbl_8047E188;
    extern f32 lbl_8047E194;
    extern void* lbl_80467378[6];
    void* person;
    void* effectModel;
    u16 count;
    u16 i;

    effectModel = fn_800F92D4(fn_80113F48());
    count = fn_801D0AFC(0);
    person = peopleSearchID(fn_8018D998(peopleGroup, peopleId));
    if (person != NULL) {
        peopleMoveCheck(peopleGroup, peopleId, 1);
    }

    for (i = 0; i < count && i < 6; i++) {
        lbl_80467378[i] = floorOpenObject(fn_801D0AA0(i));
        fn_80166AB8(0x3C3, 0, 0);
        if (effectModel != NULL) {
            GSmodelSetAnimIndex(effectModel, i);
            GSmodelSetAnimFrame(effectModel, lbl_8047E188);
            GSmodelSetAnimRate(effectModel, lbl_8047E194);
            GSmodelSetAnimType(effectModel, 0);
            GSmodelStartAnimation(effectModel);
            while (GSmodelIsAnimating(effectModel)) {
                _threadSwitch();
            }
        }
        fn_801653CC(0x19, 2000, 0xFF);
    }

    for (i = 0; i < 6; i++) {
        if (lbl_80467378[i] != NULL) {
            GSmodelSetVisibility(lbl_80467378[i], 0);
            GSmodelFree(lbl_80467378[i]);
            lbl_80467378[i] = NULL;
        }
    }
}
