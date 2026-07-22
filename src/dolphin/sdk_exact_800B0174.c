#include "src/dolphin/card_dsp_private.h"

void __CARDSetDiskID(DVDDiskID* diskId)
{
    CARDControl* card = lbl_803FC620;

    card[0].diskId = diskId ? diskId : (DVDDiskID*) &card[2];
    card[1].diskId = diskId ? diskId : (DVDDiskID*) &card[2];
}
