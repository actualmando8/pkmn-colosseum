    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r4, r3
    stw r0, 0x14(r1)
    bl fn_800A335C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
