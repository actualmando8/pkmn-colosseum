    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stmw r26, 0x68(r1)
    lis r4, lbl_80466BC0@ha
    lis r0, 0x4330
    addi r4, r4, lbl_80466BC0@l
    mr r30, r3
    mr r26, r4
    stw r0, 0x48(r1)
    lhz r0, 0x6(r4)
    lfd f1, -23776(r2)
    stw r0, 0x4c(r1)
    lfs f2, 0x14(r3)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_8019644C
    li r3, 0x0
    b @L_8019666C
@L_8019644C:
    lhz r3, 0x6(r26)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    lfd f1, -23776(r2)
    stw r3, 0x4c(r1)
    lfs f2, 0x18(r30)
    lfd f0, 0x48(r1)
    lfs f30, 0xc(r30)
    fsubs f0, f0, f1
    lfs f3, 0x10(r30)
    lfs f31, 0x14(r30)
    fcmpo cr0, f2, f0
    bge @L_80196488
    lfs f0, 0x18(r30)
    b @L_801964A4
@L_80196488:
    lhz r3, 0x6(r26)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    lfd f1, -23776(r2)
    stw r3, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f1
@L_801964A4:
    fsubs f4, f0, f31
    fsubs f29, f3, f30
    fmr f1, f4
    bl fn_800C46B0
    fmr f1, f29
    mr r27, r3
    bl fn_800C46B0
    fmr f1, f31
    mr r28, r3
    bl fn_800C46B0
    fmr f1, f30
    mr r29, r3
    bl fn_800C46B0
    mr r4, r29
    mr r5, r28
    mr r6, r27
    bl fn_800BD7A0
    lhz r3, 0x6(r26)
    lis r0, 0x4330
    stw r0, 0x58(r1)
    lfd f1, -23776(r2)
    stw r3, 0x5c(r1)
    lfs f2, 0x18(r30)
    lfd f0, 0x58(r1)
    lfs f30, 0xc(r30)
    fsubs f0, f0, f1
    lfs f3, 0x10(r30)
    lfs f31, 0x14(r30)
    fcmpo cr0, f2, f0
    bge @L_80196524
    lfs f0, 0x18(r30)
    b @L_80196540
@L_80196524:
    lhz r3, 0x6(r26)
    lis r0, 0x4330
    stw r0, 0x60(r1)
    lfd f1, -23776(r2)
    stw r3, 0x64(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f1
@L_80196540:
    fsubs f4, f0, f31
    lfs f2, 0x18(r30)
    lfs f0, 0x14(r30)
    fsubs f29, f3, f30
    lwz r12, lbl_80478C58(r13)
    fmr f1, f30
    fsubs f0, f2, f0
    lfs f5, -23848(r2)
    fmr f3, f29
    lfs f6, -23792(r2)
    fmr f2, f31
    li r3, 0x0
    fdivs f29, f4, f0
    mtctr r12
    bctrl
    lbz r0, 0x50(r30)
    cmpwi r0, 0x2
    beq @L_80196600
    bge @L_80196598
    cmpwi r0, 0x1
    bge @L_801965A4
    b @L_8019665C
@L_80196598:
    cmpwi r0, 0x4
    bge @L_8019665C
    b @L_80196630
@L_801965A4:
    lfd f1, -23816(r2)
    li r31, 0x0
    lfs f0, 0x40(r30)
    lfd f2, -23768(r2)
    fmul f0, f1, f0
    fmul f1, f2, f0
    frsp f1, f1
    bl fn_800CE220
    frsp f1, f1
    lfs f3, 0x38(r30)
    lfs f2, -23756(r2)
    addi r3, r1, 0x8
    lfs f0, -23792(r2)
    fmuls f1, f3, f1
    fnmsubs f0, f2, f29, f0
    lfs f2, 0x44(r30)
    lfs f5, 0x38(r30)
    fmuls f4, f1, f2
    lfs f6, 0x3c(r30)
    fmuls f2, f1, f0
    fneg f3, f4
    bl fn_800A3874
    b @L_8019665C
@L_80196600:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x8
    lfs f0, 0x44(r30)
    li r31, 0x0
    lfs f3, 0x48(r30)
    fsubs f0, f1, f0
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    fnmsubs f2, f29, f0, f1
    bl fn_800A3874
    b @L_8019665C
@L_80196630:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x8
    lfs f0, 0x44(r30)
    li r31, 0x1
    lfs f3, 0x48(r30)
    fsubs f0, f1, f0
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    fnmsubs f2, f29, f0, f1
    bl fn_800A39E0
@L_8019665C:
    mr r4, r31
    addi r3, r1, 0x8
    bl fn_800BD2E0
    li r3, 0x1
@L_8019666C:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    lmw r26, 0x68(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
