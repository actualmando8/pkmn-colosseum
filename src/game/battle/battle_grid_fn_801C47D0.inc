    nofralloc
    lis r3, lbl_80466E30@ha
    li r0, 0x0
    addi r3, r3, lbl_80466E30@l
    stb r0, 0x0(r3)
    stb r0, 0x1(r3)
    sth r0, 0x2(r3)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x4(r3)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x8(r3)
    stw r0, 0xc(r3)
    stw r0, 0x10(r3)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x14(r3)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x18(r3)
    blr
