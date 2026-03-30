    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBD50
    li r3, 0x0
    blr
    @L_801FBD50:
    lhz r3, 0x0(r3)
    blr
