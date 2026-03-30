    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32733
    li r6, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, -7348
    addi r5, r1, 0x8
    stb r3, 0x8(r1)
    li r3, 0x0
    bl fn_801F37B0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
