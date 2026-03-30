    nofralloc
    fmuls f11, f2, f2
    lfs f0, -22624(r2)
    fmuls f10, f1, f1
    lfs f8, -22628(r2)
    lfs f7, -22684(r2)
    fmuls f9, f11, f2
    fmuls f0, f0, f11
    fmuls f11, f11, f1
    fmuls f9, f10, f9
    fmuls f10, f0, f10
    fmuls f8, f8, f9
    fsubs f0, f9, f11
    fsubs f9, f9, f11
    fmuls f1, f8, f1
    fsubs f8, f0, f11
    fneg f0, f1
    fsubs f1, f1, f10
    fadds f2, f2, f8
    fadds f0, f0, f10
    fadds f1, f7, f1
    fmuls f0, f4, f0
    fmadds f0, f3, f1, f0
    fmadds f0, f5, f2, f0
    fmadds f1, f6, f9, f0
    blr
