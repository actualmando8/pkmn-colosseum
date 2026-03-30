    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC714
    li r4, 0x0
    b @L_801FC734
    @L_801FC714:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    blt @L_801FC728
    li r4, 0x0
    b @L_801FC734
    @L_801FC728:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x30
    add r4, r3, r4
    @L_801FC734:
    cmplwi r4, 0x0
    beqlr
    stb r5, 0x0(r4)
    blr
