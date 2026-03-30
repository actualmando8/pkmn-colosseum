    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r8, 0x0
    stw r0, 0x54(r1)
    stmw r17, 0x14(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    mr r3, r8
    b @L_801F6228
    @L_801F621C:
    clrlslwi r0, r8, 16, 2
    addi r8, r8, 0x1
    stwx r3, r28, r0
    @L_801F6228:
    clrlwi r0, r8, 16
    cmplwi r0, 0x8
    blt @L_801F621C
    mr r3, r27
    li r18, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r17, r3, 16
    mr r3, r27
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r20, r3, 16
    mr r3, r27
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi. r0, r30, 24
    clrlwi r19, r3, 16
    beq @L_801F62B8
    mr r3, r31
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F62A0
    li r3, 0x0
    b @L_801F63F8
    @L_801F62A0:
    mr r4, r31
    mr r5, r17
    li r3, 0x2
    bl fn_801F02AC
    mr r24, r3
    b @L_801F62BC
    @L_801F62B8:
    li r24, 0x0
    @L_801F62BC:
    li r21, 0x0
    b @L_801F63E8
    @L_801F62C4:
    mr r3, r27
    mr r6, r21
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r25, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F62EC
    li r25, 0x0
    @L_801F62EC:
    cmplwi r25, 0x0
    beq @L_801F63E4
    clrlwi r0, r30, 24
    cmplwi r0, 0x1
    bne @L_801F630C
    cmplw r24, r25
    bne @L_801F63E4
    b @L_801F631C
    @L_801F630C:
    cmplwi r0, 0x2
    bne @L_801F631C
    cmplw r24, r25
    beq @L_801F63E4
    @L_801F631C:
    li r23, 0x0
    b @L_801F63D8
    @L_801F6324:
    mr r3, r25
    mr r4, r23
    bl fn_801F7258
    mr. r17, r3
    beq @L_801F63D4
    li r22, 0x0
    b @L_801F63C8
    @L_801F6340:
    mr r3, r17
    mr r4, r22
    bl fn_801F981C
    mr. r26, r3
    beq @L_801F63C4
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    bne @L_801F636C
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F63C4
    @L_801F636C:
    clrlwi r0, r30, 24
    cmplwi r0, 0x3
    bne @L_801F6380
    cmplw r26, r31
    beq @L_801F63C4
    @L_801F6380:
    li r4, 0x0
    b @L_801F63A8
    @L_801F6388:
    clrlslwi r3, r4, 16, 2
    lwzx r0, r28, r3
    cmplwi r0, 0x0
    bne @L_801F63A4
    stwx r26, r28, r3
    extsh r0, r4
    b @L_801F63B8
    @L_801F63A4:
    addi r4, r4, 0x1
    @L_801F63A8:
    clrlwi r0, r4, 16
    cmplwi r0, 0x8
    blt @L_801F6388
    li r0, -0x1
    @L_801F63B8:
    extsh. r0, r0
    blt @L_801F63C4
    addi r18, r18, 0x1
    @L_801F63C4:
    addi r22, r22, 0x1
    @L_801F63C8:
    clrlwi r0, r22, 16
    cmplw r0, r19
    blt @L_801F6340
    @L_801F63D4:
    addi r23, r23, 0x1
    @L_801F63D8:
    clrlwi r0, r23, 16
    cmplw r0, r20
    blt @L_801F6324
    @L_801F63E4:
    addi r21, r21, 0x1
    @L_801F63E8:
    clrlwi r0, r21, 16
    cmplwi r0, 0x2
    blt @L_801F62C4
    mr r3, r18
    @L_801F63F8:
    lmw r17, 0x14(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
