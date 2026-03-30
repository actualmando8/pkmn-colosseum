    nofralloc
    lwz r7, -24224(r13)
    cmpwi r3, 0x1
    li r9, 0x1
    addi r10, r7, 0x2
    lhz r7, 0x2(r7)
    beq @L_800B8F00
    cmpwi r3, 0x3
    beq @L_800B8F00
    li r9, 0x0
@L_800B8F00:
    rlwinm r8, r7, 0, 0, 30
    subfic r0, r3, 0x3
    cntlzw r7, r0
    subfic r0, r3, 0x2
    or r8, r8, r9
    cntlzw r0, r0
    rlwinm r8, r8, 0, 21, 19
    rlwinm r3, r7, 6, 0, 20
    or r3, r8, r3
    rlwinm r3, r3, 0, 31, 29
    rlwinm r0, r0, 28, 4, 30
    or r0, r3, r0
    rlwinm r3, r0, 0, 20, 15
    rlwinm r0, r6, 12, 0, 19
    or r0, r3, r0
    rlwinm r3, r0, 0, 24, 20
    rlwinm r0, r4, 8, 0, 23
    or r0, r3, r0
    rlwinm r3, r0, 0, 27, 23
    rlwinm r0, r5, 5, 0, 26
    or r0, r3, r0
    rlwinm r0, r0, 0, 8, 31
    oris r0, r0, 0x4100
    sth r0, 0x0(r10)
    blr
