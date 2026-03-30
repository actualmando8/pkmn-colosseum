    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    bl OSDisableInterrupts
    lis r3, 0xcc00
    addi r3, r3, 0x2000
    li r0, 0x0
    sth r0, 0x2(r3)
    bl ICFlashInvalidate
    rlwinm r3, r31, 3, 0, 28
    bl Reset
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
