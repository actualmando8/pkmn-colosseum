    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r21, 0x24(r1)
    lis r3, lbl_803A1D40@ha
    li r4, 0x0
    addi r24, r3, lbl_803A1D40@l
    li r3, -0x1
    lbz r0, 0x1(r24)
    addi r30, r24, 0x6
    stb r4, 0x0(r30)
    cmplwi r0, 0x0
    stb r4, 0x5(r24)
    stb r3, 0x7(r24)
    bne @L_8001B22C
    stb r3, 0x0(r30)
    @L_8001B22C:
    lis r3, lbl_803A1C20@ha
    li r4, 0x0
    addi r3, r3, lbl_803A1C20@l
    li r5, 0x120
    bl memset
    lis r3, lbl_803A1D40@ha
    li r27, 0x0
    addi r3, r3, lbl_803A1D40@l
    addi r31, r3, 0xc
    addi r26, r3, 0x8
    lis r3, lbl_803A1C20@ha
    addi r29, r3, lbl_803A1C20@l
    b @L_8001B4B0
    @L_8001B260:
    clrlwi r4, r27, 16
    lwz r5, 0x0(r26)
    mulli r0, r4, 0x30
    lwz r3, 0x0(r31)
    cmpwi r5, 0x1
    li r21, 0x0
    add r28, r29, r0
    beq @L_8001B340
    bge @L_8001B290
    cmpwi r5, 0x0
    bge @L_8001B29C
    b @L_8001B3A8
    @L_8001B290:
    cmpwi r5, 0x3
    bge @L_8001B3A8
    b @L_8001B39C
    @L_8001B29C:
    cmplwi r4, 0x6
    blt @L_8001B2A8
    b @L_8001B3C0
    @L_8001B2A8:
    cmpwi r5, 0x1
    beq @L_8001B2E8
    bge @L_8001B31C
    cmpwi r5, 0x0
    bge @L_8001B2C0
    b @L_8001B31C
    @L_8001B2C0:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001B2E0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001B320
    @L_8001B2E0:
    bl fn_8006AEEC
    b @L_8001B320
    @L_8001B2E8:
    cmplwi r3, 0x0
    bne @L_8001B2F8
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B2F8:
    cmplwi r3, 0x0
    bne @L_8001B308
    li r3, 0x0
    b @L_8001B320
    @L_8001B308:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001B320
    @L_8001B31C:
    li r3, 0x0
    @L_8001B320:
    cmplwi r3, 0x0
    bne @L_8001B330
    li r21, 0x0
    b @L_8001B3C0
    @L_8001B330:
    mr r4, r27
    bl fn_8012AC08
    mr r21, r3
    b @L_8001B3A8
    @L_8001B340:
    cmplwi r4, 0x6
    blt @L_8001B34C
    b @L_8001B3C0
    @L_8001B34C:
    cmplwi r3, 0x0
    bne @L_8001B35C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B35C:
    cmplwi r3, 0x0
    bne @L_8001B36C
    li r21, 0x0
    b @L_8001B3C0
    @L_8001B36C:
    mr r4, r27
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001B384
    li r21, 0x0
    b @L_8001B3C0
    @L_8001B384:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r21, r3
    b @L_8001B3A8
    @L_8001B39C:
    cmplwi r4, 0x1e
    blt @L_8001B3A8
    b @L_8001B3C0
    @L_8001B3A8:
    mr r3, r21
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001B3C0
    li r21, 0x0
    @L_8001B3C0:
    cmplwi r21, 0x0
    bne @L_8001B3D4
    li r0, 0x0
    sth r0, 0x0(r28)
    b @L_8001B4AC
    @L_8001B3D4:
    mr r3, r21
    mr r4, r28
    bl fn_8011FDC8
    mr r3, r21
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001B408
    li r0, 0x0
    sth r0, 0x1a(r28)
    @L_8001B408:
    mr r3, r21
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001B430
    li r0, 0x1
    b @L_8001B494
    @L_8001B430:
    mr r3, r21
    bl fn_80120FE0
    clrlwi r0, r3, 16
    cmpwi r0, 0x3c
    beq @L_8001B478
    bge @L_8001B458
    cmpwi r0, 0x3a
    beq @L_8001B468
    bge @L_8001B470
    b @L_8001B490
    @L_8001B458:
    cmpwi r0, 0x3e
    beq @L_8001B488
    bge @L_8001B490
    b @L_8001B480
    @L_8001B468:
    li r0, 0x2
    b @L_8001B494
    @L_8001B470:
    li r0, 0x3
    b @L_8001B494
    @L_8001B478:
    li r0, 0x4
    b @L_8001B494
    @L_8001B480:
    li r0, 0x5
    b @L_8001B494
    @L_8001B488:
    li r0, 0x6
    b @L_8001B494
    @L_8001B490:
    li r0, 0x0
    @L_8001B494:
    clrlwi r0, r0, 16
    lis r3, lbl_802E4EB8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EB8@l
    lhzx r0, r3, r0
    sth r0, 0x24(r28)
    @L_8001B4AC:
    addi r27, r27, 0x1
    @L_8001B4B0:
    clrlwi r0, r27, 16
    cmplwi r0, 0x6
    blt @L_8001B260
    li r0, 0x0
    lis r3, 0x8002
    sth r0, lbl_8047A30A(r13)
    addi r4, r3, -0x4144
    li r3, 0x0
    la r5, lbl_8047A30A(r13)
    bl fn_8010B01C
    lis r3, lbl_803A1D40@ha
    addi r29, r3, lbl_803A1D40@l
    lis r3, lbl_803A1C20@ha
    addi r23, r3, lbl_803A1C20@l
    @L_8001B4E8:
    lbz r0, 0x0(r30)
    li r3, 0x0
    stb r3, 0x14(r29)
    mr r9, r23
    extsb r0, r0
    addi r5, r1, 0x10
    stw r0, 0x10(r1)
    li r3, 0x63
    li r4, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    lbz r0, 0x1(r24)
    extsb r22, r3
    cmplwi r0, 0x0
    bne @L_8001B534
    li r22, -0x2
    @L_8001B534:
    extsb r0, r22
    stb r22, 0x0(r30)
    cmpwi r0, -0x2
    beq @L_8001BA88
    li r3, 0x63
    bl fn_801022B8
    cmpwi r3, 0x3b6
    beq @L_8001B560
    extsb r27, r22
    cmpwi r27, -0x1
    bne @L_8001B56C
    @L_8001B560:
    li r0, -0x1
    stb r0, 0x0(r30)
    b @L_8001BA88
    @L_8001B56C:
    lbz r0, 0x0(r29)
    cmpwi r0, 0x5
    beq @L_8001B87C
    bge @L_8001B594
    cmpwi r0, 0x2
    beq @L_8001B5B4
    bge @L_8001B874
    cmpwi r0, 0x1
    bge @L_8001B5A4
    b @L_8001BA74
    @L_8001B594:
    cmpwi r0, 0x7
    beq @L_8001B8A0
    bge @L_8001BA74
    b @L_8001B898
    @L_8001B5A4:
    mr r3, r22
    bl fn_8001C7B8
    mr r25, r3
    b @L_8001BA74
    @L_8001B5B4:
    li r0, 0x1
    stb r0, 0x14(r29)
    lis r4, fn_80019D5C@ha
    lis r3, lbl_803A1C20@ha
    addi r28, r4, fn_80019D5C@l
    addi r27, r3, lbl_803A1C20@l
    @L_8001B5CC:
    lbz r0, 0x15(r29)
    cmplwi r0, 0x0
    beq @L_8001B5F4
    li r3, 0x6d
    li r4, 0x1
    bl fn_8010264C
    mr r21, r3
    li r3, 0x6d
    bl fn_80102510
    b @L_8001B60C
    @L_8001B5F4:
    li r3, 0x108
    li r4, 0x1
    bl fn_8010264C
    mr r21, r3
    li r3, 0x108
    bl fn_80102510
    @L_8001B60C:
    cmpwi r21, 0x1
    beq @L_8001B660
    bge @L_8001B628
    cmpwi r21, -0x1
    beq @L_8001B868
    bge @L_8001B634
    b @L_8001B86C
    @L_8001B628:
    cmpwi r21, 0x3
    bge @L_8001B86C
    b @L_8001B868
    @L_8001B634:
    lwz r3, 0x18(r29)
    extsb r5, r22
    lwz r4, 0x0(r31)
    bl fn_80010C98
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001B658
    li r21, -0x1
    b @L_8001B86C
    @L_8001B658:
    li r21, 0x1
    b @L_8001B86C
    @L_8001B660:
    li r3, 0x63
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r4, 0x0(r26)
    li r21, 0x0
    lwz r3, 0x0(r31)
    cmpwi r4, 0x1
    beq @L_8001B74C
    bge @L_8001B694
    cmpwi r4, 0x0
    bge @L_8001B6A0
    b @L_8001B7C4
    @L_8001B694:
    cmpwi r4, 0x3
    bge @L_8001B7C4
    b @L_8001B7B0
    @L_8001B6A0:
    extsb r25, r22
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8001B6B4
    b @L_8001B7DC
    @L_8001B6B4:
    cmpwi r4, 0x1
    beq @L_8001B6F4
    bge @L_8001B728
    cmpwi r4, 0x0
    bge @L_8001B6CC
    b @L_8001B728
    @L_8001B6CC:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001B6EC
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001B72C
    @L_8001B6EC:
    bl fn_8006AEEC
    b @L_8001B72C
    @L_8001B6F4:
    cmplwi r3, 0x0
    bne @L_8001B704
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B704:
    cmplwi r3, 0x0
    bne @L_8001B714
    li r3, 0x0
    b @L_8001B72C
    @L_8001B714:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001B72C
    @L_8001B728:
    li r3, 0x0
    @L_8001B72C:
    cmplwi r3, 0x0
    bne @L_8001B73C
    li r21, 0x0
    b @L_8001B7DC
    @L_8001B73C:
    mr r4, r25
    bl fn_8012AC08
    mr r21, r3
    b @L_8001B7C4
    @L_8001B74C:
    extsb r22, r22
    clrlwi r0, r22, 16
    cmplwi r0, 0x6
    blt @L_8001B760
    b @L_8001B7DC
    @L_8001B760:
    cmplwi r3, 0x0
    bne @L_8001B770
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B770:
    cmplwi r3, 0x0
    bne @L_8001B780
    li r21, 0x0
    b @L_8001B7DC
    @L_8001B780:
    mr r4, r22
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001B798
    li r21, 0x0
    b @L_8001B7DC
    @L_8001B798:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r21, r3
    b @L_8001B7C4
    @L_8001B7B0:
    extsb r0, r22
    clrlwi r0, r0, 16
    cmplwi r0, 0x1e
    blt @L_8001B7C4
    b @L_8001B7DC
    @L_8001B7C4:
    mr r3, r21
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001B7DC
    li r21, 0x0
    @L_8001B7DC:
    lfs f1, lbl_8047B7C0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, 0x0(r31)
    mr r4, r21
    mr r5, r28
    li r6, 0x0
    bl fn_80097E58
    lbz r0, 0x0(r30)
    mr r9, r27
    addi r5, r1, 0xc
    li r3, 0x63
    extsb r0, r0
    li r4, 0x0
    stw r0, 0xc(r1)
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x63
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8001B84C
    li r0, 0x1
    stb r0, 0x98(r3)
    @L_8001B84C:
    lfs f1, lbl_8047B7C0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lbz r22, 0x0(r30)
    b @L_8001B5CC
    @L_8001B868:
    li r21, 0x1
    @L_8001B86C:
    mr r25, r21
    b @L_8001BA74
    @L_8001B874:
    li r25, -0x1
    b @L_8001BA74
    @L_8001B87C:
    lbz r4, 0x11(r29)
    mr r3, r22
    lhz r5, 0x12(r29)
    addi r6, r1, 0x8
    bl fn_8001C064
    mr r25, r3
    b @L_8001BA74
    @L_8001B898:
    li r25, -0x1
    b @L_8001BA74
    @L_8001B8A0:
    lwz r4, 0x0(r26)
    li r22, 0x0
    lwz r3, 0x0(r31)
    cmpwi r4, 0x1
    beq @L_8001B978
    bge @L_8001B8C4
    cmpwi r4, 0x0
    bge @L_8001B8D0
    b @L_8001B9E8
    @L_8001B8C4:
    cmpwi r4, 0x3
    bge @L_8001B9E8
    b @L_8001B9D8
    @L_8001B8D0:
    clrlwi r0, r27, 16
    cmplwi r0, 0x6
    blt @L_8001B8E0
    b @L_8001BA00
    @L_8001B8E0:
    cmpwi r4, 0x1
    beq @L_8001B920
    bge @L_8001B954
    cmpwi r4, 0x0
    bge @L_8001B8F8
    b @L_8001B954
    @L_8001B8F8:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001B918
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001B958
    @L_8001B918:
    bl fn_8006AEEC
    b @L_8001B958
    @L_8001B920:
    cmplwi r3, 0x0
    bne @L_8001B930
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B930:
    cmplwi r3, 0x0
    bne @L_8001B940
    li r3, 0x0
    b @L_8001B958
    @L_8001B940:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001B958
    @L_8001B954:
    li r3, 0x0
    @L_8001B958:
    cmplwi r3, 0x0
    bne @L_8001B968
    li r22, 0x0
    b @L_8001BA00
    @L_8001B968:
    mr r4, r27
    bl fn_8012AC08
    mr r22, r3
    b @L_8001B9E8
    @L_8001B978:
    clrlwi r0, r27, 16
    cmplwi r0, 0x6
    blt @L_8001B988
    b @L_8001BA00
    @L_8001B988:
    cmplwi r3, 0x0
    bne @L_8001B998
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001B998:
    cmplwi r3, 0x0
    bne @L_8001B9A8
    li r22, 0x0
    b @L_8001BA00
    @L_8001B9A8:
    mr r4, r27
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001B9C0
    li r22, 0x0
    b @L_8001BA00
    @L_8001B9C0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r22, r3
    b @L_8001B9E8
    @L_8001B9D8:
    clrlwi r0, r27, 16
    cmplwi r0, 0x1e
    blt @L_8001B9E8
    b @L_8001BA00
    @L_8001B9E8:
    mr r3, r22
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001BA00
    li r22, 0x0
    @L_8001BA00:
    cmplwi r22, 0x0
    beq @L_8001B4E8
    mr r3, r22
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001BA3C
    li r3, 0x2
    li r4, 0x44dd
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8001B4E8
    @L_8001BA3C:
    mr r3, r22
    bl fn_8011FC14
    lfs f0, lbl_8047B7C4(r2)
    fcmpu cr0, f0, f1
    beq @L_8001BA70
    li r3, 0x2
    li r4, 0x44de
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_8001B4E8
    @L_8001BA70:
    li r25, -0x1
    @L_8001BA74:
    cmpwi r25, 0x0
    blt @L_8001BA88
    b @L_8001B4E8
    b @L_8001BA88
    @L_8001BA84:
    bl fn_800F0308
    @L_8001BA88:
    bl fn_8010B560
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001BA84
    lbz r0, 0x0(r30)
    lis r3, lbl_803A1D40@ha
    addi r4, r3, lbl_803A1D40@l
    mr r3, r25
    extsb r0, r0
    stw r0, 0x1c(r4)
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
