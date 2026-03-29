    nofralloc
    stwu r1, -0x60(r1)
    lwz r8, 0x60(r3)
    lwz r6, lbl_8047B928(r2)
    lwz r3, 0x28(r8)
    lwz r5, lbl_8047B92C(r2)
    lwz r0, 0x0(r3)
    lwz r3, 0x2c(r8)
    cmpwi r0, 0xf
    stw r6, 0x8(r1)
    lwz r0, 0x0(r3)
    stw r5, 0xc(r1)
    bge @L_80027028
    lis r3, 0x1
    subi r0, r3, 0x1
    b @L_80027050
    @L_80027028:
    cmpwi r0, 0x0
    blt @L_80027038
    cmpwi r0, 0x4
    blt @L_80027044
    @L_80027038:
    lis r3, 0x1
    subi r0, r3, 0x1
    b @L_80027050
    @L_80027044:
    slwi r0, r0, 1
    addi r3, r1, 0x8
    lhzx r0, r3, r0
    @L_80027050:
    lha r3, 0x6(r4)
    clrlwi r0, r0, 16
    cmpw r0, r3
    bne @L_80027168
    mulli r6, r3, 0x1c
    lis r7, 0x4330
    lis r3, lbl_802EF0A8@ha
    lwz r5, 0x30(r8)
    stw r7, 0x10(r1)
    addi r0, r3, lbl_802EF0A8@l
    add r8, r0, r6
    stw r7, 0x18(r1)
    lha r0, 0x6(r8)
    lha r3, 0x8(r8)
    xoris r6, r0, 0x8000
    lha r0, 0x2(r8)
    stw r6, 0x14(r1)
    xoris r6, r3, 0x8000
    lfd f5, lbl_8047B948(r2)
    xoris r3, r0, 0x8000
    stw r6, 0x1c(r1)
    lfd f1, 0x10(r1)
    lfd f0, 0x18(r1)
    lha r0, 0x4(r8)
    fsubs f9, f1, f5
    lfs f10, 0x0(r5)
    fsubs f8, f0, f5
    xoris r0, r0, 0x8000
    stw r3, 0x2c(r1)
    fmuls f2, f9, f10
    stw r7, 0x28(r1)
    fmuls f1, f8, f10
    lfs f3, lbl_8047B93C(r2)
    lfd f0, 0x28(r1)
    fmuls f6, f3, f2
    stw r0, 0x3c(r1)
    fmuls f7, f3, f1
    lfs f4, lbl_8047B940(r2)
    stw r7, 0x38(r1)
    fsubs f2, f0, f5
    lfd f0, 0x38(r1)
    fadds f1, f9, f6
    fnmsubs f2, f6, f4, f2
    lfs f3, lbl_8047B934(r2)
    fsubs f0, f0, f5
    lfs f6, lbl_8047B938(r2)
    fsubs f5, f3, f10
    fctiwz f3, f2
    fnmsubs f2, f7, f4, f0
    fadds f0, f8, f7
    stfd f3, 0x30(r1)
    fctiwz f1, f1
    fctiwz f2, f2
    fmuls f3, f6, f5
    lwz r0, 0x34(r1)
    fctiwz f0, f0
    stfd f1, 0x48(r1)
    stfd f2, 0x40(r1)
    fctiwz f1, f3
    lwz r3, 0x4c(r1)
    stfd f0, 0x50(r1)
    lwz r5, 0x44(r1)
    sth r0, 0x50(r4)
    lwz r0, 0x54(r1)
    sth r5, 0x52(r4)
    stfd f1, 0x20(r1)
    sth r3, 0x54(r4)
    lwz r3, 0x24(r1)
    sth r0, 0x56(r4)
    b @L_8002716C
    @L_80027168:
    li r3, 0x0
    @L_8002716C:
    stb r3, 0x67(r4)
    li r3, 0x0
    addi r1, r1, 0x60
    blr
