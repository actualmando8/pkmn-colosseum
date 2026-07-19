/**
 * @file TRKNub_exact_800BE47C.c
 * @brief Exact pure-C MetroTRK event queue and nub helpers.
 */
#include "dolphin/types.h"

typedef struct TRKEvent {
    s32 type;
    s32 id;
    s32 bufferIndex;
} TRKEvent;

typedef struct TRKEventQueue {
    s32 mutex;
    s32 count;
    s32 next;
    TRKEvent events[2];
    u32 eventID;
} TRKEventQueue;

extern TRKEventQueue lbl_803FCDD8;
extern void fn_80003488(void* dst, const void* src, u32 size);
extern void fn_800BF080(void);
extern void fn_800C0CC0(void* mutex);
extern void fn_800C0CC8(void* mutex);
extern void fn_800C0CD0(void* mutex);
extern void TRK_board_display(const char* msg);

s32 TRKPostEvent(TRKEvent* event)
{
    s32 result = 0;
    TRKEventQueue* queue;

    fn_800C0CC8(&lbl_803FCDD8.mutex);
    queue = &lbl_803FCDD8;
    if (queue->count == 2) {
        result = 0x100;
    } else {
        s32 index = (queue->next + queue->count) % 2;
        u8* slot = (u8*)queue + index * (s32)sizeof(TRKEvent);
        slot += 12;
        fn_80003488(slot, event, sizeof(TRKEvent));
        lbl_803FCDD8.events[index].id = lbl_803FCDD8.eventID;
        lbl_803FCDD8.eventID++;
        if (lbl_803FCDD8.eventID < 0x100) {
            lbl_803FCDD8.eventID = 0x100;
        }
        queue->count++;
    }
    fn_800C0CC0(&lbl_803FCDD8.mutex);
    return result;
}

s32 TRKGetNextEvent(TRKEvent* event)
{
    s32 result = 0;

    fn_800C0CC8(&lbl_803FCDD8.mutex);
    if (lbl_803FCDD8.count > 0) {
        fn_80003488(event, lbl_803FCDD8.events + lbl_803FCDD8.next,
                    sizeof(TRKEvent));
        lbl_803FCDD8.count = lbl_803FCDD8.count - 1;
        lbl_803FCDD8.next = lbl_803FCDD8.next + 1;
        if (lbl_803FCDD8.next == 2) {
            lbl_803FCDD8.next = 0;
        }
        result = 1;
    }
    fn_800C0CC0(&lbl_803FCDD8.mutex);
    return result;
}

s32 TRKInitializeEventQueue(void)
{
    fn_800C0CD0(&lbl_803FCDD8.mutex);
    fn_800C0CC8(&lbl_803FCDD8.mutex);
    lbl_803FCDD8.count = 0;
    lbl_803FCDD8.next = 0;
    lbl_803FCDD8.eventID = 0x100;
    fn_800C0CC0(&lbl_803FCDD8.mutex);
    return 0;
}

void TRKNubWelcome(void)
{
    TRK_board_display("MetroTRK for GAMECUBE v2.6");
}

s32 TRKTerminateNub(void)
{
    fn_800BF080();
    return 0;
}
