    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x38(r1)
    fmr f31, f1
    stmw r21, 0xc(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    lhz r0, 0x150(r3)
    cmplwi r0, 0x0
    bne @L_800E61F8
    li r0, 0x0
    b @L_800E6228
    @L_800E61F8:
    lwz r3, 0x14c(r28)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_800E6210
    li r0, 0x0
    b @L_800E6228
    @L_800E6210:
    bl fn_800DF240
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800E6224
    li r0, 0x1
    b @L_800E6228
    @L_800E6224:
    li r0, 0x0
    @L_800E6228:
    clrlwi. r0, r0, 24
    bne @L_800E6374
    lhz r0, 0x152(r28)
    cmplwi r0, 0x0
    bne @L_800E631C
    mr r3, r28
    li r24, 0x0
    bl fn_800EE0E8
    mr r26, r3
    li r27, 0x0
    b @L_800E6278
    @L_800E6254:
    mr r3, r28
    mr r4, r27
    bl fn_800EE150
    mr r25, r3
    bl fn_800EE758
    add r24, r24, r3
    mr r3, r25
    bl fn_800EE828
    addi r27, r27, 0x1
    @L_800E6278:
    cmpw r27, r26
    blt @L_800E6254
    cmpwi r24, 0x0
    bne @L_800E6290
    li r24, 0x0
    b @L_800E6314
    @L_800E6290:
    slwi r3, r24, 2
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r27, r3
    beq @L_800E6314
    bl fn_800E27B0
    stw r3, 0x14c(r28)
    mr r23, r3
    li r22, 0x0
    sth r27, 0x154(r28)
    sth r24, 0x150(r28)
    b @L_800E630C
    @L_800E62C0:
    mr r3, r28
    mr r4, r22
    bl fn_800EE150
    mr r21, r3
    bl fn_800EE758
    mr r27, r3
    li r25, 0x0
    b @L_800E62F8
    @L_800E62E0:
    mr r3, r21
    mr r4, r25
    bl fn_800EE6B4
    stw r3, 0x0(r23)
    addi r25, r25, 0x1
    addi r23, r23, 0x4
    @L_800E62F8:
    cmpw r25, r27
    blt @L_800E62E0
    mr r3, r21
    bl fn_800EE828
    addi r22, r22, 0x1
    @L_800E630C:
    cmpw r22, r26
    blt @L_800E62C0
    @L_800E6314:
    mr r22, r24
    b @L_800E6320
    @L_800E631C:
    lhz r22, 0x150(r28)
    @L_800E6320:
    lhz r3, 0x152(r28)
    li r21, 0x0
    addi r0, r3, 0x1
    sth r0, 0x152(r28)
    lwz r23, 0x14c(r28)
    b @L_800E636C
    @L_800E6338:
    lwz r3, 0x0(r23)
    cmplwi r3, 0x0
    beq @L_800E6364
    fmr f1, f31
    mr r4, r29
    mr r5, r30
    mr r6, r31
    bl fn_800DF1D0
    lwz r3, 0x0(r23)
    li r4, 0x4
    bl fn_800DF384
    @L_800E6364:
    addi r21, r21, 0x1
    addi r23, r23, 0x4
    @L_800E636C:
    cmpw r21, r22
    blt @L_800E6338
    @L_800E6374:
    lfd f31, 0x38(r1)
    lmw r21, 0xc(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
