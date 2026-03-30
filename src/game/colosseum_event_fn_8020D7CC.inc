    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D7DC
    li r3, -0x80
    blr
@L_8020D7DC:
    lbz r0, 0x0(r3)
    extsb r3, r0
    blr
