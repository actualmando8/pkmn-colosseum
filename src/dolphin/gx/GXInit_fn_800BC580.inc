    nofralloc
    rlwinm r10, r3, 1, 0, 30
    lwz r9, -29632(r13)
    rlwinm r3, r3, 3, 0, 28
    addi r11, r3, 0x1b0
    add r11, r9, r11
    lwz r8, 0x0(r11)
    addi r0, r10, 0x1
    rlwinm r3, r0, 2, 0, 29
    rlwinm r0, r8, 0, 0, 29
    or r0, r0, r4
    stw r0, 0x0(r11)
    addi r10, r3, 0x1b0
    rlwinm r0, r5, 2, 0, 29
    lwz r3, 0x0(r11)
    li r8, 0x61
    lis r5, 0xcc01
    rlwinm r3, r3, 0, 30, 27
    or r0, r3, r0
    stw r0, 0x0(r11)
    add r10, r9, r10
    rlwinm r3, r7, 2, 0, 29
    stb r8, -0x8000(r5)
    li r0, 0x0
    lwz r4, 0x0(r11)
    stw r4, -0x8000(r5)
    lwz r4, 0x0(r10)
    rlwinm r4, r4, 0, 0, 29
    or r4, r4, r6
    stw r4, 0x0(r10)
    lwz r4, 0x0(r10)
    rlwinm r4, r4, 0, 30, 27
    or r3, r4, r3
    stw r3, 0x0(r10)
    stb r8, -0x8000(r5)
    lwz r3, 0x0(r10)
    stw r3, -0x8000(r5)
    sth r0, 0x2(r9)
    blr
