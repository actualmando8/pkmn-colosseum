    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    li r30, 0x1
    lbz r31, 0x22(r3)
    cmplwi r31, 0x0
    beq @L_80184D54
    addi r4, r1, 0x8
    bl fn_8018FC2C
    mr r3, r29
    lis r4, 0x4000
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80184B04
    mr r3, r29
    li r30, 0x0
    li r4, 0x8
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80184B04
    li r30, 0x1
    @L_80184B04:
    clrlwi. r0, r30, 24
    beq @L_80184C4C
    lfs f2, 0xc(r1)
    lfs f1, lbl_8047D7A0(r2)
    fcmpo cr0, f2, f1
    bge @L_80184B38
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B2C
    @L_80184B24:
    fadd f2, f2, f0
    frsp f2, f2
    @L_80184B2C:
    fcmpo cr0, f2, f1
    blt @L_80184B24
    b @L_80184B54
    @L_80184B38:
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B48
    @L_80184B40:
    fsub f2, f2, f0
    frsp f2, f2
    @L_80184B48:
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    beq @L_80184B40
    @L_80184B54:
    stfs f2, 0xc(r1)
    lfs f1, lbl_8047D7A0(r2)
    lfs f31, 0x40(r29)
    fcmpo cr0, f31, f1
    bge @L_80184B84
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B78
    @L_80184B70:
    fadd f31, f31, f0
    frsp f31, f31
    @L_80184B78:
    fcmpo cr0, f31, f1
    blt @L_80184B70
    b @L_80184BA0
    @L_80184B84:
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B94
    @L_80184B8C:
    fsub f31, f31, f0
    frsp f31, f31
    @L_80184B94:
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    beq @L_80184B8C
    @L_80184BA0:
    lfs f1, lbl_8047D7F8(r2)
    lfs f0, 0x44(r29)
    fmuls f30, f1, f0
    bl fn_800D3088
    lfd f2, lbl_8047D7A8(r2)
    lfs f1, lbl_8047D7A0(r2)
    mtctr r3
    cmplwi r3, 0x0
    beq @L_80184D44
    @L_80184BC4:
    lfs f0, 0xc(r1)
    fsubs f3, f31, f0
    fabs f0, f3
    fcmpo cr0, f0, f2
    cror eq, gt, eq
    bne @L_80184C04
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f3, f0
    bge @L_80184BF8
    lfd f0, lbl_8047D7F0(r2)
    fadd f3, f0, f3
    frsp f3, f3
    b @L_80184C04
    @L_80184BF8:
    lfd f0, lbl_8047D7F0(r2)
    fsub f3, f3, f0
    frsp f3, f3
    @L_80184C04:
    fabs f0, f3
    fcmpo cr0, f0, f30
    cror eq, lt, eq
    bne @L_80184C20
    stfs f31, 0xc(r1)
    li r31, 0x0
    b @L_80184D44
    @L_80184C20:
    fcmpo cr0, f3, f1
    ble @L_80184C38
    lfs f0, 0xc(r1)
    fadds f0, f0, f30
    stfs f0, 0xc(r1)
    b @L_80184C44
    @L_80184C38:
    lfs f0, 0xc(r1)
    fsubs f0, f0, f30
    stfs f0, 0xc(r1)
    @L_80184C44:
    bdnz @L_80184BC4
    b @L_80184D44
    @L_80184C4C:
    lfs f2, 0xc(r1)
    lfs f3, 0x40(r29)
    lfs f1, lbl_8047D7FC(r2)
    fsubs f0, f2, f3
    fcmpo cr0, f0, f1
    ble @L_80184C74
    lfs f0, lbl_8047D7C0(r2)
    fsubs f0, f2, f0
    stfs f0, 0xc(r1)
    b @L_80184C8C
    @L_80184C74:
    fsubs f0, f3, f2
    fcmpo cr0, f0, f1
    ble @L_80184C8C
    lfs f0, lbl_8047D7C0(r2)
    fadds f0, f2, f0
    stfs f0, 0xc(r1)
    @L_80184C8C:
    lfs f1, lbl_8047D804(r2)
    lfs f0, 0x44(r29)
    lfs f4, 0x40(r29)
    lfs f3, 0xc(r1)
    fsubs f1, f1, f0
    lfs f2, lbl_8047D800(r2)
    fsubs f3, f4, f3
    lfd f0, lbl_8047D808(r2)
    fmuls f1, f2, f1
    fdivs f31, f3, f1
    fabs f1, f31
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80184CDC
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f31, f0
    bge @L_80184CD8
    lfs f31, lbl_8047D810(r2)
    b @L_80184CDC
    @L_80184CD8:
    lfs f31, lbl_8047D814(r2)
    @L_80184CDC:
    bl fn_800D3088
    lis r0, 0x4330
    lfs f0, lbl_8047D7A0(r2)
    stw r3, 0x1c(r1)
    lfd f2, lbl_8047D7D0(r2)
    fcmpo cr0, f31, f0
    stw r0, 0x18(r1)
    lfs f0, 0xc(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fmadds f0, f31, f1, f0
    stfs f0, 0xc(r1)
    bge @L_80184D2C
    lfs f1, 0x40(r29)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_80184D44
    stfs f1, 0xc(r1)
    li r31, 0x0
    b @L_80184D44
    @L_80184D2C:
    lfs f1, 0x40(r29)
    fcmpo cr0, f0, f1
    cror eq, gt, eq
    bne @L_80184D44
    stfs f1, 0xc(r1)
    li r31, 0x0
    @L_80184D44:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8018FC08
    stb r31, 0x22(r29)
    @L_80184D54:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
