    nofralloc
    cmplwi r3, 0x0
    bne @L_80143770
    li r3, 0x0
    blr
    @L_80143770:
    lbz r3, 0xc(r3)
    blr
