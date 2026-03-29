    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stmw r24, 0x60(r1)
    mr r28, r3
    lbz r3, 0x58(r28)
    lfs f1, lbl_8047CD34(r2)
    addi r0, r3, 0x1
    stb r0, 0x58(r28)
    lbz r4, 0x5b(r28)
    lbz r0, 0x58(r28)
    lbz r3, 0x5a(r28)
    mullw r4, r4, r0
    lbz r0, 0x59(r28)
    lfs f31, 0xc(r28)
    lfs f30, 0x10(r28)
    lfs f0, 0x4c(r28)
    stfs f0, 0xc(r28)
    divw r3, r4, r3
    lfs f0, 0x50(r28)
    stfs f0, 0x10(r28)
    lfs f0, 0x60(r28)
    fmuls f0, f0, f1
    stfs f0, 0x60(r28)
    lfs f0, 0x64(r28)
    clrlwi r31, r3, 24
    subf r0, r0, r31
    fmuls f0, f0, f1
    clrlwi r30, r0, 24
    stfs f0, 0x64(r28)
    lwz r29, 0x54(r28)
    b @L_800FD630
@L_800FD3D8:
    lhz r4, 0x0(r29)
    addi r29, r29, 0x2
    cmplwi r4, 0xffff
    bne @L_800FD3F0
    addi r29, r29, 0x1
    b @L_800FD630
@L_800FD3F0:
    cmplwi r4, 0x20
    bne @L_800FD42C
    lbz r3, 0x22(r28)
    lis r0, 0x4330
    stw r0, 0x10(r1)
    srwi r0, r3, 1
    lfd f2, lbl_8047CD10(r2)
    xoris r0, r0, 0x8000
    lfs f0, 0x60(r28)
    stw r0, 0x14(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r28)
    b @L_800FD61C
@L_800FD42C:
    mr r3, r28
    addi r5, r1, 0x8
    bl fn_800FDF1C
    mr r6, r3
    cmplwi r6, 0x0
    bne @L_800FD5BC
    lfs f0, 0x10(r28)
    lis r6, 0x4330
    lfs f1, 0xc(r28)
    lis r3, 0x8000
    fctiwz f0, f0
    lbz r0, 0x22(r28)
    fctiwz f1, f1
    lbz r4, 0x23(r28)
    stw r0, 0x24(r1)
    addi r3, r3, 0x2
    stfd f0, 0x18(r1)
    lfd f6, lbl_8047CD28(r2)
    stfd f1, 0x10(r1)
    lwz r5, 0x1c(r1)
    lwz r24, 0x14(r1)
    addi r0, r5, 0x2
    stw r6, 0x20(r1)
    extsh r26, r0
    extsh r0, r24
    xoris r5, r0, 0x8000
    stw r6, 0x28(r1)
    xoris r0, r26, 0x8000
    lfd f0, 0x20(r1)
    stw r5, 0x2c(r1)
    lfd f4, lbl_8047CD10(r2)
    fsubs f5, f0, f6
    lfd f0, 0x28(r1)
    stw r4, 0x3c(r1)
    fsubs f2, f0, f4
    lfs f3, 0x60(r28)
    stw r6, 0x38(r1)
    lfs f1, 0x64(r28)
    lfd f0, 0x38(r1)
    fmadds f3, f5, f3, f2
    stw r0, 0x44(r1)
    fsubs f2, f0, f6
    stw r6, 0x40(r1)
    fctiwz f3, f3
    lfd f0, 0x40(r1)
    stfd f3, 0x30(r1)
    fsubs f0, f0, f4
    lwz r25, 0x34(r1)
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r27, 0x4c(r1)
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    mr r3, r24
    mr r4, r26
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    mr r3, r25
    mr r4, r27
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
    lbz r3, 0x22(r28)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0x54(r1)
    lfs f0, 0x60(r28)
    lfd f1, 0x50(r1)
    lfs f3, lbl_8047CD30(r2)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fadds f0, f3, f0
    stfs f0, 0x14(r28)
    b @L_800FD61C
@L_800FD5BC:
    lwz r8, 0x8(r1)
    mr r3, r28
    lwz r0, 0x4(r6)
    lwz r5, 0x4(r8)
    clrlwi r4, r0, 8
    lbz r27, 0x2(r6)
    add r4, r5, r4
    srwi r0, r0, 24
    lbz r6, 0x3(r6)
    extsb r7, r0
    mr r5, r27
    add r4, r8, r4
    bl fn_800FD69C
    extsh r3, r27
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x50(r1)
    lfd f2, lbl_8047CD10(r2)
    stw r3, 0x54(r1)
    lfs f0, 0x60(r28)
    lfd f1, 0x50(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r28)
@L_800FD61C:
    lfs f1, 0xc(r28)
    subi r30, r30, 0x1
    lfs f0, 0x14(r28)
    fadds f0, f1, f0
    stfs f0, 0xc(r28)
@L_800FD630:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    bne @L_800FD3D8
    lfs f0, 0xc(r28)
    lfs f1, lbl_8047CD30(r2)
    stfs f0, 0x4c(r28)
    lfs f0, 0x10(r28)
    stfs f0, 0x50(r28)
    stw r29, 0x54(r28)
    stb r31, 0x59(r28)
    stfs f31, 0xc(r28)
    stfs f30, 0x10(r28)
    lfs f0, 0x60(r28)
    fmuls f0, f0, f1
    stfs f0, 0x60(r28)
    lfs f0, 0x64(r28)
    fmuls f0, f0, f1
    stfs f0, 0x64(r28)
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    lmw r24, 0x60(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
