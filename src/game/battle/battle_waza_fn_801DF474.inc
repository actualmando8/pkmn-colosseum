    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r25, r4
    mr r24, r3
    li r31, 0x0
    li r30, 0x1
    li r29, 0x0
    li r4, 0x0
    lwz r5, -31088(r13)
    lwz r6, -31084(r13)
    lwz r26, 0x0(r5)
    cmplwi r26, 0x0
    ble @L_801DF53C
    cmplwi r26, 0x8
    subi r3, r26, 0x8
    ble @L_801DF51C
    addi r0, r3, 0x7
    rlwinm r0, r0, 29, 3, 31
    mtctr r0
    cmplwi r3, 0x0
    ble @L_801DF51C
@L_801DF4D0:
    lhz r3, 0x2(r6)
    addi r4, r4, 0x8
    lhz r0, 0x6(r6)
    add r29, r29, r3
    lhz r3, 0xa(r6)
    add r29, r29, r0
    lhz r0, 0xe(r6)
    add r29, r29, r3
    lhz r3, 0x12(r6)
    add r29, r29, r0
    lhz r0, 0x16(r6)
    add r29, r29, r3
    lhz r3, 0x1a(r6)
    add r29, r29, r0
    lhz r0, 0x1e(r6)
    add r29, r29, r3
    addi r6, r6, 0x20
    add r29, r29, r0
    bdnz @L_801DF4D0
@L_801DF51C:
    subf r0, r4, r26
    mtctr r0
    cmplw r4, r26
    bge @L_801DF53C
@L_801DF52C:
    lhz r0, 0x2(r6)
    addi r6, r6, 0x4
    add r29, r29, r0
    bdnz @L_801DF52C
@L_801DF53C:
    bl fn_800E0C54
    rlwinm r3, r3, 0, 16, 31
    lwz r5, -31084(r13)
    divwu r0, r3, r29
    li r4, 0x0
    mullw r0, r0, r29
    subf r3, r0, r3
    mtctr r26
    cmplwi r26, 0x0
    ble @L_801DF58C
@L_801DF564:
    lhz r0, 0x2(r5)
    add r4, r4, r0
    cmplw r3, r4
    bge @L_801DF584
    lhz r0, 0x0(r5)
    mr r28, r0
    mr r27, r0
    b @L_801DF58C
@L_801DF584:
    addi r5, r5, 0x4
    bdnz @L_801DF564
@L_801DF58C:
    mr r26, r28
    rlwinm r28, r27, 0, 16, 31
@L_801DF594:
    cmplwi r31, 0x7
    bgt @L_801DF774
    lis r3, -32713
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 20736
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r24
    mr r4, r25
    bl fn_8018D998
    bl fn_8018D928
    mr. r29, r3
    beq @L_801DF620
    bl fn_8018FC50
    addi r3, r1, 0x8
    bl fn_8012D32C
    mr r3, r29
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    bl fn_800E0168
    lfs f1, 0x14(r1)
    lfs f2, 0x1c(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f2, -21168(r2)
    mr r3, r24
    mr r4, r25
    bl fn_8018805C
    mr r3, r24
    mr r4, r25
    li r5, 0x1
    bl fn_8018A280
@L_801DF620:
    li r3, 0xd0
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_801DF680
    li r3, 0xd1
    bl fn_801906A0
    cmplwi r3, 0x1f4
    blt @L_801DF654
    li r3, 0xd1
    li r4, 0x0
    bl fn_8019075C
    li r0, 0x1
    b @L_801DF690
@L_801DF654:
    li r3, 0xafe
    bl fn_801902E0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DF678
    li r3, 0xafe
    li r4, 0x0
    bl fn_8019075C
    li r0, -0x1
    b @L_801DF690
@L_801DF678:
    li r0, 0x0
    b @L_801DF690
@L_801DF680:
    li r3, 0xd0
    li r4, 0x1
    bl fn_8019075C
    li r0, 0x1
@L_801DF690:
    cmpwi r0, 0x0
    ble @L_801DF6A0
    li r31, 0x1
    b @L_801DF774
@L_801DF6A0:
    bge @L_801DF6AC
    li r31, 0x3
    b @L_801DF774
@L_801DF6AC:
    li r31, 0x4
    b @L_801DF774
    li r3, 0x5571
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r31, 0x2
    b @L_801DF774
    li r3, 0x3ca
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    mr r4, r26
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x3
    li r4, 0x3cb8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    cmpwi r3, 0x0
    beq @L_801DF730
    ble @L_801DF730
    mr r4, r28
    rlwinm r5, r3, 0, 16, 31
    li r3, 0x0
    bl fn_8013467C
@L_801DF730:
    li r31, 0x7
    b @L_801DF774
    li r3, 0x5573
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r31, 0x7
    b @L_801DF774
    li r3, 0x5574
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r31, 0x7
    b @L_801DF774
    li r30, 0x0
@L_801DF774:
    cmpwi r30, 0x0
    bne @L_801DF594
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
