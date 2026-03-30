    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32704
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    addi r30, r3, 19560
    stw r0, 0x3704(r30)
    stw r0, 0x370c(r30)
    bl fn_800D7894
    lis r5, -32704
    li r4, 0x1
    addi r31, r5, 19560
    li r5, 0x0
    stw r3, 0x3708(r31)
    li r6, 0x1
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, 0x3708(r31)
    li r4, 0x4
    li r5, 0x0
    li r6, 0x6
    li r7, 0xa
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    li r0, 0x0
    stw r0, 0x370c(r30)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
