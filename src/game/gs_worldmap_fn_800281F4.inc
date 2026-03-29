    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r24, 0x60(r1)
    mr r25, r3
    mr r26, r5
    mr r27, r6
    mr r28, r7
    addi r3, r1, 0x8
    li r29, 0x0
    bl fn_800F9E70
    li r24, 0x0
    lfs f0, lbl_8047B930(r2)
    sth r24, 0x0(r25)
    la r12, lbl_8047A3D4(r13)
    la r11, lbl_8047A3D0(r13)
    la r10, lbl_8047A3CC(r13)
    la r9, lbl_8047A3C8(r13)
    la r8, lbl_8047A3C4(r13)
    la r7, lbl_8047A3C0(r13)
    la r6, lbl_8047A3BC(r13)
    la r5, lbl_8047A3B8(r13)
    la r4, lbl_8047A3B4(r13)
    la r0, lbl_8047A3B0(r13)
    stw r25, 0x20(r1)
    li r3, 0x0
    stw r26, 0x24(r1)
    stw r27, 0x28(r1)
    stw r24, lbl_8047A3D4(r13)
    stw r12, 0x2c(r1)
    stw r24, lbl_8047A3D0(r13)
    stw r11, 0x30(r1)
    stw r24, lbl_8047A3CC(r13)
    stw r10, 0x34(r1)
    stfs f0, lbl_8047A3C8(r13)
    stw r9, 0x38(r1)
    stw r24, lbl_8047A3C4(r13)
    stw r8, 0x3c(r1)
    stw r7, 0x40(r1)
    stw r6, 0x44(r1)
    stw r5, 0x48(r1)
    stw r4, 0x4c(r1)
    stw r0, 0x50(r1)
    bl fn_801337A8
    addi r26, r1, 0x8
    b @L_800283E8
    @L_800282AC:
    bl fn_801046B8
    mr r4, r3
    mr r9, r26
    li r3, 0x6e
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r24, 0x20(r1)
    li r25, 0x0
    mr r3, r24
    b @L_800282EC
    @L_800282E4:
    addi r3, r3, 0x2
    addi r25, r25, 0x1
    @L_800282EC:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800282E4
    cmpwi r25, 0x0
    bgt @L_80028308
    li r0, 0x0
    b @L_80028350
    @L_80028308:
    li r3, 0x2ef9
    bl fn_800FA280
    li r5, 0x0
    mtctr r25
    cmpwi r25, 0x0
    ble @L_8002833C
    @L_80028320:
    lhz r4, 0x0(r24)
    lhz r0, 0x0(r3)
    cmplw r4, r0
    bne @L_8002833C
    addi r5, r5, 0x1
    addi r24, r24, 0x2
    bdnz @L_80028320
    @L_8002833C:
    cmpw r5, r25
    blt @L_8002834C
    li r0, 0x0
    b @L_80028350
    @L_8002834C:
    li r0, 0x1
    @L_80028350:
    cmpwi r0, 0x0
    beq @L_80028360
    lwz r31, 0x20(r1)
    b @L_80028364
    @L_80028360:
    addi r31, r1, 0x8
    @L_80028364:
    li r3, 0x440
    bl fn_80166A28
    mr r4, r31
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2ef6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r24, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r24, 0x1
    beq @L_800283BC
    cmpwi r24, -0x1
    bne @L_800283C4
    @L_800283BC:
    li r0, 0x0
    b @L_800283C8
    @L_800283C4:
    li r0, 0x1
    @L_800283C8:
    cmpwi r28, 0x0
    mr r30, r0
    bne @L_800283DC
    li r29, 0x1
    b @L_800283E8
    @L_800283DC:
    cmpwi r0, 0x0
    beq @L_800283E8
    li r29, 0x1
    @L_800283E8:
    cmpwi r29, 0x0
    beq @L_800282AC
    li r3, 0x1
    bl fn_801337A8
    li r3, 0x6e
    bl fn_80102510
    li r3, 0x6e
    li r4, 0x1
    bl fn_80102428
    cmpwi r30, 0x0
    beq @L_8002842C
    lis r3, lbl_803A2068@ha
    mr r4, r31
    addi r3, r3, lbl_803A2068@l
    bl fn_800F9E70
    li r3, 0x1
    b @L_80028430
    @L_8002842C:
    li r3, 0x0
    @L_80028430:
    lmw r24, 0x60(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
