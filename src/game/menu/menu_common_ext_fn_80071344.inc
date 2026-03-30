    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, -32709
    li r4, 0x0
    addi r3, r3, 28040
    li r6, 0x10
    lwz r0, 0x40(r3)
    li r7, 0x1
    li r8, 0x0
    li r9, 0x0
    rlwinm r0, r0, 3, 0, 28
    add r5, r3, r0
    lwz r3, 0x0(r5)
    addi r5, r5, 0x4
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
