    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A1EC
    li r3, 0x0
    blr
@L_8020A1EC:
    lwz r3, 0x9c(r3)
    blr
