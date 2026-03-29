    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r23, 0x5c(r1)
    mr r28, r3
    mr r25, r4
    mr r27, r5
    mr r26, r6
    lis r6, 0x4330
    clrlwi r0, r7, 24
    stw r0, 0x1c(r1)
    lis r3, lbl_80266C20@ha
    addi r7, r3, lbl_80266C20@l
    lfd f3, lbl_8047B7D8(r2)
    stw r6, 0x18(r1)
    li r3, 0x1
    lwz r5, 0x0(r7)
    lfd f1, 0x18(r1)
    lfs f0, lbl_8047B7E4(r2)
    fsubs f1, f1, f3
    lwz r4, 0x4(r7)
    lwz r0, 0x8(r7)
    stw r5, 0x8(r1)
    fdivs f4, f1, f0
    stw r4, 0xc(r1)
    lbz r5, 0xb(r1)
    stw r0, 0x10(r1)
    lbz r4, 0xf(r1)
    lbz r0, 0x13(r1)
    stw r5, 0x24(r1)
    stw r6, 0x20(r1)
    lfd f0, 0x20(r1)
    stw r4, 0x34(r1)
    fsubs f1, f0, f3
    stw r6, 0x30(r1)
    lfd f0, 0x30(r1)
    fmuls f2, f1, f4
    stw r0, 0x44(r1)
    fsubs f1, f0, f3
    stw r6, 0x40(r1)
    fctiwz f2, f2
    lfd f0, 0x40(r1)
    fmuls f1, f1, f4
    stfd f2, 0x28(r1)
    fsubs f0, f0, f3
    fctiwz f1, f1
    lwz r0, 0x2c(r1)
    fmuls f0, f0, f4
    stb r0, 0xb(r1)
    stfd f1, 0x38(r1)
    fctiwz f0, f0
    lwz r4, 0x3c(r1)
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    stb r4, 0xf(r1)
    stb r0, 0x13(r1)
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x6
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    subi r3, r28, 0xa
    subi r0, r25, 0xa
    extsh r31, r3
    li r3, 0x4
    extsh r23, r0
    bl fn_800D67BC
    mr r3, r31
    mr r4, r23
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    addi r0, r27, 0x14
    mr r4, r23
    extsh r0, r0
    add r30, r31, r0
    extsh r3, r30
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    addi r0, r26, 0x14
    extsh r3, r30
    extsh r24, r0
    add r29, r23, r24
    extsh r4, r29
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    mr r3, r31
    extsh r4, r29
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    li r3, 0x1
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    lfs f1, lbl_8047B7E0(r2)
    bl fn_800D5648
    mr r29, r23
    li r23, 0x0
    b @L_8001E844
    @L_8001E800:
    li r3, 0x2
    bl fn_800D67BC
    mr r3, r31
    extsh r4, r29
    bl fn_800D61E4
    lwz r4, 0x10(r1)
    li r3, 0x0
    bl fn_800D5BA0
    extsh r3, r30
    extsh r4, r29
    bl fn_800D61E4
    lwz r4, 0x10(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    addi r29, r29, 0x4
    addi r23, r23, 0x4
    @L_8001E844:
    cmpw r23, r24
    blt @L_8001E800
    li r3, 0xbb
    bl fn_8005D858
    lha r6, 0xc(r3)
    add r5, r28, r27
    lha r4, 0xe(r3)
    li r3, 0xb8
    srwi r0, r6, 31
    add r6, r0, r6
    srwi r0, r4, 31
    srawi r8, r6, 1
    add r0, r0, r4
    srawi r9, r0, 1
    subf r7, r8, r28
    subf r4, r9, r25
    add r6, r28, r8
    subi r0, r9, 0xa
    subf r5, r8, r5
    subi r4, r4, 0xa
    extsh r31, r7
    add r0, r25, r0
    extsh r30, r6
    extsh r29, r5
    extsh r28, r4
    extsh r27, r0
    bl fn_8005D858
    lha r7, 0xe(r3)
    add r0, r25, r26
    mr r3, r31
    mr r4, r28
    srwi r6, r7, 31
    li r5, 0x0
    add r7, r6, r7
    li r6, 0xbb
    srawi r8, r7, 1
    li r7, 0x0
    subf r8, r8, r0
    addi r0, r8, 0xa
    extsh r26, r0
    bl fn_801040F0
    mr r3, r29
    mr r4, r28
    li r5, 0x0
    li r6, 0xbb
    li r7, 0x1
    bl fn_801040F0
    mr r3, r31
    mr r4, r26
    li r5, 0x0
    li r6, 0xb8
    li r7, 0x0
    bl fn_801040F0
    mr r3, r29
    mr r4, r26
    li r5, 0x0
    li r6, 0xb8
    li r7, 0x1
    bl fn_801040F0
    li r3, 0xba
    bl fn_8005D858
    mr r24, r3
    li r3, 0x84
    bl fn_8005D934
    mr r25, r3
    li r3, 0x87
    bl fn_8005D934
    lha r7, 0x2(r25)
    subf r23, r27, r26
    lha r0, 0x2(r3)
    mr r4, r27
    lha r5, 0xc(r24)
    extsh r6, r23
    subf r0, r7, r0
    li r7, -0x1
    extsh r0, r0
    li r8, 0x0
    add r0, r31, r0
    li r9, 0xba
    extsh r3, r0
    li r10, 0x0
    bl fn_80104160
    li r3, 0x85
    bl fn_8005D934
    mr r25, r3
    li r3, 0x86
    bl fn_8005D934
    lha r8, 0x2(r25)
    mr r4, r27
    lha r0, 0x2(r3)
    extsh r6, r23
    lha r5, 0xc(r24)
    li r7, -0x1
    subf r0, r8, r0
    li r8, 0x0
    extsh r0, r0
    li r9, 0xba
    add r0, r29, r0
    li r10, 0x0
    extsh r3, r0
    bl fn_80104160
    li r3, 0xb7
    bl fn_8005D858
    mr r27, r3
    li r3, 0x84
    bl fn_8005D934
    mr r25, r3
    li r3, 0x8b
    bl fn_8005D934
    mr r5, r3
    subf r23, r30, r29
    lha r4, 0x4(r25)
    mr r3, r30
    lha r0, 0x4(r5)
    extsh r5, r23
    lha r6, 0xe(r27)
    li r7, -0x1
    subf r0, r4, r0
    li r8, 0x0
    extsh r0, r0
    li r9, 0xb7
    add r0, r28, r0
    li r10, 0x0
    extsh r4, r0
    bl fn_80104160
    li r3, 0x88
    bl fn_8005D934
    mr r25, r3
    li r3, 0x8a
    bl fn_8005D934
    lha r0, 0x4(r3)
    mr r3, r30
    lha r4, 0x4(r25)
    extsh r5, r23
    lha r6, 0xe(r27)
    li r7, -0x1
    subf r0, r4, r0
    li r8, 0x0
    extsh r0, r0
    li r9, 0xb7
    add r0, r26, r0
    li r10, 0x0
    extsh r4, r0
    bl fn_80104160
    lmw r23, 0x5c(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
