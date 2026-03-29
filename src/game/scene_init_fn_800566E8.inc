    nofralloc
    lfs f1, lbl_8047BEC0(r2)
    lfs f0, lbl_8047A578(r13)
    fcmpu cr0, f1, f0
    mfcr r0
    extrwi r0, r0, 1, 2
    xori r3, r0, 0x1
    blr
