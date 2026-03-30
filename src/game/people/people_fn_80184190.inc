    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    li r27, 0x0
    b @L_801841EC
    @L_801841B4:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801841E8
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_801841E8
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_801841E8
    lwz r27, 0x4(r3)
    b @L_80184258
    @L_801841E8:
    addi r27, r27, 0x1
    @L_801841EC:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_801841B4
    li r31, 0x0
    b @L_80184248
    @L_80184200:
    mr r3, r31
    bl fn_8018FD88
    mr r27, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184244
    lwz r0, 0x2c(r27)
    cmplw r0, r29
    bne @L_80184244
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r27, 0x4(r27)
    b @L_80184258
    @L_80184244:
    addi r31, r31, 0x1
    @L_80184248:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80184200
    li r27, 0x0
    @L_80184258:
    li r31, 0x0
    b @L_8018428C
    @L_80184260:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184288
    lwz r0, 0x4(r3)
    cmplw r0, r27
    bne @L_80184288
    mr r31, r3
    b @L_8018429C
    @L_80184288:
    addi r31, r31, 0x1
    @L_8018428C:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80184260
    li r31, 0x0
    @L_8018429C:
    cmplwi r31, 0x0
    bne @L_801842AC
    li r3, 0x0
    b @L_8018443C
    @L_801842AC:
    li r27, 0x0
    b @L_801842EC
    @L_801842B4:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801842E8
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_801842E8
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_801842E8
    lwz r27, 0x4(r3)
    b @L_80184358
    @L_801842E8:
    addi r27, r27, 0x1
    @L_801842EC:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_801842B4
    li r26, 0x0
    b @L_80184348
    @L_80184300:
    mr r3, r26
    bl fn_8018FD88
    mr r27, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184344
    lwz r0, 0x2c(r27)
    cmplw r0, r29
    bne @L_80184344
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r27, 0x4(r27)
    b @L_80184358
    @L_80184344:
    addi r26, r26, 0x1
    @L_80184348:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80184300
    li r27, 0x0
    @L_80184358:
    li r28, 0x0
    b @L_8018438C
    @L_80184360:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184388
    lwz r0, 0x4(r3)
    cmplw r0, r27
    bne @L_80184388
    mr r27, r3
    b @L_8018439C
    @L_80184388:
    addi r28, r28, 0x1
    @L_8018438C:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80184360
    li r27, 0x0
    @L_8018439C:
    cmplwi r27, 0x0
    beq @L_801843D8
    lhz r3, 0x6c(r27)
    cmplwi r3, 0x0
    beq @L_801843D8
    bl fn_800E24B0
    lhz r3, 0x6c(r27)
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x70(r27)
    sth r0, 0x6c(r27)
    stb r0, 0x54(r27)
    sth r0, 0x68(r27)
    sth r0, 0x6a(r27)
    stb r0, 0x55(r27)
    @L_801843D8:
    clrlwi r0, r30, 16
    mulli r27, r0, 0xc
    mr r3, r27
    bl fn_800E3534
    sth r3, 0x6c(r31)
    lhz r3, 0x6c(r31)
    cmplwi r3, 0x0
    bne @L_80184400
    li r3, 0x0
    b @L_8018443C
    @L_80184400:
    bl fn_800E27B0
    stw r3, 0x70(r31)
    lwz r3, 0x70(r31)
    cmplwi r3, 0x0
    bne @L_8018441C
    li r3, 0x0
    b @L_8018443C
    @L_8018441C:
    mr r5, r27
    li r4, 0x0
    bl memset
    sth r30, 0x68(r31)
    li r0, 0x0
    li r3, 0x1
    sth r0, 0x6a(r31)
    stb r0, 0x55(r31)
    @L_8018443C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
