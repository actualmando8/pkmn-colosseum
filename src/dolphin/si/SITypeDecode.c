#include "dolphin/types.h"

u32 SIDecodeType(u32 type) {
    u32 error;
    u32 id;
    u32 masked;

    error = type & 0xFF;
    id = type & ~0xFF;
    if ((type & 8) != 0) {
        return 8;
    }
    if ((error & 0x47) != 0) {
        return 0x40;
    }
    if (error != 0) {
        return 0x80;
    }

    masked = id & 0x18000000;
    if (masked == 0) {
        masked = id & 0xFFFF0000;
        switch (masked) {
        case 0x00010000:
        case 0x00020000:
        case 0x00040000:
        case 0x02000000:
        case 0x05000000:
            return masked;
        }
        return 0x40;
    }

    if (masked != 0x08000000) {
        return 0x40;
    }

    masked = id & 0xFFFF0000;
    switch (masked) {
    case 0x08000000:
    case 0x09000000:
        return masked;
    }

    masked = id & 0xFFE00000;
    if (masked == 0x08200000) {
        return 0x08200000;
    }

    if ((id & 0x80000000) != 0) {
        if ((id & 0x04000000) == 0) {
            masked = id & 0x8B100000;
            if (masked == 0x8B100000) {
                return 0x8B100000;
            }
            if ((id & 0x02000000) == 0) {
                return 0x88000000;
            }
        }
    }

    masked = id & 0x09000000;
    if (masked == 0x09000000) {
        return 0x09000000;
    }
    return 0x40;
}
