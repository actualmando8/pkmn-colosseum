    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC4F4
    li r3, 0x0
    blr
    @L_801FC4F4:
    bne @L_801FC500
    li r4, 0x0
    b @L_801FC520
    @L_801FC500:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC514
    li r4, 0x0
    b @L_801FC520
    @L_801FC514:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x20
    add r4, r3, r4
    @L_801FC520:
    cmplwi r4, 0x0
    bne @L_801FC530
    li r3, 0x0
    blr
    @L_801FC530:
    lbz r3, 0x0(r4)
    blr
