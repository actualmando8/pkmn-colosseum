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
    stmw r24, 0x80(r1)
    mr r24, r3
    lfs f29, lbl_8047D154(r2)
    lwz r30, 0x4(r3)
    mr r25, r4
    lhz r27, 0x8(r3)
    li r29, 0x0
    b @L_80138AC8
    @L_8013887C:
    cmplwi r25, 0x0
    beq @L_801388D0
    bl fn_800E0BA0
    lfs f2, 0x58(r24)
    lfs f0, 0x54(r24)
    fmadds f31, f2, f1, f0
    bl fn_800E0BE4
    lfs f0, lbl_8047D15C(r2)
    fmuls f30, f0, f1
    fmr f1, f30
    bl fn_800CDBE0
    frsp f2, f1
    lfs f0, lbl_8047D154(r2)
    fmr f1, f30
    fmuls f2, f31, f2
    stfs f2, 0x960(r30)
    stfs f0, 0x964(r30)
    bl fn_800CE148
    frsp f0, f1
    fmuls f0, f31, f0
    stfs f0, 0x968(r30)
    @L_801388D0:
    addi r3, r1, 0x24
    addi r4, r30, 0x960
    bl fn_800E0060
    lfs f1, 0x24(r1)
    addi r3, r1, 0x3c
    lfs f2, lbl_8047D14C(r2)
    lfs f3, 0x2c(r1)
    bl fn_800E01F4
    addi r3, r1, 0x3c
    mr r4, r3
    bl fn_800E0060
    li r0, 0x0
    mr r31, r30
    stw r0, 0x978(r30)
    addi r26, r30, 0x960
    li r28, 0x0
    b @L_80138AB4
    @L_80138914:
    addi r3, r1, 0x18
    addi r4, r1, 0x3c
    bl fn_800E01D0
    addi r3, r1, 0x18
    stfs f29, 0x1c(r1)
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x18
    addi r4, r1, 0x3c
    mr r5, r3
    bl fn_800DFFCC
    addi r3, r1, 0x18
    mr r4, r3
    bl fn_800E0060
    bl fn_800E0BA0
    lfs f0, 0x4c(r24)
    addi r3, r1, 0x8
    addi r4, r1, 0x18
    fmuls f1, f0, f1
    bl fn_800E0718
    addi r3, r1, 0x3c
    addi r4, r1, 0x8
    mr r5, r3
    bl fn_800DFEEC
    bl fn_800E0BA0
    lfs f0, 0x50(r24)
    addi r3, r1, 0x48
    fmuls f1, f0, f1
    bl fn_800E0518
    addi r3, r1, 0x3c
    addi r4, r1, 0x48
    mr r5, r3
    bl fn_800DFF98
    cmplwi r25, 0x0
    beq @L_801389B0
    addi r3, r31, 0xc
    addi r4, r1, 0x3c
    bl fn_800E01D0
    b @L_801389F4
    @L_801389B0:
    lfs f1, lbl_8047D14C(r2)
    addi r3, r31, 0xc
    lfs f0, 0x64(r24)
    mr r4, r3
    fsubs f1, f1, f0
    bl fn_800E013C
    addi r3, r1, 0x3c
    lfs f1, 0x64(r24)
    mr r4, r3
    bl fn_800E013C
    addi r3, r31, 0xc
    addi r4, r1, 0x3c
    mr r5, r3
    bl fn_800E019C
    addi r3, r31, 0xc
    mr r4, r3
    bl fn_800E0060
    @L_801389F4:
    bl fn_800E0BA0
    lfs f2, 0x60(r24)
    addi r3, r1, 0x3c
    lfs f0, 0x5c(r24)
    addi r4, r31, 0xc
    fmadds f1, f2, f1, f0
    bl fn_800E013C
    bl fn_800E0BA0
    lfs f0, 0x48(r24)
    fmuls f30, f0, f1
    bl fn_800E0BA0
    lfs f0, 0x44(r24)
    fmuls f31, f0, f1
    bl fn_800E0BA0
    lfs f0, 0x40(r24)
    fmr f2, f31
    fmr f3, f30
    addi r3, r1, 0x30
    fmuls f1, f0, f1
    bl fn_800E01F4
    addi r3, r1, 0x30
    addi r5, r24, 0x34
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x3c
    addi r5, r1, 0x30
    mr r4, r3
    bl fn_800E019C
    mr r3, r31
    mr r4, r26
    addi r5, r1, 0x3c
    bl fn_800E019C
    addi r3, r1, 0x3c
    addi r4, r31, 0xc
    bl fn_800E01D0
    lwz r3, 0x978(r30)
    addi r0, r3, 0x1
    stw r0, 0x978(r30)
    lfs f0, 0x4(r31)
    fcmpo cr0, f0, f29
    bge @L_80138AA8
    mr r4, r26
    addi r3, r30, 0x96c
    bl fn_800E01D0
    b @L_80138AC0
    @L_80138AA8:
    mr r26, r31
    addi r28, r28, 0x1
    addi r31, r31, 0x18
    @L_80138AB4:
    clrlwi r0, r28, 16
    cmplwi r0, 0x64
    blt @L_80138914
    @L_80138AC0:
    addi r29, r29, 0x1
    addi r30, r30, 0x97c
    @L_80138AC8:
    clrlwi r0, r29, 16
    cmplw r0, r27
    blt @L_8013887C
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    lmw r24, 0x80(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
