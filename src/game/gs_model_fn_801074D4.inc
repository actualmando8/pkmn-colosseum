    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    stw r29, 0x84(r1)
    mr r29, r3
    mr r30, r4
    lwz r0, 0x0(r30)
    li r31, 0x0
    cmplwi r0, 0x0
    beq @L_80107E54
    lbz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_80107818
    lha r3, 0x14(r30)
    addi r0, r3, 0x1
    sth r0, 0x14(r30)
    lbz r0, 0x8(r30)
    cmpwi r0, 0x2
    beq @L_8010761C
    bge @L_80107540
    cmpwi r0, 0x1
    bge @L_8010754C
    b @L_80107800
@L_80107540:
    cmpwi r0, 0x4
    bge @L_80107800
    b @L_80107714
@L_8010754C:
    lha r3, 0x14(r30)
    lis r5, 0x4330
    lha r0, 0x16(r30)
    xoris r3, r3, 0x8000
    lha r6, 0xc(r30)
    xoris r4, r0, 0x8000
    stw r3, 0xc(r1)
    lha r3, 0x10(r30)
    xoris r0, r6, 0x8000
    stw r5, 0x8(r1)
    lfd f2, lbl_8047CE28(r13)
    subf r3, r6, r3
    lfd f0, 0x8(r1)
    xoris r3, r3, 0x8000
    stw r4, 0x14(r1)
    fsubs f1, f0, f2
    stw r5, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r3, 0x1c(r1)
    fsubs f0, f0, f2
    stw r5, 0x18(r1)
    fdivs f3, f1, f0
    lfd f1, 0x18(r1)
    stw r0, 0x24(r1)
    stw r5, 0x20(r1)
    lfd f0, 0x20(r1)
    stw r5, 0x30(r1)
    fsubs f1, f1, f2
    stw r5, 0x38(r1)
    fsubs f0, f0, f2
    fmadds f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    sth r0, 0x0(r29)
    lha r4, 0xe(r30)
    lha r3, 0x12(r30)
    xoris r0, r4, 0x8000
    subf r3, r4, r3
    stw r0, 0x3c(r1)
    xoris r0, r3, 0x8000
    stw r0, 0x34(r1)
    lfd f0, 0x38(r1)
    lfd f1, 0x30(r1)
    fsubs f0, f0, f2
    fsubs f1, f1, f2
    fmadds f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x40(r1)
    lwz r0, 0x44(r1)
    sth r0, 0x2(r29)
    b @L_80107800
@L_8010761C:
    lha r0, 0x14(r30)
    lis r5, 0x4330
    lha r3, 0x16(r30)
    xoris r6, r0, 0x8000
    lha r7, 0xc(r30)
    stw r6, 0x44(r1)
    xoris r4, r3, 0x8000
    lha r3, 0x10(r30)
    xoris r0, r7, 0x8000
    stw r5, 0x40(r1)
    lfd f4, lbl_8047CE28(r13)
    subf r3, r7, r3
    lfd f0, 0x40(r1)
    xoris r3, r3, 0x8000
    stw r6, 0x3c(r1)
    fsubs f2, f0, f4
    stw r5, 0x38(r1)
    lfd f0, 0x38(r1)
    stw r4, 0x34(r1)
    fsubs f1, f0, f4
    stw r5, 0x30(r1)
    lfd f0, 0x30(r1)
    fmuls f1, f2, f1
    stw r4, 0x2c(r1)
    fsubs f0, f0, f4
    stw r5, 0x28(r1)
    fdivs f3, f1, f0
    lfd f1, 0x28(r1)
    stw r3, 0x24(r1)
    stw r5, 0x20(r1)
    lfd f0, 0x20(r1)
    stw r0, 0x1c(r1)
    fsubs f2, f1, f4
    stw r5, 0x18(r1)
    fsubs f1, f0, f4
    lfd f0, 0x18(r1)
    fdivs f2, f3, f2
    stw r5, 0x8(r1)
    stw r5, 0x48(r1)
    fsubs f0, f0, f4
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x0(r29)
    lha r4, 0xe(r30)
    lha r3, 0x12(r30)
    xoris r0, r4, 0x8000
    subf r3, r4, r3
    stw r0, 0x4c(r1)
    xoris r0, r3, 0x8000
    stw r0, 0xc(r1)
    lfd f0, 0x48(r1)
    lfd f1, 0x8(r1)
    fsubs f0, f0, f4
    fsubs f1, f1, f4
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x50(r1)
    lwz r0, 0x54(r1)
    sth r0, 0x2(r29)
    b @L_80107800
@L_80107714:
    lha r3, 0x16(r30)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CE28(r13)
    stw r0, 0x54(r1)
    lfd f0, 0x50(r1)
    fsub f1, f0, f1
    bl fn_800CE77C
    lha r3, 0x14(r30)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    fmr f31, f1
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CE28(r13)
    stw r0, 0x4c(r1)
    lfd f0, 0x48(r1)
    fsub f1, f0, f1
    bl fn_800CE77C
    fdiv f3, f1, f31
    lis r3, 0x4330
    lha r5, 0xc(r30)
    lha r4, 0x10(r30)
    xoris r0, r5, 0x8000
    stw r3, 0x40(r1)
    subf r4, r5, r4
    stw r0, 0x3c(r1)
    xoris r0, r4, 0x8000
    lfd f2, lbl_8047CE28(r13)
    stw r0, 0x44(r1)
    frsp f3, f3
    stw r3, 0x38(r1)
    lfd f1, 0x40(r1)
    lfd f0, 0x38(r1)
    fsubs f1, f1, f2
    stw r3, 0x28(r1)
    fsubs f0, f0, f2
    stw r3, 0x20(r1)
    fmadds f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    sth r0, 0x0(r29)
    lha r4, 0xe(r30)
    lha r3, 0x12(r30)
    xoris r0, r4, 0x8000
    subf r3, r4, r3
    stw r0, 0x24(r1)
    xoris r0, r3, 0x8000
    stw r0, 0x2c(r1)
    lfd f0, 0x20(r1)
    lfd f1, 0x28(r1)
    fsubs f0, f0, f2
    fsubs f1, f1, f2
    fmadds f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    sth r0, 0x2(r29)
@L_80107800:
    lha r3, 0x14(r30)
    lha r0, 0x16(r30)
    cmpw r3, r0
    blt @L_80107818
    li r0, 0x0
    stb r0, 0x8(r30)
@L_80107818:
    lbz r0, 0x9(r30)
    cmplwi r0, 0x0
    beq @L_801079A8
    lha r3, 0x20(r30)
    addi r0, r3, 0x1
    sth r0, 0x20(r30)
    lbz r0, 0x9(r30)
    cmpwi r0, 0x1
    beq @L_80107840
    b @L_80107990
@L_80107840:
    lha r3, 0x20(r30)
    lis r4, 0x4330
    lha r0, 0x22(r30)
    xoris r5, r3, 0x8000
    lbz r6, 0x18(r30)
    xoris r3, r0, 0x8000
    stw r5, 0x54(r1)
    lbz r0, 0x1c(r30)
    stw r4, 0x50(r1)
    lfd f3, lbl_8047CE28(r13)
    subf r0, r6, r0
    lfd f0, 0x50(r1)
    xoris r0, r0, 0x8000
    stw r3, 0x4c(r1)
    fsubs f1, f0, f3
    lfd f2, lbl_8047CE30(r13)
    stw r4, 0x48(r1)
    lfd f0, 0x48(r1)
    stw r0, 0x44(r1)
    fsubs f0, f0, f3
    stw r4, 0x40(r1)
    fdivs f4, f1, f0
    lfd f1, 0x40(r1)
    stw r6, 0x3c(r1)
    stw r4, 0x38(r1)
    lfd f0, 0x38(r1)
    stw r4, 0x28(r1)
    fsubs f1, f1, f3
    stw r4, 0x20(r1)
    fsubs f0, f0, f2
    stw r4, 0x10(r1)
    fmadds f0, f4, f1, f0
    stw r4, 0x8(r1)
    stw r4, 0x60(r1)
    fctiwz f0, f0
    stw r4, 0x68(r1)
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stb r0, 0x4(r29)
    lbz r3, 0x19(r30)
    lbz r0, 0x1d(r30)
    stw r3, 0x24(r1)
    subf r0, r3, r0
    xoris r0, r0, 0x8000
    lfd f0, 0x20(r1)
    stw r0, 0x2c(r1)
    fsubs f0, f0, f2
    lfd f1, 0x28(r1)
    fsubs f1, f1, f3
    fmadds f0, f4, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x5(r29)
    lbz r3, 0x1a(r30)
    lbz r0, 0x1e(r30)
    stw r3, 0xc(r1)
    subf r0, r3, r0
    xoris r0, r0, 0x8000
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f0, f0, f2
    lfd f1, 0x10(r1)
    fsubs f1, f1, f3
    fmadds f0, f4, f1, f0
    fctiwz f0, f0
    stfd f0, 0x58(r1)
    lwz r0, 0x5c(r1)
    stb r0, 0x6(r29)
    lbz r3, 0x1b(r30)
    lbz r0, 0x1f(r30)
    stw r3, 0x6c(r1)
    subf r0, r3, r0
    xoris r0, r0, 0x8000
    lfd f0, 0x68(r1)
    stw r0, 0x64(r1)
    fsubs f0, f0, f2
    lfd f1, 0x60(r1)
    fsubs f1, f1, f3
    fmadds f0, f4, f1, f0
    fctiwz f0, f0
    stfd f0, 0x70(r1)
    lwz r0, 0x74(r1)
    stb r0, 0x7(r29)
@L_80107990:
    lha r3, 0x20(r30)
    lha r0, 0x22(r30)
    cmpw r3, r0
    blt @L_801079A8
    li r0, 0x0
    stb r0, 0x9(r30)
@L_801079A8:
    lbz r0, 0xa(r30)
    cmplwi r0, 0x0
    beq @L_80107A4C
    lha r3, 0x34(r30)
    addi r0, r3, 0x1
    sth r0, 0x34(r30)
    lbz r0, 0xa(r30)
    cmpwi r0, 0x1
    beq @L_801079D0
    b @L_80107A34
@L_801079D0:
    lha r4, 0x34(r30)
    lis r3, 0x4330
    lha r0, 0x36(r30)
    xoris r4, r4, 0x8000
    stw r3, 0x70(r1)
    xoris r0, r0, 0x8000
    lfd f3, lbl_8047CE28(r13)
    stw r4, 0x74(r1)
    lfs f4, 0x24(r30)
    lfd f0, 0x70(r1)
    stw r0, 0x6c(r1)
    fsubs f2, f0, f3
    lfs f0, 0x2c(r30)
    stw r3, 0x68(r1)
    fsubs f0, f0, f4
    lfd f1, 0x68(r1)
    fsubs f1, f1, f3
    fdivs f2, f2, f1
    fmadds f0, f2, f0, f4
    stfs f0, 0xc(r29)
    lfs f1, 0x28(r30)
    lfs f0, 0x30(r30)
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f1
    stfs f0, 0x10(r29)
@L_80107A34:
    lha r3, 0x34(r30)
    lha r0, 0x36(r30)
    cmpw r3, r0
    blt @L_80107A4C
    li r0, 0x0
    stb r0, 0xa(r30)
@L_80107A4C:
    lha r3, 0x6(r30)
    cmpwi r3, 0x0
    ble @L_80107A64
    subi r0, r3, 0x1
    sth r0, 0x6(r30)
    b @L_80107E54
@L_80107A64:
    lha r0, 0x4(r30)
    lwz r3, 0x0(r30)
    mulli r0, r0, 0xc
    add r3, r3, r0
    lbz r5, 0x1(r3)
    cmplwi r5, 0xb
    bgt @L_80107E38
    lis r4, jumptable_8035B400@ha
    rlwinm r0, r5, 2, 0, 29
    addi r4, r4, jumptable_8035B400@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r0, 0x0
    li r31, 0x1
    stw r0, 0x0(r30)
    sth r0, 0x4(r30)
    b @L_80107E38
    lha r0, 0x2(r3)
    sth r0, 0x6(r30)
    b @L_80107E38
    cmplwi r5, 0x9
    bne @L_80107AD4
    lha r0, 0x8(r29)
    sth r0, 0x10(r30)
    lha r0, 0xa(r29)
    sth r0, 0x12(r30)
    b @L_80107B38
@L_80107AD4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmpwi r0, 0x1
    beq @L_80107B10
    bge @L_80107B38
    cmpwi r0, 0x0
    bge @L_80107AF4
    b @L_80107B38
@L_80107AF4:
    lwz r0, 0x4(r3)
    extsh r0, r0
    sth r0, 0x10(r30)
    lwz r0, 0x8(r3)
    extsh r0, r0
    sth r0, 0x12(r30)
    b @L_80107B38
@L_80107B10:
    lha r4, 0x0(r29)
    lwz r0, 0x4(r3)
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x10(r30)
    lha r4, 0x2(r29)
    lwz r0, 0x8(r3)
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x12(r30)
@L_80107B38:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 29, 30, 31
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_80107B54
    lha r0, 0x0(r29)
    sth r0, 0x10(r30)
@L_80107B54:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 29, 30, 31
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80107B70
    lha r0, 0x2(r29)
    sth r0, 0x12(r30)
@L_80107B70:
    lha r0, 0x2(r3)
    sth r0, 0x16(r30)
    lha r0, 0x16(r30)
    cmpwi r0, 0x0
    bne @L_80107BA0
    lha r3, 0x10(r30)
    li r0, 0x0
    sth r3, 0x0(r29)
    lha r3, 0x12(r30)
    sth r3, 0x2(r29)
    stb r0, 0x8(r30)
    b @L_80107E38
@L_80107BA0:
    lha r4, 0x0(r29)
    li r0, 0x0
    sth r4, 0xc(r30)
    lha r4, 0x2(r29)
    sth r4, 0xe(r30)
    sth r0, 0x14(r30)
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 27, 30, 31
    cmpwi r0, 0x2
    beq @L_80107C00
    bge @L_80107BDC
    cmpwi r0, 0x0
    beq @L_80107BE8
    bge @L_80107BF4
    b @L_80107E38
@L_80107BDC:
    cmpwi r0, 0x4
    bge @L_80107E38
    b @L_80107C0C
@L_80107BE8:
    li r0, 0x1
    stb r0, 0x8(r30)
    b @L_80107E38
@L_80107BF4:
    li r0, 0x2
    stb r0, 0x8(r30)
    b @L_80107E38
@L_80107C00:
    li r0, 0x3
    stb r0, 0x8(r30)
    b @L_80107E38
@L_80107C0C:
    li r0, 0x1
    stb r0, 0x8(r30)
    b @L_80107E38
    lbz r5, 0x4(r29)
    li r4, 0x0
    li r0, 0x1
    stb r5, 0x18(r30)
    lbz r5, 0x5(r29)
    stb r5, 0x19(r30)
    lbz r5, 0x6(r29)
    stb r5, 0x1a(r30)
    lbz r5, 0x7(r29)
    stb r5, 0x1b(r30)
    lwz r5, 0x4(r3)
    rlwinm r5, r5, 8, 24, 31
    stb r5, 0x1c(r30)
    lwz r5, 0x4(r3)
    rlwinm r5, r5, 16, 24, 31
    stb r5, 0x1d(r30)
    lwz r5, 0x4(r3)
    rlwinm r5, r5, 24, 24, 31
    stb r5, 0x1e(r30)
    lwz r5, 0x4(r3)
    rlwinm r5, r5, 0, 24, 31
    stb r5, 0x1f(r30)
    sth r4, 0x20(r30)
    lha r3, 0x2(r3)
    sth r3, 0x22(r30)
    stb r0, 0x9(r30)
    lha r0, 0x22(r30)
    cmpwi r0, 0x0
    bne @L_80107E38
    lbz r0, 0x1c(r30)
    stb r0, 0x4(r29)
    lbz r0, 0x1d(r30)
    stb r0, 0x5(r29)
    lbz r0, 0x1e(r30)
    stb r0, 0x6(r29)
    lbz r0, 0x1f(r30)
    stb r0, 0x7(r29)
    stb r4, 0x9(r30)
    b @L_80107E38
    lfs f0, 0xc(r29)
    lis r5, 0x4330
    stw r5, 0x70(r1)
    li r4, 0x0
    lfd f2, lbl_8047CE28(r13)
    li r0, 0x1
    stfs f0, 0x24(r30)
    lfs f1, lbl_8047CE20(r13)
    lfs f0, 0x10(r29)
    stw r5, 0x68(r1)
    stfs f0, 0x28(r30)
    lwz r5, 0x4(r3)
    xoris r5, r5, 0x8000
    stw r5, 0x74(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f2
    fdivs f0, f0, f1
    stfs f0, 0x2c(r30)
    lwz r5, 0x8(r3)
    xoris r5, r5, 0x8000
    stw r5, 0x6c(r1)
    lfd f0, 0x68(r1)
    fsubs f0, f0, f2
    fdivs f0, f0, f1
    stfs f0, 0x30(r30)
    sth r4, 0x34(r30)
    lha r3, 0x2(r3)
    sth r3, 0x36(r30)
    stb r0, 0xa(r30)
    lha r0, 0x36(r30)
    cmpwi r0, 0x0
    bne @L_80107E38
    lfs f0, 0x2c(r30)
    stfs f0, 0xc(r29)
    lfs f0, 0x30(r30)
    stfs f0, 0x10(r29)
    stb r4, 0xa(r30)
    b @L_80107E38
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80107D6C
    lbz r0, 0x20(r29)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x20(r29)
    b @L_80107E38
@L_80107D6C:
    lbz r0, 0x20(r29)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x20(r29)
    b @L_80107E38
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80107D98
    li r0, 0x1
    stb r0, 0x3a(r30)
    b @L_80107E38
@L_80107D98:
    li r0, 0x0
    stb r0, 0x3a(r30)
    b @L_80107E38
    lbz r0, 0xb(r30)
    cmplwi r0, 0x0
    bne @L_80107DC0
    li r0, 0x1
    stb r0, 0xb(r30)
    lha r0, 0x2(r3)
    sth r0, 0x38(r30)
@L_80107DC0:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    bne @L_80107DDC
    lha r4, 0x38(r30)
    subi r0, r4, 0x1
    sth r0, 0x38(r30)
@L_80107DDC:
    lha r0, 0x38(r30)
    cmpwi r0, 0x0
    bge @L_80107DF8
    li r0, 0x0
    sth r0, 0x38(r30)
    stb r0, 0xb(r30)
    b @L_80107E38
@L_80107DF8:
    lwz r0, 0x4(r3)
    extsh r0, r0
    sth r0, 0x4(r30)
    b @L_80107A4C
    lwz r3, 0x4(r3)
    bl fn_8005D858
    lwz r0, 0x10(r3)
    stw r0, 0x14(r29)
    lha r0, 0x8(r3)
    sth r0, 0x18(r29)
    lha r0, 0xa(r3)
    sth r0, 0x1a(r29)
    lha r0, 0xc(r3)
    sth r0, 0x1c(r29)
    lha r0, 0xe(r3)
    sth r0, 0x1e(r29)
@L_80107E38:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80107E54
    lha r3, 0x4(r30)
    addi r0, r3, 0x1
    sth r0, 0x4(r30)
    b @L_80107A4C
@L_80107E54:
    psq_l f31, 0x98(r1), 0, 0
    lwz r0, 0xa4(r1)
    lfd f31, 0x90(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    lwz r29, 0x84(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
