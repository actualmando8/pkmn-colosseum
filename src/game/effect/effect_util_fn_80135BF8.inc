    nofralloc
    cmplwi r3, 0x0
    beq @L_80135C08
    lwz r3, 0x1c(r3)
    blr
@L_80135C08:
    li r3, 0x0
    blr
