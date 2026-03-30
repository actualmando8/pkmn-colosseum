    nofralloc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lfs f1, 0x4bc(r4)
    lfs f0, 0x4b8(r4)
    stfs f0, -0x8000(r3)
    stfs f1, -0x8000(r3)
    blr
