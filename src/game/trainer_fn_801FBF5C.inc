    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBF6C
    li r3, 0x0
    blr
    @L_801FBF6C:
    lwz r3, 0x8(r3)
    blr
