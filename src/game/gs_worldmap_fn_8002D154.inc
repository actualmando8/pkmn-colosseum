    nofralloc
    stwu r1, -0x7f0(r1)
    mflr r0
    stw r0, 0x7f4(r1)
    stmw r22, 0x7c8(r1)
    mr r23, r3
    mr r24, r4
    clrlwi r0, r24, 24
    lis r3, lbl_80266E70@ha
    mulli r4, r0, 0x3
    slwi r29, r23, 2
    addi r0, r3, lbl_80266E70@l
    add r26, r0, r4
    addi r28, r26, 0x1
    addi r27, r26, 0x2
    @L_8002D18C:
    lwz r0, lbl_804788A8(r13)
    cmpwi r0, 0x0
    beq @L_8002D214
    lis r3, lbl_802E4F68@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_802E4F68@l
    li r0, 0x0
    lwz r5, 0x0(r3)
    addi r9, r4, lbl_802EF0A8@l
    lwz r7, 0x8(r3)
    lwz r6, 0x10(r3)
    mulli r8, r5, 0x1c
    lwz r5, 0x18(r3)
    lwz r4, 0x20(r3)
    mulli r7, r7, 0x1c
    stw r0, lbl_804788A8(r13)
    add r8, r9, r8
    lha r8, 0x4(r8)
    mulli r0, r6, 0x1c
    add r6, r9, r7
    sth r8, 0x4(r3)
    lha r7, 0x4(r6)
    mulli r5, r5, 0x1c
    add r6, r9, r0
    sth r7, 0xc(r3)
    lha r6, 0x4(r6)
    mulli r0, r4, 0x1c
    add r4, r9, r5
    sth r6, 0x14(r3)
    lha r5, 0x4(r4)
    add r4, r9, r0
    lha r0, 0x4(r4)
    sth r5, 0x1c(r3)
    sth r0, 0x24(r3)
    @L_8002D214:
    li r7, 0x0
    lwz r3, lbl_80478E54(r13)
    la r4, lbl_8047A3F8(r13)
    sth r7, lbl_8047A3F8(r13)
    addi r0, r3, 0x2
    lwz r3, lbl_80478E44(r13)
    stw r4, 0x30(r1)
    lhzx r0, r29, r0
    slwi r0, r0, 1
    add r8, r3, r0
    mr r3, r8
    b @L_8002D24C
    @L_8002D244:
    addi r3, r3, 0x2
    addi r7, r7, 0x1
    @L_8002D24C:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002D244
    la r6, lbl_8047A3F4(r13)
    la r5, lbl_8047A3F0(r13)
    la r4, lbl_8047A3EC(r13)
    la r3, lbl_8047A3E8(r13)
    li r0, 0x0
    stw r7, 0x38(r1)
    stw r8, 0x34(r1)
    stw r6, 0x3c(r1)
    stw r5, 0x40(r1)
    stw r4, 0x44(r1)
    stw r3, 0x48(r1)
    stb r24, 0x4c(r1)
    stb r0, 0x4d(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x30
    li r3, 0x60
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8002D2C4
    li r31, 0x0
    b @L_8002D2CC
    @L_8002D2C4:
    lwz r3, 0x30(r1)
    lhz r31, 0x0(r3)
    @L_8002D2CC:
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    beq @L_8002D5B0
    mr r3, r31
    bl fn_801440A0
    cmplwi r3, 0x0
    beq @L_8002D18C
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    bne @L_8002D328
    li r3, 0x0
    li r4, 0x21e
    bl fn_80129B2C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8002D328
    mr r3, r23
    li r4, 0x8
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D18C
    @L_8002D328:
    mr r3, r31
    bl fn_801440A0
    bl fn_80143FFC
    clrlwi r22, r3, 16
    cmpwi r22, 0x0
    ble @L_8002D364
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    divw r22, r3, r22
    cmpwi r22, 0x63
    ble @L_8002D368
    li r22, 0x63
    b @L_8002D368
    @L_8002D364:
    li r22, 0x63
    @L_8002D368:
    cmpwi r22, 0x0
    bgt @L_8002D388
    mr r3, r23
    li r4, 0x5
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D18C
    @L_8002D388:
    mr r3, r31
    bl fn_801440A0
    bl fn_80143FFC
    clrlwi r25, r31, 16
    clrlwi r30, r3, 16
    mr r4, r23
    addi r3, r1, 0x10
    mr r7, r25
    li r5, 0xc
    li r6, 0x2d
    li r8, -0x1
    crxor 6, 6, 6
    bl fn_8002A0B8
    cmpwi r22, 0x1
    stw r3, lbl_8047A3E4(r13)
    bge @L_8002D3D0
    li r22, 0x0
    b @L_8002D464
    @L_8002D3D0:
    lbz r5, 0x0(r26)
    li r7, 0x1
    lbz r4, 0x0(r28)
    la r6, lbl_8047A3E0(r13)
    lbz r3, 0x0(r27)
    li r0, 0x0
    stw r7, 0x18(r1)
    stw r22, 0x1c(r1)
    stw r30, 0x20(r1)
    stw r7, lbl_8047A3E0(r13)
    stw r6, 0x24(r1)
    stb r5, 0x28(r1)
    stb r4, 0x29(r1)
    stb r3, 0x2a(r1)
    stw r0, 0x2c(r1)
    stw r7, 0x14(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x14
    addi r9, r1, 0x18
    li r3, 0x61
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r22, r3
    li r3, 0x61
    bl fn_80102510
    li r3, 0x61
    li r4, 0x1
    bl fn_80102428
    cmpwi r22, -0x1
    bne @L_8002D460
    li r22, -0x1
    b @L_8002D464
    @L_8002D460:
    lwz r22, lbl_8047A3E0(r13)
    @L_8002D464:
    cmpwi r22, 0x0
    blt @L_8002D18C
    mullw r30, r22, r30
    li r0, -0x1
    mr r4, r23
    mr r7, r25
    mr r9, r22
    addi r3, r1, 0x10
    stw r30, 0x8(r1)
    li r5, 0x3
    li r6, 0x2d
    li r8, 0x2f
    stw r0, 0xc(r1)
    li r10, 0x4b
    crxor 6, 6, 6
    bl fn_8002A0B8
    lbz r7, 0x10(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r25, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r25, 0x1
    beq @L_8002D18C
    cmpwi r25, -0x1
    beq @L_8002D18C
    mr r4, r31
    li r3, 0x0
    bl fn_801298B8
    cmpw r3, r22
    bge @L_8002D518
    mr r3, r23
    li r4, 0x6
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D18C
    @L_8002D518:
    li r3, 0x3cb
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    mr r4, r30
    li r3, 0x0
    bl fn_80129474
    mr r4, r31
    clrlwi r5, r22, 16
    li r3, 0x0
    li r6, -0x1
    bl fn_80129A78
    mr r3, r23
    li r4, 0x4
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    clrlwi r0, r31, 16
    cmplwi r0, 0x4
    bne @L_8002D18C
    cmpwi r22, 0xa
    blt @L_8002D18C
    li r3, 0x0
    li r4, 0xc
    bl fn_801298B8
    cmpwi r3, 0x1
    blt @L_8002D18C
    mr r3, r23
    li r4, 0x7
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    li r3, 0x0
    li r4, 0xc
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    b @L_8002D18C
    @L_8002D5B0:
    li r3, 0x60
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lmw r22, 0x7c8(r1)
    lwz r0, 0x7f4(r1)
    mtlr r0
    addi r1, r1, 0x7f0
    blr
