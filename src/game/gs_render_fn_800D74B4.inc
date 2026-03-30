    nofralloc
    lwz r5, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lbz r4, 0x4ad(r5)
    lbz r0, 0x4ac(r5)
    stb r0, -0x8000(r3)
    stb r4, -0x8000(r3)
    blr
