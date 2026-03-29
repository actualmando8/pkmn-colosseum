    nofralloc
    stwu r1, -0x40(r1)
    lha r0, 0x6(r4)
    lis r6, 0x4330
    lwz r9, 0x60(r3)
    lis r3, lbl_802EF0A8@ha
    mulli r5, r0, 0x1c
    stw r6, 0x8(r1)
    addi r0, r3, lbl_802EF0A8@l
    lwz r3, 0x30(r9)
    lwz r8, 0x28(r9)
    add r7, r0, r5
    lha r5, 0x6(r7)
    lha r0, 0x8(r7)
    xoris r5, r5, 0x8000
    lwz r7, 0x0(r8)
    xoris r0, r0, 0x8000
    stw r5, 0xc(r1)
    lwz r5, 0x2c(r9)
    cmpwi r7, 0xf
    stw r0, 0x14(r1)
    lfd f7, lbl_8047B948(r2)
    stw r6, 0x10(r1)
    lfd f1, 0x8(r1)
    lfd f0, 0x10(r1)
    fsubs f3, f1, f7
    lfs f8, 0x0(r3)
    fsubs f4, f0, f7
    lfs f2, lbl_8047B93C(r2)
    lwz r8, 0x0(r5)
    fmuls f1, f3, f8
    fmuls f0, f4, f8
    fmuls f9, f2, f1
    fmuls f10, f2, f0
    bge @L_800275E0
    lwz r3, 0x38(r9)
    fadds f1, f3, f9
    mulli r0, r7, 0x1b
    lwz r5, 0x3c(r9)
    lwz r3, 0x0(r3)
    fadds f0, f4, f10
    lfs f5, lbl_8047B934(r2)
    add r0, r3, r0
    xoris r0, r0, 0x8000
    lwz r5, 0x0(r5)
    mulli r3, r8, 0x23
    stw r0, 0xc(r1)
    fctiwz f1, f1
    lfs f4, lbl_8047B940(r2)
    stw r6, 0x8(r1)
    fctiwz f0, f0
    add r0, r5, r3
    lfd f2, 0x8(r1)
    xoris r0, r0, 0x8000
    stw r6, 0x20(r1)
    fsubs f3, f2, f7
    lfs f6, lbl_8047B938(r2)
    stw r0, 0x24(r1)
    fsubs f5, f5, f8
    lfd f2, 0x20(r1)
    fnmsubs f3, f9, f4, f3
    fmuls f5, f6, f5
    stfd f1, 0x30(r1)
    fsubs f1, f2, f7
    fctiwz f2, f3
    stfd f0, 0x38(r1)
    fctiwz f3, f5
    fnmsubs f0, f10, f4, f1
    lwz r3, 0x34(r1)
    stfd f2, 0x18(r1)
    lwz r0, 0x3c(r1)
    fctiwz f0, f0
    lwz r5, 0x1c(r1)
    stfd f3, 0x10(r1)
    stfd f0, 0x28(r1)
    lwz r6, 0x14(r1)
    sth r5, 0x50(r4)
    lwz r5, 0x2c(r1)
    sth r5, 0x52(r4)
    sth r3, 0x54(r4)
    sth r0, 0x56(r4)
    b @L_800275E4
    @L_800275E0:
    li r6, 0x0
    @L_800275E4:
    stb r6, 0x67(r4)
    li r3, 0x0
    addi r1, r1, 0x40
    blr
