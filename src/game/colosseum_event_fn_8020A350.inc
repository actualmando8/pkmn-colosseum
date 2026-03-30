    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A360
    li r3, 0x0
    blr
@L_8020A360:
    lwz r3, 0x4(r3)
    blr
