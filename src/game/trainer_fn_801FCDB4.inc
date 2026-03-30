    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCDC4
    li r3, 0x0
    blr
    @L_801FCDC4:
    lbz r3, 0x27bc(r3)
    blr
