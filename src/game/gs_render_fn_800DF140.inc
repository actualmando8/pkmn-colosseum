    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lfd f2, lbl_8047CAD0(r2)
    stw r0, 0x14(r1)
    lis r0, 0x4330
    lfs f0, lbl_8047CACC(r2)
    lbz r4, 0x1(r3)
    stw r0, 0x8(r1)
    lwz r3, 0x8(r3)
    stw r4, 0xc(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f0
    bl fn_801A6DDC
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
