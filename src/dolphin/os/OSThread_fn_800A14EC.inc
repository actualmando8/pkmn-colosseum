    nofralloc
    lwz r4, 0x2d4(r3)
    lwz r5, 0x2f4(r3)
    b @L_800A1518
    @L_800A14F8:
    lwz r3, 0x0(r5)
    cmplwi r3, 0x0
    beq @L_800A1514
    lwz r0, 0x2d0(r3)
    cmpw r0, r4
    bge @L_800A1514
    mr r4, r0
    @L_800A1514:
    lwz r5, 0x10(r5)
    @L_800A1518:
    cmplwi r5, 0x0
    bne @L_800A14F8
    mr r3, r4
    blr
