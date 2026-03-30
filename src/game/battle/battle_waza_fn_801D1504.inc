    nofralloc
    cmpwi r3, 0x0
    blt @L_801D151C
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1524
@L_801D151C:
    li r3, 0x0
    b @L_801D1530
@L_801D1524:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1530:
    cmplwi r3, 0x0
    bne @L_801D1540
    li r3, -0x1
    blr
@L_801D1540:
    lwz r3, 0x20(r3)
    blr
