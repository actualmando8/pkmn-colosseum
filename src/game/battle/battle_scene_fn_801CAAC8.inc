    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r5, r4
    mr r4, r3
    li r3, 0x4
    bl fn_80115E6C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
