    nofralloc
    cmpwi r3, 0x0
    blt @L_801D1560
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1568
@L_801D1560:
    li r3, 0x0
    b @L_801D1574
@L_801D1568:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1574:
    cmplwi r3, 0x0
    bne @L_801D1584
    li r3, -0x1
    blr
@L_801D1584:
    lwz r3, 0x1c(r3)
    blr
