    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E000
    li r3, 0x0
    blr
@L_8020E000:
    lwz r3, 0x10(r3)
    blr
