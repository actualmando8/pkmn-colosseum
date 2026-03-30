    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D9E0
    li r3, 0x0
    blr
@L_8020D9E0:
    lhz r3, 0x0(r3)
    blr
