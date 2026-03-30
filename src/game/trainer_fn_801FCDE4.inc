    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCDF4
    li r3, 0x0
    blr
    @L_801FCDF4:
    lwz r3, 0x27b8(r3)
    blr
