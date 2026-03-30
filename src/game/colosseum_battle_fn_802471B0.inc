    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r29, r5
    mr r28, r4
    mr r27, r3
    mr r4, r29
    bl fn_80239564
    clrlwi r30, r3, 24
    li r3, 0x0
    li r4, 0x131
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r31, r30, r3
    li r3, 0x0
    mr r4, r31
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r31, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x131
    bl fn_80239CCC
    mr r3, r27
    mr r4, r28
    bl fn_80235AA0
    clrlwi r0, r3, 24
    cmplwi r0, 0xc
    blt @L_802472C0
    mr r3, r27
    mr r4, r29
    bl fn_80239564
    clrlwi r31, r3, 24
    li r3, 0x0
    li r4, 0x132
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r31, r31, r3
    mr r3, r30
    mr r4, r31
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r31, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x132
    bl fn_80239CCC
@L_802472C0:
    mr r3, r30
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
