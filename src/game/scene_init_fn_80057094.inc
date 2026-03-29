    nofralloc
    stwu r1, -0x20(r1)
    lis r5, lbl_803A9768@ha
    addi r5, r5, lbl_803A9768@l
    lfs f0, 0x27c(r5)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x0(r3)
    lfs f0, 0x280(r5)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x0(r4)
    addi r1, r1, 0x20
    blr
