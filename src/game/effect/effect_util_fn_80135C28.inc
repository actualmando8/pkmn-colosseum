    nofralloc
    cmplwi r3, 0x0
    beq @L_80135C38
    lwz r3, 0x14(r3)
    blr
@L_80135C38:
    li r3, 0x0
    blr
