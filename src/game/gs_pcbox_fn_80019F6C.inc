    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r3
    mr r31, r4
    li r4, 0x0
    bl fn_801040D0
    mr r28, r3
    li r29, 0x0
    cmplwi r28, 0x0
    li r3, 0x0
    beq @L_8001A970
    lhz r0, 0x0(r28)
    cmplwi r0, 0x0
    bne @L_80019FB4
    li r3, -0x1
    b @L_80019FC4
    @L_80019FB4:
    lha r0, 0x1a(r28)
    cmpwi r0, 0x0
    bne @L_80019FC4
    li r3, 0x1
    @L_80019FC4:
    extsb r0, r3
    li r30, 0x1
    cmpwi r0, -0x1
    li r27, -0x1
    bne @L_8001A03C
    lha r0, 0x6(r31)
    cmpwi r0, 0x3cc
    bge @L_8001A010
    cmpwi r0, 0x3c4
    beq @L_8001A034
    bge @L_8001A004
    cmpwi r0, 0x3c2
    bge @L_8001A094
    cmpwi r0, 0x3ba
    bge @L_8001A034
    b @L_8001A094
    @L_8001A004:
    cmpwi r0, 0x3c9
    bge @L_8001A034
    b @L_8001A094
    @L_8001A010:
    cmpwi r0, 0x12a0
    bge @L_8001A02C
    cmpwi r0, 0x548
    bge @L_8001A094
    cmpwi r0, 0x545
    bge @L_8001A034
    b @L_8001A094
    @L_8001A02C:
    cmpwi r0, 0x12a3
    bge @L_8001A094
    @L_8001A034:
    li r30, 0x0
    b @L_8001A094
    @L_8001A03C:
    cmpwi r0, 0x1
    bne @L_8001A094
    lha r0, 0x6(r31)
    cmpwi r0, 0x129d
    bge @L_8001A078
    cmpwi r0, 0x3c4
    bge @L_8001A06C
    cmpwi r0, 0x3ba
    bge @L_8001A094
    cmpwi r0, 0x3b8
    bge @L_8001A08C
    b @L_8001A094
    @L_8001A06C:
    cmpwi r0, 0x3c9
    bge @L_8001A094
    b @L_8001A08C
    @L_8001A078:
    cmpwi r0, 0x12a0
    beq @L_8001A08C
    bge @L_8001A094
    cmpwi r0, 0x129f
    bge @L_8001A094
    @L_8001A08C:
    lis r3, 0x8081
    subi r27, r3, 0x7f01
    @L_8001A094:
    mr r3, r31
    mr r4, r30
    bl fn_80109220
    clrlwi r0, r30, 24
    stw r27, 0x64(r31)
    cmplwi r0, 0x0
    beq @L_8001A970
    mr r3, r26
    mr r4, r31
    bl fn_8001D834
    mr r0, r3
    mr r3, r26
    mr r30, r0
    li r4, 0x1
    bl fn_801040D0
    lis r4, lbl_803A1D40@ha
    clrlwi r27, r3, 16
    addi r3, r4, lbl_803A1D40@l
    lbz r0, 0x0(r3)
    cmplwi r0, 0x3
    beq @L_8001A0F0
    cmplwi r0, 0x4
    bne @L_8001A318
    @L_8001A0F0:
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lhz r3, 0x12(r3)
    bl fn_801440A0
    mr r25, r3
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x4
    bne @L_8001A318
    lha r0, 0x6(r31)
    cmpwi r0, 0x3be
    beq @L_8001A308
    bge @L_8001A130
    cmpwi r0, 0x3bc
    beq @L_8001A148
    b @L_8001A318
    @L_8001A130:
    cmpwi r0, 0x12a2
    beq @L_8001A308
    bge @L_8001A318
    cmpwi r0, 0x12a1
    bge @L_8001A148
    b @L_8001A318
    @L_8001A148:
    mr r3, r25
    bl fn_80143F24
    lis r4, lbl_803A1D40@ha
    mr r29, r3
    addi r3, r4, lbl_803A1D40@l
    li r26, 0x0
    lwz r0, 0x8(r3)
    lwz r3, 0xc(r3)
    cmpwi r0, 0x1
    beq @L_8001A230
    bge @L_8001A180
    cmpwi r0, 0x0
    bge @L_8001A18C
    b @L_8001A298
    @L_8001A180:
    cmpwi r0, 0x3
    bge @L_8001A298
    b @L_8001A28C
    @L_8001A18C:
    cmplwi r27, 0x6
    blt @L_8001A198
    b @L_8001A2B0
    @L_8001A198:
    cmpwi r0, 0x1
    beq @L_8001A1D8
    bge @L_8001A20C
    cmpwi r0, 0x0
    bge @L_8001A1B0
    b @L_8001A20C
    @L_8001A1B0:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001A1D0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001A210
    @L_8001A1D0:
    bl fn_8006AEEC
    b @L_8001A210
    @L_8001A1D8:
    cmplwi r3, 0x0
    bne @L_8001A1E8
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001A1E8:
    cmplwi r3, 0x0
    bne @L_8001A1F8
    li r3, 0x0
    b @L_8001A210
    @L_8001A1F8:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001A210
    @L_8001A20C:
    li r3, 0x0
    @L_8001A210:
    cmplwi r3, 0x0
    bne @L_8001A220
    li r26, 0x0
    b @L_8001A2B0
    @L_8001A220:
    mr r4, r27
    bl fn_8012AC08
    mr r26, r3
    b @L_8001A298
    @L_8001A230:
    cmplwi r27, 0x6
    blt @L_8001A23C
    b @L_8001A2B0
    @L_8001A23C:
    cmplwi r3, 0x0
    bne @L_8001A24C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001A24C:
    cmplwi r3, 0x0
    bne @L_8001A25C
    li r26, 0x0
    b @L_8001A2B0
    @L_8001A25C:
    mr r4, r27
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001A274
    li r26, 0x0
    b @L_8001A2B0
    @L_8001A274:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    b @L_8001A298
    @L_8001A28C:
    cmplwi r27, 0x1e
    blt @L_8001A298
    b @L_8001A2B0
    @L_8001A298:
    mr r3, r26
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001A2B0
    li r26, 0x0
    @L_8001A2B0:
    mr r3, r26
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001A2CC
    li r6, 0x2b65
    b @L_8001A2F8
    @L_8001A2CC:
    mr r3, r26
    bl fn_8011F5C8
    bl fn_8011E778
    clrlwi r4, r29, 24
    bl fn_8011E2AC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001A2F4
    li r6, 0x2b65
    b @L_8001A2F8
    @L_8001A2F4:
    li r6, 0x2b64
    @L_8001A2F8:
    mr r5, r30
    li r3, 0x0
    li r4, -0x4
    bl fn_800FB680
    @L_8001A308:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A318:
    lha r0, 0x6(r31)
    cmpwi r0, 0x3cb
    beq @L_8001A4CC
    bge @L_8001A3A4
    cmpwi r0, 0x3c0
    beq @L_8001A608
    bge @L_8001A370
    cmpwi r0, 0x3bb
    beq @L_8001A4FC
    bge @L_8001A360
    cmpwi r0, 0x3b8
    bge @L_8001A354
    cmpwi r0, 0x3b6
    bge @L_8001A920
    b @L_8001A970
    @L_8001A354:
    cmpwi r0, 0x3ba
    bge @L_8001A4CC
    b @L_8001A440
    @L_8001A360:
    cmpwi r0, 0x3be
    beq @L_8001A6E8
    bge @L_8001A528
    b @L_8001A970
    @L_8001A370:
    cmpwi r0, 0x3c4
    beq @L_8001A6D4
    bge @L_8001A38C
    cmpwi r0, 0x3c2
    beq @L_8001A414
    bge @L_8001A440
    b @L_8001A634
    @L_8001A38C:
    cmpwi r0, 0x3c9
    beq @L_8001A970
    bge @L_8001A4FC
    cmpwi r0, 0x3c7
    bge @L_8001A440
    b @L_8001A414
    @L_8001A3A4:
    cmpwi r0, 0x129d
    bge @L_8001A3E0
    cmpwi r0, 0x545
    beq @L_8001A598
    bge @L_8001A3D0
    cmpwi r0, 0x543
    beq @L_8001A798
    bge @L_8001A970
    cmpwi r0, 0x542
    bge @L_8001A80C
    b @L_8001A970
    @L_8001A3D0:
    cmpwi r0, 0x547
    beq @L_8001A634
    bge @L_8001A970
    b @L_8001A608
    @L_8001A3E0:
    cmpwi r0, 0x12a2
    beq @L_8001A6E8
    bge @L_8001A404
    cmpwi r0, 0x12a0
    beq @L_8001A6D4
    bge @L_8001A970
    cmpwi r0, 0x129f
    bge @L_8001A970
    b @L_8001A414
    @L_8001A404:
    cmpwi r0, 0x12a4
    beq @L_8001A798
    bge @L_8001A970
    b @L_8001A80C
    @L_8001A414:
    lbz r0, 0x29(r28)
    cmplwi r0, 0x0
    bne @L_8001A430
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8001A970
    @L_8001A430:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A440:
    lbz r0, 0x29(r28)
    cmpwi r0, 0x1
    beq @L_8001A478
    bge @L_8001A45C
    cmpwi r0, 0x0
    bge @L_8001A468
    b @L_8001A970
    @L_8001A45C:
    cmpwi r0, 0x3
    bge @L_8001A970
    b @L_8001A488
    @L_8001A468:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A478:
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8001A970
    @L_8001A488:
    lha r4, lbl_8047A308(r13)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    mr r3, r26
    xoris r0, r4, 0x8000
    lfd f2, lbl_8047B7B8(r2)
    stw r0, 0xc(r1)
    mr r4, r31
    lfs f0, lbl_8047B7B0(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f0
    bl fn_8001DACC
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A4CC:
    lhz r9, 0x24(r28)
    cmplwi r9, 0x0
    beq @L_8001A970
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    mr r8, r26
    li r3, 0x0
    li r4, 0x0
    li r10, 0x0
    bl fn_80104160
    b @L_8001A970
    @L_8001A4FC:
    lhz r0, 0x2a(r28)
    cmplwi r0, 0x0
    beq @L_8001A518
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8001A970
    @L_8001A518:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A528:
    mr r5, r30
    li r3, 0x2e
    li r4, 0x0
    li r6, 0x2bd4
    bl fn_800FB680
    lha r4, 0x1a(r28)
    li r3, 0x34
    bl fn_80132A38
    lha r6, 0x56(r31)
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2c
    li r8, 0xde
    bl fn_800FBB34
    lha r4, 0x18(r28)
    li r3, 0x34
    bl fn_80132A38
    lha r4, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    subi r0, r4, 0x2
    li r4, 0x0
    extsh r5, r0
    li r8, 0xde
    bl fn_800FBB34
    b @L_8001A970
    @L_8001A598:
    mr r5, r30
    li r3, 0x1b
    li r4, 0x0
    li r6, 0x195
    bl fn_800FB680
    lha r4, 0x1a(r28)
    li r3, 0x34
    bl fn_80132A38
    lha r6, 0x56(r31)
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1b
    li r8, 0xdf
    bl fn_800FBB34
    lha r4, 0x18(r28)
    li r3, 0x34
    bl fn_80132A38
    lha r4, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    subi r0, r4, 0x2
    li r4, 0x0
    extsh r5, r0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_8001A970
    @L_8001A608:
    lbz r4, 0x17(r28)
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r30
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xd3
    bl fn_800FBB34
    b @L_8001A970
    @L_8001A634:
    mr r4, r28
    li r3, 0x37
    bl fn_80132A38
    mr r5, r30
    li r3, 0x0
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    li r3, 0xe7
    bl fn_800FA444
    lbz r0, 0x28(r28)
    srwi r3, r3, 16
    extsh r26, r3
    cmpwi r0, 0x1
    beq @L_8001A694
    bge @L_8001A680
    cmpwi r0, 0x0
    bge @L_8001A68C
    b @L_8001A6A0
    @L_8001A680:
    cmpwi r0, 0x3
    bge @L_8001A6A0
    b @L_8001A69C
    @L_8001A68C:
    li r29, 0xd67
    b @L_8001A6A0
    @L_8001A694:
    li r29, 0xd68
    b @L_8001A6A0
    @L_8001A69C:
    li r29, 0x0
    @L_8001A6A0:
    cmplwi r29, 0x0
    beq @L_8001A970
    mr r3, r29
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r5, r30
    subi r3, r26, 0x2
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_8001A970
    @L_8001A6D4:
    lhz r5, 0x26(r28)
    mr r3, r26
    mr r4, r31
    bl fn_8010B9E8
    b @L_8001A970
    @L_8001A6E8:
    lha r5, 0x1a(r28)
    lha r6, 0x18(r28)
    cmpwi r5, 0x0
    bgt @L_8001A700
    li r9, 0x0
    b @L_8001A754
    @L_8001A700:
    mulli r0, r6, 0x14
    lis r3, 0x51ec
    subi r4, r3, 0x7ae1
    mulhw r0, r4, r0
    srawi r0, r0, 5
    srwi r3, r0, 31
    add r0, r0, r3
    cmpw r5, r0
    bgt @L_8001A72C
    li r9, 0x66
    b @L_8001A754
    @L_8001A72C:
    mulli r0, r6, 0x32
    mulhw r0, r4, r0
    srawi r0, r0, 5
    srwi r3, r0, 31
    add r0, r0, r3
    cmpw r5, r0
    bgt @L_8001A750
    li r9, 0x65
    b @L_8001A754
    @L_8001A750:
    li r9, 0x64
    @L_8001A754:
    lha r3, 0x54(r31)
    clrlwi r0, r9, 16
    cmplwi r0, 0x0
    mullw r0, r5, r3
    add r3, r6, r0
    subi r0, r3, 0x1
    divw r0, r0, r6
    extsh r5, r0
    beq @L_8001A970
    lha r6, 0x56(r31)
    mr r7, r30
    mr r8, r26
    li r3, 0x0
    li r4, 0x0
    li r10, 0x0
    bl fn_80104160
    b @L_8001A970
    @L_8001A798:
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x7(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_8001A7DC
    cmpwi r0, 0x6
    bge @L_8001A7DC
    lbz r4, 0x4(r3)
    lis r3, lbl_802E4E58@ha
    addi r3, r3, lbl_802E4E58@l
    slwi r0, r0, 3
    extsb r4, r4
    mulli r4, r4, 0x30
    add r3, r3, r4
    lhax r3, r3, r0
    b @L_8001A7E0
    @L_8001A7DC:
    li r3, 0x0
    @L_8001A7E0:
    lwz r0, 0x4(r26)
    cmpw r3, r0
    bne @L_8001A7FC
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8001A970
    @L_8001A7FC:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A80C:
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x6(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_8001A850
    cmpwi r0, 0x6
    bge @L_8001A850
    lbz r4, 0x4(r3)
    lis r3, lbl_802E4E58@ha
    addi r3, r3, lbl_802E4E58@l
    slwi r0, r0, 3
    extsb r4, r4
    mulli r4, r4, 0x30
    add r3, r3, r4
    lhax r3, r3, r0
    b @L_8001A854
    @L_8001A850:
    li r3, 0x0
    @L_8001A854:
    lwz r0, 0x4(r26)
    cmpw r3, r0
    bne @L_8001A910
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x14(r3)
    cmpwi r0, 0x2
    beq @L_8001A8A0
    bge @L_8001A894
    cmpwi r0, 0x0
    beq @L_8001A8A0
    bge @L_8001A8D8
    b @L_8001A970
    @L_8001A894:
    cmpwi r0, 0x4
    bge @L_8001A970
    b @L_8001A8D8
    @L_8001A8A0:
    lha r0, 0x6(r31)
    mr r3, r26
    li r5, 0x2d
    clrlwi r4, r0, 16
    bl fn_80107E78
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001A970
    lha r0, 0x6(r31)
    mr r3, r26
    li r5, 0x2d
    clrlwi r4, r0, 16
    bl fn_801081F8
    b @L_8001A970
    @L_8001A8D8:
    lha r0, 0x6(r31)
    mr r3, r26
    li r5, 0x20d
    clrlwi r4, r0, 16
    bl fn_80107E78
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001A970
    lha r0, 0x6(r31)
    mr r3, r26
    li r5, 0x20d
    clrlwi r4, r0, 16
    bl fn_801081F8
    b @L_8001A970
    @L_8001A910:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8001A970
    @L_8001A920:
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x14(r3)
    cmpwi r0, 0x2
    beq @L_8001A954
    bge @L_8001A948
    cmpwi r0, 0x0
    beq @L_8001A954
    bge @L_8001A964
    b @L_8001A970
    @L_8001A948:
    cmpwi r0, 0x4
    bge @L_8001A970
    b @L_8001A964
    @L_8001A954:
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8001A970
    @L_8001A964:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_8001A970:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
