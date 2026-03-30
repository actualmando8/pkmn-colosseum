    nofralloc
    cmplwi r3, 0x0
    bne @L_80109204
    li r3, 0x0
    blr
@L_80109204:
    lbz r0, 0x4(r3)
    extsb r0, r0
    rlwinm r3, r0, 0, 30, 30
    neg r0, r3
    or r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
    blr
