    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6610
    li r3, 0x0
    blr
    @L_801F6610:
    lwz r3, 0x4(r3)
    blr
