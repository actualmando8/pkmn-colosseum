    nofralloc
    stwu r1, -0x170(r1)
    mflr r0
    stw r0, 0x174(r1)
    stmw r21, 0x144(r1)
    mr r21, r3
    mr r31, r4
    li r0, 0x0
    li r4, 0x1
    stb r0, 0x8(r1)
    bl fn_800141BC
    mr r24, r3
    cmpwi r24, 0x0
    blt @L_800236B4
    addi r4, r1, 0x14
    addi r5, r1, 0x10
    bl fn_80014118
    lwz r3, 0x14(r1)
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80023410
    lwz r23, 0x14(r1)
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    mr r3, r23
    bl fn_8012640C
    extsh r30, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    extsh r29, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    extsh r28, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    extsh r27, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    extsh r26, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    lwz r4, 0x14(r1)
    extsh r25, r3
    lwz r5, 0x10(r1)
    addi r3, r1, 0x38
    clrlwi r6, r21, 16
    li r7, 0x0
    bl fn_80144574
    mr r23, r3
    extsh r0, r23
    cmpwi r0, 0x0
    ble @L_80023408
    bl fn_8001D378
    b @L_80023414
    @L_80023408:
    li r23, 0x0
    b @L_80023414
    @L_80023410:
    li r23, -0x1
    @L_80023414:
    extsh r0, r23
    cmpwi r0, 0x0
    beq @L_80023430
    bge @L_80023470
    cmpwi r0, -0x1
    bge @L_80023450
    b @L_80023470
    @L_80023430:
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_800236B4
    @L_80023450:
    li r3, 0x2
    li r4, 0x424c
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_800236B4
    @L_80023470:
    li r3, 0x4ca
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    lwz r3, 0x14(r1)
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    lwz r3, 0x14(r1)
    bl fn_8011F4A8
    mr r0, r3
    li r3, 0x2f
    clrlwi r4, r0, 24
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x423e
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    lwz r22, 0x14(r1)
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    mr r3, r22
    bl fn_8012640C
    extsh r0, r3
    mr r3, r22
    sth r0, 0x1a(r1)
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    extsh r0, r3
    mr r3, r22
    sth r0, 0x1c(r1)
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    extsh r0, r3
    mr r3, r22
    sth r0, 0x1e(r1)
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    extsh r0, r3
    mr r3, r22
    sth r0, 0x22(r1)
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    extsh r0, r3
    mr r3, r22
    sth r0, 0x24(r1)
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    lha r4, 0x1a(r1)
    extsh r11, r3
    lha r3, 0x1c(r1)
    subf r0, r25, r11
    lha r5, 0x1e(r1)
    subf r7, r30, r4
    lha r4, 0x22(r1)
    subf r6, r29, r3
    lha r3, 0x24(r1)
    subf r5, r28, r5
    subf r4, r27, r4
    extsh r10, r7
    subf r3, r26, r3
    extsh r9, r6
    extsh r8, r5
    extsh r7, r4
    extsh r6, r3
    extsh r5, r0
    li r0, 0x1
    sth r11, 0x20(r1)
    addi r3, r1, 0x28
    li r4, 0x1
    sth r10, 0x2a(r1)
    sth r9, 0x2c(r1)
    sth r8, 0x2e(r1)
    sth r7, 0x32(r1)
    sth r6, 0x34(r1)
    sth r5, 0x30(r1)
    stb r0, 0x28(r1)
    bl fn_80105D48
    li r0, 0x0
    addi r3, r1, 0x18
    stb r0, 0x18(r1)
    li r4, 0x1
    bl fn_80105D48
    li r3, 0x1
    bl fn_80105C68
    lwz r3, 0x14(r1)
    bl fn_8011F4A8
    li r0, 0x0
    mr r22, r3
    stb r0, 0x8(r1)
    b @L_80023678
    @L_8002361C:
    lwz r3, 0x14(r1)
    mr r4, r25
    bl fn_80123B5C
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8002366C
    lis r4, fn_80023274@ha
    lwz r3, 0x14(r1)
    addi r7, r4, fn_80023274@l
    addi r5, r1, 0x9
    mr r4, r25
    li r6, 0x1
    li r8, 0x0
    bl fn_802600E4
    cmpwi r3, 0x0
    beq @L_8002366C
    lwz r3, 0x14(r1)
    mr r5, r25
    lbz r4, 0x9(r1)
    bl fn_80123D58
    @L_8002366C:
    lbz r3, 0x8(r1)
    addi r0, r3, 0x1
    stb r0, 0x8(r1)
    @L_80023678:
    lwz r3, 0x14(r1)
    mr r4, r22
    addi r5, r1, 0x8
    bl fn_801236F8
    mr r25, r3
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    bne @L_8002361C
    lwz r3, 0x14(r1)
    addi r6, r1, 0xa
    addi r7, r1, 0xc
    li r4, 0x0
    li r5, 0x0
    bl fn_80128A64
    mr r22, r3
    @L_800236B4:
    mr r3, r24
    bl fn_80014198
    cmpwi r24, 0x0
    blt @L_80023748
    extsh r0, r23
    cmpwi r0, 0x0
    ble @L_80023748
    clrlwi r0, r22, 16
    cmplwi r0, 0x0
    beq @L_80023738
    lfs f1, lbl_8047B8A4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x0
    bl fn_801096F8
    lwz r3, 0x14(r1)
    mr r4, r22
    lhz r5, 0xa(r1)
    addi r6, r1, 0xc
    li r7, 0x0
    li r8, 0x1
    li r9, 0x1
    li r10, 0x0
    bl fn_8012805C
    li r3, 0x1
    bl fn_801096F8
    lfs f1, lbl_8047B8A4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    @L_80023738:
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x0(r31)
    b @L_8002374C
    @L_80023748:
    li r3, 0x1
    @L_8002374C:
    lmw r21, 0x144(r1)
    lwz r0, 0x174(r1)
    mtlr r0
    addi r1, r1, 0x170
    blr
