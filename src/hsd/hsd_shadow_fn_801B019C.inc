    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stfd f29, 0x70(r1)
    psq_st f29, 0x78(r1), 0, 0
    stfd f28, 0x60(r1)
    psq_st f28, 0x68(r1), 0, 0
    stfd f27, 0x50(r1)
    psq_st f27, 0x58(r1), 0, 0
    stfd f26, 0x40(r1)
    psq_st f26, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    mr. r30, r3
    fmr f29, f1
    fmr f30, f2
    mr r31, r4
    fmr f31, f3
    fmr f26, f4
    bne @L_801B0210
    lis r3, 0x8027
    li r4, 0x3a2
    addi r3, r3, 21184
    addi r5, r2, -22760
    bl fn_80196E10
@L_801B0210:
    cmplwi r31, 0x0
    bne @L_801B0230
    lis r3, 0x8027
    lis r5, 0x8027
    addi r3, r3, 21184
    li r4, 0x3a3
    addi r5, r5, 21196
    bl fn_80196E10
@L_801B0230:
    mr r3, r31
    mr r4, r30
    addi r5, r1, 0x20
    bl fn_800A3A9C
    addi r3, r1, 0x20
    addi r4, r30, 0x30
    bl fn_800A3B7C
    lwz r0, 0x50(r30)
    cmpwi r0, 0x0
    beq @L_801B02C0
    lfs f0, -22752(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    beq @L_801B0358
    lfs f0, 0x3c(r30)
    addi r3, r1, 0x20
    addi r4, r1, 0x20
    fdivs f27, f0, f1
    fmr f1, f27
    bl fn_800A3AC0
    addi r3, r1, 0x20
    addi r4, r30, 0x24
    addi r5, r1, 0x14
    bl fn_800A3A9C
    addi r3, r30, 0x18
    addi r4, r1, 0x14
    bl fn_800A3B7C
    fmr f28, f1
    addi r3, r30, 0xc
    addi r4, r1, 0x14
    bl fn_800A3B7C
    fmuls f29, f29, f27
    fmuls f30, f30, f27
    fmuls f31, f31, f27
    fmuls f26, f26, f27
    b @L_801B02F8
@L_801B02C0:
    addi r3, r30, 0x30
    addi r4, r1, 0x8
    bl fn_800A3AC0
    addi r3, r1, 0x20
    addi r4, r1, 0x8
    addi r5, r1, 0x14
    bl fn_800A3A9C
    addi r3, r30, 0x18
    addi r4, r1, 0x14
    bl fn_800A3B7C
    fmr f28, f1
    addi r3, r30, 0xc
    addi r4, r1, 0x14
    bl fn_800A3B7C
@L_801B02F8:
    fadds f2, f28, f26
    lfs f0, 0x4c(r30)
    fcmpo cr0, f2, f0
    ble @L_801B0310
    fadds f0, f28, f26
    stfs f0, 0x4c(r30)
@L_801B0310:
    fadds f2, f28, f31
    lfs f0, 0x48(r30)
    fcmpo cr0, f2, f0
    bge @L_801B0328
    fadds f0, f28, f31
    stfs f0, 0x48(r30)
@L_801B0328:
    fadds f2, f1, f29
    lfs f0, 0x40(r30)
    fcmpo cr0, f2, f0
    ble @L_801B0340
    fadds f0, f1, f29
    stfs f0, 0x40(r30)
@L_801B0340:
    fadds f2, f1, f30
    lfs f0, 0x44(r30)
    fcmpo cr0, f2, f0
    bge @L_801B0358
    fadds f0, f1, f30
    stfs f0, 0x44(r30)
@L_801B0358:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    psq_l f28, 0x68(r1), 0, 0
    lfd f28, 0x60(r1)
    psq_l f27, 0x58(r1), 0, 0
    lfd f27, 0x50(r1)
    psq_l f26, 0x48(r1), 0, 0
    lfd f26, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r0, 0xa4(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
