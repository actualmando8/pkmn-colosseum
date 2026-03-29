    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stmw r27, 0x3c(r1)
    mr r27, r3
    mr r28, r4
    bl fn_80103FE4
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_801040A0
    mr r31, r3
    mr r3, r27
    bl fn_80103FE4
    lha r6, 0x6(r28)
    li r4, 0x1
    lbz r5, 0x16(r3)
    cmpwi r6, 0xa8
    bge @L_80011F44
    cmpwi r6, 0x9f
    bge @L_80011F2C
    cmpwi r6, 0x9a
    beq @L_80011FC0
    bge @L_80011F20
    cmpwi r6, 0x99
    bge @L_80011FA8
    b @L_80011FC0
    @L_80011F20:
    cmpwi r6, 0x9d
    bge @L_80011FA8
    b @L_80011F8C
    @L_80011F2C:
    cmpwi r6, 0xa5
    beq @L_80011FC0
    bge @L_80011F8C
    cmpwi r6, 0xa4
    bge @L_80011FA8
    b @L_80011FC0
    @L_80011F44:
    cmpwi r6, 0x538
    bge @L_80011F6C
    cmpwi r6, 0x534
    bge @L_80011F60
    cmpwi r6, 0xaa
    bge @L_80011FC0
    b @L_80011FA8
    @L_80011F60:
    cmpwi r6, 0x536
    bge @L_80011FA8
    b @L_80011F8C
    @L_80011F6C:
    cmpwi r6, 0x53e
    bge @L_80011F80
    cmpwi r6, 0x53c
    bge @L_80011F8C
    b @L_80011FC0
    @L_80011F80:
    cmpwi r6, 0x540
    bge @L_80011FC0
    b @L_80011FA8
    @L_80011F8C:
    lbz r0, 0x29(r3)
    cmplwi r0, 0x0
    beq @L_80011FA0
    li r4, 0x0
    b @L_80011FC0
    @L_80011FA0:
    li r4, 0x1
    b @L_80011FC0
    @L_80011FA8:
    lbz r0, 0x29(r3)
    cmplwi r0, 0x0
    beq @L_80011FBC
    li r4, 0x1
    b @L_80011FC0
    @L_80011FBC:
    li r4, 0x0
    @L_80011FC0:
    cmpwi r6, 0x534
    bge @L_80011FE0
    cmpwi r6, 0xa4
    beq @L_80011FFC
    bge @L_80012008
    cmpwi r6, 0x99
    beq @L_80011FFC
    b @L_80012008
    @L_80011FE0:
    cmpwi r6, 0x53c
    bge @L_80011FF4
    cmpwi r6, 0x538
    bge @L_80012008
    b @L_80011FFC
    @L_80011FF4:
    cmpwi r6, 0x540
    bge @L_80012008
    @L_80011FFC:
    cmplwi r5, 0x1
    bne @L_80012008
    li r4, 0x0
    @L_80012008:
    mr r3, r28
    bl fn_80109220
    mr r3, r28
    bl fn_801091F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80012834
    lha r0, 0x6(r28)
    li r3, -0x100
    lbz r8, 0x8b(r27)
    cmpwi r0, 0xae
    or r29, r8, r3
    beq @L_8001230C
    bge @L_800120AC
    cmpwi r0, 0xa3
    beq @L_8001230C
    bge @L_8001207C
    cmpwi r0, 0x9f
    bge @L_8001206C
    cmpwi r0, 0x9a
    beq @L_8001215C
    blt @L_80012834
    cmpwi r0, 0x9d
    bge @L_80012110
    b @L_80012834
    @L_8001206C:
    cmpwi r0, 0xa1
    beq @L_80012170
    bge @L_800122C8
    b @L_80012834
    @L_8001207C:
    cmpwi r0, 0xaa
    bge @L_8001209C
    cmpwi r0, 0xa5
    beq @L_8001215C
    blt @L_80012834
    cmpwi r0, 0xa8
    bge @L_80012110
    b @L_80012834
    @L_8001209C:
    cmpwi r0, 0xac
    beq @L_80012170
    bge @L_800122C8
    b @L_80012834
    @L_800120AC:
    cmpwi r0, 0x53a
    beq @L_800123A4
    bge @L_800120E8
    cmpwi r0, 0x535
    beq @L_80012680
    bge @L_800120DC
    cmpwi r0, 0x533
    beq @L_800123CC
    bge @L_80012834
    cmpwi r0, 0x532
    bge @L_800123A4
    b @L_80012834
    @L_800120DC:
    cmpwi r0, 0x537
    beq @L_80012758
    b @L_80012834
    @L_800120E8:
    cmpwi r0, 0x53e
    beq @L_80012834
    bge @L_80012104
    cmpwi r0, 0x53c
    beq @L_80012834
    bge @L_80012680
    b @L_800123CC
    @L_80012104:
    cmpwi r0, 0x540
    bge @L_80012834
    b @L_80012758
    @L_80012110:
    lbz r0, 0x29(r30)
    cmplwi r0, 0x2
    bne @L_80012834
    lhz r5, 0x6(r31)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    mr r3, r27
    lfd f2, lbl_8047B730(r2)
    mr r4, r28
    stw r5, 0xc(r1)
    lfs f0, lbl_8047B720(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f0
    bl fn_8001DACC
    mr r3, r28
    li r4, 0x0
    bl fn_80109220
    b @L_80012834
    @L_8001215C:
    lhz r5, 0x26(r30)
    mr r3, r27
    mr r4, r28
    bl fn_8010B9E8
    b @L_80012834
    @L_80012170:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_801F54A4
    cmpwi r3, 0x0
    beq @L_80012198
    lbz r0, 0x16(r30)
    cmplwi r0, 0x0
    bne @L_80012834
    @L_80012198:
    lha r3, 0x2(r31)
    cmpwi r3, 0x0
    beq @L_80012240
    lha r0, 0x4(r31)
    lis r5, 0x4330
    xoris r4, r3, 0x8000
    lha r6, 0x0(r31)
    xoris r0, r0, 0x8000
    lha r3, 0x1a(r30)
    stw r0, 0xc(r1)
    xoris r0, r6, 0x8000
    subf r3, r6, r3
    lfd f3, lbl_8047B738(r2)
    stw r5, 0x8(r1)
    xoris r3, r3, 0x8000
    lfd f0, 0x8(r1)
    stw r4, 0x14(r1)
    fsubs f1, f0, f3
    stw r5, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r3, 0x1c(r1)
    fsubs f0, f0, f3
    stw r5, 0x18(r1)
    fdivs f2, f1, f0
    lfd f1, 0x18(r1)
    stw r0, 0x24(r1)
    stw r5, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fmadds f1, f2, f1, f0
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r27, 0x2c(r1)
    extsh r0, r27
    cmpwi r0, 0x0
    bne @L_80012244
    lfs f0, lbl_8047B718(r2)
    fcmpo cr0, f1, f0
    ble @L_80012244
    li r27, 0x1
    b @L_80012244
    @L_80012240:
    lha r27, 0x1a(r30)
    @L_80012244:
    mr r5, r29
    li r3, 0x20
    li r4, -0x2
    li r6, 0x195
    bl fn_800FB680
    extsh r4, r27
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xcb
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r29
    extsh r0, r0
    li r4, -0x1
    subfic r0, r0, 0x18
    li r6, 0xcb
    extsh r3, r0
    bl fn_800FB680
    lha r4, 0x18(r30)
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xcb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r3, r3
    mr r5, r29
    subf r0, r3, r0
    li r4, -0x1
    extsh r3, r0
    li r6, 0xcb
    bl fn_800FB680
    b @L_80012834
    @L_800122C8:
    lbz r4, 0x17(r30)
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xcb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r3, r3
    lbz r5, 0x8b(r27)
    subf r3, r3, r0
    li r0, -0x100
    extsh r3, r3
    li r4, -0x1
    or r5, r5, r0
    li r6, 0xcb
    bl fn_800FB680
    b @L_80012834
    @L_8001230C:
    mr r4, r30
    li r3, 0x37
    bl fn_80132A38
    mr r5, r29
    li r3, 0x0
    li r4, -0x1
    li r6, 0xe9
    bl fn_800FB680
    li r3, 0xe9
    bl fn_800FA444
    lbz r0, 0x28(r30)
    srwi r3, r3, 16
    extsh r27, r3
    cmpwi r0, 0x1
    beq @L_80012368
    bge @L_80012358
    cmpwi r0, 0x0
    bge @L_80012360
    b @L_80012370
    @L_80012358:
    cmpwi r0, 0x3
    b @L_80012370
    @L_80012360:
    li r3, 0xd67
    b @L_80012374
    @L_80012368:
    li r3, 0xd68
    b @L_80012374
    @L_80012370:
    li r3, 0x0
    @L_80012374:
    cmplwi r3, 0x0
    beq @L_80012834
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r3, r27
    mr r5, r29
    li r4, -0x1
    li r6, 0xd0
    bl fn_800FB680
    b @L_80012834
    @L_800123A4:
    lhz r9, 0x24(r30)
    mr r7, r29
    lha r5, 0x54(r28)
    mr r8, r27
    lha r6, 0x56(r28)
    li r3, 0x0
    li r4, 0x0
    li r10, 0x0
    bl fn_80104160
    b @L_80012834
    @L_800123CC:
    lha r6, 0x2(r31)
    cmpwi r6, 0x0
    beq @L_8001244C
    lha r0, 0x4(r31)
    lis r5, 0x4330
    xoris r4, r6, 0x8000
    lha r7, 0x0(r31)
    xoris r0, r0, 0x8000
    lha r3, 0x1a(r30)
    stw r0, 0x2c(r1)
    xoris r0, r7, 0x8000
    subf r3, r7, r3
    lfd f3, lbl_8047B738(r2)
    stw r5, 0x28(r1)
    xoris r3, r3, 0x8000
    lfd f0, 0x28(r1)
    stw r4, 0x24(r1)
    fsubs f1, f0, f3
    stw r5, 0x20(r1)
    lfd f0, 0x20(r1)
    stw r3, 0x1c(r1)
    fsubs f0, f0, f3
    stw r5, 0x18(r1)
    fdivs f2, f1, f0
    lfd f1, 0x18(r1)
    stw r0, 0x14(r1)
    stw r5, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fmadds f31, f2, f1, f0
    b @L_8001246C
    @L_8001244C:
    lha r3, 0x1a(r30)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047B738(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    @L_8001246C:
    lha r0, 0x18(r30)
    lis r3, 0x4330
    stw r3, 0x30(r1)
    cmpwi r6, 0x0
    xoris r0, r0, 0x8000
    lfd f3, lbl_8047B738(r2)
    stw r0, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f30, f0, f3
    beq @L_800125B8
    lha r4, 0x0(r31)
    cmpwi r4, 0x0
    bgt @L_800124A8
    li r31, 0x0
    b @L_80012514
    @L_800124A8:
    lfs f0, lbl_8047B724(r2)
    xoris r0, r4, 0x8000
    lfs f1, lbl_8047B71C(r2)
    fmuls f0, f0, f30
    stw r0, 0x34(r1)
    stw r3, 0x30(r1)
    fdivs f0, f0, f1
    lfd f2, 0x30(r1)
    fsubs f2, f2, f3
    fcmpo cr0, f2, f0
    cror eq, lt, eq
    bne @L_800124E0
    lis r31, 0x8000
    b @L_80012514
    @L_800124E0:
    lfs f0, lbl_8047B728(r2)
    stw r0, 0x34(r1)
    fmuls f0, f0, f30
    stw r3, 0x30(r1)
    fdivs f0, f0, f1
    lfd f1, 0x30(r1)
    fsubs f1, f1, f3
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80012510
    lis r31, 0x6464
    b @L_80012514
    @L_80012510:
    lis r31, 0x80
    @L_80012514:
    lha r3, 0x54(r28)
    lis r0, 0x4330
    lfs f0, lbl_8047B72C(r2)
    or r31, r31, r8
    mullw r4, r4, r3
    stw r0, 0x30(r1)
    lfd f1, lbl_8047B738(r2)
    fsubs f2, f30, f0
    li r3, 0x1
    xoris r0, r4, 0x8000
    stw r0, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    fadds f0, f2, f0
    fdivs f0, f0, f30
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r30, 0x2c(r1)
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    mr r4, r31
    li r3, 0x0
    bl fn_800D5BA0
    lha r4, 0x56(r28)
    mr r3, r30
    bl fn_800D61E4
    mr r4, r31
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    @L_800125B8:
    lfs f0, lbl_8047B718(r2)
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_800125D0
    li r9, 0x0
    b @L_80012618
    @L_800125D0:
    lfs f0, lbl_8047B724(r2)
    lfs f1, lbl_8047B71C(r2)
    fmuls f0, f0, f30
    fdivs f0, f0, f1
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_800125F4
    li r9, 0x1ad
    b @L_80012618
    @L_800125F4:
    lfs f0, lbl_8047B728(r2)
    fmuls f0, f0, f30
    fdivs f0, f0, f1
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_80012614
    li r9, 0x1b0
    b @L_80012618
    @L_80012614:
    li r9, 0x1b1
    @L_80012618:
    lha r4, 0x54(r28)
    lis r3, 0x4330
    lfs f0, lbl_8047B72C(r2)
    clrlwi r0, r9, 16
    xoris r4, r4, 0x8000
    stw r3, 0x30(r1)
    lfd f2, lbl_8047B738(r2)
    fsubs f0, f30, f0
    stw r4, 0x34(r1)
    cmplwi r0, 0x0
    lfd f1, 0x30(r1)
    fsubs f1, f1, f2
    fmadds f0, f31, f1, f0
    fdivs f0, f0, f30
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r5, 0x2c(r1)
    beq @L_80012834
    lha r6, 0x56(r28)
    mr r7, r29
    mr r8, r27
    li r3, 0x0
    li r4, 0x0
    li r10, 0x0
    bl fn_80104160
    b @L_80012834
    @L_80012680:
    lha r3, 0xc(r31)
    cmpwi r3, 0x0
    beq @L_80012700
    lha r0, 0xe(r31)
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    lwz r6, 0x8(r31)
    xoris r5, r0, 0x8000
    lwz r0, 0x20(r30)
    stw r5, 0x34(r1)
    subf r0, r6, r0
    lfd f3, lbl_8047B738(r2)
    stw r4, 0x30(r1)
    lfd f2, lbl_8047B730(r2)
    lfd f0, 0x30(r1)
    stw r3, 0x2c(r1)
    fsubs f1, f0, f3
    stw r4, 0x28(r1)
    lfd f0, 0x28(r1)
    stw r0, 0x24(r1)
    fsubs f0, f0, f3
    stw r4, 0x20(r1)
    fdivs f3, f1, f0
    lfd f1, 0x20(r1)
    stw r6, 0x1c(r1)
    stw r4, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fmadds f1, f3, f1, f0
    bl fn_800C46B0
    b @L_80012704
    @L_80012700:
    lwz r3, 0x20(r30)
    @L_80012704:
    lwz r11, 0x1c(r30)
    cmplw r3, r11
    ble @L_80012714
    mr r3, r11
    @L_80012714:
    cmplwi r11, 0x0
    beq @L_80012834
    lha r0, 0x54(r28)
    mr r7, r29
    lha r6, 0x56(r28)
    mr r8, r27
    mullw r0, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r9, 0x1ac
    li r10, 0x0
    add r5, r11, r0
    subi r0, r5, 0x1
    divwu r0, r0, r11
    extsh r5, r0
    bl fn_80104160
    b @L_80012834
    @L_80012758:
    lha r0, 0xc(r31)
    cmpwi r0, 0x0
    beq @L_800127B0
    lha r4, 0xe(r31)
    lis r3, 0x4330
    xoris r0, r0, 0x8000
    stw r3, 0x30(r1)
    xoris r4, r4, 0x8000
    lfd f3, lbl_8047B738(r2)
    stw r4, 0x34(r1)
    lfs f4, 0x8(r31)
    lfd f0, 0x30(r1)
    stw r0, 0x2c(r1)
    fsubs f2, f0, f3
    lfs f0, 0x20(r30)
    stw r3, 0x28(r1)
    fsubs f0, f0, f4
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fdivs f1, f2, f1
    fmadds f2, f1, f0, f4
    b @L_800127B4
    @L_800127B0:
    lfs f2, 0x20(r30)
    @L_800127B4:
    lfs f3, 0x1c(r30)
    fcmpo cr0, f2, f3
    ble @L_800127C4
    fmr f2, f3
    @L_800127C4:
    lha r3, 0x54(r28)
    lis r0, 0x4330
    lfs f0, lbl_8047B718(r2)
    xoris r3, r3, 0x8000
    stw r0, 0x20(r1)
    lfd f1, lbl_8047B738(r2)
    fcmpo cr0, f2, f0
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fmuls f0, f2, f0
    fdivs f0, f0, f3
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r5, 0x1c(r1)
    ble @L_80012814
    extsh r0, r5
    cmpwi r0, 0x0
    bne @L_80012814
    li r5, 0x1
    @L_80012814:
    lha r6, 0x56(r28)
    mr r7, r29
    mr r8, r27
    li r3, 0x0
    li r4, 0x0
    li r9, 0x1ab
    li r10, 0x0
    bl fn_80104160
    @L_80012834:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    lmw r27, 0x3c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
