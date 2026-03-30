    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r22, 0x28(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    li r30, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r27
    bl fn_801F0134
    mr r31, r3
    mr r4, r26
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    mr r23, r3
    addi r24, r1, 0x8
    clrlwi r25, r3, 16
    li r22, 0x0
    b @L_802484FC
@L_802484D4:
    clrlslwi r0, r22, 16, 2
    lwzx r5, r24, r0
    cmplwi r5, 0x0
    beq @L_802484F8
    mr r3, r26
    mr r4, r29
    bl fn_80236D60
    cmpwi r3, 0x0
    bgt @L_80248508
@L_802484F8:
    addi r22, r22, 0x1
@L_802484FC:
    clrlwi r0, r22, 16
    cmplw r0, r25
    blt @L_802484D4
@L_80248508:
    clrlwi r3, r22, 16
    clrlwi r0, r23, 16
    cmplw r3, r0
    bge @L_80248560
    mr r4, r26
    li r3, 0x0
    li r5, 0x107
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x107
    bl fn_80239EE8
@L_80248560:
    mr r3, r26
    mr r4, r29
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802485E0
    mr r3, r29
    li r4, 0x1d
    bl fn_80201D84
    clrlwi r3, r3, 16
    clrlwi r0, r31, 16
    cmplw r0, r3
    bne @L_802485E0
    mr r3, r30
    mr r4, r26
    li r5, 0x108
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x108
    bl fn_80239EE8
@L_802485E0:
    mr r3, r26
    mr r4, r29
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_8024862C
    mr r3, r26
    mr r4, r29
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024862C
    mr r3, r26
    mr r4, r29
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248674
@L_8024862C:
    mr r3, r30
    mr r4, r26
    li r5, 0x109
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x109
    bl fn_80239EE8
@L_80248674:
    mr r3, r30
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
