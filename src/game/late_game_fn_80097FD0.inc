    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80113F48
    lis r4, 0x1267
    bl fn_800F9318
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
