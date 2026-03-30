    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x94(r1)
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    mr r30, r4
    beq @L_8013B018
    mr r31, r3
    lwz r5, 0x54(r3)
    cmplwi r5, 0x0
    beq @L_8013B018
    lwz r3, 0x8(r5)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_8013AE5C
    li r0, 0x0
    stw r0, 0x58(r31)
    b @L_8013AE5C
    @L_8013ADE8:
    lwz r0, 0x58(r31)
    mr r4, r5
    subf r0, r3, r0
    stw r0, 0x58(r31)
    lwz r5, 0x10(r5)
    cmplwi r5, 0x0
    stw r5, 0x54(r31)
    bne @L_8013AE40
    lbz r0, 0x4c(r31)
    cmplwi r0, 0x0
    beq @L_8013AE24
    mr r3, r31
    addi r4, r4, 0x4
    bl fn_8013B268
    b @L_8013AE38
    @L_8013AE24:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013AE38
    addi r4, r4, 0x4
    bl fn_80168408
    @L_8013AE38:
    li r3, 0x0
    b @L_8013B01C
    @L_8013AE40:
    lwz r3, 0x8(r5)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_8013AE5C
    li r0, 0x0
    stw r0, 0x58(r31)
    b @L_8013AE6C
    @L_8013AE5C:
    lwz r0, 0x58(r31)
    lwz r3, 0x8(r5)
    cmplw r0, r3
    bge @L_8013ADE8
    @L_8013AE6C:
    lwz r4, 0x58(r31)
    lis r3, 0x4330
    lwz r0, 0x8(r5)
    stw r4, 0x14(r1)
    lfd f3, lbl_8047D1E8(r2)
    stw r3, 0x10(r1)
    lfs f0, lbl_8047D1F0(r2)
    lfd f1, 0x10(r1)
    stw r0, 0x1c(r1)
    fsubs f2, f1, f3
    stw r3, 0x18(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f3
    fdivs f3, f2, f1
    fcmpo cr0, f3, f0
    bge @L_8013AEB0
    fmr f3, f0
    @L_8013AEB0:
    lfs f0, lbl_8047D1F4(r2)
    fcmpo cr0, f3, f0
    ble @L_8013AEC0
    fmr f3, f0
    @L_8013AEC0:
    lbz r4, 0x0(r5)
    lis r3, 0x4330
    lbz r0, 0x4(r5)
    stw r4, 0x24(r1)
    lfd f2, lbl_8047D1E8(r2)
    stw r3, 0x20(r1)
    lfd f0, 0x20(r1)
    stw r0, 0x2c(r1)
    fsubs f1, f0, f2
    stw r3, 0x28(r1)
    lfd f0, 0x28(r1)
    stw r3, 0x38(r1)
    fsubs f0, f0, f2
    stw r3, 0x40(r1)
    fsubs f0, f0, f1
    stw r3, 0x50(r1)
    stw r3, 0x58(r1)
    fmadds f0, f3, f0, f1
    stw r3, 0x68(r1)
    fctiwz f0, f0
    stw r3, 0x70(r1)
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stb r0, 0x8(r1)
    lbz r3, 0x1(r5)
    lbz r0, 0x5(r5)
    stw r3, 0x3c(r1)
    stw r0, 0x44(r1)
    lfd f1, 0x38(r1)
    lfd f0, 0x40(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fsubs f0, f0, f1
    fmadds f0, f3, f0, f1
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    stb r0, 0x9(r1)
    lbz r3, 0x2(r5)
    lbz r0, 0x6(r5)
    stw r3, 0x54(r1)
    stw r0, 0x5c(r1)
    lfd f1, 0x50(r1)
    lfd f0, 0x58(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fsubs f0, f0, f1
    fmadds f0, f3, f0, f1
    fctiwz f0, f0
    stfd f0, 0x60(r1)
    lwz r0, 0x64(r1)
    stb r0, 0xa(r1)
    lbz r3, 0x3(r5)
    lbz r0, 0x7(r5)
    stw r3, 0x6c(r1)
    stw r0, 0x74(r1)
    lfd f1, 0x68(r1)
    lfd f0, 0x70(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fsubs f0, f0, f1
    fmadds f0, f3, f0, f1
    fctiwz f0, f0
    stfd f0, 0x78(r1)
    lwz r0, 0x7c(r1)
    stb r0, 0xb(r1)
    lbz r0, 0x4c(r31)
    cmplwi r0, 0x0
    beq @L_8013AFE4
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8013B268
    b @L_8013B004
    @L_8013AFE4:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013AFFC
    addi r4, r1, 0x8
    bl fn_80168408
    b @L_8013B004
    @L_8013AFFC:
    li r3, 0x0
    b @L_8013B01C
    @L_8013B004:
    lwz r0, 0x58(r31)
    li r3, 0x1
    add r0, r0, r30
    stw r0, 0x58(r31)
    b @L_8013B01C
    @L_8013B018:
    li r3, 0x0
    @L_8013B01C:
    lwz r0, 0x94(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
