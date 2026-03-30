    nofralloc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lbz r0, 0x4ac(r4)
    stb r0, -0x8000(r3)
    blr
