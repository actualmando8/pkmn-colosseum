    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    li r31, 0x0
    stw r30, 0x10(r1)
    stw r29, 0xc(r1)
    mr r29, r4
    stw r28, 0x8(r1)
    mr r28, r3
    b @L_801849B4
    @L_8018497C:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801849B0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_801849B0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_801849B0
    lwz r31, 0x4(r3)
    b @L_80184A20
    @L_801849B0:
    addi r31, r31, 0x1
    @L_801849B4:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_8018497C
    li r30, 0x0
    b @L_80184A10
    @L_801849C8:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184A0C
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_80184A0C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80184A20
    @L_80184A0C:
    addi r30, r30, 0x1
    @L_80184A10:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801849C8
    li r31, 0x0
    @L_80184A20:
    li r30, 0x0
    b @L_80184A50
    @L_80184A28:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184A4C
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80184A4C
    b @L_80184A60
    @L_80184A4C:
    addi r30, r30, 0x1
    @L_80184A50:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80184A28
    li r3, 0x0
    @L_80184A60:
    cmplwi r3, 0x0
    beq @L_80184A6C
    stfs f31, 0x58(r3)
    @L_80184A6C:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    lwz r29, 0xc(r1)
    lwz r28, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
