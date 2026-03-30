    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    li r0, 0x0
    stmw r20, 0x10(r1)
    mr r22, r3
    lis r3, lbl_80401168@ha
    mr r23, r4
    mr r24, r6
    mr r31, r5
    mr r30, r22
    addi r21, r3, lbl_80401168@l
    li r28, 0x0
    li r27, 0x0
    li r26, 0x0
    li r25, 0x0
    b @L_800DE65C
    @L_800DE16C:
    clrlwi. r3, r27, 24
    bne @L_800DE1C0
    lbz r4, 0x0(r31)
    cmpwi r4, 0x25
    bne @L_800DE1B4
    lbz r3, 0x1(r31)
    cmpwi r3, 0x25
    bne @L_800DE1A0
    li r3, 0x25
    addi r31, r31, 0x1
    stb r3, 0x0(r30)
    addi r30, r30, 0x1
    b @L_800DE63C
    @L_800DE1A0:
    lis r3, lbl_80401178@ha
    li r27, 0x1
    addi r3, r3, lbl_80401178@l
    mr r29, r3
    b @L_800DE63C
    @L_800DE1B4:
    stb r4, 0x0(r30)
    addi r30, r30, 0x1
    b @L_800DE63C
    @L_800DE1C0:
    lbz r5, 0x0(r31)
    extsb r3, r5
    subi r4, r3, 0x58
    cmplwi r4, 0x20
    bgt @L_800DE4D8
    lis r3, jumptable_80315388@ha
    slwi r4, r4, 2
    addi r3, r3, jumptable_80315388@l
    lwzx r3, r3, r4
    mtctr r3
    bctr
    lis r4, lbl_80401168@ha
    mr r3, r24
    addi r0, r4, lbl_80401168@l
    li r4, 0x1
    mr r20, r0
    bl __va_arg
    lwz r0, 0x0(r3)
    lis r3, lbl_80401168@ha
    addi r4, r3, lbl_80401168@l
    li r3, 0x0
    stb r0, 0x0(r4)
    li r0, 0x1
    stb r3, 0x1(r21)
    b @L_800DE4E0
    mr r3, r24
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    lis r3, lbl_80401168@ha
    addi r3, r3, lbl_80401168@l
    li r8, 0x0
    cmpwi r0, 0x0
    mr r9, r0
    bge @L_800DE254
    li r8, 0x1
    neg r9, r0
    @L_800DE254:
    lis r4, 0x6666
    lwz r7, lbl_80478AE8(r13)
    addi r6, r4, 0x6667
    @L_800DE260:
    mulhw r0, r6, r9
    srawi r4, r0, 2
    srwi r5, r4, 31
    srawi r0, r0, 2
    add r4, r4, r5
    mulli r5, r4, 0xa
    srwi r4, r0, 31
    subf r5, r5, r9
    add. r9, r0, r4
    lbzx r0, r7, r5
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
    bne @L_800DE260
    clrlwi r0, r8, 24
    cmplwi r0, 0x1
    bne @L_800DE2AC
    li r0, 0x2d
    stb r0, 0x0(r3)
    addi r3, r3, 0x1
    @L_800DE2AC:
    li r0, 0x0
    lis r4, lbl_80401168@ha
    addi r20, r4, lbl_80401168@l
    stb r0, 0x0(r3)
    mr r3, r20
    bl strlen
    lis r4, lbl_80401168@ha
    addi r0, r4, lbl_80401168@l
    add r4, r3, r0
    subi r4, r4, 0x1
    b @L_800DE310
    @L_800DE2D8:
    lbz r3, 0x0(r20)
    lbz r0, 0x0(r4)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    lbz r3, 0x0(r4)
    lbz r0, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r4)
    lbz r0, 0x0(r4)
    subi r4, r4, 0x1
    lbz r3, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    addi r20, r20, 0x1
    @L_800DE310:
    cmplw r20, r4
    blt @L_800DE2D8
    lis r3, lbl_80401168@ha
    li r0, 0x1
    addi r3, r3, lbl_80401168@l
    mr r20, r3
    b @L_800DE4E0
    mr r3, r24
    li r4, 0x3
    bl __va_arg
    la r20, lbl_8047CAA0(r2)
    li r0, 0x1
    b @L_800DE4E0
    mr r3, r24
    li r4, 0x1
    bl __va_arg
    lwz r20, 0x0(r3)
    cmplwi r20, 0x0
    bne @L_800DE360
    la r20, lbl_8047CAA8(r2)
    @L_800DE360:
    li r0, 0x1
    b @L_800DE4E0
    mr r3, r24
    li r4, 0x1
    bl __va_arg
    lbz r0, 0x0(r31)
    lwz r6, 0x0(r3)
    cmpwi r0, 0x58
    bne @L_800DE41C
    lis r4, lbl_80401168@ha
    lwz r3, lbl_80478AE8(r13)
    addi r4, r4, lbl_80401168@l
    mr r5, r6
    @L_800DE394:
    clrlwi r0, r5, 28
    srwi. r5, r5, 4
    lbzx r0, r3, r0
    stb r0, 0x0(r4)
    addi r4, r4, 0x1
    bne @L_800DE394
    li r0, 0x0
    lis r3, lbl_80401168@ha
    addi r20, r3, lbl_80401168@l
    stb r0, 0x0(r4)
    mr r3, r20
    bl strlen
    lis r4, lbl_80401168@ha
    addi r0, r4, lbl_80401168@l
    add r4, r3, r0
    subi r4, r4, 0x1
    b @L_800DE410
    @L_800DE3D8:
    lbz r3, 0x0(r20)
    lbz r0, 0x0(r4)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    lbz r3, 0x0(r4)
    lbz r0, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r4)
    lbz r0, 0x0(r4)
    subi r4, r4, 0x1
    lbz r3, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    addi r20, r20, 0x1
    @L_800DE410:
    cmplw r20, r4
    blt @L_800DE3D8
    b @L_800DE4C4
    @L_800DE41C:
    lis r3, lbl_80401168@ha
    lwz r4, lbl_80478AE8(r13)
    addi r5, r3, lbl_80401168@l
    @L_800DE428:
    clrlwi r0, r6, 28
    lbzx r0, r4, r0
    stb r0, 0x0(r5)
    lbz r3, 0x0(r5)
    extsb r0, r3
    cmpwi r0, 0x41
    blt @L_800DE44C
    addi r0, r3, 0x20
    stb r0, 0x0(r5)
    @L_800DE44C:
    srwi. r6, r6, 4
    addi r5, r5, 0x1
    bne @L_800DE428
    li r0, 0x0
    lis r3, lbl_80401168@ha
    addi r20, r3, lbl_80401168@l
    stb r0, 0x0(r5)
    mr r3, r20
    bl strlen
    lis r4, lbl_80401168@ha
    addi r0, r4, lbl_80401168@l
    add r4, r3, r0
    subi r4, r4, 0x1
    b @L_800DE4BC
    @L_800DE484:
    lbz r3, 0x0(r20)
    lbz r0, 0x0(r4)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    lbz r3, 0x0(r4)
    lbz r0, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r4)
    lbz r0, 0x0(r4)
    subi r4, r4, 0x1
    lbz r3, 0x0(r20)
    xor r0, r3, r0
    stb r0, 0x0(r20)
    addi r20, r20, 0x1
    @L_800DE4BC:
    cmplw r20, r4
    blt @L_800DE484
    @L_800DE4C4:
    lis r3, lbl_80401168@ha
    li r0, 0x1
    addi r3, r3, lbl_80401168@l
    mr r20, r3
    b @L_800DE4E0
    @L_800DE4D8:
    stb r5, 0x0(r29)
    addi r29, r29, 0x1
    @L_800DE4E0:
    clrlwi r3, r0, 24
    cmplwi r3, 0x1
    bne @L_800DE63C
    li r0, 0x0
    lis r3, lbl_80401178@ha
    stb r0, 0x0(r29)
    addi r3, r3, lbl_80401178@l
    mr r29, r3
    lbz r0, 0x0(r3)
    cmpwi r0, 0x2d
    bne @L_800DE514
    li r26, 0x1
    addi r29, r3, 0x1
    @L_800DE514:
    lbz r0, 0x0(r29)
    cmpwi r0, 0x30
    bne @L_800DE528
    li r25, 0x1
    addi r29, r29, 0x1
    @L_800DE528:
    mr r3, r29
    li r27, 0x0
    b @L_800DE560
    @L_800DE534:
    extsb r0, r4
    cmpwi r0, 0x30
    blt @L_800DE56C
    cmpwi r0, 0x39
    bgt @L_800DE56C
    lbz r0, 0x0(r3)
    mulli r27, r27, 0xa
    addi r3, r3, 0x1
    extsb r0, r0
    add r27, r0, r27
    subi r27, r27, 0x30
    @L_800DE560:
    lbz r4, 0x0(r3)
    extsb. r0, r4
    bne @L_800DE534
    @L_800DE56C:
    mr r3, r20
    bl strlen
    cmpw r27, r3
    ble @L_800DE5E0
    clrlwi. r0, r26, 24
    subf r27, r3, r27
    bne @L_800DE5E0
    clrlwi r5, r25, 24
    li r4, 0x30
    li r3, 0x20
    b @L_800DE5B4
    @L_800DE598:
    cmplwi r5, 0x0
    beq @L_800DE5AC
    stb r4, 0x0(r30)
    addi r30, r30, 0x1
    b @L_800DE5B4
    @L_800DE5AC:
    stb r3, 0x0(r30)
    addi r30, r30, 0x1
    @L_800DE5B4:
    cmpwi r27, 0x0
    subi r27, r27, 0x1
    beq @L_800DE5E0
    subf r0, r22, r30
    cmplw r0, r23
    blt @L_800DE598
    b @L_800DE5E0
    @L_800DE5D0:
    lbz r0, 0x0(r20)
    addi r20, r20, 0x1
    stb r0, 0x0(r30)
    addi r30, r30, 0x1
    @L_800DE5E0:
    lbz r0, 0x0(r20)
    extsb. r0, r0
    beq @L_800DE5F8
    subf r0, r22, r30
    cmplw r0, r23
    blt @L_800DE5D0
    @L_800DE5F8:
    clrlwi r0, r26, 24
    cmplwi r0, 0x1
    bne @L_800DE62C
    li r3, 0x20
    b @L_800DE614
    @L_800DE60C:
    stb r3, 0x0(r30)
    addi r30, r30, 0x1
    @L_800DE614:
    cmpwi r27, 0x0
    subi r27, r27, 0x1
    beq @L_800DE62C
    subf r0, r22, r30
    cmplw r0, r23
    blt @L_800DE60C
    @L_800DE62C:
    li r26, 0x0
    li r25, 0x0
    li r27, 0x0
    li r0, 0x0
    @L_800DE63C:
    lbz r3, 0x0(r31)
    extsb. r3, r3
    beq @L_800DE654
    subf r3, r22, r30
    cmplw r3, r23
    blt @L_800DE658
    @L_800DE654:
    li r28, 0x1
    @L_800DE658:
    addi r31, r31, 0x1
    @L_800DE65C:
    clrlwi. r3, r28, 24
    beq @L_800DE16C
    li r0, 0x0
    stb r0, 0x0(r30)
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
