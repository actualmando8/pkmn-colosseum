    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DE78
    lfs f1, -20848(r2)
    blr
@L_8020DE78:
    lfs f1, 0x4(r3)
    blr
