    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    clrlslwi r0, r4, 24, 22
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    lis r31, 0xcc01
    stw r30, 0x18(r1)
    li r30, 0x61
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r5, -29632(r13)
    lwz r4, 0x7c(r5)
    rlwinm r4, r4, 0, 10, 8
    or r0, r4, r0
    stw r0, 0x7c(r5)
    stb r30, -0x8000(r31)
    lwz r0, 0x7c(r5)
    stw r0, -0x8000(r31)
    bl __GXFlushTextureState
    rlwinm r0, r29, 0, 24, 31
    stb r30, -0x8000(r31)
    oris r0, r0, 0x6800
    stw r0, -0x8000(r31)
    bl __GXFlushTextureState
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
