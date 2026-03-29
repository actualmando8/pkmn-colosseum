    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stmw r27, 0xcc(r1)
    mr r28, r3
    mr r27, r4
    mr r29, r5
    bne cr1, @L_800FAF38
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
@L_800FAF38:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    addi r4, r1, 0xe8
    addi r0, r1, 0x8
    lis r5, lbl_80401DE0@ha
    lis r3, 0x400
    addi r31, r5, lbl_80401DE0@l
    addi r7, r1, 0x6c
    stw r3, 0x6c(r1)
    mr r5, r6
    mr r6, r7
    addi r3, r31, 0x4d0
    stw r4, 0x70(r1)
    li r4, 0xff
    stw r0, 0x74(r1)
    bl fn_800DE680
    addi r4, r31, 0x4d0
    li r0, 0x0
    stb r0, 0xff(r4)
    addi r3, r31, 0xd0
    bl fn_80080ED8
    addi r30, r31, 0x5d0
    li r4, 0x0
    mr r3, r30
    li r5, 0x68
    bl memset
    lis r4, 0x4330
    xoris r0, r28, 0x8000
    stw r0, 0x7c(r1)
    xoris r3, r27, 0x8000
    li r8, 0x1
    mr r7, r30
    stw r4, 0x78(r1)
    addi r5, r31, 0xd0
    lfd f2, lbl_8047CD10(r2)
    li r6, -0x1
    lfd f0, 0x78(r1)
    li r0, 0x2
    stw r3, 0x84(r1)
    li r9, 0x0
    fsubs f1, f0, f2
    lfs f3, lbl_8047CD08(r2)
    stw r6, 0x24(r7)
    lwz r6, lbl_80478B08(r13)
    stw r4, 0x80(r1)
    lfd f0, 0x80(r1)
    stb r8, 0x5d0(r31)
    fsubs f0, f0, f2
    stfs f3, 0x60(r7)
    stfs f3, 0x64(r7)
    stw r5, 0x28(r7)
    stw r5, 0x2c(r7)
    stw r5, 0x30(r7)
    stfs f1, 0x4(r7)
    stfs f0, 0x8(r7)
    stw r29, 0x24(r7)
    stb r8, 0x2(r7)
    sth r0, 0x20(r7)
    lhz r3, 0x4(r6)
    clrlwi r4, r0, 16
    mtctr r3
    cmpwi r3, 0x0
    ble @L_800FB0E0
@L_800FB04C:
    lwz r0, 0x24(r6)
    add r3, r0, r9
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800FB0D8
    lbz r0, 0x2(r3)
    cmplwi r4, 0x0
    stb r0, 0x22(r30)
    lbz r3, 0x3(r3)
    stb r3, 0x23(r30)
    bne @L_800FB084
    li r0, 0xb
    stb r0, 0x42(r30)
    b @L_800FB0E0
@L_800FB084:
    cmplwi r4, 0x1
    beq @L_800FB090
    bne @L_800FB09C
@L_800FB090:
    li r0, 0x6
    stb r0, 0x42(r30)
    b @L_800FB0E0
@L_800FB09C:
    lis r0, 0x4330
    stw r3, 0x84(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r0, 0x80(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x80(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x78(r1)
    lwz r0, 0x7c(r1)
    extsb r0, r0
    stb r0, 0x42(r30)
    b @L_800FB0E0
@L_800FB0D8:
    addi r9, r9, 0x8
    bdnz @L_800FB04C
@L_800FB0E0:
    bl fn_800FE4D4
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    lwz r4, lbl_80478B08(r13)
    li r3, 0x0
    lbz r0, 0x1d(r4)
    extsb r0, r0
    slwi r0, r0, 2
    add r4, r4, r0
    lwz r4, 0xc(r4)
    bl fn_800D85D4
    lwz r3, lbl_80478B08(r13)
    li r4, 0x0
    lbz r0, 0x1d(r3)
    extsb r0, r0
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    bl fn_800EF548
    lwz r4, lbl_80478B08(r13)
    stw r3, 0x14(r4)
    lfs f1, 0x4(r30)
    lfs f0, 0x8(r30)
    stfs f1, 0xc(r30)
    stfs f0, 0x10(r30)
@L_800FB160:
    lwz r3, 0x30(r30)
    lhz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_800FB1A4
    lbz r3, 0x40(r30)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FB184
    b @L_800FB1AC
@L_800FB184:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r30)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r30, r0
    stw r0, 0x30(r30)
    b @L_800FB160
@L_800FB1A4:
    addi r0, r3, 0x2
    stw r0, 0x30(r30)
@L_800FB1AC:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_800FB408
    cmplwi r0, 0xa
    beq @L_800FB160
    cmplwi r0, 0xd
    beq @L_800FB160
    cmplwi r0, 0x20
    bne @L_800FB204
    lbz r3, 0x22(r30)
    lis r0, 0x4330
    stw r0, 0x80(r1)
    srwi r0, r3, 1
    lfd f2, lbl_8047CD10(r2)
    xoris r0, r0, 0x8000
    lfs f0, 0x60(r30)
    stw r0, 0x84(r1)
    lfd f1, 0x80(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r30)
    b @L_800FB3F4
@L_800FB204:
    mr r3, r30
    addi r5, r1, 0x68
    bl fn_800FDF1C
    mr r6, r3
    cmplwi r6, 0x0
    bne @L_800FB394
    lfs f0, 0x10(r30)
    lis r6, 0x4330
    lfs f1, 0xc(r30)
    lis r3, 0x8000
    fctiwz f0, f0
    lbz r0, 0x22(r30)
    fctiwz f1, f1
    lbz r4, 0x23(r30)
    stw r0, 0x8c(r1)
    addi r3, r3, 0x2
    stfd f0, 0x78(r1)
    lfd f6, lbl_8047CD28(r2)
    stfd f1, 0x80(r1)
    lwz r5, 0x7c(r1)
    lwz r27, 0x84(r1)
    addi r0, r5, 0x2
    stw r6, 0x88(r1)
    extsh r31, r0
    extsh r0, r27
    xoris r5, r0, 0x8000
    stw r6, 0x90(r1)
    xoris r0, r31, 0x8000
    lfd f0, 0x88(r1)
    stw r5, 0x94(r1)
    lfd f4, lbl_8047CD10(r2)
    fsubs f5, f0, f6
    lfd f0, 0x90(r1)
    stw r4, 0xa4(r1)
    fsubs f2, f0, f4
    lfs f3, 0x60(r30)
    stw r6, 0xa0(r1)
    lfs f1, 0x64(r30)
    lfd f0, 0xa0(r1)
    fmadds f3, f5, f3, f2
    stw r0, 0xac(r1)
    fsubs f2, f0, f6
    stw r6, 0xa8(r1)
    fctiwz f3, f3
    lfd f0, 0xa8(r1)
    stfd f3, 0x98(r1)
    fsubs f0, f0, f4
    lwz r28, 0x9c(r1)
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r29, 0xb4(r1)
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    mr r3, r27
    mr r4, r31
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    mr r3, r28
    mr r4, r29
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    lis r3, 0x8000
    addi r3, r3, 0x2
    bl fn_800D88DC
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800DC1D4
    lbz r3, 0x22(r30)
    lis r0, 0x4330
    stw r0, 0xb8(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0xbc(r1)
    lfs f0, 0x60(r30)
    lfd f1, 0xb8(r1)
    lfs f3, lbl_8047CD30(r2)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fadds f0, f3, f0
    stfs f0, 0x14(r30)
    b @L_800FB3F4
@L_800FB394:
    lwz r8, 0x68(r1)
    mr r3, r30
    lwz r0, 0x4(r6)
    lwz r5, 0x4(r8)
    clrlwi r4, r0, 8
    lbz r29, 0x2(r6)
    add r4, r5, r4
    srwi r0, r0, 24
    lbz r6, 0x3(r6)
    extsb r7, r0
    mr r5, r29
    add r4, r8, r4
    bl fn_800FD69C
    extsh r3, r29
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0xb8(r1)
    lfd f2, lbl_8047CD10(r2)
    stw r3, 0xbc(r1)
    lfs f0, 0x60(r30)
    lfd f1, 0xb8(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r30)
@L_800FB3F4:
    lfs f1, 0xc(r30)
    lfs f0, 0x14(r30)
    fadds f0, f1, f0
    stfs f0, 0xc(r30)
    b @L_800FB160
@L_800FB408:
    lwz r3, lbl_80478B08(r13)
    lbz r0, 0x1d(r3)
    extsb r0, r0
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    bl fn_800EF504
    li r3, 0x0
    lmw r27, 0xcc(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
