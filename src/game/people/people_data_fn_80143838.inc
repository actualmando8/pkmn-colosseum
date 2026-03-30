    nofralloc
    cmplwi r3, 0x0
    bne @L_80143848
    li r3, 0x0
    blr
    @L_80143848:
    lbz r3, 0x8(r3)
    blr
