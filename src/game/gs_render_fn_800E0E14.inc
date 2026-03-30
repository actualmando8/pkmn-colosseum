    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r25, r3
    lis r3, lbl_80270658@ha
    mr r26, r4
    clrlwi. r0, r25, 24
    li r29, 0x1
    addi r31, r3, lbl_80270658@l
    li r28, 0x0
    li r27, 0x0
    beq @L_800E0E54
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD970
    @L_800E0E54:
    lwz r3, lbl_8047AB30(r13)
    cmplwi r3, 0x0
    beq @L_800E0E7C
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800E0E7C
    addi r3, r31, 0x34
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0E7C:
    lwz r24, lbl_8047AB30(r13)
    b @L_800E0F54
    @L_800E0E84:
    lwz r0, lbl_8047AB68(r13)
    addi r27, r27, 0x1
    cmplw r24, r0
    blt @L_800E0EA0
    lwz r0, lbl_8047AB64(r13)
    cmplw r24, r0
    ble @L_800E0EB0
    @L_800E0EA0:
    addi r3, r31, 0x64
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0EB0:
    lwz r0, lbl_8047AB38(r13)
    cmplw r24, r0
    blt @L_800E0ECC
    addi r3, r31, 0x90
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0ECC:
    lwz r3, 0x8(r24)
    lwz r0, lbl_8047AB38(r13)
    add r3, r24, r3
    cmplw r3, r0
    ble @L_800E0EF0
    addi r3, r31, 0xc0
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0EF0:
    lwz r30, 0x4(r24)
    cmplwi r30, 0x0
    beq @L_800E0F50
    lwz r0, 0x0(r30)
    cmplw r0, r24
    beq @L_800E0F18
    addi r3, r31, 0xf8
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0F18:
    lwz r0, 0x8(r24)
    add r0, r24, r0
    cmplw r30, r0
    bne @L_800E0F38
    addi r3, r31, 0x124
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0F38:
    cmplw r24, r30
    ble @L_800E0F50
    addi r3, r31, 0x154
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0F50:
    lwz r24, 0x4(r24)
    @L_800E0F54:
    cmplwi r24, 0x0
    bne @L_800E0E84
    lwz r30, lbl_8047AB34(r13)
    b @L_800E11EC
    @L_800E0F64:
    lhz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_800E11E8
    lwz r3, 0x4(r30)
    addi r28, r28, 0x1
    lwz r0, lbl_8047AB68(r13)
    cmplw r3, r0
    blt @L_800E0F94
    lwz r0, 0x8(r30)
    add r0, r3, r0
    cmplw r0, r4
    ble @L_800E0FA4
    @L_800E0F94:
    addi r3, r31, 0x180
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E0FA4:
    lbz r0, lbl_8047AB28(r13)
    cmplwi r0, 0x0
    beq @L_800E11E8
    lwz r4, 0x4(r30)
    lbz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_800E0FC8
    li r0, 0x0
    b @L_800E1064
    @L_800E0FC8:
    lbz r0, 0x1(r4)
    cmplwi r0, 0x0
    beq @L_800E0FDC
    li r0, 0x0
    b @L_800E1064
    @L_800E0FDC:
    lbz r0, 0x2(r4)
    cmplwi r0, 0x0
    beq @L_800E0FF0
    li r0, 0x0
    b @L_800E1064
    @L_800E0FF0:
    lbz r0, 0x3(r4)
    cmplwi r0, 0x0
    beq @L_800E1004
    li r0, 0x0
    b @L_800E1064
    @L_800E1004:
    lwz r3, 0x8(r30)
    subi r3, r3, 0x4
    add r3, r4, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800E1024
    li r0, 0x0
    b @L_800E1064
    @L_800E1024:
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    beq @L_800E1038
    li r0, 0x0
    b @L_800E1064
    @L_800E1038:
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_800E104C
    li r0, 0x0
    b @L_800E1064
    @L_800E104C:
    lbz r0, 0x3(r3)
    cmplwi r0, 0x0
    beq @L_800E1060
    li r0, 0x0
    b @L_800E1064
    @L_800E1060:
    li r0, 0x1
    @L_800E1064:
    clrlwi. r0, r0, 24
    bne @L_800E10B8
    lhz r4, 0x0(r30)
    addi r3, r31, 0x1ac
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r3, 0x4(r30)
    li r4, 0x0
    li r29, 0x0
    stb r4, 0x0(r3)
    stb r4, 0x1(r3)
    stb r4, 0x2(r3)
    stb r4, 0x3(r3)
    lwz r3, 0x8(r30)
    lwz r0, 0x4(r30)
    subi r3, r3, 0x4
    add r3, r0, r3
    stb r4, 0x0(r3)
    stb r4, 0x1(r3)
    stb r4, 0x2(r3)
    stb r4, 0x3(r3)
    @L_800E10B8:
    lhz r0, 0x2(r30)
    cmplwi r0, 0x0
    bne @L_800E11E8
    lwz r0, 0x8(r30)
    li r6, 0x3d94
    lwz r5, 0x4(r30)
    srwi. r4, r0, 1
    clrlwi r3, r0, 31
    beq @L_800E114C
    srwi. r0, r4, 3
    mtctr r0
    beq @L_800E1138
    @L_800E10E8:
    lhz r0, 0x0(r5)
    add r6, r6, r0
    lhz r0, 0x2(r5)
    add r6, r6, r0
    lhz r0, 0x4(r5)
    add r6, r6, r0
    lhz r0, 0x6(r5)
    add r6, r6, r0
    lhz r0, 0x8(r5)
    add r6, r6, r0
    lhz r0, 0xa(r5)
    add r6, r6, r0
    lhz r0, 0xc(r5)
    add r6, r6, r0
    lhz r0, 0xe(r5)
    addi r5, r5, 0x10
    add r6, r6, r0
    bdnz @L_800E10E8
    andi. r4, r4, 0x7
    beq @L_800E114C
    @L_800E1138:
    mtctr r4
    @L_800E113C:
    lhz r0, 0x0(r5)
    addi r5, r5, 0x2
    add r6, r6, r0
    bdnz @L_800E113C
    @L_800E114C:
    cmplwi r3, 0x0
    beq @L_800E11C4
    srwi. r0, r3, 3
    mtctr r0
    beq @L_800E11B0
    @L_800E1160:
    lbz r0, 0x0(r5)
    add r6, r6, r0
    lbz r0, 0x1(r5)
    add r6, r6, r0
    lbz r0, 0x2(r5)
    add r6, r6, r0
    lbz r0, 0x3(r5)
    add r6, r6, r0
    lbz r0, 0x4(r5)
    add r6, r6, r0
    lbz r0, 0x5(r5)
    add r6, r6, r0
    lbz r0, 0x6(r5)
    add r6, r6, r0
    lbz r0, 0x7(r5)
    addi r5, r5, 0x8
    add r6, r6, r0
    bdnz @L_800E1160
    andi. r3, r3, 0x7
    beq @L_800E11C4
    @L_800E11B0:
    mtctr r3
    @L_800E11B4:
    lbz r0, 0x0(r5)
    addi r5, r5, 0x1
    add r6, r6, r0
    bdnz @L_800E11B4
    @L_800E11C4:
    lhz r3, 0xe(r30)
    clrlwi r0, r6, 16
    cmplw r3, r0
    beq @L_800E11E8
    lhz r4, 0x0(r30)
    addi r3, r31, 0x1d8
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E11E8:
    subi r30, r30, 0x10
    @L_800E11EC:
    lwz r4, lbl_8047AB38(r13)
    cmplw r30, r4
    bge @L_800E0F64
    clrlwi. r0, r26, 24
    beq @L_800E120C
    addi r3, r31, 0x208
    crxor 6, 6, 6
    bl fn_800DD970
    @L_800E120C:
    lwz r30, lbl_8047AB68(r13)
    b @L_800E1368
    @L_800E1214:
    lwz r24, lbl_8047AB34(r13)
    addi r0, r24, 0x10
    subf r0, r3, r0
    srwi r0, r0, 4
    mtctr r0
    cmplw r24, r3
    blt @L_800E1254
    @L_800E1230:
    lhz r0, 0x0(r24)
    cmplwi r0, 0x0
    beq @L_800E124C
    lwz r0, 0x4(r24)
    cmplw r0, r30
    bne @L_800E124C
    b @L_800E1258
    @L_800E124C:
    subi r24, r24, 0x10
    bdnz @L_800E1230
    @L_800E1254:
    li r24, 0x0
    @L_800E1258:
    cmplwi r24, 0x0
    beq @L_800E12A4
    lhz r7, 0x0(r24)
    cmplwi r7, 0x0
    beq @L_800E12A4
    clrlwi. r0, r26, 24
    beq @L_800E1298
    lwz r6, 0x8(r24)
    mr r4, r30
    lhz r8, 0x2(r24)
    addi r3, r31, 0x21c
    add r5, r6, r30
    lhz r9, 0xc(r24)
    crxor 6, 6, 6
    subi r5, r5, 0x1
    bl fn_800DD970
    @L_800E1298:
    lwz r0, 0x8(r24)
    add r30, r30, r0
    b @L_800E1368
    @L_800E12A4:
    clrlwi. r0, r26, 24
    beq @L_800E12D0
    lwz r6, 0x8(r30)
    mr r4, r30
    lwz r7, 0x0(r30)
    addi r3, r31, 0x268
    add r5, r6, r30
    lwz r8, 0x4(r30)
    crxor 6, 6, 6
    subi r5, r5, 0x1
    bl fn_800DD970
    @L_800E12D0:
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_800E12F4
    lwz r0, lbl_8047AB68(r13)
    cmplw r3, r0
    blt @L_800E1318
    lwz r0, lbl_8047AB64(r13)
    cmplw r3, r0
    bgt @L_800E1318
    @L_800E12F4:
    lwz r3, 0x4(r30)
    cmplwi r3, 0x0
    beq @L_800E1330
    lwz r0, lbl_8047AB68(r13)
    cmplw r3, r0
    blt @L_800E1318
    lwz r0, lbl_8047AB64(r13)
    cmplw r3, r0
    ble @L_800E1330
    @L_800E1318:
    mr r4, r30
    addi r3, r31, 0x2a8
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    b @L_800E1374
    @L_800E1330:
    lwz r5, 0x8(r30)
    lwz r0, lbl_8047AB38(r13)
    add r3, r30, r5
    cmplw r3, r0
    bgt @L_800E134C
    cmplwi r5, 0x0
    bne @L_800E1364
    @L_800E134C:
    mr r4, r30
    addi r3, r31, 0x2e8
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    b @L_800E1374
    @L_800E1364:
    add r30, r30, r5
    @L_800E1368:
    lwz r3, lbl_8047AB38(r13)
    cmplw r30, r3
    blt @L_800E1214
    @L_800E1374:
    lwz r5, lbl_8047AB38(r13)
    cmplw r30, r5
    beq @L_800E1394
    mr r4, r30
    addi r3, r31, 0x318
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E1394:
    lwz r0, lbl_8047AB4C(r13)
    cmplw r28, r0
    beq @L_800E13B0
    addi r3, r31, 0x36c
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    @L_800E13B0:
    clrlwi. r0, r25, 24
    beq @L_800E151C
    lwz r4, lbl_8047AB68(r13)
    addi r3, r31, 0x3a4
    lwz r5, lbl_8047AB64(r13)
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB4C(r13)
    addi r3, r31, 0x3cc
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB48(r13)
    addi r3, r31, 0x3e8
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB38(r13)
    addi r3, r31, 0x404
    lwz r0, lbl_8047AB34(r13)
    subf r4, r4, r0
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r3, lbl_8047AB30(r13)
    li r4, 0x0
    b @L_800E1428
    @L_800E1414:
    lwz r0, 0x8(r3)
    cmplw r0, r4
    ble @L_800E1424
    mr r4, r0
    @L_800E1424:
    lwz r3, 0x4(r3)
    @L_800E1428:
    cmplwi r3, 0x0
    bne @L_800E1414
    addi r3, r31, 0x424
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r3, lbl_8047AB30(r13)
    li r4, 0x0
    b @L_800E1454
    @L_800E1448:
    lwz r0, 0x8(r3)
    lwz r3, 0x4(r3)
    add r4, r4, r0
    @L_800E1454:
    cmplwi r3, 0x0
    bne @L_800E1448
    addi r3, r31, 0x444
    crxor 6, 6, 6
    bl fn_800DD970
    mr r4, r27
    addi r3, r31, 0x464
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB60(r13)
    addi r3, r31, 0x480
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB5C(r13)
    addi r3, r31, 0x49c
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB58(r13)
    addi r3, r31, 0x4b8
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB54(r13)
    addi r3, r31, 0x4d4
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r4, lbl_8047AB50(r13)
    addi r3, r31, 0x4f0
    crxor 6, 6, 6
    bl fn_800DD970
    lis r4, 0x4330
    subi r3, r27, 0x1
    add r0, r27, r28
    stw r3, 0xc(r1)
    lfd f2, lbl_8047CB50(r2)
    addi r3, r31, 0x50c
    stw r4, 0x8(r1)
    lfs f3, lbl_8047CB48(r2)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    stw r4, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f2
    fdivs f0, f1, f0
    fmuls f1, f3, f0
    creqv 6, 6, 6
    bl fn_800DD38C
    addi r3, r31, 0x52c
    crxor 6, 6, 6
    bl fn_800DD970
    @L_800E151C:
    clrlwi. r0, r29, 24
    bne @L_800E152C
    li r0, 0x0
    stw r0, lbl_8047AB3C(r13)
    @L_800E152C:
    mr r3, r29
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
