    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    bl fn_801ADC7C
    fmr f31, f1
    bl fn_801ADC7C
    fadds f1, f1, f31
    lfs f0, lbl_8047CB10(r2)
    fsubs f1, f1, f0
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
