    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDB70
    li r3, 0x0
    blr
    @L_801FDB70:
    lwz r3, 0x0(r3)
    blr
