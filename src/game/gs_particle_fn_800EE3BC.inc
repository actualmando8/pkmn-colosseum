    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r24, 0x50(r1)
    mr r25, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    li r31, 0x0
    li r27, 0x0
    lwz r24, 0x8(r3)
    lwz r3, 0x4(r3)
    bl fn_800EC0E8
    lwz r3, 0x4(r25)
    bl fn_800EC5AC
    clrlwi. r0, r3, 24
    bne @L_800EE460
    cmplwi r24, 0x0
    beq @L_800EE448
    bne @L_800EE41C
    la r3, lbl_8047CCA0(r2)
    li r4, 0x25d
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE41C:
    lwz r4, 0x14(r24)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EE438
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EE438
    li r3, 0x1
    @L_800EE438:
    cmpwi r3, 0x0
    beq @L_800EE448
    mr r3, r24
    bl fn_8019D9DC
    @L_800EE448:
    lfs f1, 0x50(r24)
    addi r3, r1, 0x38
    lfs f2, 0x60(r24)
    lfs f3, 0x70(r24)
    bl fn_800E01F4
    b @L_800EE490
    @L_800EE460:
    lwz r0, 0x14(r24)
    rlwinm. r0, r0, 0, 9, 10
    beq @L_800EE47C
    addi r3, r1, 0x38
    addi r4, r24, 0x38
    bl fn_800E01D0
    b @L_800EE490
    @L_800EE47C:
    lfs f1, 0x50(r24)
    addi r3, r1, 0x38
    lfs f2, 0x60(r24)
    lfs f3, 0x70(r24)
    bl fn_800E01F4
    @L_800EE490:
    cmplwi r29, 0x0
    bne @L_800EE4A0
    cmplwi r30, 0x0
    beq @L_800EE664
    @L_800EE4A0:
    lis r3, lbl_80270F78@ha
    lis r4, lbl_804018B0@ha
    addi r26, r3, lbl_80270F78@l
    addi r25, r4, lbl_804018B0@l
    b @L_800EE4EC
    @L_800EE4B4:
    cmplwi r24, 0x0
    stwx r24, r25, r27
    addi r31, r31, 0x1
    addi r27, r27, 0x4
    bne @L_800EE4D0
    li r24, 0x0
    b @L_800EE4D4
    @L_800EE4D0:
    lwz r24, 0xc(r24)
    @L_800EE4D4:
    cmplwi r31, 0x10
    blt @L_800EE4EC
    mr r3, r26
    crxor 6, 6, 6
    bl fn_800DD970
    li r24, 0x0
    @L_800EE4EC:
    cmplwi r24, 0x0
    bne @L_800EE4B4
    cmplwi r29, 0x0
    beq @L_800EE5AC
    lfs f1, lbl_8047CCB0(r2)
    mr r27, r31
    addi r3, r1, 0x2c
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lis r3, lbl_804018B0@ha
    slwi r4, r31, 2
    addi r0, r3, lbl_804018B0@l
    add r25, r0, r4
    b @L_800EE5A4
    @L_800EE528:
    lwz r26, -0x4(r25)
    cmplwi r26, 0x0
    bne @L_800EE544
    la r3, lbl_8047CCA0(r2)
    li r4, 0x2ec
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE544:
    lfs f0, 0x1c(r26)
    cmplwi r26, 0x0
    stfs f0, 0x20(r1)
    bne @L_800EE564
    la r3, lbl_8047CCA0(r2)
    li r4, 0x2fa
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE564:
    lfs f0, 0x20(r26)
    cmplwi r26, 0x0
    stfs f0, 0x24(r1)
    bne @L_800EE584
    la r3, lbl_8047CCA0(r2)
    li r4, 0x308
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE584:
    lfs f0, 0x24(r26)
    addi r3, r1, 0x2c
    mr r4, r3
    addi r5, r1, 0x20
    stfs f0, 0x28(r1)
    bl fn_800E019C
    subi r25, r25, 0x4
    subi r27, r27, 0x1
    @L_800EE5A4:
    cmplwi r27, 0x0
    bne @L_800EE528
    @L_800EE5AC:
    cmplwi r30, 0x0
    beq @L_800EE664
    lfs f1, lbl_8047CCB4(r2)
    mr r27, r31
    addi r3, r1, 0x14
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lis r3, lbl_804018B0@ha
    slwi r4, r31, 2
    addi r0, r3, lbl_804018B0@l
    add r25, r0, r4
    b @L_800EE65C
    @L_800EE5E0:
    lwz r26, -0x4(r25)
    cmplwi r26, 0x0
    bne @L_800EE5FC
    la r3, lbl_8047CCA0(r2)
    li r4, 0x383
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE5FC:
    lfs f0, 0x2c(r26)
    cmplwi r26, 0x0
    stfs f0, 0x8(r1)
    bne @L_800EE61C
    la r3, lbl_8047CCA0(r2)
    li r4, 0x38f
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE61C:
    lfs f0, 0x30(r26)
    cmplwi r26, 0x0
    stfs f0, 0xc(r1)
    bne @L_800EE63C
    la r3, lbl_8047CCA0(r2)
    li r4, 0x39b
    la r5, lbl_8047CCA8(r2)
    bl fn_80196E10
    @L_800EE63C:
    lfs f0, 0x34(r26)
    addi r3, r1, 0x14
    mr r4, r3
    addi r5, r1, 0x8
    stfs f0, 0x10(r1)
    bl fn_800E0108
    subi r25, r25, 0x4
    subi r27, r27, 0x1
    @L_800EE65C:
    cmplwi r27, 0x0
    bne @L_800EE5E0
    @L_800EE664:
    cmplwi r28, 0x0
    beq @L_800EE678
    mr r3, r28
    addi r4, r1, 0x38
    bl fn_800E01D0
    @L_800EE678:
    cmplwi r29, 0x0
    beq @L_800EE68C
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E01D0
    @L_800EE68C:
    cmplwi r30, 0x0
    beq @L_800EE6A0
    mr r3, r30
    addi r4, r1, 0x14
    bl fn_800E01D0
    @L_800EE6A0:
    lmw r24, 0x50(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
