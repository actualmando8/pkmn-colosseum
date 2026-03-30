    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r4, 0x59
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r3
    addi r3, r1, 0x8
    bl fn_800A3074
    mr r3, r31
    mr r5, r31
    addi r4, r1, 0x8
    bl fn_800A2D98
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
