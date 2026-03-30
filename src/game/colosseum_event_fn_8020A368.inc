    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A378
    li r3, 0x0
    blr
@L_8020A378:
    lhz r3, 0x2(r3)
    blr
