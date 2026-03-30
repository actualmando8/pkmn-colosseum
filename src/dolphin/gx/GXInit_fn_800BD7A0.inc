    nofralloc
    lwz r8, -29632(r13)
    addi r9, r3, 0x156
    addi r10, r4, 0x156
    lwz r3, 0xf8(r8)
    subi r0, r5, 0x1
    rlwinm r4, r9, 12, 0, 19
    rlwinm r3, r3, 0, 0, 20
    or r3, r3, r10
    stw r3, 0xf8(r8)
    subi r3, r6, 0x1
    add r6, r10, r3
    lwz r5, 0xf8(r8)
    add r0, r9, r0
    rlwinm r3, r5, 0, 20, 8
    or r3, r3, r4
    stw r3, 0xf8(r8)
    rlwinm r3, r0, 12, 0, 19
    li r5, 0x61
    lwz r7, 0xfc(r8)
    lis r4, 0xcc01
    li r0, 0x0
    rlwinm r7, r7, 0, 0, 20
    or r6, r7, r6
    stw r6, 0xfc(r8)
    lwz r6, 0xfc(r8)
    rlwinm r6, r6, 0, 20, 8
    or r3, r6, r3
    stw r3, 0xfc(r8)
    stb r5, -0x8000(r4)
    lwz r3, 0xf8(r8)
    stw r3, -0x8000(r4)
    stb r5, -0x8000(r4)
    lwz r3, 0xfc(r8)
    stw r3, -0x8000(r4)
    sth r0, 0x2(r8)
    blr
