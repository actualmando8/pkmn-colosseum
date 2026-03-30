    nofralloc
    stwu r1, -0x10(r1)
    stw r0, 0x8(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AC38(r13)
    mtctr r4
    lwz r3, lbl_8047AC3C(r13)
    lfs f8, 0x1c(r3)
    lfs f7, 0x18(r3)
    lfs f6, 0x14(r3)
    lfs f5, 0x10(r3)
    lfs f4, 0xc(r3)
    lfs f3, 0x8(r3)
    lfs f2, 0x4(r3)
    lfs f1, 0x0(r3)
    lwz r3, lbl_8047AC40(r13)
    lwz r10, 0x1c(r3)
    lwz r9, 0x18(r3)
    lwz r8, 0x14(r3)
    lwz r7, 0x10(r3)
    lwz r6, 0xc(r3)
    lwz r5, 0x8(r3)
    lwz r4, 0x4(r3)
    lwz r3, 0x0(r3)
    crxor 6, 6, 6
    bctrl
    lwz r0, 0x14(r1)
    mtlr r0
    lwz r0, 0x8(r1)
    addi r1, r1, 0x10
    blr
