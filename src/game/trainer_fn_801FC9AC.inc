    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC9BC
    li r3, -0x1
    blr
    @L_801FC9BC:
    clrlwi r0, r4, 24
    cmplwi r0, 0x6
    blt @L_801FC9D0
    li r3, -0x1
    blr
    @L_801FC9D0:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lha r3, 0x22(r3)
    blr
