    nofralloc
    lwz r4, -29632(r2)
    clrlslwi r0, r3, 24, 16
    lwz r3, 0x204(r4)
    rlwinm r3, r3, 0, 16, 12
    or r0, r3, r0
    stw r0, 0x204(r4)
    lwz r0, 0x4f4(r4)
    ori r0, r0, 0x6
    stw r0, 0x4f4(r4)
    blr
