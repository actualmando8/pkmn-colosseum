    nofralloc
    lwz r6, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lbz r5, 0x4ae(r6)
    lbz r4, 0x4ad(r6)
    lbz r0, 0x4ac(r6)
    stb r0, -0x8000(r3)
    stb r4, -0x8000(r3)
    stb r5, -0x8000(r3)
    blr
