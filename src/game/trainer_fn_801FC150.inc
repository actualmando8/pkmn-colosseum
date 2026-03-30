    nofralloc
    cmplwi r3, 0x0
    beqlr
    bne @L_801FC164
    li r4, 0x0
    b @L_801FC184
    @L_801FC164:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801FC178
    li r4, 0x0
    b @L_801FC184
    @L_801FC178:
    clrlslwi r4, r4, 16, 1
    addi r4, r4, 0x20
    add r4, r3, r4
    @L_801FC184:
    cmplwi r4, 0x0
    beqlr
    stb r5, 0x1(r4)
    blr
