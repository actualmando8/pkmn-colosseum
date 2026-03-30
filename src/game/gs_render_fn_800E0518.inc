    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x59
    stw r0, 0x14(r1)
    bl fn_800A3074
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
