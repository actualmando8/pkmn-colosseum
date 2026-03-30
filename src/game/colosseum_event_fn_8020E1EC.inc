    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E1FC
    li r3, 0x0
    blr
@L_8020E1FC:
    lwz r3, 0x4(r3)
    blr
