    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D8E8
    li r3, 0x0
    blr
@L_8020D8E8:
    lwz r3, 0x10(r3)
    blr
