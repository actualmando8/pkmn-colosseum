    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stfd f31, 0xf0(r1)
    psq_st f31, 0xf8(r1), 0, 0
    stfd f30, 0xe0(r1)
    psq_st f30, 0xe8(r1), 0, 0
    stfd f29, 0xd0(r1)
    psq_st f29, 0xd8(r1), 0, 0
    stfd f28, 0xc0(r1)
    psq_st f28, 0xc8(r1), 0, 0
    stfd f27, 0xb0(r1)
    psq_st f27, 0xb8(r1), 0, 0
    stfd f26, 0xa0(r1)
    psq_st f26, 0xa8(r1), 0, 0
    stfd f25, 0x90(r1)
    psq_st f25, 0x98(r1), 0, 0
    stfd f24, 0x80(r1)
    psq_st f24, 0x88(r1), 0, 0
    stfd f23, 0x70(r1)
    psq_st f23, 0x78(r1), 0, 0
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    stw r29, 0x64(r1)
    stw r28, 0x60(r1)
    mr r28, r3
    mr r29, r4
    lfs f31, 0x20(r3)
    lfs f30, 0x18(r3)
    lfs f29, 0x24(r3)
    lfs f28, 0x1c(r3)
    lhz r3, 0x46(r3)
    lhz r4, 0x48(r28)
    bl fn_800F9318
    mr. r31, r3
    li r30, 0x0
    bne @L_80139414
    li r3, 0x0
    b @L_801397B8
    @L_80139414:
    lwz r3, 0x8(r28)
    bl fn_800E4C98
    stw r3, 0x58(r29)
    lwz r3, 0x58(r29)
    cmplwi r3, 0x0
    bne @L_80139444
    lis r3, lbl_80272CC4@ha
    addi r3, r3, lbl_80272CC4@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801397B8
    @L_80139444:
    li r4, 0x1
    bl fn_800E4014
    lha r4, 0x4a(r28)
    extsh. r0, r4
    blt @L_80139464
    mr r3, r31
    bl fn_800EE150
    mr r30, r3
    @L_80139464:
    cmplwi r30, 0x0
    beq @L_8013948C
    mr r3, r30
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r30
    bl fn_800EE828
    b @L_80139498
    @L_8013948C:
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_800E3D98
    @L_80139498:
    bl fn_800E0BA0
    lfs f3, 0x14(r28)
    lfs f2, 0x10(r28)
    lfs f0, lbl_8047D168(r2)
    fmadds f1, f3, f1, f2
    stfs f1, 0x50(r29)
    lfs f2, 0x50(r29)
    lfs f1, 0xc(r28)
    fmuls f1, f2, f1
    stfs f1, 0x50(r29)
    stfs f0, 0x54(r29)
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f26, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f27, f0, f1
    fmr f1, f27
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    fmr f1, f27
    bl fn_800CDBE0
    frsp f27, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f0, f1
    addi r3, r1, 0x14
    fmr f2, f27
    fmuls f3, f25, f24
    fmuls f1, f25, f0
    bl fn_800E01F4
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    addi r3, r29, 0x30
    addi r4, r1, 0x14
    fmuls f1, f0, f1
    bl fn_800E0718
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f27, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f26, f0, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    fmr f1, f27
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f27
    bl fn_800CDBE0
    frsp f0, f1
    addi r3, r1, 0x8
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    addi r3, r29, 0x40
    addi r4, r1, 0x8
    fmuls f1, f0, f1
    bl fn_800E0718
    mr r3, r29
    addi r4, r1, 0x20
    bl fn_800E01D0
    addi r31, r29, 0x18
    bl fn_800E0BA0
    fmadds f27, f29, f1, f31
    lfs f0, 0x50(r29)
    fmuls f27, f27, f0
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f23, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f26, f0, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    fmr f1, f23
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f23
    bl fn_800CDBE0
    frsp f0, f1
    mr r3, r31
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    fmr f1, f27
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x20
    bl fn_800E019C
    addi r31, r29, 0xc
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f23, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D188(r2)
    fmuls f26, f0, f1
    fmr f1, f26
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BA0
    fmadds f27, f28, f1, f30
    bl fn_800E0BA0
    lfs f3, 0x34(r28)
    lfs f2, 0x30(r28)
    lfs f0, 0x50(r29)
    fmadds f2, f3, f1, f2
    fmr f1, f23
    fmuls f27, f27, f2
    fmuls f27, f27, f0
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f26
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f23
    bl fn_800CDBE0
    frsp f0, f1
    mr r3, r31
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    fmr f1, f27
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x20
    bl fn_800E019C
    addi r31, r29, 0x24
    bl fn_800E0BE4
    lfs f0, lbl_8047D180(r2)
    fmuls f28, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D184(r2)
    fmuls f23, f0, f1
    fmr f1, f23
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BA0
    fmadds f27, f29, f1, f31
    lfs f0, 0x50(r29)
    fmr f1, f28
    fmuls f27, f27, f0
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f23
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f28
    bl fn_800CDBE0
    frsp f0, f1
    mr r3, r31
    fmr f2, f26
    fmuls f3, f24, f25
    fmuls f1, f24, f0
    bl fn_800E01F4
    fmr f1, f27
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    mr r3, r31
    mr r4, r31
    addi r5, r29, 0xc
    bl fn_800E019C
    addi r3, r1, 0x2c
    addi r4, r29, 0x30
    bl fn_800E040C
    lfs f1, 0x50(r29)
    addi r3, r1, 0x2c
    fmr f2, f1
    fmr f3, f1
    bl fn_800E02C4
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    bl fn_800E03B4
    lwz r3, 0x58(r29)
    addi r4, r1, 0x2c
    bl fn_800E4598
    li r3, 0x1
    @L_801397B8:
    psq_l f31, 0xf8(r1), 0, 0
    lfd f31, 0xf0(r1)
    psq_l f30, 0xe8(r1), 0, 0
    lfd f30, 0xe0(r1)
    psq_l f29, 0xd8(r1), 0, 0
    lfd f29, 0xd0(r1)
    psq_l f28, 0xc8(r1), 0, 0
    lfd f28, 0xc0(r1)
    psq_l f27, 0xb8(r1), 0, 0
    lfd f27, 0xb0(r1)
    psq_l f26, 0xa8(r1), 0, 0
    lfd f26, 0xa0(r1)
    psq_l f25, 0x98(r1), 0, 0
    lfd f25, 0x90(r1)
    psq_l f24, 0x88(r1), 0, 0
    lfd f24, 0x80(r1)
    psq_l f23, 0x78(r1), 0, 0
    lfd f23, 0x70(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    lwz r29, 0x64(r1)
    lwz r0, 0x104(r1)
    lwz r28, 0x60(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
