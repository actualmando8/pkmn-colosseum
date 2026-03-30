    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stfd f26, 0x60(r1)
    psq_st f26, 0x68(r1), 0, 0
    stfd f25, 0x50(r1)
    psq_st f25, 0x58(r1), 0, 0
    stfd f24, 0x40(r1)
    psq_st f24, 0x48(r1), 0, 0
    stfd f23, 0x30(r1)
    psq_st f23, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r30, r4
    mr r29, r3
    lfs f1, 0x54(r4)
    addi r3, r30, 0x30
    lfs f0, lbl_8047D160(r2)
    addi r4, r30, 0x40
    lfs f31, 0x20(r29)
    fsubs f0, f1, f0
    lfs f30, 0x18(r29)
    lfs f29, 0x24(r29)
    lfs f28, 0x1c(r29)
    stfs f0, 0x54(r30)
    bl fn_800E076C
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f26, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f27, f0, f1
    fmr f1, f27
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    fmr f1, f27
    bl fn_800CDBE0
    frsp f27, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f0, f1
    addi r3, r1, 0x8
    fmr f2, f27
    fmuls f3, f25, f24
    fmuls f1, f25, f0
    bl fn_800E01F4
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    addi r3, r30, 0x40
    addi r4, r1, 0x8
    fmuls f1, f0, f1
    bl fn_800E0718
    mr r3, r30
    addi r4, r30, 0xc
    bl fn_800E01D0
    addi r31, r30, 0x18
    addi r4, r30, 0x24
    mr r3, r31
    bl fn_800E01D0
    mr r3, r31
    mr r4, r31
    addi r5, r30, 0xc
    bl fn_800E0168
    mr r3, r31
    mr r4, r31
    bl fn_800E0060
    bl fn_800E0BA0
    fmadds f1, f29, f1, f31
    lfs f0, 0x50(r30)
    mr r3, r31
    mr r4, r31
    fmuls f1, f1, f0
    fneg f1, f1
    bl fn_800E013C
    mr r3, r31
    mr r4, r31
    mr r5, r30
    bl fn_800E019C
    addi r31, r30, 0xc
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f23, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f26, f0, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BA0
    fmadds f27, f28, f1, f30
    lfs f0, 0x50(r30)
    fmr f1, f23
    fmuls f27, f27, f0
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f23
    bl fn_800CDBE0
    frsp f0, f1
    mr r3, r31
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    fmr f1, f27
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    lfs f1, 0x4(r31)
    mr r3, r31
    lfs f0, 0x28(r29)
    mr r4, r31
    mr r5, r30
    fadds f0, f1, f0
    stfs f0, 0x4(r31)
    bl fn_800E019C
    addi r31, r30, 0x24
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f28, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f26, f0, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BA0
    fmadds f27, f29, f1, f31
    lfs f0, 0x50(r30)
    fmr f1, f28
    fmuls f27, f27, f0
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f28
    bl fn_800CDBE0
    frsp f0, f1
    mr r3, r31
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    fmr f1, f27
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    mr r3, r31
    mr r4, r31
    addi r5, r30, 0xc
    bl fn_800E019C
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    psq_l f26, 0x68(r1), 0, 0
    lfd f26, 0x60(r1)
    psq_l f25, 0x58(r1), 0, 0
    lfd f25, 0x50(r1)
    psq_l f24, 0x48(r1), 0, 0
    lfd f24, 0x40(r1)
    psq_l f23, 0x38(r1), 0, 0
    lfd f23, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0xc4(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
