    nofralloc
    cmpwi r3, 0x0
    beq @L_80054708
    lfs f0, lbl_8047BE68(r2)
    stfs f0, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    blr
    @L_80054708:
    lfs f1, lbl_8047BE60(r2)
    lfs f0, lbl_8047BE94(r2)
    stfs f1, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    blr
