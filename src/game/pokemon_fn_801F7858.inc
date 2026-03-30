    nofralloc
    cmplwi r3, 0x0
    bne @L_801F7868
    li r3, 0x0
    blr
    @L_801F7868:
    lhz r3, 0x0(r3)
    blr
