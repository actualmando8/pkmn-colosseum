    nofralloc
    lwz r9, -29632(r13)
    li r10, 0x0
    rlwinm r7, r5, 0, 16, 31
    stw r10, 0x1e0(r9)
    rlwinm r5, r6, 0, 16, 31
    subi r0, r5, 0x1
    lwz r8, 0x1e0(r9)
    rlwinm r3, r3, 0, 16, 31
    clrlslwi r4, r4, 16, 10
    rlwinm r5, r8, 0, 0, 21
    or r3, r5, r3
    stw r3, 0x1e0(r9)
    subi r3, r7, 0x1
    rlwinm r0, r0, 10, 0, 21
    lwz r5, 0x1e0(r9)
    rlwinm r5, r5, 0, 22, 11
    or r4, r5, r4
    stw r4, 0x1e0(r9)
    lwz r4, 0x1e0(r9)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x4900
    stw r4, 0x1e0(r9)
    stw r10, 0x1e4(r9)
    lwz r4, 0x1e4(r9)
    rlwinm r4, r4, 0, 0, 21
    or r3, r4, r3
    stw r3, 0x1e4(r9)
    lwz r3, 0x1e4(r9)
    rlwinm r3, r3, 0, 22, 11
    or r0, r3, r0
    stw r0, 0x1e4(r9)
    lwz r0, 0x1e4(r9)
    rlwinm r0, r0, 0, 8, 31
    oris r0, r0, 0x4a00
    stw r0, 0x1e4(r9)
    blr
