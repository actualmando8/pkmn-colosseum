    nofralloc
    lis r6, -32704
    rlwinm r7, r3, 2, 0, 29
    addi r0, r6, -19688
    lis r5, -32704
    add r9, r0, r7
    rlwinm r10, r3, 1, 0, 30
    subi r9, r9, 0x4
    addi r6, r5, -19704
    lwz r3, 0x0(r9)
    addi r5, r13, -24988
    add r7, r6, r7
    li r8, 0x0
    add r6, r5, r10
    addi r0, r13, -24996
    add r5, r0, r10
    stw r4, 0x0(r9)
    stw r8, -0x4(r7)
    sth r8, -0x2(r6)
    sth r8, -0x2(r5)
    blr
