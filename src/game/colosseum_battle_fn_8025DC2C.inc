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
    lfs f0, -20540(r2)
    lis r31, 0x4330
    lfs f29, -20544(r2)
    fmuls f30, f0, f1
    lfd f31, -20536(r2)
    b @L_8025DC88
@L_8025DC6C:
    bl fn_800F0308
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f31
    fadds f29, f29, f0
@L_8025DC88:
    fcmpo cr0, f29, f30
    blt @L_8025DC6C
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    lwz r0, 0x54(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
