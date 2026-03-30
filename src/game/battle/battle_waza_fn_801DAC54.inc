    nofralloc
    cmplwi r3, 0x0
    bne @L_801DAC64
    li r3, 0x0
    blr
@L_801DAC64:
    lwz r3, 0x6c(r3)
    neg r0, r3
    or r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
    blr
