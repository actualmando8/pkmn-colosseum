    nofralloc
    rlwinm. r0, r3, 0, 24, 31
    beq @L_800BCDA0
    lhz r8, 0x2(r5)
    li r0, 0x61
    lhz r10, 0x0(r5)
    lis r6, 0xcc01
    rlwinm r8, r8, 12, 0, 19
    rlwimi r8, r10, 0, 20, 31
    stb r0, -0x8000(r6)
    li r7, 0xe9
    rlwinm r9, r8, 0, 8, 31
    rlwimi r9, r7, 24, 0, 7
    stw r9, -0x8000(r6)
    li r7, 0xea
    rlwinm r9, r7, 24, 0, 7
    lhzu r10, 0x4(r5)
    li r7, 0xeb
    lhz r8, 0x2(r5)
    rlwinm r8, r8, 12, 0, 19
    stb r0, -0x8000(r6)
    rlwimi r8, r10, 0, 20, 31
    rlwimi r9, r8, 0, 8, 31
    stw r9, -0x8000(r6)
    rlwinm r9, r7, 24, 0, 7
    li r7, 0xec
    lhz r8, 0x6(r5)
    lhz r10, 0x4(r5)
    rlwinm r8, r8, 12, 0, 19
    rlwimi r8, r10, 0, 20, 31
    stb r0, -0x8000(r6)
    rlwimi r9, r8, 0, 8, 31
    stw r9, -0x8000(r6)
    rlwinm r9, r7, 24, 0, 7
    li r7, 0xed
    lhz r8, 0xa(r5)
    lhz r10, 0x8(r5)
    rlwinm r8, r8, 12, 0, 19
    rlwimi r8, r10, 0, 20, 31
    stb r0, -0x8000(r6)
    rlwimi r9, r8, 0, 8, 31
    stw r9, -0x8000(r6)
    lhz r8, 0xe(r5)
    lhz r10, 0xc(r5)
    rlwinm r8, r8, 12, 0, 19
    rlwimi r8, r10, 0, 20, 31
    stb r0, -0x8000(r6)
    rlwinm r9, r8, 0, 8, 31
    rlwimi r9, r7, 24, 0, 7
    stw r9, -0x8000(r6)
@L_800BCDA0:
    rlwinm r5, r4, 0, 16, 31
    lwz r4, -29632(r13)
    addi r0, r5, 0x156
    rlwinm r5, r0, 0, 22, 20
    clrlslwi r0, r3, 24, 10
    or r0, r5, r0
    rlwinm r5, r0, 0, 8, 31
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    oris r5, r5, 0xe800
    li r0, 0x0
    stw r5, -0x8000(r3)
    sth r0, 0x2(r4)
    blr
