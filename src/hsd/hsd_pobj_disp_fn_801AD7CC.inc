    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stfd f29, 0x20(r1)
    psq_st f29, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    lfs f2, 0x4(r3)
    fmr f31, f1
    lfs f0, 0x4(r4)
    mr r31, r5
    lfs f3, 0x0(r3)
    fmuls f0, f2, f0
    lfs f1, 0x0(r4)
    lfs f5, 0x8(r3)
    lfs f4, 0x8(r4)
    fmadds f0, f3, f1, f0
    lfs f6, 0xc(r3)
    lfs f3, 0xc(r4)
    lfs f2, -22920(r2)
    fmadds f1, f5, f4, f0
    lfs f0, -22912(r2)
    fmadds f1, f6, f3, f1
    fadds f2, f2, f1
    fcmpo cr0, f2, f0
    ble @L_801AD914
    lfs f2, -22920(r2)
    lfs f0, -22912(r2)
    fsubs f2, f2, f1
    fcmpo cr0, f2, f0
    ble @L_801AD8B0
    bl fn_800CE298
    frsp f0, f1
    fmr f30, f0
    fmr f1, f30
    bl fn_800CE148
    lfs f0, -22920(r2)
    frsp f2, f1
    fsubs f0, f0, f31
    fmr f29, f2
    fmuls f1, f0, f30
    bl fn_800CE148
    fmr f0, f1
    fmuls f1, f31, f30
    frsp f0, f0
    fdivs f31, f0, f29
    bl fn_800CE148
    frsp f0, f1
    fdivs f0, f0, f29
    b @L_801AD8C0
@L_801AD8B0:
    lfd f1, -22904(r2)
    fmr f0, f31
    fsub f31, f1, f31
    frsp f31, f31
@L_801AD8C0:
    lfs f1, 0x0(r30)
    lfs f2, 0x0(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x0(r31)
    lfs f1, 0x4(r30)
    lfs f2, 0x4(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x4(r31)
    lfs f1, 0x8(r30)
    lfs f2, 0x8(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x8(r31)
    lfs f1, 0xc(r30)
    lfs f2, 0xc(r29)
    fmuls f0, f0, f1
    fmadds f0, f31, f2, f0
    stfs f0, 0xc(r31)
    b @L_801ADA74
@L_801AD914:
    lfs f1, 0x4(r29)
    lfs f0, -22916(r2)
    fneg f1, f1
    fcmpo cr0, f31, f0
    stfs f1, 0x0(r31)
    lfs f0, 0x0(r29)
    stfs f0, 0x4(r31)
    lfs f0, 0xc(r29)
    fneg f0, f0
    stfs f0, 0x8(r31)
    lfs f0, 0x8(r29)
    stfs f0, 0xc(r31)
    bge @L_801AD9DC
    lfs f1, -22888(r2)
    lfs f0, -22920(r2)
    lfd f2, -22896(r2)
    fnmsubs f0, f1, f31, f0
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE148
    lfs f0, -22888(r2)
    frsp f3, f1
    lfd f2, -22896(r2)
    fmuls f0, f0, f31
    fmr f31, f3
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE148
    frsp f0, f1
    lfs f2, 0x0(r30)
    lfs f3, 0x0(r29)
    fmuls f1, f0, f2
    fmadds f1, f31, f3, f1
    stfs f1, 0x0(r31)
    lfs f1, 0x4(r30)
    lfs f2, 0x4(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x4(r31)
    lfs f1, 0x8(r30)
    lfs f2, 0x8(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x8(r31)
    lfs f1, 0xc(r30)
    lfs f2, 0xc(r29)
    fmuls f0, f0, f1
    fmadds f0, f31, f2, f0
    stfs f0, 0xc(r31)
    b @L_801ADA74
@L_801AD9DC:
    lfs f0, -22916(r2)
    lfs f1, -22888(r2)
    fsubs f31, f31, f0
    lfs f0, -22920(r2)
    lfd f2, -22896(r2)
    fnmsubs f0, f1, f31, f0
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE148
    lfs f0, -22888(r2)
    frsp f3, f1
    lfd f2, -22896(r2)
    fmuls f0, f0, f31
    fmr f31, f3
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE148
    frsp f0, f1
    lfs f2, 0x0(r30)
    lfs f3, 0x0(r29)
    fmuls f1, f0, f2
    fmadds f1, f31, f3, f1
    stfs f1, 0x0(r31)
    lfs f1, 0x4(r30)
    lfs f2, 0x4(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x4(r31)
    lfs f1, 0x8(r30)
    lfs f2, 0x8(r29)
    fmuls f1, f0, f1
    fmadds f1, f31, f2, f1
    stfs f1, 0x8(r31)
    lfs f1, 0xc(r30)
    lfs f2, 0xc(r29)
    fmuls f0, f0, f1
    fmadds f0, f31, f2, f0
    stfs f0, 0xc(r31)
@L_801ADA74:
    li r3, 0x1
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
