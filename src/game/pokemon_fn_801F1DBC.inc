    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    stmw r24, 0x10(r1)
    mr r31, r4
    mr r24, r3
    beq @L_801F1DE8
    cmplwi r0, 0x3
    bne @L_801F1F18
    @L_801F1DE8:
    mr r3, r24
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r30, r3, 16
    mr r3, r24
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r24
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    li r27, 0x0
    b @L_801F1EA8
    @L_801F1E34:
    mr r3, r24
    mr r6, r27
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r25, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F1E5C
    li r25, 0x0
    @L_801F1E5C:
    cmplwi r25, 0x0
    beq @L_801F1EA4
    li r26, 0x0
    b @L_801F1E98
    @L_801F1E6C:
    mr r3, r25
    mr r4, r26
    bl fn_801F7258
    mr. r29, r3
    beq @L_801F1E94
    bl fn_801FB8F8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1E94
    b @L_801F1EB8
    @L_801F1E94:
    addi r26, r26, 0x1
    @L_801F1E98:
    clrlwi r0, r26, 16
    cmplw r0, r28
    blt @L_801F1E6C
    @L_801F1EA4:
    addi r27, r27, 0x1
    @L_801F1EA8:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F1E34
    li r29, 0x0
    @L_801F1EB8:
    cmplwi r29, 0x0
    beq @L_801F1EE0
    mr r3, r29
    mr r4, r30
    bl fn_801F0058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1EE0
    li r0, 0x1
    b @L_801F1EE4
    @L_801F1EE0:
    li r0, 0x0
    @L_801F1EE4:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F1F04
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    bne @L_801F1F18
    li r3, 0x1
    b @L_801F1F1C
    @L_801F1F04:
    clrlwi r0, r31, 16
    cmplwi r0, 0x3
    bne @L_801F1F18
    li r3, 0x1
    b @L_801F1F1C
    @L_801F1F18:
    li r3, 0x0
    @L_801F1F1C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
