#ifndef DOLPHIN_OS_OS_H
#define DOLPHIN_OS_OS_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

/* Boot info at 0x80000000 */
typedef struct OSBootInfo {
    u8  diskID[0x20];          /* 0x00 */
    u32 magic;                 /* 0x20 */
    u32 version;               /* 0x24 */
    u32 memorySize;            /* 0x28 */
    u32 consoleType;           /* 0x2C */
    u32 arenaLo;               /* 0x30 */
    u32 arenaHi;               /* 0x34 */
    u32 FSTLocation;           /* 0x38 */
    u32 FSTMaxLength;          /* 0x3C */
} OSBootInfo;

/* Exception types */
#define OS_EXCEPTION_SYSTEM_RESET     0
#define OS_EXCEPTION_MACHINE_CHECK    1
#define OS_EXCEPTION_DSI              2
#define OS_EXCEPTION_ISI              3
#define OS_EXCEPTION_EXTERNAL_INTERRUPT 4
#define OS_EXCEPTION_ALIGNMENT        5
#define OS_EXCEPTION_PROGRAM          6
#define OS_EXCEPTION_FLOATING_POINT   7
#define OS_EXCEPTION_DECREMENTER      8
#define OS_EXCEPTION_SYSTEM_CALL      9
#define OS_EXCEPTION_TRACE            10
#define OS_EXCEPTION_PERFORMANCE_MONITOR 11
#define OS_EXCEPTION_BREAKPOINT       12
#define OS_EXCEPTION_SYSTEM_INTERRUPT 13
#define OS_EXCEPTION_THERMAL_INTERRUPT 14
#define OS_EXCEPTION_MAX              15

typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

/* Error types for OSSetErrorHandler */
#define OS_ERROR_DSI                 0
#define OS_ERROR_ISI                 1
#define OS_ERROR_ALIGNMENT           2
#define OS_ERROR_PROGRAM             3
#define OS_ERROR_EXTERNAL_INTERRUPT  4
#define OS_ERROR_MACHINE_CHECK       5
#define OS_ERROR_FLOATING_POINT      6
#define OS_ERROR_DECREMENTER         7
#define OS_ERROR_SYSTEM_CALL         8
#define OS_ERROR_TRACE               9
#define OS_ERROR_PERFORMANCE_MONITOR 10
#define OS_ERROR_BREAKPOINT          11
#define OS_ERROR_SYSTEM_INTERRUPT    12
#define OS_ERROR_THERMAL_INTERRUPT   13
#define OS_ERROR_PROTECTION          15
#define OS_ERROR_FPE                 16
#define OS_ERROR_MAX                 17

typedef void (*OSErrorHandler)(u16 error, OSContext* context, ...);

void OSInit(void);
void OSReport(const char* fmt, ...);
void OSRegisterVersion(const char* id);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void  OSSetArenaHi(void* addr);
void  OSSetArenaLo(void* addr);

u32 OSGetResetCode(void);

__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
void OSDefaultExceptionHandler(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

OSErrorHandler OSSetErrorHandler(u16 error, OSErrorHandler handler);
void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

/* Global variables */
extern u32 __OSInIPL;
extern u32 __OSIsGcam;
extern s64 __OSStartTime;

#endif /* DOLPHIN_OS_OS_H */
