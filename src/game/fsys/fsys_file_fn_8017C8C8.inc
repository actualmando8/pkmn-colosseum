    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r3, 0x8(r1)
    stw r0, 0x14(r1)
    lwz r3, 0x8(r1)
    bl fn_8017D92C
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
