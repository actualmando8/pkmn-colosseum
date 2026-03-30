    nofralloc
    rlwinm r3, r3, 2, 0, 29
    lwz r7, -29632(r13)
    addi r8, r3, 0x170
    add r8, r7, r8
    lwz r0, 0x0(r8)
    rlwinm r5, r5, 2, 0, 29
    li r3, 0x61
    rlwinm r0, r0, 0, 0, 29
    or r0, r0, r4
    stw r0, 0x0(r8)
    lis r4, 0xcc01
    li r0, 0x0
    lwz r6, 0x0(r8)
    rlwinm r6, r6, 0, 30, 27
    or r5, r6, r5
    stw r5, 0x0(r8)
    stb r3, -0x8000(r4)
    lwz r3, 0x0(r8)
    stw r3, -0x8000(r4)
    sth r0, 0x2(r7)
    blr
