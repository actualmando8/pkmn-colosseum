    nofralloc
    cmplwi r3, 0x0
    bne @L_801A6DD4
    li r3, 0x0
    blr
    @L_801A6DD4:
    lwz r3, 0x8(r3)
    blr
