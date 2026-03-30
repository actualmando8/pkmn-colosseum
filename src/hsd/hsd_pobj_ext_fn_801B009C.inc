    nofralloc
    cmplwi r3, 0x0
    beqlr
    cmpwi r4, 0x1
    bnelr
    lfs f1, 0x0(r5)
    lfd f0, -22792(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801B00D0
    lwz r0, 0x4(r3)
    oris r0, r0, 0x8000
    stw r0, 0x4(r3)
    blr
@L_801B00D0:
    lwz r0, 0x4(r3)
    rlwinm r0, r0, 0, 1, 31
    stw r0, 0x4(r3)
    blr
