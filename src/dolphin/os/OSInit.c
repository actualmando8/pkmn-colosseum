#include "dolphin/os/OS.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSMemory.h"
#include "dolphin/os/OSReset.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/PPCArch.h"
#include "dolphin/dvd/dvd.h"
#include "dolphin/exi/EXI.h"
#include "dolphin/si/SI.h"

extern void* memset(void* dest, int val, u32 n);
extern void* memcpy(void* dest, const void* src, u32 n);

/*
 * OSInit.c - Dolphin OS initialization.
 *
 * Contains OSInit, ClearArena, InquiryCallback, and OSRegisterVersion.
 * OSInit is called once at startup to initialize all OS subsystems.
 *
 * Matches: 0x800998E0 - 0x8009A27C
 */

/* Memory-mapped boot info locations */
#define OS_BOOT_INFO        ((volatile u32*)0x80000000)
#define OS_BI2_DEBUG_ADDR   (*(volatile u32*)0x800000F4)
#define OS_GAMEID_ADDR      ((volatile u8*)0x800030E6)
#define OS_DEBUG_FLAG_ADDR  (*(volatile u8*)0x800030E8)
#define OS_PADSPEC_ADDR     (*(volatile u8*)0x800030E9)
#define OS_CONSOLE_TYPE     (*(volatile u32*)0x8000002C)
#define OS_PHYSICAL_MEM     (*(volatile u32*)0x80000028)

/* External symbols from the linker */
extern u32 __ArenaLo[];
extern u32 __ArenaHi[];
extern u32 _stack_addr[];

/* String table pointer - used by OSInit for various log strings */
extern const char* __OSVersion;

/* Globals */
static BOOL  AreWeInitialized;
static u32*  BootInfo;
static u32*  BI2DebugFlag;
static u32   BI2DebugFlagHolder;
static u32   __DVDLongFileNameFlag;
static u32   __PADSpec;

u32   __OSInIPL;
u32   __OSIsGcam;
s64   __OSStartTime;

static u32*  __OSSavedRegionStart;
static u32*  __OSSavedRegionEnd;

/* Drive info buffer for DVD inquiry */
static DVDDriveInfo DriveInfo;

/* Forward declarations */
static void ClearArena(void);
static void InquiryCallback(s32 result, DVDCommandBlock* block);
extern void OSExceptionInit(void);
extern void __OSInitAudioSystem(void);
extern void __OSInitSystemCall(void);
extern void __OSInitSram(void);
extern void EnableMetroTRKInterrupts(void);

static void ClearArena(void) {
    u32 resetCode;
    void* arenaLo;
    void* arenaHi;

    resetCode = OSGetResetCode();

    if ((resetCode + 0x80000000) == 0) {
        /* Cold boot: zero-fill the entire arena */
        __OSSavedRegionStart = 0;
        __OSSavedRegionEnd   = 0;

        arenaHi = OSGetArenaHi();
        arenaLo = OSGetArenaLo();
        memset(arenaLo, 0, (u32)arenaHi - (u32)arenaLo);
    } else {
        /* Warm boot: read saved region from 0x812FDFF0/0x812FDFEC */
        u32* savedInfo = (u32*)0x812FDFF0;
        u32 regionStart = savedInfo[-1]; /* 0x812FDFEC */
        u32 regionEnd   = savedInfo[-2]; /* 0x812FDFE8 -- actually at DFF0 and DFEC */

        __OSSavedRegionStart = (u32*)*(u32*)0x812FDFF0;
        __OSSavedRegionEnd   = (u32*)*(u32*)0x812FDFEC;

        if ((u32)__OSSavedRegionStart == 0) {
            /* No saved region; zero the entire arena */
            arenaHi = OSGetArenaHi();
            arenaLo = OSGetArenaLo();
            memset(arenaLo, 0, (u32)arenaHi - (u32)arenaLo);
        } else {
            /* Zero everything except the saved region */
            arenaLo = OSGetArenaLo();

            if ((u32)arenaLo < (u32)__OSSavedRegionStart) {
                arenaHi = OSGetArenaHi();

                if ((u32)arenaHi <= (u32)__OSSavedRegionStart) {
                    /* Entire arena is below saved region */
                    memset(arenaLo, 0, (u32)arenaHi - (u32)arenaLo);
                } else {
                    /* Arena overlaps with saved region */
                    /* Zero from arenaLo to savedRegionStart */
                    memset(arenaLo, 0, (u32)__OSSavedRegionStart - (u32)arenaLo);

                    /* Zero from savedRegionEnd to arenaHi */
                    if ((u32)arenaHi > (u32)__OSSavedRegionEnd) {
                        memset(__OSSavedRegionEnd, 0,
                               (u32)arenaHi - (u32)__OSSavedRegionEnd);
                    }
                }
            }
        }
    }
}

static void InquiryCallback(s32 result, DVDCommandBlock* block) {
    if (block->state != 0) {
        /* Error: store 1 as the game ID */
        *(volatile u16*)0x800030E6 = 1;
    } else {
        /* Success: store device code with high bit set */
        u16 deviceCode = DriveInfo.deviceCode;
        *(volatile u16*)0x800030E6 = deviceCode | 0x8000;
    }
}

void OSInit(void) {
    u32 consoleType;
    u32* bi2;

    if (AreWeInitialized) {
        return;
    }
    AreWeInitialized = TRUE;

    __OSStartTime = __OSGetSystemTime();
    OSDisableInterrupts();

    /* Clear performance counters */
    PPCMtmmcr0(0);
    PPCMtmmcr1(0);
    PPCMtpmc1(0);
    PPCMtpmc2(0);
    PPCMtpmc3(0);
    PPCMtpmc4(0);
    PPCDisableSpeculation();
    PPCSetFpNonIEEEMode();

    /* Initialize boot info */
    BI2DebugFlag = NULL;
    BootInfo = (u32*)0x80000000;
    __DVDLongFileNameFlag = 0;

    /* Read BI2 debug info from boot info */
    bi2 = (u32*)BootInfo[0xF4 / 4];
    if (bi2 != NULL) {
        BI2DebugFlag = (u32*)(bi2 + 3); /* offset 0x0C in BI2 */
        __PADSpec = bi2[0x24 / 4];

        /* Store debug level and pad spec in low memory */
        OS_DEBUG_FLAG_ADDR = (u8)(*BI2DebugFlag);
        OS_PADSPEC_ADDR    = (u8)__PADSpec;
    } else {
        /* No BI2: check if there's DVDDiskID info */
        if (BootInfo[0x34 / 4] != 0) {
            /* Read from low memory flags instead */
            BI2DebugFlagHolder = (u32)OS_DEBUG_FLAG_ADDR;
            BI2DebugFlag = &BI2DebugFlagHolder;
            __PADSpec = (u32)OS_PADSPEC_ADDR;
        }
    }

    __DVDLongFileNameFlag = 1;

    /* Set up arena bounds */
    {
        void* lo = (void*)BootInfo[0x30 / 4];
        if (lo == NULL) {
            lo = (void*)__ArenaLo;
        }
        OSSetArenaLo(lo);
    }

    /* Check if ArenaLo needs adjustment for debug mode */
    if (BootInfo[0x30 / 4] == 0 && BI2DebugFlag != NULL && *BI2DebugFlag < 2) {
        /* Align stack address up to 32-byte boundary */
        void* lo = (void*)((((u32)_stack_addr) + 31) & ~31);
        OSSetArenaLo(lo);
    }

    /* Set arena hi */
    {
        void* hi = (void*)BootInfo[0x34 / 4];
        if (hi == NULL) {
            hi = (void*)__ArenaHi;
        }
        OSSetArenaHi(hi);
    }

    /* Initialize OS subsystems */
    OSExceptionInit();
    __OSInitSystemCall();
    OSInitAlarm();
    __OSModuleInit();
    __OSInterruptInit();
    __OSSetInterruptHandler(0x16,
                            (__OSInterruptHandler)__OSResetSWInterruptHandler);
    __OSContextInit();
    __OSCacheInit();
    EXIInit();
    SIInit();
    __OSInitSram();
    __OSThreadInit();
    __OSInitAudioSystem();

    /* Disable write-gather pipe speculation bit */
    {
        u32 hid2 = PPCMfhid2();
        hid2 &= 0xBFFFFFFF;  /* clear bit 1 */
        PPCMthid2(hid2);
    }

    if (!__OSInIPL) {
        __OSInitMemoryProtection();
    }

    /* Print boot info */
    OSReport("Dolphin OS $Revision: 54 $.\n");
    OSReport("Kernel built : %s %s\n", __DATE__, __TIME__);
    OSReport("Console Type : ");

    /* Detect and report console type */
    consoleType = 0;
    if (BootInfo != NULL) {
        consoleType = BootInfo[0x2C / 4];
    }

    if (consoleType == 0) {
        consoleType = 0x10000002;
    }

    {
        u32 type  = consoleType & 0xF0000000;
        u32 devId = consoleType & 0x0FFFFFFF;

        if (type == 0x10000000 || type == 0x20000000) {
            /* Retail or dev unit */
            switch (devId) {
            case 0x10000000:
                OSReport("Retail 1\n");
                break;
            case 0x10000001:
                OSReport("HW2 Production Board\n");
                break;
            case 0x10000002:
                OSReport("Latest Production Board\n");
                break;
            case 0x10000003:
                OSReport("NPDP-GDEV\n");
                break;
            default:
                OSReport("Unknown type : 0x%x (0x%x - %d)\n",
                         consoleType, devId, devId - 3);
                break;
            }
        } else if (type == 0) {
            OSReport("Mac Emulator\n");
        } else {
            OSReport("Unknown type : 0x%x\n", consoleType);
        }
    }

    /* Report memory size */
    OSReport("Memory %d MB\n", BootInfo[0x28 / 4] >> 20);

    /* Report arena */
    {
        void* lo = OSGetArenaLo();
        void* hi = OSGetArenaHi();
        OSReport("Arena : 0x%x - 0x%x\n", lo, hi);
    }

    /* Register OS version string */
    OSRegisterVersion(__OSVersion);

    /* Enable TRK interrupts if in debug mode */
    if (BI2DebugFlag != NULL && *BI2DebugFlag >= 2) {
        EnableMetroTRKInterrupts();
    }

    /* Clear the arena memory */
    ClearArena();

    /* Enable interrupts */
    OSEnableInterrupts();

    if (__OSInIPL) {
        return;
    }

    /* Initialize DVD */
    DVDInit();

    if (__OSIsGcam) {
        /* GC-AM (arcade) mode: set special game ID */
        *(volatile u16*)0x800030E6 = 0x9000;
        return;
    }

    /* Issue DVD inquiry to get drive info */
    DCInvalidateRange(&DriveInfo, sizeof(DriveInfo));
    DVDInquiryAsync((DVDCommandBlock*)((u8*)&DriveInfo + 0x20),
                    &DriveInfo,
                    (DVDCBCallback)InquiryCallback);
}

void OSRegisterVersion(const char* id) {
    OSReport("%s\n", id);
}
