    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32698
    li r6, 0x0
    stw r0, 0x14(r1)
    li r0, 0x1
    addi r3, r3, 31992
    li r4, 0x0
    stb r6, -21504(r13)
    li r5, 0x10
    stw r6, -21500(r13)
    stw r6, -21492(r13)
    stw r6, -21480(r13)
    stb r0, -21484(r13)
    stb r6, -21476(r13)
    bl memset
    lis r4, -32738
    li r3, 0x1
    addi r6, r4, 4968
    li r5, 0xa
    li r4, 0xfd
    bl fn_800FE834
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
