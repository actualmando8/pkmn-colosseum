    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stmw r27, 0x3c(r1)
    mr r29, r4
    mr r28, r3
    lwz r30, 0x4(r29)
    mr r27, r29
    li r4, 0x0
    li r5, 0xd8
    addi r29, r29, 0xc
    bl memset
    lwz r0, 0x0(r27)
    stw r0, 0x0(r28)
    bl fn_800D37CC
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    xoris r0, r30, 0x8000
    stw r3, 0x14(r1)
    lfd f3, lbl_8047D128(r2)
    stw r4, 0x10(r1)
    lfs f0, lbl_8047D118(r2)
    lfd f1, 0x10(r1)
    stw r0, 0x1c(r1)
    fsubs f2, f1, f3
    stw r4, 0x18(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r0, 0x4(r28)
    lwz r0, 0x0(r27)
    cmplwi r0, 0xc
    bgt @L_801370EC
    lis r3, jumptable_80363C70@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80363C70@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r4, r29
    mr r5, r30
    addi r3, r28, 0x8
    bl fn_80137780
    mr r29, r3
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x60
    bl memset
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x24(r1)
    xoris r0, r30, 0x8000
    lfd f3, lbl_8047D128(r2)
    mr r4, r29
    stw r5, 0x20(r1)
    addi r3, r28, 0x10
    lfs f0, lbl_8047D118(r2)
    lfd f1, 0x20(r1)
    stw r0, 0x1c(r1)
    fsubs f2, f1, f3
    stw r5, 0x18(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r28)
    lwz r0, 0x3c(r29)
    sth r0, 0x4c(r28)
    lwz r0, 0x40(r29)
    sth r0, 0x4e(r28)
    lfs f0, 0x38(r29)
    stfs f0, 0x48(r28)
    lwz r0, 0x44(r29)
    srwi r0, r0, 24
    stb r0, 0x67(r28)
    lwz r0, 0x44(r29)
    extrwi r0, r0, 8, 8
    stb r0, 0x66(r28)
    lwz r0, 0x44(r29)
    extrwi r0, r0, 8, 16
    stb r0, 0x65(r28)
    lwz r0, 0x44(r29)
    stb r0, 0x64(r28)
    lwz r0, 0x48(r29)
    stw r0, 0x58(r28)
    lfs f0, 0x34(r29)
    stfs f0, 0x44(r28)
    lfs f0, 0x30(r29)
    stfs f0, 0x40(r28)
    bl fn_800E01D0
    addi r3, r28, 0x34
    addi r4, r29, 0x24
    bl fn_800E01D0
    addi r3, r28, 0x1c
    addi r4, r29, 0xc
    bl fn_800E01D0
    addi r3, r28, 0x28
    addi r4, r29, 0x18
    bl fn_800E01D0
    lwz r3, 0x4c(r29)
    addi r0, r29, 0x73
    clrrwi r27, r0, 5
    li r4, 0x20
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r31, r3
    beq @L_801366C8
    bl fn_800E27B0
    lwz r5, 0x4c(r29)
    mr r30, r3
    mr r4, r27
    addi r0, r5, 0x1f
    clrrwi r5, r0, 5
    bl memcpy
    mr r3, r30
    bl fn_800EFD3C
    stw r3, 0x60(r28)
    mr r4, r31
    lwz r3, 0x60(r28)
    bl fn_800EFD14
    b @L_801366D0
    @L_801366C8:
    li r0, 0x0
    stw r0, 0x60(r28)
    @L_801366D0:
    lwz r4, 0x4c(r29)
    lis r3, lbl_80314AE8@ha
    addi r0, r3, lbl_80314AE8@l
    addi r3, r4, 0x1f
    stw r0, 0x5c(r28)
    clrrwi r0, r3, 5
    add r27, r27, r0
    mr r29, r27
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x24
    bl memset
    lfs f0, 0x4(r29)
    lis r4, 0x4330
    lis r3, lbl_80314638@ha
    stw r4, 0x20(r1)
    addi r0, r3, lbl_80314638@l
    lfd f1, lbl_8047D128(r2)
    stfs f0, 0x18(r28)
    lfs f0, 0x8(r29)
    stw r4, 0x18(r1)
    stfs f0, 0x1c(r28)
    lwz r3, 0xc(r29)
    xoris r3, r3, 0x8000
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x20(r28)
    lwz r3, 0x10(r29)
    xoris r3, r3, 0x8000
    stw r3, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    stfs f0, 0x24(r28)
    lfs f0, 0x14(r29)
    stfs f0, 0x28(r28)
    lwz r3, 0x0(r29)
    srwi r3, r3, 24
    stb r3, 0xb(r28)
    lwz r3, 0x0(r29)
    extrwi r3, r3, 8, 8
    stb r3, 0xa(r28)
    lwz r3, 0x0(r29)
    extrwi r3, r3, 8, 16
    stb r3, 0x9(r28)
    lwz r3, 0x0(r29)
    stb r3, 0x8(r28)
    stw r0, 0xc(r28)
    bl fn_800D37CC
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x14(r1)
    xoris r0, r30, 0x8000
    lfd f3, lbl_8047D128(r2)
    addi r29, r29, 0x1c
    stw r4, 0x10(r1)
    lfs f0, lbl_8047D118(r2)
    lfd f1, 0x10(r1)
    stw r0, 0x2c(r1)
    fsubs f2, f1, f3
    stw r4, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    sth r0, 0x12(r28)
    b @L_801370EC
    mr r4, r29
    mr r5, r30
    addi r3, r28, 0x8
    bl fn_8013757C
    mr r29, r3
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x2c
    bl memset
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    xoris r3, r30, 0x8000
    addi r0, r29, 0x37
    lfd f3, lbl_8047D128(r2)
    stw r5, 0x30(r1)
    clrrwi r31, r0, 5
    lfs f0, lbl_8047D118(r2)
    li r4, 0x20
    lfd f1, 0x30(r1)
    stw r3, 0x2c(r1)
    fsubs f2, f1, f3
    stw r5, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0x28(r28)
    lwz r0, 0x0(r29)
    srwi r0, r0, 24
    stb r0, 0x23(r28)
    lwz r0, 0x0(r29)
    extrwi r0, r0, 8, 8
    stb r0, 0x22(r28)
    lwz r0, 0x0(r29)
    extrwi r0, r0, 8, 16
    stb r0, 0x21(r28)
    lwz r0, 0x0(r29)
    stb r0, 0x20(r28)
    lwz r0, 0x4(r29)
    sth r0, 0x2a(r28)
    lwz r0, 0x8(r29)
    sth r0, 0x30(r28)
    lwz r0, 0xc(r29)
    sth r0, 0x32(r28)
    lwz r3, 0x10(r29)
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_80136908
    bl fn_800E27B0
    lwz r5, 0x10(r29)
    mr r27, r3
    mr r4, r31
    addi r0, r5, 0x1f
    clrrwi r5, r0, 5
    bl memcpy
    mr r3, r27
    bl fn_800EFD3C
    stw r3, 0x1c(r28)
    mr r4, r30
    lwz r3, 0x1c(r28)
    bl fn_800EFD14
    b @L_80136910
    @L_80136908:
    li r0, 0x0
    stw r0, 0x1c(r28)
    @L_80136910:
    lwz r4, 0x10(r29)
    lwz r3, 0x1c(r28)
    addi r0, r4, 0x1f
    clrrwi r0, r0, 5
    cmplwi r3, 0x0
    add r31, r31, r0
    beq @L_80136938
    li r4, 0x0
    li r5, 0x0
    bl fn_800EF590
    @L_80136938:
    lis r3, lbl_80314AE8@ha
    mr r29, r31
    addi r0, r3, lbl_80314AE8@l
    stw r0, 0x18(r28)
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x4c
    bl memset
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    xoris r4, r30, 0x8000
    addi r3, r29, 0x5b
    lfd f3, lbl_8047D128(r2)
    stw r5, 0x30(r1)
    clrrwi r27, r3, 5
    lfs f0, lbl_8047D118(r2)
    li r0, 0x4e20
    lfd f1, 0x30(r1)
    stw r4, 0x2c(r1)
    fsubs f2, f1, f3
    stw r5, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r3, 0x24(r1)
    sth r3, 0x46(r28)
    lfs f0, 0x0(r29)
    stfs f0, 0x18(r28)
    lfs f0, 0x4(r29)
    stfs f0, 0x1c(r28)
    lfs f0, 0x8(r29)
    stfs f0, 0x20(r28)
    lfs f0, 0xc(r29)
    stfs f0, 0x24(r28)
    lfs f0, 0x10(r29)
    stfs f0, 0x28(r28)
    lfs f0, 0x14(r29)
    stfs f0, 0x2c(r28)
    lfs f0, 0x18(r29)
    stfs f0, 0x30(r28)
    lfs f0, 0x1c(r29)
    stfs f0, 0x34(r28)
    lfs f0, 0x20(r29)
    stfs f0, 0x38(r28)
    lfs f0, 0x24(r29)
    stfs f0, 0x3c(r28)
    lwz r3, 0x28(r29)
    sth r3, 0x40(r28)
    lwz r3, 0x2c(r29)
    sth r3, 0x42(r28)
    lwz r3, 0x30(r29)
    sth r3, 0x52(r28)
    sth r0, 0x48(r28)
    bl fn_801DB060
    sth r3, 0x4a(r28)
    bl fn_801DB060
    sth r3, 0x4c(r28)
    mr r3, r27
    li r5, 0x4e20
    lwz r4, 0x34(r29)
    lhz r6, 0x4a(r28)
    bl fn_8010147C
    lhz r4, 0x4a(r28)
    li r3, 0x4e20
    bl fn_800F9318
    lhz r6, 0x4c(r28)
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    lhz r4, 0x4c(r28)
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80136A80
    li r4, 0x0
    bl fn_800E4014
    @L_80136A80:
    lwz r3, 0x34(r29)
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    add r0, r27, r0
    mr r29, r0
    b @L_801370EC
    mr r4, r29
    mr r5, r30
    addi r3, r28, 0x8
    bl fn_8013735C
    mr r29, r3
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0xd0
    bl memset
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    xoris r4, r30, 0x8000
    addi r3, r29, 0x53
    lfd f3, lbl_8047D128(r2)
    stw r5, 0x30(r1)
    clrrwi r31, r3, 5
    lfs f0, lbl_8047D118(r2)
    li r0, 0x4e20
    lfd f1, 0x30(r1)
    stw r4, 0x2c(r1)
    fsubs f2, f1, f3
    stw r5, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r3, 0x24(r1)
    sth r3, 0xd6(r28)
    lfs f0, 0x0(r29)
    stfs f0, 0x2c(r28)
    lfs f0, 0x4(r29)
    stfs f0, 0x30(r28)
    lfs f0, 0x8(r29)
    stfs f0, 0x34(r28)
    lwz r3, 0xc(r29)
    srawi r3, r3, 24
    stb r3, 0x47(r28)
    lwz r3, 0xc(r29)
    extrwi r3, r3, 8, 8
    stb r3, 0x46(r28)
    lwz r3, 0xc(r29)
    extrwi r3, r3, 8, 16
    stb r3, 0x45(r28)
    lwz r3, 0xc(r29)
    stb r3, 0x44(r28)
    lfs f0, 0x10(r29)
    stfs f0, 0x48(r28)
    lfs f0, 0x14(r29)
    stfs f0, 0x4c(r28)
    lfs f0, 0x18(r29)
    stfs f0, 0x50(r28)
    lwz r3, 0x1c(r29)
    sth r3, 0x54(r28)
    lwz r3, 0x20(r29)
    sth r3, 0x56(r28)
    lfs f0, 0x24(r29)
    stfs f0, 0xcc(r28)
    lfs f0, 0x28(r29)
    stfs f0, 0xd0(r28)
    stw r0, 0x58(r28)
    bl fn_801DB060
    stw r3, 0x60(r28)
    bl fn_801DB060
    stw r3, 0x5c(r28)
    mr r3, r31
    li r5, 0x4e20
    lwz r4, 0x2c(r29)
    lwz r6, 0x60(r28)
    bl fn_8010147C
    lwz r4, 0x60(r28)
    li r3, 0x4e20
    bl fn_800F9318
    lwz r6, 0x5c(r28)
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    lwz r4, 0x5c(r28)
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80136BF8
    li r4, 0x0
    bl fn_800E4014
    @L_80136BF8:
    lwz r3, 0x2c(r29)
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    add r0, r31, r0
    mr r29, r0
    b @L_801370EC
    lwz r27, 0x8(r29)
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x18
    bl memset
    lwz r0, 0x0(r29)
    stw r0, 0xc(r28)
    lwz r0, 0x4(r29)
    stw r0, 0x10(r28)
    lwz r0, 0xc(r29)
    cmpwi r0, 0x2
    beq @L_80136C50
    bge @L_80136D78
    cmpwi r0, 0x1
    bge @L_80136CD0
    b @L_80136D78
    @L_80136C50:
    lfd f30, lbl_8047D128(r2)
    addi r31, r29, 0x10
    lfs f31, lbl_8047D118(r2)
    li r30, 0x0
    lis r29, 0x4330
    b @L_80136CC4
    @L_80136C68:
    bl fn_800D37CC
    lwz r0, 0x8(r31)
    xoris r3, r3, 0x8000
    stw r3, 0x34(r1)
    addi r3, r28, 0x8
    xoris r0, r0, 0x8000
    lfs f1, 0x0(r31)
    stw r29, 0x30(r1)
    lfs f2, 0x4(r31)
    lfd f0, 0x30(r1)
    stw r0, 0x2c(r1)
    fsubs f3, f0, f30
    stw r29, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f30
    fmuls f0, f0, f3
    fdivs f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r4, 0x24(r1)
    bl fn_8013D604
    addi r30, r30, 0x1
    addi r31, r31, 0x10
    @L_80136CC4:
    cmpw r30, r27
    blt @L_80136C68
    b @L_80136D78
    @L_80136CD0:
    lwz r0, 0x8(r29)
    stw r0, 0x8(r1)
    lfs f30, 0x8(r1)
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    lis r5, 0x4330
    xoris r4, r30, 0x8000
    stw r0, 0x34(r1)
    lis r3, 0x5555
    lfd f5, lbl_8047D128(r2)
    stw r5, 0x30(r1)
    addi r0, r3, 0x5556
    lfs f0, lbl_8047D118(r2)
    fmr f2, f30
    lfd f1, 0x30(r1)
    addi r3, r28, 0x8
    stw r4, 0x2c(r1)
    fsubs f4, f1, f5
    lfs f1, lbl_8047D11C(r2)
    stw r5, 0x28(r1)
    lfd f3, 0x28(r1)
    fsubs f3, f3, f5
    fmuls f3, f3, f4
    fdivs f0, f3, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r4, 0x24(r1)
    mulhw r4, r0, r4
    srwi r0, r4, 31
    add r27, r4, r0
    mr r4, r27
    bl fn_8013D604
    fmr f1, f30
    mr r4, r27
    fmr f2, f30
    addi r3, r28, 0x8
    bl fn_8013D604
    lfs f1, lbl_8047D11C(r2)
    mr r4, r27
    addi r3, r28, 0x8
    fmr f2, f1
    bl fn_8013D604
    @L_80136D78:
    mr r29, r31
    b @L_801370EC
    mr r4, r29
    mr r5, r30
    addi r3, r28, 0x8
    bl fn_80137114
    mr r29, r3
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x34
    bl memset
    bl fn_800D37CC
    lis r4, 0x4330
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    xoris r3, r30, 0x8000
    lfd f3, lbl_8047D128(r2)
    li r0, 0x0
    stw r4, 0x30(r1)
    lfs f0, lbl_8047D118(r2)
    lfd f1, 0x30(r1)
    stw r3, 0x2c(r1)
    fsubs f2, f1, f3
    stw r4, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r3, 0x24(r1)
    sth r3, 0x3a(r28)
    lfs f0, 0x18(r29)
    stfs f0, 0x24(r28)
    lfs f0, 0x1c(r29)
    stfs f0, 0x28(r28)
    lfs f0, 0x20(r29)
    stfs f0, 0x2c(r28)
    lwz r3, 0x4(r29)
    stb r3, 0x20(r28)
    stb r0, 0x21(r28)
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    beq @L_80136E38
    lbz r0, 0x21(r28)
    ori r0, r0, 0x2
    stb r0, 0x21(r28)
    @L_80136E38:
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    beq @L_80136E50
    lbz r0, 0x21(r28)
    ori r0, r0, 0x8
    stb r0, 0x21(r28)
    @L_80136E50:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_80136E68
    lbz r0, 0x21(r28)
    ori r0, r0, 0x4
    stb r0, 0x21(r28)
    @L_80136E68:
    lwz r0, 0x10(r29)
    cmpwi r0, 0x0
    beq @L_80136E80
    lbz r0, 0x21(r28)
    ori r0, r0, 0x1
    stb r0, 0x21(r28)
    @L_80136E80:
    lwz r0, 0x0(r29)
    srawi r0, r0, 24
    stb r0, 0x13(r28)
    lwz r0, 0x0(r29)
    extrwi r0, r0, 8, 8
    stb r0, 0x12(r28)
    lwz r0, 0x0(r29)
    extrwi r0, r0, 8, 16
    stb r0, 0x11(r28)
    lwz r0, 0x0(r29)
    addi r29, r29, 0x28
    stb r0, 0x10(r28)
    b @L_801370EC
    addi r3, r28, 0x8
    li r4, 0x0
    li r5, 0x24
    bl memset
    lwz r0, 0x8(r29)
    sth r0, 0x24(r28)
    lwz r0, 0x10(r29)
    cmpwi r0, 0x1
    beq @L_80136EF4
    bge @L_80136EFC
    cmpwi r0, 0x0
    bge @L_80136EE8
    b @L_80136EFC
    @L_80136EE8:
    li r0, 0x0
    stw r0, 0x20(r28)
    b @L_80136EFC
    @L_80136EF4:
    li r0, 0x1
    stw r0, 0x20(r28)
    @L_80136EFC:
    lwz r4, 0xc(r29)
    addi r3, r29, 0x37
    li r0, 0x4e20
    sth r4, 0x26(r28)
    clrrwi r27, r3, 5
    stw r0, 0x8(r28)
    lwz r0, 0x0(r29)
    cmpwi r0, 0x0
    beq @L_80136F90
    bl fn_801DB060
    stw r3, 0xc(r28)
    bl fn_801DB060
    stw r3, 0x10(r28)
    mr r3, r27
    li r5, 0x4e20
    lwz r4, 0x0(r29)
    lwz r6, 0xc(r28)
    bl fn_8010147C
    lwz r4, 0xc(r28)
    li r3, 0x4e20
    bl fn_800F9318
    lwz r6, 0x10(r28)
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    lwz r4, 0x10(r28)
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80136F7C
    li r4, 0x0
    bl fn_800E4014
    @L_80136F7C:
    lwz r3, 0x0(r29)
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    add r27, r27, r0
    b @L_80136F9C
    @L_80136F90:
    li r0, 0x0
    stw r0, 0xc(r28)
    stw r0, 0x10(r28)
    @L_80136F9C:
    li r0, 0x0
    mr r29, r27
    stw r0, 0x14(r28)
    b @L_801370EC
    addi r3, r28, 0x8
    li r27, 0x0
    li r4, 0x0
    li r5, 0xb4
    bl memset
    lwz r0, 0x14(r29)
    cmpwi r0, 0x2
    beq @L_80136FEC
    bge @L_80136FF4
    cmpwi r0, 0x1
    bge @L_80136FDC
    b @L_80136FF4
    @L_80136FDC:
    lfs f0, lbl_8047D120(r2)
    li r27, -0x4
    stfs f0, 0x24(r28)
    b @L_80136FF4
    @L_80136FEC:
    lfs f0, 0x18(r29)
    stfs f0, 0x24(r28)
    @L_80136FF4:
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    xoris r3, r30, 0x8000
    add r4, r27, r29
    lfd f3, lbl_8047D128(r2)
    stw r5, 0x30(r1)
    addi r0, r4, 0x3b
    lfs f0, lbl_8047D118(r2)
    clrrwi r31, r0, 5
    lfd f1, 0x30(r1)
    li r4, 0x20
    stw r3, 0x2c(r1)
    fsubs f2, f1, f3
    stw r5, 0x28(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0xba(r28)
    lfs f0, 0x8(r29)
    stfs f0, 0x20(r28)
    lfs f0, 0x20(r28)
    stfs f0, 0x1c(r28)
    lfs f0, 0xc(r29)
    stfs f0, 0x2c(r28)
    lwz r0, 0x0(r29)
    sth r0, 0x30(r28)
    lwz r0, 0x4(r29)
    sth r0, 0x32(r28)
    lwz r3, 0x10(r29)
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_801370D0
    bl fn_800E27B0
    lwz r5, 0x10(r29)
    mr r27, r3
    mr r4, r31
    addi r0, r5, 0x1f
    clrrwi r5, r0, 5
    bl memcpy
    mr r3, r27
    bl fn_800EFD3C
    stw r3, 0xc(r28)
    mr r4, r30
    lwz r3, 0xc(r28)
    bl fn_800EFD14
    b @L_801370D8
    @L_801370D0:
    li r0, 0x0
    stw r0, 0xc(r28)
    @L_801370D8:
    lwz r3, 0x10(r29)
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    add r31, r31, r0
    mr r29, r31
    @L_801370EC:
    mr r3, r29
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    lmw r27, 0x3c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
