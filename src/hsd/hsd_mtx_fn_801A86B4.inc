    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    mr r31, r7
    stw r30, 0x48(r1)
    mr r30, r6
    stw r29, 0x44(r1)
    mr r29, r5
    stw r28, 0x40(r1)
    mr r28, r3
    lfs f1, 0x0(r4)
    lfs f2, 0x4(r4)
    lfs f3, 0x8(r4)
    bl fn_800A3334
    mr r4, r29
    addi r3, r1, 0x8
    bl fn_800A33B4
    cmplwi r31, 0x0
    beq @L_801A883C
    lfs f2, 0x0(r31)
    lfs f0, -23108(r2)
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_801A8730
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fadds f0, f2, f0
    fdivs f6, f1, f0
    b @L_801A8744
@L_801A8730:
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fsubs f0, f2, f0
    fdivs f6, f1, f0
@L_801A8744:
    lfs f2, 0x4(r31)
    lfs f0, -23108(r2)
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_801A8770
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fadds f0, f2, f0
    fdivs f7, f1, f0
    b @L_801A8784
@L_801A8770:
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fsubs f0, f2, f0
    fdivs f7, f1, f0
@L_801A8784:
    lfs f2, 0x8(r31)
    lfs f0, -23108(r2)
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_801A87B0
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fadds f0, f2, f0
    fdivs f8, f1, f0
    b @L_801A87C4
@L_801A87B0:
    lis r3, 0x8048
    lfs f1, -23112(r2)
    lfs f0, -30004(r3)
    fsubs f0, f2, f0
    fdivs f8, f1, f0
@L_801A87C4:
    lfs f0, 0x4(r31)
    lfs f1, 0xc(r1)
    fmuls f0, f0, f6
    lfs f5, 0x10(r1)
    lfs f4, 0x18(r1)
    lfs f3, 0x20(r1)
    fmuls f0, f1, f0
    lfs f2, 0x28(r1)
    lfs f1, 0x2c(r1)
    stfs f0, 0xc(r1)
    lfs f0, 0x4(r31)
    fmuls f0, f0, f6
    fmuls f0, f5, f0
    stfs f0, 0x10(r1)
    lfs f0, 0x0(r31)
    fmuls f0, f0, f7
    fmuls f0, f4, f0
    stfs f0, 0x18(r1)
    lfs f0, 0x8(r31)
    fmuls f0, f0, f7
    fmuls f0, f3, f0
    stfs f0, 0x20(r1)
    lfs f0, 0x0(r31)
    fmuls f0, f0, f8
    fmuls f0, f2, f0
    stfs f0, 0x28(r1)
    lfs f0, 0x4(r31)
    fmuls f0, f0, f8
    fmuls f0, f1, f0
    stfs f0, 0x2c(r1)
@L_801A883C:
    mr r4, r28
    mr r5, r28
    addi r3, r1, 0x8
    bl fn_800A2D98
    lfs f0, 0x0(r30)
    stfs f0, 0xc(r28)
    lfs f0, 0x4(r30)
    stfs f0, 0x1c(r28)
    lfs f0, 0x8(r30)
    stfs f0, 0x2c(r28)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r28, 0x40(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
