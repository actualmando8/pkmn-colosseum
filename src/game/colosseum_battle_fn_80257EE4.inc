    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r6, 0x0
    li r7, 0x1
    stw r0, 0x44(r1)
    addi r5, r1, 0x8
    stmw r27, 0x2c(r1)
    mr r28, r3
    mr r27, r4
    bl fn_802367CC
    mr r29, r3
    mr r3, r28
    mr r4, r27
    li r5, 0x8
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80257F30
    li r3, 0x0
    b @L_80257FB4
@L_80257F30:
    addi r30, r1, 0x8
    clrlwi r31, r29, 16
    li r28, 0x0
    b @L_80257F8C
@L_80257F40:
    clrlslwi r0, r28, 16, 1
    lhzx r27, r30, r0
    cmplwi r27, 0x165
    beq @L_80257F88
    cmplwi r27, 0x163
    beq @L_80257F88
    mr r3, r27
    bl fn_80218FDC
    clrlwi. r0, r3, 24
    bne @L_80257F88
    cmplwi r27, 0x108
    beq @L_80257F88
    cmplwi r27, 0xfd
    beq @L_80257F88
    mr r3, r27
    bl fn_8021901C
    clrlwi. r0, r3, 24
    beq @L_80257F98
@L_80257F88:
    addi r28, r28, 0x1
@L_80257F8C:
    clrlwi r0, r28, 16
    cmplw r0, r31
    blt @L_80257F40
@L_80257F98:
    clrlwi r3, r28, 16
    clrlwi r0, r29, 16
    cmplw r3, r0
    blt @L_80257FB0
    li r3, 0x0
    b @L_80257FB4
@L_80257FB0:
    li r3, 0x1
@L_80257FB4:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
