    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r10, r4
    mr r9, r5
    mr r0, r6
    mr r8, r7
    mr r4, r3
    mr r5, r10
    mr r6, r9
    mr r7, r0
    li r3, 0x1
    bl fn_8001D8E8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
