    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stfd f29, 0x70(r1)
    psq_st f29, 0x78(r1), 0, 0
    stfd f28, 0x60(r1)
    psq_st f28, 0x68(r1), 0, 0
    stmw r24, 0x40(r1)
    mr r25, r3
    mr r24, r4
    lis r3, lbl_80267518@ha
    li r4, 0x0
    lha r6, 0x6(r24)
    addi r8, r3, lbl_80267518@l
    mr r5, r4
    li r28, 0x1
    li r30, 0x0
    @L_800560D8:
    mr r7, r8
    li r3, 0x0
    li r0, 0x3
    mtctr r0
    @L_800560E8:
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_800560FC
    li r4, 0x1
    b @L_80056204
    @L_800560FC:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_80056118
    li r4, 0x1
    b @L_80056204
    @L_80056118:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_80056134
    li r4, 0x1
    b @L_80056204
    @L_80056134:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_80056150
    li r4, 0x1
    b @L_80056204
    @L_80056150:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_8005616C
    li r4, 0x1
    b @L_80056204
    @L_8005616C:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_80056188
    li r4, 0x1
    b @L_80056204
    @L_80056188:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_800561A4
    li r4, 0x1
    b @L_80056204
    @L_800561A4:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_800561C0
    li r4, 0x1
    b @L_80056204
    @L_800561C0:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_800561DC
    li r4, 0x1
    b @L_80056204
    @L_800561DC:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    lwz r0, 0x0(r7)
    cmpw r6, r0
    bne @L_800561F8
    li r4, 0x1
    b @L_80056204
    @L_800561F8:
    addi r7, r7, 0x4
    addi r3, r3, 0x1
    bdnz @L_800560E8
    @L_80056204:
    cmpwi r4, 0x0
    bne @L_8005621C
    addi r8, r8, 0x78
    addi r5, r5, 0x1
    cmpwi r5, 0x3
    blt @L_800560D8
    @L_8005621C:
    cmpwi r4, 0x0
    bne @L_8005622C
    li r5, -0x1
    b @L_80056230
    @L_8005622C:
    mr r31, r3
    @L_80056230:
    cmpwi r5, 0x0
    blt @L_80056278
    extsb r4, r5
    extsb r5, r31
    li r3, 0x0
    bl fn_80134EF0
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_80056278
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80056274
    mr r3, r29
    bl fn_80057F94
    mr r30, r3
    b @L_80056278
    @L_80056274:
    li r28, 0x0
    @L_80056278:
    cmpwi r28, 0x0
    bne @L_80056288
    li r3, 0x0
    b @L_800565DC
    @L_80056288:
    cmplwi r30, 0x0
    beq @L_800565A0
    mr r3, r29
    bl fn_80057DE8
    cmpwi r3, 0x0
    beq @L_800562E4
    lfs f1, lbl_8047A574(r13)
    lfs f0, lbl_8047BEB8(r2)
    fcmpo cr0, f1, f0
    bge @L_800562BC
    lfs f0, lbl_8047BEB0(r2)
    fmuls f1, f0, f1
    b @L_800562CC
    @L_800562BC:
    fsubs f1, f1, f0
    lfs f2, lbl_8047BEB0(r2)
    lfs f0, lbl_8047BEB4(r2)
    fnmsubs f1, f2, f1, f0
    @L_800562CC:
    lfs f0, lbl_8047BEBC(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r29, 0xc(r1)
    b @L_800562E8
    @L_800562E4:
    li r29, 0x0
    @L_800562E8:
    lfs f1, lbl_8047A570(r13)
    lfs f0, lbl_8047BEB4(r2)
    fcmpo cr0, f1, f0
    bge @L_80056394
    lwz r0, lbl_8047A56C(r13)
    cmpw r0, r31
    bne @L_80056394
    lha r5, 0x54(r24)
    lis r3, 0x4330
    lha r6, 0x56(r24)
    fsubs f3, f0, f1
    xoris r4, r5, 0x8000
    stw r3, 0x8(r1)
    xoris r0, r6, 0x8000
    lfd f2, lbl_8047BEA8(r2)
    stw r4, 0xc(r1)
    lfd f0, 0x8(r1)
    stw r0, 0x1c(r1)
    fsubs f1, f0, f2
    stw r3, 0x18(r1)
    lfd f0, 0x18(r1)
    fmuls f1, f3, f1
    fsubs f0, f0, f2
    fctiwz f1, f1
    fmuls f0, f3, f0
    stfd f1, 0x10(r1)
    fctiwz f0, f0
    lwz r26, 0x14(r1)
    extsh r0, r26
    stfd f0, 0x20(r1)
    subf r3, r0, r5
    srwi r0, r3, 31
    lwz r25, 0x24(r1)
    add r0, r0, r3
    srawi r4, r0, 1
    extsh r0, r25
    subf r3, r0, r6
    extsh r28, r4
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r27, r0
    b @L_800563A4
    @L_80056394:
    lha r26, 0x54(r24)
    li r28, 0x0
    lha r25, 0x56(r24)
    li r27, 0x0
    @L_800563A4:
    lis r3, 0x8000
    lha r31, 0x56(r24)
    lha r24, 0x54(r24)
    addi r3, r3, 0x2
    bl fn_800D88DC
    li r3, 0x0
    li r4, 0xf
    li r5, 0xb
    li r6, 0xa
    li r7, 0x8
    bl fn_800DC0D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x0
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x4
    bl fn_800DBFD4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC04C
    mr r3, r30
    bl fn_800EF4FC
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0x24(r1)
    mr r3, r30
    lfd f2, lbl_8047BEA0(r2)
    stw r0, 0x20(r1)
    lfs f0, lbl_8047BEC0(r2)
    lfd f1, 0x20(r1)
    fsubs f1, f1, f2
    fdivs f31, f0, f1
    bl fn_800EF4FC
    clrlwi r3, r3, 16
    lis r4, 0x4330
    xoris r0, r24, 0x8000
    stw r3, 0x1c(r1)
    lfd f2, lbl_8047BEA0(r2)
    mr r3, r30
    stw r4, 0x18(r1)
    lfd f1, lbl_8047BEA8(r2)
    lfd f0, 0x18(r1)
    stw r0, 0x14(r1)
    fsubs f2, f0, f2
    stw r4, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f28, f0, f2
    bl fn_800EF4F4
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r30
    lfd f2, lbl_8047BEA0(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047BEC0(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f29, f0, f1
    bl fn_800EF4F4
    lis r4, 0x4330
    clrlwi r3, r3, 16
    stw r3, 0x2c(r1)
    xoris r0, r31, 0x8000
    lis r3, lbl_80314F98@ha
    lfd f2, lbl_8047BEA0(r2)
    stw r4, 0x28(r1)
    addi r3, r3, lbl_80314F98@l
    lfd f1, lbl_8047BEA8(r2)
    lfd f0, 0x28(r1)
    stw r0, 0x34(r1)
    fsubs f2, f0, f2
    stw r4, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    fdivs f30, f0, f2
    bl fn_800D7820
    mr r4, r30
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x2
    bl fn_800D67BC
    mr r3, r28
    mr r4, r27
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x3c
    li r5, 0xc
    li r6, 0xff
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    add r3, r28, r26
    add r0, r27, r25
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x3c
    li r5, 0xc
    li r6, 0xff
    bl fn_800D5CB8
    fmr f1, f28
    li r3, 0x0
    fmr f2, f30
    bl fn_800D59B8
    bl fn_800D6728
    lis r3, 0x8000
    bl fn_800D888C
    b @L_800565D8
    @L_800565A0:
    li r3, 0x232
    bl fn_8005D858
    lha r24, 0xc(r3)
    li r3, 0x232
    bl fn_8005D858
    lha r6, 0xe(r3)
    mr r5, r24
    mr r8, r25
    li r3, 0x0
    li r4, 0x0
    li r7, -0x1
    li r9, 0x232
    li r10, 0x0
    bl fn_80104160
    @L_800565D8:
    li r3, 0x0
    @L_800565DC:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    psq_l f28, 0x68(r1), 0, 0
    lfd f28, 0x60(r1)
    lmw r24, 0x40(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
