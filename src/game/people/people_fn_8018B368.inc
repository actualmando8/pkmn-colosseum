    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr. r29, r5
    mr r25, r3
    mr r26, r4
    mr r30, r6
    mr r31, r7
    blt @L_8018B544
    li r28, 0x0
    b @L_8018B3D0
    @L_8018B398:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B3CC
    lwz r0, 0x28(r3)
    cmplw r0, r25
    bne @L_8018B3CC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_8018B3CC
    lwz r28, 0x4(r3)
    b @L_8018B43C
    @L_8018B3CC:
    addi r28, r28, 0x1
    @L_8018B3D0:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018B398
    li r27, 0x0
    b @L_8018B42C
    @L_8018B3E4:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B428
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_8018B428
    lis r3, lbl_80273FD8@ha
    mr r4, r25
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018B43C
    @L_8018B428:
    addi r27, r27, 0x1
    @L_8018B42C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B3E4
    li r28, 0x0
    @L_8018B43C:
    li r27, 0x0
    b @L_8018B470
    @L_8018B444:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B46C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018B46C
    mr r28, r3
    b @L_8018B480
    @L_8018B46C:
    addi r27, r27, 0x1
    @L_8018B470:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B444
    li r28, 0x0
    @L_8018B480:
    cmplwi r28, 0x0
    beq @L_8018B544
    mr r3, r28
    bl fn_8018FBD4
    mr. r27, r3
    beq @L_8018B544
    stw r29, 0x48(r28)
    mr r4, r29
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xd8(r28)
    bl fn_800ECCA8
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r27
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r27
    bl fn_800EC9DC
    mr r3, r27
    mr r4, r29
    bl fn_800EC35C
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    mr r3, r27
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f0, f1
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r27
    bl fn_800EC308
    clrlwi. r0, r31, 24
    beq @L_8018B530
    mr r3, r27
    li r4, 0x1
    bl fn_800ECB74
    b @L_8018B53C
    @L_8018B530:
    mr r3, r27
    li r4, 0x0
    bl fn_800ECB74
    @L_8018B53C:
    mr r3, r27
    bl fn_800EC990
    @L_8018B544:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
