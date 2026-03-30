    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x84(r1)
    stmw r19, 0x4c(r1)
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
    b @L_8024A27C
@L_8024A1D8:
    clrlslwi r0, r28, 16, 2
    lwzx r19, r31, r0
    cmplwi r19, 0x0
    beq @L_8024A278
    addi r21, r1, 0x28
    clrlwi r22, r30, 16
    li r20, 0x0
    b @L_8024A26C
@L_8024A1F8:
    clrlslwi r0, r20, 16, 2
    lwzx r5, r21, r0
    cmplwi r5, 0x0
    beq @L_8024A268
    mr r3, r24
    mr r4, r19
    bl fn_80236D60
    cmpwi r3, 0x0
    ble @L_8024A268
    mr r3, r29
    mr r4, r24
    li r5, 0xcf
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
    li r10, 0xcf
    bl fn_80239EE8
    b @L_8024A278
@L_8024A268:
    addi r20, r20, 0x1
@L_8024A26C:
    clrlwi r0, r20, 16
    cmplw r0, r22
    blt @L_8024A1F8
@L_8024A278:
    addi r28, r28, 0x1
@L_8024A27C:
    clrlwi r0, r28, 16
    cmplw r0, r23
    blt @L_8024A1D8
    cmplwi r23, 0x2
    blt @L_8024A2D8
    mr r3, r29
    mr r4, r24
    li r5, 0xd0
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
    li r10, 0xd0
    bl fn_80239EE8
@L_8024A2D8:
    addi r31, r1, 0x28
    clrlwi r28, r30, 16
    li r21, 0x0
    b @L_8024A310
@L_8024A2E8:
    clrlslwi r0, r21, 16, 2
    lwzx r5, r31, r0
    cmplwi r5, 0x0
    beq @L_8024A30C
    mr r3, r24
    mr r4, r27
    bl fn_80236D60
    cmpwi r3, 0x0
    blt @L_8024A31C
@L_8024A30C:
    addi r21, r21, 0x1
@L_8024A310:
    clrlwi r0, r21, 16
    cmplw r0, r28
    blt @L_8024A2E8
@L_8024A31C:
    clrlwi r3, r21, 16
    clrlwi r0, r30, 16
    cmplw r3, r0
    bge @L_8024A374
    mr r3, r29
    mr r4, r24
    li r5, 0xd1
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
    li r10, 0xd1
    bl fn_80239EE8
@L_8024A374:
    mr r3, r24
    mr r4, r27
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024A3C8
    mr r3, r24
    mr r4, r27
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024A3C8
    mr r3, r24
    mr r4, r27
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024A410
@L_8024A3C8:
    mr r3, r29
    mr r4, r24
    li r5, 0xd2
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
    li r10, 0xd2
    bl fn_80239EE8
@L_8024A410:
    mr r3, r29
    lmw r19, 0x4c(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
