    nofralloc
    lwz r5, -24224(r13)
    clrlslwi r0, r3, 24, 8
    rlwimi r0, r4, 0, 24, 31
    sth r0, 0x4(r5)
    blr
