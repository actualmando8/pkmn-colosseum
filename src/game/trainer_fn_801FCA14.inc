    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCA24
    li r3, 0x0
    blr
    @L_801FCA24:
    lwz r3, 0x14(r3)
    blr
