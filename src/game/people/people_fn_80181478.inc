    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r27, r3
    mr r25, r4
    mr r29, r5
    li r28, 0x0
    b @L_801814D4
    @L_8018149C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801814D0
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_801814D0
    lwz r0, 0x2c(r3)
    cmplw r0, r25
    bne @L_801814D0
    lwz r28, 0x4(r3)
    b @L_80181540
    @L_801814D0:
    addi r28, r28, 0x1
    @L_801814D4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018149C
    li r26, 0x0
    b @L_80181530
    @L_801814E8:
    mr r3, r26
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018152C
    lwz r0, 0x2c(r28)
    cmplw r0, r25
    bne @L_8018152C
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r25
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_80181540
    @L_8018152C:
    addi r26, r26, 0x1
    @L_80181530:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_801814E8
    li r28, 0x0
    @L_80181540:
    li r30, 0x0
    b @L_80181574
    @L_80181548:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80181570
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80181570
    mr r28, r3
    b @L_80181584
    @L_80181570:
    addi r30, r30, 0x1
    @L_80181574:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80181548
    li r28, 0x0
    @L_80181584:
    cmplwi r28, 0x0
    bne @L_80181594
    li r3, 0x0
    b @L_8018183C
    @L_80181594:
    lbz r0, 0x54(r28)
    cmplwi r0, 0x0
    bne @L_801815A8
    li r3, 0x1
    b @L_8018183C
    @L_801815A8:
    clrlwi. r0, r29, 24
    beq @L_80181824
    lbz r0, 0x56(r28)
    cmplwi r0, 0x0
    bne @L_8018181C
    li r0, 0x1
    stb r0, 0x56(r28)
    stw r0, 0x90(r28)
    lwz r3, 0x30(r28)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_8018181C
    lwz r0, 0x90(r28)
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r30, 0x14(r1)
    cmpwi r30, -0x1
    beq @L_8018181C
    cmpwi r30, 0x0
    lbz r29, 0x8(r1)
    lwz r26, 0x2c(r28)
    li r31, 0x0
    lwz r27, 0x28(r28)
    blt @L_8018181C
    mr r28, r31
    b @L_80181650
    @L_80181618:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018164C
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_8018164C
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_8018164C
    lwz r26, 0x4(r3)
    b @L_801816BC
    @L_8018164C:
    addi r28, r28, 0x1
    @L_80181650:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80181618
    li r25, 0x0
    b @L_801816AC
    @L_80181664:
    mr r3, r25
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801816A8
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_801816A8
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r28)
    b @L_801816BC
    @L_801816A8:
    addi r25, r25, 0x1
    @L_801816AC:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80181664
    li r26, 0x0
    @L_801816BC:
    li r27, 0x0
    b @L_801816F0
    @L_801816C4:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801816EC
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_801816EC
    mr r26, r3
    b @L_80181700
    @L_801816EC:
    addi r27, r27, 0x1
    @L_801816F0:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_801816C4
    li r26, 0x0
    @L_80181700:
    cmplwi r26, 0x0
    beq @L_8018181C
    mr r3, r26
    bl fn_8018FBD4
    mr. r28, r3
    beq @L_8018181C
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_8018172C
    li r31, 0x1
    b @L_80181770
    @L_8018172C:
    mr r3, r28
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_80181744
    li r31, 0x1
    b @L_80181770
    @L_80181744:
    mr r3, r28
    addi r4, r1, 0xc
    addi r5, r1, 0x10
    bl fn_800EC578
    lwz r0, 0xc(r1)
    cmpw r0, r30
    bne @L_8018176C
    lwz r0, 0x10(r1)
    cmpwi r0, -0x1
    beq @L_80181770
    @L_8018176C:
    li r31, 0x1
    @L_80181770:
    clrlwi. r0, r31, 24
    beq @L_801817F8
    stw r30, 0x48(r26)
    mr r3, r28
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r30
    stfs f0, 0xd8(r26)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    mr r4, r30
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC308
    cmplwi r29, 0x0
    beq @L_801817E4
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_801817F0
    @L_801817E4:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    @L_801817F0:
    mr r3, r28
    bl fn_800EC990
    @L_801817F8:
    cmplwi r29, 0x0
    beq @L_80181810
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_8018181C
    @L_80181810:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    @L_8018181C:
    li r3, 0x1
    b @L_8018183C
    @L_80181824:
    lbz r0, 0x56(r28)
    cmplwi r0, 0x0
    beq @L_80181838
    li r0, 0x0
    stb r0, 0x56(r28)
    @L_80181838:
    li r3, 0x0
    @L_8018183C:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
