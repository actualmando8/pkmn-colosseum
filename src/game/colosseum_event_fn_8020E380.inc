    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E390
    li r3, 0x0
    blr
@L_8020E390:
    lbz r3, 0xb(r3)
    blr
