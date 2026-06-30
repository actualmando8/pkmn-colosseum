#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSAlarm.h"

typedef struct WACommand {
    u32 type;
    u32 cmd;
    u32 addr;
    u32 offset;
    u32 callback;
} WACommand;

extern u32 NextCommandNumber;
extern WACommand CommandList[];

void __DVDInitWA(void) {
    NextCommandNumber = 0;
    CommandList[0].type = (u32)-1;
    __DVDLowSetWAType(0, 0);
    OSInitAlarm();
}
