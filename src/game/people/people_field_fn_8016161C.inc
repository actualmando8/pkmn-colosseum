    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, 0x214(r3)
    rlwinm. r0, r4, 0, 30, 30
    bne @L_8016163C
    lhz r3, 0x25c(r3)
    b @L_80161654
    @L_8016163C:
    rlwinm r0, r4, 0, 31, 29
    addi r4, r3, 0x23c
    stw r0, 0x214(r3)
    lbz r5, 0x121(r3)
    lbz r6, 0x122(r3)
    bl fn_80161134
    @L_80161654:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
