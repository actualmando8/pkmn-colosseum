    nofralloc
    cmpwi r3, 0x0
    blt @L_801D13B4
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D13BC
@L_801D13B4:
    li r3, 0x0
    b @L_801D13C8
@L_801D13BC:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D13C8:
    cmplwi r3, 0x0
    bne @L_801D13DC
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D13DC:
    lhz r3, 0x8(r3)
    blr
