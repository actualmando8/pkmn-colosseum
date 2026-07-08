#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSAlarm.h"

typedef struct WACommand {
    u32 type;
    u32 cmd;
    u32 addr;
    u32 offset;
    u32 callback;
} WACommand;

WACommand CommandList_803FC290[3];
extern u32 NextCommandNumber_8047A7C4;

void __DVDInitWA(void) {
    NextCommandNumber_8047A7C4 = 0;
    CommandList_803FC290[0].type = (u32)-1;
    __DVDLowSetWAType(0, 0);
    OSInitAlarm();
}
