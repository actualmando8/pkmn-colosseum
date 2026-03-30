    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC4A0
    li r3, 0x0
    blr
    @L_801FC4A0:
    bne @L_801FC4AC
    li r4, 0x0
    b @L_801FC4CC
    @L_801FC4AC:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC4C0
    li r4, 0x0
    b @L_801FC4CC
    @L_801FC4C0:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x20
    add r4, r3, r4
    @L_801FC4CC:
    cmplwi r4, 0x0
    bne @L_801FC4DC
    li r3, 0x0
    blr
    @L_801FC4DC:
    lbz r3, 0x1(r4)
    blr
