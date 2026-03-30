    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r20, 0x20(r1)
    mr r31, r3
    mr r30, r4
    mr r29, r5
    mr r20, r6
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r27, r3, 16
    addi r3, r1, 0x8
    li r5, 0x0
    li r4, 0x0
    b @L_801F2BC8
    @L_801F2BBC:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
    @L_801F2BC8:
    clrlwi r0, r5, 16
    cmplwi r0, 0x4
    blt @L_801F2BBC
    clrlwi r0, r20, 24
    cmplwi r0, 0x1
    bne @L_801F2D88
    mr r25, r28
    li r22, 0x0
    li r24, 0x0
    b @L_801F2CF4
    @L_801F2BF0:
    mr r3, r31
    mr r6, r24
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r26, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F2C18
    li r26, 0x0
    @L_801F2C18:
    cmplwi r26, 0x0
    beq @L_801F2CF0
    li r23, 0x0
    b @L_801F2CE4
    @L_801F2C28:
    mr r3, r26
    mr r4, r23
    bl fn_801F7258
    mr. r21, r3
    beq @L_801F2CE0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r20, r3, 16
    mr r3, r21
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r20, 0x0
    bne @L_801F2CE0
    clrlwi r0, r22, 16
    cmplwi r0, 0x4
    bge @L_801F2CE0
    bl fn_800FF560
    mr r4, r3
    mr r8, r30
    li r3, 0x12
    li r5, 0x2000
    li r6, 0x1
    li r7, 0x0
    bl fn_800F07A8
    clrlslwi r4, r22, 16, 2
    addi r5, r1, 0x8
    addic. r0, r1, 0x8
    stwx r3, r5, r4
    beq @L_801F2CE0
    lwzx r3, r5, r4
    mr r5, r21
    mr r6, r25
    mr r7, r29
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800F0654
    addi r22, r22, 0x1
    @L_801F2CE0:
    addi r23, r23, 0x1
    @L_801F2CE4:
    clrlwi r0, r23, 16
    cmplw r0, r27
    blt @L_801F2C28
    @L_801F2CF0:
    addi r24, r24, 0x1
    @L_801F2CF4:
    clrlwi r0, r24, 16
    cmplwi r0, 0x2
    blt @L_801F2BF0
    addi r21, r1, 0x8
    @L_801F2D04:
    bl fn_800F0308
    li r22, 0x0
    b @L_801F2D34
    @L_801F2D10:
    clrlslwi r0, r22, 16, 2
    lwzx r3, r21, r0
    cmplwi r3, 0x0
    beq @L_801F2D30
    bl fn_800F04BC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_801F2D40
    @L_801F2D30:
    addi r22, r22, 0x1
    @L_801F2D34:
    clrlwi r0, r22, 16
    cmplwi r0, 0x4
    blt @L_801F2D10
    @L_801F2D40:
    clrlwi r0, r22, 16
    cmplwi r0, 0x4
    blt @L_801F2D04
    li r24, 0x0
    addi r23, r1, 0x8
    mr r21, r24
    b @L_801F2D78
    @L_801F2D5C:
    clrlslwi r22, r24, 16, 2
    lwzx r3, r23, r22
    cmplwi r3, 0x0
    beq @L_801F2D74
    bl fn_800F0494
    stwx r21, r23, r22
    @L_801F2D74:
    addi r24, r24, 0x1
    @L_801F2D78:
    clrlwi r0, r24, 16
    cmplwi r0, 0x4
    blt @L_801F2D5C
    b @L_801F2E58
    @L_801F2D88:
    li r22, 0x0
    b @L_801F2E4C
    @L_801F2D90:
    mr r3, r31
    mr r6, r22
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r21, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F2DB8
    li r21, 0x0
    @L_801F2DB8:
    cmplwi r21, 0x0
    beq @L_801F2E48
    li r23, 0x0
    b @L_801F2E3C
    @L_801F2DC8:
    mr r3, r21
    mr r4, r23
    bl fn_801F7258
    mr. r24, r3
    beq @L_801F2E38
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r25, r3, 16
    mr r3, r24
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r25, 0x0
    bne @L_801F2E38
    mr r12, r30
    mr r3, r24
    mr r4, r28
    mr r5, r29
    mtctr r12
    bctrl
    @L_801F2E38:
    addi r23, r23, 0x1
    @L_801F2E3C:
    clrlwi r0, r23, 16
    cmplw r0, r27
    blt @L_801F2DC8
    @L_801F2E48:
    addi r22, r22, 0x1
    @L_801F2E4C:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_801F2D90
    @L_801F2E58:
    li r22, 0x0
    b @L_801F2F1C
    @L_801F2E60:
    mr r3, r31
    mr r6, r22
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r21, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F2E88
    li r21, 0x0
    @L_801F2E88:
    cmplwi r21, 0x0
    beq @L_801F2F18
    li r23, 0x0
    b @L_801F2F0C
    @L_801F2E98:
    mr r3, r21
    mr r4, r23
    bl fn_801F7258
    mr. r24, r3
    beq @L_801F2F08
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r25, r3, 16
    mr r3, r24
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r25, 0x0
    beq @L_801F2F08
    mr r12, r30
    mr r3, r24
    mr r4, r28
    mr r5, r29
    mtctr r12
    bctrl
    @L_801F2F08:
    addi r23, r23, 0x1
    @L_801F2F0C:
    clrlwi r0, r23, 16
    cmplw r0, r27
    blt @L_801F2E98
    @L_801F2F18:
    addi r22, r22, 0x1
    @L_801F2F1C:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_801F2E60
    lmw r20, 0x20(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
