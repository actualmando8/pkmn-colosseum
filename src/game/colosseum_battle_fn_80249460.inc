    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r29, r5
    mr r28, r4
    mr r27, r3
    mr r30, r6
    mr r4, r29
    bl fn_80239564
    clrlwi r26, r3, 24
    li r3, 0x0
    li r4, 0xe7
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r26, r26, r3
    li r3, 0x0
    mr r4, r26
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r26, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe7
    bl fn_80239CCC
    mr r4, r27
    mr r8, r28
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024955C
    mr r3, r31
    mr r4, r27
    li r5, 0xe8
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe8
    bl fn_80239EE8
@L_8024955C:
    mr r3, r27
    mr r4, r30
    li r5, 0x7
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802495C0
    mr r3, r31
    mr r4, r27
    li r5, 0xe9
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe9
    bl fn_80239EE8
@L_802495C0:
    mr r3, r27
    mr r4, r30
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_802495F0
    mr r3, r27
    mr r4, r30
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80249660
@L_802495F0:
    mr r3, r27
    mr r4, r29
    bl fn_80239564
    clrlwi r30, r3, 24
    li r3, 0x0
    li r4, 0xea
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r30, r30, r3
    mr r3, r31
    mr r4, r30
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r30, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xea
    bl fn_80239CCC
@L_80249660:
    mr r3, r31
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
