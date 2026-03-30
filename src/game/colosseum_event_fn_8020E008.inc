    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E018
    li r3, 0x0
    blr
@L_8020E018:
    lwz r3, 0xc(r3)
    blr
