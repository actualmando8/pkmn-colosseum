    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    lwz r5, lbl_8047B610(r13)
    lbz r3, 0x1(r5)
    lbz r28, 0x2(r5)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0x121
    li r6, 0x0
    bl fn_8012640C
    extsh r25, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r24, r3
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    mr r29, r3
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    mr r3, r24
    mr r30, r0
    li r4, 0x4
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F708
    mr r3, r24
    li r4, 0x4
    li r5, 0x1
    bl fn_80202A2C
@L_8021F708:
    mr r3, r29
    mr r4, r25
    bl fn_801F8F24
    mr r4, r24
    bl fn_80204970
    mr r3, r24
    li r4, 0x0
    li r5, 0x0
    bl fn_8010AE2C
    mr r3, r24
    bl fn_80205BE8
    bl fn_80121C18
    mr r0, r3
    mr r3, r31
    mr r5, r0
    mr r4, r24
    bl fn_802068C8
    mr r3, r31
    bl fn_80208028
    lis r3, lbl_80279FE0@ha
    li r0, 0x2
    addi r3, r3, lbl_80279FE0@l
    addi r5, r1, 0xc
    subi r4, r3, 0x4
    mtctr r0
@L_8021F76C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8021F76C
    lwz r0, 0x4(r4)
    cmplwi r28, 0x1
    li r26, 0x0
    stw r0, 0x4(r5)
    lhz r0, 0x8(r4)
    sth r0, 0x8(r5)
    bne @L_8021F7A0
    li r26, 0x7f
@L_8021F7A0:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x7f
    bne @L_8021F804
    mr r3, r31
    mr r4, r30
    bl fn_80201600
    addi r27, r1, 0x10
    li r25, 0x0
    b @L_8021F7F8
@L_8021F7C4:
    clrlslwi r0, r25, 24, 1
    mr r3, r30
    lhzx r24, r27, r0
    mr r4, r24
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F7F4
    mr r3, r31
    mr r4, r30
    mr r5, r24
    bl fn_802019BC
@L_8021F7F4:
    addi r25, r25, 0x1
@L_8021F7F8:
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0xb
    blt @L_8021F7C4
@L_8021F804:
    rlwinm r0, r26, 0, 16, 31
    lis r3, -32733
    stw r31, 0x8(r1)
    addi r4, r3, 11560
    addi r5, r1, 0x8
    li r3, 0x0
    stw r0, 0xc(r1)
    li r6, 0x0
    bl fn_801F37B0
    mr r3, r30
    li r4, 0x34
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F850
    mr r3, r31
    mr r4, r30
    li r5, 0x34
    bl fn_802019BC
@L_8021F850:
    mr r3, r30
    li r4, 0x35
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F878
    mr r3, r31
    mr r4, r30
    li r5, 0x35
    bl fn_802019BC
@L_8021F878:
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r29
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr. r24, r3
    beq @L_8021F8CC
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r4, r3
    beq @L_8021F8CC
    mr r3, r24
    bl fn_801C3E3C
@L_8021F8CC:
    cmplwi r28, 0x2
    beq @L_8021F8E4
    mr r3, r29
    mr r4, r31
    li r5, 0x0
    bl fn_801FBC20
@L_8021F8E4:
    mr r3, r31
    li r4, 0x0
    bl fn_80208C18
    bl fn_801C3430
    cmplwi r28, 0x2
    beq @L_8021F90C
    mr r3, r29
    mr r4, r31
    li r5, 0x1
    bl fn_801FBC20
@L_8021F90C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
