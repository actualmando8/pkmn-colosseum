    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBE38
    li r3, 0x0
    blr
    @L_801FBE38:
    lbz r3, 0x12(r3)
    blr
