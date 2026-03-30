    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stfd f30, 0xb0(r1)
    psq_st f30, 0xb8(r1), 0, 0
    stfd f29, 0xa0(r1)
    psq_st f29, 0xa8(r1), 0, 0
    stfd f28, 0x90(r1)
    psq_st f28, 0x98(r1), 0, 0
    stfd f27, 0x80(r1)
    psq_st f27, 0x88(r1), 0, 0
    stfd f26, 0x70(r1)
    psq_st f26, 0x78(r1), 0, 0
    stfd f25, 0x60(r1)
    psq_st f25, 0x68(r1), 0, 0
    stmw r18, 0x28(r1)
    fmr f28, f1
    mr r18, r3
    fmr f29, f2
    mr r19, r4
    fmr f31, f3
    fmr f30, f4
    lis r4, 0x4330
    lis r3, -32698
    stw r18, 0x14(r1)
    addi r30, r3, 28752
    lfd f3, -21928(r2)
    addi r3, r18, 0x1
    stw r4, 0x10(r1)
    addi r0, r19, 0x1
    mullw r22, r3, r0
    lfs f2, -21968(r2)
    lfd f0, 0x10(r1)
    stw r19, 0x1c(r1)
    fsubs f0, f0, f3
    lfs f1, -21964(r2)
    stw r4, 0x18(r1)
    fdivs f2, f2, f0
    mulli r20, r22, 0xc
    lfd f0, 0x18(r1)
    stw r18, 0x0(r30)
    mr r3, r20
    stw r19, 0x4(r30)
    fsubs f0, f0, f3
    stfs f2, 0x20(r30)
    fdivs f0, f1, f0
    stfs f0, 0x24(r30)
    bl fn_801C7630
    lis r4, -32698
    addi r4, r4, 28752
    addi r26, r4, 0x28
    stw r3, 0x0(r26)
    mr r3, r20
    bl fn_801C7630
    lis r4, -32698
    li r0, 0x0
    addi r4, r4, 28752
    stw r0, -21608(r13)
    addi r25, r4, 0x2c
    stw r3, 0x0(r25)
    mr r3, r20
    bl fn_801C7630
    lis r4, -32698
    addi r4, r4, 28752
    addi r24, r4, 0x30
    stw r3, 0x0(r24)
    mr r3, r20
    bl fn_801C7630
    lis r4, -32698
    addi r4, r4, 28752
    addi r23, r4, 0x34
    stw r3, 0x0(r23)
    rlwinm r3, r22, 3, 0, 28
    bl fn_801C7630
    lfs f1, -21980(r2)
    lis r4, -32698
    addi r4, r4, 28752
    lfs f0, -22004(r2)
    fadds f4, f1, f30
    addi r22, r4, 0x38
    stw r3, 0x0(r22)
    fcmpo cr0, f4, f0
    ble @L_801C734C
    frsqrte f1, f4
    lfd f3, -21960(r2)
    lfd f2, -21952(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801C73E0
@L_801C734C:
    lfd f0, -21944(r2)
    fcmpo cr0, f4, f0
    bge @L_801C7368
    lis r3, -32696
    addi r3, r3, -30016
    lfs f4, 0x0(r3)
    b @L_801C73E0
@L_801C7368:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r4, 0x8(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801C7390
    bge @L_801C73C8
    cmpwi r3, 0x0
    beq @L_801C73AC
    b @L_801C73C8
@L_801C7390:
    rlwinm r0, r4, 0, 9, 31
    cmpwi r0, 0x0
    beq @L_801C73A4
    li r0, 0x1
    b @L_801C73CC
@L_801C73A4:
    li r0, 0x2
    b @L_801C73CC
@L_801C73AC:
    rlwinm r0, r4, 0, 9, 31
    cmpwi r0, 0x0
    beq @L_801C73C0
    li r0, 0x5
    b @L_801C73CC
@L_801C73C0:
    li r0, 0x3
    b @L_801C73CC
@L_801C73C8:
    li r0, 0x4
@L_801C73CC:
    cmpwi r0, 0x1
    bne @L_801C73E0
    lis r3, -32696
    addi r3, r3, -30016
    lfs f4, 0x0(r3)
@L_801C73E0:
    lfs f1, -21980(r2)
    lfs f0, -22004(r2)
    fmuls f1, f1, f29
    fcmpo cr0, f0, f31
    fdivs f0, f28, f1
    fmuls f25, f0, f4
    bge @L_801C7404
    fcmpo cr0, f31, f25
    blt @L_801C7424
@L_801C7404:
    fmr f1, f31
    lis r3, -32729
    fmr f2, f25
    addi r3, r3, 22624
    cror 6, 6, 6
    bl OSReport
    lfs f0, -21936(r2)
    fsubs f31, f25, f0
@L_801C7424:
    lis r3, -32698
    fmr f1, f29
    addi r3, r3, 28752
    stfs f28, 0x1c(r3)
    stfs f31, 0x14(r3)
    stfs f30, 0x18(r3)
    bl fn_801C7090
    lis r3, -32698
    lis r31, 0x4330
    addi r3, r3, 28752
    lwz r0, 0x4(r30)
    lwz r3, 0x0(r3)
    li r28, 0x0
    lfs f0, -21980(r2)
    mr r27, r28
    stw r3, 0x1c(r1)
    li r20, 0x0
    lfd f2, -21928(r2)
    fmuls f31, f0, f28
    stw r31, 0x18(r1)
    lfs f3, -22008(r2)
    lfd f0, 0x18(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fdivs f30, f3, f1
    fsubs f0, f0, f2
    fdivs f25, f3, f0
    lfd f29, -21992(r2)
    b @L_801C7598
@L_801C74A0:
    xoris r0, r20, 0x8000
    stw r31, 0x18(r1)
    mr r30, r28
    mr r29, r27
    stw r0, 0x1c(r1)
    li r21, 0x0
    lfd f0, 0x18(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f29
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fmuls f27, f28, f1
    fsubs f0, f0, f29
    fmuls f26, f25, f0
    b @L_801C758C
@L_801C74DC:
    xoris r0, r21, 0x8000
    lwz r3, 0x0(r26)
    stw r0, 0x1c(r1)
    fmr f2, f27
    lfs f3, -22004(r2)
    add r3, r3, r30
    stw r31, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f29
    fmuls f1, f28, f0
    bl fn_800E01F4
    lwz r3, 0x0(r25)
    lwz r0, 0x0(r26)
    add r3, r3, r30
    add r4, r0, r30
    bl fn_800E01D0
    lfs f1, -22004(r2)
    fmr f3, f31
    lwz r0, 0x0(r24)
    fmr f2, f1
    add r3, r0, r30
    bl fn_800E01F4
    lfs f2, -22004(r2)
    fmr f1, f31
    lwz r0, 0x0(r23)
    fmr f3, f2
    add r3, r0, r30
    bl fn_800E01F4
    xoris r4, r21, 0x8000
    stw r31, 0x10(r1)
    lwz r3, 0x0(r22)
    addi r0, r29, 0x4
    stw r4, 0x14(r1)
    addi r30, r30, 0xc
    addi r28, r28, 0xc
    addi r27, r27, 0x8
    lfd f0, 0x10(r1)
    addi r21, r21, 0x1
    fsubs f0, f0, f29
    fmuls f0, f30, f0
    stfsx f0, r3, r29
    addi r29, r29, 0x8
    lwz r3, 0x0(r22)
    stfsx f26, r3, r0
@L_801C758C:
    cmplw r21, r18
    ble @L_801C74DC
    addi r20, r20, 0x1
@L_801C7598:
    cmplw r20, r19
    ble @L_801C74A0
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    psq_l f28, 0x98(r1), 0, 0
    lfd f28, 0x90(r1)
    psq_l f27, 0x88(r1), 0, 0
    lfd f27, 0x80(r1)
    psq_l f26, 0x78(r1), 0, 0
    lfd f26, 0x70(r1)
    psq_l f25, 0x68(r1), 0, 0
    lfd f25, 0x60(r1)
    lmw r18, 0x28(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
