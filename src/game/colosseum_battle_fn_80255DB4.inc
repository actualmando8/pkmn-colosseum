    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r7, 0x10
    li r8, 0x2
    stw r0, 0x14(r1)
    mr r0, r5
    mr r5, r6
    li r9, 0x41
    mr r6, r0
    bl fn_8025C808
    rlwinm r3, r3, 0, 24, 31
    subic r0, r3, 0x1
    subfe r3, r0, r3
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
