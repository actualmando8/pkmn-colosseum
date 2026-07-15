/**
 * @file gs_range_8017F2C4.c
 * @brief gs-engine code, 0x8017F2C4 - 0x80180C78 (21 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct GsRangeRequest {
    u8 _pad_0[0x20];
    s32 field_20;
    s32 field_24;
    void* flushStart;
    u32 _pad_2C;
    u32 flushSize;
    u32 callbackArg;
    void (*callback)(u32 arg, u32 userData);
    u32 userData;
} GsRangeRequest;

typedef struct GsRangeCacheNode {
    void* data;
    u32 _pad_04;
    struct GsRangeCacheNode* next;
    u32 field_0C;
    u32 fileHandle;
    u32 groupID;
    u32 nameHash;
} GsRangeCacheNode;

typedef struct GsRangePeopleWork {
    u32 state;
    void* self;
    void* model;
    u32 subState;
    void (*callback)(void);
    u32 _pad_14;
    void* thread;
    void* next;
    void* parent;
    u32 _pad_24;
    void* extra;
    u8 _pad_2C[0x14];
} GsRangePeopleWork;

void* fn_8017F3F8(void* group, void* model, volatile u32 size)
{
    extern void* GSresGetResource(void* group);
    extern void* GSresAllocResourceAlign(u32 size, u32 alignment, void* group,
                                         void* model, u32 flags);
    void* volatile resource;
    register void* volatile result;
    u32 alignedSize;

    alignedSize = (size + 0x1F) & ~0x1F;
    resource = GSresGetResource(group);
    if ((result = GSresAllocResourceAlign(alignedSize, 0x20, group, model, 0)) == NULL) {
        return NULL;
    }
    return result;
}

#pragma push
#pragma optimization_level 0
void* fn_8017F6B4(u32 unusedArg, u32 group, u32 model)
{
    extern void* GSresGetResource(u32 group, u32 model);
    extern void fn_8009EFE4(void* resource);
    void* block;
    void* resource;
    u32 unused = 0;

    block = GSresGetResource(group, model);
    resource = block;
    if (*(void (**)(void))((u8*)resource + 0x38)) {
        (*(void (**)(void))((u8*)resource + 0x38))();
    }
    fn_8009EFE4(resource);
    return (void*)1;
}
#pragma pop

#pragma push
#pragma optimization_level 0
u32 fn_8017F728(u32 fileHandle, u32 groupID, u32 nameHash)
{
    extern GsRangeCacheNode* lbl_80454038[];
    GsRangeCacheNode* node;
    u32 count;

    node = lbl_80454038[0];
    count = 0;
    while (node) {
        if (node->fileHandle == fileHandle && node->groupID == groupID &&
            node->nameHash == nameHash) {
            return node->field_0C;
        }
        node = node->next;
        count++;
    }
    return 0;
}
#pragma pop

#pragma push
#pragma optimization_level 0
void* fn_8017F794(u32 fileHandle, u32 groupID, u32 nameHash)
{
    extern GsRangeCacheNode* lbl_80454038[];
    GsRangeCacheNode* node;
    u32 count;

    node = lbl_80454038[0];
    count = 0;
    while (node) {
        if (node->fileHandle == fileHandle && node->groupID == groupID &&
            node->nameHash == nameHash) {
            return node->data;
        }
        node = node->next;
        count++;
    }
    return NULL;
}
#pragma pop

#pragma push
#pragma optimization_level 0
u32 fn_8017FA5C(void)
{
    extern u32 lbl_8047B1D0;
    extern u8 lbl_80455048[];
    u32 accum;
    register u32* list;
    u32* node;
    u32 count;

    accum = 0;
    count = 0;
    list = (u32*)lbl_8047B1D0;
    if (lbl_8047B1D0 == 0) {
        accum = *(u32*)(lbl_80455048 + 0x24);
    } else {
        count = 0;
        node = (u32*)list[0];
        while (1) {
            count++;
            if ((u32)node <= 0x80000000u) {
                return accum;
            }
            if (node) {
                accum += node[1];
            }
            if (node != (u32*)lbl_8047B1D0) {
                node = (u32*)node[0];
                continue;
            }
            accum += *(u32*)(lbl_80455048 + 0x24);
            break;
        }
    }
    return accum;
}
#pragma pop

#pragma optimize_for_size on
s32 fn_801808B4(volatile GsRangeRequest* req)
{
    volatile GsRangeRequest* ptr = req;
    s32 out;

    if (ptr->field_24 != 1) {
        ptr->field_20 = 0;
    }
    out = ptr->field_20;
    return out;
}
#pragma optimize_for_size reset

void fn_801808E4(GsRangeRequest* req)
{
    extern void DCFlushRange(void* addr, u32 size);
    GsRangeRequest* volatile savedReq = req;
    volatile GsRangeRequest* ptr = savedReq;
    void (*callback)(u32 arg, u32 userData);

    ptr->field_24 = 0;
    if (ptr->callback != NULL) {
        callback = ptr->callback;
        callback(ptr->callbackArg, ptr->userData);
    }
    ptr->field_20 = 0;
    DCFlushRange(ptr->flushStart, ptr->flushSize);
}

#pragma push
#pragma optimization_level 0
void fn_80180B94(s32 count)
{
    extern u16 fn_800E2C04(u32 size, u32 alignment);
    extern void* fn_800E27B0(u16 handle);
    extern u32 lbl_8047B1E8[2];
    extern void* lbl_8047B1E0;
    extern void* lbl_8047B1E4;
    GsRangePeopleWork* allocated;
    GsRangePeopleWork* work;
    u16 handle;
    u32 size;
    s32 i;

    size = (count * sizeof(GsRangePeopleWork) + 0x1F) & ~0x1F;
    lbl_8047B1E8[0] = count;
    handle = fn_800E2C04(size, 0x20);
    if (handle != 0) {
        allocated = fn_800E27B0(handle);
    } else {
        allocated = NULL;
    }
    lbl_8047B1E8[1] = (u32)allocated;
    lbl_8047B1E0 = NULL;
    work = (GsRangePeopleWork*)lbl_8047B1E8[1];
    lbl_8047B1E4 = NULL;

    for (i = 0; i < count; i++, work++) {
        work->state = 0;
        work->self = NULL;
        work->model = NULL;
        work->next = NULL;
        work->subState = 0;
        work->parent = NULL;
        work->extra = NULL;
        work->thread = NULL;
        work->callback = NULL;
    }
}
#pragma pop
