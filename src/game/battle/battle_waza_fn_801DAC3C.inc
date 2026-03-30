    nofralloc
    cmplwi r3, 0x0
    beq @L_801DAC4C
    lwz r3, 0x24(r3)
    blr
@L_801DAC4C:
    li r3, 0x0
    blr
