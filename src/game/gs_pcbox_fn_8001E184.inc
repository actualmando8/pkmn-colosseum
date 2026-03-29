    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r0, 0x0
    stw r0, 0x8(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x12
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x12
    li r4, 0x1
    bl fn_801045A8
    li r3, 0x12
    bl fn_801043A4
    extsb r31, r3
    li r3, 0x12
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
