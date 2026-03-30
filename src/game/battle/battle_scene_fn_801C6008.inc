    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    mr r28, r3
    mr r29, r4
    fdivs f31, f1, f2
    lis r3, -32698
    addi r30, r3, 28720
    lfs f0, -22208(r2)
    lfs f2, 0x10(r30)
    lfs f1, 0x14(r30)
    fadds f1, f1, f2
    lhz r3, 0x8(r30)
    fcmpo cr0, f2, f0
    addi r0, r3, 0x1
    sth r0, 0x8(r30)
    stfs f1, 0x14(r30)
    cror eq, gt, eq
    bne @L_801C6098
    lfs f0, -22212(r2)
    lfs f3, 0x18(r30)
    fmadds f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r30)
    cror eq, gt, eq
    bne @L_801C60B8
    stfs f3, 0x14(r30)
    b @L_801C60B8
@L_801C6098:
    lfs f0, -22212(r2)
    lfs f3, 0x18(r30)
    fnmsubs f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r30)
    cror eq, lt, eq
    bne @L_801C60B8
    stfs f3, 0x14(r30)
@L_801C60B8:
    lfs f2, -22208(r2)
    fmr f3, f31
    lfs f1, -22168(r2)
    mr r3, r29
    lfs f0, -22164(r2)
    fmr f4, f2
    stfs f1, 0x8(r1)
    lfs f1, -22204(r2)
    addi r4, r1, 0x8
    stfs f0, 0xc(r1)
    stfs f2, 0x10(r1)
    bl fn_801C63C0
    lis r3, -32698
    addi r31, r3, 28720
    lhz r0, 0xc(r31)
    cmpwi r0, 0x4
    beq @L_801C613C
    bge @L_801C6114
    cmpwi r0, 0x3
    bge @L_801C6154
    cmpwi r0, 0x1
    bge @L_801C6120
    b @L_801C6154
@L_801C6114:
    cmpwi r0, 0x8
    beq @L_801C613C
    b @L_801C6154
@L_801C6120:
    lfs f2, -22168(r2)
    lfs f1, 0x14(r30)
    lfs f0, -22164(r2)
    fadds f1, f2, f1
    stfs f0, 0xc(r1)
    stfs f1, 0x8(r1)
    b @L_801C6154
@L_801C613C:
    lfs f1, -22164(r2)
    lfs f0, 0x14(r30)
    lfs f2, -22168(r2)
    fadds f0, f1, f0
    stfs f2, 0x8(r1)
    stfs f0, 0xc(r1)
@L_801C6154:
    lhz r3, 0x8(r30)
    lis r0, 0x4330
    lfs f1, -22204(r2)
    fmr f3, f31
    stw r3, 0x1c(r1)
    mr r3, r29
    fsubs f0, f1, f31
    lfs f2, -22052(r2)
    stw r0, 0x18(r1)
    lfd f5, -22176(r2)
    addi r4, r1, 0x8
    lfd f4, 0x18(r1)
    fmadds f0, f2, f0, f1
    lfs f6, -22208(r2)
    fsubs f2, f4, f5
    lfs f4, -22048(r2)
    stfs f6, 0x10(r1)
    fdivs f30, f2, f0
    fmr f2, f30
    bl fn_801C63C0
    lhz r0, 0xc(r31)
    cmpwi r0, 0x4
    beq @L_801C61EC
    bge @L_801C61C8
    cmpwi r0, 0x3
    bge @L_801C6200
    cmpwi r0, 0x1
    bge @L_801C61D4
    b @L_801C6200
@L_801C61C8:
    cmpwi r0, 0x8
    beq @L_801C61EC
    b @L_801C6200
@L_801C61D4:
    lfs f2, -22044(r2)
    lfs f1, 0x14(r30)
    lfs f0, -22168(r2)
    fmadds f0, f2, f1, f0
    stfs f0, 0x8(r1)
    b @L_801C6200
@L_801C61EC:
    lfs f2, -22044(r2)
    lfs f1, 0x14(r30)
    lfs f0, -22164(r2)
    fmadds f0, f2, f1, f0
    stfs f0, 0xc(r1)
@L_801C6200:
    lfs f0, -22040(r2)
    fmr f3, f31
    lfs f1, -22204(r2)
    mr r3, r29
    fmuls f2, f30, f0
    lfs f4, -22048(r2)
    addi r4, r1, 0x8
    bl fn_801C63C0
    lbz r0, -21616(r13)
    cmplwi r0, 0x0
    bne @L_801C6240
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C6240
    mr r3, r29
    bl fn_800DC298
@L_801C6240:
    mr r3, r28
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x54(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
