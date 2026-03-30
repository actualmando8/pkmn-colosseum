    nofralloc
    cmplwi r3, 0x0
    bne @L_80143830
    li r3, 0x0
    blr
    @L_80143830:
    lbz r3, 0x9(r3)
    blr
