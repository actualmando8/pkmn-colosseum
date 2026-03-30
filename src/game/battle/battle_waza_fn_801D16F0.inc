    nofralloc
    cmpwi r3, 0x0
    blt @L_801D1708
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1710
@L_801D1708:
    li r3, 0x0
    b @L_801D171C
@L_801D1710:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D171C:
    cmplwi r3, 0x0
    bne @L_801D172C
    li r3, 0x0
    blr
@L_801D172C:
    lwz r3, 0x18(r3)
    blr
