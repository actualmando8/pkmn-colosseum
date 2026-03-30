    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_800E1544
    lis r5, -32729
    mr r4, r3
    addi r3, r5, 10936
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
