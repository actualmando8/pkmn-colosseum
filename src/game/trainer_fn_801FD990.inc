    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD9A0
    li r3, 0x0
    blr
    @L_801FD9A0:
    lbz r3, 0x608(r3)
    blr
