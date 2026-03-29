    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stw r31, 0x9c(r1)
    stw r30, 0x98(r1)
    mr r31, r4
    lis r3, lbl_80267338@ha
    addi r8, r3, lbl_80267338@l
    lwz r7, 0x0(r8)
    lwz r6, 0x4(r8)
    lwz r5, 0x8(r8)
    lwz r4, 0xc(r8)
    lwz r3, 0x10(r8)
    lwz r0, 0x14(r8)
    stw r7, 0x8(r1)
    stw r6, 0xc(r1)
    stw r5, 0x10(r1)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_80057E40
    cmpwi r3, 0x2
    beq @L_80053580
    li r30, 0x1
    b @L_80053584
    @L_80053580:
    li r30, 0x0
    @L_80053584:
    mr r3, r31
    clrlwi r4, r30, 24
    bl fn_80109220
    cmpwi r30, 0x0
    beq @L_8005370C
    lha r4, 0x8(r1)
    lis r10, 0x4330
    lha r3, 0x14(r1)
    xoris r0, r4, 0x8000
    lha r5, 0xa(r1)
    subf r3, r4, r3
    lha r4, 0x16(r1)
    xoris r3, r3, 0x8000
    stw r10, 0x60(r1)
    lha r6, 0xc(r1)
    subf r4, r5, r4
    stw r3, 0x64(r1)
    xoris r11, r4, 0x8000
    lfd f3, lbl_8047BE58(r2)
    xoris r9, r5, 0x8000
    lfd f0, 0x60(r1)
    xoris r7, r6, 0x8000
    lha r3, 0x18(r1)
    lha r5, 0xe(r1)
    fsubs f1, f0, f3
    lha r4, 0x1a(r1)
    subf r3, r6, r3
    lwz r12, 0x10(r1)
    xoris r8, r3, 0x8000
    lwz r3, 0x1c(r1)
    subf r6, r5, r4
    xoris r5, r5, 0x8000
    subf r4, r12, r3
    xoris r3, r12, 0x8000
    xoris r6, r6, 0x8000
    stw r0, 0x6c(r1)
    xoris r4, r4, 0x8000
    lfs f4, lbl_8047A540(r13)
    stw r10, 0x68(r1)
    lfd f0, 0x68(r1)
    stw r11, 0x24(r1)
    fsubs f0, f0, f3
    stw r10, 0x20(r1)
    fmadds f0, f4, f1, f0
    lfd f1, 0x20(r1)
    stw r9, 0x2c(r1)
    fsubs f2, f1, f3
    stw r10, 0x28(r1)
    fctiwz f0, f0
    lfd f1, 0x28(r1)
    stfd f0, 0x70(r1)
    fsubs f0, f1, f3
    lwz r0, 0x74(r1)
    stw r8, 0x34(r1)
    fmadds f5, f4, f2, f0
    stw r10, 0x30(r1)
    lfd f0, 0x30(r1)
    stw r7, 0x3c(r1)
    fsubs f2, f0, f3
    stw r10, 0x38(r1)
    lfd f0, 0x38(r1)
    stw r6, 0x44(r1)
    fsubs f1, f0, f3
    stw r10, 0x40(r1)
    lfd f0, 0x40(r1)
    fmadds f6, f4, f2, f1
    stw r5, 0x4c(r1)
    fsubs f2, f0, f3
    stw r10, 0x48(r1)
    lfd f0, 0x48(r1)
    stw r4, 0x54(r1)
    fsubs f1, f0, f3
    stw r10, 0x50(r1)
    lfd f0, 0x50(r1)
    fmadds f7, f4, f2, f1
    stw r3, 0x5c(r1)
    fsubs f1, f0, f3
    stw r10, 0x58(r1)
    lfd f0, 0x58(r1)
    sth r0, 0x50(r31)
    fsubs f0, f0, f3
    fmadds f3, f4, f1, f0
    fctiwz f0, f5
    fctiwz f2, f6
    fctiwz f1, f7
    stfd f0, 0x78(r1)
    fctiwz f0, f3
    stfd f2, 0x80(r1)
    lwz r0, 0x7c(r1)
    stfd f1, 0x88(r1)
    lwz r3, 0x84(r1)
    sth r0, 0x52(r31)
    lwz r0, 0x8c(r1)
    sth r3, 0x54(r31)
    stfd f0, 0x90(r1)
    sth r0, 0x56(r31)
    lwz r0, 0x94(r1)
    stb r0, 0x67(r31)
    @L_8005370C:
    li r3, 0x0
    lwz r0, 0xa4(r1)
    lwz r31, 0x9c(r1)
    lwz r30, 0x98(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
