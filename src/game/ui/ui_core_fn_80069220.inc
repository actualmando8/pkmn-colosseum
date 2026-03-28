    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    mr r31, r3
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047BFF0(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lis r4, lbl_803A9F08@ha
    lfd f1, lbl_8047C020(r2)
    stw r0, 0x10(r1)
    addi r5, r4, lbl_803A9F08@l
    addis r6, r5, 0x1
    li r3, 0x0
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f31
    stfs f0, -0x3278(r6)
@L_80069290:
    addis r4, r5, 0x1
    subi r4, r4, 0x3274
    li r0, 0x6
    mtctr r0
@L_800692A0:
    lfs f0, 0x0(r4)
    lfs f1, 0x18(r4)
    fcmpu cr0, f0, f1
    beq @L_80069358
    fsubs f1, f1, f0
    lfs f2, lbl_8047C010(r2)
    lfs f0, -0x3278(r6)
    fmuls f1, f2, f1
    fmuls f3, f1, f0
    fcmpo cr0, f3, f2
    ble @L_800692D0
    fmr f3, f2
@L_800692D0:
    lfs f0, lbl_8047C014(r2)
    fcmpo cr0, f3, f0
    cror eq, lt, eq
    bne @L_800692E4
    fmr f3, f0
@L_800692E4:
    lfs f1, 0x0(r4)
    lfs f0, lbl_8047BFE8(r2)
    fadds f1, f1, f3
    fcmpo cr0, f3, f0
    stfs f1, 0x0(r4)
    lfs f2, 0x18(r4)
    lfs f0, 0x0(r4)
    fsubs f1, f2, f0
    ble @L_8006930C
    b @L_80069310
@L_8006930C:
    fneg f3, f3
@L_80069310:
    lfs f0, lbl_8047BFE8(r2)
    fcmpo cr0, f1, f0
    ble @L_80069324
    fmr f0, f1
    b @L_80069328
@L_80069324:
    fneg f0, f1
@L_80069328:
    fcmpo cr0, f0, f3
    cror eq, lt, eq
    beq @L_80069354
    lfs f0, lbl_8047BFE8(r2)
    fcmpo cr0, f1, f0
    ble @L_80069344
    b @L_80069348
@L_80069344:
    fneg f1, f1
@L_80069348:
    lfs f0, lbl_8047C018(r2)
    fcmpo cr0, f1, f0
    bge @L_80069358
@L_80069354:
    stfs f2, 0x0(r4)
@L_80069358:
    addi r4, r4, 0x4
    bdnz @L_800692A0
    addi r5, r5, 0x30
    addi r3, r3, 0x1
    cmpwi r3, 0x4
    blt @L_80069290
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    addis r3, r3, 0x1
    lwz r0, -0x3280(r3)
    extsh r0, r0
    sth r0, 0x84(r31)
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
