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
    stw r31, 0x1c(r1)
    fmr f27, f1
    lfs f28, lbl_8047B7C8(r2)
    lfd f29, lbl_8047B7D0(r2)
    lis r31, 0x4330
    lfd f31, lbl_8047B7D8(r2)
    b @L_8001D7A0
    @L_8001D768:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f29
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f31
    fdivs f0, f0, f30
    fadds f28, f28, f0
    @L_8001D7A0:
    fcmpo cr0, f28, f27
    blt @L_8001D768
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
    lwz r0, 0x74(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
