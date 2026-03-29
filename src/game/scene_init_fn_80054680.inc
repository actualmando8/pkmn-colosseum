    nofralloc
    lfs f1, lbl_8047A554(r13)
    lfs f0, lbl_8047BE68(r2)
    fcmpo cr0, f1, f0
    ble @L_80054698
    li r3, 0x3
    blr
    @L_80054698:
    bge @L_800546A4
    li r3, 0x2
    blr
    @L_800546A4:
    lfs f1, lbl_8047A558(r13)
    lfs f0, lbl_8047BE60(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    mfcr r0
    extrwi r3, r0, 1, 2
    blr
