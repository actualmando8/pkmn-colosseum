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
    stw r31, 0x6c(r1)
    mr r31, r3
    li r3, 0x0
    lbz r0, 0x48(r31)
    cmplwi r0, 0x0
    beq @L_801BE300
    lbz r0, 0x49(r31)
    cmplwi r0, 0x0
    beq @L_801BE300
    li r3, 0x1
@L_801BE300:
    cmpwi r3, 0x0
    bne @L_801BE31C
    lis r4, lbl_802756C4@ha
    addi r3, r2, -22512
    addi r5, r4, lbl_802756C4@l
    li r4, 0x267
    bl fn_80196E10
@L_801BE31C:
    lfs f1, 0x28(r31)
    lfs f0, -22432(r2)
    fabs f1, f1
    frsp f1, f1
    fcmpo cr0, f1, f0
    bge @L_801BE33C
    lfs f31, -22464(r2)
    b @L_801BE360
@L_801BE33C:
    lbz r3, 0x48(r31)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    lfd f2, -22424(r2)
    stw r3, 0x4c(r1)
    lfs f0, 0x28(r31)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fdivs f31, f1, f0
@L_801BE360:
    lfs f1, 0x2c(r31)
    lfs f0, -22432(r2)
    fabs f1, f1
    frsp f1, f1
    fcmpo cr0, f1, f0
    bge @L_801BE380
    lfs f30, -22464(r2)
    b @L_801BE3A4
@L_801BE380:
    lbz r3, 0x49(r31)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    lfd f2, -22424(r2)
    stw r3, 0x54(r1)
    lfs f0, 0x2c(r31)
    lfd f1, 0x50(r1)
    fsubs f1, f1, f2
    fdivs f30, f1, f0
@L_801BE3A4:
    lfs f29, 0x30(r31)
    lfs f0, 0x18(r31)
    stfs f0, 0x8(r1)
    lfs f0, 0x1c(r31)
    stfs f0, 0xc(r1)
    lfs f0, 0x20(r31)
    fneg f0, f0
    stfs f0, 0x10(r1)
    lfs f0, 0x34(r31)
    lwz r0, 0x44(r31)
    fneg f1, f0
    lfs f5, 0x38(r31)
    cmpwi r0, 0x2
    bne @L_801BE40C
    lbz r3, 0x49(r31)
    lis r0, 0x4330
    stw r0, 0x58(r1)
    lfd f3, -22424(r2)
    stw r3, 0x5c(r1)
    lfs f0, 0x2c(r31)
    lfd f2, 0x58(r1)
    lfs f4, -22460(r2)
    fsubs f2, f2, f3
    fdivs f0, f2, f0
    fdivs f0, f4, f0
    b @L_801BE410
@L_801BE40C:
    lfs f0, -22464(r2)
@L_801BE410:
    fadds f0, f5, f0
    lfs f3, 0x3c(r31)
    addi r3, r31, 0x74
    fneg f2, f0
    bl fn_800A32B4
    addi r3, r1, 0x18
    addi r4, r1, 0x8
    bl fn_801A8B94
    addi r3, r1, 0x18
    addi r4, r31, 0x74
    addi r5, r31, 0x74
    bl fn_800A2D98
    fmr f1, f31
    addi r3, r1, 0x18
    fmr f2, f30
    fmr f3, f29
    bl fn_800A3334
    addi r3, r1, 0x18
    addi r4, r31, 0x74
    addi r5, r31, 0x74
    bl fn_800A2D98
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    lwz r0, 0xa4(r1)
    lwz r31, 0x6c(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr

