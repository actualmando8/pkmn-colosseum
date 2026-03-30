    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r23, r3
    mr r24, r4
    mr r25, r7
    clrlwi r31, r6, 16
    li r27, 0x0
    b @L_801F6C2C
    @L_801F6B7C:
    cmplwi r23, 0x0
    bne @L_801F6B8C
    li r3, 0x0
    b @L_801F6B98
    @L_801F6B8C:
    mr r3, r23
    mr r4, r27
    bl fn_801F78D4
    @L_801F6B98:
    mr r29, r3
    bl fn_801FA634
    clrlwi. r0, r3, 24
    beq @L_801F6C28
    clrlwi r30, r25, 16
    li r26, 0x0
    b @L_801F6C1C
    @L_801F6BB4:
    mr r3, r29
    mr r6, r26
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r28, r3
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F6C18
    mr r3, r28
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh. r7, r3
    blt @L_801F6C18
    mr r3, r24
    li r4, 0x0
    li r5, 0x57
    li r6, 0x0
    bl fn_801FAA58
    @L_801F6C18:
    addi r26, r26, 0x1
    @L_801F6C1C:
    clrlwi r0, r26, 16
    cmplw r0, r30
    blt @L_801F6BB4
    @L_801F6C28:
    addi r27, r27, 0x1
    @L_801F6C2C:
    clrlwi r0, r27, 16
    cmplw r0, r31
    blt @L_801F6B7C
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
