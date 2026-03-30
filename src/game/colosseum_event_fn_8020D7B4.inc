    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D7C4
    li r3, 0x0
    blr
@L_8020D7C4:
    lwz r3, 0x4(r3)
    blr
