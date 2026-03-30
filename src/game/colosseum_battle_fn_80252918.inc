    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r6
    bl fn_802376EC
    rlwinm r31, r3, 0, 16, 31
    mr r3, r29
    mr r4, r30
    bl fn_802376EC
    rlwinm r3, r3, 0, 16, 31
    add r0, r31, r3
    srawi r0, r0, 1
    addze r0, r0
    subf r3, r0, r3
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
