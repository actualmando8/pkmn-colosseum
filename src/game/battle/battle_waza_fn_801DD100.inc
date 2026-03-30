    nofralloc
    cmplwi r3, 0x0
    beqlr
    cmplwi r4, 0x0
    bne @L_801DD128
    li r0, 0x0
    sth r0, 0x32(r3)
    sth r0, 0x34(r3)
    lwz r3, 0x2c(r3)
    stw r0, 0x90(r3)
    blr
@L_801DD128:
    lwz r5, 0xc(r4)
    li r0, 0x0
    rlwinm r5, r5, 0, 24, 31
    sth r5, 0x32(r3)
    sth r0, 0x34(r3)
    lwz r0, 0xc(r4)
    lwz r5, 0x10(r4)
    mulli r4, r0, 0xd4
    lwz r3, 0x2c(r3)
    addi r0, r4, 0x90
    stwx r5, r3, r0
    blr
