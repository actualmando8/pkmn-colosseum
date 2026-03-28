    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32737
    li r4, 0x0
    stw r0, 0x14(r1)
    addi r3, r3, -11336
    bl fn_8012BDE0
    stw r3, -21096(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
