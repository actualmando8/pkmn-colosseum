    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mulli r0, r3, 0x38
    lwz r4, -30900(r13)
    add r4, r4, r0
    beq @L_801EE49C
    cmplwi r3, 0x60
    ble @L_801EE4A0
@L_801EE49C:
    li r4, 0x0
@L_801EE4A0:
    cmplwi r4, 0x0
    beq @L_801EE4C4
    lhz r31, 0xa(r4)
    li r3, 0x0
    bl fn_801EF1E4
    mulli r4, r31, 0xc
    addi r0, r4, 0x6
    lhzx r3, r3, r0
    b @L_801EE4C8
@L_801EE4C4:
    li r3, 0x0
@L_801EE4C8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
