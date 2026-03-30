    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r7, 0x0
    li r8, 0x0
    stw r0, 0x14(r1)
    mr r0, r4
    mr r4, r5
    li r9, 0x0
    mr r5, r0
    li r10, 0x0
    bl fn_80211170
    lwz r0, 0x14(r1)
    mulli r3, r3, 0x3
    mtlr r0
    addi r1, r1, 0x10
    blr
