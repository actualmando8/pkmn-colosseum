    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    lwz r31, 0x38(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    bl fn_800BA9E4
    lbz r0, 0x1f(r30)
    rlwinm r0, r0, 0, 31, 29
    stb r0, 0x1f(r30)
    stw r31, 0x18(r30)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
