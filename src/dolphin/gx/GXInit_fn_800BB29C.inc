    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    bl __GXFlushTextureState
    li r6, 0x61
    lis r3, 0x6600
    lis r5, 0xcc01
    stb r6, -0x8000(r5)
    addi r4, r3, 0x1000
    addi r0, r3, 0x1100
    stw r4, -0x8000(r5)
    stb r6, -0x8000(r5)
    stw r0, -0x8000(r5)
    bl __GXFlushTextureState
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
