    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC898
    li r4, 0x0
    b @L_801FC8B8
    @L_801FC898:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    blt @L_801FC8AC
    li r4, 0x0
    b @L_801FC8B8
    @L_801FC8AC:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x30
    add r4, r3, r4
    @L_801FC8B8:
    cmplwi r4, 0x0
    bne @L_801FC8C8
    li r3, 0x0
    blr
    @L_801FC8C8:
    lbz r3, 0x0(r4)
    blr
