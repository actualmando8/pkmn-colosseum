    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stfd f30, 0xf0(r1)
    psq_st f30, 0xf8(r1), 0, 0
    stw r31, 0xec(r1)
    stw r30, 0xe8(r1)
    stw r29, 0xe4(r1)
    stw r28, 0xe0(r1)
    mr r29, r3
    mr r28, r4
    cmplwi r29, 0x0
    bne @L_800478F8
    li r3, 0x0
    b @L_80047C90
    @L_800478F8:
    cmplwi r28, 0x0
    bne @L_80047908
    li r3, 0x0
    b @L_80047C90
    @L_80047908:
    lbz r0, 0x14(r29)
    cmplwi r0, 0x0
    beq @L_8004793C
    lwz r3, 0x24(r29)
    bl fn_801DAC3C
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80047930
    li r3, 0x0
    b @L_80047C90
    @L_80047930:
    lwz r3, 0x24(r29)
    bl fn_801DAC24
    b @L_80047940
    @L_8004793C:
    lwz r31, 0x24(r29)
    @L_80047940:
    cmplwi r28, 0x0
    beq @L_8004794C
    lwz r30, 0x24(r28)
    @L_8004794C:
    cmplwi r31, 0x0
    bne @L_80047964
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x0(r29)
    b @L_80047C90
    @L_80047964:
    cmplwi r30, 0x0
    bne @L_80047974
    li r3, 0x0
    b @L_80047C90
    @L_80047974:
    li r3, 0x3
    bl fn_80177A44
    lwz r3, 0x38(r29)
    addi r4, r1, 0x14
    addi r5, r1, 0x10
    addi r6, r1, 0xc
    addi r7, r1, 0x8
    bl fn_800D1FDC
    mr r3, r31
    bl fn_800E3C5C
    addi r4, r1, 0x3c
    bl fn_80190E34
    mr r3, r30
    bl fn_800E3C5C
    addi r4, r1, 0x30
    bl fn_80190E34
    lfs f31, 0x40(r1)
    lfs f2, 0x34(r1)
    lfs f1, 0x3c(r1)
    lfs f0, 0x30(r1)
    fcmpo cr0, f31, f2
    fadds f4, f1, f0
    cror eq, gt, eq
    bne @L_800479D8
    b @L_800479DC
    @L_800479D8:
    fmr f31, f2
    @L_800479DC:
    lwz r4, 0x2c(r29)
    lis r3, 0x4330
    lwz r0, 0x30(r29)
    fcmpo cr0, f31, f4
    xoris r4, r4, 0x8000
    lfs f0, lbl_8047BD30(r2)
    xoris r0, r0, 0x8000
    stw r4, 0xd4(r1)
    lfd f3, lbl_8047BCB0(r2)
    stw r3, 0xd0(r1)
    lfd f1, 0xd0(r1)
    stw r0, 0xdc(r1)
    fsubs f2, f1, f3
    stw r3, 0xd8(r1)
    lfd f1, 0xd8(r1)
    stfs f0, 0x14(r1)
    fsubs f0, f1, f3
    fdivs f0, f2, f0
    stfs f0, 0x10(r1)
    cror eq, gt, eq
    bne @L_80047A38
    fmr f30, f31
    b @L_80047A3C
    @L_80047A38:
    fmr f30, f4
    @L_80047A3C:
    lfs f1, lbl_8047BD34(r2)
    bl fn_800CE220
    frsp f0, f1
    lfs f1, lbl_8047BC94(r2)
    addi r3, r1, 0x54
    fmr f2, f1
    fdivs f30, f30, f0
    fmr f3, f30
    bl fn_800E01F4
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0x54
    addi r3, r3, lbl_803A6818@l
    addi r3, r3, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5430@ha
    addi r5, r4, lbl_803A6818@l
    addi r4, r3, lbl_802E5430@l
    addi r3, r5, 0x1f4
    bl fn_800E01D0
    lis r3, lbl_803A6818@ha
    lfs f1, 0x14(r1)
    addi r3, r3, lbl_803A6818@l
    lfs f2, 0x10(r1)
    lfs f3, 0xc(r1)
    lfs f4, 0x8(r1)
    stfs f1, 0x200(r3)
    stfs f2, 0x204(r3)
    stfs f3, 0x208(r3)
    stfs f4, 0x20c(r3)
    stfs f30, 0x210(r3)
    lwz r3, 0x38(r29)
    bl fn_800D20CC
    lwz r3, 0x38(r29)
    addi r4, r1, 0x54
    bl fn_800D207C
    lfs f0, lbl_8047BD18(r2)
    lis r3, lbl_802E5430@ha
    addi r5, r3, lbl_802E5430@l
    fmuls f0, f31, f0
    lis r3, lbl_802E5424@ha
    addi r4, r3, lbl_802E5424@l
    stfs f0, 0x4(r5)
    lwz r3, 0x38(r29)
    bl fn_800D1F04
    mr r3, r31
    addi r4, r1, 0x60
    bl fn_800E3D98
    lfs f1, 0x3c(r1)
    mr r3, r31
    lfs f0, lbl_8047BD18(r2)
    addi r4, r1, 0x60
    lfs f2, lbl_8047BC94(r2)
    fmuls f0, f1, f0
    stfs f2, 0x64(r1)
    fneg f0, f0
    stfs f2, 0x68(r1)
    stfs f0, 0x60(r1)
    bl fn_800E43A4
    addi r3, r1, 0x9c
    bl fn_800E064C
    addi r3, r1, 0x9c
    addi r4, r1, 0x60
    bl fn_800E03B4
    mr r3, r31
    addi r4, r1, 0x9c
    bl fn_800E4598
    mr r3, r30
    addi r4, r1, 0x48
    bl fn_800E3D98
    lfs f1, 0x30(r1)
    mr r3, r30
    lfs f0, lbl_8047BD18(r2)
    addi r4, r1, 0x48
    lfs f2, lbl_8047BC94(r2)
    fmuls f0, f1, f0
    stfs f2, 0x4c(r1)
    stfs f0, 0x48(r1)
    stfs f2, 0x50(r1)
    bl fn_800E43A4
    addi r3, r1, 0x6c
    bl fn_800E064C
    addi r3, r1, 0x6c
    addi r4, r1, 0x48
    bl fn_800E03B4
    mr r3, r30
    addi r4, r1, 0x6c
    bl fn_800E4598
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x24
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x54
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x18(r1)
    stw r6, 0x1c(r1)
    stw r0, 0x20(r1)
    bl memcpy
    lfs f3, 0x24(r1)
    li r4, 0x2
    lfs f2, lbl_8047BC98(r2)
    lfs f1, 0x28(r1)
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    fadds f0, f1, f0
    stfs f2, 0x24(r1)
    stfs f0, 0x28(r1)
    lwz r3, 0x44(r29)
    bl fn_800DCCF0
    lwz r3, 0x44(r29)
    addi r4, r1, 0x18
    bl fn_800DCC84
    lwz r3, 0x44(r29)
    addi r4, r1, 0x24
    bl fn_800DCC60
    lwz r3, 0x44(r29)
    addi r4, r1, 0x60
    bl fn_800DCC3C
    lwz r3, 0x44(r29)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r29, 0x114(r4)
    cmplwi r29, 0x0
    beq @L_80047C7C
    mr r3, r29
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r29
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r29
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_80047C7C:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r3, 0x1
    @L_80047C90:
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    psq_l f30, 0xf8(r1), 0, 0
    lfd f30, 0xf0(r1)
    lwz r31, 0xec(r1)
    lwz r30, 0xe8(r1)
    lwz r29, 0xe4(r1)
    lwz r0, 0x114(r1)
    lwz r28, 0xe0(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
