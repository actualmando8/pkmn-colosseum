    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stmw r27, 0x3c(r1)
    mr r27, r3
    mr r28, r5
    mr r29, r6
    lha r0, 0x56(r4)
    lis r3, 0x4330
    lha r5, 0x54(r4)
    addi r6, r1, 0xc
    xoris r0, r0, 0x8000
    lfs f4, lbl_8047B748(r2)
    stw r0, 0x2c(r1)
    xoris r0, r5, 0x8000
    lfs f0, lbl_8047B744(r2)
    li r31, 0x0
    stw r3, 0x28(r1)
    lfd f7, lbl_8047B768(r2)
    lfd f3, 0x28(r1)
    lfs f2, lbl_8047B75C(r2)
    fsubs f29, f3, f7
    stw r0, 0x24(r1)
    lfs f3, lbl_8047B770(r2)
    stw r3, 0x20(r1)
    fmuls f5, f2, f29
    lfs f2, lbl_8047B774(r2)
    lfd f6, 0x20(r1)
    stfs f4, 0x8(r1)
    fsubs f30, f6, f7
    fmuls f3, f5, f3
    stfs f0, 0x18(r1)
    fadds f5, f30, f3
    fmadds f0, f2, f30, f3
    fmuls f6, f2, f5
    fdivs f3, f30, f6
    fdivs f2, f5, f6
    stfs f3, 0xc(r1)
    fdivs f0, f0, f6
    stfs f2, 0x10(r1)
    stfs f0, 0x14(r1)
    frsp f0, f4
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_800168B0
    frsp f0, f3
    fcmpo cr0, f0, f1
    bgt @L_80016914
    @L_800168B0:
    li r31, 0x1
    lfs f0, 0xc(r1)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_800168D0
    lfs f0, 0x10(r1)
    fcmpo cr0, f0, f1
    bgt @L_80016914
    @L_800168D0:
    li r31, 0x2
    lfs f0, 0x10(r1)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_800168F0
    lfs f0, 0x14(r1)
    fcmpo cr0, f0, f1
    bgt @L_80016914
    @L_800168F0:
    li r31, 0x3
    lfs f0, 0x14(r1)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_80016910
    lfs f0, 0x18(r1)
    fcmpo cr0, f0, f1
    bgt @L_80016914
    @L_80016910:
    li r31, 0x4
    @L_80016914:
    slwi r0, r31, 2
    addi r3, r1, 0x8
    lfsx f2, r3, r0
    cmpwi r31, 0x0
    lfsx f0, r6, r0
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fdivs f31, f1, f0
    bne @L_8001694C
    fmuls f0, f31, f30
    li r4, 0x0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r30, 0x2c(r1)
    @L_8001694C:
    cmpwi r31, 0x1
    bne @L_800169BC
    lfs f1, lbl_8047B75C(r2)
    lfs f0, lbl_8047B778(r2)
    fmsubs f28, f1, f31, f0
    fmr f1, f28
    bl fn_800CDBE0
    lfs f2, lbl_8047B774(r2)
    frsp f3, f1
    lfs f0, lbl_8047B770(r2)
    fmr f1, f28
    fsubs f2, f29, f2
    fmuls f2, f2, f3
    fmadds f0, f2, f0, f30
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r30, 0x2c(r1)
    bl fn_800CE148
    lfs f0, lbl_8047B774(r2)
    frsp f2, f1
    lfs f1, lbl_8047B770(r2)
    fsubs f0, f29, f0
    fmuls f0, f0, f2
    fmuls f0, f0, f1
    fmadds f0, f29, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r4, 0x24(r1)
    @L_800169BC:
    cmpwi r31, 0x2
    bne @L_800169F0
    lfs f1, lbl_8047B744(r2)
    lfs f0, lbl_8047B774(r2)
    fsubs f1, f1, f31
    fsubs f0, f29, f0
    fmuls f1, f1, f30
    fctiwz f0, f0
    fctiwz f1, f1
    stfd f0, 0x20(r1)
    stfd f1, 0x28(r1)
    lwz r4, 0x24(r1)
    lwz r30, 0x2c(r1)
    @L_800169F0:
    cmpwi r31, 0x3
    bne @L_80016A60
    lfs f1, lbl_8047B75C(r2)
    lfs f0, lbl_8047B778(r2)
    fmadds f30, f1, f31, f0
    fmr f1, f30
    bl fn_800CDBE0
    lfs f2, lbl_8047B774(r2)
    frsp f3, f1
    lfs f0, lbl_8047B770(r2)
    fmr f1, f30
    fsubs f2, f29, f2
    fmuls f2, f2, f3
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r30, 0x2c(r1)
    bl fn_800CE148
    lfs f0, lbl_8047B774(r2)
    frsp f2, f1
    lfs f1, lbl_8047B770(r2)
    fsubs f0, f29, f0
    fmuls f0, f0, f2
    fmuls f0, f0, f1
    fmadds f0, f29, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r4, 0x24(r1)
    @L_80016A60:
    clrlwi r5, r29, 24
    li r0, -0x100
    or r7, r5, r0
    mr r3, r30
    mr r8, r27
    clrlwi r9, r28, 16
    li r5, 0x2
    li r6, 0x2
    li r10, 0x0
    bl fn_80104160
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    lmw r27, 0x3c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
