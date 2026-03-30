    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBF84
    li r3, 0x0
    blr
    @L_801FBF84:
    lwz r3, 0x4(r3)
    blr
