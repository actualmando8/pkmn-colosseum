    nofralloc
    cmpwi r3, 0x0
    blt @L_801D1AE4
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801D1AEC
@L_801D1AE4:
    li r3, 0x0
    b @L_801D1AF8
@L_801D1AEC:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1AF8:
    cmplwi r3, 0x0
    bne @L_801D1B08
    li r3, 0x0
    blr
@L_801D1B08:
    lwz r3, 0x10(r3)
    blr
