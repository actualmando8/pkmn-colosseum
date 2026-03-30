    nofralloc
    stwu r1, -0x1b0(r1)
    mflr r0
    stw r0, 0x1b4(r1)
    stmw r22, 0x188(r1)
    mr. r23, r3
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r27, r7
    mr r28, r8
    beq @L_800EA5F8
    lwz r4, 0x14(r23)
    rlwinm. r0, r4, 0, 19, 19
    beq @L_800EA1CC
    rlwinm. r0, r4, 0, 27, 27
    bne @L_800EA5F8
    cmplwi r23, 0x0
    beq @L_800E9F18
    bne @L_800E9EEC
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800E9EEC:
    lwz r4, 0x14(r23)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E9F08
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E9F08
    li r3, 0x1
    @L_800E9F08:
    cmpwi r3, 0x0
    beq @L_800E9F18
    mr r3, r23
    bl fn_8019D9DC
    @L_800E9F18:
    lwz r29, 0x10(r23)
    cmplwi r29, 0x0
    beq @L_800E9F64
    bne @L_800E9F38
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800E9F38:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E9F54
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E9F54
    li r3, 0x1
    @L_800E9F54:
    cmpwi r3, 0x0
    beq @L_800E9F64
    mr r3, r29
    bl fn_8019D9DC
    @L_800E9F64:
    lwz r3, 0x10(r23)
    addi r4, r1, 0x158
    addi r3, r3, 0x44
    bl fn_800A2EB4
    addi r4, r1, 0x158
    addi r3, r23, 0x44
    mr r5, r4
    bl fn_800A2D98
    cmplwi r24, 0x0
    beq @L_800E9F9C
    addi r4, r1, 0x158
    mr r3, r24
    mr r5, r4
    bl fn_800A2D98
    @L_800E9F9C:
    lwz r23, 0x10(r23)
    cmplwi r23, 0x0
    beq @L_800EA5F8
    lwz r3, 0x14(r23)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA0B8
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA5F8
    mr r3, r23
    bl fn_800EA664
    lwz r3, 0x10(r23)
    bl fn_800EA664
    lwz r3, 0x10(r23)
    addi r4, r1, 0x128
    addi r3, r3, 0x44
    bl fn_800A2EB4
    addi r4, r1, 0x128
    addi r3, r23, 0x44
    mr r5, r4
    bl fn_800A2D98
    addi r4, r1, 0x128
    addi r3, r1, 0x158
    mr r5, r4
    bl fn_800A2D98
    lwz r23, 0x10(r23)
    cmplwi r23, 0x0
    beq @L_800EA5F8
    lwz r3, 0x14(r23)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA04C
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA5F8
    mr r4, r23
    addi r3, r1, 0x128
    addi r5, r1, 0x98
    bl fn_800EA6D4
    lwz r3, 0x10(r23)
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x98
    bl fn_800E9E90
    b @L_800EA5F8
    @L_800EA04C:
    slwi r0, r25, 18
    and. r0, r3, r0
    beq @L_800EA074
    mr r3, r23
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x128
    bl fn_800EA7E4
    @L_800EA074:
    lwz r3, 0x14(r23)
    slwi r0, r25, 28
    and. r0, r3, r0
    beq @L_800EA5F8
    lwz r22, 0x10(r23)
    b @L_800EA0AC
    @L_800EA08C:
    mr r3, r22
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x128
    bl fn_800E9E90
    lwz r22, 0x8(r22)
    @L_800EA0AC:
    cmplwi r22, 0x0
    bne @L_800EA08C
    b @L_800EA5F8
    @L_800EA0B8:
    slwi r31, r25, 18
    and. r0, r3, r31
    beq @L_800EA0F8
    cmplwi r23, 0x0
    beq @L_800EA0F8
    andi. r0, r3, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EA0F8
    mr r3, r23
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x158
    bl fn_800EA820
    @L_800EA0F8:
    lwz r0, 0x14(r23)
    slwi r22, r25, 28
    and. r0, r0, r22
    beq @L_800EA5F8
    lwz r24, 0x10(r23)
    b @L_800EA1C0
    @L_800EA110:
    cmplwi r24, 0x0
    beq @L_800EA1BC
    lwz r3, 0x14(r24)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA15C
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA1BC
    mr r4, r24
    addi r3, r1, 0x158
    addi r5, r1, 0x68
    bl fn_800EA6D4
    lwz r3, 0x10(r24)
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x68
    bl fn_800E9E90
    b @L_800EA1BC
    @L_800EA15C:
    and. r0, r3, r31
    beq @L_800EA180
    mr r3, r24
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x158
    bl fn_800EA7E4
    @L_800EA180:
    lwz r0, 0x14(r24)
    and. r0, r0, r22
    beq @L_800EA1BC
    lwz r23, 0x10(r24)
    b @L_800EA1B4
    @L_800EA194:
    mr r3, r23
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x158
    bl fn_800E9E90
    lwz r23, 0x8(r23)
    @L_800EA1B4:
    cmplwi r23, 0x0
    bne @L_800EA194
    @L_800EA1BC:
    lwz r24, 0x8(r24)
    @L_800EA1C0:
    cmplwi r24, 0x0
    bne @L_800EA110
    b @L_800EA5F8
    @L_800EA1CC:
    slwi r29, r25, 18
    and. r30, r4, r29
    beq @L_800EA3B4
    cmplwi r23, 0x0
    beq @L_800EA3B4
    andi. r0, r4, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EA3B4
    rlwinm. r0, r4, 0, 27, 27
    mr r31, r24
    bne @L_800EA3B4
    cmplwi r30, 0x0
    beq @L_800EA3B4
    cmplwi r23, 0x0
    beq @L_800EA220
    bl fn_800EA60C
    cmpwi r3, 0x0
    beq @L_800EA220
    mr r3, r23
    bl fn_8019D9DC
    @L_800EA220:
    cmplwi r24, 0x0
    bne @L_800EA23C
    lis r3, lbl_804016A0@ha
    addi r3, r3, lbl_804016A0@l
    bl fn_800E064C
    lis r3, lbl_804016A0@ha
    addi r31, r3, lbl_804016A0@l
    @L_800EA23C:
    mr r3, r23
    mr r4, r31
    addi r5, r1, 0xf8
    bl fn_80197B6C
    rlwinm. r0, r30, 0, 13, 13
    beq @L_800EA2C4
    mr r3, r23
    bl fn_8019F024
    li r3, 0x0
    li r4, 0x0
    bl fn_801AB63C
    lwz r22, 0x18(r23)
    b @L_800EA2AC
    @L_800EA270:
    lwz r3, 0x14(r22)
    clrlwi. r0, r3, 31
    bne @L_800EA2A8
    rlwinm. r0, r3, 0, 30, 30
    beq @L_800EA2A8
    mr r3, r22
    bl fn_80199704
    mr r3, r22
    mr r4, r31
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r5, r1, 0xf8
    bl fn_800EACD0
    @L_800EA2A8:
    lwz r22, 0x4(r22)
    @L_800EA2AC:
    cmplwi r22, 0x0
    bne @L_800EA270
    li r3, 0x0
    bl fn_80199704
    li r3, 0x0
    bl fn_8019F024
    @L_800EA2C4:
    rlwinm. r0, r30, 0, 11, 11
    beq @L_800EA33C
    mr r3, r23
    bl fn_8019F024
    li r3, 0x0
    li r4, 0x0
    bl fn_801AB63C
    lwz r22, 0x18(r23)
    b @L_800EA324
    @L_800EA2E8:
    lwz r3, 0x14(r22)
    clrlwi. r0, r3, 31
    bne @L_800EA320
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800EA320
    mr r3, r22
    bl fn_80199704
    mr r3, r22
    mr r4, r31
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r5, r1, 0xf8
    bl fn_800EACD0
    @L_800EA320:
    lwz r22, 0x4(r22)
    @L_800EA324:
    cmplwi r22, 0x0
    bne @L_800EA2E8
    li r3, 0x0
    bl fn_80199704
    li r3, 0x0
    bl fn_8019F024
    @L_800EA33C:
    rlwinm. r0, r30, 0, 12, 12
    beq @L_800EA3B4
    mr r3, r23
    bl fn_8019F024
    li r3, 0x0
    li r4, 0x0
    bl fn_801AB63C
    lwz r22, 0x18(r23)
    b @L_800EA39C
    @L_800EA360:
    lwz r3, 0x14(r22)
    clrlwi. r0, r3, 31
    bne @L_800EA398
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800EA398
    mr r3, r22
    bl fn_80199704
    mr r3, r22
    mr r4, r31
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r5, r1, 0xf8
    bl fn_800EACD0
    @L_800EA398:
    lwz r22, 0x4(r22)
    @L_800EA39C:
    cmplwi r22, 0x0
    bne @L_800EA360
    li r3, 0x0
    bl fn_80199704
    li r3, 0x0
    bl fn_8019F024
    @L_800EA3B4:
    lwz r0, 0x14(r23)
    slwi r30, r25, 28
    and. r0, r0, r30
    beq @L_800EA5F8
    lwz r23, 0x10(r23)
    b @L_800EA5F0
    @L_800EA3CC:
    cmplwi r23, 0x0
    beq @L_800EA5EC
    lwz r3, 0x14(r23)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA4E4
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA5EC
    mr r3, r23
    bl fn_800EA664
    lwz r3, 0x10(r23)
    bl fn_800EA664
    lwz r3, 0x10(r23)
    addi r4, r1, 0xc8
    addi r3, r3, 0x44
    bl fn_800A2EB4
    addi r4, r1, 0xc8
    addi r3, r23, 0x44
    mr r5, r4
    bl fn_800A2D98
    cmplwi r24, 0x0
    beq @L_800EA430
    addi r4, r1, 0xc8
    mr r3, r24
    mr r5, r4
    bl fn_800A2D98
    @L_800EA430:
    lwz r22, 0x10(r23)
    cmplwi r22, 0x0
    beq @L_800EA5EC
    lwz r3, 0x14(r22)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA480
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA5EC
    mr r4, r22
    addi r3, r1, 0xc8
    addi r5, r1, 0x38
    bl fn_800EA6D4
    lwz r3, 0x10(r22)
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x38
    bl fn_800E9E90
    b @L_800EA5EC
    @L_800EA480:
    and. r0, r3, r29
    beq @L_800EA4A4
    mr r3, r22
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0xc8
    bl fn_800EA7E4
    @L_800EA4A4:
    lwz r0, 0x14(r22)
    and. r0, r0, r30
    beq @L_800EA5EC
    lwz r22, 0x10(r22)
    b @L_800EA4D8
    @L_800EA4B8:
    mr r3, r22
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0xc8
    bl fn_800E9E90
    lwz r22, 0x8(r22)
    @L_800EA4D8:
    cmplwi r22, 0x0
    bne @L_800EA4B8
    b @L_800EA5EC
    @L_800EA4E4:
    and. r0, r3, r29
    beq @L_800EA520
    cmplwi r23, 0x0
    beq @L_800EA520
    andi. r0, r3, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EA520
    mr r3, r23
    mr r4, r24
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    bl fn_800EA820
    @L_800EA520:
    lwz r0, 0x14(r23)
    and. r0, r0, r30
    beq @L_800EA5EC
    lwz r31, 0x10(r23)
    b @L_800EA5E4
    @L_800EA534:
    cmplwi r31, 0x0
    beq @L_800EA5E0
    lwz r3, 0x14(r31)
    rlwinm. r0, r3, 0, 19, 19
    beq @L_800EA580
    rlwinm. r0, r3, 0, 27, 27
    bne @L_800EA5E0
    mr r3, r24
    mr r4, r31
    addi r5, r1, 0x8
    bl fn_800EA6D4
    lwz r3, 0x10(r31)
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    addi r4, r1, 0x8
    bl fn_800E9E90
    b @L_800EA5E0
    @L_800EA580:
    and. r0, r3, r29
    beq @L_800EA5A4
    mr r3, r31
    mr r4, r24
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    bl fn_800EA7E4
    @L_800EA5A4:
    lwz r0, 0x14(r31)
    and. r0, r0, r30
    beq @L_800EA5E0
    lwz r22, 0x10(r31)
    b @L_800EA5D8
    @L_800EA5B8:
    mr r3, r22
    mr r4, r24
    mr r5, r25
    mr r6, r26
    mr r7, r27
    mr r8, r28
    bl fn_800E9E90
    lwz r22, 0x8(r22)
    @L_800EA5D8:
    cmplwi r22, 0x0
    bne @L_800EA5B8
    @L_800EA5E0:
    lwz r31, 0x8(r31)
    @L_800EA5E4:
    cmplwi r31, 0x0
    bne @L_800EA534
    @L_800EA5EC:
    lwz r23, 0x8(r23)
    @L_800EA5F0:
    cmplwi r23, 0x0
    bne @L_800EA3CC
    @L_800EA5F8:
    lmw r22, 0x188(r1)
    lwz r0, 0x1b4(r1)
    mtlr r0
    addi r1, r1, 0x1b0
    blr
