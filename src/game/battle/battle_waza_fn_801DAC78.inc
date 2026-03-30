    nofralloc
    cmplwi r3, 0x0
    beq @L_801DAC88
    lhz r3, 0x70(r3)
    blr
@L_801DAC88:
    li r3, 0x0
    blr
