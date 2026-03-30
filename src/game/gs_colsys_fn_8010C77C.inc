    nofralloc
    lfs f1, 0x4(r5)
    lfs f0, 0x4(r4)
    lfs f3, 0x0(r5)
    lfs f2, 0x0(r4)
    fsubs f0, f1, f0
    lfs f1, 0x4(r3)
    fsubs f2, f3, f2
    lfs f4, 0x8(r5)
    lfs f3, 0x8(r4)
    fmuls f0, f1, f0
    lfs f1, 0x0(r3)
    fsubs f3, f4, f3
    fmadds f0, f1, f2, f0
    lfs f1, 0x8(r3)
    fmadds f1, f1, f3, f0
    blr
