    nofralloc
    cmpwi r3, 0x0
    blt @L_801D1444
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D144C
@L_801D1444:
    li r3, 0x0
    b @L_801D1458
@L_801D144C:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1458:
    cmplwi r3, 0x0
    bne @L_801D1468
    li r3, 0xff
    blr
@L_801D1468:
    lbz r3, 0x1(r3)
    blr
