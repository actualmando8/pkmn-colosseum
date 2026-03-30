    nofralloc
    cmplwi r3, 0x0
    bne @L_80143758
    li r3, 0x0
    blr
    @L_80143758:
    lbz r3, 0xd(r3)
    blr
