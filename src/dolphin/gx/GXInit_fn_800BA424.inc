    nofralloc
    fneg f4, f1
    fneg f1, f2
    fneg f0, f3
    stfs f4, 0x34(r3)
    stfs f1, 0x38(r3)
    stfs f0, 0x3c(r3)
    blr
