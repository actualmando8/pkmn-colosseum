    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    beq @L_801F16EC
    mr r3, r31
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r25, r3, 16
    mr r3, r31
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r24, r3, 16
    li r3, 0x4
    li r4, 0x0
    bl fn_801F025C
    mr r4, r25
    mr r26, r3
    mr r5, r24
    bl fn_801F6FD4
    mr r28, r3
    mr r3, r26
    mr r4, r25
    mr r5, r24
    bl fn_801F7174
    mr r29, r3
    mr r3, r26
    mr r4, r25
    mr r5, r24
    bl fn_801F7090
    mulli r0, r29, 0x64
    li r4, 0x0
    divwu r26, r0, r3
    li r3, 0x5
    bl fn_801F025C
    mr r4, r25
    mr r27, r3
    mr r5, r24
    bl fn_801F6FD4
    mr r29, r3
    mr r3, r27
    mr r4, r25
    mr r5, r24
    bl fn_801F7174
    mr r30, r3
    mr r3, r27
    mr r4, r25
    mr r5, r24
    bl fn_801F7090
    mulli r0, r30, 0x64
    li r4, 0x0
    divwu r27, r0, r3
    mr r3, r31
    bl fn_801F3984
    clrlwi r3, r28, 16
    clrlwi r0, r29, 16
    cmplw r3, r0
    ble @L_801F169C
    mr r3, r31
    li r4, 0x2
    bl fn_801F3984
    @L_801F169C:
    clrlwi r3, r28, 16
    clrlwi r0, r29, 16
    cmplw r3, r0
    bge @L_801F16B8
    mr r3, r31
    li r4, 0x3
    bl fn_801F3984
    @L_801F16B8:
    bl fn_801EF634
    clrlwi. r0, r3, 16
    bne @L_801F16EC
    cmplw r26, r27
    blt @L_801F16D8
    mr r3, r31
    li r4, 0x2
    bl fn_801F3984
    @L_801F16D8:
    cmplw r26, r27
    bgt @L_801F16EC
    mr r3, r31
    li r4, 0x3
    bl fn_801F3984
    @L_801F16EC:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
