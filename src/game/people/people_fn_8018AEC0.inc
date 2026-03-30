    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    li r28, 0x0
    b @L_8018AF24
    @L_8018AEEC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AF20
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_8018AF20
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018AF20
    lwz r28, 0x4(r3)
    b @L_8018AF90
    @L_8018AF20:
    addi r28, r28, 0x1
    @L_8018AF24:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018AEEC
    li r27, 0x0
    b @L_8018AF80
    @L_8018AF38:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AF7C
    lwz r0, 0x2c(r28)
    cmplw r0, r30
    bne @L_8018AF7C
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018AF90
    @L_8018AF7C:
    addi r27, r27, 0x1
    @L_8018AF80:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AF38
    li r28, 0x0
    @L_8018AF90:
    li r27, 0x0
    b @L_8018AFC4
    @L_8018AF98:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AFC0
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018AFC0
    mr r27, r3
    b @L_8018AFD4
    @L_8018AFC0:
    addi r27, r27, 0x1
    @L_8018AFC4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AF98
    li r27, 0x0
    @L_8018AFD4:
    cmplwi r27, 0x0
    bne @L_8018AFE4
    li r3, 0x0
    b @L_8018B060
    @L_8018AFE4:
    mr r3, r27
    bl fn_8018FBD4
    mr. r28, r3
    bne @L_8018AFFC
    li r3, 0x0
    b @L_8018B060
    @L_8018AFFC:
    clrlwi r31, r31, 24
    lfs f31, lbl_8047D79C(r2)
    @L_8018B004:
    lfs f0, 0xd4(r27)
    fcmpu cr0, f31, f0
    bne @L_8018B018
    li r3, 0x0
    b @L_8018B060
    @L_8018B018:
    cmplwi r31, 0x0
    beq @L_8018B05C
    lwz r0, 0x8c(r28)
    cmpwi r0, 0x1
    bne @L_8018B054
    lis r3, lbl_80274008@ha
    lis r4, lbl_8036C510@ha
    addi r3, r3, lbl_80274008@l
    mr r5, r29
    addi r4, r4, lbl_8036C510@l
    mr r6, r30
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8018B060
    @L_8018B054:
    bl fn_800F0308
    b @L_8018B004
    @L_8018B05C:
    li r3, 0x1
    @L_8018B060:
    psq_l f31, 0x28(r1), 0, 0
    lfd f31, 0x20(r1)
    lmw r27, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
