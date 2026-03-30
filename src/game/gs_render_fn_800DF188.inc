    nofralloc
    stwu r1, -0x10(r1)
    lfs f1, lbl_8047CACC(r2)
    lwz r4, 0x8(r3)
    lwz r4, 0xc(r4)
    lfs f0, 0xc(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1(r3)
    addi r1, r1, 0x10
    blr
