    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x80(r1)
    stfd f31, 0x78(r1)
    stfd f30, 0x70(r1)
    stfd f29, 0x68(r1)
    stfd f28, 0x60(r1)
    stfd f27, 0x58(r1)
    stfd f26, 0x50(r1)
    stfd f25, 0x48(r1)
    stfd f24, 0x40(r1)
    stfd f23, 0x38(r1)
    stw r31, 0x34(r1)
    stw r30, 0x30(r1)
    stw r29, 0x2c(r1)
    lfd f1, -29408(r2)
    lfs f0, 0x3c(r5)
    fcmpu cr0, f1, f0
    bne @L_800BCB8C
    lfs f2, 0x28(r5)
    lfs f1, -29460(r2)
    lfs f3, 0x2c(r5)
    fsubs f2, f2, f1
    lfs f0, 0x8(r5)
    fadds f1, f1, f0
    lfs f0, 0x0(r5)
    fdivs f2, f3, f2
    fmuls f1, f2, f1
    fdivs f24, f1, f0
    b @L_800BCBB4
@L_800BCB8C:
    lfs f4, -29460(r2)
    lfs f0, 0xc(r5)
    lfs f3, 0x2c(r5)
    fsubs f1, f0, f4
    lfs f2, 0x28(r5)
    fadds f3, f4, f3
    lfs f0, 0x0(r5)
    fneg f1, f1
    fdivs f2, f3, f2
    fdivs f24, f1, f0
@L_800BCBB4:
    rlwinm r0, r4, 0, 16, 31
    lfd f27, -29384(r2)
    stw r0, 0x24(r1)
    lis r31, 0x4330
    lfs f1, -29440(r2)
    fmuls f26, f2, f2
    stw r31, 0x20(r1)
    lfs f28, -29460(r2)
    mr r30, r3
    lfd f0, 0x20(r1)
    lfs f29, -29464(r2)
    li r29, 0x0
    fsubs f0, f0, f27
    lfd f30, -29432(r2)
    lfd f31, -29400(r2)
    lfs f23, -29392(r2)
    fdivs f25, f1, f0
@L_800BCBF8:
    addi r0, r29, 0x1
    rlwinm r0, r0, 5, 0, 26
    stw r0, 0x24(r1)
    stw r31, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f27
    fmuls f0, f0, f25
    fmuls f0, f0, f24
    fmuls f0, f0, f0
    fdivs f0, f0, f26
    fadds f2, f28, f0
    fcmpo cr0, f2, f29
    ble @L_800BCC7C
    frsqrte f1, f2
    fmul f0, f1, f1
    fmul f1, f30, f1
    fmul f0, f2, f0
    fsub f0, f31, f0
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f30, f1
    fmul f0, f2, f0
    fsub f0, f31, f0
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f30, f1
    fmul f0, f2, f0
    fsub f0, f31, f0
    fmul f0, f1, f0
    fmul f0, f2, f0
    frsp f0, f0
    stfs f0, 0x14(r1)
    lfs f2, 0x14(r1)
@L_800BCC7C:
    fmuls f1, f23, f2
    bl fn_800C46B0
    addi r29, r29, 0x1
    rlwinm r0, r3, 0, 20, 31
    sth r0, 0x0(r30)
    cmplwi r29, 0xa
    addi r30, r30, 0x2
    blt @L_800BCBF8
    lwz r0, 0x84(r1)
    lfd f31, 0x78(r1)
    lfd f30, 0x70(r1)
    lfd f29, 0x68(r1)
    lfd f28, 0x60(r1)
    lfd f27, 0x58(r1)
    lfd f26, 0x50(r1)
    lfd f25, 0x48(r1)
    lfd f24, 0x40(r1)
    lfd f23, 0x38(r1)
    lwz r31, 0x34(r1)
    lwz r30, 0x30(r1)
    lwz r29, 0x2c(r1)
    addi r1, r1, 0x80
    mtlr r0
    blr
