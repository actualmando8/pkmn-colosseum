    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32697
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x1c0
    stw r31, 0xc(r1)
    addi r31, r3, -23488
    addi r3, r31, 0x820
    bl memset
    addi r3, r31, 0x54
    addi r4, r31, 0x48
    li r5, 0x3
    bl fn_8009F1D0
    li r0, 0x1
    li r3, 0x1
    stw r0, -21432(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
