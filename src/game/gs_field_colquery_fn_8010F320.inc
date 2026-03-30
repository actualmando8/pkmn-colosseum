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
    stw r29, 0x34(r1)
    fmr f26, f1
    mr r29, r3
    mr r30, r4
    mr r31, r5
    bl fn_8010CBC0
    cmplwi r3, 0x0
    bne @L_8010F38C
    li r3, 0x0
    b @L_8010F46C
@L_8010F38C:
    mr r3, r29
    mr r4, r30
    bl fn_800A3C00
    lfs f29, -26528(r2)
    fcmpo cr0, f1, f29
    ble @L_8010F3B8
    fdivs f29, f26, f1
    lfs f0, -26524(r2)
    fcmpo cr0, f29, f0
    ble @L_8010F3B8
    fmr f29, f0
@L_8010F3B8:
    mr r3, r30
    mr r4, r29
    addi r5, r1, 0x8
    bl fn_800A3A9C
    lfs f27, -26528(r2)
    lfs f30, -26524(r2)
    fmr f31, f27
    b @L_8010F460
@L_8010F3D8:
    fadds f28, f27, f29
    fcmpo cr0, f28, f30
    ble @L_8010F3E8
    fmr f28, f30
@L_8010F3E8:
    fmr f1, f27
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800A3AC0
    addi r3, r1, 0x14
    mr r4, r29
    mr r5, r3
    bl fn_800A3A78
    fmr f1, f28
    addi r3, r1, 0x8
    addi r4, r1, 0x20
    bl fn_800A3AC0
    addi r3, r1, 0x20
    mr r4, r29
    mr r5, r3
    bl fn_800A3A78
    fmr f1, f26
    mr r5, r31
    addi r3, r1, 0x14
    addi r4, r1, 0x20
    li r6, 0x0
    bl fn_8010EFE4
    cmpwi r3, 0x0
    beq @L_8010F450
    li r3, 0x1
    b @L_8010F46C
@L_8010F450:
    fcmpo cr0, f29, f31
    cror eq, lt, eq
    beq @L_8010F468
    fadds f27, f27, f29
@L_8010F460:
    fcmpo cr0, f27, f30
    blt @L_8010F3D8
@L_8010F468:
    li r3, 0x0
@L_8010F46C:
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
    lwz r30, 0x38(r1)
    lwz r0, 0xa4(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr

