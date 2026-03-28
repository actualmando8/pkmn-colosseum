    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0xb52
    stw r0, 0x14(r1)
    addi r3, r3, 0x1200
    bl fn_800F92D4
    stw r3, -21480(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
