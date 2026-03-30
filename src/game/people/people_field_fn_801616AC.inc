    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, 0x214(r3)
    rlwinm. r0, r4, 0, 28, 28
    bne @L_801616CC
    lhz r3, 0x2a4(r3)
    b @L_801616E4
    @L_801616CC:
    rlwinm r0, r4, 0, 29, 27
    addi r4, r3, 0x284
    stw r0, 0x214(r3)
    lbz r5, 0x121(r3)
    lbz r6, 0x122(r3)
    bl fn_80161134
    @L_801616E4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
