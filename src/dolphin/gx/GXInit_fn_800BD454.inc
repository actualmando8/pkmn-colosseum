    nofralloc
    stwu r1, -0x18(r1)
    lis r0, 0x4330
    lwz r5, -29632(r2)
    lfd f1, -29360(r2)
    lwz r4, 0x420(r5)
    stw r4, 0x14(r1)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r3)
    lfs f0, 0x424(r5)
    stfs f0, 0x4(r3)
    lfs f0, 0x428(r5)
    stfs f0, 0x8(r3)
    lfs f0, 0x42c(r5)
    stfs f0, 0xc(r3)
    lfs f0, 0x430(r5)
    stfs f0, 0x10(r3)
    lfs f0, 0x434(r5)
    stfs f0, 0x14(r3)
    lfs f0, 0x438(r5)
    stfs f0, 0x18(r3)
    addi r1, r1, 0x18
    blr
