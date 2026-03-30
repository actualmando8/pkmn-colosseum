    nofralloc
    cmpwi r3, 0x0
    blt @L_801D13FC
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1404
@L_801D13FC:
    li r3, 0x0
    b @L_801D1410
@L_801D1404:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1410:
    cmplwi r3, 0x0
    bne @L_801D1424
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D1424:
    lhz r3, 0x4(r3)
    blr
