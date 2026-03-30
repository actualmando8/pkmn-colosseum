    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    cmplwi r5, 0x0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    beq @L_8013E454
    lwz r6, 0x18(r5)
    lis r3, 0x4330
    lwz r4, 0x1c(r5)
    lwz r0, 0x8(r6)
    lfs f1, lbl_8047D27C(r2)
    stw r4, 0xc(r1)
    lfd f5, lbl_8047D288(r2)
    fmr f2, f1
    stw r3, 0x8(r1)
    lfs f8, 0x0(r6)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f6, f0, f5
    lfs f0, 0x4(r6)
    stw r3, 0x10(r1)
    fsubs f0, f0, f8
    lfs f7, lbl_8047D298(r2)
    lfd f4, 0x10(r1)
    lfs f3, lbl_8047D29C(r2)
    fsubs f5, f4, f5
    lfs f4, lbl_8047D2A0(r2)
    fdivs f5, f6, f5
    fmadds f0, f5, f0, f8
    fmuls f0, f7, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r31, 0x1c(r1)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    bl fn_800DA1E8
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lis r3, lbl_80314AE8@ha
    addi r3, r3, lbl_80314AE8@l
    bl fn_800D7820
    mr r4, r30
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, lbl_8047D27C(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047D27C(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f2, lbl_8047D27C(r2)
    lfs f1, lbl_8047D29C(r2)
    fmr f3, f2
    bl fn_800D6680
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047D280(r2)
    li r3, 0x0
    lfs f2, lbl_8047D27C(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D27C(r2)
    lfs f2, lbl_8047D2A0(r2)
    fmr f3, f1
    bl fn_800D6680
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047D27C(r2)
    li r3, 0x0
    lfs f2, lbl_8047D280(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D29C(r2)
    lfs f2, lbl_8047D2A0(r2)
    lfs f3, lbl_8047D27C(r2)
    bl fn_800D6680
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047D280(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x1
    b @L_8013E458
    @L_8013E454:
    li r3, 0x0
    @L_8013E458:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
