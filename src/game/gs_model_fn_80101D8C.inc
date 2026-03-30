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
    lwz r0, lbl_8047ACF0(r13)
    cmpwi r0, 0x0
    beq @L_80101F8C
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047CD80(r13)
    lfs f3, lbl_8047CD84(r13)
    fmr f2, f1
    lfs f4, lbl_8047CD88(r13)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x2
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    lwz r5, lbl_8047ACF8(r13)
    lis r4, 0x4330
    lbz r0, lbl_80478B28(r13)
    li r3, 0x7
    stw r5, 0xc(r1)
    lfd f2, lbl_8047CD98(r13)
    stw r4, 0x8(r1)
    lfs f0, lbl_8047CDA0(r13)
    lfd f1, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f1, f2
    lfs f3, lbl_8047CD84(r13)
    stw r4, 0x10(r1)
    fdivs f1, f1, f0
    lfd f0, 0x10(r1)
    fsubs f0, f0, f2
    fdivs f0, f1, f0
    fmuls f30, f3, f0
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, lbl_8047CD80(r13)
    lfs f2, lbl_8047CDA4(r13)
    fmr f3, f1
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, lbl_8047CDA8(r13)
    lfs f3, lbl_8047CD80(r13)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    li r3, 0x1
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    lfd f31, lbl_8047CDB8(r13)
    li r29, 0x0
    li r30, 0x0
    lis r31, 0x4330
    b @L_80101F6C
@L_80101EF0:
    divw r0, r30, r3
    stw r31, 0x10(r1)
    li r3, 0x2
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D67BC
    fmr f1, f30
    lfs f2, lbl_8047CDAC(r13)
    lfs f3, lbl_8047CD80(r13)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, lbl_8047CDB0(r13)
    lfs f3, lbl_8047CD80(r13)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    addi r30, r30, 0x280
    addi r29, r29, 0x1
@L_80101F6C:
    lbz r3, lbl_80478B28(r13)
    extsb r0, r29
    cmpw r0, r3
    ble @L_80101EF0
    bl OSGetTick
    stw r3, lbl_8047ACE8(r13)
    stw r3, lbl_8047ACEC(r13)
    bl fn_800BE30C
@L_80101F8C:
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

