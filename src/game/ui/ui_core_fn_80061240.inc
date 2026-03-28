    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stmw r25, 0x34(r1)
    mr r27, r4
    mulli r5, r5, 0xb4
    lis r4, lbl_803A9A60@ha
    slwi r6, r6, 2
    lfs f0, lbl_8047BFA8(r2)
    addi r0, r4, lbl_803A9A60@l
    lbz r26, 0x8b(r3)
    add r3, r0, r5
    addi r0, r3, 0x58
    add r3, r0, r6
    lfs f2, 0x6c(r3)
    lfs f1, 0x9c(r3)
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047891C(r13)
    cror eq, lt, eq
    bne @L_800612AC
    li r28, 0xa7
    li r29, 0x23
    li r25, 0x13
    b @L_800612D8
@L_800612AC:
    lfs f0, lbl_8047BF68(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_800612CC
    li r28, 0xc1
    li r29, 0xbd
    li r25, 0x16
    b @L_800612D8
@L_800612CC:
    li r28, 0x5
    li r29, 0xb3
    li r25, 0x11
@L_800612D8:
    lfs f0, lbl_8047BF60(r2)
    fcmpu cr0, f0, f1
    beq @L_80061438
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    lha r3, lbl_80478918(r13)
    lha r4, lbl_8047891A(r13)
    bl fn_800D61E4
    clrlwi r30, r28, 24
    clrlwi r31, r29, 24
    clrlwi r29, r25, 24
    clrlwi r28, r26, 24
    subi r4, r30, 0x2
    subi r0, r31, 0x2
    subi r5, r29, 0x2
    li r3, 0x0
    clrlwi r4, r4, 24
    clrlwi r0, r0, 24
    clrlwi r5, r5, 24
    slwi r4, r4, 24
    slwi r0, r0, 16
    slwi r5, r5, 8
    or r0, r4, r0
    or r0, r5, r0
    or r26, r28, r0
    mr r4, r26
    bl fn_800D5BA0
    lha r5, lbl_80478918(r13)
    lis r3, 0x4330
    lha r4, 0x54(r27)
    xoris r0, r5, 0x8000
    stw r3, 0x8(r1)
    subf r4, r5, r4
    lfd f2, lbl_8047BF80(r2)
    xoris r4, r4, 0x8000
    stw r0, 0x14(r1)
    lfs f3, lbl_8047891C(r13)
    stw r4, 0xc(r1)
    lha r4, lbl_8047891A(r13)
    stw r3, 0x10(r1)
    lfd f1, 0x8(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fmadds f31, f3, f1, f0
    fctiwz f0, f31
    stfd f0, 0x18(r1)
    lwz r3, 0x1c(r1)
    bl fn_800D61E4
    mr r4, r26
    li r3, 0x0
    bl fn_800D5BA0
    lha r4, lbl_8047891A(r13)
    lha r0, 0x56(r27)
    lha r3, lbl_80478918(r13)
    subf r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    slwi r4, r30, 24
    slwi r0, r31, 16
    slwi r5, r29, 8
    li r3, 0x0
    or r0, r4, r0
    or r0, r5, r0
    or r26, r28, r0
    mr r4, r26
    bl fn_800D5BA0
    fctiwz f0, f31
    lha r3, lbl_8047891A(r13)
    lha r0, 0x56(r27)
    stfd f0, 0x20(r1)
    subf r0, r3, r0
    extsh r4, r0
    lwz r3, 0x24(r1)
    bl fn_800D61E4
    mr r4, r26
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
@L_80061438:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    lmw r25, 0x34(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
