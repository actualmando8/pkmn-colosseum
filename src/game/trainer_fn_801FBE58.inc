    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBE68
    li r3, 0x0
    blr
    @L_801FBE68:
    lbz r3, 0x10(r3)
    blr
