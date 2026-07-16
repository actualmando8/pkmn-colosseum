#include "trk/trk.h"

typedef void (*CommFunc)(void);

void EnableEXI2Interrupts(void)
{
    if (!TRK_Use_BBA) {
        CommFunc initInterrupts = (CommFunc)gDBCommTable.initInterrupts;

        if (initInterrupts != 0) {
            initInterrupts();
        }
    }
}
