    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r30, lbl_8047AC5C(r13)
    lwz r31, lbl_8047AC60(r13)
    b @L_800F91E8
@L_800F9184:
    lwz r3, 0x4(r30)
    cmplwi r3, 0x0
    beq @L_800F91E4
    lwz r4, 0x8(r30)
    cmplw r4, r29
    bne @L_800F91E4
    lwz r12, 0x10(r30)
    cmplwi r12, 0x0
    beq @L_800F91BC
    lwz r5, 0xc(r30)
    mtctr r12
    bctrl
    clrlwi. r0, r3, 24
    beq @L_800F91E4
@L_800F91BC:
    lhz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_800F91DC
    bl fn_800E24B0
    lhz r3, 0x0(r30)
    bl fn_800E209C
    li r0, 0x0
    sth r0, 0x0(r30)
@L_800F91DC:
    li r0, 0x0
    stw r0, 0x4(r30)
@L_800F91E4:
    addi r30, r30, 0x14
@L_800F91E8:
    cmplwi r31, 0x0
    subi r31, r31, 0x1
    bne @L_800F9184
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
