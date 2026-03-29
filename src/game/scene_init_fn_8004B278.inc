    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r30, r3
    lwz r31, 0x60(r30)
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047BDC0(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lis r3, lbl_803A6A60@ha
    lfd f1, lbl_8047BDE8(r2)
    stw r0, 0x10(r1)
    addi r29, r3, lbl_803A6A60@l
    li r3, 0x3f0
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f31
    stfs f0, 0x8(r29)
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8004B310
    li r3, 0x438
    li r4, 0x1
    bl fn_8005D8F8
    b @L_8004B31C
    @L_8004B310:
    li r3, 0x438
    li r4, 0x0
    bl fn_8005D8F8
    @L_8004B31C:
    lis r3, lbl_803A6A60@ha
    lfs f3, lbl_8047BDA8(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f2, 0x8(r29)
    lfs f1, 0x40(r3)
    lfs f0, lbl_8047BDA0(r2)
    fmadds f1, f3, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, 0x40(r3)
    ble @L_8004B34C
    lfs f0, lbl_8047BDAC(r2)
    stfs f0, 0x40(r3)
    @L_8004B34C:
    lis r3, lbl_803A6A60@ha
    addi r3, r3, lbl_803A6A60@l
    lfs f0, 0x30(r3)
    lfs f1, 0x38(r3)
    fcmpu cr0, f0, f1
    beq @L_8004B40C
    fsubs f3, f1, f0
    lfs f2, lbl_8047BDD8(r2)
    lfs f1, 0x8(r29)
    lfs f0, lbl_8047BDDC(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_8004B388
    fmr f4, f0
    @L_8004B388:
    lfs f0, lbl_8047BDE0(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_8004B39C
    fmr f4, f0
    @L_8004B39C:
    lfs f1, 0x30(r3)
    lfs f0, lbl_8047BDAC(r2)
    fadds f1, f1, f4
    lfs f3, 0x38(r3)
    fcmpo cr0, f4, f0
    fsubs f2, f3, f1
    stfs f1, 0x30(r3)
    ble @L_8004B3C0
    b @L_8004B3C4
    @L_8004B3C0:
    fneg f4, f4
    @L_8004B3C4:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f2, f0
    ble @L_8004B3D8
    fmr f0, f2
    b @L_8004B3DC
    @L_8004B3D8:
    fneg f0, f2
    @L_8004B3DC:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_8004B408
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f2, f0
    ble @L_8004B3F8
    b @L_8004B3FC
    @L_8004B3F8:
    fneg f2, f2
    @L_8004B3FC:
    lfs f0, lbl_8047BDA0(r2)
    fcmpo cr0, f2, f0
    bge @L_8004B40C
    @L_8004B408:
    stfs f3, 0x30(r3)
    @L_8004B40C:
    lis r3, lbl_803A6A60@ha
    addi r3, r3, lbl_803A6A60@l
    lfs f0, 0x34(r3)
    lfs f1, 0x3c(r3)
    fcmpu cr0, f0, f1
    beq @L_8004B4CC
    fsubs f3, f1, f0
    lfs f2, lbl_8047BDD8(r2)
    lfs f1, 0x8(r29)
    lfs f0, lbl_8047BDDC(r2)
    fmuls f2, f2, f3
    fmuls f3, f2, f1
    fcmpo cr0, f3, f0
    ble @L_8004B448
    fmr f3, f0
    @L_8004B448:
    lfs f0, lbl_8047BDE0(r2)
    fcmpo cr0, f3, f0
    cror eq, lt, eq
    bne @L_8004B45C
    fmr f3, f0
    @L_8004B45C:
    lfs f1, 0x34(r3)
    lfs f0, lbl_8047BDAC(r2)
    fadds f1, f1, f3
    lfs f2, 0x3c(r3)
    fcmpo cr0, f3, f0
    fsubs f4, f2, f1
    stfs f1, 0x34(r3)
    ble @L_8004B480
    b @L_8004B484
    @L_8004B480:
    fneg f3, f3
    @L_8004B484:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f4, f0
    ble @L_8004B498
    fmr f0, f4
    b @L_8004B49C
    @L_8004B498:
    fneg f0, f4
    @L_8004B49C:
    fcmpo cr0, f0, f3
    cror eq, lt, eq
    beq @L_8004B4C8
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f4, f0
    ble @L_8004B4B8
    b @L_8004B4BC
    @L_8004B4B8:
    fneg f4, f4
    @L_8004B4BC:
    lfs f0, lbl_8047BDA0(r2)
    fcmpo cr0, f4, f0
    bge @L_8004B4CC
    @L_8004B4C8:
    stfs f2, 0x34(r3)
    @L_8004B4CC:
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8004B520
    bge @L_8004B4EC
    cmpwi r0, 0x0
    beq @L_8004B4F8
    b @L_8004B570
    @L_8004B4EC:
    cmpwi r0, 0x4
    bge @L_8004B570
    b @L_8004B558
    @L_8004B4F8:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8004B570
    lfs f0, lbl_8047BDAC(r2)
    li r0, 0x1
    lwz r3, 0x8(r31)
    stfs f0, 0x0(r3)
    stb r0, 0x2(r30)
    b @L_8004B570
    @L_8004B520:
    lwz r3, 0x8(r31)
    lfs f0, lbl_8047BDE4(r2)
    lfs f2, 0x0(r3)
    lfs f1, lbl_8047BDA0(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    cror eq, gt, eq
    bne @L_8004B570
    lwz r3, 0x8(r31)
    lfs f0, 0x0(r3)
    fsubs f0, f0, f1
    stfs f0, 0x0(r3)
    b @L_8004B570
    @L_8004B558:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8004B570
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_8004B570:
    li r3, 0x0
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
