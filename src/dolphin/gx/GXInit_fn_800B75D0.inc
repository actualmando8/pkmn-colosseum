    nofralloc
    lwz r6, -29632(r13)
    rlwinm r3, r3, 0, 24, 31
    clrlslwi r0, r4, 24, 1
    lwz r5, 0x10(r6)
    rlwinm r4, r5, 0, 0, 30
    or r3, r4, r3
    stw r3, 0x10(r6)
    lwz r3, 0x10(r6)
    rlwinm r3, r3, 0, 31, 29
    or r0, r3, r0
    stw r0, 0x10(r6)
    lwz r0, 0x10(r6)
    lwz r3, -24228(r13)
    sth r0, 0x4(r3)
    blr
