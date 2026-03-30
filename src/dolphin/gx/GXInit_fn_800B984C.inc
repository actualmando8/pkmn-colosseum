    nofralloc
    lwz r4, -29632(r13)
    rlwinm r0, r3, 12, 0, 19
    lwz r3, 0x1ec(r4)
    rlwinm r3, r3, 0, 20, 17
    or r0, r3, r0
    stw r0, 0x1ec(r4)
    lwzu r0, 0x1fc(r4)
    rlwinm r0, r0, 0, 20, 17
    stw r0, 0x0(r4)
    blr
