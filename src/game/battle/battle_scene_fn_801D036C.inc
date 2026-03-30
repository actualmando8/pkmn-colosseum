    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0x2
    li r4, 0x20
    stw r0, 0x14(r1)
    subi r3, r3, 0x2030
    bl fn_800E2C04
    bl fn_800E27B0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
