    nofralloc
    stwu r1, -0x10(r1)
    lfs f0, 0x4(r3)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    lfd f2, lbl_8047D0E0(r2)
    stfs f0, 0xc(r3)
    lbz r0, 0x42(r3)
    lbz r4, 0x23(r3)
    extsb r0, r0
    lfs f3, 0x64(r3)
    add r0, r4, r0
    lfs f0, 0x10(r3)
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fmadds f0, f3, f1, f0
    stfs f0, 0x10(r3)
    li r3, 0x0
    addi r1, r1, 0x10
    blr
