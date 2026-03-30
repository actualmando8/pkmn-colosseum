    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    lbz r0, lbl_8047AD24(r13)
    cmplwi r0, 0x0
    beq @L_80109454
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047CE60(r13)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f2, lbl_8047CE68(r13)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047AD3C(r13)
    lfd f1, 0x10(r1)
    lfs f3, lbl_8047AD38(r13)
    fsubs f1, f1, f2
    fdivs f1, f1, f31
    fadds f0, f0, f1
    fcmpo cr0, f0, f3
    stfs f0, lbl_8047AD3C(r13)
    cror eq, gt, eq
    bne @L_80109454
    li r0, 0x0
    stfs f3, lbl_8047AD3C(r13)
    stb r0, lbl_8047AD24(r13)
@L_80109454:
    lbz r0, lbl_8047AD22(r13)
    cmplwi r0, 0x0
    beq @L_80109648
    lbz r0, lbl_8047AD20(r13)
    cmplwi r0, 0x0
    beq @L_80109648
    lfs f1, lbl_8047AD3C(r13)
    lfs f0, lbl_8047AD38(r13)
    lbz r0, lbl_8047AD23(r13)
    fdivs f3, f1, f0
    lfs f4, lbl_8047AD2C(r13)
    lfs f1, lbl_8047AD34(r13)
    cmplwi r0, 0x0
    lfs f2, lbl_8047CE48(r13)
    lfs f0, lbl_8047CE4C(r13)
    fsubs f1, f1, f4
    fmadds f3, f3, f1, f4
    fmuls f1, f2, f3
    stfs f3, lbl_8047AD30(r13)
    fdivs f0, f1, f0
    fsubs f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r31, 0x14(r1)
    beq @L_801094C4
    li r3, 0x1
    bl fn_800D9ED8
    b @L_801094CC
@L_801094C4:
    li r3, 0x0
    bl fn_800D9ED8
@L_801094CC:
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lfs f1, lbl_8047CE50(r13)
    lfs f3, lbl_8047CE54(r13)
    fmr f2, f1
    lfs f4, lbl_8047CE58(r13)
    bl fn_800D9B58
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x4
    bl fn_800D6A00
    lis r3, lbl_80314AE8@ha
    addi r3, r3, lbl_80314AE8@l
    bl fn_800D7820
    lwz r4, lbl_8047AD28(r13)
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, lbl_8047CE50(r13)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    mr r4, r31
    mr r5, r31
    mr r6, r31
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047CE50(r13)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f2, lbl_8047CE50(r13)
    lfs f1, lbl_8047CE54(r13)
    fmr f3, f2
    bl fn_800D6680
    mr r4, r31
    mr r5, r31
    mr r6, r31
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047CE5C(r13)
    li r3, 0x0
    lfs f2, lbl_8047CE50(r13)
    bl fn_800D59B8
    lfs f1, lbl_8047CE50(r13)
    lfs f2, lbl_8047CE58(r13)
    fmr f3, f1
    bl fn_800D6680
    mr r4, r31
    mr r5, r31
    mr r6, r31
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047CE50(r13)
    li r3, 0x0
    lfs f2, lbl_8047CE5C(r13)
    bl fn_800D59B8
    lfs f1, lbl_8047CE54(r13)
    lfs f2, lbl_8047CE58(r13)
    lfs f3, lbl_8047CE50(r13)
    bl fn_800D6680
    mr r4, r31
    mr r5, r31
    mr r6, r31
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047CE5C(r13)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    lbz r0, lbl_8047AD23(r13)
    cmplwi r0, 0x0
    bne @L_80109648
    li r3, 0x0
    bl fn_800D9F40
@L_80109648:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

