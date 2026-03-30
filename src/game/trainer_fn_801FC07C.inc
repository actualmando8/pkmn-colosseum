    nofralloc
    cmplwi r3, 0x0
    beqlr
    bne @L_801FC090
    li r4, 0x0
    b @L_801FC0B0
    @L_801FC090:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC0A4
    li r4, 0x0
    b @L_801FC0B0
    @L_801FC0A4:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x24
    add r4, r3, r4
    @L_801FC0B0:
    cmplwi r4, 0x0
    beqlr
    stb r5, 0x0(r4)
    blr
