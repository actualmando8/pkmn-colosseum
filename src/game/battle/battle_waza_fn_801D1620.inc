    nofralloc
    lwz r0, -31592(r13)
    rlwinm r4, r3, 0, 24, 31
    cmpw r4, r0
    blt @L_801D1638
    li r3, 0x0
    blr
@L_801D1638:
    lis r3, -32713
    rlwinm r0, r4, 3, 0, 28
    addi r3, r3, -7968
    add r3, r3, r0
    lwz r3, 0x4(r3)
    blr
