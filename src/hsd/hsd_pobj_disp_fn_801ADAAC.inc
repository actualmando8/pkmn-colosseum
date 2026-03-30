    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stfd f29, 0x30(r1)
    psq_st f29, 0x38(r1), 0, 0
    stfd f28, 0x20(r1)
    psq_st f28, 0x28(r1), 0, 0
    stfd f27, 0x10(r1)
    psq_st f27, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lfs f1, -22916(r2)
    mr r30, r3
    lfs f0, 0x0(r3)
    mr r31, r4
    fmuls f1, f1, f0
    bl fn_800CDBE0
    frsp f3, f1
    lfs f2, -22916(r2)
    lfs f0, 0x4(r30)
    fmuls f1, f2, f0
    fmr f31, f3
    bl fn_800CDBE0
    frsp f3, f1
    lfs f2, -22916(r2)
    lfs f0, 0x8(r30)
    fmuls f1, f2, f0
    fmr f30, f3
    bl fn_800CDBE0
    frsp f3, f1
    lfs f2, -22916(r2)
    lfs f0, 0x0(r30)
    fmuls f1, f2, f0
    fmr f29, f3
    bl fn_800CE148
    frsp f3, f1
    lfs f2, -22916(r2)
    lfs f0, 0x4(r30)
    fmuls f1, f2, f0
    fmr f28, f3
    bl fn_800CE148
    frsp f3, f1
    lfs f2, -22916(r2)
    lfs f0, 0x8(r30)
    fmuls f1, f2, f0
    fmr f27, f3
    bl fn_800CE148
    frsp f4, f1
    li r3, 0x1
    fmuls f0, f28, f27
    fmuls f2, f28, f30
    fmuls f7, f27, f4
    fmuls f1, f31, f30
    fmuls f0, f29, f0
    fmuls f6, f30, f29
    fmuls f3, f31, f27
    fmuls f2, f4, f2
    fmsubs f0, f4, f1, f0
    fmuls f5, f28, f7
    fmadds f1, f29, f3, f2
    fmuls f4, f31, f7
    fmadds f3, f31, f6, f5
    fmsubs f2, f28, f6, f4
    stfs f3, 0xc(r31)
    stfs f2, 0x0(r31)
    stfs f1, 0x4(r31)
    stfs f0, 0x8(r31)
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    psq_l f29, 0x38(r1), 0, 0
    lfd f29, 0x30(r1)
    psq_l f28, 0x28(r1), 0, 0
    lfd f28, 0x20(r1)
    psq_l f27, 0x18(r1), 0, 0
    lfd f27, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r0, 0x64(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
