    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD170
    li r3, -0x1
    blr
    @L_801FD170:
    lha r3, 0x6ae(r3)
    blr
