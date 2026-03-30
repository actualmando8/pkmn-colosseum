    nofralloc
    cmplwi r3, 0x0
    beq @L_80135C50
    lwz r3, 0x8(r3)
    blr
@L_80135C50:
    li r3, 0x0
    blr
