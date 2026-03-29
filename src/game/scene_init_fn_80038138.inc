    nofralloc
    lfs f1, lbl_8047BA78(r2)
    lfs f0, lbl_8047A494(r13)
    lfs f2, lbl_8047A478(r13)
    fmuls f1, f1, f0
    lfs f0, lbl_8047BA60(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A478(r13)
    ble @L_80038164
    fsubs f0, f1, f0
    stfs f0, lbl_8047A478(r13)
    @L_80038164:
    lfs f0, lbl_8047A478(r13)
    stfs f0, 0x70(r4)
    blr
