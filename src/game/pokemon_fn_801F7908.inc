    nofralloc
    mr. r5, r3
    bne @L_801F7918
    li r3, 0x0
    blr
    @L_801F7918:
    clrlwi r0, r4, 16
    cmplwi r0, 0x6
    blt @L_801F792C
    li r3, 0x0
    blr
    @L_801F792C:
    clrlslwi r3, r4, 16, 4
    addi r3, r3, 0x4
    add r3, r5, r3
    blr
