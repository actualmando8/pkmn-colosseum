    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stfd f26, 0x60(r1)
    psq_st f26, 0x68(r1), 0, 0
    stfd f25, 0x50(r1)
    psq_st f25, 0x58(r1), 0, 0
    stfd f24, 0x40(r1)
    psq_st f24, 0x48(r1), 0, 0
    lfs f2, 0x4(r3)
    mr r6, r4
    lfs f1, 0x18(r3)
    lfs f4, 0x0(r3)
    fmuls f1, f2, f1
    lfs f3, 0x14(r3)
    lfs f2, 0x20(r3)
    fmuls f3, f4, f3
    lfs f5, 0x8(r3)
    fmuls f1, f2, f1
    lfs f4, 0x10(r3)
    lfs f2, 0x28(r3)
    fmuls f4, f5, f4
    lfs f5, 0x20(r3)
    fmadds f1, f2, f3, f1
    lfs f2, 0x24(r3)
    lfs f3, 0x14(r3)
    lfs f6, 0x10(r3)
    fmadds f1, f2, f4, f1
    lfs f4, 0x4(r3)
    fmuls f2, f5, f3
    lfs f3, 0x8(r3)
    fmuls f4, f6, f4
    lfs f5, 0x0(r3)
    fnmsubs f1, f3, f2, f1
    lfs f2, 0x28(r3)
    lfs f3, 0x24(r3)
    lfs f0, -23108(r2)
    fnmsubs f1, f2, f4, f1
    lfs f4, 0x18(r3)
    fmuls f2, f5, f3
    fnmsubs f8, f4, f2, f1
    fcmpu cr0, f0, f8
    bne @L_801A9ECC
    li r3, 0x0
    b @L_801AA300
@L_801A9ECC:
    lfs f0, -23112(r2)
    cmplw r3, r5
    lfs f6, 0x24(r3)
    fdivs f8, f0, f8
    lfs f0, 0x18(r3)
    lfs f5, 0x24(r3)
    lfs f3, 0x8(r3)
    lfs f2, 0x20(r3)
    lfs f1, 0x18(r3)
    lfs f4, 0x20(r3)
    fmuls f6, f6, f0
    lfs f0, 0x8(r3)
    fmuls f7, f5, f3
    lfs f3, 0x20(r3)
    fmuls f5, f2, f1
    lfs f2, 0x14(r3)
    lfs f1, 0x20(r3)
    fmuls f4, f4, f0
    lfs f0, 0x4(r3)
    fmuls f3, f3, f2
    lfs f10, 0x14(r3)
    lfs f9, 0x28(r3)
    fmuls f1, f1, f0
    lfs f2, 0x4(r3)
    lfs f0, 0x28(r3)
    fmsubs f10, f10, f9, f6
    lfs f9, 0x10(r3)
    lfs f6, 0x28(r3)
    fnmsubs f12, f2, f0, f7
    fmuls f0, f8, f10
    lfs f7, 0x0(r3)
    lfs f2, 0x28(r3)
    fnmsubs f9, f9, f6, f5
    lfs f6, 0x10(r3)
    lfs f5, 0x24(r3)
    fmsubs f10, f7, f2, f4
    lfs f4, 0x0(r3)
    lfs f2, 0x24(r3)
    fmsubs f5, f6, f5, f3
    fmuls f3, f8, f9
    lfs f11, 0x14(r3)
    fnmsubs f2, f4, f2, f1
    lfs f4, 0x8(r3)
    fmuls f6, f8, f5
    lfs f7, 0x10(r3)
    lfs f5, 0x8(r3)
    fmuls f1, f8, f12
    lfs f9, 0x1c(r3)
    fmuls f12, f11, f4
    lfs f11, 0x10(r3)
    fmuls f4, f8, f10
    lfs f10, 0x4(r3)
    fmuls f13, f7, f5
    lfs f5, 0x1c(r3)
    fmuls f7, f8, f2
    lfs f2, 0x1c(r3)
    fmuls f10, f11, f10
    lfs f24, 0x4(r3)
    lfs f11, 0x18(r3)
    fneg f27, f0
    fmsubs f29, f24, f11, f12
    lfs f24, 0x0(r3)
    lfs f11, 0x18(r3)
    fmuls f12, f1, f9
    lfs f28, 0xc(r3)
    fneg f30, f3
    fnmsubs f24, f24, f11, f13
    lfs f25, 0x0(r3)
    lfs f9, 0x14(r3)
    fmuls f13, f4, f5
    lfs f31, 0xc(r3)
    fneg f11, f6
    fmsubs f25, f25, f9, f10
    lfs f10, 0xc(r3)
    fmuls f9, f7, f2
    lfs f26, 0x2c(r3)
    fmuls f2, f8, f29
    lfs f29, 0x2c(r3)
    fmsubs f28, f27, f28, f12
    lfs f12, 0x2c(r3)
    fmuls f5, f8, f24
    fmsubs f13, f30, f31, f13
    fmuls f8, f8, f25
    fmsubs f11, f11, f10, f9
    fnmsubs f9, f2, f26, f28
    fnmsubs f10, f5, f29, f13
    fnmsubs f11, f8, f12, f11
    beq @L_801AA034
    cmplw r6, r5
    bne @L_801AA1A0
@L_801AA034:
    lfs f12, 0x10(r6)
    mr r4, r5
    lfs f13, 0x0(r6)
    addi r3, r1, 0x8
    fmuls f12, f1, f12
    lfs f31, 0x20(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0x8(r1)
    lfs f12, 0x14(r6)
    lfs f13, 0x4(r6)
    fmuls f12, f1, f12
    lfs f31, 0x24(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0xc(r1)
    lfs f12, 0x18(r6)
    lfs f13, 0x8(r6)
    fmuls f12, f1, f12
    lfs f31, 0x28(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0x10(r1)
    lfs f12, 0x1c(r6)
    lfs f13, 0xc(r6)
    fmuls f1, f1, f12
    lfs f12, 0x2c(r6)
    fmadds f0, f0, f13, f1
    fmadds f0, f2, f12, f0
    fadds f0, f9, f0
    stfs f0, 0x14(r1)
    lfs f0, 0x10(r6)
    lfs f1, 0x0(r6)
    fmuls f0, f4, f0
    lfs f2, 0x20(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x18(r1)
    lfs f0, 0x14(r6)
    lfs f1, 0x4(r6)
    fmuls f0, f4, f0
    lfs f2, 0x24(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x1c(r1)
    lfs f0, 0x18(r6)
    lfs f1, 0x8(r6)
    fmuls f0, f4, f0
    lfs f2, 0x28(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x20(r1)
    lfs f0, 0x1c(r6)
    lfs f1, 0xc(r6)
    fmuls f0, f4, f0
    lfs f2, 0x2c(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    fadds f0, f10, f0
    stfs f0, 0x24(r1)
    lfs f0, 0x10(r6)
    lfs f1, 0x0(r6)
    fmuls f0, f7, f0
    lfs f2, 0x20(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x28(r1)
    lfs f0, 0x14(r6)
    lfs f1, 0x4(r6)
    fmuls f0, f7, f0
    lfs f2, 0x24(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x2c(r1)
    lfs f0, 0x18(r6)
    lfs f1, 0x8(r6)
    fmuls f0, f7, f0
    lfs f2, 0x28(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x30(r1)
    lfs f0, 0x1c(r6)
    lfs f1, 0xc(r6)
    fmuls f0, f7, f0
    lfs f2, 0x2c(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    fadds f0, f11, f0
    stfs f0, 0x34(r1)
    bl fn_800A2D64
    b @L_801AA2FC
@L_801AA1A0:
    lfs f12, 0x10(r6)
    lfs f13, 0x0(r6)
    fmuls f12, f1, f12
    lfs f31, 0x20(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0x0(r5)
    lfs f12, 0x14(r6)
    lfs f13, 0x4(r6)
    fmuls f12, f1, f12
    lfs f31, 0x24(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0x4(r5)
    lfs f12, 0x18(r6)
    lfs f13, 0x8(r6)
    fmuls f12, f1, f12
    lfs f31, 0x28(r6)
    fmadds f12, f0, f13, f12
    fmadds f12, f2, f31, f12
    stfs f12, 0x8(r5)
    lfs f12, 0x1c(r6)
    lfs f13, 0xc(r6)
    fmuls f1, f1, f12
    lfs f12, 0x2c(r6)
    fmadds f0, f0, f13, f1
    fmadds f0, f2, f12, f0
    fadds f0, f9, f0
    stfs f0, 0xc(r5)
    lfs f0, 0x10(r6)
    lfs f1, 0x0(r6)
    fmuls f0, f4, f0
    lfs f2, 0x20(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x10(r5)
    lfs f0, 0x14(r6)
    lfs f1, 0x4(r6)
    fmuls f0, f4, f0
    lfs f2, 0x24(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x14(r5)
    lfs f0, 0x18(r6)
    lfs f1, 0x8(r6)
    fmuls f0, f4, f0
    lfs f2, 0x28(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    stfs f0, 0x18(r5)
    lfs f0, 0x1c(r6)
    lfs f1, 0xc(r6)
    fmuls f0, f4, f0
    lfs f2, 0x2c(r6)
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    fadds f0, f10, f0
    stfs f0, 0x1c(r5)
    lfs f0, 0x10(r6)
    lfs f1, 0x0(r6)
    fmuls f0, f7, f0
    lfs f2, 0x20(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x20(r5)
    lfs f0, 0x14(r6)
    lfs f1, 0x4(r6)
    fmuls f0, f7, f0
    lfs f2, 0x24(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x24(r5)
    lfs f0, 0x18(r6)
    lfs f1, 0x8(r6)
    fmuls f0, f7, f0
    lfs f2, 0x28(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    stfs f0, 0x28(r5)
    lfs f0, 0x1c(r6)
    lfs f1, 0xc(r6)
    fmuls f0, f7, f0
    lfs f2, 0x2c(r6)
    fmadds f0, f6, f1, f0
    fmadds f0, f8, f2, f0
    fadds f0, f11, f0
    stfs f0, 0x2c(r5)
@L_801AA2FC:
    li r3, 0x1
@L_801AA300:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    psq_l f26, 0x68(r1), 0, 0
    lfd f26, 0x60(r1)
    psq_l f25, 0x58(r1), 0, 0
    lfd f25, 0x50(r1)
    psq_l f24, 0x48(r1), 0, 0
    lwz r0, 0xc4(r1)
    lfd f24, 0x40(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
