    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, fn_801C4C98@ha
    addi r3, r3, fn_801C4C98@l
    bl fn_801C431C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
