    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r5, r3
    mr r3, r4
    stw r0, 0x14(r1)
    addi r4, r5, 0x88
    bl fn_800E01D0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
