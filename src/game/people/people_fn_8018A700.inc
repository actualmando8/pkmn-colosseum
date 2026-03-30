    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stmw r25, 0x34(r1)
    lfs f0, lbl_8047D800(r2)
    addic. r0, r1, 0x8
    mr r27, r3
    mr r28, r4
    fmuls f31, f0, f1
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_8018A838
    li r26, 0x0
    b @L_8018A77C
    @L_8018A744:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A778
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_8018A778
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_8018A778
    lwz r26, 0x4(r3)
    b @L_8018A7E8
    @L_8018A778:
    addi r26, r26, 0x1
    @L_8018A77C:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A744
    li r25, 0x0
    b @L_8018A7D8
    @L_8018A790:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A7D4
    lwz r0, 0x2c(r26)
    cmplw r0, r28
    bne @L_8018A7D4
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_8018A7E8
    @L_8018A7D4:
    addi r25, r25, 0x1
    @L_8018A7D8:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A790
    li r26, 0x0
    @L_8018A7E8:
    li r25, 0x0
    b @L_8018A818
    @L_8018A7F0:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A814
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018A814
    b @L_8018A828
    @L_8018A814:
    addi r25, r25, 0x1
    @L_8018A818:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A7F0
    li r3, 0x0
    @L_8018A828:
    cmplwi r3, 0x0
    beq @L_8018A838
    addi r4, r1, 0x8
    bl fn_8018FC98
    @L_8018A838:
    addic. r0, r1, 0x14
    beq @L_8018A93C
    li r25, 0x0
    b @L_8018A880
    @L_8018A848:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A87C
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_8018A87C
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018A87C
    lwz r25, 0x4(r3)
    b @L_8018A8EC
    @L_8018A87C:
    addi r25, r25, 0x1
    @L_8018A880:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A848
    li r25, 0x0
    b @L_8018A8DC
    @L_8018A894:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A8D8
    lwz r0, 0x2c(r26)
    cmplw r0, r30
    bne @L_8018A8D8
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r26)
    b @L_8018A8EC
    @L_8018A8D8:
    addi r25, r25, 0x1
    @L_8018A8DC:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A894
    li r25, 0x0
    @L_8018A8EC:
    li r26, 0x0
    b @L_8018A91C
    @L_8018A8F4:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A918
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_8018A918
    b @L_8018A92C
    @L_8018A918:
    addi r26, r26, 0x1
    @L_8018A91C:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A8F4
    li r3, 0x0
    @L_8018A92C:
    cmplwi r3, 0x0
    beq @L_8018A93C
    addi r4, r1, 0x14
    bl fn_8018FC98
    @L_8018A93C:
    addi r3, r1, 0x20
    addi r4, r1, 0x8
    addi r5, r1, 0x14
    bl fn_800E0168
    addi r3, r1, 0x20
    bl fn_800E008C
    lfs f0, lbl_8047D7A0(r2)
    fcmpu cr0, f0, f1
    bne @L_8018A968
    lfs f1, lbl_8047D79C(r2)
    b @L_8018A96C
    @L_8018A968:
    fdivs f1, f31, f1
    @L_8018A96C:
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018AA80
    li r26, 0x0
    b @L_8018A9BC
    @L_8018A984:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A9B8
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_8018A9B8
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_8018A9B8
    lwz r26, 0x4(r3)
    b @L_8018AA28
    @L_8018A9B8:
    addi r26, r26, 0x1
    @L_8018A9BC:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A984
    li r25, 0x0
    b @L_8018AA18
    @L_8018A9D0:
    mr r3, r25
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AA14
    lwz r0, 0x2c(r29)
    cmplw r0, r28
    bne @L_8018AA14
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r29)
    b @L_8018AA28
    @L_8018AA14:
    addi r25, r25, 0x1
    @L_8018AA18:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A9D0
    li r26, 0x0
    @L_8018AA28:
    li r27, 0x0
    b @L_8018AA58
    @L_8018AA30:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AA54
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018AA54
    b @L_8018AA68
    @L_8018AA54:
    addi r27, r27, 0x1
    @L_8018AA58:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AA30
    li r3, 0x0
    @L_8018AA68:
    cmplwi r3, 0x0
    beq @L_8018AAB0
    li r0, 0x0
    stb r0, 0x54(r3)
    stb r0, 0x22(r3)
    b @L_8018AAB0
    @L_8018AA80:
    addi r3, r1, 0x20
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x14
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
    mr r3, r27
    mr r4, r28
    mr r5, r31
    addi r6, r1, 0x14
    bl fn_8018AACC
    @L_8018AAB0:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    lmw r25, 0x34(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
