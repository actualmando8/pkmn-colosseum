    nofralloc
    lwz r6, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lhz r5, 0x4d0(r6)
    lhz r4, 0x4ce(r6)
    lhz r0, 0x4cc(r6)
    sth r0, -0x8000(r3)
    sth r4, -0x8000(r3)
    sth r5, -0x8000(r3)
    blr
