    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDAAC
    li r3, 0x0
    blr
    @L_801FDAAC:
    lwz r3, 0x0(r3)
    blr
