    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stfd f29, 0x30(r1)
    psq_st f29, 0x38(r1), 0, 0
    stfd f28, 0x20(r1)
    psq_st f28, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    fmr f28, f1
    lfs f0, lbl_8047CAE4(r2)
    mr r30, r3
    mr r31, r4
    fcmpo cr0, f28, f0
    bge @L_800E0834
    fmr f28, f0
    @L_800E0834:
    lfs f0, lbl_8047CAE0(r2)
    fcmpo cr0, f28, f0
    ble @L_800E0844
    fmr f28, f0
    @L_800E0844:
    lfs f0, lbl_8047CAE0(r2)
    fmuls f31, f28, f28
    mr r3, r30
    mr r4, r31
    fsubs f30, f0, f28
    fmuls f29, f30, f30
    fmuls f1, f29, f30
    bl fn_800E013C
    lfs f0, lbl_8047CAE8(r2)
    addi r3, r1, 0x8
    addi r4, r31, 0x18
    fmuls f0, f0, f28
    fmuls f1, f0, f29
    bl fn_800E013C
    mr r3, r30
    mr r4, r30
    addi r5, r1, 0x8
    bl fn_800E019C
    lfs f0, lbl_8047CAE8(r2)
    addi r3, r1, 0x8
    addi r4, r31, 0x24
    fmuls f0, f0, f31
    fmuls f1, f0, f30
    bl fn_800E013C
    mr r3, r30
    mr r4, r30
    addi r5, r1, 0x8
    bl fn_800E019C
    fmuls f1, f31, f28
    addi r3, r1, 0x8
    addi r4, r31, 0xc
    bl fn_800E013C
    mr r3, r30
    mr r4, r30
    addi r5, r1, 0x8
    bl fn_800E019C
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    psq_l f29, 0x38(r1), 0, 0
    lfd f29, 0x30(r1)
    psq_l f28, 0x28(r1), 0, 0
    lfd f28, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x64(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
