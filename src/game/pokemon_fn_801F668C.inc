    nofralloc
    cmplwi r3, 0x0
    bne @L_801F669C
    li r3, 0x0
    blr
    @L_801F669C:
    lwz r3, 0x10(r3)
    blr
