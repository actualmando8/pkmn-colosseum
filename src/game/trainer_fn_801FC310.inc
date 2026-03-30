    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC320
    li r3, 0x0
    blr
    @L_801FC320:
    bne @L_801FC32C
    li r4, 0x0
    b @L_801FC34C
    @L_801FC32C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC340
    li r4, 0x0
    b @L_801FC34C
    @L_801FC340:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x24
    add r4, r3, r4
    @L_801FC34C:
    cmplwi r4, 0x0
    bne @L_801FC35C
    li r3, 0x0
    blr
    @L_801FC35C:
    lbz r3, 0x1(r4)
    blr
