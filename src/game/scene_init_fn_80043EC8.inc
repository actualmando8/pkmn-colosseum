    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lis r4, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x4c(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80043F20
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80043F34
    @L_80043F20:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80043F94
    @L_80043F34:
    lfs f1, lbl_8047BCA8(r2)
    lfs f0, 0x40(r4)
    lfs f2, lbl_8047BCA4(r2)
    fmuls f0, f1, f0
    fmuls f0, f1, f0
    fmadds f1, f2, f0, f2
    bl fn_800CE148
    lis r3, lbl_802EF0A8@ha
    lis r0, 0x4330
    addi r3, r3, lbl_802EF0A8@l
    stw r0, 0x8(r1)
    lha r0, 0x57bc(r3)
    frsp f3, f1
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BD0C(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fmadds f0, f2, f3, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r31)
    @L_80043F94:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
