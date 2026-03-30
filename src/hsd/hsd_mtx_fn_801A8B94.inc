    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stfd f26, 0x10(r1)
    psq_st f26, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lfs f1, 0x0(r4)
    mr r31, r4
    mr r30, r3
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x0(r31)
    frsp f0, f0
    fmr f31, f0
    bl fn_800CDBE0
    fmr f0, f1
    lfs f1, 0x4(r31)
    frsp f0, f0
    fmr f28, f0
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x4(r31)
    frsp f0, f0
    fmr f30, f0
    bl fn_800CDBE0
    fmr f0, f1
    lfs f1, 0x8(r31)
    frsp f0, f0
    fmr f27, f0
    bl fn_800CE148
    fmr f0, f1
    lfs f1, 0x8(r31)
    frsp f0, f0
    fmr f29, f0
    bl fn_800CDBE0
    frsp f5, f1
    lfs f3, -23108(r2)
    fmuls f12, f27, f29
    lfs f2, -23108(r2)
    fmuls f4, f31, f30
    lfs f0, -23108(r2)
    fmr f26, f5
    fmuls f1, f28, f29
    fneg f11, f30
    fmuls f13, f27, f26
    fmsubs f10, f26, f4, f1
    fmuls f9, f31, f30
    stfs f13, 0x0(r30)
    fmuls f8, f28, f26
    fmuls f6, f28, f30
    stfs f12, 0x10(r30)
    fmuls f5, f31, f29
    fmadds f8, f29, f9, f8
    stfs f11, 0x20(r30)
    fmuls f7, f31, f27
    fmuls f4, f28, f30
    stfs f10, 0x4(r30)
    fmuls f1, f31, f26
    fmadds f5, f26, f6, f5
    stfs f8, 0x14(r30)
    fmsubs f4, f29, f4, f1
    stfs f7, 0x24(r30)
    fmuls f1, f28, f27
    stfs f5, 0x8(r30)
    stfs f4, 0x18(r30)
    stfs f1, 0x28(r30)
    stfs f3, 0xc(r30)
    stfs f2, 0x1c(r30)
    stfs f0, 0x2c(r30)
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    psq_l f26, 0x18(r1), 0, 0
    lfd f26, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r0, 0x74(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
