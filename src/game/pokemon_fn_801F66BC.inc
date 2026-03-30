    nofralloc
    cmplwi r3, 0x0
    bne @L_801F66CC
    li r3, 0x0
    blr
    @L_801F66CC:
    lwz r3, 0x8(r3)
    blr
