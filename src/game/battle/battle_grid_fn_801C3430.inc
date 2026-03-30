    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stfd f30, 0xc0(r1)
    psq_st f30, 0xc8(r1), 0, 0
    stfd f29, 0xb0(r1)
    psq_st f29, 0xb8(r1), 0, 0
    stfd f28, 0xa0(r1)
    psq_st f28, 0xa8(r1), 0, 0
    stfd f27, 0x90(r1)
    psq_st f27, 0x98(r1), 0, 0
    stfd f26, 0x80(r1)
    psq_st f26, 0x88(r1), 0, 0
    stfd f25, 0x70(r1)
    psq_st f25, 0x78(r1), 0, 0
    stmw r25, 0x54(r1)
    lis r3, lbl_80466DE8@ha
    addi r29, r3, lbl_80466DE8@l
    lhz r0, 0x40(r29)
    mr r31, r29
    cmplwi r0, 0x0
    beq @L_801C3A18
    mr r30, r29
    li r27, -0x2
    bne @L_801C34A4
    li r27, 0x0
    b @L_801C3508
    @L_801C34A4:
    li r25, 0x0
    b @L_801C34FC
    @L_801C34AC:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801C34F4
    addi r28, r30, 0x4
    li r26, 0x0
    b @L_801C34E8
    @L_801C34C4:
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801C34E0
    bl fn_801DAC24
    cmpw r3, r27
    ble @L_801C34E0
    mr r27, r3
    @L_801C34E0:
    addi r26, r26, 0x1
    addi r28, r28, 0x4
    @L_801C34E8:
    clrlwi r0, r26, 16
    cmplwi r0, 0x2
    blt @L_801C34C4
    @L_801C34F4:
    addi r25, r25, 0x1
    addi r30, r30, 0x10
    @L_801C34FC:
    clrlwi r0, r25, 16
    cmplwi r0, 0x4
    blt @L_801C34AC
    @L_801C3508:
    cmpwi r27, 0x1
    beq @L_801C3540
    bge @L_801C3528
    cmpwi r27, 0x0
    bge @L_801C3558
    cmpwi r27, -0x2
    bge @L_801C3538
    b @L_801C3558
    @L_801C3528:
    cmpwi r27, 0x3
    beq @L_801C3550
    bge @L_801C3558
    b @L_801C3548
    @L_801C3538:
    lfs f25, lbl_8047DF6C(r2)
    b @L_801C355C
    @L_801C3540:
    lfs f25, lbl_8047DF64(r2)
    b @L_801C355C
    @L_801C3548:
    lfs f25, lbl_8047DF70(r2)
    b @L_801C355C
    @L_801C3550:
    lfs f25, lbl_8047DF74(r2)
    b @L_801C355C
    @L_801C3558:
    lfs f25, lbl_8047DF68(r2)
    @L_801C355C:
    lhz r0, 0x40(r29)
    lis r3, lbl_80466DE8@ha
    addi r27, r3, lbl_80466DE8@l
    li r25, -0x2
    cmplwi r0, 0x0
    bne @L_801C357C
    li r25, 0x0
    b @L_801C35E0
    @L_801C357C:
    li r29, 0x0
    b @L_801C35D4
    @L_801C3584:
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    beq @L_801C35CC
    addi r26, r27, 0x4
    li r28, 0x0
    b @L_801C35C0
    @L_801C359C:
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_801C35B8
    bl fn_801DAC24
    cmpw r3, r25
    ble @L_801C35B8
    mr r25, r3
    @L_801C35B8:
    addi r28, r28, 0x1
    addi r26, r26, 0x4
    @L_801C35C0:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801C359C
    @L_801C35CC:
    addi r29, r29, 0x1
    addi r27, r27, 0x10
    @L_801C35D4:
    clrlwi r0, r29, 16
    cmplwi r0, 0x4
    blt @L_801C3584
    @L_801C35E0:
    bl fn_80115BD8
    mr r29, r3
    bl fn_80113F48
    cmpwi r25, 0x2
    mr r27, r3
    li r28, 0x0
    beq @L_801C3620
    bge @L_801C360C
    cmpwi r25, 0x1
    bge @L_801C3618
    b @L_801C3630
    @L_801C360C:
    cmpwi r25, 0x4
    bge @L_801C3630
    b @L_801C3628
    @L_801C3618:
    lfs f1, lbl_8047DF5C(r2)
    b @L_801C3634
    @L_801C3620:
    lfs f1, lbl_8047DF60(r2)
    b @L_801C3634
    @L_801C3628:
    lfs f1, lbl_8047DF64(r2)
    b @L_801C3634
    @L_801C3630:
    lfs f1, lbl_8047DF68(r2)
    @L_801C3634:
    fmr f2, f1
    addi r3, r1, 0x8
    fmr f3, f1
    bl fn_800E01F4
    bl fn_80113F48
    lwz r4, 0x8(r29)
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C36C0
    lis r4, lbl_802757FC@ha
    addi r4, r4, lbl_802757FC@l
    bl fn_80191ECC
    mr. r30, r3
    beq @L_801C36C0
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801C36C0
    lwz r3, 0x8(r29)
    bl fn_80115274
    mr r29, r3
    li r25, 0x0
    b @L_801C36B0
    @L_801C368C:
    mr r3, r27
    or r4, r29, r28
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C36A8
    addi r4, r1, 0x8
    bl fn_800E407C
    @L_801C36A8:
    addi r25, r25, 0x4
    addi r28, r28, 0x1
    @L_801C36B0:
    lwz r3, 0x0(r30)
    lwzx r0, r3, r25
    cmplwi r0, 0x0
    bne @L_801C368C
    @L_801C36C0:
    addi r3, r1, 0x8
    bl fn_80176600
    addi r3, r1, 0x20
    bl fn_800E0204
    addi r3, r1, 0x38
    bl fn_800E0204
    addi r3, r1, 0x2c
    bl fn_800E0204
    fneg f4, f25
    lfs f0, lbl_8047DF78(r2)
    lfs f3, lbl_8047DF7C(r2)
    li r30, 0x0
    lfs f2, lbl_8047DF80(r2)
    fmuls f29, f0, f25
    lfs f1, lbl_8047DF84(r2)
    fmuls f31, f0, f4
    lfs f0, lbl_8047DF88(r2)
    fmuls f30, f3, f4
    fmuls f28, f3, f25
    fmuls f27, f2, f25
    fmuls f26, f1, f25
    fmuls f25, f0, f25
    b @L_801C3A0C
    @L_801C371C:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_801C3A04
    lbz r0, 0xe(r31)
    cmplwi r0, 0x0
    beq @L_801C3748
    lfs f0, lbl_8047DF8C(r2)
    stfs f31, 0x20(r1)
    stfs f30, 0x38(r1)
    stfs f0, 0x30(r1)
    b @L_801C3758
    @L_801C3748:
    lfs f0, lbl_8047DF90(r2)
    stfs f29, 0x20(r1)
    stfs f28, 0x38(r1)
    stfs f0, 0x30(r1)
    @L_801C3758:
    lbz r4, 0xf(r31)
    lis r3, 0x4330
    lhz r0, 0xc(r31)
    extsb r4, r4
    stw r3, 0x48(r1)
    xoris r3, r4, 0x8000
    lfd f1, lbl_8047DF98(r2)
    stw r3, 0x4c(r1)
    cmpwi r0, 0x1
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fmuls f0, f27, f0
    stfs f0, 0x28(r1)
    stfs f0, 0x40(r1)
    beq @L_801C37A8
    bge @L_801C379C
    b @L_801C39B0
    @L_801C379C:
    cmpwi r0, 0x3
    bge @L_801C39B0
    b @L_801C384C
    @L_801C37A8:
    fadds f0, f0, f26
    lwz r25, 0x4(r31)
    cmplwi r25, 0x0
    stfs f0, 0x28(r1)
    beq @L_801C3800
    lbz r28, 0xe(r31)
    beq @L_801C39B0
    mr r3, r25
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C39B0
    addi r4, r1, 0x38
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r28, 0x0
    li r0, 0x1
    beq @L_801C37F8
    li r0, -0x1
    @L_801C37F8:
    stb r0, 0x76(r25)
    b @L_801C39B0
    @L_801C3800:
    lwz r28, 0x8(r31)
    lbz r29, 0xe(r31)
    cmplwi r28, 0x0
    beq @L_801C39B0
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C39B0
    addi r4, r1, 0x38
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C3844
    li r0, -0x1
    @L_801C3844:
    stb r0, 0x76(r28)
    b @L_801C39B0
    @L_801C384C:
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    bl fn_800E01D0
    lbz r29, 0xe(r31)
    cmplwi r29, 0x0
    beq @L_801C390C
    lfs f0, 0x40(r1)
    lwz r28, 0x4(r31)
    fsubs f0, f0, f25
    cmplwi r28, 0x0
    stfs f0, 0x1c(r1)
    beq @L_801C38B4
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C38B4
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C38B0
    li r0, -0x1
    @L_801C38B0:
    stb r0, 0x76(r28)
    @L_801C38B4:
    lfs f0, 0x40(r1)
    lwz r28, 0x8(r31)
    fadds f0, f0, f25
    lbz r29, 0xe(r31)
    cmplwi r28, 0x0
    stfs f0, 0x1c(r1)
    beq @L_801C39B0
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C39B0
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C3904
    li r0, -0x1
    @L_801C3904:
    stb r0, 0x76(r28)
    b @L_801C39B0
    @L_801C390C:
    lfs f0, 0x40(r1)
    lwz r28, 0x8(r31)
    fsubs f0, f0, f25
    cmplwi r28, 0x0
    stfs f0, 0x1c(r1)
    beq @L_801C395C
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C395C
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C3958
    li r0, -0x1
    @L_801C3958:
    stb r0, 0x76(r28)
    @L_801C395C:
    lfs f0, 0x40(r1)
    lwz r28, 0x4(r31)
    fadds f0, f0, f25
    lbz r29, 0xe(r31)
    cmplwi r28, 0x0
    stfs f0, 0x1c(r1)
    beq @L_801C39B0
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C39B0
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C39AC
    li r0, -0x1
    @L_801C39AC:
    stb r0, 0x76(r28)
    @L_801C39B0:
    lwz r28, 0x0(r31)
    lbz r29, 0xe(r31)
    cmplwi r28, 0x0
    beq @L_801C39F8
    mr r3, r28
    bl fn_801DAC3C
    mr. r27, r3
    beq @L_801C39F8
    addi r4, r1, 0x20
    bl fn_800E43A4
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E4170
    cmplwi r29, 0x0
    li r0, 0x1
    beq @L_801C39F4
    li r0, -0x1
    @L_801C39F4:
    stb r0, 0x76(r28)
    @L_801C39F8:
    lwz r3, 0x0(r31)
    li r4, 0x1
    bl fn_801DA4E8
    @L_801C3A04:
    addi r30, r30, 0x1
    addi r31, r31, 0x10
    @L_801C3A0C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x4
    blt @L_801C371C
    @L_801C3A18:
    psq_l f31, 0xd8(r1), 0, 0
    lfd f31, 0xd0(r1)
    psq_l f30, 0xc8(r1), 0, 0
    lfd f30, 0xc0(r1)
    psq_l f29, 0xb8(r1), 0, 0
    lfd f29, 0xb0(r1)
    psq_l f28, 0xa8(r1), 0, 0
    lfd f28, 0xa0(r1)
    psq_l f27, 0x98(r1), 0, 0
    lfd f27, 0x90(r1)
    psq_l f26, 0x88(r1), 0, 0
    lfd f26, 0x80(r1)
    psq_l f25, 0x78(r1), 0, 0
    lfd f25, 0x70(r1)
    lmw r25, 0x54(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
