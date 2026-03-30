    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E258
    li r3, 0x0
    blr
@L_8020E258:
    lwz r3, 0x1c(r3)
    blr
