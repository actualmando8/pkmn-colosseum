    nofralloc
    lwz r4, lbl_8047AA80(r13)
    slwi r0, r3, 4
    lis r3, 0xcc01
    add r4, r4, r0
    lfs f1, 0x52c(r4)
    lfs f0, 0x528(r4)
    stfs f0, -0x8000(r3)
    stfs f1, -0x8000(r3)
    blr
