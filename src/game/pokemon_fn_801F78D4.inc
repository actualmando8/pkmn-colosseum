    nofralloc
    mr. r5, r3
    bne @L_801F78E4
    li r3, 0x0
    blr
    @L_801F78E4:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    blt @L_801F78F8
    li r3, 0x0
    blr
    @L_801F78F8:
    mulli r3, r0, 0x28e4
    addi r3, r3, 0x64
    add r3, r5, r3
    blr
