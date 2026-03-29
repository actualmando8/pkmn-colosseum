    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    mr r31, r4
    lis r4, lbl_80267228@ha
    li r3, 0xa
    addi r30, r4, lbl_80267228@l
    lwz r12, 0x0(r30)
    lwz r11, 0x4(r30)
    lwz r10, 0x8(r30)
    lwz r9, 0xc(r30)
    lwz r8, 0x10(r30)
    lwz r7, 0x14(r30)
    lwz r6, 0x18(r30)
    lwz r5, 0x1c(r30)
    lwz r4, 0x20(r30)
    lwz r0, 0x24(r30)
    stw r12, 0x10(r1)
    stw r11, 0x14(r1)
    stw r10, 0x18(r1)
    stw r9, 0x1c(r1)
    stw r8, 0x20(r1)
    stw r7, 0x24(r1)
    stw r6, 0x28(r1)
    stw r5, 0x2c(r1)
    stw r4, 0x30(r1)
    stw r0, 0x34(r1)
    bl fn_80103E68
    srwi r0, r3, 16
    li r3, 0x0
    sth r0, 0x8(r1)
    sth r0, 0xc(r1)
    lha r4, 0x6(r31)
    lwz r0, 0x10(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x1
    lwz r0, 0x14(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x2
    lwz r0, 0x18(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x3
    lwz r0, 0x1c(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x4
    lwz r0, 0x20(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x5
    lwz r0, 0x24(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x6
    lwz r0, 0x28(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x7
    lwz r0, 0x2c(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x8
    lwz r0, 0x30(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0x9
    lwz r0, 0x34(r1)
    cmpw r4, r0
    beq @L_8004CD64
    li r3, 0xa
    @L_8004CD64:
    cmpwi r3, 0xa
    blt @L_8004CD74
    li r3, 0x0
    b @L_8004CDC0
    @L_8004CD74:
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    add r3, r3, r0
    bl fn_8004BE40
    cmpwi r3, 0x0
    blt @L_8004CDAC
    bl fn_801D1B78
    cmpwi r3, 0x0
    beq @L_8004CDA4
    li r0, 0x1
    b @L_8004CDB0
    @L_8004CDA4:
    li r0, 0x0
    b @L_8004CDB0
    @L_8004CDAC:
    li r0, 0x0
    @L_8004CDB0:
    mr r3, r31
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x0
    @L_8004CDC0:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
