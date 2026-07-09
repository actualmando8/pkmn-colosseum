/**
 * @file field_range_801CB180.c
 * @brief field/hero, 0x801CB180 - 0x801D0AA0.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) - mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct MemcardTaskState {
    s32 task_kind;
    s32 error_code;
    s32 task_result;
    s32 state;
    s32 resume_state;
    s32 card_channel;
    s32 sector_size;
    s32 memory_size;
    s32 field_20;
    s32 retry_count;
    s32 card_result;
    s32 field_2c;
    u32 serial_hi;
    s32 random_delay;
    u8 field_38[4];
    u8 callback_finished;
    u8 field_3d;
    u8 dialog_result;
    u8 initial_dialog_result;
    u8 format_requested;
    u8 serial_check_enabled;
    u8 field_42[0xE];
    void* work_buffer;
    void* card_work_area;
    s32 savedata_status;
    s32 card_work_size;
    s32 next_state_after_delay;
    void* gapp;
} MemcardTaskState;

extern u8 lbl_8047B3C8;
extern u8 lbl_8047B3D0;
extern MemcardTaskState* lbl_8047B3D4;
extern u8 lbl_80467168[];
extern const u8 lbl_802758C8[];
extern const u8 lbl_8036DCA8[];

extern u32 fn_800F7BC4(s32 pad);
extern s32 fn_800F7A7C(s32 pad, s32 axis);
extern s32 fn_800F7A08(s32 pad, s32 axis);
extern void fn_800F7068(s32 id, s32 value);
extern void fn_800F7274(s32 id);
extern void* GSthreadGetCurrentThread(void);
extern s32 fn_800F036C(void);
extern void GSlogWrite(const void* format, const void* text, ...);
extern s32 fn_800F7318(s32 task, void* callback, s32 stack_size, s32 arg3, s32 arg4, s32 arg5, ...);
extern void fn_800F7434(void* callback, s32 arg, ...);
extern u32 fn_80113F48(void);
extern void* GSresGetResource(u32 group, u32 resource);
extern void fn_80118874(void* resource, u32 arg);
extern void GSmodelLinkToGSparticleBank(void* model, void* particle_bank);
extern void GSmodelSetGSparticleLinkAttachMode(void* model, s32 mode);
extern void* fn_8018D998(u32 group, u32 resource);
extern void fn_80184470(u32 group, u32 resource);
extern void fn_801845E4(u32 group, u32 resource, u32 part_group, u32 part_resource, s32 part);
extern void fn_8018B220(u32 group, u32 resource);
extern void fn_8018B368(u32 group, u32 resource, u32 anim_index, s32 frame, u8 loop);
extern void GSmodelStopAnimation(void* model);
extern void fn_8018DB68(u32 group, u32 resource);
extern void fn_8018C1E8(u32 group, u32 resource, u8 visible);
extern void GSmodelSetVisibility(void* model, u8 visible);
extern void GSmodelDetachFromGSpart(void* model, s32 arg);
extern void* GSmodelGetPart(void* model, s32 part);
extern void GSmodelAttachToGSpart(void* model, void* part, s32 arg2, s32 arg3, s32 arg4);
extern void GSpartFree(void* part);
extern u32 peopleWaitSyncMotion(u32 group, u32 resource, u8 wait);
extern u32 GSmodelHasAnimationEnded(void* model);
extern void _threadSwitch(void);
extern void GSmodelSetAnimIndex(void* model, u32 index);
extern void GSmodelSetAnimFrame(void* model, f32 frame);
extern void GSmodelSetAnimRate(void* model, f32 rate);
extern void GSmodelSetTexAnimIndex(void* model, u32 index);
extern void GSmodelSetTexAnimFrame(void* model, f32 frame);
extern void GSmodelSetTexAnimRate(void* model, f32 rate);
extern void GSmodelSetAnimType(void* model, u32 type);
extern void GSmodelStartAnimation(void* model);
extern void* fn_8018E050(u32 group, s32 people_id, void* param);
extern void* peopleSearchID(void* people);
extern void* peopleGetModel(void* people);
extern void GSmodelSetBoundCheck(void* model, s32 enabled);
extern s32 fn_800FF58C(s32 msg_id);
extern void winMsgOpen(s32 id, s32 msg_id, s32 arg2, s32 arg3);
extern s32 fn_8001E184(void);
extern s32 fn_800889E4(s32 arg);
extern void* fn_800E202C(void* ptr);
extern void fn_800E24B0(void);
extern s32 fn_800E209C(void* ptr);
extern void fn_800E2C04(s32 size, s32 align);
extern void* fn_800E27B0(void);
extern void winMsgClose(s32 id);
extern void GSgappTerminate(void* app);
extern u32 _fadeEffectGetRandom__FUl(u32 limit);
extern s32 fn_801D0090(s32 error_code);
extern void CARDInit(void);

#pragma push
#pragma scheduling off
#pragma peephole off
s32 scriptIsTrigerPush(void)
{
    s32 pushed = 0;

    if ((fn_800F7BC4(1) & 0x1F70) != 0) {
        pushed = 1;
    }

#pragma scheduling on
    return pushed;
}
#pragma pop

#pragma push
#pragma scheduling off
#pragma peephole off
s32 fn_801CB1C4(void)
{
    s32 pushed = 0;
    s32 axis;
    s32 buttons;

    buttons = fn_800F7BC4(1) & 0xF;
    if (buttons != 0) {
        pushed = 1;
    }

    axis = (s8)fn_800F7A7C(1, 0);
    if (axis > 0) {
        axis = (s8)fn_800F7A7C(1, 0);
    } else {
        axis = -(s8)fn_800F7A7C(1, 0);
    }
    if (axis > 2) {
        pushed = 1;
    }

    axis = (s8)fn_800F7A08(1, 0);
    if (axis > 0) {
        axis = (s8)fn_800F7A08(1, 0);
    } else {
        axis = -(s8)fn_800F7A08(1, 0);
    }
    if (axis > 2) {
        pushed = 1;
    }

    if ((fn_800F7BC4(1) & 0x1F70) != 0) {
        pushed = 1;
    }

#pragma scheduling on
#pragma scheduling on
    return pushed;
}
#pragma pop

#pragma push
#pragma scheduling off
#pragma peephole off
s32 scriptIsMoveButtonPush(void)
{
    s32 pushed = 0;
    s32 axis;
    s32 buttons;

    buttons = fn_800F7BC4(1) & 0xF;
    if (buttons != 0) {
        pushed = 1;
    }

    axis = (s8)fn_800F7A7C(1, 0);
    if (axis > 0) {
        axis = (s8)fn_800F7A7C(1, 0);
    } else {
        axis = -(s8)fn_800F7A7C(1, 0);
    }
    if (axis > 2) {
        pushed = 1;
    }

    axis = (s8)fn_800F7A08(1, 0);
    if (axis > 0) {
        axis = (s8)fn_800F7A08(1, 0);
    } else {
        axis = -(s8)fn_800F7A08(1, 0);
    }
    if (axis > 2) {
        pushed = 1;
    }

    return pushed;
}
#pragma pop

#pragma push
#pragma scheduling off
void fn_801CB394(s32 id)
{
    fn_800F7068(id, 0);
}

void fn_801CB3B8(s32 id)
{
    fn_800F7068(id, 1);
}

void fn_801CB3DC(s32 id)
{
    fn_800F7274(id);
}
#pragma pop

s32 scriptExecTask(void* callback, u32 priority, u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
    s32 task;

    if (GSthreadGetCurrentThread() != NULL) {
        task = fn_800F036C();
    } else {
        GSlogWrite(lbl_802758C8, lbl_8036DCA8);
        task = 0x7F;
    }

    if ((u8)priority > 7) {
        priority = 7;
    }

    return fn_800F7318(task + priority, callback, 0x1000, 1, 0, 4, arg2, arg3, arg4, arg5);
}

void fn_801CB4A8(void* callback, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    fn_800F7434(callback, 4, arg1, arg2, arg3, arg4);
}

#pragma push
#pragma scheduling off
void fn_801CB4E8(u32 resource, u32 arg)
{
    void* object = GSresGetResource(fn_80113F48(), resource);

    fn_80118874(object, arg);
}

void fn_801CB530(u32 model_id, u32 particle_bank_id)
{
    void* model = GSresGetResource(fn_80113F48(), model_id);
    void* particle_bank = GSresGetResource(fn_80113F48(), particle_bank_id);

    GSmodelLinkToGSparticleBank(model, particle_bank);
    GSmodelSetGSparticleLinkAttachMode(model, 4);
}
#pragma pop

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

s32 fn_801CB61C(u32 resource, u32 part_resource, s32 part)
{
    u32 group = fn_80113F48();
    void* model;
    void* part_model;

    if (fn_8018D998(group, resource) != NULL) {
        fn_801845E4(group, resource, group, part_resource, part);
    } else {
        model = GSresGetResource(group, resource);
        if (model == NULL) {
            return 0;
        }

        part_model = GSresGetResource(group, part_resource);
        if (part_model == NULL) {
            return 0;
        }

        part_model = GSmodelGetPart(part_model, part);
        GSmodelAttachToGSpart(model, part_model, 7, 0, 1);
        GSpartFree(part_model);
    }

    return 1;
}

#pragma push
#pragma peephole off
s32 scriptWaitSyncMotion(u32 resource, s32 wait)
{
    u32 group = fn_80113F48();
    void* model;

    if (fn_8018D998(group, resource) != NULL) {
        return (u8)peopleWaitSyncMotion(group, resource, wait);
    }

    model = GSresGetResource(group, resource);
    if (model == NULL) {
        return 0;
    }

    while (1) {
        if ((u8)GSmodelHasAnimationEnded(model) != 0) {
            return 0;
        }
        if (wait != 0) {
            _threadSwitch();
            continue;
        }
        return 1;
    }
}
#pragma pop

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

#pragma push
#pragma peephole off
void fn_801CB834(u32 resource, u32 anim_index, s32 frame, s32 loop)
{
    u32 group = fn_80113F48();
    void* model;

    if (fn_8018D998(group, resource) != NULL) {
        fn_8018B368(group, resource, anim_index, frame, (u8)loop);
        return;
    }

    model = GSresGetResource(group, resource);
    if (model != NULL) {
        GSmodelSetAnimIndex(model, anim_index);
        GSmodelSetAnimFrame(model, (f32)frame);
        GSmodelSetAnimRate(model, 0.5f);
        GSmodelSetTexAnimIndex(model, anim_index);
        GSmodelSetTexAnimFrame(model, (f32)frame);
        GSmodelSetTexAnimRate(model, 0.5f);
        if (loop != 0) {
            GSmodelSetAnimType(model, 1);
        } else {
            GSmodelSetAnimType(model, 0);
        }
        GSmodelStartAnimation(model);
    }
}
#pragma pop

void fn_801CB954(u32 resource, s32 visible)
{
    u32 group = fn_80113F48();
    void* model;

    if (fn_8018D998(group, resource) != NULL) {
        fn_8018C1E8(group, resource, (u8)visible);
    } else {
        model = GSresGetResource(group, resource);
        if (model != NULL) {
            GSmodelSetVisibility(model, (u8)visible);
        }
    }
}

void fn_801CB9D8(u32 resource)
{
    fn_8018DB68(fn_80113F48(), resource);
}

#pragma push
#pragma scheduling off
s32 fn_801CBA0C(void* param)
{
    s32 people_id;
    u32 raw_id;
    void* people;

    raw_id = lbl_8047B3C8;
    lbl_8047B3C8 = raw_id + 1;
    people_id = (s8)raw_id;
    people_id |= 0x7FFE0000;

    people = fn_8018E050(fn_80113F48(), people_id, param);
    if (people == NULL) {
#pragma scheduling on
        return 0;
    }

    GSmodelSetBoundCheck(peopleGetModel(peopleSearchID(people)), 0);
#pragma scheduling on
    return people_id;
}
#pragma pop

void fn_801CBA84(void)
{
    lbl_8047B3C8 = 0;
}

#pragma push
#pragma scheduling off
s32 fn_801CBA90(void)
{
    fn_800FF58C(0x395);
    return 0;
}
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_801CBAB8(void)
{
    s32 done;
    s32 state;
    s32 result;
    s32 input;

    result = 0;
    state = result;
    done = result;

    while (done == 0) {
        switch (state) {
        case 0:
            winMsgOpen(2, 0x3C46, 1, 1);
            input = (s8)fn_8001E184();
            winMsgClose(1);
            if (input != 0) {
                done = 1;
            } else {
                state = 2;
            }
            break;
        case 2:
            if (fn_800889E4(1) == 0) {
                state = 3;
                result = 1;
            } else {
                state = 4;
            }
            break;
        case 3:
            fn_800FF58C(0x395);
            state = 4;
            break;
        case 4:
            done = 1;
            break;
        }
    }

#pragma scheduling on
    return result;
}
#pragma pop

void fn_801D0080(void)
{
    lbl_8047B3D4->callback_finished = 1;
}

void* fn_801D0314(void* ptr)
{
    void* aligned = fn_800E202C(ptr);

    fn_800E24B0();
    if (fn_800E209C(aligned) == 0) {
        return NULL;
    }
    return ptr;
}

void fn_801D036C(void)
{
    fn_800E2C04(0x1DFD0, 0x20);
    fn_800E27B0();
}

void fn_801D039C(void)
{
    MemcardTaskState* task = lbl_8047B3D4;
    void* work;
    void* aligned;

    if (task->task_kind != 0) {
        if (task->task_kind != 8) {
            winMsgClose(1);
        }

        GSgappTerminate(task->gapp);

        work = task->card_work_area;
        aligned = fn_800E202C(work);
        fn_800E24B0();
        if (fn_800E209C(aligned) == 0) {
            work = NULL;
        }
        lbl_8047B3D4->card_work_area = work;

        work = lbl_8047B3D4->work_buffer;
        aligned = fn_800E202C(work);
        fn_800E24B0();
        if (fn_800E209C(aligned) == 0) {
            work = NULL;
        }
        lbl_8047B3D4->work_buffer = work;
    }

    lbl_8047B3D4->task_kind = 0;
}

void fn_801D046C(u8 resume)
{
    if (lbl_8047B3D4->state == 0x31 && resume != 0) {
        lbl_8047B3D4->state = lbl_8047B3D4->resume_state;
        lbl_8047B3D4->random_delay = _fadeEffectGetRandom__FUl(0x3C);
    } else {
        lbl_8047B3D4->error_code = 0xF;
        lbl_8047B3D4->state = 0x2B;
    }
}

u8 fn_801D04D0(void)
{
    return lbl_8047B3D4->state == 0x31;
}

u8 fn_801D04E8(void)
{
    return lbl_8047B3D4->serial_check_enabled;
}

s32 memcardGetTaskResult(void)
{
    if (lbl_8047B3D4->state == 0x30) {
        lbl_8047B3D4->error_code = (u8)fn_801D0090(lbl_8047B3D4->error_code);
        lbl_8047B3D4->state = lbl_8047B3D4->resume_state;
    }

    if (lbl_8047B3D4->state != 0x32) {
        return 0;
    }
    return lbl_8047B3D4->task_result;
}

void fn_801D0A30(void)
{
    u32 offset;

    if (lbl_8047B3D0 == 0) {
        CARDInit();
        offset = _fadeEffectGetRandom__FUl(0x100) & ~7;
        lbl_8047B3D4 = (MemcardTaskState*)&lbl_80467168[offset];
        lbl_8047B3D4->task_kind = 0;
        lbl_8047B3D4->work_buffer = NULL;
        lbl_8047B3D4->card_work_area = NULL;
        lbl_8047B3D4->field_2c = 0;
        lbl_8047B3D0 = 1;
    }
}
