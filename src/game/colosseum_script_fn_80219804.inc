    nofralloc
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_80219828
    cmplwi r0, 0xe3
    beq @L_80219828
    cmplwi r0, 0x77
    beq @L_80219828
    cmplwi r0, 0xffff
    bne @L_80219830
@L_80219828:
    li r3, 0x1
    blr
@L_80219830:
    li r3, 0x0
    blr
