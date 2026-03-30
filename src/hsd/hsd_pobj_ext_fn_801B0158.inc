    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32698
    li r4, 0x1c
    stw r0, 0x14(r1)
    addi r3, r3, 22196
    li r5, 0x4
    bl fn_801AA35C
    lis r3, -32698
    li r4, 0xc
    addi r3, r3, 22152
    li r5, 0x4
    bl fn_801AA35C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
