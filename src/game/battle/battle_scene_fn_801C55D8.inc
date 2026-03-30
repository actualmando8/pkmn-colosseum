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
    stfd f28, 0x10(r1)
    psq_st f28, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    fmr f28, f1
    mr r30, r3
    fmr f31, f2
    fmr f29, f3
    fmr f30, f4
    mr r31, r4
    cmplwi r31, 0x0
    bne @L_801C5630
    b @L_801C5710
@L_801C5630:
    lbz r0, -21616(r13)
    cmplwi r0, 0x1
    bne @L_801C5648
    li r0, 0x0
    stb r0, -21616(r13)
    bl fn_801C5748
@L_801C5648:
    lbz r0, -21616(r13)
    cmplwi r0, 0x0
    bne @L_801C5664
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C5664
    bl fn_801C70FC
@L_801C5664:
    fdivs f31, f28, f31
    lis r3, -32698
    addi r4, r3, 28720
    lfs f0, -22208(r2)
    lfs f2, 0x10(r4)
    lfs f1, 0x14(r4)
    fadds f1, f1, f2
    lhz r3, 0x8(r4)
    fdivs f30, f29, f30
    addi r0, r3, 0x1
    sth r0, 0x8(r4)
    stfs f1, 0x14(r4)
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_801C56C4
    lfs f0, -22212(r2)
    lfs f3, 0x18(r4)
    fmadds f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r4)
    cror eq, gt, eq
    bne @L_801C56E4
    stfs f3, 0x14(r4)
    b @L_801C56E4
@L_801C56C4:
    lfs f0, -22212(r2)
    lfs f3, 0x18(r4)
    fnmsubs f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r4)
    cror eq, lt, eq
    bne @L_801C56E4
    stfs f3, 0x14(r4)
@L_801C56E4:
    fmr f1, f31
    bl fn_801C6688
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801C570C
    bl fn_801C6EE4
    fmr f1, f31
    mr r3, r31
    fmr f2, f30
    bl fn_801C6934
@L_801C570C:
    mr r3, r30
@L_801C5710:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    psq_l f28, 0x18(r1), 0, 0
    lfd f28, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r0, 0x54(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
