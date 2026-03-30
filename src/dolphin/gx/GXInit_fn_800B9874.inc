    nofralloc
    lwz r6, -29632(r13)
    rlwinm r4, r3, 0, 31, 31
    subi r0, r4, 0x1
    lwz r4, 0x1ec(r6)
    cntlzw r0, r0
    rlwinm r3, r3, 0, 30, 30
    rlwinm r5, r4, 0, 0, 30
    rlwinm r4, r0, 27, 24, 31
    or r0, r5, r4
    stw r0, 0x1ec(r6)
    subi r0, r3, 0x2
    cntlzw r0, r0
    lwz r3, 0x1ec(r6)
    rlwinm r5, r0, 28, 23, 30
    rlwinm r3, r3, 0, 31, 29
    or r0, r3, r5
    stw r0, 0x1ec(r6)
    lwz r0, 0x1fc(r6)
    rlwinm r0, r0, 0, 0, 30
    or r0, r0, r4
    stw r0, 0x1fc(r6)
    lwz r0, 0x1fc(r6)
    rlwinm r0, r0, 0, 31, 29
    or r0, r0, r5
    stw r0, 0x1fc(r6)
    blr
