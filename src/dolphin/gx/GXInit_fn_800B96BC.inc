    nofralloc
    lwz r4, -29632(r13)
    li r0, 0x0
    stw r0, 0x1e8(r4)
    addi r5, r4, 0x1e8
    clrlslwi r0, r3, 17, 1
    lwz r4, 0x1e8(r4)
    srawi r0, r0, 5
    rlwinm r3, r4, 0, 0, 21
    or r0, r3, r0
    stw r0, 0x0(r5)
    lwz r0, 0x0(r5)
    rlwinm r0, r0, 0, 8, 31
    oris r0, r0, 0x4d00
    stw r0, 0x0(r5)
    blr
