    nofralloc
    cmplwi r3, 0x0
    bne @L_801F66E4
    li r3, 0x0
    blr
    @L_801F66E4:
    lbz r3, 0x0(r3)
    blr
