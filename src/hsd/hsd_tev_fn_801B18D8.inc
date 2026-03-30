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
    lfs f0, -22688(r2)
    mr r30, r3
    lfs f31, -22688(r2)
    li r31, 0x0
    fcmpo cr0, f1, f0
    lfs f30, -22684(r2)
    cror eq, lt, eq
    bne @L_801B1934
    lfs f1, -22688(r2)
    b @L_801B1A98
@L_801B1934:
    lfs f0, -22684(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801B1950
    lfs f1, -22684(r2)
    b @L_801B1A98
@L_801B194C:
    addi r31, r31, 0x1
@L_801B1950:
    lwz r3, 0x10(r30)
    rlwinm r0, r31, 2, 0, 29
    add r3, r3, r0
    lfs f0, 0x4(r3)
    fcmpo cr0, f0, f1
    blt @L_801B194C
    lbz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_801B1984
    blt @L_801B1A4C
    cmpwi r0, 0x4
    bge @L_801B1A4C
    b @L_801B19BC
@L_801B1984:
    lwz r3, 0x10(r30)
    rlwinm r0, r31, 2, 0, 29
    lwz r6, 0x10(r30)
    rlwinm r5, r31, 2, 0, 29
    add r4, r3, r0
    lwz r3, 0x10(r30)
    rlwinm r0, r31, 2, 0, 29
    lfsx f3, r6, r5
    lfs f2, 0x4(r4)
    lfsx f0, r3, r0
    fsubs f1, f1, f3
    fsubs f0, f2, f0
    fdivs f29, f1, f0
    b @L_801B1A4C
@L_801B19BC:
    lwz r3, 0x10(r30)
    rlwinm r0, r31, 2, 0, 29
    lfs f2, 0xc(r30)
    lfsx f0, r3, r0
    fsubs f0, f1, f0
    fmuls f28, f2, f0
    b @L_801B1A1C
@L_801B19D8:
    fadds f2, f31, f30
    lfs f0, -22680(r2)
    mulli r0, r31, 0x14
    lwz r3, 0x14(r30)
    fmr f1, f31
    fmuls f29, f2, f0
    add r3, r3, r0
    fmr f2, f29
    bl fn_801B1AD0
    lfs f0, -22676(r2)
    fadds f0, f0, f1
    fcmpo cr0, f28, f0
    bge @L_801B1A14
    fmr f30, f29
    b @L_801B1A1C
@L_801B1A14:
    fmr f31, f29
    fsubs f28, f28, f1
@L_801B1A1C:
    fsubs f1, f31, f30
    lfs f0, -22688(r2)
    fcmpo cr0, f1, f0
    bge @L_801B1A38
    fsubs f0, f31, f30
    fneg f1, f0
    b @L_801B1A3C
@L_801B1A38:
    fsubs f1, f31, f30
@L_801B1A3C:
    lfs f0, -22676(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_801B19D8
@L_801B1A4C:
    lha r3, 0x2(r30)
    xoris r5, r31, 0x8000
    lis r4, 0x4330
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r5, 0xc(r1)
    lfd f1, -22672(r2)
    stw r4, 0x8(r1)
    lfd f2, -22672(r2)
    lfd f0, 0x8(r1)
    stw r3, 0x14(r1)
    fsubs f3, f0, f1
    lfs f0, -22684(r2)
    stw r0, 0x10(r1)
    lfd f1, 0x10(r1)
    fadds f3, f29, f3
    fsubs f1, f1, f2
    fsubs f0, f1, f0
    fdivs f1, f3, f0
@L_801B1A98:
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
