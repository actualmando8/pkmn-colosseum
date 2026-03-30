    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_800FF560
    lis r5, fn_8000CD50@ha
    mr r4, r3
    addi r8, r5, fn_8000CD50@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
