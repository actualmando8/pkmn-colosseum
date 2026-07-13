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
extern u32 lbl_804670E8[];
extern const u8 lbl_802758C8[];
extern const u8 lbl_8036DCA8[];

extern void* memcpy(void* dst, const void* src, u32 size);

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
  ;
  if ((fn_800F7BC4(1) & 0xF) != 0)
  {
    pushed = 1;
  }
  axis = (s8) fn_800F7A7C(1, 0);
  if (axis > (1 * 0))
  {
    axis = (s8) fn_800F7A7C(1, 0);
  }
  else
  {
    axis = -((s8) fn_800F7A7C(1, 0));
  }
  if (axis > 2)
  {
    pushed = 1;
  }
  axis = (s8) fn_800F7A08(1, 0);
  if (axis > 0)
  {
    axis = (s8) fn_800F7A08(1, 0);
  }
  else
  {
    axis = -((s8) fn_800F7A08(1, 0));
  }
  if (axis > 2)
  {
    pushed = 1;
  }
  if ((fn_800F7BC4(1) & 0x1F70) != 0)
  {
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

    if ((fn_800F7BC4(1) & 0xF) != 0) {
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

#pragma scheduling on
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

    task += priority;

    return fn_800F7318(task, callback, 0x1000, 1, 0, 4, arg2, arg3, arg4, arg5);
}

#pragma push
#pragma peephole off
void fn_801CB4A8(void* callback, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    fn_800F7434(callback, 4, arg1, arg2, arg3, arg4);
}
#pragma pop

#pragma push
#pragma peephole off
void fn_801CB4E8(u32 resource, u32 arg)
{
    void* object = GSresGetResource(fn_80113F48(), resource);

    fn_80118874(object, arg);
}
#pragma pop

#pragma push
#pragma peephole off
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

#pragma push
#pragma scheduling off
#pragma peephole off
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
#pragma pop

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

#pragma push
#pragma peephole off
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
#pragma pop

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
    people_id = (s8)raw_id | 0x7FFE0000;

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

    done = state = result = 0;

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

    return result;
}
#pragma scheduling on
#pragma pop

/* SHA-1's 16-word circular message schedule. */
#define SHA1_ROTL(value, bits) \
    (((value) << (bits)) | ((value) >> (32 - (bits))))
#define SHA1_BLK0(i) (block[(i)])
#define SHA1_BLK(i) \
    (block[(i) & 15] = SHA1_ROTL(block[((i) + 13) & 15] ^ \
                                      block[((i) + 8) & 15] ^ \
                                      block[((i) + 2) & 15] ^ \
                                      block[(i) & 15], 1))
#define SHA1_R0(v, w, x, y, z, i) \
    z += ((w & (x ^ y)) ^ y) + SHA1_BLK0(i) + 0x5A827999 + SHA1_ROTL(v, 5); \
    w = SHA1_ROTL(w, 30)
#define SHA1_R1(v, w, x, y, z, i) \
    z += ((w & (x ^ y)) ^ y) + SHA1_BLK(i) + 0x5A827999 + SHA1_ROTL(v, 5); \
    w = SHA1_ROTL(w, 30)
#define SHA1_R2(v, w, x, y, z, i) \
    z += (w ^ x ^ y) + SHA1_BLK(i) + 0x6ED9EBA1 + SHA1_ROTL(v, 5); \
    w = SHA1_ROTL(w, 30)
#define SHA1_R3(v, w, x, y, z, i) \
    z += (((w | x) & y) | (w & x)) + SHA1_BLK(i) + 0x8F1BBCDC + \
         SHA1_ROTL(v, 5); \
    w = SHA1_ROTL(w, 30)
#define SHA1_R4(v, w, x, y, z, i) \
    z += (w ^ x ^ y) + SHA1_BLK(i) + 0xCA62C1D6 + SHA1_ROTL(v, 5); \
    w = SHA1_ROTL(w, 30)

void fn_801CC380(u32 state[5], const u8 input[64])
{
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u32* block = lbl_804670E8;

    memcpy(block, input, 64);

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    SHA1_R0(a, b, c, d, e, 0);
    SHA1_R0(e, a, b, c, d, 1);
    SHA1_R0(d, e, a, b, c, 2);
    SHA1_R0(c, d, e, a, b, 3);
    SHA1_R0(b, c, d, e, a, 4);
    SHA1_R0(a, b, c, d, e, 5);
    SHA1_R0(e, a, b, c, d, 6);
    SHA1_R0(d, e, a, b, c, 7);
    SHA1_R0(c, d, e, a, b, 8);
    SHA1_R0(b, c, d, e, a, 9);
    SHA1_R0(a, b, c, d, e, 10);
    SHA1_R0(e, a, b, c, d, 11);
    SHA1_R0(d, e, a, b, c, 12);
    SHA1_R0(c, d, e, a, b, 13);
    SHA1_R0(b, c, d, e, a, 14);
    SHA1_R0(a, b, c, d, e, 15);
    SHA1_R1(e, a, b, c, d, 16);
    SHA1_R1(d, e, a, b, c, 17);
    SHA1_R1(c, d, e, a, b, 18);
    SHA1_R1(b, c, d, e, a, 19);
    SHA1_R2(a, b, c, d, e, 20);
    SHA1_R2(e, a, b, c, d, 21);
    SHA1_R2(d, e, a, b, c, 22);
    SHA1_R2(c, d, e, a, b, 23);
    SHA1_R2(b, c, d, e, a, 24);
    SHA1_R2(a, b, c, d, e, 25);
    SHA1_R2(e, a, b, c, d, 26);
    SHA1_R2(d, e, a, b, c, 27);
    SHA1_R2(c, d, e, a, b, 28);
    SHA1_R2(b, c, d, e, a, 29);
    SHA1_R2(a, b, c, d, e, 30);
    SHA1_R2(e, a, b, c, d, 31);
    SHA1_R2(d, e, a, b, c, 32);
    SHA1_R2(c, d, e, a, b, 33);
    SHA1_R2(b, c, d, e, a, 34);
    SHA1_R2(a, b, c, d, e, 35);
    SHA1_R2(e, a, b, c, d, 36);
    SHA1_R2(d, e, a, b, c, 37);
    SHA1_R2(c, d, e, a, b, 38);
    SHA1_R2(b, c, d, e, a, 39);
    SHA1_R3(a, b, c, d, e, 40);
    SHA1_R3(e, a, b, c, d, 41);
    SHA1_R3(d, e, a, b, c, 42);
    SHA1_R3(c, d, e, a, b, 43);
    SHA1_R3(b, c, d, e, a, 44);
    SHA1_R3(a, b, c, d, e, 45);
    SHA1_R3(e, a, b, c, d, 46);
    SHA1_R3(d, e, a, b, c, 47);
    SHA1_R3(c, d, e, a, b, 48);
    SHA1_R3(b, c, d, e, a, 49);
    SHA1_R3(a, b, c, d, e, 50);
    SHA1_R3(e, a, b, c, d, 51);
    SHA1_R3(d, e, a, b, c, 52);
    SHA1_R3(c, d, e, a, b, 53);
    SHA1_R3(b, c, d, e, a, 54);
    SHA1_R3(a, b, c, d, e, 55);
    SHA1_R3(e, a, b, c, d, 56);
    SHA1_R3(d, e, a, b, c, 57);
    SHA1_R3(c, d, e, a, b, 58);
    SHA1_R3(b, c, d, e, a, 59);
    SHA1_R4(a, b, c, d, e, 60);
    SHA1_R4(e, a, b, c, d, 61);
    SHA1_R4(d, e, a, b, c, 62);
    SHA1_R4(c, d, e, a, b, 63);
    SHA1_R4(b, c, d, e, a, 64);
    SHA1_R4(a, b, c, d, e, 65);
    SHA1_R4(e, a, b, c, d, 66);
    SHA1_R4(d, e, a, b, c, 67);
    SHA1_R4(c, d, e, a, b, 68);
    SHA1_R4(b, c, d, e, a, 69);
    SHA1_R4(a, b, c, d, e, 70);
    SHA1_R4(e, a, b, c, d, 71);
    SHA1_R4(d, e, a, b, c, 72);
    SHA1_R4(c, d, e, a, b, 73);
    SHA1_R4(b, c, d, e, a, 74);
    SHA1_R4(a, b, c, d, e, 75);
    SHA1_R4(e, a, b, c, d, 76);
    SHA1_R4(d, e, a, b, c, 77);
    SHA1_R4(c, d, e, a, b, 78);
    SHA1_R4(b, c, d, e, a, 79);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

#undef SHA1_R4
#undef SHA1_R3
#undef SHA1_R2
#undef SHA1_R1
#undef SHA1_R0
#undef SHA1_BLK
#undef SHA1_BLK0
#undef SHA1_ROTL

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
    if (lbl_8047B3D4->task_kind != 0) {
        if (((volatile MemcardTaskState*)lbl_8047B3D4)->task_kind != 8) {
            winMsgClose(1);
        }

        GSgappTerminate(lbl_8047B3D4->gapp);

        lbl_8047B3D4->card_work_area = fn_801D0314(lbl_8047B3D4->card_work_area);
        lbl_8047B3D4->work_buffer = fn_801D0314(lbl_8047B3D4->work_buffer);
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
