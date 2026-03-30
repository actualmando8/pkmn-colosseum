    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stfd f29, 0x20(r1)
    psq_st f29, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lhz r0, 0x152(r3)
    fmr f29, f1
    cmplwi r0, 0x0
    beq @L_800E68A0
    lhz r29, 0x150(r3)
    li r30, 0x0
    lwz r28, 0x14c(r3)
    lis r31, 0x4330
    lfd f30, lbl_8047CB90(r2)
    lfs f31, lbl_8047CB88(r2)
    b @L_800E6898
    @L_800E6864:
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_800E6890
    lbz r0, 0x1(r3)
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f30
    fmuls f0, f0, f29
    fdivs f1, f0, f31
    bl fn_800DF21C
    @L_800E6890:
    addi r30, r30, 0x1
    addi r28, r28, 0x4
    @L_800E6898:
    cmpw r30, r29
    blt @L_800E6864
    @L_800E68A0:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x54(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
