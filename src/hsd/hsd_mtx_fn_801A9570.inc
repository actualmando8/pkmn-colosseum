    nofralloc
    lfs f0, 0xc(r3)
    stfs f0, 0x0(r4)
    lfs f0, 0x1c(r3)
    stfs f0, 0x4(r4)
    lfs f0, 0x2c(r3)
    stfs f0, 0x8(r4)
    blr
