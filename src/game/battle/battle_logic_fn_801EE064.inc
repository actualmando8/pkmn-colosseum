    nofralloc
    cmplwi r3, 0x0
    bne @L_801EE074
    li r3, 0x0
    blr
@L_801EE074:
    lbz r3, 0x0(r3)
    blr
