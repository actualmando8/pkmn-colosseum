    nofralloc
    cmplwi r3, 0x0
    beq @L_80103FF4
    lwz r3, 0xb0(r3)
    blr
    @L_80103FF4:
    li r3, 0x0
    blr
