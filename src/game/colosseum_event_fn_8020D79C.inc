    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D7AC
    li r3, 0x0
    blr
@L_8020D7AC:
    lwz r3, 0x8(r3)
    blr
