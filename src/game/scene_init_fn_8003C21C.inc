    nofralloc
    stwu r1, -0x10(r1)
    lis r4, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r4, r4, lbl_803A6748@l
    lfs f0, 0x44(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    addi r1, r1, 0x10
    blr
