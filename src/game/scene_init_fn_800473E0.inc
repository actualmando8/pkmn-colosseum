    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stmw r26, 0x68(r1)
    mr r29, r3
    lwz r30, 0x0(r29)
    cmplwi r30, 0x0
    beq @L_80047880
    lbz r3, 0x0(r30)
    lis r4, 0x4330
    stw r4, 0x30(r1)
    addi r27, r3, 0x2
    lfd f2, lbl_8047BCD8(r2)
    clrlwi r28, r27, 24
    stw r28, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f2
    stfs f0, 0x4(r29)
    lwz r3, 0x18(r29)
    cmpw r3, r28
    bge @L_80047460
    addi r0, r3, 0x1
    stw r0, 0x18(r29)
    b @L_80047534
    @L_80047460:
    lwz r3, 0x10(r29)
    cmpw r3, r28
    bge @L_80047478
    addi r0, r3, 0x1
    stw r0, 0x10(r29)
    b @L_80047534
    @L_80047478:
    addi r0, r28, 0x1e
    cmpw r3, r0
    bge @L_800474B8
    stw r28, 0x34(r1)
    addi r0, r3, 0x1
    stw r4, 0x30(r1)
    lfd f0, 0x30(r1)
    stw r0, 0x10(r29)
    fsubs f0, f0, f2
    lfs f1, 0x14(r29)
    fcmpo cr0, f1, f0
    bge @L_80047534
    lfs f0, 0x20(r29)
    fadds f0, f1, f0
    stfs f0, 0x14(r29)
    b @L_80047534
    @L_800474B8:
    stw r28, 0x34(r1)
    lfs f1, 0xc(r29)
    stw r4, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f2
    fcmpo cr0, f1, f0
    bge @L_8004750C
    lfs f0, 0x20(r29)
    stw r28, 0x34(r1)
    fadds f1, f1, f0
    stw r4, 0x30(r1)
    lfd f0, 0x30(r1)
    stfs f1, 0xc(r29)
    fsubs f0, f0, f2
    lfs f1, 0x14(r29)
    fcmpo cr0, f1, f0
    bge @L_80047534
    lfs f0, 0x20(r29)
    fadds f0, f1, f0
    stfs f0, 0x14(r29)
    b @L_80047534
    @L_8004750C:
    stw r28, 0x34(r1)
    lfs f1, 0x14(r29)
    stw r4, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f2
    fcmpo cr0, f1, f0
    bge @L_80047534
    lfs f0, 0x20(r29)
    fadds f0, f1, f0
    stfs f0, 0x14(r29)
    @L_80047534:
    clrlwi r5, r27, 24
    li r3, 0x3
    li r4, 0x1
    bl fn_80176228
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80047880
    clrlwi r4, r27, 24
    lis r0, 0x4330
    stw r4, 0x34(r1)
    addi r4, r1, 0x20
    lfs f1, lbl_8047BC94(r2)
    stw r0, 0x30(r1)
    lfd f2, lbl_8047BCD8(r2)
    lfd f0, 0x30(r1)
    lfs f3, lbl_8047BCBC(r2)
    fsubs f0, f0, f2
    stfs f1, 0x20(r1)
    stfs f1, 0x24(r1)
    fdivs f28, f3, f0
    lfs f0, 0x1c(r29)
    stfs f0, 0x28(r1)
    lfs f29, 0x4(r29)
    bl fn_801760C4
    lfs f0, lbl_8047BD20(r2)
    fmr f31, f28
    lfs f1, 0x20(r1)
    subi r27, r28, 0x2
    fdivs f30, f0, f29
    li r26, 0x0
    fadds f0, f1, f30
    stfs f0, 0x20(r1)
    lfd f29, lbl_8047BCB0(r2)
    lis r28, 0x4330
    b @L_80047610
    @L_800475C0:
    addi r0, r26, 0x1
    stw r28, 0x30(r1)
    lbzx r0, r30, r0
    fmr f1, f31
    mr r3, r31
    addi r4, r1, 0x20
    extsb r0, r0
    xoris r0, r0, 0x8000
    stw r0, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f2, f0, f29
    fneg f0, f2
    stfs f2, 0x24(r1)
    stfs f0, 0x24(r1)
    bl fn_801760C4
    lfs f0, 0x20(r1)
    fadds f31, f31, f28
    addi r26, r26, 0x1
    fadds f0, f0, f30
    stfs f0, 0x20(r1)
    @L_80047610:
    cmpw r26, r27
    blt @L_800475C0
    lfs f1, lbl_8047BC94(r2)
    mr r3, r31
    lfs f0, lbl_8047BD20(r2)
    addi r4, r1, 0x20
    stfs f1, 0x24(r1)
    lfs f1, lbl_8047BCBC(r2)
    stfs f0, 0x20(r1)
    bl fn_801760C4
    li r26, 0x0
    li r27, 0x0
    lfs f29, lbl_8047BC94(r2)
    b @L_80047674
    @L_80047648:
    lwz r0, 0xc(r31)
    addi r3, r1, 0x14
    add r4, r0, r27
    bl fn_800E01D0
    stfs f29, 0x1c(r1)
    addi r4, r1, 0x14
    lwz r0, 0xc(r31)
    add r3, r0, r27
    bl fn_800E01D0
    addi r27, r27, 0xc
    addi r26, r26, 0x1
    @L_80047674:
    lfs f0, 0xc(r29)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r28, 0x34(r1)
    cmpw r26, r28
    blt @L_80047648
    mulli r27, r28, 0xc
    lfd f29, lbl_8047BCB0(r2)
    lis r30, 0x4330
    b @L_80047724
    @L_8004769C:
    lwz r0, 0xc(r31)
    addi r3, r1, 0x14
    add r4, r0, r27
    bl fn_800E01D0
    lfs f1, 0xc(r29)
    addi r4, r1, 0x14
    lfs f0, 0x14(r29)
    fctiwz f1, f1
    stw r30, 0x40(r1)
    fctiwz f0, f0
    lfs f2, 0x1c(r29)
    stw r30, 0x48(r1)
    stfd f1, 0x30(r1)
    stfd f0, 0x38(r1)
    lwz r5, 0x34(r1)
    lwz r3, 0x3c(r1)
    subf r0, r5, r28
    subf r3, r5, r3
    xoris r3, r3, 0x8000
    xoris r0, r0, 0x8000
    stw r3, 0x44(r1)
    lfd f0, 0x40(r1)
    stw r0, 0x4c(r1)
    fsubs f1, f0, f29
    lfd f0, 0x48(r1)
    fdivs f1, f2, f1
    fsubs f0, f0, f29
    fmuls f0, f1, f0
    stfs f0, 0x1c(r1)
    lwz r0, 0xc(r31)
    add r3, r0, r27
    bl fn_800E01D0
    addi r27, r27, 0xc
    addi r28, r28, 0x1
    @L_80047724:
    lfs f0, 0x14(r29)
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    cmpw r28, r0
    blt @L_8004769C
    li r28, 0x0
    mr r27, r28
    lis r3, lbl_80314E08@ha
    lfs f31, lbl_8047BD24(r2)
    lfs f29, lbl_8047BD2C(r2)
    addi r30, r3, lbl_80314E08@l
    lfs f30, lbl_8047BD28(r2)
    b @L_80047868
    @L_8004775C:
    lwz r0, 0xc(r31)
    addi r3, r1, 0x14
    add r4, r0, r27
    bl fn_800E01D0
    addi r0, r28, 0x1
    lwz r4, 0xc(r31)
    mulli r0, r0, 0xc
    addi r3, r1, 0x8
    add r4, r4, r0
    bl fn_800E01D0
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    mr r3, r30
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D6A00
    lfs f1, lbl_8047BCA8(r2)
    bl fn_800D5648
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, 0x14(r1)
    lfs f0, 0x18(r1)
    fadds f1, f31, f1
    fmadds f0, f29, f0, f30
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x48(r1)
    stfd f0, 0x40(r1)
    lwz r3, 0x4c(r1)
    lwz r4, 0x44(r1)
    bl fn_800D61E4
    lfs f0, 0x1c(r1)
    li r3, 0x0
    fctiwz f0, f0
    stfd f0, 0x38(r1)
    lwz r0, 0x3c(r1)
    clrlwi r0, r0, 24
    oris r4, r0, 0x361e
    ori r4, r4, 0x500
    bl fn_800D5BA0
    lfs f1, 0x8(r1)
    lfs f0, 0xc(r1)
    fadds f1, f31, f1
    fmadds f0, f29, f0, f30
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x30(r1)
    stfd f0, 0x50(r1)
    lwz r3, 0x34(r1)
    lwz r4, 0x54(r1)
    bl fn_800D61E4
    lfs f0, 0x10(r1)
    li r3, 0x0
    fctiwz f0, f0
    stfd f0, 0x58(r1)
    lwz r0, 0x5c(r1)
    clrlwi r0, r0, 24
    oris r4, r0, 0x361e
    ori r4, r4, 0x500
    bl fn_800D5BA0
    bl fn_800D6728
    lfs f1, lbl_8047BCBC(r2)
    bl fn_800D5648
    addi r27, r27, 0xc
    addi r28, r28, 0x1
    @L_80047868:
    lwz r3, 0x18(r29)
    subi r0, r3, 0x1
    cmpw r28, r0
    blt @L_8004775C
    mr r3, r31
    bl fn_80176068
    @L_80047880:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    lmw r26, 0x68(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
