    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    mr r31, r4
    lbz r6, 0x8b(r3)
    lis r5, 0x8102
    lbz r0, 0x67(r31)
    lis r4, 0x4330
    lha r3, 0x6(r31)
    subi r7, r5, 0x7dfd
    mullw r5, r6, r0
    lwz r8, lbl_8047CDF0(r13)
    subi r0, r3, 0x68
    lwz r6, lbl_8047CDF4(r13)
    stw r4, 0x10(r1)
    lfd f1, lbl_8047CE10(r13)
    mulhw r3, r7, r5
    cmplwi r0, 0x1b
    stw r8, 0x8(r1)
    stw r6, 0xc(r1)
    add r3, r3, r5
    srawi r3, r3, 15
    rlwinm r4, r3, 1, 31, 31
    add r3, r3, r4
    xoris r3, r3, 0x8000
    stw r3, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f31, f0, f1
    bgt @L_801058A8
    lis r3, jumptable_8035B088@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8035B088@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lbz r5, 0xb(r1)
    lis r4, 0x4330
    lbz r0, 0xf(r1)
    li r3, 0x1
    stw r5, 0x14(r1)
    lfd f2, lbl_8047CE18(r13)
    stw r4, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r0, 0x24(r1)
    fsubs f1, f0, f2
    stw r4, 0x20(r1)
    lfd f0, 0x20(r1)
    fmuls f1, f1, f31
    fsubs f0, f0, f2
    fctiwz f1, f1
    fmuls f0, f0, f31
    stfd f1, 0x18(r1)
    fctiwz f0, f0
    lwz r0, 0x1c(r1)
    stb r0, 0xb(r1)
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    stb r0, 0xf(r1)
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x6
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    li r4, 0x0
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    lha r4, 0x56(r31)
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r4, 0x56(r31)
    li r3, 0x0
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    li r4, 0x0
    bl fn_800FE38C
    b @L_801058A8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047CDF8(r13)
    bl fn_800D5648
    li r3, 0x1
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    lfs f0, lbl_8047CDFC(r13)
    li r0, 0xff
    stb r0, 0x8(r1)
    li r30, 0x0
    fmuls f0, f0, f31
    stb r0, 0x9(r1)
    fctiwz f0, f0
    stb r0, 0xa(r1)
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    stb r0, 0xb(r1)
    b @L_8010586C
@L_8010582C:
    li r3, 0x2
    bl fn_800D67BC
    mr r4, r30
    li r3, 0x0
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    mr r4, r30
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    addi r30, r30, 0x4
@L_8010586C:
    lha r0, 0x56(r31)
    extsh r3, r30
    cmpw r3, r0
    blt @L_8010582C
    bl fn_800FE35C
    b @L_801058A8
    lfs f1, 0x70(r31)
    lfs f0, lbl_8047CE00(r13)
    lfd f2, lbl_8047CE08(r13)
    fadds f0, f1, f0
    stfs f0, 0x70(r31)
    lfs f1, 0x70(r31)
    bl fn_800CE318
    frsp f0, f1
    stfs f0, 0x70(r31)
@L_801058A8:
    li r3, 0x0
    psq_l f31, 0x48(r1), 0, 0
    lwz r0, 0x54(r1)
    lfd f31, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
