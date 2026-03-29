    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802E52C8@ha
    addi r31, r3, lbl_803A6818@l
    lfs f1, lbl_8047BCA0(r2)
    lfs f0, 0x4c(r31)
    addi r30, r4, lbl_802E52C8@l
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r27)
    lha r5, 0x6(r28)
    subi r0, r5, 0xd47
    cmplwi r0, 0x44
    bgt @L_8003FFF4
    lis r3, jumptable_802E60C4@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E60C4@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x1dc(r3)
    mulli r3, r5, 0x1c
    addi r0, r4, lbl_802EF0A8@l
    fctiwz f0, f0
    add r3, r0, r3
    lha r3, 0x2(r3)
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    b @L_8003FFF4
    lfs f0, 0x1dc(r31)
    lis r3, lbl_802EF0A8@ha
    mulli r4, r5, 0x1c
    fctiwz f0, f0
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r4
    stfd f0, 0x8(r1)
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lbz r3, 0x1c8(r31)
    lis r4, lbl_802EF0A8@ha
    lbz r0, 0x15f(r31)
    addi r4, r4, lbl_802EF0A8@l
    extsb r3, r3
    extsb r0, r0
    addis r4, r4, 0x1
    subf r0, r3, r0
    cmpwi r0, 0x1d
    blt @L_8003F5C8
    li r0, 0x1c
    @L_8003F5C8:
    mulli r0, r0, 0x1d
    lha r3, 0x7b00(r4)
    extsh r0, r0
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x52(r28)
    b @L_8003FFF4
    lfs f0, 0x1dc(r31)
    lis r3, lbl_802EF0A8@ha
    mulli r4, r5, 0x1c
    fctiwz f0, f0
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r4
    stfd f0, 0x8(r1)
    lha r3, 0x2(r3)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r0, 0x1c8(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_8003F668
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003F67C
    @L_8003F668:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003F67C:
    lfs f1, lbl_8047BCA8(r2)
    lfs f0, 0x40(r3)
    lfs f2, lbl_8047BCA4(r2)
    fmuls f0, f1, f0
    fmuls f0, f1, f0
    fmadds f1, f2, f0, f2
    bl fn_800CE148
    lis r3, lbl_802EF0A8@ha
    lis r0, 0x4330
    addi r3, r3, lbl_802EF0A8@l
    stw r0, 0x8(r1)
    addis r3, r3, 0x1
    lfd f2, lbl_8047BCB0(r2)
    lha r0, 0x7b1c(r3)
    frsp f3, f1
    lfs f1, lbl_8047BCA8(r2)
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f2
    fmadds f0, f1, f3, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r28)
    b @L_8003FFF4
    lfs f0, 0x1dc(r31)
    lis r3, lbl_802EF0A8@ha
    mulli r4, r5, 0x1c
    fctiwz f0, f0
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r4
    stfd f0, 0x10(r1)
    lha r3, 0x2(r3)
    lwz r0, 0x14(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lbz r0, 0x1c9(r31)
    extsb r3, r0
    cmpwi r3, 0xa
    blt @L_8003F784
    lbz r0, 0x160(r31)
    extsb r0, r0
    cmpw r0, r3
    beq @L_8003F770
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003F798
    @L_8003F770:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003F784:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003F798:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BCA8(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f2, lbl_8047BCA4(r2)
    lfs f0, 0x40(r3)
    fmuls f0, f1, f0
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    lis r3, lbl_802EF0A8@ha
    lis r0, 0x4330
    addi r3, r3, lbl_802EF0A8@l
    stw r0, 0x10(r1)
    addis r3, r3, 0x1
    lfd f2, lbl_8047BCB0(r2)
    lha r0, 0x7b38(r3)
    frsp f3, f1
    lfs f1, lbl_8047BCA8(r2)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f2
    fmadds f0, f1, f3, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x52(r28)
    b @L_8003FFF4
    lis r3, lbl_803A6818@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x1dc(r3)
    mulli r3, r5, 0x1c
    addi r0, r4, lbl_802EF0A8@l
    fctiwz f0, f0
    add r3, r0, r3
    lha r3, 0x2(r3)
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    b @L_8003FFF4
    lfs f0, 0x1dc(r31)
    lis r3, lbl_802EF0A8@ha
    mulli r4, r5, 0x1c
    fctiwz f0, f0
    addi r0, r3, lbl_802EF0A8@l
    add r3, r0, r4
    stfd f0, 0x10(r1)
    lha r3, 0x2(r3)
    lwz r0, 0x14(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x50(r28)
    lha r5, 0x84(r27)
    lha r3, 0x50(r28)
    lha r4, 0x86(r27)
    lha r0, 0x52(r28)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lha r0, 0x6(r28)
    lis r3, lbl_802EF0A8@ha
    addi r4, r3, lbl_802EF0A8@l
    mulli r0, r0, 0x1c
    addis r7, r4, 0x1
    lha r3, 0x7b52(r7)
    add r4, r4, r0
    lha r0, 0x7b54(r7)
    lha r6, 0x2(r4)
    lha r4, 0x4(r4)
    lha r5, 0x7b56(r7)
    subf r3, r6, r3
    lha r6, 0x7b58(r7)
    subf r4, r4, r0
    bl fn_800FE38C
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x4
    beq @L_8003F9A8
    li r29, 0x0
    lfs f30, 0x1d0(r31)
    mr r28, r29
    lfs f31, lbl_8047BCAC(r2)
    b @L_8003F994
    @L_8003F924:
    lbz r0, 0x1c8(r31)
    extsb r4, r29
    extsb r3, r0
    subi r0, r3, 0x1
    cmpw r4, r0
    blt @L_8003F988
    lbz r0, 0x1c9(r31)
    extsb r3, r0
    addi r0, r3, 0x1
    cmpw r4, r0
    bgt @L_8003F988
    lbz r0, 0x15d(r31)
    fctiwz f0, f30
    addi r3, r30, 0x24c
    lbz r5, 0x8b(r27)
    extsb r0, r0
    li r4, -0x100
    slwi r0, r0, 2
    stfd f0, 0x10(r1)
    or r5, r5, r4
    lwzx r6, r3, r0
    li r3, 0x0
    lwz r4, 0x14(r1)
    lwzx r6, r6, r28
    bl fn_800FB680
    @L_8003F988:
    fadds f30, f30, f31
    addi r28, r28, 0x4
    addi r29, r29, 0x1
    @L_8003F994:
    lbz r0, 0x160(r31)
    extsb r3, r29
    extsb r0, r0
    cmpw r3, r0
    blt @L_8003F924
    @L_8003F9A8:
    bl fn_800FE35C
    b @L_8003FFF4
    li r3, 0x36ed
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r4, r3
    lbz r3, 0x8b(r27)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, -0x2
    add r6, r5, r6
    or r5, r3, r0
    srawi r0, r6, 1
    li r6, 0x36ed
    extsh r3, r0
    bl fn_800FB680
    b @L_8003FFF4
    li r3, 0x36ec
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r4, r3
    lbz r3, 0x8b(r27)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, -0x2
    add r6, r5, r6
    or r5, r3, r0
    srawi r0, r6, 1
    li r6, 0x36ec
    extsh r3, r0
    bl fn_800FB680
    b @L_8003FFF4
    li r3, 0x36eb
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r4, r3
    lbz r3, 0x8b(r27)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, -0x2
    add r6, r5, r6
    or r5, r3, r0
    srawi r0, r6, 1
    li r6, 0x36eb
    extsh r3, r0
    bl fn_800FB680
    b @L_8003FFF4
    li r3, 0x36ea
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r4, r3
    lbz r3, 0x8b(r27)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, -0x2
    add r6, r5, r6
    or r5, r3, r0
    srawi r0, r6, 1
    li r6, 0x36ea
    extsh r3, r0
    bl fn_800FB680
    b @L_8003FFF4
    li r3, 0x36e9
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r28)
    extsh r4, r3
    lbz r3, 0x8b(r27)
    subf r6, r4, r0
    li r0, -0x100
    srwi r5, r6, 31
    li r4, -0x2
    add r6, r5, r6
    or r5, r3, r0
    srawi r0, r6, 1
    li r6, 0x36e9
    extsh r3, r0
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x15c(r31)
    addi r3, r30, 0x24c
    lwz r4, 0xc(r3)
    li r5, -0x100
    extsb r0, r0
    lbz r7, 0x8b(r27)
    slwi r0, r0, 2
    li r3, 0x0
    lwzx r6, r4, r0
    or r5, r7, r5
    li r4, 0x0
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x15b(r31)
    addi r3, r30, 0x24c
    lwz r3, 0x8(r3)
    extsb r0, r0
    slwi r0, r0, 2
    lwzx r3, r3, r0
    bl fn_800FA280
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    lbz r5, 0x8b(r27)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x15a(r31)
    addi r3, r30, 0x24c
    lwz r3, 0x8(r3)
    extsb r0, r0
    slwi r0, r0, 2
    lwzx r3, r3, r0
    bl fn_800FA280
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    lbz r5, 0x8b(r27)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x159(r31)
    addi r3, r30, 0x24c
    lwz r4, 0x4(r3)
    li r5, -0x100
    extsb r0, r0
    lbz r7, 0x8b(r27)
    slwi r0, r0, 2
    li r3, 0x0
    lwzx r6, r4, r0
    or r5, r7, r5
    li r4, 0x0
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x158(r31)
    li r5, -0x100
    lbz r6, 0x8b(r27)
    li r3, 0x0
    extsb r0, r0
    lwz r4, 0x24c(r30)
    slwi r0, r0, 2
    or r5, r6, r5
    lwzx r6, r4, r0
    li r4, 0x0
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003FC58
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FC58:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x1
    bne @L_8003FC90
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FC90:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    bne @L_8003FCC8
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FCC8:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x3
    bne @L_8003FD00
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FD00:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x4
    bne @L_8003FD38
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FD38:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lwz r0, 0x154(r31)
    cmpwi r0, 0x1
    beq @L_8003FDD4
    bge @L_8003FFF4
    cmpwi r0, 0x0
    bge @L_8003FD68
    b @L_8003FFF4
    @L_8003FD68:
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x4
    bne @L_8003FDAC
    lwz r4, 0x1e4(r31)
    cmpwi r4, 0x0
    bne @L_8003FD90
    slwi r0, r0, 2
    addi r3, r30, 0x270
    lwzx r6, r3, r0
    @L_8003FD90:
    cmpwi r4, 0x2
    bne @L_8003FD9C
    li r6, 0x3712
    @L_8003FD9C:
    cmpwi r4, 0x1
    bne @L_8003FDB8
    li r6, 0x3714
    b @L_8003FDB8
    @L_8003FDAC:
    slwi r0, r0, 2
    addi r3, r30, 0x270
    lwzx r6, r3, r0
    @L_8003FDB8:
    lbz r4, 0x8b(r27)
    li r0, -0x100
    li r3, 0x0
    or r5, r4, r0
    li r4, 0x0
    bl fn_800FB680
    b @L_8003FFF4
    @L_8003FDD4:
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x3
    bne @L_8003FFF4
    lbz r0, 0x15f(r31)
    addi r4, r30, 0x25c
    lbz r6, 0x8b(r27)
    li r5, -0x100
    extsb r0, r0
    li r3, 0x0
    slwi r0, r0, 2
    or r5, r6, r5
    lwzx r6, r4, r0
    li r4, 0x0
    bl fn_800FB680
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003FE54
    cmpwi r0, 0x0
    bne @L_8003FE40
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FE40:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FE54:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003FEA8
    cmpwi r0, 0x1
    bne @L_8003FE94
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FE94:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FEA8:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003FEF8
    bne @L_8003FEE4
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FEE4:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FEF8:
    lbz r0, 0x15e(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003FF1C
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FF1C:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003FF6C
    bne @L_8003FF58
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FF58:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FF6C:
    lbz r0, 0x15e(r31)
    extsb r0, r0
    cmpwi r0, 0x1
    bne @L_8003FF90
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FF90:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    lbz r0, 0x15d(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003FFE4
    cmpwi r0, 0x3
    bne @L_8003FFD0
    lbz r0, 0x4(r28)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FFD0:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    b @L_8003FFF4
    @L_8003FFE4:
    lbz r0, 0x4(r28)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r28)
    @L_8003FFF4:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lmw r27, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
