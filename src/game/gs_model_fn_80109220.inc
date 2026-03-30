    nofralloc
    cmplwi r3, 0x0
    beqlr
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80109248
    lbz r0, 0x4(r3)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r3)
    blr
@L_80109248:
    lbz r0, 0x4(r3)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r3)
    blr
