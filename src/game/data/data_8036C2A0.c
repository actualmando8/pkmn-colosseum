#include "dolphin/types.h"

#pragma section ".data"

extern void* lbl_8036C2A0[];
extern void* lbl_8036C3E0[];

extern u8 floorReadBGMPostFunc[];
extern u8 floorReadBGMPreFunc[];
extern u8 floorReadCameraPostFunc[];
extern u8 floorReadCameraPreFunc[];
extern u8 floorReadColPostFunc[];
extern u8 floorReadColPreFunc[];
extern u8 floorReadFontPostFunc[];
extern u8 floorReadFontPreFunc[];
extern u8 floorReadGFLPostFunc[];
extern u8 floorReadGFLPreFunc[];
extern u8 floorReadMapPostFunc[];
extern u8 floorReadMapPreFunc[];
extern u8 floorReadMsgPostFunc[];
extern u8 floorReadMsgPreFunc[];
extern u8 floorReadNormalPreFunc[];
extern u8 floorReadNotLinkedParticlePostFunc[];
extern u8 floorReadNotLinkedParticlePreFunc[];
extern u8 floorReadObjPostFunc[];
extern u8 floorReadObjPreFunc[];
extern u8 floorReadPKXPreFunc[];
extern u8 floorReadParticlePostFunc[];
extern u8 floorReadParticlePreFunc[];
extern u8 floorReadScriptPostFunc[];
extern u8 floorReadScriptPreFunc[];
extern u8 floorReadTexPostFunc[];
extern u8 floorReadTexPreFunc[];
extern u8 floorReadWZXPreFunc[];
extern u8 fn_80114254[];
extern u8 fn_801142B4[];
extern u8 fn_8017BFE8[];
extern u8 fn_8017BFF0[];
extern u8 fn_8017BFF8[];
extern u8 fn_8017C000[];
extern u8 fn_8017C008[];
extern u8 fn_8017C394[];
extern u8 fn_8017C39C[];
extern u8 fn_8017C414[];
extern u8 fn_8017C568[];
extern u8 fn_8017C570[];
extern u8 fn_8017C578[];
extern u8 fn_8017C580[];
extern u8 fn_8017C590[];
extern u8 fn_8017C598[];
extern u8 fn_8017C5A0[];
extern u8 fn_8017C5B0[];
extern u8 fn_8017C5B8[];
extern u8 fn_8017C6E0[];
extern u8 fn_8017C88C[];
extern u8 fn_8017C894[];
extern u8 fn_8017C8C0[];
extern u8 fn_8017C8C8[];
extern u8 fn_8017C8F4[];
extern u8 fn_8017C8FC[];
extern u8 fn_8017CE7C[];
extern u8 fn_8017CEC8[];
extern u8 fn_8017CED0[];
extern u8 fn_8017CED8[];
extern u8 fn_8017D3A0[];
extern u8 fn_8017D3D4[];
extern u8 fn_8017D400[];
extern u8 fn_8017D408[];
extern u8 fn_8017F3F8[];
extern u8 fn_8017F484[];

/* Auto-carved .data unit 0x8036C2A0..0x8036C4E8 (2 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* lbl_8036C2A0[80] = {
    (void*)0x00000320,
    (void*)0x00000000,
    (void*)((u8*)floorReadNormalPreFunc),
    (void*)0x00000000,
    (void*)0x00000258,
    (void*)0x00000001,
    (void*)((u8*)floorReadMapPreFunc),
    (void*)((u8*)floorReadMapPostFunc),
    (void*)0x000002BC,
    (void*)0x00000002,
    (void*)((u8*)floorReadObjPreFunc),
    (void*)((u8*)floorReadObjPostFunc),
    (void*)0x00000320,
    (void*)0x00000003,
    (void*)((u8*)floorReadColPreFunc),
    (void*)((u8*)floorReadColPostFunc),
    (void*)0x0000012C,
    (void*)0x00000004,
    (void*)((u8*)floorReadBGMPreFunc),
    (void*)((u8*)floorReadBGMPostFunc),
    (void*)0x00000190,
    (void*)0x00000005,
    (void*)((u8*)floorReadMsgPreFunc),
    (void*)((u8*)floorReadMsgPostFunc),
    (void*)0x000001F4,
    (void*)0x00000006,
    (void*)((u8*)floorReadFontPreFunc),
    (void*)((u8*)floorReadFontPostFunc),
    (void*)0x000000C8,
    (void*)0x00000007,
    (void*)((u8*)floorReadScriptPreFunc),
    (void*)((u8*)floorReadScriptPostFunc),
    (void*)0x00000320,
    (void*)0x00000008,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000384,
    (void*)0x00000009,
    (void*)((u8*)floorReadTexPreFunc),
    (void*)((u8*)floorReadTexPostFunc),
    (void*)0x00000320,
    (void*)0x0000000A,
    (void*)((u8*)floorReadParticlePreFunc),
    (void*)((u8*)floorReadParticlePostFunc),
    (void*)0x00000320,
    (void*)0x0000000B,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000320,
    (void*)0x0000000C,
    (void*)((u8*)floorReadCameraPreFunc),
    (void*)((u8*)floorReadCameraPostFunc),
    (void*)0x00000320,
    (void*)0x0000000D,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000064,
    (void*)0x0000000E,
    (void*)((u8*)fn_8017F3F8),
    (void*)((u8*)fn_8017F484),
    (void*)0x000002BC,
    (void*)0x0000000F,
    (void*)((u8*)floorReadPKXPreFunc),
    (void*)0x00000000,
    (void*)0x00000320,
    (void*)0x00000010,
    (void*)((u8*)floorReadWZXPreFunc),
    (void*)0x00000000,
    (void*)0x000003B6,
    (void*)0x00000011,
    (void*)((u8*)floorReadGFLPreFunc),
    (void*)((u8*)floorReadGFLPostFunc),
    (void*)0x00000320,
    (void*)0x00000012,
    (void*)((u8*)floorReadNotLinkedParticlePreFunc),
    (void*)((u8*)floorReadNotLinkedParticlePostFunc),
    (void*)0x00000064,
    (void*)0x00000013,
    (void*)((u8*)fn_801142B4),
    (void*)((u8*)fn_80114254),
};

void* lbl_8036C3E0[66] = {
    (void*)0x00000000,
    (void*)((u8*)fn_8017D408),
    (void*)0x00000001,
    (void*)((u8*)fn_8017D400),
    (void*)0x00000002,
    (void*)((u8*)fn_8017D3D4),
    (void*)0x00000003,
    (void*)((u8*)fn_8017D3A0),
    (void*)0x00000004,
    (void*)((u8*)fn_8017CED8),
    (void*)0x00000005,
    (void*)((u8*)fn_8017CED0),
    (void*)0x000003E8,
    (void*)((u8*)fn_8017CEC8),
    (void*)0x000007D0,
    (void*)((u8*)fn_8017CE7C),
    (void*)0x00000064,
    (void*)((u8*)fn_8017C8FC),
    (void*)0x00000065,
    (void*)((u8*)fn_8017C88C),
    (void*)0x00000066,
    (void*)((u8*)fn_8017C6E0),
    (void*)0x00000067,
    (void*)((u8*)fn_8017C008),
    (void*)0x00000096,
    (void*)((u8*)fn_8017C5B8),
    (void*)0x00000097,
    (void*)((u8*)fn_8017C5B0),
    (void*)0x00000098,
    (void*)((u8*)fn_8017C5A0),
    (void*)0x000000A0,
    (void*)((u8*)fn_8017C598),
    (void*)0x000000A1,
    (void*)((u8*)fn_8017C590),
    (void*)0x000000A2,
    (void*)((u8*)fn_8017C580),
    (void*)0x0000012C,
    (void*)((u8*)fn_8017C578),
    (void*)0x0000012D,
    (void*)((u8*)fn_8017C570),
    (void*)0x0000012E,
    (void*)((u8*)fn_8017C568),
    (void*)0x00000130,
    (void*)((u8*)fn_8017C39C),
    (void*)0x00000131,
    (void*)((u8*)fn_8017C394),
    (void*)0x0000012F,
    (void*)((u8*)fn_8017C414),
    (void*)0x000001F4,
    (void*)((u8*)fn_8017C000),
    (void*)0x00000258,
    (void*)((u8*)fn_8017BFF8),
    (void*)0x000000C8,
    (void*)((u8*)fn_8017C8F4),
    (void*)0x000000C9,
    (void*)((u8*)fn_8017C8C8),
    (void*)0x00000190,
    (void*)((u8*)fn_8017C8C0),
    (void*)0x00000191,
    (void*)((u8*)fn_8017C894),
    (void*)0x0000270F,
    (void*)((u8*)fn_8017BFF0),
    (void*)0x00001F40,
    (void*)((u8*)fn_8017BFE8),
    (void*)0xFFFFFFFF,
    (void*)0x00000000,
};

