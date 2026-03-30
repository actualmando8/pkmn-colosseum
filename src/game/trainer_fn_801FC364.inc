    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC374
    li r3, 0x0
    blr
    @L_801FC374:
    bne @L_801FC380
    li r4, 0x0
    b @L_801FC3A0
    @L_801FC380:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC394
    li r4, 0x0
    b @L_801FC3A0
    @L_801FC394:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x24
    add r4, r3, r4
    @L_801FC3A0:
    cmplwi r4, 0x0
    bne @L_801FC3B0
    li r3, 0x0
    blr
    @L_801FC3B0:
    lbz r3, 0x0(r4)
    blr
