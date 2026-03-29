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
    lis r3, lbl_802E61E8@ha
    li r31, 0x1
    addi r4, r3, lbl_802E61E8@l
    li r5, 0x0
    li r0, 0x11
    mtctr r0
    @L_80053F28:
    lha r3, 0x6(r24)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80053F44
    addi r4, r4, 0x8
    addi r5, r5, 0x1
    bdnz @L_80053F28
    @L_80053F44:
    cmpwi r5, 0x11
    blt @L_80053F54
    li r3, 0x0
    b @L_800543EC
    @L_80053F54:
    lfs f1, lbl_8047BE80(r2)
    lis r3, lbl_802E61E8@ha
    lfs f0, lbl_8047A558(r13)
    addi r3, r3, lbl_802E61E8@l
    slwi r0, r5, 3
    fmuls f0, f1, f0
    add r3, r3, r0
    lha r0, 0x4(r3)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x52(r24)
    lha r5, 0x84(r25)
    lha r3, 0x50(r24)
    lha r4, 0x86(r25)
    lha r0, 0x52(r24)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lis r3, lbl_80267320@ha
    lha r4, 0x6(r24)
    addi r3, r3, lbl_80267320@l
    li r30, 0x0
    li r26, 0x0
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    addi r3, r3, 0x4
    li r26, 0x1
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    addi r3, r3, 0x4
    li r26, 0x2
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    addi r3, r3, 0x4
    li r26, 0x3
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    addi r3, r3, 0x4
    li r26, 0x4
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    addi r3, r3, 0x4
    li r26, 0x5
    lwz r0, 0x0(r3)
    cmpw r4, r0
    beq @L_8005403C
    li r26, 0x6
    @L_8005403C:
    cmpwi r26, 0x6
    blt @L_80054048
    li r26, -0x1
    @L_80054048:
    cmpwi r26, 0x0
    blt @L_80054090
    clrlwi r5, r26, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_80054090
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8005408C
    mr r3, r29
    bl fn_80057F94
    mr r30, r3
    b @L_80054090
    @L_8005408C:
    li r31, 0x0
    @L_80054090:
    cmplwi r30, 0x0
    beq @L_800543A8
    mr r3, r29
    bl fn_80057DE8
    cmpwi r3, 0x0
    beq @L_800540EC
    lfs f1, lbl_8047A550(r13)
    lfs f0, lbl_8047BE6C(r2)
    fcmpo cr0, f1, f0
    bge @L_800540C4
    lfs f0, lbl_8047BE64(r2)
    fmuls f1, f0, f1
    b @L_800540D4
    @L_800540C4:
    fsubs f1, f1, f0
    lfs f2, lbl_8047BE64(r2)
    lfs f0, lbl_8047BE60(r2)
    fnmsubs f1, f2, f1, f0
    @L_800540D4:
    lfs f0, lbl_8047BE84(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r29, 0xc(r1)
    b @L_800540F0
    @L_800540EC:
    li r29, 0x0
    @L_800540F0:
    lfs f1, lbl_8047A54C(r13)
    lfs f0, lbl_8047BE60(r2)
    fcmpo cr0, f1, f0
    bge @L_8005419C
    lwz r0, lbl_8047A548(r13)
    cmpw r0, r26
    bne @L_8005419C
    lha r5, 0x54(r24)
    lis r3, 0x4330
    lha r6, 0x56(r24)
    fsubs f3, f0, f1
    xoris r4, r5, 0x8000
    stw r3, 0x8(r1)
    xoris r0, r6, 0x8000
    lfd f2, lbl_8047BE58(r2)
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
    b @L_800541AC
    @L_8005419C:
    lha r26, 0x54(r24)
    li r28, 0x0
    lha r25, 0x56(r24)
    li r27, 0x0
    @L_800541AC:
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
    lfd f2, lbl_8047BE78(r2)
    stw r0, 0x20(r1)
    lfs f0, lbl_8047BE68(r2)
    lfd f1, 0x20(r1)
    fsubs f1, f1, f2
    fdivs f31, f0, f1
    bl fn_800EF4FC
    clrlwi r3, r3, 16
    lis r4, 0x4330
    xoris r0, r24, 0x8000
    stw r3, 0x1c(r1)
    lfd f2, lbl_8047BE78(r2)
    mr r3, r30
    stw r4, 0x18(r1)
    lfd f1, lbl_8047BE58(r2)
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
    lfd f2, lbl_8047BE78(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047BE68(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f29, f0, f1
    bl fn_800EF4F4
    lis r4, 0x4330
    clrlwi r3, r3, 16
    stw r3, 0x2c(r1)
    xoris r0, r31, 0x8000
    lis r3, lbl_80314F98@ha
    lfd f2, lbl_8047BE78(r2)
    stw r4, 0x28(r1)
    addi r3, r3, lbl_80314F98@l
    lfd f1, lbl_8047BE58(r2)
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
    b @L_800543E8
    @L_800543A8:
    cmpwi r31, 0x0
    beq @L_800543E8
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
    @L_800543E8:
    li r3, 0x0
    @L_800543EC:
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
