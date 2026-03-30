    nofralloc
    cmplwi r3, 0x0
    beq @L_8013AD94
    lwz r4, 0x54(r3)
    cmplwi r4, 0x0
    beq @L_8013AD94
    lwz r4, 0x10(r4)
    li r0, 0x0
    stw r4, 0x54(r3)
    stw r0, 0x58(r3)
    li r3, 0x1
    blr
    @L_8013AD94:
    li r3, 0x0
    blr
