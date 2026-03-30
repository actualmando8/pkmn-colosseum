    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x84(r1)
    stmw r18, 0x48(r1)
    mr r24, r3
    mr r25, r4
    mr r26, r5
    mr r27, r6
    mr r4, r24
    addi r5, r1, 0x28
    li r29, 0x0
    li r3, 0x0
    li r6, 0x1
    bl fn_801F1C18
    mr r30, r3
    mr r4, r24
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    addi r31, r1, 0x8
    clrlwi r23, r3, 16
    li r28, 0x0
    b @L_8024A548
@L_8024A490:
    clrlslwi r0, r28, 16, 2
    lwzx r19, r31, r0
    cmplwi r19, 0x0
    beq @L_8024A544
    addi r21, r1, 0x28
    clrlwi r22, r30, 16
    li r18, 0x0
    li r20, 0x0
    b @L_8024A52C
@L_8024A4B4:
    clrlslwi r0, r20, 16, 2
    lwzx r5, r21, r0
    cmplwi r5, 0x0
    beq @L_8024A528
    mr r3, r24
    mr r4, r19
    bl fn_80236D60
    cmpwi r3, 0x0
    ble @L_8024A528
    mr r3, r29
    mr r4, r24
    li r5, 0xcc
    bl fn_80239984
    mr r0, r3
    mr r3, r25
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r24
    mr r8, r26
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xcc
    bl fn_80239EE8
    li r18, 0x1
    b @L_8024A538
@L_8024A528:
    addi r20, r20, 0x1
@L_8024A52C:
    clrlwi r0, r20, 16
    cmplw r0, r22
    blt @L_8024A4B4
@L_8024A538:
    clrlwi r0, r18, 24
    cmplwi r0, 0x1
    beq @L_8024A554
@L_8024A544:
    addi r28, r28, 0x1
@L_8024A548:
    clrlwi r0, r28, 16
    cmplw r0, r23
    blt @L_8024A490
@L_8024A554:
    mr r3, r24
    mr r4, r27
    bl fn_80235910
    clrlwi. r0, r3, 24
    bne @L_8024A5B0
    mr r3, r29
    mr r4, r24
    li r5, 0xcd
    bl fn_80239984
    mr r0, r3
    mr r3, r25
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r24
    mr r8, r26
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xcd
    bl fn_80239EE8
@L_8024A5B0:
    mr r3, r24
    mr r4, r27
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024A604
    mr r3, r24
    mr r4, r27
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024A604
    mr r3, r24
    mr r4, r27
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024A64C
@L_8024A604:
    mr r3, r29
    mr r4, r24
    li r5, 0xce
    bl fn_80239984
    mr r0, r3
    mr r3, r25
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r24
    mr r8, r26
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xce
    bl fn_80239EE8
@L_8024A64C:
    mr r3, r29
    lmw r18, 0x48(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
