    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E360
    li r3, 0x0
    blr
@L_8020E360:
    lbz r3, 0xd(r3)
    blr
