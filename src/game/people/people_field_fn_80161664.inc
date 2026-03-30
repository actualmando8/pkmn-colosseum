    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, 0x214(r3)
    rlwinm. r0, r4, 0, 29, 29
    bne @L_80161684
    lhz r3, 0x280(r3)
    b @L_8016169C
    @L_80161684:
    rlwinm r0, r4, 0, 30, 28
    addi r4, r3, 0x260
    stw r0, 0x214(r3)
    lbz r5, 0x121(r3)
    lbz r6, 0x122(r3)
    bl fn_80161134
    @L_8016169C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
