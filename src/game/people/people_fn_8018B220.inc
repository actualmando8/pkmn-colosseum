    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    b @L_8018B284
    @L_8018B24C:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B280
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_8018B280
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018B280
    lwz r31, 0x4(r3)
    b @L_8018B2F0
    @L_8018B280:
    addi r31, r31, 0x1
    @L_8018B284:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_8018B24C
    li r30, 0x0
    b @L_8018B2E0
    @L_8018B298:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B2DC
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_8018B2DC
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_8018B2F0
    @L_8018B2DC:
    addi r30, r30, 0x1
    @L_8018B2E0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018B298
    li r31, 0x0
    @L_8018B2F0:
    li r30, 0x0
    b @L_8018B320
    @L_8018B2F8:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B31C
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_8018B31C
    b @L_8018B330
    @L_8018B31C:
    addi r30, r30, 0x1
    @L_8018B320:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018B2F8
    li r3, 0x0
    @L_8018B330:
    cmplwi r3, 0x0
    beq @L_8018B348
    bl fn_8018FBD4
    cmplwi r3, 0x0
    beq @L_8018B348
    bl fn_800EC96C
    @L_8018B348:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
