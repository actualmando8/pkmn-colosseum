    nofralloc
    stwu r1, -0x30(r1)
    lis r5, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r6, r5, lbl_803A6818@l
    lfs f0, 0x4c(r6)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lwz r0, 0x10(r6)
    cmpwi r0, 0xa
    ble @L_80043D20
    lbz r0, 0x4(r4)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_80043D34
    @L_80043D20:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_80043DB8
    @L_80043D34:
    lwz r3, 0x10(r6)
    lis r5, 0x4330
    lfs f0, 0x30(r6)
    lis r6, lbl_802EF0A8@ha
    subi r0, r3, 0x1
    stw r5, 0x8(r1)
    xoris r0, r0, 0x8000
    addi r6, r6, lbl_802EF0A8@l
    stw r0, 0xc(r1)
    fneg f1, f0
    lha r0, 0x5788(r6)
    lfd f4, lbl_8047BCB0(r2)
    lfd f0, 0x8(r1)
    xoris r3, r0, 0x8000
    lha r0, 0x5784(r6)
    fsubs f0, f0, f4
    lfs f2, lbl_8047BCF4(r2)
    xoris r0, r0, 0x8000
    stw r3, 0x14(r1)
    fdivs f3, f1, f0
    stw r5, 0x10(r1)
    lfd f1, 0x10(r1)
    stw r0, 0x1c(r1)
    stw r5, 0x18(r1)
    lfd f0, 0x18(r1)
    fdivs f2, f3, f2
    fsubs f1, f1, f4
    fsubs f0, f0, f4
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0x52(r4)
    @L_80043DB8:
    addi r1, r1, 0x30
    blr
