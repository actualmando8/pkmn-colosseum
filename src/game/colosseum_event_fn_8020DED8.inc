    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DEE8
    li r3, 0x0
    blr
@L_8020DEE8:
    lwz r3, 0x8(r3)
    blr
