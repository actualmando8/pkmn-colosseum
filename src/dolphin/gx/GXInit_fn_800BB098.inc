    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    addi r3, r4, 0x0
    lwz r5, -29632(r13)
    lwz r12, 0x414(r5)
    mtlr r12
    blrl
    mr r31, r3
    bl __GXFlushTextureState
    li r4, 0x61
    lis r3, 0xcc01
    stb r4, -0x8000(r3)
    lwz r0, 0x4(r30)
    stw r0, -0x8000(r3)
    stb r4, -0x8000(r3)
    lwz r0, 0x0(r31)
    stw r0, -0x8000(r3)
    bl __GXFlushTextureState
    lwz r0, 0x0(r31)
    lwz r3, 0x0(r30)
    rlwinm r0, r0, 0, 22, 31
    rlwimi r0, r3, 0, 0, 21
    stw r0, 0x0(r30)
    lwz r3, 0x0(r30)
    lwz r0, 0x4(r30)
    stw r3, 0x4(r31)
    stw r0, 0x8(r31)
    lwz r0, 0x8(r30)
    stw r0, 0xc(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
