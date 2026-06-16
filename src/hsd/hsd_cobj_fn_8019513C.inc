    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    mr. r30, r3
    fmr f31, f1
    mr r31, r4
    beq @L_80195180
    lwz r0, 0x24(r30)
    cmplwi r0, 0x0
    beq @L_80195180
    lwz r0, 0x28(r30)
    cmplwi r0, 0x0
    bne @L_80195188
@L_80195180:
    li r0, 0x0
    b @L_80195284
@L_80195188:
    cmplwi r30, 0x0
    bne @L_801951A0
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_801951A0:
    cmplwi r30, 0x0
    bne @L_801951B8
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_801951B8:
    lwz r3, 0x24(r30)
    addi r4, r1, 0x10
    bl fn_80191688
    cmplwi r30, 0x0
    bne @L_801951DC
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_801951DC:
    cmplwi r30, 0x0
    bne @L_801951F4
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_801951F4:
    lwz r3, 0x28(r30)
    addi r4, r1, 0x1c
    bl fn_80191688
    addi r3, r1, 0x1c
    addi r4, r1, 0x10
    addi r5, r1, 0x40
    bl fn_800A3A9C
    lfs f0, 0x40(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019526C
    lfs f0, 0x44(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019526C
    lfs f0, 0x48(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019526C
    li r0, -0x1
    b @L_8019527C
@L_8019526C:
    addi r3, r1, 0x40
    addi r4, r1, 0x40
    bl fn_800A3ADC
    li r0, 0x0
@L_8019527C:
    cntlzw r0, r0
    srwi r0, r0, 5
@L_80195284:
    cmpwi r0, 0x0
    bne @L_80195294
    li r3, 0x0
    b @L_80195570
@L_80195294:
    lfs f0, 0x44(r1)
    lfd f1, -23832(r2)
    fabs f2, f0
    lfd f0, -23824(r2)
    frsp f2, f2
    fsub f1, f1, f2
    fcmpo cr0, f1, f0
    bge @L_801953FC
    lfs f1, 0x48(r1)
    lfs f0, 0x48(r1)
    lfs f3, 0x44(r1)
    fmuls f1, f1, f0
    lfs f2, 0x44(r1)
    lfs f0, -23848(r2)
    fmadds f8, f3, f2, f1
    fcmpo cr0, f8, f0
    ble @L_80195330
    frsqrte f7, f8
    lfd f6, -23816(r2)
    lfd f3, -23808(r2)
    lfd f4, -23816(r2)
    fmul f5, f7, f7
    lfd f1, -23808(r2)
    lfd f2, -23816(r2)
    fmul f6, f6, f7
    lfd f0, -23808(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_801953BC
@L_80195330:
    lfd f0, -23800(r2)
    fcmpo cr0, f8, f0
    bge @L_80195348
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_801953BC
@L_80195348:
    stfs f8, 0xc(r1)
    lis r0, 0x7f80
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_80195370
    bge @L_801953A8
    cmpwi r3, 0x0
    beq @L_8019538C
    b @L_801953A8
@L_80195370:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_80195384
    li r0, 0x1
    b @L_801953AC
@L_80195384:
    li r0, 0x2
    b @L_801953AC
@L_8019538C:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_801953A0
    li r0, 0x5
    b @L_801953AC
@L_801953A0:
    li r0, 0x3
    b @L_801953AC
@L_801953A8:
    li r0, 0x4
@L_801953AC:
    cmpwi r0, 0x1
    bne @L_801953BC
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
@L_801953BC:
    lfs f1, 0x40(r1)
    stfs f8, 0x34(r1)
    lfs f0, 0x40(r1)
    fneg f3, f1
    lfs f2, 0x34(r1)
    fneg f1, f0
    lfs f0, 0x34(r1)
    fdivs f3, f3, f2
    lfs f4, 0x44(r1)
    lfs f2, 0x48(r1)
    fdivs f0, f1, f0
    fmuls f1, f4, f3
    fmuls f0, f2, f0
    stfs f1, 0x38(r1)
    stfs f0, 0x3c(r1)
    b @L_80195540
@L_801953FC:
    lfs f1, 0x48(r1)
    lfs f0, 0x48(r1)
    lfs f3, 0x40(r1)
    fmuls f1, f1, f0
    lfs f2, 0x40(r1)
    lfs f0, -23848(r2)
    fmadds f8, f3, f2, f1
    fcmpo cr0, f8, f0
    ble @L_80195478
    frsqrte f7, f8
    lfd f6, -23816(r2)
    lfd f3, -23808(r2)
    lfd f4, -23816(r2)
    fmul f5, f7, f7
    lfd f1, -23808(r2)
    lfd f2, -23816(r2)
    fmul f6, f6, f7
    lfd f0, -23808(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_80195504
@L_80195478:
    lfd f0, -23800(r2)
    fcmpo cr0, f8, f0
    bge @L_80195490
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_80195504
@L_80195490:
    stfs f8, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801954B8
    bge @L_801954F0
    cmpwi r3, 0x0
    beq @L_801954D4
    b @L_801954F0
@L_801954B8:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_801954CC
    li r0, 0x1
    b @L_801954F4
@L_801954CC:
    li r0, 0x2
    b @L_801954F4
@L_801954D4:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_801954E8
    li r0, 0x5
    b @L_801954F4
@L_801954E8:
    li r0, 0x3
    b @L_801954F4
@L_801954F0:
    li r0, 0x4
@L_801954F4:
    cmpwi r0, 0x1
    bne @L_80195504
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
@L_80195504:
    lfs f1, 0x44(r1)
    stfs f8, 0x38(r1)
    lfs f0, 0x44(r1)
    fneg f3, f1
    lfs f2, 0x38(r1)
    fneg f1, f0
    lfs f0, 0x38(r1)
    fdivs f3, f3, f2
    lfs f4, 0x40(r1)
    lfs f2, 0x48(r1)
    fdivs f0, f1, f0
    fmuls f1, f4, f3
    fmuls f0, f2, f0
    stfs f1, 0x34(r1)
    stfs f0, 0x3c(r1)
@L_80195540:
    fneg f1, f31
    addi r3, r1, 0x4c
    addi r4, r1, 0x40
    bl fn_800A3244
    addi r3, r1, 0x4c
    addi r4, r1, 0x34
    addi r5, r1, 0x28
    bl fn_800A3820
    mr r4, r31
    addi r3, r1, 0x28
    bl fn_800A3ADC
    li r3, 0x1
@L_80195570:
    psq_l f31, 0x98(r1), 0, 0
    lwz r0, 0xa4(r1)
    lfd f31, 0x90(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
