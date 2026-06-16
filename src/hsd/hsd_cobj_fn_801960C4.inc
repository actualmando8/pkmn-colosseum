    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stfd f30, 0xb0(r1)
    psq_st f30, 0xb8(r1), 0, 0
    stfd f29, 0xa0(r1)
    psq_st f29, 0xa8(r1), 0, 0
    stmw r26, 0x88(r1)
    lis r4, lbl_80466BC0@ha
    mr r29, r3
    addi r4, r4, lbl_80466BC0@l
    lis r0, 0x4330
    lhz r3, 0x6(r4)
    stw r0, 0x48(r1)
    subi r30, r3, 0x8
    lfd f1, -23776(r2)
    stw r30, 0x4c(r1)
    lfs f2, 0x18(r29)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    bge @L_8019612C
    li r3, 0x0
    b @L_801963B4
@L_8019612C:
    lhz r0, 0x1c(r29)
    lis r5, 0x4330
    lhz r4, 0x1e(r29)
    lis r3, 0x4330
    stw r0, 0x4c(r1)
    lhz r0, 0x20(r29)
    stw r5, 0x48(r1)
    lfd f2, -23776(r2)
    cmplw r0, r30
    lfd f0, 0x48(r1)
    stw r4, 0x54(r1)
    lfd f1, -23776(r2)
    fsubs f30, f0, f2
    stw r3, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f3, f0, f1
    ble @L_80196178
    lhz r0, 0x20(r29)
    b @L_8019617C
@L_80196178:
    mr r0, r30
@L_8019617C:
    lhz r3, 0x22(r29)
    subf r5, r30, r0
    lis r4, 0x4330
    lis r0, 0x4330
    subf r3, r30, r3
    stw r5, 0x5c(r1)
    lfd f2, -23776(r2)
    fsubs f29, f3, f30
    stw r4, 0x58(r1)
    lfd f1, -23776(r2)
    lfd f0, 0x58(r1)
    stw r3, 0x64(r1)
    fsubs f31, f0, f2
    stw r0, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f4, f0, f1
    fsubs f4, f4, f31
    fmr f1, f4
    bl fn_800C46B0
    fmr f1, f29
    mr r26, r3
    bl fn_800C46B0
    fmr f1, f31
    mr r27, r3
    bl fn_800C46B0
    fmr f1, f30
    mr r28, r3
    bl fn_800C46B0
    mr r4, r28
    mr r5, r27
    mr r6, r26
    bl fn_800BD7A0
    lis r0, 0x4330
    stw r30, 0x6c(r1)
    lfd f1, -23776(r2)
    stw r0, 0x68(r1)
    lfs f2, 0x14(r29)
    lfd f0, 0x68(r1)
    lfs f30, 0xc(r29)
    fsubs f0, f0, f1
    lfs f3, 0x10(r29)
    fcmpo cr0, f2, f0
    ble @L_80196230
    lfs f6, 0x14(r29)
    b @L_80196248
@L_80196230:
    lis r0, 0x4330
    stw r30, 0x74(r1)
    lfd f1, -23776(r2)
    stw r0, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f6, f0, f1
@L_80196248:
    lis r3, 0x4330
    lis r0, 0x4330
    stw r3, 0x78(r1)
    fsubs f29, f3, f30
    lfd f2, -23776(r2)
    fmr f1, f30
    stw r30, 0x7c(r1)
    li r3, 0x0
    lfd f5, -23776(r2)
    lfd f0, 0x78(r1)
    fmr f3, f29
    stw r30, 0x84(r1)
    fsubs f0, f0, f2
    lfs f7, 0x18(r29)
    stw r0, 0x80(r1)
    lfs f2, 0x18(r29)
    fsubs f31, f6, f0
    lfd f4, 0x80(r1)
    lfs f0, 0x14(r29)
    fsubs f4, f4, f5
    lwz r12, lbl_80478C58(r13)
    fsubs f0, f2, f0
    fmr f2, f31
    lfs f5, -23848(r2)
    fsubs f4, f7, f4
    lfs f6, -23792(r2)
    fsubs f4, f4, f31
    fdivs f29, f4, f0
    mtctr r12
    bctrl
    lbz r0, 0x50(r29)
    cmpwi r0, 0x2
    beq @L_80196348
    bge @L_801962DC
    cmpwi r0, 0x1
    bge @L_801962E8
    b @L_801963A4
@L_801962DC:
    cmpwi r0, 0x4
    bge @L_801963A4
    b @L_80196378
@L_801962E8:
    lfd f1, -23816(r2)
    li r31, 0x0
    lfs f0, 0x40(r29)
    lfd f2, -23768(r2)
    fmul f0, f1, f0
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE220
    frsp f4, f1
    lfs f3, 0x38(r29)
    lfs f2, -23756(r2)
    addi r3, r1, 0x8
    lfs f0, -23760(r2)
    lfs f1, 0x44(r29)
    fmadds f0, f2, f29, f0
    lfs f5, 0x38(r29)
    fmuls f2, f3, f4
    lfs f6, 0x3c(r29)
    fmuls f4, f2, f1
    fmuls f1, f2, f0
    fneg f2, f2
    fneg f3, f4
    bl fn_800A3874
    b @L_801963A4
@L_80196348:
    lfs f2, 0x44(r29)
    addi r3, r1, 0x8
    lfs f0, 0x40(r29)
    li r31, 0x0
    lfs f3, 0x48(r29)
    fsubs f0, f0, f2
    lfs f4, 0x4c(r29)
    lfs f5, 0x38(r29)
    lfs f6, 0x3c(r29)
    fmadds f1, f29, f0, f2
    bl fn_800A3874
    b @L_801963A4
@L_80196378:
    lfs f2, 0x44(r29)
    addi r3, r1, 0x8
    lfs f0, 0x40(r29)
    li r31, 0x1
    lfs f3, 0x48(r29)
    fsubs f0, f0, f2
    lfs f4, 0x4c(r29)
    lfs f5, 0x38(r29)
    lfs f6, 0x3c(r29)
    fmadds f1, f29, f0, f2
    bl fn_800A39E0
@L_801963A4:
    mr r4, r31
    addi r3, r1, 0x8
    bl fn_800BD2E0
    li r3, 0x1
@L_801963B4:
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    lmw r26, 0x88(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
