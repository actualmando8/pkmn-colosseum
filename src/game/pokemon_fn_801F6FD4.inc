    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    li r29, 0x0
    mr r24, r3
    mr r25, r5
    clrlwi r31, r4, 16
    mr r27, r29
    b @L_801F706C
    @L_801F6FFC:
    cmplwi r24, 0x0
    bne @L_801F700C
    li r3, 0x0
    b @L_801F7018
    @L_801F700C:
    mr r3, r24
    mr r4, r27
    bl fn_801F78D4
    @L_801F7018:
    mr r26, r3
    bl fn_801FA634
    clrlwi. r0, r3, 24
    beq @L_801F7068
    clrlwi r30, r25, 16
    li r28, 0x0
    b @L_801F705C
    @L_801F7034:
    mr r3, r26
    mr r4, r28
    bl fn_801F986C
    cmplwi r3, 0x0
    beq @L_801F7058
    bl fn_80206608
    clrlwi. r0, r3, 24
    beq @L_801F7058
    addi r29, r29, 0x1
    @L_801F7058:
    addi r28, r28, 0x1
    @L_801F705C:
    clrlwi r0, r28, 16
    cmplw r0, r30
    blt @L_801F7034
    @L_801F7068:
    addi r27, r27, 0x1
    @L_801F706C:
    clrlwi r0, r27, 16
    cmplw r0, r31
    blt @L_801F6FFC
    clrlwi r3, r29, 16
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
