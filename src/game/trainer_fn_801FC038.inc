    nofralloc
    cmplwi r3, 0x0
    beqlr
    bne @L_801FC04C
    li r4, 0x0
    b @L_801FC06C
    @L_801FC04C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC060
    li r4, 0x0
    b @L_801FC06C
    @L_801FC060:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x24
    add r4, r3, r4
    @L_801FC06C:
    cmplwi r4, 0x0
    beqlr
    stb r5, 0x1(r4)
    blr
