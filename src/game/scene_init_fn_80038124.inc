    nofralloc
    lfs f1, lbl_8047BA74(r2)
    lfs f0, lbl_8047A478(r13)
    fsubs f0, f1, f0
    stfs f0, 0x70(r4)
    blr
