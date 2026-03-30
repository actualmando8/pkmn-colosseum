    nofralloc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    lfs f2, 0x4dc(r4)
    lfs f1, 0x4d8(r4)
    lfs f0, 0x4d4(r4)
    stfs f0, -0x8000(r3)
    stfs f1, -0x8000(r3)
    stfs f2, -0x8000(r3)
    blr
