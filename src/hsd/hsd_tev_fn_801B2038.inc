    nofralloc
    lfs f0, -22688(r2)
    stwu r1, -0x20(r1)
    fcmpo cr0, f1, f0
    blt @L_801B2558
    lfs f0, -22684(r2)
    fcmpo cr0, f1, f0
    ble @L_801B2058
    b @L_801B2558
@L_801B2058:
    lfs f0, -22684(r2)
    fcmpo cr0, f1, f0
    bge @L_801B23B8
    lha r7, 0x2(r4)
    lis r6, 0x4330
    lis r5, 0x4330
    lbz r0, 0x0(r4)
    subi r7, r7, 0x1
    stw r6, 0x8(r1)
    xoris r6, r7, 0x8000
    lfd f3, -22672(r2)
    stw r6, 0xc(r1)
    cmpwi r0, 0x2
    lfd f2, -22672(r2)
    lfd f0, 0x8(r1)
    stw r5, 0x18(r1)
    fsubs f0, f0, f3
    fmuls f1, f1, f0
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r6, 0x14(r1)
    extsh r5, r6
    xoris r5, r5, 0x8000
    stw r5, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f2
    fsubs f1, f1, f0
    beq @L_801B21F4
    bge @L_801B20DC
    cmpwi r0, 0x0
    beq @L_801B20E8
    bge @L_801B2144
    b @L_801B2558
@L_801B20DC:
    cmpwi r0, 0x4
    bge @L_801B2558
    b @L_801B22D0
@L_801B20E8:
    extsh r0, r6
    lwz r4, 0x8(r4)
    mulli r0, r0, 0xc
    add r6, r4, r0
    lfs f3, 0xc(r6)
    lfs f2, 0x0(r6)
    lfs f0, 0x0(r6)
    fsubs f2, f3, f2
    fmadds f0, f1, f2, f0
    stfs f0, 0x0(r3)
    lfs f3, 0x10(r6)
    lfs f2, 0x4(r6)
    lfs f0, 0x4(r6)
    fsubs f2, f3, f2
    fmadds f0, f1, f2, f0
    stfs f0, 0x4(r3)
    lfs f3, 0x14(r6)
    lfs f2, 0x8(r6)
    lfs f0, 0x8(r6)
    fsubs f2, f3, f2
    fmadds f0, f1, f2, f0
    stfs f0, 0x8(r3)
    b @L_801B2558
@L_801B2144:
    lfs f0, -22684(r2)
    extsh r0, r6
    lfs f3, -22624(r2)
    fmuls f8, f1, f1
    fsubs f6, f0, f1
    lfs f0, -22624(r2)
    mulli r0, r0, 0x24
    lwz r4, 0x8(r4)
    fmuls f2, f0, f8
    fmuls f4, f6, f6
    fmuls f3, f3, f1
    add r4, r4, r0
    lfs f0, 0xc(r4)
    fmuls f5, f2, f6
    fmuls f7, f4, f6
    fmuls f6, f3, f4
    lfs f2, 0x0(r4)
    fmuls f4, f8, f1
    lfs f1, 0x18(r4)
    fmuls f0, f0, f6
    lfs f3, 0x24(r4)
    fmadds f0, f2, f7, f0
    fmadds f0, f1, f5, f0
    fmadds f0, f3, f4, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x10(r4)
    lfs f1, 0x4(r4)
    fmuls f0, f0, f6
    lfs f2, 0x1c(r4)
    lfs f3, 0x28(r4)
    fmadds f0, f1, f7, f0
    fmadds f0, f2, f5, f0
    fmadds f0, f3, f4, f0
    stfs f0, 0x4(r3)
    lfs f0, 0x14(r4)
    lfs f1, 0x8(r4)
    fmuls f0, f0, f6
    lfs f2, 0x20(r4)
    lfs f3, 0x2c(r4)
    fmadds f0, f1, f7, f0
    fmadds f0, f2, f5, f0
    fmadds f0, f3, f4, f0
    stfs f0, 0x8(r3)
    b @L_801B2558
@L_801B21F4:
    fmuls f6, f1, f1
    lfs f0, -22616(r2)
    lfs f3, -22684(r2)
    extsh r0, r6
    lfs f2, -22624(r2)
    mulli r0, r0, 0xc
    fmuls f12, f6, f1
    lfs f10, -22620(r2)
    fmuls f0, f0, f6
    lwz r4, 0x8(r4)
    fsubs f11, f3, f1
    lfs f3, -22632(r2)
    fmsubs f0, f2, f12, f0
    add r4, r4, r0
    fneg f2, f12
    lfs f7, -22624(r2)
    fmuls f8, f10, f11
    lfs f5, -22684(r2)
    fadds f4, f3, f0
    lfs f0, 0xc(r4)
    fadds f6, f2, f6
    lfs f2, 0x0(r4)
    fmuls f8, f11, f8
    lfs f3, 0x18(r4)
    fmuls f9, f10, f4
    lfs f4, 0x24(r4)
    fadds f1, f1, f6
    fmuls f8, f11, f8
    fmuls f0, f0, f9
    fmadds f1, f7, f1, f5
    fmuls f5, f10, f12
    fmadds f0, f2, f8, f0
    fmuls f6, f10, f1
    fmadds f0, f3, f6, f0
    fmadds f0, f4, f5, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x10(r4)
    lfs f1, 0x4(r4)
    fmuls f0, f0, f9
    lfs f2, 0x1c(r4)
    lfs f3, 0x28(r4)
    fmadds f0, f1, f8, f0
    fmadds f0, f2, f6, f0
    fmadds f0, f3, f5, f0
    stfs f0, 0x4(r3)
    lfs f0, 0x14(r4)
    lfs f1, 0x8(r4)
    fmuls f0, f0, f9
    lfs f2, 0x20(r4)
    lfs f3, 0x2c(r4)
    fmadds f0, f1, f8, f0
    fmadds f0, f2, f6, f0
    fmadds f0, f3, f5, f0
    stfs f0, 0x8(r3)
    b @L_801B2558
@L_801B22D0:
    fmuls f12, f1, f1
    lfs f2, 0x4(r4)
    lfs f0, -22624(r2)
    extsh r0, r6
    lfs f3, -22628(r2)
    mulli r0, r0, 0xc
    fmuls f11, f12, f1
    lwz r4, 0x8(r4)
    fsubs f0, f2, f0
    lfs f9, -22628(r2)
    fsubs f7, f3, f2
    lfs f3, -22628(r2)
    fmuls f5, f0, f12
    lfs f0, -22624(r2)
    fneg f8, f11
    lfs f4, -22628(r2)
    fnmsubs f0, f3, f2, f0
    add r4, r4, r0
    fmadds f8, f9, f12, f8
    lfs f3, 0xc(r4)
    fmadds f5, f7, f11, f5
    lfs f7, -22684(r2)
    fsubs f6, f11, f12
    fsubs f9, f8, f1
    fadds f10, f7, f5
    lfs f5, 0x18(r4)
    fsubs f8, f2, f4
    lfs f4, 0x0(r4)
    fmuls f7, f0, f12
    fmuls f0, f2, f9
    fmuls f3, f3, f10
    fmadds f7, f8, f11, f7
    fmuls f8, f2, f6
    lfs f6, 0x24(r4)
    fmadds f3, f4, f0, f3
    fmadds f7, f2, f1, f7
    fmadds f1, f5, f7, f3
    fmadds f1, f6, f8, f1
    stfs f1, 0x0(r3)
    lfs f1, 0x10(r4)
    lfs f2, 0x4(r4)
    fmuls f1, f1, f10
    lfs f3, 0x1c(r4)
    lfs f4, 0x28(r4)
    fmadds f1, f2, f0, f1
    fmadds f1, f3, f7, f1
    fmadds f1, f4, f8, f1
    stfs f1, 0x4(r3)
    lfs f1, 0x14(r4)
    lfs f2, 0x8(r4)
    fmuls f1, f1, f10
    lfs f3, 0x20(r4)
    lfs f4, 0x2c(r4)
    fmadds f0, f2, f0, f1
    fmadds f0, f3, f7, f0
    fmadds f0, f4, f8, f0
    stfs f0, 0x8(r3)
    b @L_801B2558
@L_801B23B8:
    lbz r0, 0x0(r4)
    lha r5, 0x2(r4)
    cmpwi r0, 0x2
    subi r5, r5, 0x1
    extsh r6, r5
    beq @L_801B2448
    bge @L_801B23E4
    cmpwi r0, 0x0
    beq @L_801B23F0
    bge @L_801B241C
    b @L_801B2558
@L_801B23E4:
    cmpwi r0, 0x4
    bge @L_801B2558
    b @L_801B252C
@L_801B23F0:
    extsh r0, r6
    lwz r4, 0x8(r4)
    mulli r0, r0, 0xc
    add r6, r4, r0
    lwz r4, 0x0(r6)
    lwz r0, 0x4(r6)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r0, 0x8(r6)
    stw r0, 0x8(r3)
    b @L_801B2558
@L_801B241C:
    extsh r0, r6
    lwz r4, 0x8(r4)
    mulli r0, r0, 0x24
    add r6, r4, r0
    lwz r4, 0x0(r6)
    lwz r0, 0x4(r6)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r0, 0x8(r6)
    stw r0, 0x8(r3)
    b @L_801B2558
@L_801B2448:
    lfs f4, -22684(r2)
    extsh r0, r6
    lfs f1, -22684(r2)
    mulli r5, r0, 0xc
    lfs f0, -22616(r2)
    fmuls f9, f4, f1
    lfs f2, -22624(r2)
    fmuls f1, f0, f4
    lfs f8, -22620(r2)
    lfs f7, -22688(r2)
    subi r5, r5, 0xc
    fneg f0, f9
    lfs f3, -22632(r2)
    fmsubs f2, f2, f9, f1
    lwz r0, 0x8(r4)
    fmuls f5, f8, f7
    lfs f1, -22684(r2)
    add r5, r0, r5
    fadds f3, f3, f2
    fadds f0, f0, f4
    lfs f4, -22624(r2)
    fmuls f5, f7, f5
    lfs f2, -22684(r2)
    fmuls f6, f8, f3
    fadds f3, f1, f0
    lfs f0, 0xc(r5)
    fmuls f7, f7, f5
    lfs f1, 0x0(r5)
    fmuls f5, f8, f9
    fmadds f4, f4, f3, f2
    fmuls f0, f0, f6
    lfs f2, 0x18(r5)
    lfs f3, 0x24(r5)
    fmuls f4, f8, f4
    fmadds f0, f1, f7, f0
    fmadds f0, f2, f4, f0
    fmadds f0, f3, f5, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x10(r5)
    lfs f1, 0x4(r5)
    fmuls f0, f0, f6
    lfs f2, 0x1c(r5)
    lfs f3, 0x28(r5)
    fmadds f0, f1, f7, f0
    fmadds f0, f2, f4, f0
    fmadds f0, f3, f5, f0
    stfs f0, 0x4(r3)
    lfs f0, 0x14(r5)
    lfs f1, 0x8(r5)
    fmuls f0, f0, f6
    lfs f2, 0x20(r5)
    lfs f3, 0x2c(r5)
    fmadds f0, f1, f7, f0
    fmadds f0, f2, f4, f0
    fmadds f0, f3, f5, f0
    stfs f0, 0x8(r3)
    b @L_801B2558
@L_801B252C:
    extsh r0, r6
    lwz r5, 0x8(r4)
    mulli r4, r0, 0xc
    addi r6, r4, 0xc
    add r6, r5, r6
    lwz r4, 0x0(r6)
    lwz r0, 0x4(r6)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r0, 0x8(r6)
    stw r0, 0x8(r3)
@L_801B2558:
    addi r1, r1, 0x20
    blr
