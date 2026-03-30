    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC754
    li r4, 0x0
    b @L_801FC774
    @L_801FC754:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    blt @L_801FC768
    li r4, 0x0
    b @L_801FC774
    @L_801FC768:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x30
    add r4, r3, r4
    @L_801FC774:
    cmplwi r4, 0x0
    beqlr
    stw r5, 0x4(r4)
    blr
