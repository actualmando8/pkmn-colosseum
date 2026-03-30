    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_8020D908
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    mr r7, r31
    li r5, 0x45
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    bl fn_8020D8D8
    extsh r7, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_801254B4
    mr r3, r30
    bl fn_80213158
    lmw r30, 0x8(r1)
    li r3, 0x1
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
