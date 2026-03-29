    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    addi r11, r1, 0xb0
    bl _savefpr_26
    stmw r21, 0x54(r1)
    clrlwi r0, r8, 24
    lis r8, lbl_803A1D60@ha
    cmplwi r0, 0x0
    addi r27, r8, lbl_803A1D60@l
    beq @L_8001EC48
    clrlwi r7, r7, 24
    srwi r0, r7, 31
    add r0, r0, r7
    srawi r0, r0, 1
    clrlwi r7, r0, 24
    @L_8001EC48:
    srwi r8, r5, 31
    srwi r0, r6, 31
    add r8, r8, r5
    add r9, r6, r4
    srawi r8, r8, 1
    add r0, r0, r6
    srawi r0, r0, 1
    add r6, r5, r3
    addi r9, r9, 0x2
    add r5, r8, r3
    add r0, r0, r4
    extsh r10, r6
    addi r28, r27, 0x0
    extsh r9, r9
    extsh r8, r4
    extsh r6, r3
    extsh r5, r5
    extsh r0, r0
    clrlwi r24, r7, 24
    addi r23, r27, 0x128
    addi r22, r27, 0x28
    sth r10, 0x0(r27)
    mr r26, r23
    li r21, 0x0
    sth r9, 0x2(r28)
    mr r25, r22
    sth r10, 0x8(r28)
    sth r8, 0xa(r28)
    sth r6, 0x10(r28)
    sth r8, 0x12(r28)
    sth r6, 0x18(r28)
    sth r9, 0x1a(r28)
    sth r5, 0x20(r28)
    sth r0, 0x22(r28)
    stw r24, 0x24(r28)
    lfd f28, lbl_8047B7F0(r2)
    xoris r30, r3, 0x8000
    lfd f29, lbl_8047B7D0(r2)
    xoris r31, r4, 0x8000
    lfd f30, lbl_8047B7F8(r2)
    lis r29, 0x4330
    lfd f31, lbl_8047B7E8(r2)
    lfd f27, lbl_8047B800(r2)
    @L_8001ECF4:
    xoris r0, r21, 0x8000
    stw r29, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsub f0, f0, f29
    fmul f0, f28, f0
    fmul f26, f0, f30
    fmr f1, f26
    bl fn_800CE148
    stw r30, 0x14(r1)
    stw r29, 0x10(r1)
    lfd f0, 0x10(r1)
    fsub f0, f0, f29
    fmadd f0, f31, f1, f0
    fmr f1, f26
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    sth r0, 0x0(r26)
    bl fn_800CDBE0
    stw r31, 0x24(r1)
    li r0, 0x0
    stw r29, 0x20(r1)
    lfd f0, 0x20(r1)
    fsub f0, f0, f29
    fmadd f0, f31, f1, f0
    fmr f1, f26
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r3, 0x2c(r1)
    sth r3, 0x2(r26)
    stw r0, 0x4(r26)
    bl fn_800CE148
    stw r30, 0x34(r1)
    stw r29, 0x30(r1)
    lfd f0, 0x30(r1)
    fsub f0, f0, f29
    fmadd f0, f27, f1, f0
    fmr f1, f26
    fctiwz f0, f0
    stfd f0, 0x38(r1)
    lwz r0, 0x3c(r1)
    sth r0, 0x0(r25)
    bl fn_800CDBE0
    stw r31, 0x44(r1)
    srawi r0, r21, 3
    addze r0, r0
    addi r3, r27, 0x0
    stw r29, 0x40(r1)
    slwi r0, r0, 3
    add r3, r3, r0
    lha r0, 0x0(r26)
    lfd f0, 0x40(r1)
    addi r5, r3, 0x2
    lha r4, 0x0(r3)
    addi r21, r21, 0x1
    fsub f0, f0, f29
    lha r3, 0x0(r5)
    add r0, r0, r4
    sth r0, 0x0(r26)
    fmadd f0, f27, f1, f0
    lha r0, 0x2(r26)
    add r0, r0, r3
    fctiwz f0, f0
    sth r0, 0x2(r26)
    addi r26, r26, 0x8
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    sth r0, 0x2(r25)
    stw r24, 0x4(r25)
    lha r0, 0x0(r25)
    add r0, r0, r4
    sth r0, 0x0(r25)
    lha r0, 0x2(r25)
    add r0, r0, r3
    sth r0, 0x2(r25)
    addi r25, r25, 0x8
    cmpwi r21, 0x20
    blt @L_8001ECF4
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x4
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x42
    bl fn_800D67BC
    mr r21, r22
    li r24, 0x0
    @L_8001EE64:
    lha r3, 0x0(r23)
    lha r4, 0x2(r23)
    bl fn_800D61E4
    lwz r4, 0x4(r23)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x0(r21)
    lha r4, 0x2(r21)
    bl fn_800D61E4
    lwz r4, 0x4(r21)
    li r3, 0x0
    bl fn_800D5BA0
    addi r23, r23, 0x8
    addi r21, r21, 0x8
    addi r24, r24, 0x1
    cmpwi r24, 0x20
    blt @L_8001EE64
    addi r4, r27, 0x128
    lha r3, 0x128(r27)
    lha r4, 0x2(r4)
    bl fn_800D61E4
    addi r4, r27, 0x128
    li r3, 0x0
    lwz r4, 0x4(r4)
    bl fn_800D5BA0
    addi r21, r27, 0x28
    lha r3, 0x28(r27)
    lha r4, 0x2(r21)
    bl fn_800D61E4
    mr r23, r21
    li r3, 0x0
    lwz r4, 0x4(r23)
    bl fn_800D5BA0
    bl fn_800D6728
    li r3, 0x5
    bl fn_800D6A00
    li r3, 0x22
    bl fn_800D67BC
    lha r3, 0x20(r28)
    lha r4, 0x22(r28)
    bl fn_800D61E4
    lwz r4, 0x24(r28)
    li r3, 0x0
    bl fn_800D5BA0
    li r24, 0x0
    @L_8001EF18:
    lha r3, 0x0(r22)
    lha r4, 0x2(r22)
    bl fn_800D61E4
    lwz r4, 0x4(r22)
    li r3, 0x0
    bl fn_800D5BA0
    addi r22, r22, 0x8
    addi r24, r24, 0x1
    cmpwi r24, 0x20
    blt @L_8001EF18
    lha r3, 0x28(r27)
    lha r4, 0x2(r21)
    bl fn_800D61E4
    lwz r4, 0x4(r23)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    addi r11, r1, 0xb0
    bl _restfpr_26
    lmw r21, 0x54(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
