    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stmw r20, 0x20(r1)
    mr r24, r3
    mr r25, r4
    bl fn_801DB060
    mr r29, r3
    bl fn_801DB060
    lwz r0, 0x0(r25)
    mr r31, r3
    mr r28, r25
    cmpwi r0, 0x0
    bne @L_801DD638
    li r3, 0x0
    b @L_801DDB28
@L_801DD638:
    li r3, -0x1
    li r0, 0x0
    sth r3, 0x1a(r24)
    sth r3, 0x1c(r24)
    sth r3, 0x1e(r24)
    stb r0, 0x4e(r24)
    lwz r0, 0x10(r25)
    cmpwi r0, 0x4
    beq @L_801DD69C
    bge @L_801DD6B0
    cmpwi r0, 0x3
    bge @L_801DD680
    cmpwi r0, 0x1
    bge @L_801DD678
    b @L_801DD6B0
    b @L_801DD6B0
@L_801DD678:
    li r4, -0x10
    b @L_801DD6D8
@L_801DD680:
    lwz r3, 0x14(r25)
    li r4, -0xc
    neg r0, r3
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x4e(r24)
    b @L_801DD6D8
@L_801DD69C:
    lwz r0, 0x14(r25)
    li r4, -0xc
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x4e(r24)
    b @L_801DD6D8
@L_801DD6B0:
    lwz r0, 0x14(r28)
    li r4, 0x0
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x4e(r24)
    lwz r0, 0x18(r28)
    sth r0, 0x1a(r24)
    lwz r0, 0x20(r28)
    sth r0, 0x1c(r24)
    lwz r0, 0x1c(r28)
    sth r0, 0x1e(r24)
@L_801DD6D8:
    li r3, 0x4e20
    addi r0, r4, 0x43
    stw r3, 0x0(r24)
    rlwinm r0, r0, 0, 0, 26
    li r3, 0x0
    lwz r4, 0xc(r28)
    add r25, r25, r0
    stw r4, 0x10(r24)
    sth r3, 0x32(r24)
    sth r3, 0x34(r24)
    lwz r0, 0x8(r28)
    sth r0, 0x14(r24)
    rlwinm r30, r0, 0, 16, 31
    sth r3, 0x7c(r24)
    lhz r0, 0x70(r24)
    cmplwi r0, 0x18
    beq @L_801DD72C
    cmplwi r0, 0x4a
    beq @L_801DD72C
    cmplwi r0, 0x134
    bne @L_801DD7A0
@L_801DD72C:
    lwz r3, 0x0(r28)
    li r4, 0x20
    addi r0, r3, 0x1f
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    sth r3, 0x7c(r24)
    lhz r3, 0x7c(r24)
    cmplwi r3, 0x0
    bne @L_801DD758
    li r3, 0x0
    b @L_801DDB28
@L_801DD758:
    bl fn_800E27B0
    lwz r5, 0x0(r28)
    mr r20, r3
    mr r4, r25
    addi r0, r5, 0x1f
    rlwinm r5, r0, 0, 0, 26
    bl memcpy
    lwz r4, 0x0(r28)
    mr r3, r20
    addi r0, r4, 0x1f
    rlwinm r4, r0, 0, 0, 26
    bl DCFlushRange
    lwz r4, 0x0(r28)
    mr r3, r20
    mr r6, r29
    li r5, 0x4e20
    bl fn_8010147C
    b @L_801DD7B4
@L_801DD7A0:
    lwz r4, 0x0(r28)
    mr r3, r25
    mr r6, r29
    li r5, 0x4e20
    bl fn_8010147C
@L_801DD7B4:
    mr r4, r29
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DD7D0
    li r3, 0x0
    b @L_801DDB28
@L_801DD7D0:
    stw r29, 0x8(r24)
    mr r4, r29
    li r3, 0x4e20
    bl fn_800F9318
    mr r6, r31
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    mr r4, r31
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DD80C
    li r3, 0x0
    b @L_801DDB28
@L_801DD80C:
    stw r31, 0x4(r24)
    lwz r3, 0x0(r28)
    lwz r0, 0x4(r28)
    addi r3, r3, 0x1f
    rlwinm r3, r3, 0, 0, 26
    cmpwi r0, 0x0
    add r25, r25, r3
    beq @L_801DD87C
    bl fn_801DB060
    mr r20, r3
    lwz r4, 0x4(r28)
    mr r3, r25
    li r5, 0x4e20
    mr r6, r20
    bl fn_80101244
    mr r4, r20
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DD864
    stw r20, 0xc(r24)
    b @L_801DD86C
@L_801DD864:
    li r0, 0x0
    stw r0, 0xc(r24)
@L_801DD86C:
    lwz r3, 0x4(r28)
    addi r0, r3, 0x1f
    rlwinm r0, r0, 0, 0, 26
    add r25, r25, r0
@L_801DD87C:
    mulli r3, r30, 0xd4
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    sth r3, 0x30(r24)
    beq @L_801DDB1C
    mr r27, r25
    bl fn_800E27B0
    lfd f30, -21216(r2)
    mr r26, r3
    stw r3, 0x2c(r24)
    li r29, 0x0
    lfs f31, -21224(r2)
    lis r31, 0x4330
    b @L_801DDA18
@L_801DD8B4:
    lwz r23, 0x0(r27)
    mr r22, r27
    mr r21, r26
    li r20, 0x0
    stw r23, 0x0(r26)
    b @L_801DD920
@L_801DD8CC:
    bl fn_800D37CC
    lwz r0, 0xc(r22)
    xoris r3, r3, 0x8000
    stw r3, 0xc(r1)
    addi r22, r22, 0x4
    xoris r0, r0, 0x8000
    addi r20, r20, 0x1
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f30
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fmuls f0, f0, f1
    fdivs f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r21)
    addi r21, r21, 0x4
@L_801DD920:
    cmpw r20, r23
    blt @L_801DD8CC
    lwz r4, 0x8(r27)
    li r3, 0x1
    li r0, 0x0
    mr r5, r27
    stw r4, 0x8(r26)
    addi r6, r26, 0x94
    stw r3, 0x8c(r26)
    stw r0, 0x90(r26)
    lwz r4, 0x4(r27)
    addi r0, r4, 0x1
    stw r0, 0x4(r26)
    mtctr r4
    cmpwi r4, 0x0
    ble @L_801DD98C
@L_801DD960:
    lwz r0, 0x8c(r5)
    stw r0, 0x0(r6)
    lwz r0, 0x90(r5)
    stw r0, 0x4(r6)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_801DD980
    stw r3, 0x4(r6)
@L_801DD980:
    addi r5, r5, 0x8
    addi r6, r6, 0x8
    bdnz @L_801DD960
@L_801DD98C:
    lwz r0, 0x4c(r27)
    addi r29, r29, 0x1
    stw r0, 0x4c(r26)
    lwz r0, 0x50(r27)
    stw r0, 0x50(r26)
    lwz r0, 0x54(r27)
    stw r0, 0x54(r26)
    lwz r0, 0x58(r27)
    stw r0, 0x58(r26)
    lwz r0, 0x5c(r27)
    stw r0, 0x5c(r26)
    lwz r0, 0x60(r27)
    stw r0, 0x60(r26)
    lwz r0, 0x64(r27)
    stw r0, 0x64(r26)
    lwz r0, 0x68(r27)
    stw r0, 0x68(r26)
    lwz r0, 0x6c(r27)
    stw r0, 0x6c(r26)
    lwz r0, 0x70(r27)
    stw r0, 0x70(r26)
    lwz r0, 0x74(r27)
    stw r0, 0x74(r26)
    lwz r0, 0x78(r27)
    stw r0, 0x78(r26)
    lwz r0, 0x7c(r27)
    stw r0, 0x7c(r26)
    lwz r0, 0x80(r27)
    stw r0, 0x80(r26)
    lwz r0, 0x84(r27)
    stw r0, 0x84(r26)
    lwz r0, 0x88(r27)
    addi r27, r27, 0xd0
    stw r0, 0x88(r26)
    addi r26, r26, 0xd4
@L_801DDA18:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r30
    blt @L_801DD8B4
    lwz r0, 0x10(r28)
    cmpwi r0, 0x4
    bge @L_801DDA3C
    mulli r0, r30, 0xd0
    add r25, r25, r0
    b @L_801DDA4C
@L_801DDA3C:
    mulli r3, r30, 0xd0
    addi r0, r3, 0x1f
    rlwinm r0, r0, 0, 0, 26
    add r25, r25, r0
@L_801DDA4C:
    lbz r0, 0x4e(r24)
    cmplwi r0, 0x0
    beq @L_801DDB14
    lwz r0, 0x0(r25)
    stw r0, 0x38(r24)
    lwz r0, 0x4(r25)
    stw r0, 0x3c(r24)
    lwz r0, 0x8(r25)
    stw r0, 0x40(r24)
    lwz r0, 0xc(r25)
    stw r0, 0x44(r24)
    lwz r0, 0x38(r24)
    cmpwi r0, 0x0
    bne @L_801DDAA8
    lwz r0, 0x3c(r24)
    cmpwi r0, 0x1
    bne @L_801DDAA8
    lwz r0, 0x40(r24)
    cmpwi r0, 0x2
    bne @L_801DDAA8
    lwz r0, 0x44(r24)
    cmpwi r0, 0x3
    beq @L_801DDAB0
@L_801DDAA8:
    li r0, 0x1
    stb r0, 0x4c(r24)
@L_801DDAB0:
    lwz r0, 0x10(r25)
    srawi r0, r0, 24
    stb r0, 0x4b(r24)
    lwz r0, 0x10(r25)
    rlwinm r0, r0, 16, 24, 31
    stb r0, 0x4a(r24)
    lwz r0, 0x10(r25)
    rlwinm r0, r0, 24, 24, 31
    stb r0, 0x49(r24)
    lwz r0, 0x10(r25)
    stb r0, 0x48(r24)
    lbz r0, 0x48(r24)
    cmplwi r0, 0x7f
    bne @L_801DDB0C
    lbz r0, 0x49(r24)
    cmplwi r0, 0x7f
    bne @L_801DDB0C
    lbz r0, 0x4a(r24)
    cmplwi r0, 0x7f
    bne @L_801DDB0C
    lbz r0, 0x4b(r24)
    cmplwi r0, 0x7f
    beq @L_801DDB14
@L_801DDB0C:
    li r0, 0x1
    stb r0, 0x4d(r24)
@L_801DDB14:
    li r3, 0x1
    b @L_801DDB28
@L_801DDB1C:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x2c(r24)
@L_801DDB28:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    lmw r20, 0x20(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
