    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lis r3, lbl_803A67FC@ha
    lis r4, lbl_803A6818@ha
    addi r3, r3, lbl_803A67FC@l
    lhz r31, 0x6(r3)
    addi r30, r4, lbl_803A6818@l
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047BCB0(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    lis r4, lbl_803A6818@ha
    stw r3, 0x14(r1)
    addi r3, r4, lbl_803A6818@l
    lfd f3, lbl_8047BCD8(r2)
    stw r0, 0x10(r1)
    lfs f1, 0x40(r3)
    lfd f2, 0x10(r1)
    lfs f0, lbl_8047BCBC(r2)
    fsubs f2, f2, f3
    fdivs f2, f2, f31
    fadds f1, f1, f2
    stfs f2, 0x3c(r3)
    fcmpo cr0, f1, f0
    stfs f1, 0x40(r3)
    cror eq, gt, eq
    bne @L_80041EE8
    lfs f0, lbl_8047BC94(r2)
    stfs f0, 0x40(r3)
    @L_80041EE8:
    lis r4, lbl_803A6818@ha
    addi r5, r4, lbl_803A6818@l
    lfs f6, 0x2c(r5)
    lfs f5, 0x34(r5)
    fcmpu cr0, f6, f5
    beq @L_8004200C
    fsubs f4, f5, f6
    lfs f0, lbl_8047BC94(r2)
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r3)
    fcmpo cr0, f4, f0
    fmuls f3, f2, f1
    ble @L_80041F24
    fmr f1, f4
    b @L_80041F28
    @L_80041F24:
    fneg f1, f4
    @L_80041F28:
    lfs f0, lbl_8047BCD0(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80041F40
    stfs f5, 0x2c(r5)
    b @L_8004200C
    @L_80041F40:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lbz r6, 0x48(r4)
    extsb r0, r6
    cmpwi r0, 0x1
    bne @L_80041F88
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x10(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_80041F88:
    extsb r0, r6
    cmpwi r0, 0x2
    bne @L_80041FC4
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_80041FC4:
    fmuls f2, f4, f3
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f6, f2
    fcmpo cr0, f2, f0
    fsubs f3, f5, f1
    stfs f1, 0x2c(r5)
    ble @L_80041FE4
    b @L_80041FE8
    @L_80041FE4:
    fneg f2, f2
    @L_80041FE8:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f3, f0
    ble @L_80041FF8
    b @L_80041FFC
    @L_80041FF8:
    fneg f3, f3
    @L_80041FFC:
    fcmpo cr0, f3, f2
    cror eq, lt, eq
    bne @L_8004200C
    stfs f5, 0x2c(r5)
    @L_8004200C:
    lis r4, lbl_803A6818@ha
    addi r5, r4, lbl_803A6818@l
    lfs f6, 0x30(r5)
    lfs f5, 0x38(r5)
    fcmpu cr0, f6, f5
    beq @L_80042130
    fsubs f4, f5, f6
    lfs f0, lbl_8047BC94(r2)
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r3)
    fcmpo cr0, f4, f0
    fmuls f3, f2, f1
    ble @L_80042048
    fmr f1, f4
    b @L_8004204C
    @L_80042048:
    fneg f1, f4
    @L_8004204C:
    lfs f0, lbl_8047BCD0(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80042064
    stfs f5, 0x30(r5)
    b @L_80042130
    @L_80042064:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lbz r6, 0x48(r4)
    extsb r0, r6
    cmpwi r0, 0x1
    bne @L_800420AC
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x10(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_800420AC:
    extsb r0, r6
    cmpwi r0, 0x2
    bne @L_800420E8
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_800420E8:
    fmuls f2, f4, f3
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f6, f2
    fcmpo cr0, f2, f0
    fsubs f3, f5, f1
    stfs f1, 0x30(r5)
    ble @L_80042108
    b @L_8004210C
    @L_80042108:
    fneg f2, f2
    @L_8004210C:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f3, f0
    ble @L_8004211C
    b @L_80042120
    @L_8004211C:
    fneg f3, f3
    @L_80042120:
    fcmpo cr0, f3, f2
    cror eq, lt, eq
    bne @L_80042130
    stfs f5, 0x30(r5)
    @L_80042130:
    lfs f3, 0x1d0(r30)
    lfs f6, 0x1d8(r30)
    fcmpu cr0, f3, f6
    beq @L_8004224C
    fsubs f5, f6, f3
    lfs f0, lbl_8047BC94(r2)
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r3)
    fcmpo cr0, f5, f0
    fmuls f4, f2, f1
    ble @L_80042164
    fmr f1, f5
    b @L_80042168
    @L_80042164:
    fneg f1, f5
    @L_80042168:
    lfs f0, lbl_8047BCD0(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80042180
    stfs f6, 0x1d0(r30)
    b @L_8004224C
    @L_80042180:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lbz r5, 0x48(r4)
    extsb r0, r5
    cmpwi r0, 0x1
    bne @L_800421C8
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x10(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f4, f4, f0
    @L_800421C8:
    extsb r0, r5
    cmpwi r0, 0x2
    bne @L_80042204
    lbz r4, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    extsb r0, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f4, f4, f0
    @L_80042204:
    fmuls f2, f5, f4
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f3, f2
    fcmpo cr0, f2, f0
    fsubs f3, f6, f1
    stfs f1, 0x1d0(r30)
    ble @L_80042224
    b @L_80042228
    @L_80042224:
    fneg f2, f2
    @L_80042228:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f3, f0
    ble @L_80042238
    b @L_8004223C
    @L_80042238:
    fneg f3, f3
    @L_8004223C:
    fcmpo cr0, f3, f2
    cror eq, lt, eq
    bne @L_8004224C
    stfs f6, 0x1d0(r30)
    @L_8004224C:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x50(r4)
    lfs f2, 0x4c(r4)
    fcmpu cr0, f0, f2
    beq @L_800422AC
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_80042294
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    ble @L_800422AC
    stfs f0, 0x4c(r4)
    b @L_800422AC
    @L_80042294:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    bge @L_800422AC
    stfs f0, 0x4c(r4)
    @L_800422AC:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x58(r4)
    lfs f2, 0x54(r4)
    fcmpu cr0, f0, f2
    beq @L_8004230C
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_800422F4
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r4)
    ble @L_8004230C
    stfs f0, 0x54(r4)
    b @L_8004230C
    @L_800422F4:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r4)
    bge @L_8004230C
    stfs f0, 0x54(r4)
    @L_8004230C:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x60(r4)
    lfs f2, 0x5c(r4)
    fcmpu cr0, f0, f2
    beq @L_8004236C
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCC0(r2)
    fdivs f0, f1, f0
    ble @L_80042354
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r4)
    ble @L_8004236C
    stfs f0, 0x5c(r4)
    b @L_8004236C
    @L_80042354:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r4)
    bge @L_8004236C
    stfs f0, 0x5c(r4)
    @L_8004236C:
    lfs f0, 0x1dc(r30)
    lfs f1, 0x1e0(r30)
    fcmpu cr0, f0, f1
    beq @L_80042424
    fsubs f3, f1, f0
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r3)
    lfs f0, lbl_8047BC98(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_800423A0
    fmr f4, f0
    @L_800423A0:
    lfs f0, lbl_8047BCC8(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_800423B4
    fmr f4, f0
    @L_800423B4:
    lfs f1, 0x1dc(r30)
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f1, f4
    lfs f3, 0x1e0(r30)
    fcmpo cr0, f4, f0
    fsubs f2, f3, f1
    stfs f1, 0x1dc(r30)
    ble @L_800423D8
    b @L_800423DC
    @L_800423D8:
    fneg f4, f4
    @L_800423DC:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_800423F0
    fmr f0, f2
    b @L_800423F4
    @L_800423F0:
    fneg f0, f2
    @L_800423F4:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80042420
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80042410
    b @L_80042414
    @L_80042410:
    fneg f2, f2
    @L_80042414:
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f2, f0
    bge @L_80042424
    @L_80042420:
    stfs f3, 0x1dc(r30)
    @L_80042424:
    lwz r0, 0x154(r30)
    cmpwi r0, 0x0
    bne @L_80042540
    lwz r0, 0x1e4(r30)
    cmpwi r0, 0x0
    beq @L_80042440
    b @L_80042634
    @L_80042440:
    lbz r0, 0x15d(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    bne @L_800424C4
    clrlwi r3, r31, 16
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8004248C
    lbz r0, 0x15e(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_80042480
    li r3, 0x23
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_80042480:
    li r0, 0x0
    stb r0, 0x15e(r30)
    b @L_800424CC
    @L_8004248C:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800424CC
    lbz r0, 0x15e(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800424B8
    li r3, 0x23
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_800424B8:
    li r0, 0x1
    stb r0, 0x15e(r30)
    b @L_800424CC
    @L_800424C4:
    li r0, 0x0
    stb r0, 0x15e(r30)
    @L_800424CC:
    clrlwi r3, r31, 16
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80042508
    lbz r3, 0x15d(r30)
    extsb r0, r3
    cmpwi r0, 0x4
    bge @L_80042634
    addi r0, r3, 0x1
    li r3, 0x23
    stb r0, 0x15d(r30)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    b @L_80042634
    @L_80042508:
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_80042634
    lbz r3, 0x15d(r30)
    extsb r0, r3
    cmpwi r0, 0x0
    ble @L_80042634
    subi r0, r3, 0x1
    li r3, 0x23
    stb r0, 0x15d(r30)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    b @L_80042634
    @L_80042540:
    clrlwi r3, r31, 16
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800425C4
    lbz r0, 0x160(r30)
    lbz r5, 0x15f(r30)
    extsb r3, r0
    extsb r4, r5
    subi r0, r3, 0x1
    cmpw r4, r0
    bge @L_80042584
    addi r0, r5, 0x1
    li r3, 0x23
    stb r0, 0x15f(r30)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_80042584:
    lbz r0, 0x15f(r30)
    lbz r3, 0x1c9(r30)
    extsb r4, r0
    extsb r0, r3
    cmpw r4, r0
    blt @L_80042634
    lfs f1, 0x1d8(r30)
    addi r0, r3, 0x1
    lfs f0, lbl_8047BCAC(r2)
    lbz r3, 0x1c8(r30)
    fsubs f0, f1, f0
    stb r0, 0x1c9(r30)
    addi r0, r3, 0x1
    stb r0, 0x1c8(r30)
    stfs f0, 0x1d8(r30)
    b @L_80042634
    @L_800425C4:
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_80042634
    lbz r3, 0x15f(r30)
    extsb r0, r3
    cmpwi r0, 0x0
    ble @L_800425F8
    subi r0, r3, 0x1
    li r3, 0x23
    stb r0, 0x15f(r30)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_800425F8:
    lbz r0, 0x15f(r30)
    lbz r3, 0x1c8(r30)
    extsb r4, r0
    extsb r0, r3
    cmpw r4, r0
    bge @L_80042634
    lfs f1, 0x1d8(r30)
    subi r0, r3, 0x1
    lfs f0, lbl_8047BCAC(r2)
    lbz r3, 0x1c9(r30)
    fadds f0, f1, f0
    stb r0, 0x1c8(r30)
    subi r0, r3, 0x1
    stb r0, 0x1c9(r30)
    stfs f0, 0x1d8(r30)
    @L_80042634:
    li r3, 0x0
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
