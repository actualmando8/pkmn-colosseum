#ifndef DOLPHIN_OS_OSTHREAD_H
#define DOLPHIN_OS_OSTHREAD_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

typedef struct OSThread OSThread;
typedef struct OSThreadQueue OSThreadQueue;
typedef struct OSThreadLink OSThreadLink;
typedef struct OSMutex OSMutex;
typedef struct OSMutexQueue OSMutexQueue;
typedef struct OSMutexLink OSMutexLink;

typedef s32 (*OSSwitchThreadCallback)(OSThread* from, OSThread* to);

struct OSThreadQueue {
    OSThread* head;
    OSThread* tail;
};

struct OSThreadLink {
    OSThread* next;
    OSThread* prev;
};

struct OSMutexQueue {
    OSMutex* head;
    OSMutex* tail;
};

struct OSMutexLink {
    OSMutex* next;
    OSMutex* prev;
};

struct OSThread {
    /* 0x000 */ OSContext context;
    /* 0x2C8 */ u16 state;
    /* 0x2CA */ u16 attr;
    /* 0x2CC */ s32 suspend;
    /* 0x2D0 */ s32 priority;
    /* 0x2D4 */ s32 base;
    /* 0x2D8 */ u32 val;
    /* 0x2DC */ OSThreadQueue* queue;
    /* 0x2E0 */ OSThreadLink link;
    /* 0x2E8 */ OSThreadQueue queueJoin;
    /* 0x2F0 */ OSMutex* mutex;
    /* 0x2F4 */ OSMutexQueue queueMutex;
    /* 0x2FC */ OSThreadLink linkActive;
    /* 0x304 */ u32* stackBase;
    /* 0x308 */ u32* stackEnd;
};

void __OSThreadInit(void);
void OSInitThreadQueue(OSThreadQueue* queue);
s32  OSDisableScheduler(void);
s32  OSEnableScheduler(void);
void __OSReschedule(void);
void OSCancelThread(OSThread* thread);
void OSClearStack(u8 val);
OSThread* OSGetCurrentThread(void);
BOOL OSIsThreadTerminated(OSThread* thread);

#endif /* DOLPHIN_OS_OSTHREAD_H */
