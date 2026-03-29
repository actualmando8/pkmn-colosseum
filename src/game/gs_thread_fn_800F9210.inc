    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047AC60(r13)
    lwz r31, lbl_8047AC5C(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800F9264
@L_800F9234:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    beq @L_800F925C
    lwz r0, 0x8(r31)
    cmplw r0, r3
    bne @L_800F925C
    lwz r0, 0xc(r31)
    cmplw r0, r4
    bne @L_800F925C
    b @L_800F9268
@L_800F925C:
    addi r31, r31, 0x14
    bdnz @L_800F9234
@L_800F9264:
    li r31, 0x0
@L_800F9268:
    cmplwi r31, 0x0
    beq @L_800F92C0
    lwz r12, 0x10(r31)
    cmplwi r12, 0x0
    beq @L_800F9298
    lwz r3, 0x4(r31)
    lwz r4, 0x8(r31)
    lwz r5, 0xc(r31)
    mtctr r12
    bctrl
    clrlwi. r0, r3, 24
    beq @L_800F92C0
@L_800F9298:
    lhz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800F92B8
    bl fn_800E24B0
    lhz r3, 0x0(r31)
    bl fn_800E209C
    li r0, 0x0
    sth r0, 0x0(r31)
@L_800F92B8:
    li r0, 0x0
    stw r0, 0x4(r31)
@L_800F92C0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
