    nofralloc
    lfs f0, 0x0(r4)
    fneg f0, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x4(r4)
    fneg f0, f0
    stfs f0, 0x4(r3)
    lfs f0, 0x8(r4)
    fneg f0, f0
    stfs f0, 0x8(r3)
    blr
