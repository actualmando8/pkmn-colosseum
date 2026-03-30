    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBD20
    li r3, 0x0
    blr
    @L_801FBD20:
    lwz r3, 0x8(r3)
    blr
