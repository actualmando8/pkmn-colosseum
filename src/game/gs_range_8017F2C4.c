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
    void* field_28;
    s32 field_2C;
    u32 field_30;
    void* field_34;
    void (*callback)(void* arg0, void* arg1);
    void* field_3C;
} GsRangeRequest;

extern void* GSresGetResource(u32 group, u32 handle);
extern void* GSresAllocResourceAlign(u32 size, u32 alignment, u32 loadParam,
                                      u32 loadParam2, void* callback);

void* fn_8017F3F8(u32 group, u32 handle, u32 size)
{
    u32 alignedSize = (size + 0x1F) & ~0x1F;
    void* volatile old = GSresGetResource(group, handle);
    void* buf = GSresAllocResourceAlign(alignedSize, 0x20, group, handle, NULL);
    if (buf) {
        return buf;
    }
    return NULL;
}

typedef struct GsRangeResource {
    u8 _pad_0[0x38];
    void (*release)(struct GsRangeResource* self);
} GsRangeResource;

extern void fn_8009EFE4(void* res);

s32 fn_8017F6B4(void* unused, u32 group, u32 handle)
{
    GsRangeResource* res = (GsRangeResource*)GSresGetResource(group, handle);
    if (res->release != NULL) {
        res->release(res);
    }
    fn_8009EFE4(res);
    return 1;
}

typedef struct GsRangeNode {
    s32 field_0;
    u8 _pad_4[4];
    struct GsRangeNode* next;
    s32 value;
    u32 key0;
    u32 key1;
    u32 key2;
} GsRangeNode;

extern GsRangeNode* lbl_80454038[];

s32 fn_8017F728(u32 a, u32 b, u32 c)
{
    GsRangeNode* node = lbl_80454038[0];

    while (node != NULL) {
        if (node->key0 == a && node->key1 == b && node->key2 == c) {
            return node->value;
        }
        node = node->next;
    }
    return 0;
}

s32 fn_8017F794(u32 a, u32 b, u32 c)
{
    GsRangeNode* node = lbl_80454038[0];

    while (node != NULL) {
        if (node->key0 == a && node->key1 == b && node->key2 == c) {
            return node->field_0;
        }
        node = node->next;
    }
    return 0;
}

typedef struct GsRangeMemNode {
    struct GsRangeMemNode* next;
    s32 size;
} GsRangeMemNode;

typedef struct GsRangeStats {
    u8 _pad_0[0x24];
    s32 totalBase;
} GsRangeStats;

extern GsRangeMemNode* lbl_8047B1D0;
extern GsRangeStats lbl_80455048;

s32 fn_8017FA5C(void)
{
    GsRangeMemNode* head = lbl_8047B1D0;
    GsRangeMemNode* node;
    s32 sum = 0;
    volatile s32 count;

    if (head == NULL) {
        return lbl_80455048.totalBase;
    }

    count = 0;
    node = head;
    for (;;) {
        count++;
        if ((u32)node > 0x80000000u) {
            return sum;
        }
        if (node != NULL) {
            sum += node->size;
        }
        if (node == lbl_8047B1D0) {
            break;
        }
        node = node->next;
    }
    return sum + lbl_80455048.totalBase;
}

typedef struct GsRangeDVDQueueEntry {
    u8 _pad00[0x20];
    u32 state;
    s32 mode;
    void* srcPtr;
    void* dstPtr;
    u32 size;
    u32 flag34;
    void (*callback)(void* entry);
    u32 callbackArg;
    u32 index;
} GsRangeDVDQueueEntry;

extern u32 lbl_8047B1D4;
extern u32 lbl_8047B1D8;
extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 level);
extern void fn_800AE630(void* request, void* owner, u32 direction, u32 offset,
                        void* callback, void* callbackArg, void* src,
                        void* dst, u32 size);
extern void DCFlushRange(void* addr, u32 nBytes);
void fn_801808E4(volatile GsRangeRequest* req);

void* fn_801807A8(void* src, void* dst, u32 size)
{
    GsRangeDVDQueueEntry* entry;
    GsRangeDVDQueueEntry* result;
    u32 i;
    u32 alignedSize;
    u32 savedIntr;
    u32 count;

    if (size == 0) {
        return NULL;
    }

    alignedSize = (size + 0x1F) & ~0x1F;
    entry = (GsRangeDVDQueueEntry*)lbl_8047B1D4;
    count = lbl_8047B1D8;
    result = NULL;
    for (i = 0; i < count; i++) {
        if (entry->state == 0) {
            entry->state = 1;
            result = entry;
            break;
        }
        entry++;
    }

    entry = result;
    savedIntr = OSDisableInterrupts();
    entry->flag34 = 0;
    entry->mode = 1;
    entry->callback = NULL;
    entry->callbackArg = 0;
    entry->srcPtr = src;
    entry->dstPtr = dst;
    entry->size = alignedSize;
    DCFlushRange(src, alignedSize);
    fn_800AE630(entry, entry, 0, 0, fn_801808E4, entry, src, dst,
                alignedSize);
    OSRestoreInterrupts(savedIntr);
    return entry;
}

void fn_801808E4(volatile GsRangeRequest* req)
{
    void (*cb)(void*, void*);

    req->field_24 = 0;
    if (req->callback != NULL) {
        cb = req->callback;
        cb((void*)req->field_34, (void*)req->field_3C);
    }
    req->field_20 = 0;
    DCFlushRange((void*)req->field_28, req->field_30);
}

typedef struct GsRangePoolElem {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    u8 _pad_14[4];
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    u8 _pad_24[4];
    s32 field_28;
    u8 _pad_2C[0x14];
} GsRangePoolElem;

typedef struct GsRangePoolInfo {
    s32 count;
    GsRangePoolElem* base;
} GsRangePoolInfo;

extern GsRangePoolInfo lbl_8047B1E8;
extern s32 lbl_8047B1E0;
extern s32 lbl_8047B1E4;

extern u16 fn_800E2C04(u32 size, u32 align);
extern void* fn_800E27B0(u16 handle);

#pragma optimize_for_size on
void fn_80180B94(s32 count)
{
    s32 size = count * 0x40;
    u32 alignedSize = (size + 0x1F) & ~0x1F;
    u16 handle;
    GsRangePoolElem* elem;
    s32 i;

    lbl_8047B1E8.count = count;
    handle = fn_800E2C04(alignedSize, 0x20);
    if (handle != 0) {
        lbl_8047B1E8.base = fn_800E27B0(handle);
    } else {
        lbl_8047B1E8.base = NULL;
    }
    lbl_8047B1E0 = 0;
    lbl_8047B1E4 = 0;

    elem = lbl_8047B1E8.base;
    for (i = 0; i < count; i++) {
        elem->field_0 = 0;
        elem->field_4 = 0;
        elem->field_8 = 0;
        elem->field_C = 0;
        elem->field_10 = 0;
        elem->field_18 = 0;
        elem->field_1C = 0;
        elem->field_20 = 0;
        elem->field_28 = 0;
        elem++;
    }
}
#pragma optimize_for_size reset

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
