    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr. r29, r3
    lis r4, 0x8037
    lis r3, 0x8027
    addi r31, r4, -12856
    addi r30, r3, 21184
    bne @L_801B0F24
    addi r3, r30, 0x0
    li r4, 0x18d
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0F24:
    lwz r0, 0x4(r29)
    cmplwi r0, 0x0
    bne @L_801B0F40
    addi r3, r30, 0x0
    addi r5, r30, 0xac
    li r4, 0x18e
    bl fn_80196E10
@L_801B0F40:
    lwz r0, 0x8(r29)
    cmplwi r0, 0x0
    bne @L_801B0F5C
    addi r3, r30, 0x0
    addi r5, r30, 0xbc
    li r4, 0x18f
    bl fn_80196E10
@L_801B0F5C:
    lwz r3, 0x8(r29)
    lwz r0, 0x58(r3)
    cmplwi r0, 0x0
    bne @L_801B0F7C
    addi r3, r30, 0x0
    addi r5, r30, 0xcc
    li r4, 0x190
    bl fn_80196E10
@L_801B0F7C:
    lwz r0, 0x0(r29)
    lwz r4, 0x8(r29)
    cmplwi r0, 0x0
    lwz r3, 0x4(r29)
    lwz r30, 0x58(r4)
    beq @L_801B14DC
    bl fn_80195A6C
    bl fn_801B3884
    addi r3, r31, 0x30
    bl fn_801B3408
    bl fn_801B3890
    addi r4, r31, 0xb4
    li r3, 0x0
    bl fn_801B29E4
    addi r3, r31, 0x0
    li r7, 0xff
    addi r5, r31, 0x0
    li r6, 0xff
    addi r4, r31, 0x0
    li r0, 0xff
    stb r7, 0x10(r3)
    addi r3, r31, 0x0
    stb r6, 0x11(r5)
    stb r0, 0x12(r4)
    bl fn_801B3998
    lhz r5, 0x4(r30)
    li r3, 0x0
    lhz r6, 0x6(r30)
    li r4, 0x0
    bl fn_800BD7A0
    lis r3, 0x8037
    lwz r28, 0x4(r29)
    addi r3, r3, -13376
    li r4, 0x0
    bl fn_800BD4B4
    lis r4, 0x8037
    li r3, 0x0
    addi r5, r4, -13248
    lwz r4, 0xc(r5)
    addi r0, r4, 0x1
    stw r0, 0xc(r5)
    bl fn_800BD554
    bl fn_801ACD7C
    li r3, 0x9
    li r4, 0x1
    bl fn_800B7874
    li r3, 0x0
    li r4, 0x9
    li r5, 0x1
    li r6, 0x4
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x2
    bl fn_801B2878
    mr r3, r28
    bl fn_801946F0
    fmr f29, f1
    mr r3, r28
    bl fn_80194654
    fmr f31, f1
    mr r3, r28
    bl fn_801945B0
    fmr f30, f1
    mr r3, r28
    bl fn_80194510
    fmr f28, f1
    mr r3, r28
    bl fn_801944F8
    lfs f0, -22712(r2)
    fmr f27, f1
    lfs f4, -22712(r2)
    li r3, 0x80
    lfs f3, -22712(r2)
    fmuls f29, f29, f0
    lfs f2, -22712(r2)
    lfs f0, -22708(r2)
    fmuls f31, f31, f4
    fmuls f30, f30, f3
    li r4, 0x0
    fmuls f28, f28, f2
    li r5, 0x4
    fmuls f27, f27, f0
    bl fn_800B928C
    lis r3, 0xcc01
    lis r5, 0xcc01
    stfs f30, -0x8000(r3)
    lis r4, 0xcc01
    fmr f0, f29
    lis r3, 0xcc01
    stfs f29, -0x8000(r5)
    fmr f1, f27
    lis r5, 0xcc01
    fmr f2, f28
    stfs f27, -0x8000(r4)
    lis r4, 0xcc01
    lis r7, 0xcc01
    lis r6, 0xcc01
    stfs f28, -0x8000(r3)
    lis r3, 0xcc01
    stfs f0, -0x8000(r5)
    fmr f0, f31
    lis r5, 0xcc01
    stfs f1, -0x8000(r4)
    fmr f1, f27
    lis r4, 0xcc01
    stfs f2, -0x8000(r3)
    lis r3, 0xcc01
    stfs f31, -0x8000(r7)
    stfs f27, -0x8000(r6)
    stfs f30, -0x8000(r5)
    stfs f0, -0x8000(r4)
    stfs f1, -0x8000(r3)
    lbz r0, -21776(r13)
    cmplwi r0, 0x0
    beq @L_801B1468
    addi r3, r31, 0x0
    li r7, 0x0
    addi r5, r31, 0x0
    li r6, 0x0
    addi r4, r31, 0x0
    li r0, 0x0
    stb r7, 0x10(r3)
    addi r3, r31, 0x0
    stb r6, 0x11(r5)
    stb r0, 0x12(r4)
    bl fn_801B3998
    lhz r5, 0x4(r30)
    li r3, 0x2
    lhz r6, 0x6(r30)
    li r4, 0x2
    subi r5, r5, 0x4
    subi r6, r6, 0x4
    bl fn_800BD7A0
    lis r3, 0x8037
    lwz r28, 0x4(r29)
    addi r3, r3, -13376
    li r4, 0x0
    bl fn_800BD4B4
    lis r4, 0x8037
    li r3, 0x0
    addi r5, r4, -13248
    lwz r4, 0xc(r5)
    addi r0, r4, 0x1
    stw r0, 0xc(r5)
    bl fn_800BD554
    bl fn_801ACD7C
    li r3, 0x9
    li r4, 0x1
    bl fn_800B7874
    li r3, 0x0
    li r4, 0x9
    li r5, 0x1
    li r6, 0x4
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x2
    bl fn_801B2878
    mr r3, r28
    bl fn_801946F0
    fmr f27, f1
    mr r3, r28
    bl fn_80194654
    fmr f30, f1
    mr r3, r28
    bl fn_801945B0
    fmr f31, f1
    mr r3, r28
    bl fn_80194510
    fmr f28, f1
    mr r3, r28
    bl fn_801944F8
    lfs f0, -22712(r2)
    fmr f29, f1
    lfs f4, -22712(r2)
    li r3, 0x80
    lfs f3, -22712(r2)
    fmuls f27, f27, f0
    lfs f2, -22712(r2)
    lfs f0, -22708(r2)
    fmuls f30, f30, f4
    fmuls f31, f31, f3
    li r4, 0x0
    fmuls f28, f28, f2
    li r5, 0x4
    fmuls f29, f29, f0
    bl fn_800B928C
    lis r4, 0xcc01
    lis r3, 0xcc01
    stfs f31, -0x8000(r4)
    lis r5, 0xcc01
    fmr f0, f27
    lis r4, 0xcc01
    stfs f27, -0x8000(r3)
    fmr f1, f29
    lis r3, 0xcc01
    fmr f2, f28
    stfs f29, -0x8000(r5)
    lis r6, 0xcc01
    lis r5, 0xcc01
    lis r11, 0xcc01
    stfs f28, -0x8000(r4)
    lis r4, 0xcc01
    lis r10, 0xcc01
    lis r9, 0xcc01
    stfs f0, -0x8000(r3)
    lis r3, 0xcc01
    fmr f0, f30
    addi r7, r31, 0x0
    stfs f1, -0x8000(r6)
    fmr f1, f29
    li r8, 0xff
    li r6, 0xff
    stfs f2, -0x8000(r5)
    addi r5, r31, 0x0
    li r0, 0xff
    stfs f30, -0x8000(r4)
    addi r4, r31, 0x0
    stfs f29, -0x8000(r3)
    addi r3, r31, 0x0
    stfs f31, -0x8000(r11)
    stfs f0, -0x8000(r10)
    stfs f1, -0x8000(r9)
    stb r8, 0x10(r7)
    stb r6, 0x11(r5)
    stb r0, 0x12(r4)
    bl fn_801B3998
    lhz r5, 0x4(r30)
    li r3, 0x4
    lhz r6, 0x6(r30)
    li r4, 0x4
    subi r5, r5, 0x8
    subi r6, r6, 0x8
    bl fn_800BD7A0
    lis r3, 0x8037
    lwz r28, 0x4(r29)
    addi r3, r3, -13376
    li r4, 0x0
    bl fn_800BD4B4
    lis r4, 0x8037
    li r3, 0x0
    addi r5, r4, -13248
    lwz r4, 0xc(r5)
    addi r0, r4, 0x1
    stw r0, 0xc(r5)
    bl fn_800BD554
    bl fn_801ACD7C
    li r3, 0x9
    li r4, 0x1
    bl fn_800B7874
    li r3, 0x0
    li r4, 0x9
    li r5, 0x1
    li r6, 0x4
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x2
    bl fn_801B2878
    mr r3, r28
    bl fn_801946F0
    fmr f27, f1
    mr r3, r28
    bl fn_80194654
    fmr f30, f1
    mr r3, r28
    bl fn_801945B0
    fmr f31, f1
    mr r3, r28
    bl fn_80194510
    fmr f28, f1
    mr r3, r28
    bl fn_801944F8
    lfs f0, -22712(r2)
    fmr f29, f1
    lfs f4, -22712(r2)
    li r3, 0x80
    lfs f3, -22712(r2)
    fmuls f27, f27, f0
    lfs f2, -22712(r2)
    lfs f0, -22708(r2)
    fmuls f30, f30, f4
    fmuls f31, f31, f3
    li r4, 0x0
    fmuls f28, f28, f2
    li r5, 0x4
    fmuls f29, f29, f0
    bl fn_800B928C
    lis r3, 0xcc01
    lis r5, 0xcc01
    stfs f31, -0x8000(r3)
    lis r4, 0xcc01
    fmr f0, f27
    lis r3, 0xcc01
    stfs f27, -0x8000(r5)
    fmr f1, f29
    lis r5, 0xcc01
    fmr f2, f28
    stfs f29, -0x8000(r4)
    lis r4, 0xcc01
    lis r7, 0xcc01
    lis r6, 0xcc01
    stfs f28, -0x8000(r3)
    lis r3, 0xcc01
    stfs f0, -0x8000(r5)
    fmr f0, f30
    lis r5, 0xcc01
    stfs f1, -0x8000(r4)
    fmr f1, f29
    lis r4, 0xcc01
    stfs f2, -0x8000(r3)
    lis r3, 0xcc01
    stfs f30, -0x8000(r7)
    stfs f29, -0x8000(r6)
    stfs f31, -0x8000(r5)
    stfs f0, -0x8000(r4)
    stfs f1, -0x8000(r3)
@L_801B1468:
    lbz r0, 0x20(r29)
    addi r3, r31, 0x0
    addi r5, r31, 0x0
    addi r4, r31, 0x0
    stb r0, 0x10(r3)
    addi r3, r31, 0x0
    lbz r0, 0x20(r29)
    stb r0, 0x11(r5)
    lbz r0, 0x20(r29)
    stb r0, 0x12(r4)
    bl fn_801B3998
    lhz r5, 0x4(r30)
    li r3, 0x2
    lhz r6, 0x6(r30)
    li r4, 0x2
    subi r5, r5, 0x4
    subi r6, r6, 0x4
    bl fn_800BD7A0
    lwz r28, 0x0(r29)
    b @L_801B14D0
@L_801B14B8:
    lwz r3, 0x4(r28)
    li r4, 0x0
    li r5, 0x5
    lis r6, 0x400
    bl fn_801A13CC
    lwz r28, 0x0(r28)
@L_801B14D0:
    cmplwi r28, 0x0
    bne @L_801B14B8
    bl fn_80195A48
@L_801B14DC:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x74(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
