    nofralloc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lhz r0, 0x4cc(r4)
    sth r0, -0x8000(r3)
    blr
