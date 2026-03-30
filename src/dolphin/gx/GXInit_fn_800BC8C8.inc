    nofralloc
    lwz r4, -29632(r13)
    rlwinm r3, r3, 0, 24, 31
    subi r0, r3, 0x1
    lwz r3, 0x204(r4)
    rlwinm r0, r0, 10, 0, 21
    rlwinm r3, r3, 0, 22, 17
    or r0, r3, r0
    stw r0, 0x204(r4)
    lwz r0, 0x4f4(r4)
    ori r0, r0, 0x4
    stw r0, 0x4f4(r4)
    blr
