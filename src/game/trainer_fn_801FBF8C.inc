    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBF9C
    li r3, 0x0
    blr
    @L_801FBF9C:
    lwz r3, 0xc(r3)
    blr
