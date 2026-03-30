    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x24(r1)
    li r6, 0x0
    stmw r26, 0x8(r1)
    mr r26, r3
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    li r29, 0x0
    b @L_801F2B38
    @L_801F2AC0:
    mr r3, r26
    mr r6, r29
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r31, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F2AE8
    li r31, 0x0
    @L_801F2AE8:
    cmplwi r31, 0x0
    beq @L_801F2B34
    li r30, 0x0
    b @L_801F2B28
    @L_801F2AF8:
    mr r3, r31
    mr r4, r30
    bl fn_801F7258
    mr. r27, r3
    beq @L_801F2B24
    bl fn_801FB8F8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F2B24
    mr r3, r27
    b @L_801F2B48
    @L_801F2B24:
    addi r30, r30, 0x1
    @L_801F2B28:
    clrlwi r0, r30, 16
    cmplw r0, r28
    blt @L_801F2AF8
    @L_801F2B34:
    addi r29, r29, 0x1
    @L_801F2B38:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F2AC0
    li r3, 0x0
    @L_801F2B48:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
