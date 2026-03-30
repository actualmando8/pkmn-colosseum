    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D900
    li r3, 0x0
    blr
@L_8020D900:
    lwz r3, 0xc(r3)
    blr
