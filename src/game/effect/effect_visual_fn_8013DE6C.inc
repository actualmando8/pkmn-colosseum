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
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stfd f26, 0x60(r1)
    psq_st f26, 0x68(r1), 0, 0
    stfd f25, 0x50(r1)
    psq_st f25, 0x58(r1), 0, 0
    stfd f24, 0x40(r1)
    psq_st f24, 0x48(r1), 0, 0
    stmw r24, 0x20(r1)
    mr. r27, r3
    beq @L_8013E200
    lwz r0, 0x10(r27)
    lwz r31, 0x0(r27)
    cmpwi r0, 0x0
    lwz r25, 0x18(r27)
    lwz r29, 0x8(r27)
    bne @L_8013DEE4
    cmplwi r31, 0x0
    bne @L_8013DEEC
    @L_8013DEE4:
    li r3, 0x1
    b @L_8013E204
    @L_8013DEEC:
    mr r3, r31
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_8013DF04
    li r3, 0x1
    b @L_8013E204
    @L_8013DF04:
    lwz r4, 0x1c(r27)
    lis r3, 0x4330
    lwz r0, 0x8(r25)
    stw r4, 0xc(r1)
    lfd f3, lbl_8047D288(r2)
    stw r3, 0x8(r1)
    lfs f4, 0x0(r25)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f2, f0, f3
    lfs f0, 0x4(r25)
    stw r3, 0x10(r1)
    fsubs f0, f0, f4
    lfd f1, 0x10(r1)
    fsubs f1, f1, f3
    fdivs f1, f2, f1
    fmadds f26, f1, f0, f4
    bl fn_800D3068
    cmpwi r3, 0x3
    beq @L_8013DFAC
    bge @L_8013DF68
    cmpwi r3, 0x1
    beq @L_8013DF74
    bge @L_8013DF90
    b @L_8013DFE0
    @L_8013DF68:
    cmpwi r3, 0x5
    bge @L_8013DFE0
    b @L_8013DFC8
    @L_8013DF74:
    lfs f1, lbl_8047D26C(r2)
    lfs f0, lbl_8047D268(r2)
    fmadds f0, f1, f26, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    b @L_8013DFE0
    @L_8013DF90:
    lfs f1, lbl_8047D270(r2)
    lfs f0, lbl_8047D268(r2)
    fmadds f0, f1, f26, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    b @L_8013DFE0
    @L_8013DFAC:
    lfs f1, lbl_8047D274(r2)
    lfs f0, lbl_8047D268(r2)
    fmadds f0, f1, f26, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    b @L_8013DFE0
    @L_8013DFC8:
    lfs f1, lbl_8047D278(r2)
    lfs f0, lbl_8047D268(r2)
    fmadds f0, f1, f26, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    @L_8013DFE0:
    cmpwi r30, 0x0
    bne @L_8013DFF0
    li r3, 0x1
    b @L_8013E204
    @L_8013DFF0:
    mr r3, r31
    bl fn_800EC53C
    fmr f31, f1
    mr r3, r31
    bl fn_800EC570
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    fmr f28, f1
    lfd f2, lbl_8047D290(r2)
    stw r0, 0x10(r1)
    lfs f1, lbl_8047D27C(r2)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f2
    fnmsubs f24, f28, f0, f31
    fcmpo cr0, f24, f1
    bge @L_8013E048
    fdivs f0, f31, f28
    fctiwz f0, f0
    fmr f24, f1
    stfd f0, 0x10(r1)
    lwz r30, 0x14(r1)
    @L_8013E048:
    cmpwi r30, 0x0
    bne @L_8013E058
    li r3, 0x1
    b @L_8013E204
    @L_8013E058:
    fcmpo cr0, f24, f31
    cror eq, gt, eq
    bne @L_8013E06C
    li r3, 0x1
    b @L_8013E204
    @L_8013E06C:
    lis r4, lbl_80363CC8@ha
    mr r3, r31
    addi r4, r4, lbl_80363CC8@l
    bl fn_800E6478
    bl fn_800D45F8
    mr r0, r3
    li r3, 0x2
    mr r28, r0
    bl fn_800D4604
    bl fn_800D2248
    lwz r25, 0x4(r27)
    li r24, 0x0
    b @L_8013E0CC
    @L_8013E0A0:
    lwz r3, 0x0(r25)
    cmplwi r3, 0x0
    beq @L_8013E0C4
    bl fn_800DF3F0
    ori r4, r3, 0x20
    lwz r3, 0x0(r25)
    bl fn_800DF550
    lwz r3, 0x0(r25)
    bl fn_800DF188
    @L_8013E0C4:
    addi r24, r24, 0x1
    addi r25, r25, 0x4
    @L_8013E0CC:
    cmpw r24, r29
    blt @L_8013E0A0
    mr r3, r31
    bl fn_800EC990
    xoris r0, r30, 0x8000
    lis r30, 0x4330
    stw r0, 0x14(r1)
    li r24, 0x1
    lfs f30, lbl_8047D268(r2)
    stw r30, 0x10(r1)
    lfd f29, lbl_8047D290(r2)
    fmuls f26, f30, f28
    lfd f0, 0x10(r1)
    lfs f1, lbl_8047D280(r2)
    fsubs f0, f0, f29
    fdivs f25, f1, f0
    b @L_8013E188
    @L_8013E110:
    xoris r0, r24, 0x8000
    stw r30, 0x10(r1)
    fmr f1, f24
    mr r3, r31
    stw r0, 0x14(r1)
    addi r24, r24, 0x1
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fmuls f27, f0, f25
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC134
    fmuls f0, f27, f27
    lwz r26, 0x4(r27)
    fadds f24, f24, f28
    li r25, 0x0
    fmuls f27, f30, f0
    b @L_8013E174
    @L_8013E158:
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_8013E16C
    fmr f1, f27
    bl fn_800DF21C
    @L_8013E16C:
    addi r25, r25, 0x1
    addi r26, r26, 0x4
    @L_8013E174:
    cmpw r25, r29
    blt @L_8013E158
    mr r3, r31
    li r4, 0x3010
    bl fn_800E3760
    @L_8013E188:
    fadds f0, f24, f26
    fcmpo cr0, f0, f31
    blt @L_8013E110
    lwz r27, 0x4(r27)
    li r26, 0x0
    b @L_8013E1C0
    @L_8013E1A0:
    lwz r3, 0x0(r27)
    cmplwi r3, 0x0
    beq @L_8013E1B8
    bl fn_800DF140
    lwz r3, 0x0(r27)
    bl fn_800DF504
    @L_8013E1B8:
    addi r26, r26, 0x1
    addi r27, r27, 0x4
    @L_8013E1C0:
    cmpw r26, r29
    blt @L_8013E1A0
    mr r3, r31
    bl fn_800E638C
    fmr f1, f31
    mr r3, r31
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC134
    mr r3, r31
    li r4, 0x3010
    bl fn_800E3760
    mr r3, r28
    bl fn_800D4604
    li r3, 0x1
    b @L_8013E204
    @L_8013E200:
    li r3, 0x0
    @L_8013E204:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    psq_l f26, 0x68(r1), 0, 0
    lfd f26, 0x60(r1)
    psq_l f25, 0x58(r1), 0, 0
    lfd f25, 0x50(r1)
    psq_l f24, 0x48(r1), 0, 0
    lfd f24, 0x40(r1)
    lmw r24, 0x20(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
