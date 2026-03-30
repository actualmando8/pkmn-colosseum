    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    mr r3, r5
    mr r30, r4
    mr r31, r6
    addi r5, r1, 0x8
    bl fn_800A3A9C
    lfs f5, 0x8(r1)
    lfs f4, 0xc(r1)
    fmuls f2, f5, f5
    lfs f6, 0x10(r1)
    fmuls f0, f4, f4
    lfs f1, -26512(r2)
    fmuls f3, f6, f6
    fadds f0, f2, f0
    fadds f7, f3, f0
    fcmpu cr0, f1, f7
    bne @L_8010F624
    lfs f0, 0x0(r30)
    stfs f0, 0x0(r29)
    lfs f0, 0x4(r30)
    stfs f0, 0x4(r29)
    lfs f0, 0x8(r30)
    stfs f0, 0x8(r29)
    b @L_8010F67C
@L_8010F624:
    lfs f1, 0x4(r31)
    addi r3, r1, 0x8
    lfs f0, 0x4(r30)
    mr r4, r3
    lfs f2, 0x0(r31)
    fsubs f0, f1, f0
    lfs f1, 0x0(r30)
    lfs f3, 0x8(r31)
    fsubs f1, f2, f1
    lfs f2, 0x8(r30)
    fmuls f0, f4, f0
    fsubs f2, f3, f2
    fmadds f0, f5, f1, f0
    fmadds f0, f6, f2, f0
    fdivs f31, f0, f7
    fmr f1, f31
    bl fn_800A3AC0
    mr r4, r30
    mr r5, r29
    addi r3, r1, 0x8
    bl fn_800A3A78
    fmr f1, f31
@L_8010F67C:
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

