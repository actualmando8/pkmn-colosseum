    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E3D8
    li r3, 0x0
    blr
@L_8020E3D8:
    lbz r3, 0x8(r3)
    blr
