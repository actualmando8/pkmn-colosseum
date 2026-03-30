    nofralloc
    lwz r5, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lhz r4, 0x4b2(r5)
    lhz r0, 0x4b0(r5)
    sth r0, -0x8000(r3)
    sth r4, -0x8000(r3)
    blr
