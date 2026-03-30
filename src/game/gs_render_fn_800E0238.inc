    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    mr r3, r4
    mr r4, r0
    bl fn_800A2E64
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
