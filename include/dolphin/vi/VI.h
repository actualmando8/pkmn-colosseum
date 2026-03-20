#ifndef DOLPHIN_VI_VI_H
#define DOLPHIN_VI_VI_H

#include "dolphin/types.h"

/* TV format values */
#define VI_NTSC   0
#define VI_PAL    1
#define VI_MPAL   2
#define VI_DEBUG  3
#define VI_DEBUG_PAL 4
#define VI_EUR60  5

u32 VIGetTvFormat(void);
u32 VIGetCurrentLine(void);

#endif /* DOLPHIN_VI_VI_H */
