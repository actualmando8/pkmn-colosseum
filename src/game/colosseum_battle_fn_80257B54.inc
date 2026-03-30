    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r6, 0x1
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r25, 0x74(r1)
    mr r25, r3
    mr r26, r5
    addi r5, r1, 0x8
    mr r4, r25
    li r3, 0x0
    bl fn_801F1A6C
    addi r28, r1, 0x8
    clrlwi r31, r3, 16
    clrlwi r30, r26, 16
    li r26, 0x0
    li r27, 0x0
    b @L_80257BEC
@L_80257B9C:
    clrlslwi r29, r27, 16, 2
    lwzx r4, r28, r29
    cmplwi r4, 0x0
    beq @L_80257BE8
    cmplwi r30, 0xd7
    bne @L_80257BCC
    mr r3, r25
    li r5, 0x2b
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257BE8
@L_80257BCC:
    lwzx r4, r28, r29
    mr r3, r25
    bl fn_80238748
    clrlwi. r0, r3, 24
    bne @L_80257BE8
    li r26, 0x1
    b @L_80257BF8
@L_80257BE8:
    addi r27, r27, 0x1
@L_80257BEC:
    clrlwi r0, r27, 16
    cmplw r0, r31
    blt @L_80257B9C
@L_80257BF8:
    clrlwi. r0, r26, 24
    bne @L_80257C08
    li r3, 0x0
    b @L_80257C0C
@L_80257C08:
    li r3, 0x1
@L_80257C0C:
    lmw r25, 0x74(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
