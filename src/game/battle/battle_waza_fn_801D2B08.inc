    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, 0x801d
    li r4, 0x0
    addi r3, r3, 10712
    bl fn_8012BDE0
    lis r3, 0x8046
    li r0, 0x0
    addi r3, r3, 29584
    stw r0, 0x0(r3)
    stw r0, 0x8(r3)
    stw r0, 0xc(r3)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
