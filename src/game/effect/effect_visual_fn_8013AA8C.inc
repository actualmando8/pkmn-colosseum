    nofralloc
    cmplwi r3, 0x0
    beq @L_8013AAB4
    lhz r5, 0x8(r3)
    lhz r0, 0xa(r3)
    cmplw r5, r0
    bge @L_8013AAB4
    add r0, r5, r4
    sth r0, 0x8(r3)
    li r3, 0x1
    blr
    @L_8013AAB4:
    li r3, 0x0
    blr
