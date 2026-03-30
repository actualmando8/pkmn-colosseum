    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stmw r22, 0x68(r1)
    cmplwi r3, 0x0
    beq @L_8013904C
    mr r31, r3
    lhz r23, 0x3e(r3)
    lhz r0, 0x3c(r3)
    cmplw r0, r23
    bge @L_8013904C
    lis r3, 0x4330
    add r0, r0, r4
    stw r4, 0x5c(r1)
    clrlwi r24, r0, 16
    lhz r25, 0x3a(r31)
    stw r3, 0x58(r1)
    lfd f3, lbl_8047D170(r2)
    cmplw r24, r25
    lfd f0, 0x58(r1)
    lfs f1, 0x2c(r31)
    fsubs f0, f0, f3
    lhz r30, 0x38(r31)
    lwz r29, 0x0(r31)
    fmuls f31, f1, f0
    sth r0, 0x3c(r31)
    bge @L_80138E94
    stw r24, 0x5c(r1)
    lfs f2, lbl_8047D164(r2)
    stw r3, 0x58(r1)
    lfd f0, 0x58(r1)
    stw r25, 0x64(r1)
    fsubs f1, f0, f3
    stw r3, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f3
    fdivs f0, f1, f0
    fsubs f0, f2, f0
    fmuls f31, f31, f0
    b @L_80138F60
    @L_80138E94:
    lhz r3, 0x40(r31)
    lhz r4, 0x44(r31)
    bl fn_800F9318
    subf r5, r25, r24
    lis r4, 0x4330
    xoris r5, r5, 0x8000
    subf r0, r25, r23
    stw r5, 0x64(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047D178(r2)
    mr r22, r3
    stw r4, 0x60(r1)
    lfs f3, lbl_8047D160(r2)
    lfd f0, 0x60(r1)
    stw r0, 0x5c(r1)
    fsubs f1, f0, f2
    stw r4, 0x58(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f2
    fdivs f0, f1, f0
    fsubs f30, f3, f0
    bl fn_800EE0E8
    mr r27, r3
    li r25, 0x0
    b @L_80138F58
    @L_80138EF8:
    mr r3, r22
    mr r4, r25
    bl fn_800EE150
    mr. r23, r3
    beq @L_80138F54
    bl fn_800EE758
    mr r28, r3
    li r26, 0x0
    b @L_80138F44
    @L_80138F1C:
    mr r3, r23
    mr r4, r26
    bl fn_800EE6B4
    mr. r24, r3
    beq @L_80138F40
    fmr f1, f30
    bl fn_800DF21C
    mr r3, r24
    bl fn_800DF608
    @L_80138F40:
    addi r26, r26, 0x1
    @L_80138F44:
    cmplw r26, r28
    blt @L_80138F1C
    mr r3, r23
    bl fn_800EE828
    @L_80138F54:
    addi r25, r25, 0x1
    @L_80138F58:
    cmplw r25, r27
    blt @L_80138EF8
    @L_80138F60:
    li r23, 0x0
    b @L_8013903C
    @L_80138F68:
    lwz r0, 0x58(r29)
    cmplwi r0, 0x0
    beq @L_80139034
    lfs f1, 0x54(r29)
    lfs f0, lbl_8047D160(r2)
    fadds f1, f1, f31
    stfs f1, 0x54(r29)
    lfs f1, 0x54(r29)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80138FA0
    mr r3, r31
    mr r4, r29
    bl fn_80139074
    @L_80138FA0:
    lfs f1, 0x54(r29)
    mr r4, r29
    addi r3, r1, 0x18
    bl fn_800E07E4
    lfs f1, 0x1c(r1)
    lfs f0, lbl_8047D168(r2)
    fcmpo cr0, f1, f0
    bge @L_80138FE0
    lwz r3, 0x58(r29)
    li r4, 0x0
    bl fn_800E4014
    lwz r3, 0x58(r29)
    bl fn_800E4BF4
    li r0, 0x0
    stw r0, 0x58(r29)
    b @L_80139034
    @L_80138FE0:
    lwz r22, 0x58(r29)
    addi r3, r1, 0x8
    lfs f30, 0x50(r29)
    addi r4, r29, 0x30
    lfs f1, 0x54(r29)
    addi r5, r29, 0x40
    bl fn_800E06B8
    addi r3, r1, 0x24
    addi r4, r1, 0x8
    bl fn_800E040C
    fmr f1, f30
    addi r3, r1, 0x24
    fmr f2, f30
    fmr f3, f30
    bl fn_800E02C4
    addi r3, r1, 0x24
    addi r4, r1, 0x18
    bl fn_800E03B4
    mr r3, r22
    addi r4, r1, 0x24
    bl fn_800E4598
    @L_80139034:
    addi r23, r23, 0x1
    addi r29, r29, 0x5c
    @L_8013903C:
    cmplw r23, r30
    blt @L_80138F68
    li r3, 0x1
    b @L_80139050
    @L_8013904C:
    li r3, 0x0
    @L_80139050:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    lmw r22, 0x68(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
