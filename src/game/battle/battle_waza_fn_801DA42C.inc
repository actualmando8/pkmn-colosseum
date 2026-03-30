    nofralloc
    cmplwi r3, 0x0
    beq @L_801DA440
    lbz r0, 0x18(r3)
    rlwinm r3, r0, 0, 31, 31
    blr
@L_801DA440:
    li r3, 0x0
    blr
