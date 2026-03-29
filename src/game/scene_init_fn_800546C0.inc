    nofralloc
    cmpwi r3, 0x0
    beq @L_800546DC
    lfs f1, lbl_8047BE60(r2)
    lfs f0, lbl_8047BE68(r2)
    stfs f1, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    blr
    @L_800546DC:
    lfs f1, lbl_8047BE68(r2)
    lfs f0, lbl_8047BE90(r2)
    stfs f1, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    blr
