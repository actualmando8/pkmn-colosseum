    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stmw r21, 0x74(r1)
    mr r26, r4
    lbz r5, 0x64(r26)
    lis r4, 0x8081
    lbz r0, 0x88(r3)
    lis r31, lbl_80404B68@ha
    lbz r7, 0x65(r26)
    subi r10, r4, 0x7f7f
    lbz r4, 0x89(r3)
    mullw r8, r5, r0
    lbz r6, 0x66(r26)
    addi r31, r31, lbl_80404B68@l
    lbz r5, 0x8a(r3)
    lbz r0, 0x8b(r3)
    li r3, 0x3
    mullw r7, r7, r4
    lbz r4, 0x67(r26)
    mullw r5, r6, r5
    mullw r0, r4, r0
    mulhw r6, r10, r8
    mulhw r4, r10, r7
    add r6, r6, r8
    srawi r8, r6, 7
    mulhw r6, r10, r5
    rlwinm r9, r8, 1, 31, 31
    add r4, r4, r7
    add r8, r8, r9
    srawi r7, r4, 7
    rlwinm r30, r8, 0, 24, 31
    mulhw r4, r10, r0
    add r5, r6, r5
    rlwinm r6, r7, 1, 31, 31
    srawi r5, r5, 7
    add r7, r7, r6
    rlwinm r6, r5, 1, 31, 31
    add r0, r4, r0
    add r5, r5, r6
    srawi r0, r0, 7
    rlwinm r29, r7, 0, 24, 31
    rlwinm r4, r0, 1, 31, 31
    rlwinm r28, r5, 0, 24, 31
    add r0, r0, r4
    rlwinm r27, r0, 0, 24, 31
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lwz r3, 0x58(r26)
    bl fn_800F92D4
    mr r25, r3
    cmplwi r25, 0x0
    beq @L_80108858
    mr r4, r25
    li r3, 0x0
    bl fn_800D85D4
    lha r0, 0x54(r26)
    cmpwi r0, 0x0
    bge @L_80108684
    neg r0, r0
@L_80108684:
    lha r3, 0x60(r26)
    cmpw r3, r0
    beq @L_801086B4
    subi r0, r3, 0x2
    lha r3, 0x5c(r26)
    extsh r22, r0
    cmpwi r22, 0x0
    addi r0, r3, 0x1
    extsh r24, r0
    bge @L_801086BC
    li r22, 0x0
    b @L_801086BC
@L_801086B4:
    lha r24, 0x5c(r26)
    mr r22, r3
@L_801086BC:
    lha r0, 0x56(r26)
    cmpwi r0, 0x0
    bge @L_801086CC
    neg r0, r0
@L_801086CC:
    lha r3, 0x62(r26)
    cmpw r3, r0
    beq @L_801086FC
    subi r0, r3, 0x2
    lha r3, 0x5e(r26)
    extsh r21, r0
    cmpwi r21, 0x0
    addi r0, r3, 0x1
    extsh r23, r0
    bge @L_80108704
    li r21, 0x0
    b @L_80108704
@L_801086FC:
    lha r23, 0x5e(r26)
    mr r21, r3
@L_80108704:
    mr r3, r25
    bl fn_800EF4FC
    extsh r0, r24
    rlwinm r3, r3, 0, 16, 31
    lis r5, 0x4330
    stw r3, 0x2c(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CE30(r13)
    stw r5, 0x28(r1)
    addi r4, r31, 0x58
    lfd f1, lbl_8047CE28(r13)
    mr r3, r25
    lfd f0, 0x28(r1)
    stw r0, 0x34(r1)
    fsubs f2, f0, f2
    stw r5, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f2
    stfs f0, 0x4(r4)
    stfs f0, 0x58(r31)
    bl fn_800EF4FC
    extsh r4, r24
    extsh r0, r22
    add r0, r4, r0
    rlwinm r3, r3, 0, 16, 31
    lis r5, 0x4330
    stw r3, 0x3c(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CE30(r13)
    stw r5, 0x38(r1)
    addi r4, r31, 0x58
    lfd f1, lbl_8047CE28(r13)
    mr r3, r25
    lfd f0, 0x38(r1)
    stw r0, 0x44(r1)
    fsubs f2, f0, f2
    stw r5, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f2
    stfs f0, 0xc(r4)
    stfs f0, 0x8(r4)
    bl fn_800EF4F4
    extsh r4, r23
    extsh r0, r21
    add r0, r4, r0
    rlwinm r3, r3, 0, 16, 31
    lis r5, 0x4330
    stw r3, 0x4c(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CE30(r13)
    stw r5, 0x48(r1)
    addi r4, r31, 0x48
    lfd f1, lbl_8047CE28(r13)
    mr r3, r25
    lfd f0, 0x48(r1)
    stw r0, 0x54(r1)
    fsubs f2, f0, f2
    stw r5, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f2
    stfs f0, 0xc(r4)
    stfs f0, 0x48(r31)
    bl fn_800EF4F4
    extsh r0, r23
    lis r4, 0x4330
    rlwinm r3, r3, 0, 16, 31
    stw r4, 0x58(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CE30(r13)
    stw r3, 0x5c(r1)
    addi r3, r31, 0x48
    lfd f1, lbl_8047CE28(r13)
    lfd f0, 0x58(r1)
    stw r0, 0x64(r1)
    fsubs f2, f0, f2
    stw r4, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f2
    stfs f0, 0x8(r3)
    stfs f0, 0x4(r3)
    b @L_80108888
@L_80108858:
    lfs f1, lbl_8047CE38(r13)
    addi r3, r31, 0x48
    lfs f0, lbl_8047CE3C(r13)
    addi r4, r31, 0x58
    stfs f1, 0x4(r4)
    stfs f1, 0x58(r31)
    stfs f0, 0xc(r4)
    stfs f0, 0x8(r4)
    stfs f0, 0xc(r3)
    stfs f0, 0x48(r31)
    stfs f1, 0x8(r3)
    stfs f1, 0x4(r3)
@L_80108888:
    lha r5, 0x54(r26)
    cmpwi r5, 0x0
    bge @L_8010889C
    neg r0, r5
    b @L_801088A0
@L_8010889C:
    mr r0, r5
@L_801088A0:
    lha r7, 0x50(r26)
    lis r3, 0x4330
    xoris r4, r0, 0x8000
    lha r6, 0x56(r26)
    xoris r0, r7, 0x8000
    stw r4, 0x64(r1)
    lfd f3, lbl_8047CE28(r13)
    cmpwi r6, 0x0
    stw r3, 0x60(r1)
    lfs f1, lbl_8047CE40(r13)
    lfd f0, 0x60(r1)
    stw r0, 0x5c(r1)
    fsubs f2, f0, f3
    stw r3, 0x58(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f3
    fmadds f31, f2, f1, f0
    bge @L_801088EC
    neg r6, r6
@L_801088EC:
    lha r0, 0x52(r26)
    lis r3, 0x4330
    xoris r4, r6, 0x8000
    stw r3, 0x50(r1)
    xoris r0, r0, 0x8000
    lfd f3, lbl_8047CE28(r13)
    stw r4, 0x54(r1)
    cmpwi r5, 0x0
    lfs f1, lbl_8047CE40(r13)
    lfd f0, 0x50(r1)
    stw r0, 0x4c(r1)
    fsubs f2, f0, f3
    stw r3, 0x48(r1)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f3
    fmadds f30, f2, f1, f0
    bge @L_80108990
    bge @L_80108938
    neg r5, r5
@L_80108938:
    add r3, r7, r5
    lis r0, 0x4330
    xoris r4, r3, 0x8000
    stw r0, 0x60(r1)
    lfd f1, lbl_8047CE28(r13)
    addi r3, r31, 0x78
    stw r4, 0x64(r1)
    lfd f0, 0x60(r1)
    stw r0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f31
    stfs f0, 0x4(r3)
    stfs f0, 0x78(r31)
    lha r0, 0x50(r26)
    xoris r0, r0, 0x8000
    stw r0, 0x5c(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f31
    stfs f0, 0xc(r3)
    stfs f0, 0x8(r3)
    b @L_801089F8
@L_80108990:
    xoris r0, r7, 0x8000
    stw r3, 0x60(r1)
    addi r3, r31, 0x78
    stw r0, 0x64(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f3
    fsubs f0, f0, f31
    stfs f0, 0x4(r3)
    stfs f0, 0x78(r31)
    lha r5, 0x54(r26)
    cmpwi r5, 0x0
    bge @L_801089C4
    neg r5, r5
@L_801089C4:
    lha r4, 0x50(r26)
    lis r0, 0x4330
    stw r0, 0x58(r1)
    addi r3, r31, 0x78
    add r0, r4, r5
    lfd f1, lbl_8047CE28(r13)
    xoris r0, r0, 0x8000
    stw r0, 0x5c(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f31
    stfs f0, 0xc(r3)
    stfs f0, 0x8(r3)
@L_801089F8:
    lha r5, 0x56(r26)
    cmpwi r5, 0x0
    bge @L_80108A74
    lha r4, 0x52(r26)
    lis r0, 0x4330
    stw r0, 0x60(r1)
    addi r3, r31, 0x68
    xoris r0, r4, 0x8000
    lfd f1, lbl_8047CE28(r13)
    stw r0, 0x64(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f30
    stfs f0, 0xc(r3)
    stfs f0, 0x68(r31)
    bge @L_80108A3C
    neg r5, r5
@L_80108A3C:
    lha r4, 0x52(r26)
    lis r0, 0x4330
    stw r0, 0x58(r1)
    addi r3, r31, 0x68
    add r0, r4, r5
    lfd f1, lbl_8047CE28(r13)
    xoris r0, r0, 0x8000
    stw r0, 0x5c(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f30
    stfs f0, 0x8(r3)
    stfs f0, 0x4(r3)
    b @L_80108AD4
@L_80108A74:
    lha r0, 0x52(r26)
    bge @L_80108A80
    neg r5, r5
@L_80108A80:
    add r3, r0, r5
    lis r0, 0x4330
    xoris r4, r3, 0x8000
    stw r0, 0x60(r1)
    lfd f1, lbl_8047CE28(r13)
    addi r3, r31, 0x68
    stw r4, 0x64(r1)
    lfd f0, 0x60(r1)
    stw r0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f30
    stfs f0, 0xc(r3)
    stfs f0, 0x68(r31)
    lha r0, 0x52(r26)
    xoris r0, r0, 0x8000
    stw r0, 0x5c(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f1
    fsubs f0, f0, f30
    stfs f0, 0x8(r3)
    stfs f0, 0x4(r3)
@L_80108AD4:
    lis r4, lbl_8031554C@ha
    lfs f1, 0x70(r26)
    addi r4, r4, lbl_8031554C@l
    addi r3, r1, 0x14
    bl fn_800E0718
    addi r23, r31, 0x68
    addi r24, r31, 0x78
    mr r21, r23
    li r25, 0x0
    mr r22, r24
@L_80108AFC:
    lfs f1, 0x0(r22)
    addi r3, r1, 0x8
    lfs f2, lbl_8047CE38(r13)
    lfs f3, 0x0(r21)
    bl fn_800E01F4
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    mr r5, r3
    bl fn_800DFEEC
    lfs f2, 0x8(r1)
    addi r25, r25, 0x1
    lfs f0, 0x68(r26)
    lfs f1, 0x10(r1)
    fmuls f0, f2, f0
    stfs f0, 0x0(r22)
    addi r22, r22, 0x4
    lfs f0, 0x6c(r26)
    fmuls f0, f1, f0
    stfs f0, 0x0(r21)
    addi r21, r21, 0x4
    cmpwi r25, 0x4
    blt @L_80108AFC
    li r3, 0x6
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    addi r21, r31, 0x48
    addi r22, r31, 0x58
    li r25, 0x0
@L_80108B7C:
    lfs f1, 0x0(r24)
    lfs f0, 0x0(r23)
    fadds f1, f31, f1
    fadds f0, f30, f0
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x60(r1)
    stfd f0, 0x58(r1)
    lwz r3, 0x64(r1)
    lwz r4, 0x5c(r1)
    bl fn_800D61E4
    mr r4, r30
    mr r5, r29
    mr r6, r28
    mr r7, r27
    li r3, 0x0
    bl fn_800D5CB8
    lfs f1, 0x0(r22)
    li r3, 0x0
    lfs f2, 0x0(r21)
    bl fn_800D59B8
    addi r23, r23, 0x4
    addi r24, r24, 0x4
    addi r21, r21, 0x4
    addi r22, r22, 0x4
    addi r25, r25, 0x1
    cmpwi r25, 0x4
    blt @L_80108B7C
    bl fn_800D6728
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    lmw r21, 0x74(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
