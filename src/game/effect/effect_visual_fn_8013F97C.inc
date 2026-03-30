    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stw r31, 0x9c(r1)
    stw r30, 0x98(r1)
    stw r29, 0x94(r1)
    stw r28, 0x90(r1)
    mr r28, r3
    bl fn_800D2584
    addi r30, r28, 0x38
    mr r31, r3
    bl fn_800D2248
    li r3, 0x0
    bl fn_800D7BF8
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_800E064C
    cmplwi r31, 0x0
    beq @L_8013FA0C
    mr r3, r31
    addi r4, r1, 0x8
    addi r5, r1, 0x14
    bl fn_800D1EB8
    mr r4, r29
    addi r3, r1, 0x50
    bl fn_800E0628
    lfs f0, lbl_8047D300(r2)
    mr r3, r30
    addi r4, r1, 0x50
    stfs f0, 0x7c(r1)
    stfs f0, 0x6c(r1)
    stfs f0, 0x5c(r1)
    bl fn_800E0238
    @L_8013FA0C:
    lfs f1, 0x10(r28)
    mr r3, r30
    fmr f2, f1
    fmr f3, f1
    bl fn_800E02C4
    lfs f0, 0x2c(r28)
    addi r3, r1, 0x38
    lfs f1, lbl_8047D304(r2)
    stfs f0, 0xc(r30)
    fmr f2, f1
    lfs f3, lbl_8047D300(r2)
    lfs f0, 0x30(r28)
    stfs f0, 0x1c(r30)
    lfs f0, 0x34(r28)
    stfs f0, 0x2c(r30)
    bl fn_800E01F4
    addi r3, r1, 0x38
    mr r4, r30
    mr r5, r3
    bl fn_800DFF98
    lfs f1, lbl_8047D308(r2)
    addi r31, r1, 0x44
    lfs f3, lbl_8047D300(r2)
    mr r3, r31
    fmr f2, f1
    bl fn_800E01F4
    mr r3, r31
    mr r4, r30
    mr r5, r31
    bl fn_800DFF98
    addi r3, r1, 0x38
    addi r4, r1, 0x20
    li r5, 0x2
    bl fn_800D2DE8
    cmpwi r3, 0x2
    beq @L_8013FAB0
    lis r3, lbl_8027301C@ha
    addi r3, r3, lbl_8027301C@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_8013FBB8
    @L_8013FAB0:
    lfs f0, 0x20(r1)
    addi r3, r28, 0x68
    lfs f8, lbl_8047D318(r2)
    lfs f4, 0x2c(r1)
    fdivs f7, f0, f8
    lfs f2, 0x24(r1)
    lfs f6, lbl_8047D31C(r2)
    lfs f1, 0x30(r1)
    lfs f0, lbl_8047D308(r2)
    lfs f3, lbl_8047D300(r2)
    fdivs f5, f2, f6
    stfs f7, 0x20(r1)
    lfs f2, lbl_8047D320(r2)
    fdivs f4, f4, f8
    stfs f5, 0x24(r1)
    fdivs f1, f1, f6
    stfs f4, 0x2c(r1)
    stfs f1, 0x30(r1)
    lfs f1, 0x20(r28)
    fsubs f0, f1, f0
    stfs f2, 0x98(r28)
    stfs f3, 0x9c(r28)
    fmuls f0, f2, f0
    stfs f0, 0xa0(r28)
    stfs f3, 0xa4(r28)
    stfs f2, 0xa8(r28)
    stfs f0, 0xac(r28)
    lfs f4, 0x2c(r1)
    lfs f1, 0x20(r1)
    lfs f2, 0x30(r1)
    lfs f0, 0x24(r1)
    fsubs f1, f4, f1
    fsubs f2, f2, f0
    bl fn_800E048C
    lwz r3, lbl_8047AEE8(r13)
    bl fn_800EF4FC
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0x84(r1)
    lfd f2, lbl_8047D310(r2)
    stw r0, 0x80(r1)
    lfs f0, lbl_8047D324(r2)
    lfd f1, 0x80(r1)
    lwz r3, lbl_8047AEE8(r13)
    fsubs f1, f1, f2
    fdivs f31, f0, f1
    bl fn_800EF4F4
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0x8c(r1)
    addi r3, r28, 0x68
    lfs f5, 0x20(r28)
    stw r0, 0x88(r1)
    lfd f1, lbl_8047D310(r2)
    fmuls f31, f31, f5
    lfd f0, 0x88(r1)
    lfs f2, lbl_8047D324(r2)
    fsubs f4, f0, f1
    lfs f1, 0x20(r1)
    lfs f0, 0x24(r1)
    fadds f1, f31, f1
    lfs f3, lbl_8047D300(r2)
    fdivs f2, f2, f4
    fmuls f2, f2, f5
    fadds f2, f2, f0
    bl fn_800E03E8
    @L_8013FBB8:
    psq_l f31, 0xa8(r1), 0, 0
    lwz r0, 0xb4(r1)
    lfd f31, 0xa0(r1)
    lwz r31, 0x9c(r1)
    lwz r30, 0x98(r1)
    lwz r29, 0x94(r1)
    lwz r28, 0x90(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
