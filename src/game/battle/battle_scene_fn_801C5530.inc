    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stfd f30, 0x20(r1)
    psq_st f30, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    mr r31, r4
    fdivs f31, f1, f2
    fdivs f30, f3, f4
    fmr f1, f31
    bl fn_801C6688
    cmplwi r31, 0x0
    bne @L_801C557C
    mr r3, r30
    b @L_801C55B0
@L_801C557C:
    lfs f2, -22208(r2)
    fmr f3, f31
    lfs f1, -22168(r2)
    fmr f4, f30
    lfs f0, -22164(r2)
    mr r3, r31
    stfs f1, 0x8(r1)
    lfs f1, -22204(r2)
    addi r4, r1, 0x8
    stfs f0, 0xc(r1)
    stfs f2, 0x10(r1)
    bl fn_801C63C0
    mr r3, r30
@L_801C55B0:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    psq_l f30, 0x28(r1), 0, 0
    lfd f30, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x44(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
