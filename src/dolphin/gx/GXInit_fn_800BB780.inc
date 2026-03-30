    nofralloc
    stwu r1, -0x28(r1)
    rlwinm r4, r4, 0, 30, 27
    rlwinm r0, r5, 2, 0, 29
    or r0, r4, r0
    lwz r11, 0x34(r1)
    rlwinm r5, r0, 0, 28, 24
    lbz r12, 0x33(r1)
    rlwinm r0, r6, 4, 0, 27
    lwz r4, -29632(r13)
    or r0, r5, r0
    rlwinm r5, r0, 0, 25, 22
    rlwinm r0, r11, 7, 0, 24
    or r0, r5, r0
    rlwinm r5, r0, 0, 23, 18
    rlwinm r0, r7, 9, 0, 22
    or r0, r5, r0
    rlwinm r5, r0, 0, 19, 15
    rlwinm r0, r8, 13, 0, 18
    or r0, r5, r0
    addi r5, r3, 0x10
    rlwinm r3, r0, 0, 16, 12
    rlwinm r0, r9, 16, 0, 15
    or r0, r3, r0
    rlwinm r3, r0, 0, 13, 11
    rlwinm r0, r12, 19, 0, 12
    or r0, r3, r0
    rlwinm r3, r0, 0, 12, 10
    clrlslwi r0, r10, 24, 20
    or r6, r3, r0
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    rlwinm r0, r5, 24, 0, 7
    rlwimi r0, r6, 0, 8, 31
    stw r0, -0x8000(r3)
    li r0, 0x0
    sth r0, 0x2(r4)
    addi r1, r1, 0x28
    blr
