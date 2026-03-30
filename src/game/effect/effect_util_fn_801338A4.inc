    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    subi r27, r29, 0x1
    li r26, 0x0
    lwz r0, lbl_80478848(r13)
    add r28, r0, r29
    @L_801338C8:
    lwz r0, lbl_80478848(r13)
    srawi r3, r29, 31
    add r0, r0, r27
    andc r25, r0, r3
    mr r3, r25
    bl fn_80133C3C
    lwz r0, lbl_8047AEDC(r13)
    slwi r5, r3, 2
    mr r3, r28
    mr r4, r25
    add r30, r0, r5
    li r6, 0x0
    mr r5, r30
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    mr r25, r3
    mr r3, r28
    bl fn_80104704
    mr. r31, r3
    beq @L_8013393C
    lbz r3, 0x94(r31)
    lbz r0, 0x95(r31)
    extsb r3, r3
    extsb r0, r0
    add r0, r3, r0
    stw r0, 0x0(r30)
    b @L_80133944
    @L_8013393C:
    li r0, 0x0
    stw r0, 0x0(r30)
    @L_80133944:
    cmpwi r25, -0x1
    bne @L_8013395C
    cmpwi r29, 0x0
    bne @L_80133B30
    li r26, -0x1
    b @L_80133B30
    @L_8013395C:
    lbz r4, 0x94(r31)
    mr r3, r31
    lbz r0, 0x95(r31)
    extsb r4, r4
    extsb r0, r0
    add r4, r4, r0
    bl fn_80133E6C
    mr. r30, r3
    ble @L_801339A4
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80133994
    li r3, 0x0
    b @L_8013399C
    @L_80133994:
    mtctr r12
    bctrl
    @L_8013399C:
    cmpw r3, r30
    bgt @L_801339AC
    @L_801339A4:
    li r25, 0x0
    b @L_80133A14
    @L_801339AC:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_801339C0
    li r3, 0x0
    b @L_801339CC
    @L_801339C0:
    mr r3, r30
    mtctr r12
    bctrl
    @L_801339CC:
    cmplwi r3, 0x0
    bne @L_801339DC
    li r25, 0x0
    b @L_801339E0
    @L_801339DC:
    lha r25, 0x2(r3)
    @L_801339E0:
    extsh. r0, r25
    ble @L_80133A10
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_801339FC
    li r3, 0x0
    b @L_80133A04
    @L_801339FC:
    mtctr r12
    bctrl
    @L_80133A04:
    extsh r0, r25
    cmpw r3, r0
    bgt @L_80133A14
    @L_80133A10:
    li r25, 0x0
    @L_80133A14:
    extsh. r0, r25
    beq @L_80133AC4
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133A30
    li r3, 0x0
    b @L_80133A3C
    @L_80133A30:
    mr r3, r30
    mtctr r12
    bctrl
    @L_80133A3C:
    cmplwi r3, 0x0
    bne @L_80133A4C
    li r12, 0x0
    b @L_80133A50
    @L_80133A4C:
    lwz r12, 0x8(r3)
    @L_80133A50:
    cmplwi r12, 0x0
    beq @L_80133A7C
    lbz r4, 0x94(r31)
    mr r3, r30
    lbz r0, 0x95(r31)
    extsb r4, r4
    extsb r0, r0
    add r4, r4, r0
    mtctr r12
    bctrl
    b @L_80133A80
    @L_80133A7C:
    li r3, 0x1
    @L_80133A80:
    cmpwi r3, 0x0
    bne @L_80133A98
    lwz r3, lbl_80478848(r13)
    bl fn_80102510
    li r3, 0x1
    b @L_80133B3C
    @L_80133A98:
    cmpwi r3, -0x1
    beq @L_801338C8
    extsh r0, r25
    cmpwi r0, 0x1
    beq @L_801338C8
    addi r3, r29, 0x1
    bl fn_801338A4
    cmpwi r3, 0x1
    bne @L_801338C8
    li r3, 0x1
    b @L_80133B3C
    @L_80133AC4:
    lwz r3, lbl_80478848(r13)
    bl fn_80102510
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133AE0
    li r3, 0x0
    b @L_80133AEC
    @L_80133AE0:
    mr r3, r30
    mtctr r12
    bctrl
    @L_80133AEC:
    cmplwi r3, 0x0
    bne @L_80133AFC
    li r12, 0x0
    b @L_80133B00
    @L_80133AFC:
    lwz r12, 0x8(r3)
    @L_80133B00:
    cmplwi r12, 0x0
    beq @L_80133B28
    lbz r4, 0x94(r31)
    mr r3, r30
    lbz r0, 0x95(r31)
    extsb r4, r4
    extsb r0, r0
    add r4, r4, r0
    mtctr r12
    bctrl
    @L_80133B28:
    li r3, 0x1
    b @L_80133B3C
    @L_80133B30:
    mr r3, r28
    bl fn_80102510
    mr r3, r26
    @L_80133B3C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
