    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, 0x214(r3)
    rlwinm. r0, r4, 0, 27, 27
    bne @L_80161714
    lhz r3, 0x2c8(r3)
    b @L_8016172C
    @L_80161714:
    rlwinm r0, r4, 0, 28, 26
    addi r4, r3, 0x2a8
    stw r0, 0x214(r3)
    lbz r5, 0x121(r3)
    lbz r6, 0x122(r3)
    bl fn_80161134
    @L_8016172C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
