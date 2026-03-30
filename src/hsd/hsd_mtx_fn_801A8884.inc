    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stfd f26, 0x50(r1)
    psq_st f26, 0x58(r1), 0, 0
    stfd f25, 0x40(r1)
    psq_st f25, 0x48(r1), 0, 0
    stfd f24, 0x30(r1)
    psq_st f24, 0x38(r1), 0, 0
    stfd f23, 0x20(r1)
    psq_st f23, 0x28(r1), 0, 0
    stmw r27, 0xc(r1)
    lfs f1, 0x0(r5)
    mr r29, r5
    mr r27, r3
    mr r28, r4
    mr r30, r6
    mr r31, r7
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x0(r29)
    frsp f0, f0
    fmr f31, f0
    bl fn_800CDBE0
    fmr f0, f1
    lfs f1, 0x4(r29)
    frsp f0, f0
    fmr f30, f0
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x4(r29)
    frsp f0, f0
    fmr f29, f0
    bl fn_800CDBE0
    fmr f0, f1
    lfs f1, 0x8(r29)
    frsp f0, f0
    fmr f28, f0
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x8(r29)
    frsp f0, f0
    fmr f27, f0
    bl fn_800CDBE0
    lfs f7, 0x0(r28)
    cmplwi r31, 0x0
    lfs f8, 0x4(r28)
    frsp f0, f1
    lfs f9, 0x8(r28)
    fmr f4, f7
    fmr f5, f8
    fmr f6, f9
    fmr f1, f4
    fmr f2, f5
    fmr f3, f6
    beq @L_801A8A94
    lfs f12, 0x0(r31)
    lfs f10, -23108(r2)
    fcmpo cr0, f12, f10
    cror eq, gt, eq
    bne @L_801A89B8
    lis r3, 0x8048
    lfs f11, -23112(r2)
    lfs f10, -30004(r3)
    fadds f10, f12, f10
    fdivs f10, f11, f10
    b @L_801A89CC
@L_801A89B8:
    lis r3, 0x8048
    lfs f11, -23112(r2)
    lfs f10, -30004(r3)
    fsubs f10, f12, f10
    fdivs f10, f11, f10
@L_801A89CC:
    lfs f13, 0x4(r31)
    lfs f11, -23108(r2)
    fcmpo cr0, f13, f11
    cror eq, gt, eq
    bne @L_801A89F8
    lis r3, 0x8048
    lfs f12, -23112(r2)
    lfs f11, -30004(r3)
    fadds f11, f13, f11
    fdivs f11, f12, f11
    b @L_801A8A0C
@L_801A89F8:
    lis r3, 0x8048
    lfs f12, -23112(r2)
    lfs f11, -30004(r3)
    fsubs f11, f13, f11
    fdivs f11, f12, f11
@L_801A8A0C:
    lfs f24, 0x8(r31)
    lfs f12, -23108(r2)
    fcmpo cr0, f24, f12
    cror eq, gt, eq
    bne @L_801A8A38
    lis r3, 0x8048
    lfs f13, -23112(r2)
    lfs f12, -30004(r3)
    fadds f12, f24, f12
    fdivs f23, f13, f12
    b @L_801A8A4C
@L_801A8A38:
    lis r3, 0x8048
    lfs f13, -23112(r2)
    lfs f12, -30004(r3)
    fsubs f12, f24, f12
    fdivs f23, f13, f12
@L_801A8A4C:
    lfs f13, 0x4(r31)
    lfs f12, 0x8(r31)
    lfs f26, 0x0(r31)
    fmuls f24, f13, f10
    lfs f13, 0x8(r31)
    fmuls f25, f12, f10
    lfs f12, 0x0(r31)
    fmuls f26, f26, f11
    lfs f10, 0x4(r31)
    fmuls f13, f13, f11
    fmuls f11, f12, f23
    fmuls f10, f10, f23
    fmuls f2, f2, f24
    fmuls f3, f3, f25
    fmuls f4, f4, f26
    fmuls f6, f6, f13
    fmuls f7, f7, f11
    fmuls f8, f8, f10
@L_801A8A94:
    fmuls f10, f1, f28
    fmuls f1, f4, f28
    fneg f12, f7
    fmuls f4, f0, f10
    fmuls f10, f31, f29
    fmuls f7, f30, f27
    fmuls f13, f27, f1
    stfs f4, 0x0(r27)
    fmuls f4, f31, f29
    fmuls f1, f30, f0
    fmsubs f11, f0, f10, f7
    stfs f13, 0x10(r27)
    fmuls f12, f12, f29
    fmadds f10, f27, f4, f1
    fmuls f7, f8, f31
    fmuls f8, f2, f11
    stfs f12, 0x20(r27)
    fmuls f4, f30, f29
    fmuls f2, f31, f27
    fmuls f1, f9, f30
    stfs f8, 0x4(r27)
    fmuls f5, f5, f10
    fmadds f4, f0, f4, f2
    fmuls f2, f30, f29
    fmuls f0, f31, f0
    stfs f5, 0x14(r27)
    fmuls f5, f28, f7
    fmuls f3, f3, f4
    fmsubs f2, f27, f2, f0
    stfs f5, 0x24(r27)
    fmuls f0, f28, f1
    fmuls f1, f6, f2
    stfs f3, 0x8(r27)
    stfs f1, 0x18(r27)
    stfs f0, 0x28(r27)
    lfs f0, 0x0(r30)
    stfs f0, 0xc(r27)
    lfs f0, 0x4(r30)
    stfs f0, 0x1c(r27)
    lfs f0, 0x8(r30)
    stfs f0, 0x2c(r27)
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    psq_l f26, 0x58(r1), 0, 0
    lfd f26, 0x50(r1)
    psq_l f25, 0x48(r1), 0, 0
    lfd f25, 0x40(r1)
    psq_l f24, 0x38(r1), 0, 0
    lfd f24, 0x30(r1)
    psq_l f23, 0x28(r1), 0, 0
    lfd f23, 0x20(r1)
    lmw r27, 0xc(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
