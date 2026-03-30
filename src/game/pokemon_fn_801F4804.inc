    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x58
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_801F54A4
    extsh r31, r3
    mr r3, r30
    addi r0, r31, 0x1
    li r4, 0x0
    extsh r7, r0
    li r5, 0x58
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
