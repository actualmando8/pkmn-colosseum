    nofralloc
    stwu r1, -0x20(r1)
    lis r6, lbl_803A6748@ha
    lis r5, lbl_802EF0A8@ha
    addi r6, r6, lbl_803A6748@l
    lfs f1, lbl_8047BAC0(r2)
    lfs f0, 0x44(r6)
    addi r5, r5, lbl_802EF0A8@l
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lwz r3, 0x8(r6)
    lwz r0, 0x0(r6)
    lha r6, 0x50a0(r5)
    subf r0, r3, r0
    cmpwi r0, 0xa
    blt @L_8003C774
    li r0, 0x9
    @L_8003C774:
    mulli r5, r0, 0x18
    lis r3, lbl_803A6748@ha
    lis r0, 0x4330
    lfd f2, lbl_8047BAE0(r2)
    addi r3, r3, lbl_803A6748@l
    stw r0, 0x10(r1)
    add r0, r6, r5
    lfs f0, 0x30(r3)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    fctiwz f0, f0
    lfd f1, 0x10(r1)
    stfd f0, 0x18(r1)
    fsubs f0, f1, f2
    lwz r0, 0x1c(r1)
    stfs f0, 0x38(r3)
    sth r0, 0x52(r4)
    addi r1, r1, 0x20
    blr
