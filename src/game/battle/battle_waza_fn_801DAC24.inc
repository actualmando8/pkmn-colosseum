    nofralloc
    cmplwi r3, 0x0
    beq @L_801DAC34
    lwz r3, 0x10(r3)
    blr
@L_801DAC34:
    li r3, 0x0
    blr
