    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBF14
    li r3, 0x0
    blr
    @L_801FBF14:
    lha r3, 0x0(r3)
    blr
