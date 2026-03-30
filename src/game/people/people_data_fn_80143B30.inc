    nofralloc
    cmplwi r3, 0x0
    bne @L_80143B40
    li r3, 0x0
    blr
    @L_80143B40:
    lhz r3, 0x0(r3)
    blr
