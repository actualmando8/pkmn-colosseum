    nofralloc
    cmplwi r3, 0x0
    bne @L_80143740
    li r3, 0x0
    blr
    @L_80143740:
    lbz r3, 0xe(r3)
    blr
