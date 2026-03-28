    nofralloc
    cmplwi r3, 0x0
    bne @L_801EE05C
    li r3, 0x0
    blr
@L_801EE05C:
    lbz r3, 0x1(r3)
    blr
