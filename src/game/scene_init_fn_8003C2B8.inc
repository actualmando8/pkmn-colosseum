    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stmw r20, 0x20(r1)
    mr r30, r3
    lis r5, lbl_803A6748@ha
    lis r3, lbl_802EF0A8@ha
    addi r5, r5, lbl_803A6748@l
    lfs f1, lbl_8047BAC0(r2)
    lfs f0, 0x44(r5)
    addi r7, r3, lbl_802EF0A8@l
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r30)
    lha r0, 0x6(r4)
    lha r3, 0x4fa2(r7)
    mulli r4, r0, 0x1c
    lha r0, 0x4fa4(r7)
    lha r5, 0x4fa6(r7)
    lha r6, 0x4fa8(r7)
    add r4, r7, r4
    lha r7, 0x2(r4)
    lha r4, 0x4(r4)
    subf r3, r7, r3
    subf r4, r4, r0
    bl fn_800FE38C
    lis r3, lbl_803A6748@ha
    lfs f1, lbl_8047BAF8(r2)
    addi r27, r3, lbl_803A6748@l
    lfs f0, lbl_8047BAFC(r2)
    lfs f29, 0x14(r27)
    addi r25, r27, 0x94
    lfs f2, lbl_8047BAC8(r2)
    li r31, 0x0
    fsubs f1, f29, f1
    lfs f28, 0x18(r27)
    li r29, 0x0
    fsubs f0, f1, f0
    fadds f30, f2, f0
    lfs f31, lbl_8047BAD8(r2)
    b @L_8003C6E4
    @L_8003C384:
    lwz r3, 0x8(r27)
    subi r0, r3, 0x1
    cmpw r31, r0
    blt @L_8003C6D8
    lwz r3, 0xc(r27)
    addi r0, r3, 0x1
    cmpw r31, r0
    bgt @L_8003C6D8
    lwz r3, lbl_8047A4D4(r13)
    addi r0, r29, 0x2
    li r4, 0x0
    lhzx r22, r3, r0
    mr r3, r22
    bl fn_801EEFAC
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    bne @L_8003C3EC
    mr r3, r22
    bl fn_801EE248
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003C440
    bl fn_8011E760
    bl fn_800FA280
    mr r26, r3
    b @L_8003C440
    @L_8003C3EC:
    bl fn_801FCCC4
    bl fn_801FCC3C
    bl fn_801FCA2C
    mr r21, r3
    bl fn_801FCA14
    bl fn_800FA280
    mr r26, r3
    @L_8003C408:
    mr r3, r21
    bl fn_801FC964
    clrlwi r0, r3, 24
    cmpw r22, r0
    bne @L_8003C438
    mr r3, r21
    bl fn_801FCA14
    cmplwi r3, 0x0
    beq @L_8003C440
    bl fn_800FA280
    mr r26, r3
    b @L_8003C440
    @L_8003C438:
    addi r21, r21, 0x50
    b @L_8003C408
    @L_8003C440:
    cmplwi r26, 0x0
    mr r24, r26
    bne @L_8003C458
    li r3, 0x1
    bl fn_800FA280
    mr r24, r3
    @L_8003C458:
    lwz r0, lbl_8047A4D0(r13)
    li r21, 0x0
    cmplwi r0, 0x0
    beq @L_8003C514
    lwz r3, lbl_8047A4D4(r13)
    addi r0, r29, 0x2
    mr r4, r25
    lhzx r23, r3, r0
    mr r3, r23
    bl fn_801EE544
    lbz r0, 0x0(r25)
    mr r22, r3
    cmpwi r0, 0x0
    beq @L_8003C4A0
    blt @L_8003C4F0
    cmpwi r0, 0x3
    bge @L_8003C4F0
    b @L_8003C4A8
    @L_8003C4A0:
    li r22, 0x371f
    b @L_8003C4F0
    @L_8003C4A8:
    cmplwi r23, 0x43
    bne @L_8003C4B8
    li r22, 0x12b0
    b @L_8003C4F0
    @L_8003C4B8:
    mr r3, r23
    li r4, 0x0
    bl fn_801EEFAC
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    bne @L_8003C4E4
    mr r3, r23
    bl fn_801EE328
    li r21, 0x1
    mr r28, r3
    b @L_8003C4F0
    @L_8003C4E4:
    bl fn_801FCCC4
    bl fn_801FCC7C
    mr r22, r3
    @L_8003C4F0:
    clrlwi r0, r21, 24
    cmplwi r0, 0x0
    beq @L_8003C504
    mr r21, r28
    b @L_8003C514
    @L_8003C504:
    mr r3, r22
    bl fn_800FA280
    mr r28, r3
    mr r21, r28
    @L_8003C514:
    cmplwi r21, 0x0
    mr r23, r21
    bne @L_8003C52C
    li r3, 0x1
    bl fn_800FA280
    mr r23, r3
    @L_8003C52C:
    mr r3, r31
    bl fn_8003CE1C
    mr r22, r3
    cmplwi r22, 0x0
    bne @L_8003C54C
    li r3, 0x1
    bl fn_800FA280
    mr r22, r3
    @L_8003C54C:
    clrlwi r0, r31, 16
    lwz r4, lbl_8047A4D4(r13)
    slwi r3, r0, 2
    addi r0, r3, 0x2
    lhzx r20, r4, r0
    mr r3, r20
    bl fn_801EE614
    mr r3, r20
    bl fn_801EE8F4
    mr r3, r20
    bl fn_801EEAD0
    mr r21, r3
    mr r3, r20
    bl fn_801EEC74
    clrlwi r0, r21, 24
    cmplwi r0, 0x0
    beq @L_8003C5AC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003C5A4
    li r0, 0x1
    b @L_8003C5B0
    @L_8003C5A4:
    li r0, 0x0
    b @L_8003C5B0
    @L_8003C5AC:
    li r0, 0x2
    @L_8003C5B0:
    cmpwi r0, 0x1
    beq @L_8003C604
    bge @L_8003C5C8
    cmpwi r0, 0x0
    bge @L_8003C5D0
    b @L_8003C634
    @L_8003C5C8:
    cmpwi r0, 0x3
    b @L_8003C634
    @L_8003C5D0:
    lfs f0, lbl_8047BB00(r2)
    fctiwz f1, f30
    mr r5, r30
    li r6, 0x161
    fsubs f0, f28, f0
    li r7, 0x0
    stfd f1, 0x8(r1)
    fctiwz f0, f0
    lwz r3, 0xc(r1)
    stfd f0, 0x10(r1)
    lwz r4, 0x14(r1)
    bl fn_801040F0
    b @L_8003C634
    @L_8003C604:
    lfs f1, lbl_8047BAF8(r2)
    fctiwz f0, f28
    mr r5, r30
    li r6, 0x160
    fsubs f1, f29, f1
    li r7, 0x0
    stfd f0, 0x8(r1)
    fctiwz f0, f1
    lwz r4, 0xc(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x14(r1)
    bl fn_801040F0
    @L_8003C634:
    lwz r3, lbl_8047A4D4(r13)
    addi r0, r29, 0x2
    lhzx r3, r3, r0
    bl fn_801EED88
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003C65C
    lis r3, 0xe0e0
    addi r21, r3, 0x1800
    b @L_8003C660
    @L_8003C65C:
    li r21, -0x100
    @L_8003C660:
    mr r4, r24
    li r3, 0x37
    bl fn_80132A38
    fctiwz f0, f28
    lbz r0, 0x8b(r30)
    li r3, 0x0
    li r6, 0xe7
    or r5, r21, r0
    stfd f0, 0x10(r1)
    lwz r20, 0x14(r1)
    mr r4, r20
    bl fn_800FB680
    mr r4, r22
    li r3, 0x37
    bl fn_80132A38
    lbz r0, 0x8b(r30)
    mr r4, r20
    li r3, 0x89
    li r6, 0xe7
    or r5, r21, r0
    bl fn_800FB680
    mr r4, r23
    li r3, 0x37
    bl fn_80132A38
    lbz r0, 0x8b(r30)
    mr r4, r20
    li r3, 0x13d
    li r6, 0xe7
    or r5, r21, r0
    bl fn_800FB680
    @L_8003C6D8:
    fadds f28, f28, f31
    addi r29, r29, 0x4
    addi r31, r31, 0x1
    @L_8003C6E4:
    lwz r0, lbl_8047A4DC(r13)
    cmpw r31, r0
    blt @L_8003C384
    bl fn_800FE35C
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    lmw r20, 0x20(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
