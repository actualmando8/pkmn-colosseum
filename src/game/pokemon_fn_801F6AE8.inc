    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6AF8
    li r3, 0x0
    blr
    @L_801F6AF8:
    lhz r3, 0x12(r3)
    blr
