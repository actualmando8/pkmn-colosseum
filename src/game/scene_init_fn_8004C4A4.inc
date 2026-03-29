    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r4
    lwz r30, 0x60(r3)
    bl fn_801D1F7C
    lis r4, 0x6666
    addi r0, r3, 0x9
    addi r3, r4, 0x6667
    mulhw r0, r3, r0
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r0, r0, r3
    cmpwi r0, 0x1
    bgt @L_8004C4F8
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8004C504
    @L_8004C4F8:
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    @L_8004C504:
    lha r0, 0x6(r31)
    cmpwi r0, 0x444
    bne @L_8004C554
    lwz r3, 0x4(r30)
    lis r0, 0x4330
    lwz r4, 0x0(r30)
    xoris r3, r3, 0x8000
    stw r0, 0x8(r1)
    lfd f1, lbl_8047BE10(r2)
    stw r3, 0xc(r1)
    lfs f3, lbl_8047BE08(r2)
    lfd f0, 0x8(r1)
    lfs f2, 0x0(r4)
    fsubs f0, f0, f1
    fmadds f0, f3, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x50(r31)
    b @L_8004C594
    @L_8004C554:
    lwz r3, 0x8(r30)
    lis r0, 0x4330
    lwz r4, 0x0(r30)
    xoris r3, r3, 0x8000
    stw r0, 0x10(r1)
    lfd f1, lbl_8047BE10(r2)
    stw r3, 0x14(r1)
    lfs f3, lbl_8047BE0C(r2)
    lfd f0, 0x10(r1)
    lfs f2, 0x0(r4)
    fsubs f0, f0, f1
    fmadds f0, f3, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r31)
    @L_8004C594:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
