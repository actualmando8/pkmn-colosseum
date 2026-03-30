    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r31, r5
    mr r30, r4
    mr r29, r3
    mr r26, r6
    mr r4, r31
    bl fn_80239564
    clrlwi r27, r3, 24
    li r3, 0x0
    li r4, 0x113
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r28, r27, r3
    li r3, 0x0
    mr r4, r28
    bl fn_802399FC
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r28, 0x8(r1)
    mr r5, r3
    mr r4, r29
    subi r3, r6, 0x139c
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x113
    bl fn_80239CCC
    mr r4, r29
    mr r8, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024809C
    mr r3, r27
    mr r4, r29
    li r5, 0x114
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x114
    bl fn_80239EE8
@L_8024809C:
    mr r3, r29
    mr r4, r26
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_802480E8
    mr r3, r29
    mr r4, r26
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802480E8
    mr r3, r29
    mr r4, r26
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248158
@L_802480E8:
    mr r3, r29
    mr r4, r31
    bl fn_80239564
    clrlwi r28, r3, 24
    li r3, 0x0
    li r4, 0x115
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r28, r28, r3
    mr r3, r27
    mr r4, r28
    bl fn_802399FC
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r28, 0x8(r1)
    mr r5, r3
    mr r4, r29
    subi r3, r6, 0x139c
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x115
    bl fn_80239CCC
@L_80248158:
    mr r3, r27
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
