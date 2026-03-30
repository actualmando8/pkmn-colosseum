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
    blt @L_8018B588
    cmpwi r30, 0x0
    bge @L_8018B590
    @L_8018B588:
    li r3, 0x0
    b @L_8018B758
    @L_8018B590:
    cmplwi r31, 0x1
    bge @L_8018B5A0
    li r3, 0x0
    b @L_8018B758
    @L_8018B5A0:
    li r28, 0x0
    b @L_8018B5E0
    @L_8018B5A8:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B5DC
    lwz r0, 0x28(r3)
    cmplw r0, r25
    bne @L_8018B5DC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_8018B5DC
    lwz r28, 0x4(r3)
    b @L_8018B64C
    @L_8018B5DC:
    addi r28, r28, 0x1
    @L_8018B5E0:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018B5A8
    li r27, 0x0
    b @L_8018B63C
    @L_8018B5F4:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B638
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_8018B638
    lis r3, lbl_80273FD8@ha
    mr r4, r25
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018B64C
    @L_8018B638:
    addi r27, r27, 0x1
    @L_8018B63C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B5F4
    li r28, 0x0
    @L_8018B64C:
    li r27, 0x0
    b @L_8018B680
    @L_8018B654:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B67C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018B67C
    mr r28, r3
    b @L_8018B690
    @L_8018B67C:
    addi r27, r27, 0x1
    @L_8018B680:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B654
    li r28, 0x0
    @L_8018B690:
    cmplwi r28, 0x0
    bne @L_8018B6A0
    li r3, 0x0
    b @L_8018B758
    @L_8018B6A0:
    mr r3, r28
    bl fn_8018FBD4
    mr. r27, r3
    bne @L_8018B6B8
    li r3, 0x0
    b @L_8018B758
    @L_8018B6B8:
    lis r0, 0x4330
    stw r31, 0x14(r1)
    lfd f1, lbl_8047D7D0(r2)
    mr r4, r29
    stw r0, 0x10(r1)
    mr r5, r30
    lfs f2, lbl_8047D79C(r2)
    lfd f0, 0x10(r1)
    stw r30, 0x48(r28)
    fsubs f1, f0, f1
    lfs f0, lbl_8047D7A0(r2)
    fdivs f1, f2, f1
    stfs f1, 0xd8(r28)
    stfs f0, 0xd4(r28)
    bl fn_800EC5FC
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r27
    bl fn_800ECA78
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r27
    bl fn_800EC9DC
    lfs f1, 0xd4(r28)
    mr r3, r27
    bl fn_800EC5B8
    mr r3, r27
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r27
    bl fn_800EC990
    mr r3, r27
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    lfs f1, 0x8(r1)
    mr r3, r27
    lfs f0, lbl_8047D834(r2)
    lfs f2, lbl_8047D7A0(r2)
    fsubs f1, f1, f0
    bl fn_800EC8C8
    li r3, 0x1
    @L_8018B758:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
