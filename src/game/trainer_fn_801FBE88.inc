    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBE98
    li r3, 0x0
    blr
    @L_801FBE98:
    lbz r3, 0xe(r3)
    blr
