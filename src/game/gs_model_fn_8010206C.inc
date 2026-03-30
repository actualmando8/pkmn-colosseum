    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    fmr f31, f1
    li r3, 0x1
    bl fn_8010977C
    li r3, 0x1
    bl fn_801096F8
    li r3, 0x0
    bl fn_801096E8
    fmr f2, f31
    lfs f1, lbl_8047CDC4(r13)
    bl fn_801096AC
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
