    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    mr r27, r3
    mr r22, r5
    mulli r23, r4, 0x4c
    lis r3, lbl_80266918@ha
    li r24, 0x0
    addi r0, r3, lbl_80266918@l
    add r3, r0, r23
    li r31, 0x8
    lwz r0, 0x4(r3)
    li r30, 0x0
    cmpwi r0, 0x0
    blt @L_800153D4
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xa
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800153EC
    @L_800153D4:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xa
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800153EC:
    li r29, 0x0
    mr r26, r3
    mr r25, r29
    b @L_8001541C
    @L_800153FC:
    mr r3, r26
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015414
    addi r29, r29, 0x1
    @L_80015414:
    addi r25, r25, 0x1
    addi r26, r26, 0x4
    @L_8001541C:
    lhz r0, 0xa(r1)
    cmpw r25, r0
    blt @L_800153FC
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D0(r13)
    lbz r25, 0x0(r22)
    fcmpu cr0, f0, f1
    extsb r25, r25
    beq @L_80015470
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    beq @L_80015470
    fcmpo cr0, f1, f0
    bge @L_80015460
    subi r25, r25, 0x1
    li r24, -0x1
    b @L_80015464
    @L_80015460:
    li r31, 0x9
    @L_80015464:
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    @L_80015470:
    lis r3, lbl_80266918@ha
    mr r26, r24
    addi r0, r3, lbl_80266918@l
    mr r22, r27
    add r3, r0, r23
    mulli r28, r24, 0x1f
    addi r23, r3, 0x4
    b @L_80015568
    @L_80015490:
    cmpwi r25, 0x0
    blt @L_8001555C
    lwz r0, 0x0(r23)
    subf r24, r30, r28
    cmpwi r0, 0x0
    blt @L_800154C8
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800154E0
    @L_800154C8:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800154E0:
    mr r19, r3
    li r21, -0x1
    li r20, 0x0
    b @L_80015524
    @L_800154F0:
    mr r3, r19
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001551C
    addi r21, r21, 0x1
    cmpw r21, r25
    blt @L_8001551C
    mr r3, r19
    bl fn_80143C68
    b @L_80015534
    @L_8001551C:
    addi r20, r20, 0x1
    addi r19, r19, 0x4
    @L_80015524:
    lhz r0, 0x8(r1)
    cmpw r20, r0
    blt @L_800154F0
    li r3, 0x0
    @L_80015534:
    clrlwi r3, r3, 16
    bl fn_801440A0
    bl fn_80144088
    mr r6, r3
    cmplwi r6, 0x0
    beq @L_8001555C
    mr r3, r27
    mr r4, r24
    li r5, -0x1
    bl fn_800FB680
    @L_8001555C:
    addi r28, r28, 0x1f
    addi r26, r26, 0x1
    addi r25, r25, 0x1
    @L_80015568:
    cmpw r26, r31
    bge @L_80015578
    cmpw r25, r29
    blt @L_80015490
    @L_80015578:
    cmpw r26, r31
    bge @L_80015598
    mulli r0, r26, 0x1f
    mr r3, r22
    li r5, -0x1
    li r6, 0x2afe
    subf r4, r30, r0
    bl fn_800FB680
    @L_80015598:
    li r3, 0x0
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
