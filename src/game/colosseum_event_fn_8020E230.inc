    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E240
    li r3, 0x0
    blr
@L_8020E240:
    lbz r3, 0x18(r3)
    blr
