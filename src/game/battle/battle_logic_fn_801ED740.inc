    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32737
    li r4, 0xf0
    stw r0, 0x14(r1)
    li r0, 0x0
    addi r6, r3, -10368
    li r3, 0x1
    stb r0, -21088(r13)
    li r5, 0xa
    stb r0, -21087(r13)
    bl fn_800FE834
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
