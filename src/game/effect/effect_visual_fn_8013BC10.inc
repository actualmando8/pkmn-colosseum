    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stw r31, 0x5c(r1)
    mr r31, r3
    lfs f2, lbl_8047D200(r2)
    lfs f0, 0x44(r3)
    fmr f30, f1
    addi r3, r1, 0x44
    fmuls f31, f2, f0
    bl fn_800E0204
    addi r3, r1, 0x38
    bl fn_800E0204
    addi r3, r1, 0x2c
    bl fn_800E0204
    addi r3, r1, 0x20
    bl fn_800E0204
    addi r3, r1, 0x14
    bl fn_800E0204
    addi r3, r1, 0x8
    bl fn_800E0204
    fneg f2, f31
    lfs f3, 0xc4(r31)
    lfs f1, lbl_8047D210(r2)
    stfs f3, 0x1c(r1)
    fcmpo cr0, f30, f1
    stfs f3, 0x10(r1)
    stfs f2, 0x4c(r1)
    stfs f31, 0x34(r1)
    lfs f4, 0xc8(r31)
    stfs f4, 0x3c(r1)
    bge @L_8013BCD0
    lfs f0, lbl_8047D214(r2)
    frsp f1, f2
    lfs f2, lbl_8047D200(r2)
    fmuls f30, f30, f0
    fmuls f3, f31, f30
    fmuls f0, f4, f30
    fadds f1, f1, f3
    stfs f3, 0x34(r1)
    stfs f0, 0x3c(r1)
    fmuls f0, f2, f1
    stfs f0, 0x40(r1)
    b @L_8013BD40
    @L_8013BCD0:
    lfs f0, lbl_8047D218(r2)
    fcmpo cr0, f30, f0
    bge @L_8013BD08
    fsubs f30, f30, f1
    lfs f1, lbl_8047D214(r2)
    lfs f0, lbl_8047D21C(r2)
    fmuls f30, f30, f1
    fsubs f0, f0, f30
    fmuls f1, f31, f30
    fmuls f0, f3, f0
    stfs f1, 0x40(r1)
    stfs f0, 0x10(r1)
    stfs f1, 0x28(r1)
    b @L_8013BD40
    @L_8013BD08:
    fsubs f30, f30, f0
    lfs f1, lbl_8047D214(r2)
    lfs f0, lbl_8047D21C(r2)
    stfs f31, 0x40(r1)
    fmuls f30, f30, f1
    fsubs f2, f0, f30
    fneg f1, f30
    fadds f0, f0, f30
    fmuls f2, f4, f2
    fmuls f1, f3, f1
    fmuls f0, f31, f0
    stfs f2, 0x3c(r1)
    stfs f1, 0x10(r1)
    stfs f0, 0x28(r1)
    @L_8013BD40:
    addi r3, r1, 0x20
    addi r5, r31, 0x24
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x44
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x38
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x2c
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
    addi r3, r31, 0x5c
    addi r4, r1, 0x44
    bl fn_800E01D0
    addi r3, r31, 0x68
    addi r4, r1, 0x38
    bl fn_800E01D0
    addi r3, r31, 0x8c
    addi r4, r1, 0x38
    bl fn_800E01D0
    addi r3, r31, 0x98
    addi r4, r1, 0x2c
    bl fn_800E01D0
    addi r3, r31, 0x74
    addi r4, r1, 0x14
    bl fn_800E01D0
    addi r3, r31, 0x80
    addi r4, r1, 0x14
    bl fn_800E01D0
    addi r3, r31, 0xa4
    addi r4, r1, 0x8
    bl fn_800E01D0
    addi r3, r31, 0xb0
    addi r4, r1, 0x14
    bl fn_800E01D0
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    lwz r0, 0x84(r1)
    lwz r31, 0x5c(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
