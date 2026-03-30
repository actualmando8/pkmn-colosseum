    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stfd f30, 0x10(r1)
    psq_st f30, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    fmr f30, f1
    lis r3, -32698
    lfs f0, 0x0(r30)
    addi r3, r3, 28752
    lfs f31, 0x1c(r3)
    fdivs f1, f0, f31
    bl fn_800C46B0
    lfs f0, 0x4(r30)
    mr r31, r3
    fdivs f1, f0, f31
    bl fn_800C46B0
    fmr f1, f30
    mr r30, r3
    mr r3, r31
    mr r4, r30
    bl fn_801C6E90
    lfs f0, -21984(r2)
    subi r3, r31, 0x1
    subi r4, r30, 0x1
    fmuls f30, f30, f0
    fmr f1, f30
    bl fn_801C6E90
    fmr f1, f30
    mr r3, r31
    subi r4, r30, 0x1
    bl fn_801C6E90
    fmr f1, f30
    addi r3, r31, 0x1
    subi r4, r30, 0x1
    bl fn_801C6E90
    fmr f1, f30
    mr r4, r30
    subi r3, r31, 0x1
    bl fn_801C6E90
    fmr f1, f30
    mr r4, r30
    addi r3, r31, 0x1
    bl fn_801C6E90
    fmr f1, f30
    subi r3, r31, 0x1
    addi r4, r30, 0x1
    bl fn_801C6E90
    fmr f1, f30
    mr r3, r31
    addi r4, r30, 0x1
    bl fn_801C6E90
    fmr f1, f30
    addi r3, r31, 0x1
    addi r4, r30, 0x1
    bl fn_801C6E90
    psq_l f31, 0x28(r1), 0, 0
    lfd f31, 0x20(r1)
    psq_l f30, 0x18(r1), 0, 0
    lfd f30, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r0, 0x34(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
