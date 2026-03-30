    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D83C
    li r3, 0x0
    blr
@L_8020D83C:
    lhz r3, 0x0(r3)
    blr
