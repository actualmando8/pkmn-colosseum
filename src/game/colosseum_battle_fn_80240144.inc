    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    li r3, 0x0
    li r4, 0x1cb
    li r5, 0x3e
    bl fn_801FB1C0
    mr r31, r3
    bl fn_800E0C54
    clrlwi r5, r3, 16
    addi r4, r31, 0x1
    divw r0, r5, r4
    li r3, 0x0
    mullw r0, r0, r4
    subf r30, r0, r5
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
    li r10, 0x1cb
    bl fn_80239CCC
    mr r3, r31
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
