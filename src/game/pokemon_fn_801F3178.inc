    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x34(r1)
    li r6, 0x0
    stmw r22, 0x8(r1)
    mr r27, r3
    bl fn_801F54A4
    clrlwi r30, r3, 16
    mr r3, r27
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r29, r3, 16
    mr r3, r27
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    li r31, 0x0
    b @L_801F3290
    @L_801F31D8:
    mr r3, r27
    mr r6, r31
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r26, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F3200
    li r26, 0x0
    @L_801F3200:
    cmplwi r26, 0x0
    beq @L_801F328C
    li r25, 0x0
    b @L_801F3280
    @L_801F3210:
    mr r3, r26
    mr r4, r25
    bl fn_801F7258
    mr. r23, r3
    beq @L_801F327C
    li r24, 0x0
    b @L_801F3270
    @L_801F322C:
    mr r3, r23
    mr r4, r24
    bl fn_801F981C
    mr. r22, r3
    beq @L_801F326C
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F326C
    mr r4, r22
    mr r5, r30
    li r3, 0x3
    bl fn_801F02AC
    mr r0, r3
    mr r3, r22
    mr r4, r0
    bl fn_80205274
    @L_801F326C:
    addi r24, r24, 0x1
    @L_801F3270:
    clrlwi r0, r24, 16
    cmplw r0, r28
    blt @L_801F322C
    @L_801F327C:
    addi r25, r25, 0x1
    @L_801F3280:
    clrlwi r0, r25, 16
    cmplw r0, r29
    blt @L_801F3210
    @L_801F328C:
    addi r31, r31, 0x1
    @L_801F3290:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801F31D8
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
