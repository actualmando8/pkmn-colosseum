    nofralloc
    cmpwi r3, 0x0
    blt @L_801D15A4
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D15AC
@L_801D15A4:
    li r3, 0x0
    b @L_801D15B8
@L_801D15AC:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D15B8:
    cmplwi r3, 0x0
    bne @L_801D15C8
    li r3, 0xff
    blr
@L_801D15C8:
    lbz r3, 0x0(r3)
    blr
