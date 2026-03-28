    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mulli r0, r3, 0x38
    lwz r4, -30900(r13)
    add r4, r4, r0
    beq @L_801EE274
    cmplwi r3, 0x60
    ble @L_801EE278
@L_801EE274:
    li r4, 0x0
@L_801EE278:
    cmplwi r4, 0x0
    beq @L_801EE29C
    lhz r31, 0xa(r4)
    li r3, 0x0
    bl fn_801EF1E4
    mulli r4, r31, 0x18
    addi r0, r4, 0x4a0
    lhzx r3, r3, r0
    b @L_801EE2A0
@L_801EE29C:
    li r3, 0x0
@L_801EE2A0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
