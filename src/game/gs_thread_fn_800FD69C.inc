    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stfd f30, 0xc0(r1)
    psq_st f30, 0xc8(r1), 0, 0
    stfd f29, 0xb0(r1)
    psq_st f29, 0xb8(r1), 0, 0
    stfd f28, 0xa0(r1)
    psq_st f28, 0xa8(r1), 0, 0
    stmw r23, 0x7c(r1)
    mr r31, r3
    lwz r10, lbl_80478B08(r13)
    extsh r8, r5
    addi r0, r8, 0x2
    lha r3, 0x18(r10)
    add r0, r3, r0
    cmpwi r0, 0x200
    blt @L_800FD744
    lha r9, 0x1a(r10)
    li r0, 0x2
    lbz r3, 0x1c(r10)
    add r3, r3, r9
    addi r3, r3, 0x2
    extsh r3, r3
    sth r3, 0x1a(r10)
    lwz r3, lbl_80478B08(r13)
    sth r0, 0x18(r3)
    lbz r9, 0x23(r31)
    lwz r3, lbl_80478B08(r13)
    addi r0, r9, 0x2
    clrlwi r0, r0, 24
    stb r0, 0x1c(r3)
    lwz r9, lbl_80478B08(r13)
    lha r3, 0x1a(r9)
    lbz r0, 0x1c(r9)
    add r0, r3, r0
    cmpwi r0, 0x200
    blt @L_800FD744
    li r0, 0x1
    sth r0, 0x1a(r9)
@L_800FD744:
    lwz r3, lbl_80478B08(r13)
    extsh r9, r6
    lbz r0, 0x1c(r3)
    cmpw r0, r9
    bge @L_800FD760
    lbz r0, 0x23(r31)
    stb r0, 0x1c(r3)
@L_800FD760:
    addi r0, r8, 0x1
    lwz r11, lbl_80478B08(r13)
    clrrwi r10, r0, 1
    addi r3, r8, 0x2
    srwi r0, r10, 31
    lwz r12, 0x14(r11)
    add r10, r0, r10
    addi r0, r9, 0x1
    srawi r30, r10, 1
    li r11, -0x1
    b @L_800FD8D8
@L_800FD78C:
    lwz r25, lbl_80478B08(r13)
    li r10, -0x2
    lha r25, 0x1a(r25)
    add r23, r25, r11
    srawi r26, r23, 3
    clrlwi r25, r23, 29
    slwi r24, r26, 6
    slwi r29, r25, 3
    cmpwi r3, -0x2
    addi r26, r3, 0x3
    srwi r26, r26, 1
    li r28, 0x0
    ble @L_800FD8D4
    srwi r25, r26, 2
    cmplwi r25, 0x0
    mtctr r25
    beq @L_800FD89C
@L_800FD7D0:
    lwz r25, lbl_80478B08(r13)
    lha r25, 0x18(r25)
    add r23, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r23, 29
    srawi r27, r23, 3
    add r25, r25, r29
    add r27, r27, r24
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lha r25, 0x18(r25)
    add r23, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r23, 29
    srawi r27, r23, 3
    add r25, r25, r29
    add r27, r27, r24
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lha r25, 0x18(r25)
    add r23, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r23, 29
    srawi r27, r23, 3
    add r25, r25, r29
    add r27, r27, r24
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lha r25, 0x18(r25)
    add r23, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r23, 29
    srawi r27, r23, 3
    add r25, r25, r29
    add r27, r27, r24
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    bdnz @L_800FD7D0
    andi. r26, r26, 0x3
    beq @L_800FD8D4
@L_800FD89C:
    mtctr r26
@L_800FD8A0:
    lwz r25, lbl_80478B08(r13)
    lha r25, 0x18(r25)
    add r23, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r23, 29
    srawi r27, r23, 3
    add r25, r25, r29
    add r27, r27, r24
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    bdnz @L_800FD8A0
@L_800FD8D4:
    addi r11, r11, 0x1
@L_800FD8D8:
    cmpw r11, r0
    blt @L_800FD78C
    li r3, 0x0
    mr r0, r3
    b @L_800FDA58
@L_800FD8EC:
    lwz r25, lbl_80478B08(r13)
    add r11, r4, r0
    li r10, 0x0
    lha r25, 0x1a(r25)
    add r23, r25, r3
    srawi r26, r23, 3
    clrlwi r25, r23, 29
    slwi r23, r26, 6
    slwi r24, r25, 3
    cmpwi r8, 0x0
    addi r26, r8, 0x1
    srwi r26, r26, 1
    ble @L_800FDA50
    srwi r25, r26, 2
    cmplwi r25, 0x0
    mtctr r25
    beq @L_800FDA10
@L_800FD930:
    lwz r25, lbl_80478B08(r13)
    lbz r28, 0x0(r11)
    lha r25, 0x18(r25)
    add r27, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r27, 29
    srawi r27, r27, 3
    add r25, r25, r24
    add r27, r27, r23
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lbz r28, 0x1(r11)
    lha r25, 0x18(r25)
    add r27, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r27, 29
    srawi r27, r27, 3
    add r25, r25, r24
    add r27, r27, r23
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lbz r28, 0x2(r11)
    lha r25, 0x18(r25)
    add r27, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r27, 29
    srawi r27, r27, 3
    add r25, r25, r24
    add r27, r27, r23
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    lwz r25, lbl_80478B08(r13)
    lbz r28, 0x3(r11)
    addi r11, r11, 0x4
    lha r25, 0x18(r25)
    add r27, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r27, 29
    srawi r27, r27, 3
    add r25, r25, r24
    add r27, r27, r23
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    bdnz @L_800FD930
    andi. r26, r26, 0x3
    beq @L_800FDA50
@L_800FDA10:
    mtctr r26
@L_800FDA14:
    lwz r25, lbl_80478B08(r13)
    lbz r28, 0x0(r11)
    addi r11, r11, 0x1
    lha r25, 0x18(r25)
    add r27, r25, r10
    addi r10, r10, 0x2
    clrlwi r25, r27, 29
    srawi r27, r27, 3
    add r25, r25, r24
    add r27, r27, r23
    srawi r25, r25, 1
    slwi r27, r27, 5
    add r25, r25, r12
    stbx r28, r27, r25
    bdnz @L_800FDA14
@L_800FDA50:
    add r0, r0, r30
    addi r3, r3, 0x1
@L_800FDA58:
    cmpw r3, r9
    blt @L_800FD8EC
    extsh r3, r7
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lbz r7, 0x43(r31)
    stw r3, 0x1c(r1)
    extsh r4, r5
    extsb r3, r7
    lwz r10, lbl_80478B08(r13)
    stw r0, 0x18(r1)
    xoris r5, r3, 0x8000
    lfd f5, lbl_8047CD10(r2)
    extsh r3, r6
    lfd f0, 0x18(r1)
    xoris r6, r4, 0x8000
    stw r5, 0x14(r1)
    xoris r7, r3, 0x8000
    fsubs f1, f0, f5
    lfs f6, 0x64(r31)
    stw r0, 0x10(r1)
    lfs f0, 0x10(r31)
    lfd f2, 0x10(r1)
    fmadds f0, f1, f6, f0
    lha r4, 0x18(r10)
    fsubs f1, f2, f5
    lfs f2, 0xc(r31)
    lha r10, 0x1a(r10)
    add r3, r4, r8
    fadds f0, f1, f0
    xoris r5, r4, 0x8000
    fctiwz f1, f2
    xoris r4, r10, 0x8000
    xoris r3, r3, 0x8000
    stw r6, 0x2c(r1)
    fctiwz f0, f0
    stfd f1, 0x8(r1)
    lfs f3, 0x60(r31)
    lwz r28, 0xc(r1)
    stfd f0, 0x20(r1)
    extsh r6, r28
    lfs f1, lbl_8047CD4C(r2)
    lwz r26, 0x24(r1)
    xoris r8, r6, 0x8000
    stw r0, 0x28(r1)
    extsh r6, r26
    xoris r6, r6, 0x8000
    lfd f0, 0x28(r1)
    stw r8, 0x34(r1)
    fsubs f4, f0, f5
    stw r0, 0x30(r1)
    lfd f0, 0x30(r1)
    stw r7, 0x44(r1)
    fsubs f2, f0, f5
    stw r0, 0x40(r1)
    lfd f0, 0x40(r1)
    fmadds f3, f4, f3, f2
    stw r6, 0x4c(r1)
    fsubs f2, f0, f5
    stw r0, 0x48(r1)
    fctiwz f3, f3
    lfd f0, 0x48(r1)
    stfd f3, 0x38(r1)
    fsubs f0, f0, f5
    stw r5, 0x5c(r1)
    lwz r27, 0x3c(r1)
    stw r0, 0x58(r1)
    fmadds f2, f2, f6, f0
    lfd f0, 0x58(r1)
    fctiwz f3, f2
    stw r4, 0x64(r1)
    fsubs f2, f0, f5
    stw r0, 0x60(r1)
    lfd f0, 0x60(r1)
    fmuls f31, f2, f1
    stfd f3, 0x50(r1)
    fsubs f0, f0, f5
    stw r3, 0x6c(r1)
    lwz r25, 0x54(r1)
    stw r0, 0x68(r1)
    fmuls f29, f0, f1
    lfd f0, 0x68(r1)
    fsubs f0, f0, f5
    fmuls f30, f0, f1
    lwz r4, 0x24(r31)
    add r3, r10, r9
    xoris r5, r3, 0x8000
    lis r3, 0x8081
    clrlwi r4, r4, 24
    stw r5, 0x74(r1)
    mulli r4, r4, 0xc0
    subi r5, r3, 0x7f7f
    stw r0, 0x70(r1)
    li r3, 0x7
    mulhwu r0, r5, r4
    lfd f0, 0x70(r1)
    fsubs f0, f0, f5
    fmuls f28, f0, f1
    srwi r0, r0, 7
    clrlwi r29, r0, 24
    bl fn_800D6A00
    lbz r0, 0x2(r31)
    cmpwi r0, 0x1
    beq @L_800FDC1C
    bge @L_800FDC04
    cmpwi r0, 0x0
    b @L_800FDC10
@L_800FDC04:
    cmpwi r0, 0x3
    bge @L_800FDC10
    b @L_800FDCA0
@L_800FDC10:
    li r3, 0x2
    bl fn_800D67BC
    b @L_800FDE78
@L_800FDC1C:
    li r3, 0x4
    bl fn_800D67BC
    addi r3, r28, 0x1
    addi r0, r26, 0x1
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    addi r3, r27, 0x1
    addi r0, r25, 0x1
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
    b @L_800FDE78
@L_800FDCA0:
    li r3, 0xa
    bl fn_800D67BC
    extsh r24, r28
    mr r4, r26
    subi r0, r24, 0x1
    extsh r3, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    extsh r23, r27
    mr r4, r25
    subi r0, r23, 0x1
    extsh r3, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
    addi r0, r24, 0x1
    mr r4, r26
    extsh r3, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    addi r0, r23, 0x1
    mr r4, r25
    extsh r3, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
    extsh r23, r26
    mr r3, r28
    subi r0, r23, 0x1
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    extsh r24, r25
    mr r3, r27
    subi r0, r24, 0x1
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
    addi r0, r23, 0x1
    mr r3, r28
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    addi r0, r24, 0x1
    mr r3, r27
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
@L_800FDE78:
    mr r3, r28
    mr r4, r26
    bl fn_800D61E4
    lwz r4, 0x24(r31)
    li r3, 0x0
    bl fn_800D5BA0
    fmr f1, f31
    li r3, 0x0
    fmr f2, f29
    bl fn_800D59B8
    mr r3, r27
    mr r4, r25
    bl fn_800D61E4
    lwz r4, 0x24(r31)
    li r3, 0x0
    bl fn_800D5BA0
    fmr f1, f30
    li r3, 0x0
    fmr f2, f28
    bl fn_800D59B8
    bl fn_800D6728
    lwz r4, lbl_80478B08(r13)
    slwi r0, r30, 1
    lha r3, 0x18(r4)
    add r3, r0, r3
    addi r0, r3, 0x2
    extsh r0, r0
    sth r0, 0x18(r4)
    psq_l f31, 0xd8(r1), 0, 0
    lfd f31, 0xd0(r1)
    psq_l f30, 0xc8(r1), 0, 0
    lfd f30, 0xc0(r1)
    psq_l f29, 0xb8(r1), 0, 0
    lfd f29, 0xb0(r1)
    psq_l f28, 0xa8(r1), 0, 0
    lfd f28, 0xa0(r1)
    lmw r23, 0x7c(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
