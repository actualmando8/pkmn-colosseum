    nofralloc
    cmplwi r3, 0x0
    bne @L_801F66B4
    li r3, 0x0
    blr
    @L_801F66B4:
    lwz r3, 0xc(r3)
    blr
