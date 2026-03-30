    nofralloc
    lwz r3, lbl_8047AA80(r13)
    lwz r7, 0x480(r3)
    lwz r4, 0x484(r3)
    lwz r5, 0x4(r7)
    lwz r0, 0x8(r7)
    addi r6, r4, 0x68
    add r0, r5, r0
    cmplw r6, r0
    ble @L_800DB0C8
    li r0, 0x1
    stb r0, 0x1(r7)
    blr
    @L_800DB0C8:
    lis r5, fn_800D75D0@ha
    lwz r6, 0x4a8(r3)
    addi r0, r5, fn_800D75D0@l
    cmplw r6, r0
    bne @L_800DB10C
    lwz r0, 0x4b8(r3)
    addi r5, r4, 0xc
    stw r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x4bc(r3)
    stw r0, 0x4(r4)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x4c0(r3)
    stw r0, 0x8(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB10C:
    lis r5, fn_800D75AC@ha
    addi r0, r5, fn_800D75AC@l
    cmplw r6, r0
    beq @L_800DB12C
    lis r5, fn_800D7588@ha
    addi r0, r5, fn_800D7588@l
    cmplw r6, r0
    bne @L_800DB15C
    @L_800DB12C:
    lhz r0, 0x4b0(r3)
    addi r5, r4, 0x6
    sth r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lhz r0, 0x4b2(r3)
    sth r0, 0x2(r4)
    lwz r3, lbl_8047AA80(r13)
    lhz r0, 0x4b4(r3)
    sth r0, 0x4(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB15C:
    lis r5, fn_800D7564@ha
    addi r0, r5, fn_800D7564@l
    cmplw r6, r0
    beq @L_800DB17C
    lis r5, fn_800D7540@ha
    addi r0, r5, fn_800D7540@l
    cmplw r6, r0
    bne @L_800DB1AC
    @L_800DB17C:
    lbz r0, 0x4ac(r3)
    addi r5, r4, 0x3
    stb r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x4ad(r3)
    stb r0, 0x1(r4)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x4ae(r3)
    stb r0, 0x2(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB1AC:
    lis r5, fn_800D7524@ha
    addi r0, r5, fn_800D7524@l
    cmplw r6, r0
    bne @L_800DB1E0
    lwz r0, 0x4b8(r3)
    addi r5, r4, 0x8
    stw r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x4bc(r3)
    stw r0, 0x4(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB1E0:
    lis r5, fn_800D7508@ha
    addi r0, r5, fn_800D7508@l
    cmplw r6, r0
    beq @L_800DB200
    lis r5, fn_800D74EC@ha
    addi r0, r5, fn_800D74EC@l
    cmplw r6, r0
    bne @L_800DB224
    @L_800DB200:
    lhz r0, 0x4b0(r3)
    addi r5, r4, 0x4
    sth r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lhz r0, 0x4b2(r3)
    sth r0, 0x2(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB224:
    lis r5, fn_800D74D0@ha
    addi r0, r5, fn_800D74D0@l
    cmplw r6, r0
    beq @L_800DB244
    lis r5, fn_800D74B4@ha
    addi r0, r5, fn_800D74B4@l
    cmplw r6, r0
    bne @L_800DB268
    @L_800DB244:
    lbz r0, 0x4ac(r3)
    addi r5, r4, 0x2
    stb r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x4ad(r3)
    stb r0, 0x1(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB268:
    lis r5, fn_800D74A0@ha
    addi r0, r5, fn_800D74A0@l
    cmplw r6, r0
    bne @L_800DB290
    lhz r0, 0x4b0(r3)
    addi r5, r4, 0x2
    sth r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB2B4
    @L_800DB290:
    lis r5, fn_800D748C@ha
    addi r0, r5, fn_800D748C@l
    cmplw r6, r0
    bne @L_800DB2B4
    lbz r0, 0x4ac(r3)
    addi r5, r4, 0x1
    stb r0, 0x0(r4)
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    @L_800DB2B4:
    lwz r4, lbl_8047AA80(r13)
    lwz r3, 0x480(r4)
    lwz r5, 0x4c4(r4)
    lwz r3, 0xc(r3)
    lbz r0, 0x40(r3)
    cmplwi r0, 0x1
    bne @L_800DB3F0
    lis r3, fn_800D7468@ha
    addi r0, r3, fn_800D7468@l
    cmplw r5, r0
    bne @L_800DB314
    lwz r5, 0x484(r4)
    lwz r0, 0x4d4(r4)
    stw r0, 0x0(r5)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x4d8(r3)
    stw r0, 0x4(r5)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x4dc(r3)
    stw r0, 0x8(r5)
    addi r5, r5, 0xc
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB3F0
    @L_800DB314:
    lis r3, fn_800D7444@ha
    addi r0, r3, fn_800D7444@l
    cmplw r5, r0
    bne @L_800DB358
    lwz r5, 0x484(r4)
    lhz r0, 0x4cc(r4)
    sth r0, 0x0(r5)
    lwz r3, lbl_8047AA80(r13)
    lhz r0, 0x4ce(r3)
    sth r0, 0x2(r5)
    lwz r3, lbl_8047AA80(r13)
    lhz r0, 0x4d0(r3)
    sth r0, 0x4(r5)
    addi r5, r5, 0x6
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB3F0
    @L_800DB358:
    lis r3, fn_800D7420@ha
    addi r0, r3, fn_800D7420@l
    cmplw r5, r0
    bne @L_800DB39C
    lwz r5, 0x484(r4)
    lbz r0, 0x4c8(r4)
    stb r0, 0x0(r5)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x4c9(r3)
    stb r0, 0x1(r5)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x4ca(r3)
    stb r0, 0x2(r5)
    addi r5, r5, 0x3
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB3F0
    @L_800DB39C:
    lis r3, fn_800D740C@ha
    addi r0, r3, fn_800D740C@l
    cmplw r5, r0
    bne @L_800DB3C8
    lwz r5, 0x484(r4)
    lhz r0, 0x4cc(r4)
    sth r0, 0x0(r5)
    addi r5, r5, 0x2
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    b @L_800DB3F0
    @L_800DB3C8:
    lis r3, fn_800D73F8@ha
    addi r0, r3, fn_800D73F8@l
    cmplw r5, r0
    bne @L_800DB3F0
    lwz r5, 0x484(r4)
    lbz r0, 0x4c8(r4)
    stb r0, 0x0(r5)
    addi r5, r5, 0x1
    lwz r3, lbl_8047AA80(r13)
    stw r5, 0x484(r3)
    @L_800DB3F0:
    li r0, 0x2
    li r5, 0x4
    li r3, 0x70
    mtctr r0
    @L_800DB400:
    lwz r4, lbl_8047AA80(r13)
    subi r8, r5, 0x4
    slwi r7, r8, 2
    addi r0, r3, 0x8
    lwz r6, 0x480(r4)
    addi r7, r7, 0x4e0
    lwzx r7, r4, r7
    lwz r6, 0xc(r6)
    lbzx r0, r6, r0
    cmplwi r0, 0x1
    bne @L_800DB5B0
    lis r6, fn_800D73C4@ha
    addi r0, r6, fn_800D73C4@l
    cmplw r7, r0
    bne @L_800DB490
    mulli r8, r8, 0xc
    lwz r9, 0x484(r4)
    addi r0, r8, 0x4e8
    lbzx r7, r4, r0
    addi r6, r8, 0x4e9
    addi r4, r8, 0x4ea
    addi r0, r8, 0x4eb
    stb r7, 0x0(r9)
    lwz r7, lbl_8047AA80(r13)
    lbzx r6, r7, r6
    stb r6, 0x1(r9)
    lwz r6, lbl_8047AA80(r13)
    lbzx r4, r6, r4
    stb r4, 0x2(r9)
    lwz r4, lbl_8047AA80(r13)
    lbzx r0, r4, r0
    stb r0, 0x3(r9)
    addi r9, r9, 0x4
    lwz r4, lbl_8047AA80(r13)
    stw r9, 0x484(r4)
    b @L_800DB5B0
    @L_800DB490:
    lis r6, fn_800D7398@ha
    addi r0, r6, fn_800D7398@l
    cmplw r7, r0
    bne @L_800DB4E4
    mulli r7, r8, 0xc
    lwz r8, 0x484(r4)
    addi r0, r7, 0x4e8
    lbzx r6, r4, r0
    addi r4, r7, 0x4e9
    addi r0, r7, 0x4ea
    stb r6, 0x0(r8)
    lwz r6, lbl_8047AA80(r13)
    lbzx r4, r6, r4
    stb r4, 0x1(r8)
    lwz r4, lbl_8047AA80(r13)
    lbzx r0, r4, r0
    stb r0, 0x2(r8)
    addi r8, r8, 0x3
    lwz r4, lbl_8047AA80(r13)
    stw r8, 0x484(r4)
    b @L_800DB5B0
    @L_800DB4E4:
    lis r6, fn_800D737C@ha
    addi r0, r6, fn_800D737C@l
    cmplw r7, r0
    bne @L_800DB518
    mulli r6, r8, 0xc
    lwz r7, 0x484(r4)
    addi r0, r6, 0x4f0
    lwzx r0, r4, r0
    stw r0, 0x0(r7)
    addi r7, r7, 0x4
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB5B0
    @L_800DB518:
    lis r6, fn_800D7360@ha
    addi r0, r6, fn_800D7360@l
    cmplw r7, r0
    bne @L_800DB54C
    mulli r6, r8, 0xc
    lwz r7, 0x484(r4)
    addi r0, r6, 0x4ec
    lhzx r0, r4, r0
    sth r0, 0x0(r7)
    addi r7, r7, 0x2
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB5B0
    @L_800DB54C:
    lis r6, fn_800D7344@ha
    addi r0, r6, fn_800D7344@l
    cmplw r7, r0
    bne @L_800DB580
    mulli r6, r8, 0xc
    lwz r7, 0x484(r4)
    addi r0, r6, 0x4ec
    lhzx r0, r4, r0
    sth r0, 0x0(r7)
    addi r7, r7, 0x2
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB5B0
    @L_800DB580:
    lis r6, fn_800D7328@ha
    addi r0, r6, fn_800D7328@l
    cmplw r7, r0
    bne @L_800DB5B0
    mulli r6, r8, 0xc
    lwz r7, 0x484(r4)
    addi r0, r6, 0x4e8
    lbzx r0, r4, r0
    stb r0, 0x0(r7)
    addi r7, r7, 0x1
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    @L_800DB5B0:
    addi r3, r3, 0x1c
    addi r5, r5, 0x1
    bdnz @L_800DB400
    li r0, 0x8
    li r5, 0x6
    li r3, 0xa8
    mtctr r0
    @L_800DB5CC:
    lwz r4, lbl_8047AA80(r13)
    subi r8, r5, 0x6
    slwi r7, r8, 2
    addi r0, r3, 0x8
    lwz r6, 0x480(r4)
    addi r7, r7, 0x500
    lwzx r7, r4, r7
    lwz r6, 0xc(r6)
    lbzx r0, r6, r0
    cmplwi r0, 0x1
    bne @L_800DB748
    lis r6, fn_800D7304@ha
    addi r0, r6, fn_800D7304@l
    cmplw r7, r0
    bne @L_800DB63C
    slwi r6, r8, 4
    lwz r7, 0x484(r4)
    addi r0, r6, 0x528
    lwzx r4, r4, r0
    addi r0, r6, 0x52c
    stw r4, 0x0(r7)
    lwz r4, lbl_8047AA80(r13)
    lwzx r0, r4, r0
    stw r0, 0x4(r7)
    addi r7, r7, 0x8
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB748
    @L_800DB63C:
    lis r6, fn_800D72E4@ha
    addi r0, r6, fn_800D72E4@l
    cmplw r7, r0
    beq @L_800DB65C
    lis r6, fn_800D72C4@ha
    addi r0, r6, fn_800D72C4@l
    cmplw r7, r0
    bne @L_800DB690
    @L_800DB65C:
    slwi r6, r8, 4
    lwz r7, 0x484(r4)
    addi r0, r6, 0x522
    lhzx r4, r4, r0
    addi r0, r6, 0x524
    sth r4, 0x0(r7)
    lwz r4, lbl_8047AA80(r13)
    lhzx r0, r4, r0
    sth r0, 0x2(r7)
    addi r7, r7, 0x4
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB748
    @L_800DB690:
    lis r6, fn_800D72A4@ha
    addi r0, r6, fn_800D72A4@l
    cmplw r7, r0
    beq @L_800DB6B0
    lis r6, fn_800D7284@ha
    addi r0, r6, fn_800D7284@l
    cmplw r7, r0
    bne @L_800DB6E4
    @L_800DB6B0:
    slwi r6, r8, 4
    lwz r7, 0x484(r4)
    addi r0, r6, 0x520
    lbzx r4, r4, r0
    addi r0, r6, 0x521
    stb r4, 0x0(r7)
    lwz r4, lbl_8047AA80(r13)
    lbzx r0, r4, r0
    stb r0, 0x1(r7)
    addi r7, r7, 0x2
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB748
    @L_800DB6E4:
    lis r6, fn_800D7268@ha
    addi r0, r6, fn_800D7268@l
    cmplw r7, r0
    bne @L_800DB718
    slwi r6, r8, 4
    lwz r7, 0x484(r4)
    addi r0, r6, 0x522
    lhzx r0, r4, r0
    sth r0, 0x0(r7)
    addi r7, r7, 0x2
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    b @L_800DB748
    @L_800DB718:
    lis r6, fn_800D724C@ha
    addi r0, r6, fn_800D724C@l
    cmplw r7, r0
    bne @L_800DB748
    slwi r6, r8, 4
    lwz r7, 0x484(r4)
    addi r0, r6, 0x520
    lbzx r0, r4, r0
    stb r0, 0x0(r7)
    addi r7, r7, 0x1
    lwz r4, lbl_8047AA80(r13)
    stw r7, 0x484(r4)
    @L_800DB748:
    addi r3, r3, 0x1c
    addi r5, r5, 0x1
    bdnz @L_800DB5CC
    blr
