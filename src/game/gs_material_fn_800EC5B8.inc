    nofralloc
    lwz r4, 0x0(r3)
    rlwinm. r0, r4, 0, 29, 29
    beqlr
    rlwinm. r0, r4, 0, 24, 24
    bne @L_800EC5D0
    blr
    @L_800EC5D0:
    lfs f0, lbl_8047CC5C(r2)
    fcmpo cr0, f1, f0
    bge @L_800EC5E4
    fmr f1, f0
    b @L_800EC5F4
    @L_800EC5E4:
    lfs f0, lbl_8047CC60(r2)
    fcmpo cr0, f1, f0
    ble @L_800EC5F4
    fmr f1, f0
    @L_800EC5F4:
    stfs f1, 0xd4(r3)
    blr
