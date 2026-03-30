    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, lbl_80315568@ha
    li r5, 0x30
    stw r0, 0x34(r1)
    addi r4, r4, lbl_80315568@l
    stfd f31, 0x28(r1)
    fmr f31, f3
    stfd f30, 0x20(r1)
    fmr f30, f2
    stfd f29, 0x18(r1)
    fmr f29, f1
    stw r31, 0x14(r1)
    mr r31, r3
    bl memcpy
    stfs f29, 0xc(r31)
    stfs f30, 0x1c(r31)
    stfs f31, 0x2c(r31)
    lfd f31, 0x28(r1)
    lfd f30, 0x20(r1)
    lfd f29, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
