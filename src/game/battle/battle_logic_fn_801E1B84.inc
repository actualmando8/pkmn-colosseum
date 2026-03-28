    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32697
    li r5, 0x1
    stw r0, 0x14(r1)
    addi r4, r1, 0x8
    addi r3, r3, -23600
    bl fn_8009F2F8
    lwz r0, 0x14(r1)
    lwz r3, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
