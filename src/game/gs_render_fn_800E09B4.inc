    nofralloc
    lfs f0, lbl_8047CAF0(r2)
    fcmpo cr0, f3, f0
    cror eq, lt, eq
    beqlr
    lfs f0, lbl_8047CAF4(r2)
    fcmpo cr0, f3, f0
    cror eq, gt, eq
    bne @L_800E09DC
    fmr f1, f2
    blr
    @L_800E09DC:
    fsubs f0, f2, f1
    fmadds f1, f3, f0, f1
    blr
