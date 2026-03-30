    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    lis r3, lbl_80466E30@ha
    lwz r5, lbl_8047DFA8(r2)
    addi r3, r3, lbl_80466E30@l
    lwz r4, lbl_8047DFAC(r2)
    lbz r0, 0x1(r3)
    stw r5, 0x10(r1)
    cmplwi r0, 0x1
    stw r4, 0xc(r1)
    bne @L_801C44F0
    lbz r0, 0x0(r3)
    cmpwi r0, 0x4
    beq @L_801C44F0
    bge @L_801C44F0
    cmpwi r0, 0x0
    beq @L_801C44F0
    bge @L_801C4458
    b @L_801C44F0
    @L_801C4458:
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f1, lbl_8047DFC8(r2)
    stw r0, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    lis r4, lbl_80466E30@ha
    stw r3, 0x24(r1)
    addi r4, r4, lbl_80466E30@l
    lfd f2, lbl_8047DFD0(r2)
    stw r0, 0x20(r1)
    lfs f0, 0x8(r4)
    lfd f1, 0x20(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f31
    fadds f0, f0, f1
    stfs f0, 0x8(r4)
    lfs f1, 0x8(r4)
    lfs f0, 0x4(r4)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801C44F0
    lfs f0, 0x4(r4)
    li r3, 0x0
    stfs f0, 0x8(r4)
    stb r3, 0x1(r4)
    lhz r0, 0x2(r4)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_801C44EC
    li r0, 0x2
    stb r0, 0x0(r4)
    b @L_801C44F0
    @L_801C44EC:
    stb r3, 0x0(r4)
    @L_801C44F0:
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801C45FC
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x24(r1)
    lfd f1, lbl_8047DFC8(r2)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    lis r4, lbl_80466E30@ha
    stw r3, 0x1c(r1)
    addi r3, r4, lbl_80466E30@l
    lfd f2, lbl_8047DFD0(r2)
    stw r0, 0x18(r1)
    lfs f0, 0x18(r3)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f31
    fadds f0, f0, f1
    stfs f0, 0x18(r3)
    lfs f1, 0x18(r3)
    lfs f0, 0x14(r3)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801C4574
    lfs f0, 0x14(r3)
    stfs f0, 0x18(r3)
    @L_801C4574:
    lis r3, lbl_80466E30@ha
    addi r5, r3, lbl_80466E30@l
    lbz r0, 0x1(r5)
    lfs f1, 0x8(r5)
    lfs f2, 0x4(r5)
    clrlwi r3, r0, 24
    lfs f3, 0x18(r5)
    lfs f4, 0x14(r5)
    lwz r4, 0x10(r5)
    lwz r12, 0xc(r5)
    mtctr r12
    bctrl
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_801C45FC
    lis r3, lbl_80466E30@ha
    li r0, 0x0
    addi r3, r3, lbl_80466E30@l
    stw r0, 0xc(r3)
    lwz r0, 0x10(r3)
    cmplwi r0, 0x0
    beq @L_801C45FC
    bl fn_80109710
    lis r4, lbl_80466E30@ha
    addi r4, r4, lbl_80466E30@l
    lwz r0, 0x10(r4)
    cmplw r0, r3
    beq @L_801C45EC
    lwz r3, 0x10(r4)
    bl fn_800EF5A4
    @L_801C45EC:
    lis r3, lbl_80466E30@ha
    li r0, 0x0
    addi r3, r3, lbl_80466E30@l
    stw r0, 0x10(r3)
    @L_801C45FC:
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lbz r0, 0x0(r3)
    cmpwi r0, 0x3
    bge @L_801C4620
    cmpwi r0, 0x0
    beq @L_801C47AC
    bge @L_801C4628
    b @L_801C47AC
    @L_801C4620:
    cmpwi r0, 0x5
    b @L_801C47AC
    @L_801C4628:
    lfs f1, lbl_8047DFB8(r2)
    lfs f0, 0x4(r3)
    fcmpu cr0, f1, f0
    bne @L_801C4640
    lfs f1, lbl_8047DFB0(r2)
    b @L_801C464C
    @L_801C4640:
    lfs f1, 0x8(r3)
    lfs f0, 0x4(r3)
    fdivs f1, f1, f0
    @L_801C464C:
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lhz r0, 0x2(r3)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_801C467C
    lfs f0, lbl_8047DFBC(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r29, 0x24(r1)
    b @L_801C4698
    @L_801C467C:
    lfs f0, lbl_8047DFBC(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    subfic r0, r0, 0xff
    clrlwi r29, r0, 24
    @L_801C4698:
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lhz r0, 0x2(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_801C46BC
    lwz r0, 0xc(r1)
    stw r0, 0x28(r1)
    b @L_801C46C4
    @L_801C46BC:
    lwz r0, 0x10(r1)
    stw r0, 0x28(r1)
    @L_801C46C4:
    lwz r0, 0x28(r1)
    li r3, 0x1
    stw r0, 0x8(r1)
    bl fn_800D9ED8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047DFB8(r2)
    lfs f3, lbl_8047DFC0(r2)
    fmr f2, f1
    lfs f4, lbl_8047DFC4(r2)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, lbl_8047DFB8(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    lbz r30, 0x9(r1)
    mr r7, r29
    lbz r31, 0xa(r1)
    li r3, 0x0
    lbz r4, 0x8(r1)
    mr r5, r30
    mr r6, r31
    bl fn_800D5CB8
    lfs f1, lbl_8047DFC0(r2)
    lfs f2, lbl_8047DFC4(r2)
    lfs f3, lbl_8047DFB8(r2)
    bl fn_800D6680
    lbz r4, 0x8(r1)
    mr r5, r30
    mr r6, r31
    mr r7, r29
    li r3, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    li r3, 0x0
    bl fn_800D9ED8
    @L_801C47AC:
    psq_l f31, 0x48(r1), 0, 0
    lwz r0, 0x54(r1)
    lfd f31, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
