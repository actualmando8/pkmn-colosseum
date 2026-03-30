    nofralloc
    rlwinm. r0, r4, 0, 24, 31
    beq @L_800B9EC8
    lwz r7, -29632(r13)
    li r0, 0x61
    lis r5, 0xcc01
    lwz r6, 0x1d8(r7)
    rlwinm r6, r6, 0, 0, 30
    stb r0, -0x8000(r5)
    ori r6, r6, 0x1
    rlwinm r6, r6, 0, 31, 27
    ori r6, r6, 0xe
    stw r6, -0x8000(r5)
    lwz r6, 0x1d0(r7)
    stb r0, -0x8000(r5)
    rlwinm r0, r6, 0, 0, 29
    stw r0, -0x8000(r5)
@L_800B9EC8:
    rlwinm. r0, r4, 0, 24, 31
    li r0, 0x0
    bne @L_800B9EE8
    lwz r5, -29632(r13)
    lwz r5, 0x1dc(r5)
    rlwinm r5, r5, 0, 29, 31
    cmplwi r5, 0x3
    bne @L_800B9F14
@L_800B9EE8:
    lwz r5, -29632(r13)
    lwz r7, 0x1dc(r5)
    rlwinm r5, r7, 26, 31, 31
    cmplwi r5, 0x1
    bne @L_800B9F14
    li r0, 0x61
    lis r6, 0xcc01
    stb r0, -0x8000(r6)
    rlwinm r5, r7, 0, 26, 24
    li r0, 0x1
    stw r5, -0x8000(r6)
@L_800B9F14:
    li r9, 0x61
    lwz r7, -29632(r13)
    lis r8, 0xcc01
    stb r9, -0x8000(r8)
    rlwinm r3, r3, 27, 8, 31
    oris r10, r3, 0x4b00
    lwz r6, 0x1e0(r7)
    rlwinm. r5, r4, 0, 24, 31
    clrlslwi r3, r4, 24, 11
    stw r6, -0x8000(r8)
    stb r9, -0x8000(r8)
    lwz r4, 0x1e4(r7)
    stw r4, -0x8000(r8)
    stb r9, -0x8000(r8)
    lwz r4, 0x1e8(r7)
    stw r4, -0x8000(r8)
    stb r9, -0x8000(r8)
    stw r10, -0x8000(r8)
    lwz r4, 0x1ec(r7)
    rlwinm r4, r4, 0, 21, 19
    or r3, r4, r3
    stw r3, 0x1ec(r7)
    lwz r3, 0x1ec(r7)
    rlwinm r3, r3, 0, 18, 16
    ori r3, r3, 0x4000
    stw r3, 0x1ec(r7)
    lwz r3, 0x1ec(r7)
    rlwinm r3, r3, 0, 8, 31
    oris r3, r3, 0x5200
    stw r3, 0x1ec(r7)
    stb r9, -0x8000(r8)
    lwz r3, 0x1ec(r7)
    stw r3, -0x8000(r8)
    beq @L_800B9FB4
    stb r9, -0x8000(r8)
    lwz r3, 0x1d8(r7)
    stw r3, -0x8000(r8)
    stb r9, -0x8000(r8)
    lwz r3, 0x1d0(r7)
    stw r3, -0x8000(r8)
@L_800B9FB4:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_800B9FD4
    li r0, 0x61
    lwz r3, -29632(r13)
    lis r4, 0xcc01
    stb r0, -0x8000(r4)
    lwz r0, 0x1dc(r3)
    stw r0, -0x8000(r4)
@L_800B9FD4:
    lwz r3, -29632(r13)
    li r0, 0x0
    sth r0, 0x2(r3)
    blr
