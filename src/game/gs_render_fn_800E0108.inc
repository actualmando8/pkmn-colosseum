    nofralloc
    lfs f1, 0x0(r4)
    lfs f0, 0x0(r5)
    fmuls f0, f1, f0
    stfs f0, 0x0(r3)
    lfs f1, 0x4(r4)
    lfs f0, 0x4(r5)
    fmuls f0, f1, f0
    stfs f0, 0x4(r3)
    lfs f1, 0x8(r4)
    lfs f0, 0x8(r5)
    fmuls f0, f1, f0
    stfs f0, 0x8(r3)
    blr
