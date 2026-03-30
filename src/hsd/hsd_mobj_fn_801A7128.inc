    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr. r26, r5
    mr r24, r3
    mr r25, r4
    li r29, 0x0
    li r28, 0x0
    bne @L_801A7160
    la r3, lbl_8047DC18(r2)
    li r4, 0x18b
    la r5, lbl_8047DC38(r2)
    bl fn_80196E10
    @L_801A7160:
    li r0, 0x0
    mr r30, r25
    stw r0, 0x0(r26)
    b @L_801A7188
    @L_801A7170:
    lwz r0, 0x4c(r30)
    clrlwi r0, r0, 28
    cmplwi r0, 0x4
    bne @L_801A7184
    mr r29, r30
    @L_801A7184:
    lwz r30, 0x8(r30)
    @L_801A7188:
    cmplwi r30, 0x0
    bne @L_801A7170
    lwz r0, 0x4(r24)
    clrlwi. r30, r0, 30
    bne @L_801A71A0
    li r30, 0x1
    @L_801A71A0:
    lwz r0, 0x4(r24)
    rlwinm. r27, r0, 0, 17, 18
    bne @L_801A71B0
    slwi r27, r30, 13
    @L_801A71B0:
    mr r3, r26
    bl fn_801B707C
    lwz r0, 0x4(r24)
    mr r31, r3
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801A734C
    cmpwi r30, 0x2
    beq @L_801A71D4
    b @L_801A721C
    @L_801A71D4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x8
    bl fn_801B64EC
    b @L_801A727C
    @L_801A721C:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x4
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    @L_801A727C:
    cmpwi r27, 0x4000
    beq @L_801A7288
    b @L_801A72E8
    @L_801A7288:
    mr r6, r26
    la r3, lbl_80478C88(r13)
    li r4, 0x6
    li r5, 0x0
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B5F08
    b @L_801A75F0
    @L_801A72E8:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x6
    li r5, 0x3
    addi r3, r3, 0xc
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B5F08
    b @L_801A75F0
    @L_801A734C:
    cmpwi r30, 0x2
    beq @L_801A73C8
    bge @L_801A742C
    cmpwi r30, 0x1
    bge @L_801A7364
    b @L_801A742C
    @L_801A7364:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x4
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    b @L_801A74A8
    @L_801A73C8:
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    cmplwi r29, 0x0
    li r0, -0x2
    beq @L_801A7400
    li r0, -0x1
    @L_801A7400:
    stw r0, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    b @L_801A74A8
    @L_801A742C:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x4
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    mr r4, r29
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    cmplwi r29, 0x0
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x1
    li r5, -0x2
    beq @L_801A7490
    li r5, -0x1
    @L_801A7490:
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    @L_801A74A8:
    cmpwi r27, 0x4000
    beq @L_801A7524
    bge @L_801A757C
    cmpwi r27, 0x2000
    beq @L_801A74C0
    b @L_801A757C
    @L_801A74C0:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x6
    li r5, 0x3
    addi r3, r3, 0xc
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x6
    bl fn_801B5F08
    b @L_801A75F0
    @L_801A7524:
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, -0x2
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x5
    bl fn_801B5F08
    b @L_801A75F0
    @L_801A757C:
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x6
    li r5, 0x3
    addi r3, r3, 0xc
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    mr r4, r29
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    mr r9, r30
    li r4, 0x7
    li r5, 0x0
    li r6, 0x5
    li r7, -0x2
    li r8, 0x6
    li r10, 0x7
    bl fn_801B5F08
    @L_801A75F0:
    stw r31, 0x1c(r1)
    mr r30, r25
    stw r31, 0x10(r1)
    b @L_801A7644
    @L_801A7600:
    lwz r0, 0x4c(r30)
    andi. r0, r0, 0x50
    beq @L_801A7640
    lwz r0, 0xc(r30)
    cmpwi r0, 0xff
    beq @L_801A7640
    lwz r4, 0x0(r30)
    mr r3, r30
    mr r5, r28
    mr r8, r26
    lwz r12, 0x44(r4)
    addi r6, r1, 0x1c
    addi r7, r1, 0x10
    li r4, 0x50
    mtctr r12
    bctrl
    @L_801A7640:
    lwz r30, 0x8(r30)
    @L_801A7644:
    cmplwi r30, 0x0
    bne @L_801A7600
    lwz r0, 0x4(r24)
    ori r28, r28, 0x50
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801A7868
    rlwinm. r0, r27, 0, 17, 17
    beq @L_801A770C
    mr r3, r26
    bl fn_801B707C
    li r4, 0x0
    mr r31, r3
    li r5, 0x5
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x1c(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    stw r0, 0x8(r1)
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x5
    li r6, 0x7
    li r7, 0x0
    lwz r5, 0x10(r1)
    li r8, 0x5
    li r9, -0x2
    li r10, 0x7
    bl fn_801B5F08
    stw r31, 0x1c(r1)
    stw r31, 0x10(r1)
    @L_801A770C:
    mr r3, r26
    bl fn_801B707C
    cmplwi r29, 0x0
    mr r31, r3
    beq @L_801A7778
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    lwz r7, 0x1c(r1)
    li r8, 0x1
    li r9, -0x1
    li r10, 0x7
    bl fn_801B64EC
    b @L_801A77CC
    @L_801A7778:
    mr r3, r31
    li r4, 0x0
    li r5, 0x4
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    lwz r7, 0x1c(r1)
    li r8, 0x1
    li r9, -0x2
    li r10, 0x7
    bl fn_801B64EC
    @L_801A77CC:
    rlwinm. r0, r27, 0, 17, 17
    stw r31, 0x1c(r1)
    beq @L_801A7820
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x5
    lwz r7, 0x10(r1)
    li r8, 0x5
    li r9, -0x2
    li r10, 0x7
    bl fn_801B5F08
    b @L_801A7864
    @L_801A7820:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x5
    li r6, 0x7
    li r7, 0x0
    lwz r5, 0x10(r1)
    li r8, 0x5
    li r9, -0x2
    li r10, 0x7
    bl fn_801B5F08
    @L_801A7864:
    stw r31, 0x10(r1)
    @L_801A7868:
    lwz r0, 0x4(r24)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A79E8
    lwz r3, 0xc(r24)
    mr r6, r26
    li r4, 0x1
    li r5, 0x0
    addi r3, r3, 0x8
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_801B707C
    li r4, 0x0
    mr r31, r3
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    stw r30, 0x8(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    stw r31, 0x18(r1)
    mr r30, r25
    b @L_801A792C
    @L_801A78E8:
    lwz r0, 0x4c(r30)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_801A7928
    lwz r0, 0xc(r30)
    cmpwi r0, 0xff
    beq @L_801A7928
    lwz r4, 0x0(r30)
    mr r3, r30
    mr r5, r28
    mr r8, r26
    lwz r12, 0x44(r4)
    addi r6, r1, 0x18
    addi r7, r1, 0x10
    li r4, 0x20
    mtctr r12
    bctrl
    @L_801A7928:
    lwz r30, 0x8(r30)
    @L_801A792C:
    cmplwi r30, 0x0
    bne @L_801A78E8
    mr r3, r26
    ori r28, r28, 0x20
    bl fn_801B707C
    li r4, 0x0
    mr r31, r3
    li r5, 0x5
    bl fn_801B5E40
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r31
    stw r0, 0x8(r1)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    lwz r7, 0x18(r1)
    li r8, 0x1
    li r9, -0x2
    li r10, 0x7
    bl fn_801B64EC
    stw r31, 0x18(r1)
    mr r3, r26
    bl fn_801B707C
    li r4, 0x0
    mr r31, r3
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x1c(r1)
    mr r3, r31
    li r4, 0x1
    li r6, 0x7
    stw r0, 0x8(r1)
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    lwz r5, 0x18(r1)
    li r10, 0x1
    bl fn_801B64EC
    stw r31, 0x1c(r1)
    @L_801A79E8:
    lwz r0, 0x1c(r1)
    mr r30, r25
    stw r0, 0x14(r1)
    b @L_801A7A3C
    @L_801A79F8:
    lwz r0, 0x4c(r30)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801A7A38
    lwz r0, 0xc(r30)
    cmpwi r0, 0xff
    beq @L_801A7A38
    lwz r4, 0x0(r30)
    mr r3, r30
    mr r5, r28
    mr r8, r26
    lwz r12, 0x44(r4)
    addi r6, r1, 0x14
    addi r7, r1, 0x10
    li r4, 0x80
    mtctr r12
    bctrl
    @L_801A7A38:
    lwz r30, 0x8(r30)
    @L_801A7A3C:
    cmplwi r30, 0x0
    bne @L_801A79F8
    lwz r3, 0x14(r1)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    bne @L_801A7A74
    lwz r3, 0x14(r1)
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801A7A74
    lwz r3, 0x10(r1)
    bl fn_801B7C60
    cmpwi r3, 0x1
    beq @L_801A7B0C
    @L_801A7A74:
    mr r3, r26
    bl fn_801B707C
    li r4, 0x0
    mr r31, r3
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6E74
    lwz r0, 0x14(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    stw r0, 0x8(r1)
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x1
    bl fn_801B64EC
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    lwz r0, 0x10(r1)
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    stw r0, 0x8(r1)
    li r6, 0x7
    li r7, 0x0
    li r8, 0x7
    li r9, 0x0
    li r10, 0x5
    bl fn_801B5F08
    mr r3, r31
    b @L_801A7B10
    @L_801A7B0C:
    lwz r3, 0x14(r1)
    @L_801A7B10:
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
