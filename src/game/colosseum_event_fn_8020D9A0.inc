    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D9B0
    li r3, 0x0
    blr
@L_8020D9B0:
    lhz r3, 0x4(r3)
    blr
