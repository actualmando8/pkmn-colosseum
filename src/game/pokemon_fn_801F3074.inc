    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x34(r1)
    li r6, 0x0
    stmw r24, 0x10(r1)
    mr r24, r3
    bl fn_801F54A4
    clrlwi r28, r3, 16
    mr r3, r24
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r27, r3, 16
    mr r3, r24
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r26, r3, 16
    li r29, 0x0
    b @L_801F3158
    @L_801F30D4:
    mr r3, r24
    mr r6, r29
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r31, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F30FC
    li r31, 0x0
    @L_801F30FC:
    cmplwi r31, 0x0
    beq @L_801F3154
    li r30, 0x0
    b @L_801F3148
    @L_801F310C:
    mr r3, r31
    mr r4, r30
    bl fn_801F7258
    mr. r25, r3
    beq @L_801F3144
    mr r4, r25
    mr r5, r28
    li r3, 0x3
    bl fn_801F02AC
    mr r4, r25
    mr r5, r28
    mr r6, r27
    mr r7, r26
    bl fn_801F6B54
    @L_801F3144:
    addi r30, r30, 0x1
    @L_801F3148:
    clrlwi r0, r30, 16
    cmplw r0, r27
    blt @L_801F310C
    @L_801F3154:
    addi r29, r29, 0x1
    @L_801F3158:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F30D4
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
