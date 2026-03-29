    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    stw r8, 0x10(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    bl fn_801046B8
    stw r29, 0x8(r1)
    mr r4, r3
    mr r10, r28
    addi r5, r1, 0x10
    clrlwi r9, r27, 24
    li r3, 0xe7
    li r6, 0x0
    li r7, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    extsh r4, r30
    extsh r5, r31
    li r3, 0xe7
    bl fn_80102868
    li r3, 0xe7
    li r4, 0x1
    bl fn_801045A8
    li r3, 0xe7
    bl fn_801043A4
    mr r0, r3
    li r3, 0xe7
    mr r31, r0
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r31
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
