    nofralloc
    cmplwi r3, 0x0
    beq @L_80135C88
    lwz r3, 0xc(r3)
    blr
@L_80135C88:
    li r3, 0x0
    blr
