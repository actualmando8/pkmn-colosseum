    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x2
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    li r3, 0x2
    li r4, 0x17a7
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
