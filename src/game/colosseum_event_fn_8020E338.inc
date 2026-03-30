    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E348
    li r3, 0x0
    blr
@L_8020E348:
    lbz r3, 0xe(r3)
    blr
