    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80465608@ha
    stw r0, 0x14(r1)
    addi r4, r4, lbl_80465608@l
    lwz r12, 0x10(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
