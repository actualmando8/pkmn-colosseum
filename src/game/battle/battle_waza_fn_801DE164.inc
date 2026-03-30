    nofralloc
    cmplwi r3, 0x0
    bne @L_801DE174
    li r3, 0x0
    blr
@L_801DE174:
    lbz r0, 0x75(r3)
    cmplwi r0, 0x0
    beq @L_801DE188
    lwz r3, 0x78(r3)
    blr
@L_801DE188:
    li r3, 0x0
    blr
