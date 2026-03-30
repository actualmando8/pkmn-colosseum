    nofralloc
    cmplwi r3, 0x0
    beq @L_801A8468
    lwz r3, 0x4(r3)
    blr
@L_801A8468:
    li r3, 0x0
    blr
