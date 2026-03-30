    nofralloc
    lis r3, -32698
    lfs f5, -21980(r2)
    addi r3, r3, 28752
    lfs f6, -22008(r2)
    lfs f0, 0x14(r3)
    lfs f3, 0x1c(r3)
    fmuls f2, f0, f0
    lfs f0, 0x18(r3)
    fmuls f7, f3, f3
    lfs f3, -21972(r2)
    fmuls f9, f0, f1
    lfs f0, -21976(r2)
    fmuls f8, f1, f2
    fadds f4, f5, f9
    fsubs f2, f9, f5
    fmuls f1, f1, f8
    fdivs f4, f6, f4
    fdivs f1, f1, f7
    fnmsubs f3, f3, f1, f0
    fmuls f0, f5, f1
    fmuls f1, f4, f2
    fmuls f2, f3, f4
    fmuls f0, f0, f4
    stfs f1, 0xc(r3)
    stfs f2, 0x8(r3)
    stfs f0, 0x10(r3)
    blr
