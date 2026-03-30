    nofralloc
    lis r3, -32698
    lfs f3, -22012(r2)
    addi r3, r3, 28720
    lfs f2, 0x10(r3)
    lfs f1, 0x14(r3)
    lfs f0, 0x18(r3)
    fdivs f2, f2, f3
    fdivs f1, f1, f3
    stfs f2, 0x10(r3)
    fdivs f0, f0, f3
    stfs f1, 0x14(r3)
    stfs f0, 0x18(r3)
    blr
