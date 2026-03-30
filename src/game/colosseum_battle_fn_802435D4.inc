    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    bl fn_802376EC
    mr r31, r3
    mr r3, r27
    mr r4, r30
    bl fn_802376EC
    clrlwi r3, r3, 16
    clrlwi r0, r31, 16
    divw r30, r3, r0
    li r3, 0x0
    li r4, 0x180
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    mullw r30, r30, r3
    li r3, 0x0
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
    li r10, 0x180
    bl fn_80239CCC
    mr r3, r31
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
