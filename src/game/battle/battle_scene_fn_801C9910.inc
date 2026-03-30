    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    li r3, 0x1
    bl fn_8012B5E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9958
    li r0, 0x0
    li r3, 0x1
    stw r0, -21600(r13)
    b @L_801C9B44
@L_801C9958:
    addi r5, r1, 0x5c
    li r3, 0x0
    li r4, 0x65
    bl fn_8018BDF4
    addi r5, r1, 0x50
    li r3, 0x0
    li r4, 0x64
    bl fn_8018BDF4
    lfs f1, -21848(r2)
    addi r5, r1, 0x50
    li r3, 0x1
    li r4, 0x0
    bl fn_8012B19C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C99C8
    lis r3, -32698
    lfs f2, 0x50(r1)
    addi r4, r3, 28816
    lfs f1, 0x54(r1)
    lfs f0, 0x58(r1)
    li r0, 0x1
    stw r0, -21600(r13)
    li r3, 0x1
    stfs f2, 0x0(r4)
    stfs f1, 0x4(r4)
    stfs f0, 0x8(r4)
    b @L_801C9B44
@L_801C99C8:
    addi r3, r1, 0x5c
    addi r4, r1, 0x50
    bl fn_800A3C00
    addi r3, r1, 0x50
    addi r4, r1, 0x5c
    addi r5, r1, 0x38
    bl fn_800A3A9C
    lfs f1, -21912(r2)
    addi r3, r1, 0x38
    addi r4, r1, 0x20
    bl fn_800A3AC0
    addi r3, r1, 0x5c
    addi r4, r1, 0x20
    addi r5, r1, 0x2c
    bl fn_800A3A78
    lfs f1, 0x50(r1)
    lfs f0, 0x5c(r1)
    lfs f2, -21900(r2)
    fcmpo cr0, f1, f0
    stfs f2, 0x18(r1)
    ble @L_801C9A40
    lfs f0, 0x40(r1)
    lfs f2, -21860(r2)
    fneg f0, f0
    lfs f1, 0x38(r1)
    fmuls f1, f2, f1
    fmuls f0, f2, f0
    stfs f1, 0x1c(r1)
    stfs f0, 0x14(r1)
    b @L_801C9A60
@L_801C9A40:
    lfs f0, 0x38(r1)
    lfs f2, -21860(r2)
    fneg f1, f0
    lfs f0, 0x40(r1)
    fmuls f0, f2, f0
    fmuls f1, f2, f1
    stfs f0, 0x14(r1)
    stfs f1, 0x1c(r1)
@L_801C9A60:
    addi r3, r1, 0x14
    mr r4, r3
    bl fn_800A3ADC
    lis r3, -32698
    lfs f30, -21920(r2)
    addi r31, r3, 28816
    li r30, 0x0
    fmr f31, f30
@L_801C9A80:
    fmr f1, f30
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    bl fn_800A3AC0
    addi r3, r1, 0x2c
    addi r4, r1, 0x8
    addi r5, r1, 0x44
    bl fn_800A3A78
    lfs f1, -21848(r2)
    addi r5, r1, 0x44
    li r3, 0x1
    li r4, 0x0
    bl fn_8012B19C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9B30
    lfs f1, 0x48(r1)
    lis r3, -32698
    lfs f0, 0x4c(r1)
    addi r6, r3, 28816
    lfs f2, 0x44(r1)
    addi r4, r1, 0x44
    stfs f1, 0x4(r31)
    addi r5, r1, 0x50
    lfs f1, -21848(r2)
    li r3, 0x1
    stfs f2, 0x0(r6)
    stfs f0, 0x8(r31)
    bl fn_8012B19C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9B30
    lis r3, -32698
    lfs f2, 0x50(r1)
    addi r4, r3, 28816
    lfs f1, 0x54(r1)
    lfs f0, 0x58(r1)
    li r0, 0x2
    stw r0, -21600(r13)
    li r3, 0x1
    stfs f2, 0xc(r4)
    stfs f1, 0x10(r4)
    stfs f0, 0x14(r4)
    b @L_801C9B44
@L_801C9B30:
    addi r30, r30, 0x1
    fadds f30, f30, f31
    cmpwi r30, 0xa
    blt @L_801C9A80
    li r3, 0x0
@L_801C9B44:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    lwz r31, 0x6c(r1)
    lwz r0, 0x94(r1)
    lwz r30, 0x68(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
