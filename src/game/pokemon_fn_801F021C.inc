    nofralloc
    cmplwi r3, 0x0
    bne @L_801F022C
    li r3, 0x0
    blr
    @L_801F022C:
    lbz r3, 0x0(r3)
    blr
