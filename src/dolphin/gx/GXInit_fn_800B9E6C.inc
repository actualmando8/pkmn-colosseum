    nofralloc
    lwz r4, -29632(r13)
    rlwinm r0, r3, 7, 0, 24
    lwzu r3, 0x1ec(r4)
    rlwinm r3, r3, 0, 25, 22
    or r0, r3, r0
    stw r0, 0x0(r4)
    blr
