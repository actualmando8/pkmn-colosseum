    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stfd f30, 0xf0(r1)
    psq_st f30, 0xf8(r1), 0, 0
    stfd f29, 0xe0(r1)
    psq_st f29, 0xe8(r1), 0, 0
    stw r31, 0xdc(r1)
    stw r30, 0xd8(r1)
    stw r29, 0xd4(r1)
    stw r28, 0xd0(r1)
    mr r31, r3
    lwz r28, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BCBC(r2)
    addi r3, r3, lbl_803A6818@l
    cmplwi r28, 0x0
    lfs f30, lbl_8047BCC0(r2)
    stfs f0, 0x8(r1)
    lwz r0, 0x0(r3)
    beq @L_80048578
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r30, r3, r0
    cmplwi r30, 0x8000
    blt @L_80048518
    clrlwi r30, r30, 18
    @L_80048518:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r28
    clrlwi r4, r30, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r30
    li r3, 0x0
    bl fn_8025FDDC
    mr r29, r3
    mr r4, r30
    li r3, 0x0
    bl fn_8025FD34
    mr r30, r3
    mr r3, r28
    mr r4, r29
    bl fn_8011DFE0
    mr r3, r28
    mr r4, r30
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8004857C
    @L_80048578:
    li r3, 0x0
    @L_8004857C:
    bl fn_8011F5C8
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_8025FA20
    lfs f0, 0x8(r1)
    cmplwi r31, 0x0
    fmuls f30, f30, f0
    bne @L_800485A4
    li r3, 0x0
    b @L_800488E0
    @L_800485A4:
    lwz r0, 0x34(r31)
    cmplwi r0, 0x0
    bne @L_800485B8
    li r3, 0x0
    b @L_800488E0
    @L_800485B8:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x2
    beq @L_800485CC
    li r3, 0x0
    b @L_800488E0
    @L_800485CC:
    mr r3, r31
    li r4, 0x0
    bl fn_80109B90
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800485EC
    li r3, 0x0
    b @L_800488E0
    @L_800485EC:
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_80048674
    lwz r3, 0x24(r31)
    bl fn_801DAC3C
    mr r0, r3
    lwz r3, 0x24(r31)
    mr r28, r0
    bl fn_801DAC24
    cmpwi r3, 0x1
    beq @L_8004865C
    bge @L_80048634
    cmpwi r3, -0x1
    beq @L_8004864C
    bge @L_80048654
    cmpwi r3, -0x2
    bge @L_80048644
    b @L_80048678
    @L_80048634:
    cmpwi r3, 0x3
    beq @L_8004866C
    bge @L_80048678
    b @L_80048664
    @L_80048644:
    lfs f31, lbl_8047BD3C(r2)
    b @L_80048678
    @L_8004864C:
    lfs f31, lbl_8047BD40(r2)
    b @L_80048678
    @L_80048654:
    lfs f31, lbl_8047BD44(r2)
    b @L_80048678
    @L_8004865C:
    lfs f31, lbl_8047BD44(r2)
    b @L_80048678
    @L_80048664:
    lfs f31, lbl_8047BD48(r2)
    b @L_80048678
    @L_8004866C:
    lfs f31, lbl_8047BD4C(r2)
    b @L_80048678
    @L_80048674:
    lwz r28, 0x24(r31)
    @L_80048678:
    cmplwi r28, 0x0
    bne @L_80048688
    li r3, 0x0
    b @L_800488E0
    @L_80048688:
    li r3, 0x3
    bl fn_80177A44
    lwz r3, 0x38(r31)
    addi r4, r1, 0x18
    addi r5, r1, 0x14
    addi r6, r1, 0x10
    addi r7, r1, 0xc
    bl fn_800D1FDC
    mr r3, r28
    bl fn_800E3C5C
    mr r30, r3
    addi r4, r1, 0x4c
    bl fn_80190E34
    lwz r4, 0x2c(r31)
    lis r3, 0x4330
    lwz r0, 0x30(r31)
    xoris r4, r4, 0x8000
    lfs f0, lbl_8047BD30(r2)
    xoris r0, r0, 0x8000
    stw r4, 0xbc(r1)
    lfd f3, lbl_8047BCB0(r2)
    stw r3, 0xb8(r1)
    lfs f4, 0x4c(r1)
    lfd f1, 0xb8(r1)
    stw r0, 0xc4(r1)
    fsubs f2, f1, f3
    lfs f5, 0x50(r1)
    stw r3, 0xc0(r1)
    fcmpo cr0, f5, f4
    lfd f1, 0xc0(r1)
    stfs f0, 0x18(r1)
    fsubs f0, f1, f3
    fdivs f0, f2, f0
    stfs f0, 0x14(r1)
    cror eq, gt, eq
    bne @L_80048720
    fmr f29, f5
    b @L_80048724
    @L_80048720:
    fmr f29, f4
    @L_80048724:
    lfs f1, lbl_8047BCBC(r2)
    lfs f0, 0x8(r1)
    fcmpu cr0, f1, f0
    bne @L_8004874C
    lfs f1, lbl_8047BD34(r2)
    bl fn_800CE220
    frsp f1, f1
    fmuls f0, f29, f30
    fdivs f3, f0, f1
    b @L_80048764
    @L_8004874C:
    lfs f1, lbl_8047BD34(r2)
    bl fn_800CE220
    fdivs f0, f29, f31
    frsp f1, f1
    fmuls f0, f30, f0
    fdivs f3, f0, f1
    @L_80048764:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BC94(r2)
    addi r4, r3, lbl_803A6818@l
    lfs f0, 0x64(r4)
    fmr f2, f1
    addi r3, r1, 0x34
    fmuls f3, f3, f0
    bl fn_800E01F4
    lis r4, lbl_803A6818@ha
    addi r3, r1, 0x88
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x70(r4)
    fneg f1, f0
    bl fn_800E0518
    lis r4, lbl_803A6818@ha
    addi r3, r1, 0x88
    addi r4, r4, lbl_803A6818@l
    lfs f0, 0x6c(r4)
    fneg f1, f0
    bl fn_800E0370
    addi r3, r1, 0x34
    addi r4, r1, 0x88
    mr r5, r3
    bl fn_800DFF98
    lwz r3, 0x38(r31)
    addi r4, r1, 0x34
    bl fn_800D207C
    lwz r3, 0x38(r31)
    lfs f1, 0x18(r1)
    lfs f2, 0x14(r1)
    lfs f3, 0x10(r1)
    lfs f4, 0xc(r1)
    bl fn_800D20CC
    lfs f1, lbl_8047BC94(r2)
    addi r3, r1, 0x58
    lfs f0, lbl_8047BD18(r2)
    stfs f1, 0x40(r1)
    stfs f1, 0x48(r1)
    lfs f2, 0x14(r30)
    lfs f1, 0x20(r30)
    fadds f1, f2, f1
    fneg f1, f1
    fmuls f0, f1, f0
    stfs f0, 0x44(r1)
    bl fn_800E064C
    addi r3, r1, 0x58
    addi r4, r1, 0x40
    bl fn_800E0560
    mr r3, r28
    addi r4, r1, 0x58
    bl fn_800E4598
    lis r3, lbl_802E53F4@ha
    lis r5, lbl_802E5400@ha
    addi r4, r3, lbl_802E53F4@l
    lwz r3, 0x38(r31)
    addi r5, r5, lbl_802E5400@l
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x28
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x34
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x1c(r1)
    stw r6, 0x20(r1)
    stw r0, 0x24(r1)
    bl memcpy
    lfs f3, 0x28(r1)
    li r4, 0x2
    lfs f2, lbl_8047BC98(r2)
    lfs f1, 0x2c(r1)
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    fadds f0, f1, f0
    stfs f2, 0x28(r1)
    stfs f0, 0x2c(r1)
    lwz r3, 0x44(r31)
    bl fn_800DCCF0
    lwz r3, 0x44(r31)
    addi r4, r1, 0x1c
    bl fn_800DCC84
    lwz r3, 0x44(r31)
    addi r4, r1, 0x28
    bl fn_800DCC60
    lwz r3, 0x44(r31)
    addi r4, r1, 0x40
    bl fn_800DCC3C
    lwz r3, 0x44(r31)
    li r4, 0x1
    bl fn_800DCC34
    li r3, 0x4
    bl fn_80177A44
    li r3, 0x1
    @L_800488E0:
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    psq_l f30, 0xf8(r1), 0, 0
    lfd f30, 0xf0(r1)
    psq_l f29, 0xe8(r1), 0, 0
    lfd f29, 0xe0(r1)
    lwz r31, 0xdc(r1)
    lwz r30, 0xd8(r1)
    lwz r29, 0xd4(r1)
    lwz r0, 0x114(r1)
    lwz r28, 0xd0(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
