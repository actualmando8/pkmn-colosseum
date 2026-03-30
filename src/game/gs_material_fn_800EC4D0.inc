    nofralloc
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800EC508
    cmplwi r4, 0x0
    beq @L_800EC4F4
    lfs f1, lbl_8047CC60(r2)
    lfs f0, 0xa0(r3)
    fadds f0, f1, f0
    stfs f0, 0x0(r4)
    @L_800EC4F4:
    cmplwi r5, 0x0
    beqlr
    lfs f0, lbl_8047CC64(r2)
    stfs f0, 0x0(r5)
    blr
    @L_800EC508:
    cmplwi r4, 0x0
    beq @L_800EC520
    lfs f1, lbl_8047CC60(r2)
    lfs f0, 0xcc(r3)
    fadds f0, f1, f0
    stfs f0, 0x0(r4)
    @L_800EC520:
    cmplwi r5, 0x0
    beqlr
    lfs f1, lbl_8047CC60(r2)
    lfs f0, 0xd0(r3)
    fadds f0, f1, f0
    stfs f0, 0x0(r5)
    blr
