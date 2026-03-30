    nofralloc
    cmplwi r3, 0x0
    bne @L_801F78CC
    li r3, 0x0
    blr
    @L_801F78CC:
    lbz r3, 0x522c(r3)
    blr
