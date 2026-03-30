    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr r28, r5
    mr r31, r6
    mr r26, r3
    mr r27, r4
    addi r5, r1, 0x8
    li r6, 0x0
    bl fn_802367CC
    mr r30, r3
    mr r3, r26
    mr r4, r31
    bl fn_802364BC
    mr r0, r3
    mr r3, r26
    mr r29, r0
    mr r4, r31
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025882C
    li r3, 0x0
    b @L_802588F0
@L_8025882C:
    lis r7, 0x1
    mr r3, r26
    mr r4, r27
    mr r5, r28
    mr r6, r31
    subi r7, r7, 0x1
    bl fn_8025C264
    mr r31, r3
    mr r3, r26
    mr r4, r27
    li r5, 0x10
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80258898
    mr r3, r29
    bl fn_8021A2C0
    clrlwi. r0, r3, 24
    bne @L_80258898
    clrlwi. r5, r29, 16
    beq @L_80258898
    cmplwi r5, 0xffff
    beq @L_80258898
    cmplwi r5, 0x165
    beq @L_80258898
    cmplwi r5, 0x163
    bne @L_802588A0
@L_80258898:
    li r3, 0x0
    b @L_802588F0
@L_802588A0:
    addi r4, r1, 0x8
    clrlwi r0, r30, 16
    li r6, 0x0
    b @L_802588CC
@L_802588B0:
    clrlslwi r3, r6, 16, 1
    lhzx r3, r4, r3
    cmplw r5, r3
    bne @L_802588C8
    li r3, 0x0
    b @L_802588F0
@L_802588C8:
    addi r6, r6, 0x1
@L_802588CC:
    clrlwi r3, r6, 16
    cmplw r3, r0
    blt @L_802588B0
    cmpwi r31, 0x0
    bne @L_802588E8
    li r3, 0x0
    b @L_802588F0
@L_802588E8:
    cmpwi r31, -0x1
    li r3, 0x1
@L_802588F0:
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
