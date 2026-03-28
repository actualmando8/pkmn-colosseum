    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, -32713
    mr r4, r3
    stw r0, 0x14(r1)
    addi r3, r5, 21040
    bl fn_800E01D0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
