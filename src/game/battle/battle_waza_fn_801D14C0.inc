    nofralloc
    cmpwi r3, 0x0
    blt @L_801D14D8
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D14E0
@L_801D14D8:
    li r3, 0x0
    b @L_801D14EC
@L_801D14E0:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D14EC:
    cmplwi r3, 0x0
    bne @L_801D14FC
    li r3, -0x1
    blr
@L_801D14FC:
    lwz r3, 0x24(r3)
    blr
