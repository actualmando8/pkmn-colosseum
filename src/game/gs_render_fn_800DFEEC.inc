    nofralloc
    lfs f6, 0x4(r5)
    lfs f12, 0x4(r4)
    lfs f10, 0x8(r5)
    lfs f11, 0x0(r4)
    fmuls f0, f6, f12
    lfs f5, 0x0(r5)
    fmuls f2, f10, f12
    lfs f8, 0x8(r4)
    fmuls f1, f6, f11
    lfs f9, 0xc(r4)
    fmadds f0, f5, f11, f0
    fmadds f3, f5, f9, f2
    fmuls f2, f5, f8
    fmadds f7, f10, f8, f0
    fmadds f1, f10, f9, f1
    fnmsubs f4, f6, f8, f3
    fmuls f0, f11, f7
    fmadds f2, f6, f9, f2
    fnmsubs f6, f5, f12, f1
    fmadds f0, f9, f4, f0
    fnmsubs f5, f10, f11, f2
    fmadds f0, f12, f6, f0
    fnmsubs f0, f8, f5, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x4(r4)
    lfs f1, 0xc(r4)
    fmuls f0, f0, f7
    lfs f2, 0x8(r4)
    lfs f3, 0x0(r4)
    fmadds f0, f1, f5, f0
    fmadds f0, f2, f4, f0
    fnmsubs f0, f3, f6, f0
    stfs f0, 0x4(r3)
    lfs f0, 0x8(r4)
    lfs f1, 0xc(r4)
    fmuls f0, f0, f7
    lfs f2, 0x0(r4)
    lfs f3, 0x4(r4)
    fmadds f0, f1, f6, f0
    fmadds f0, f2, f5, f0
    fnmsubs f0, f3, f4, f0
    stfs f0, 0x8(r3)
    blr
