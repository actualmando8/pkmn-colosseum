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
    stfd f28, 0x90(r1)
    psq_st f28, 0x98(r1), 0, 0
    stfd f27, 0x80(r1)
    psq_st f27, 0x88(r1), 0, 0
    stfd f26, 0x70(r1)
    psq_st f26, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    mr r31, r3
    lbz r0, 0x55(r3)
    cmpwi r0, 0x1
    beq @L_801852B0
    bge @L_801852A4
    b @L_80185880
    @L_801852A4:
    cmpwi r0, 0x3
    bge @L_80185880
    b @L_8018549C
    @L_801852B0:
    lhz r0, 0x6a(r31)
    addi r3, r31, 0x5c
    lwz r4, 0x70(r31)
    mulli r0, r0, 0xc
    add r4, r4, r0
    bl fn_800E01D0
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x2c
    addi r4, r31, 0x5c
    bl fn_800E0168
    lfs f1, 0x2c(r1)
    lfs f2, 0x34(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f27, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f27, f0
    ble @L_80185324
    lfd f0, lbl_8047D7F0(r2)
    fsub f27, f27, f0
    frsp f27, f27
    b @L_8018533C
    @L_80185324:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f27, f0
    bge @L_8018533C
    lfd f0, lbl_8047D7F0(r2)
    fadd f27, f0, f27
    frsp f27, f27
    @L_8018533C:
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_80185384
    @L_8018534C:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185380
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_80185380
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80185380
    lwz r28, 0x4(r3)
    b @L_801853F0
    @L_80185380:
    addi r30, r30, 0x1
    @L_80185384:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018534C
    li r27, 0x0
    b @L_801853E0
    @L_80185398:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801853DC
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_801853DC
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_801853F0
    @L_801853DC:
    addi r27, r27, 0x1
    @L_801853E0:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185398
    li r28, 0x0
    @L_801853F0:
    li r29, 0x0
    b @L_80185424
    @L_801853F8:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185420
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185420
    mr r28, r3
    b @L_80185434
    @L_80185420:
    addi r29, r29, 0x1
    @L_80185424:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801853F8
    li r28, 0x0
    @L_80185434:
    cmplwi r28, 0x0
    beq @L_80185494
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0x24(r1)
    li r0, 0x1
    stw r3, 0x50(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x48(r1)
    lwz r0, 0x4c(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x54(r1)
    lfd f1, 0x50(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f27
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
    @L_80185494:
    li r0, 0x2
    stb r0, 0x55(r31)
    @L_8018549C:
    lwz r3, 0x28(r31)
    lwz r4, 0x2c(r31)
    lfs f1, 0x58(r31)
    bl fn_80188214
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    bl fn_800E0168
    addi r3, r1, 0x38
    bl fn_800E008C
    fmr f31, f1
    lfd f28, lbl_8047D7F0(r2)
    lfs f29, lbl_8047D7A0(r2)
    lfd f30, lbl_8047D7A8(r2)
    @L_801854DC:
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    addi r5, r31, 0x5c
    bl fn_800E0168
    addi r3, r1, 0x38
    bl fn_800E008C
    fmr f27, f1
    fcmpo cr0, f27, f31
    bge @L_80185840
    addi r3, r31, 0x9c
    addi r4, r31, 0x5c
    bl fn_800E01D0
    lhz r3, 0x6a(r31)
    addi r0, r3, 0x1
    sth r0, 0x6a(r31)
    lhz r3, 0x6a(r31)
    lhz r0, 0x68(r31)
    cmplw r3, r0
    blt @L_80185664
    lbz r0, 0x54(r31)
    cmplwi r0, 0x3
    bne @L_80185540
    li r0, 0x0
    sth r0, 0x6a(r31)
    b @L_80185664
    @L_80185540:
    lwz r29, 0x2c(r31)
    li r30, 0x0
    lwz r28, 0x28(r31)
    b @L_80185588
    @L_80185550:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185584
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185584
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185584
    lwz r28, 0x4(r3)
    b @L_801855F4
    @L_80185584:
    addi r30, r30, 0x1
    @L_80185588:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185550
    li r27, 0x0
    b @L_801855E4
    @L_8018559C:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801855E0
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_801855E0
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_801855F4
    @L_801855E0:
    addi r27, r27, 0x1
    @L_801855E4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018559C
    li r28, 0x0
    @L_801855F4:
    li r29, 0x0
    b @L_80185628
    @L_801855FC:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185624
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185624
    mr r28, r3
    b @L_80185638
    @L_80185624:
    addi r29, r29, 0x1
    @L_80185628:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801855FC
    li r28, 0x0
    @L_80185638:
    cmplwi r28, 0x0
    beq @L_80185658
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FC74
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FBDC
    @L_80185658:
    li r0, 0x0
    stb r0, 0x54(r31)
    b @L_80185880
    @L_80185664:
    lhz r0, 0x6a(r31)
    addi r3, r31, 0x5c
    lwz r4, 0x70(r31)
    mulli r0, r0, 0xc
    add r4, r4, r0
    bl fn_800E01D0
    addi r3, r1, 0x14
    addi r4, r31, 0x5c
    addi r5, r31, 0x9c
    bl fn_800E0168
    lfs f1, 0x14(r1)
    lfs f2, 0x1c(r1)
    bl fn_800CE2D8
    frsp f0, f1
    fmr f2, f28
    fsubs f0, f0, f29
    fadd f1, f28, f0
    bl fn_800CE318
    frsp f26, f1
    fcmpo cr0, f26, f30
    ble @L_801856C8
    lfd f0, lbl_8047D7F0(r2)
    fsub f26, f26, f0
    frsp f26, f26
    b @L_801856E0
    @L_801856C8:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f26, f0
    bge @L_801856E0
    lfd f0, lbl_8047D7F0(r2)
    fadd f26, f0, f26
    frsp f26, f26
    @L_801856E0:
    lwz r29, 0x2c(r31)
    li r30, 0x0
    lwz r28, 0x28(r31)
    b @L_80185728
    @L_801856F0:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185724
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185724
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185724
    lwz r28, 0x4(r3)
    b @L_80185794
    @L_80185724:
    addi r30, r30, 0x1
    @L_80185728:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801856F0
    li r27, 0x0
    b @L_80185784
    @L_8018573C:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185780
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80185780
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80185794
    @L_80185780:
    addi r27, r27, 0x1
    @L_80185784:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018573C
    li r28, 0x0
    @L_80185794:
    li r29, 0x0
    b @L_801857C8
    @L_8018579C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801857C4
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_801857C4
    mr r28, r3
    b @L_801857D8
    @L_801857C4:
    addi r29, r29, 0x1
    @L_801857C8:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018579C
    li r28, 0x0
    @L_801857D8:
    cmplwi r28, 0x0
    beq @L_80185838
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x48(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D7C4(r2)
    fctiwz f1, f1
    stfd f1, 0x50(r1)
    lwz r0, 0x54(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x4c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f26
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
    @L_80185838:
    fsubs f31, f31, f27
    b @L_801854DC
    @L_80185840:
    addi r3, r1, 0x38
    addi r4, r31, 0x5c
    addi r5, r31, 0x9c
    bl fn_800E0168
    fdivs f1, f31, f27
    addi r3, r1, 0x38
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    mr r5, r3
    bl fn_800E019C
    mr r3, r31
    addi r4, r1, 0x38
    addi r5, r31, 0x9c
    bl fn_8018E9B4
    @L_80185880:
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    psq_l f28, 0x98(r1), 0, 0
    lfd f28, 0x90(r1)
    psq_l f27, 0x88(r1), 0, 0
    lfd f27, 0x80(r1)
    psq_l f26, 0x78(r1), 0, 0
    lfd f26, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
