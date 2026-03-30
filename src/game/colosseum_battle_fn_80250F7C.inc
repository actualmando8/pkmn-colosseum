    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r8, -32731
    li r7, 0x0
    stw r0, 0x14(r1)
    mr r0, r5
    addi r9, r8, 4028
    mr r5, r4
    mr r4, r0
    li r8, 0x0
    li r10, 0x0
    bl fn_80211170
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
