    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stfd f26, 0x50(r1)
    psq_st f26, 0x58(r1), 0, 0
    stfd f25, 0x40(r1)
    psq_st f25, 0x48(r1), 0, 0
    stmw r27, 0x2c(r1)
    mr r28, r5
    lis r0, 0x4330
    subi r5, r28, 0x1
    stw r0, 0x18(r1)
    lfd f1, lbl_8047CB00(r2)
    mr r29, r3
    stw r5, 0x1c(r1)
    mr r27, r4
    lfs f2, lbl_8047CAF8(r2)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fdivs f25, f2, f0
    bl fn_800E01D0
    lfd f29, lbl_8047CB08(r2)
    addi r29, r29, 0xc
    lfs f30, lbl_8047CAFC(r2)
    subi r31, r28, 0x1
    lfs f31, lbl_8047CAF8(r2)
    li r28, 0x1
    lis r30, 0x4330
    b @L_800E0B40
    @L_800E0A84:
    xoris r0, r28, 0x8000
    stw r30, 0x18(r1)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f29
    fmuls f27, f0, f25
    fcmpo cr0, f27, f30
    bge @L_800E0AA8
    fmr f27, f30
    @L_800E0AA8:
    fcmpo cr0, f27, f31
    ble @L_800E0AB4
    fmr f27, f31
    @L_800E0AB4:
    fmuls f28, f27, f27
    mr r3, r29
    addi r4, r27, 0x24
    fmuls f0, f28, f27
    fsubs f26, f0, f28
    fmr f1, f26
    bl fn_800E013C
    fsubs f28, f26, f28
    addi r3, r1, 0x8
    addi r4, r27, 0x18
    fadds f1, f28, f27
    bl fn_800E013C
    mr r3, r29
    mr r4, r29
    addi r5, r1, 0x8
    bl fn_800E019C
    fadds f28, f28, f26
    mr r4, r27
    addi r3, r1, 0x8
    fadds f1, f31, f28
    bl fn_800E013C
    mr r3, r29
    mr r4, r29
    addi r5, r1, 0x8
    bl fn_800E019C
    fneg f1, f28
    addi r3, r1, 0x8
    addi r4, r27, 0xc
    bl fn_800E013C
    mr r3, r29
    mr r4, r29
    addi r5, r1, 0x8
    bl fn_800E019C
    addi r28, r28, 0x1
    addi r29, r29, 0xc
    @L_800E0B40:
    cmplw r28, r31
    blt @L_800E0A84
    mr r3, r29
    addi r4, r27, 0xc
    bl fn_800E01D0
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    psq_l f26, 0x58(r1), 0, 0
    lfd f26, 0x50(r1)
    psq_l f25, 0x48(r1), 0, 0
    lfd f25, 0x40(r1)
    lmw r27, 0x2c(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
