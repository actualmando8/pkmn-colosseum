    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r26, 0x38(r1)
    mr r26, r4
    addi r31, r3, 0x4
    lbz r28, 0x4f(r3)
    lwz r0, 0x48(r3)
    cmplwi r28, 0x0
    clrlwi r30, r0, 16
    beq @L_8013B298
    addi r27, r3, 0x44
    @L_8013B298:
    li r29, 0x0
    b @L_8013B470
    @L_8013B2A0:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013B468
    cmplwi r28, 0x0
    beq @L_8013B460
    lbz r4, 0x0(r27)
    lis r3, 0x4330
    lbz r0, 0x0(r26)
    stw r4, 0x14(r1)
    lfd f4, lbl_8047D1E8(r2)
    stw r3, 0x10(r1)
    lfs f1, lbl_8047D1F8(r2)
    lfd f0, 0x10(r1)
    stw r0, 0x1c(r1)
    fsubs f3, f0, f4
    lfs f0, lbl_8047D1F0(r2)
    stw r3, 0x18(r1)
    lfd f2, 0x18(r1)
    fsubs f2, f2, f4
    fadds f2, f3, f2
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_8013B304
    fmr f1, f0
    b @L_8013B314
    @L_8013B304:
    lfs f0, lbl_8047D1FC(r2)
    fcmpo cr0, f1, f0
    ble @L_8013B314
    fmr f1, f0
    @L_8013B314:
    fctiwz f0, f1
    lis r0, 0x4330
    stw r0, 0x10(r1)
    lfd f4, lbl_8047D1E8(r2)
    stfd f0, 0x18(r1)
    lfs f1, lbl_8047D1F8(r2)
    lwz r3, 0x1c(r1)
    stw r0, 0x20(r1)
    lfs f0, lbl_8047D1F0(r2)
    stb r3, 0x8(r1)
    lbz r3, 0x1(r27)
    lbz r0, 0x1(r26)
    stw r3, 0x14(r1)
    stw r0, 0x24(r1)
    lfd f3, 0x10(r1)
    lfd f2, 0x20(r1)
    fsubs f3, f3, f4
    fsubs f2, f2, f4
    fadds f2, f3, f2
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_8013B374
    fmr f1, f0
    b @L_8013B384
    @L_8013B374:
    lfs f0, lbl_8047D1FC(r2)
    fcmpo cr0, f1, f0
    ble @L_8013B384
    fmr f1, f0
    @L_8013B384:
    fctiwz f0, f1
    lis r0, 0x4330
    stw r0, 0x18(r1)
    lfd f4, lbl_8047D1E8(r2)
    stfd f0, 0x20(r1)
    lfs f1, lbl_8047D1F8(r2)
    lwz r3, 0x24(r1)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047D1F0(r2)
    stb r3, 0x9(r1)
    lbz r3, 0x2(r27)
    lbz r0, 0x2(r26)
    stw r3, 0x1c(r1)
    stw r0, 0x14(r1)
    lfd f3, 0x18(r1)
    lfd f2, 0x10(r1)
    fsubs f3, f3, f4
    fsubs f2, f2, f4
    fadds f2, f3, f2
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_8013B3E4
    fmr f1, f0
    b @L_8013B3F4
    @L_8013B3E4:
    lfs f0, lbl_8047D1FC(r2)
    fcmpo cr0, f1, f0
    ble @L_8013B3F4
    fmr f1, f0
    @L_8013B3F4:
    fctiwz f0, f1
    lis r0, 0x4330
    stw r0, 0x18(r1)
    addi r4, r1, 0x8
    lfd f3, lbl_8047D1E8(r2)
    stfd f0, 0x20(r1)
    lfs f0, lbl_8047D1FC(r2)
    lwz r3, 0x24(r1)
    stw r0, 0x10(r1)
    stb r3, 0xa(r1)
    lbz r3, 0x3(r27)
    lbz r0, 0x3(r26)
    stw r3, 0x1c(r1)
    stw r0, 0x14(r1)
    lfd f2, 0x18(r1)
    lfd f1, 0x10(r1)
    fsubs f2, f2, f3
    fsubs f1, f1, f3
    fmuls f1, f2, f1
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    stb r0, 0xb(r1)
    lwz r3, 0x0(r31)
    bl fn_800E5B68
    b @L_8013B468
    @L_8013B460:
    mr r4, r26
    bl fn_800E5B68
    @L_8013B468:
    addi r29, r29, 0x1
    addi r31, r31, 0x4
    @L_8013B470:
    clrlwi r0, r29, 16
    cmplw r0, r30
    blt @L_8013B2A0
    lmw r26, 0x38(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
