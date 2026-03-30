    nofralloc
    lwz r3, -29632(r13)
    lwz r0, 0x8(r3)
    rlwinm r0, r0, 0, 0, 30
    ori r0, r0, 0x1
    stw r0, 0x8(r3)
    lwz r0, 0x8(r3)
    lwz r3, -24228(r13)
    sth r0, 0x2(r3)
    blr
