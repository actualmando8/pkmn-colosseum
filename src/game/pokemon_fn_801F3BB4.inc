    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    clrlwi r29, r5, 16
    mr r23, r3
    mr r24, r4
    mr r25, r6
    subi r28, r29, 0x1
    li r27, 0x0
    b @L_801F3CC8
    @L_801F3BE0:
    addi r0, r3, 0x1
    clrlslwi r30, r27, 16, 2
    clrlwi r26, r0, 16
    b @L_801F3CB8
    @L_801F3BF0:
    lwzx r3, r24, r30
    cmplwi r3, 0x0
    bne @L_801F3C0C
    clrlslwi r0, r26, 16, 2
    lwzx r0, r24, r0
    cmplwi r0, 0x0
    beq @L_801F3CB4
    @L_801F3C0C:
    cmplwi r3, 0x0
    bne @L_801F3C28
    clrlslwi r4, r26, 16, 2
    lwzx r0, r24, r4
    stwx r0, r24, r30
    stwx r3, r24, r4
    b @L_801F3CB4
    @L_801F3C28:
    clrlslwi r31, r26, 16, 2
    lwzx r0, r24, r31
    cmplwi r0, 0x0
    beq @L_801F3CB4
    clrlwi. r0, r25, 24
    bne @L_801F3C4C
    li r22, 0x0
    li r3, 0x0
    b @L_801F3C60
    @L_801F3C4C:
    bl fn_802050F4
    mr r0, r3
    lwzx r3, r24, r31
    mr r22, r0
    bl fn_802050F4
    @L_801F3C60:
    extsb r4, r22
    extsb r0, r3
    cmpw r4, r0
    blt @L_801F3CB4
    ble @L_801F3C88
    lwzx r3, r24, r30
    lwzx r0, r24, r31
    stwx r0, r24, r30
    stwx r3, r24, r31
    b @L_801F3CB4
    @L_801F3C88:
    lwzx r4, r24, r30
    mr r3, r23
    lwzx r5, r24, r31
    mr r6, r25
    bl fn_801F3CE8
    clrlwi. r0, r3, 24
    bne @L_801F3CB4
    lwzx r3, r24, r30
    lwzx r0, r24, r31
    stwx r0, r24, r30
    stwx r3, r24, r31
    @L_801F3CB4:
    addi r26, r26, 0x1
    @L_801F3CB8:
    clrlwi r0, r26, 16
    cmplw r0, r29
    blt @L_801F3BF0
    addi r27, r27, 0x1
    @L_801F3CC8:
    clrlwi r3, r27, 16
    cmpw r3, r28
    blt @L_801F3BE0
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
