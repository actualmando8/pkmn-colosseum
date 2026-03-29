    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x3cdf
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
