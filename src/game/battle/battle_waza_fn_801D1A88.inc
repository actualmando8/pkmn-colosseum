    nofralloc
    cmpwi r3, 0x0
    blt @L_801D1AA0
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1AA8
@L_801D1AA0:
    li r3, 0x0
    b @L_801D1AB4
@L_801D1AA8:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1AB4:
    cmplwi r3, 0x0
    bne @L_801D1AC4
    li r3, 0x0
    blr
@L_801D1AC4:
    lwz r3, 0xc(r3)
    blr
