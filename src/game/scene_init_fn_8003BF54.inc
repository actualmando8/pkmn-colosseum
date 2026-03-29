    nofralloc
    stwu r1, -0x30(r1)
    lis r5, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r7, r5, lbl_803A6748@l
    lfs f0, 0x44(r7)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lwz r0, 0x10(r7)
    cmpwi r0, 0xa
    ble @L_8003BF9C
    lbz r0, 0x4(r4)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8003BFB0
    @L_8003BF9C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8003C034
    @L_8003BFB0:
    lwz r3, 0x10(r7)
    lis r5, 0x4330
    lis r6, lbl_802EF0A8@ha
    stw r5, 0x8(r1)
    subi r0, r3, 0xa
    lfd f3, lbl_8047BAE0(r2)
    xoris r0, r0, 0x8000
    addi r6, r6, lbl_802EF0A8@l
    stw r0, 0xc(r1)
    lha r3, 0x5088(r6)
    lfd f0, 0x8(r1)
    lha r0, 0x5084(r6)
    xoris r3, r3, 0x8000
    fsubs f0, f0, f3
    lfs f1, lbl_8047BAD8(r2)
    xoris r0, r0, 0x8000
    lfs f4, 0x18(r7)
    stw r3, 0x14(r1)
    fmuls f0, f1, f0
    stw r5, 0x10(r1)
    fneg f2, f0
    lfd f0, 0x10(r1)
    stw r0, 0x1c(r1)
    fsubs f1, f0, f3
    fdivs f2, f4, f2
    stw r5, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f3
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0x52(r4)
    @L_8003C034:
    addi r1, r1, 0x30
    blr
