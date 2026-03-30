    nofralloc
    lwz r0, -31592(r13)
    rlwinm r4, r3, 0, 24, 31
    cmpw r4, r0
    blt @L_801D1668
    li r3, 0x0
    blr
@L_801D1668:
    lis r3, -32713
    rlwinm r0, r4, 3, 0, 28
    addi r3, r3, -7968
    lwzx r3, r3, r0
    blr
