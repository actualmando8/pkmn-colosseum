    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A390
    li r3, 0x0
    blr
@L_8020A390:
    lhz r3, 0x0(r3)
    blr
