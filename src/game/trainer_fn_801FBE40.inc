    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBE50
    li r3, 0x0
    blr
    @L_801FBE50:
    lbz r3, 0x11(r3)
    blr
