    nofralloc
    cmplwi r3, 0x0
    bne @L_80143ACC
    li r3, 0x0
    blr
    @L_80143ACC:
    clrlwi r0, r4, 16
    cmplwi r0, 0x19
    blt @L_80143AE0
    li r3, 0x0
    blr
    @L_80143AE0:
    add r3, r3, r0
    lbz r0, 0x4(r3)
    extsb r3, r0
    blr
