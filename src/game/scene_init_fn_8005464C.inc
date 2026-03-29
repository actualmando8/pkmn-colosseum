    nofralloc
    lfs f1, lbl_8047A54C(r13)
    lfs f0, lbl_8047BE8C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    mfcr r0
    extrwi r0, r0, 1, 2
    cntlzw r0, r0
    srwi r3, r0, 5
    blr
