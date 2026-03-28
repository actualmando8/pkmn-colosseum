    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r28, r3
    mr r29, r4
    mr r25, r5
    mr r26, r6
    lis r3, lbl_803A9A60@ha
    slwi r4, r25, 3
    addi r0, r3, lbl_803A9A60@l
    lbz r31, 0x8b(r28)
    add r3, r0, r4
    li r27, 0x1
    addi r30, r3, 0xc
    bl fn_8025DA88
    cmpwi r26, 0x2
    bne @L_800614AC
    cmpwi r3, 0x2
    beq @L_800614B8
    li r27, 0x0
    b @L_800614B8
@L_800614AC:
    cmpwi r3, 0x2
    bne @L_800614B8
    li r27, 0x0
@L_800614B8:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800615E0
    mulli r4, r25, 0xc
    lis r3, lbl_803A9A60@ha
    lha r6, 0x6(r29)
    lis r5, lbl_802EF0A8@ha
    addi r0, r3, lbl_803A9A60@l
    add r3, r0, r4
    lfs f0, 0x32c(r3)
    mulli r3, r6, 0x1c
    addi r0, r5, lbl_802EF0A8@l
    fctiwz f0, f0
    add r3, r0, r3
    lha r3, 0x2(r3)
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r29)
    lha r5, 0x84(r28)
    lha r3, 0x50(r29)
    lha r4, 0x86(r28)
    lha r0, 0x52(r29)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lwz r27, 0x0(r30)
    cmplwi r27, 0x0
    beq @L_800615E0
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r27
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BF60(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r3, 0x54(r29)
    lha r4, 0x56(r29)
    bl fn_800D61E4
    mr r7, r31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BF90(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
@L_800615E0:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
