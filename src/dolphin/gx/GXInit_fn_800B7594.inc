    nofralloc
    lwz r6, -29632(r2)
    clrlslwi r3, r3, 24, 2
    clrlslwi r0, r4, 24, 3
    lwz r5, 0x8(r6)
    rlwinm r4, r5, 0, 30, 28
    or r3, r4, r3
    stw r3, 0x8(r6)
    lwz r3, 0x8(r6)
    rlwinm r3, r3, 0, 29, 27
    or r0, r3, r0
    stw r0, 0x8(r6)
    lwz r0, 0x8(r6)
    lwz r3, -24228(r13)
    sth r0, 0x2(r3)
    blr
