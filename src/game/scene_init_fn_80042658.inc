    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stmw r19, 0xcc(r1)
    mr r20, r3
    mr r29, r4
    lis r3, lbl_803A6818@ha
    lis r4, lbl_803A67FC@ha
    addi r22, r3, lbl_803A6818@l
    li r31, 0x0
    lbz r0, 0x48(r22)
    addi r21, r4, lbl_803A67FC@l
    lwz r30, 0x0(r22)
    extsb r0, r0
    cmpwi r0, 0x1
    bne @L_800426B4
    lhz r0, 0x6(r21)
    li r3, 0x3
    stb r3, lbl_804788C4(r13)
    mr r28, r0
    b @L_800426E0
    @L_800426B4:
    cmpwi r0, 0x2
    bne @L_800426D0
    lhz r0, 0x6(r21)
    li r3, 0x2
    stb r3, lbl_804788C4(r13)
    mr r28, r0
    b @L_800426E0
    @L_800426D0:
    lhz r0, 0x6(r21)
    li r3, 0x5
    stb r3, lbl_804788C4(r13)
    mr r28, r0
    @L_800426E0:
    lhz r0, 0x0(r21)
    clrlwi r0, r0, 30
    cmpwi r0, 0x0
    beq @L_800426F4
    li r31, 0x1
    @L_800426F4:
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xbc(r1)
    lfd f1, lbl_8047BCB0(r2)
    stw r0, 0xb8(r1)
    lfd f0, 0xb8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    lis r4, lbl_803A6818@ha
    stw r3, 0xc4(r1)
    addi r23, r4, lbl_803A6818@l
    lfd f3, lbl_8047BCD8(r2)
    stw r0, 0xc0(r1)
    lfs f1, 0x40(r23)
    lfd f2, 0xc0(r1)
    lfs f0, lbl_8047BCBC(r2)
    fsubs f2, f2, f3
    fdivs f2, f2, f31
    fadds f1, f1, f2
    stfs f2, 0x3c(r23)
    fcmpo cr0, f1, f0
    stfs f1, 0x40(r23)
    cror eq, gt, eq
    bne @L_80042764
    lfs f0, lbl_8047BC94(r2)
    stfs f0, 0x40(r23)
    @L_80042764:
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lfs f6, 0x2c(r4)
    lfs f5, 0x34(r4)
    fcmpu cr0, f6, f5
    beq @L_80042880
    fsubs f4, f5, f6
    lfs f0, lbl_8047BC94(r2)
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r23)
    fcmpo cr0, f4, f0
    fmuls f3, f2, f1
    ble @L_800427A0
    fmr f1, f4
    b @L_800427A4
    @L_800427A0:
    fneg f1, f4
    @L_800427A4:
    lfs f0, lbl_8047BCD0(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800427BC
    stfs f5, 0x2c(r4)
    b @L_80042880
    @L_800427BC:
    lbz r5, 0x48(r22)
    extsb r0, r5
    cmpwi r0, 0x1
    bne @L_800427FC
    lbz r3, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0xc0(r1)
    extsb r0, r3
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xc4(r1)
    lfd f0, 0xc0(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_800427FC:
    extsb r0, r5
    cmpwi r0, 0x2
    bne @L_80042838
    lbz r3, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0xb8(r1)
    extsb r0, r3
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xbc(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_80042838:
    fmuls f2, f4, f3
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f6, f2
    fcmpo cr0, f2, f0
    fsubs f3, f5, f1
    stfs f1, 0x2c(r4)
    ble @L_80042858
    b @L_8004285C
    @L_80042858:
    fneg f2, f2
    @L_8004285C:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f3, f0
    ble @L_8004286C
    b @L_80042870
    @L_8004286C:
    fneg f3, f3
    @L_80042870:
    fcmpo cr0, f3, f2
    cror eq, lt, eq
    bne @L_80042880
    stfs f5, 0x2c(r4)
    @L_80042880:
    lis r3, lbl_803A6818@ha
    addi r24, r3, lbl_803A6818@l
    lfs f6, 0x30(r24)
    lfs f5, 0x38(r24)
    fcmpu cr0, f6, f5
    beq @L_8004299C
    fsubs f4, f5, f6
    lfs f0, lbl_8047BC94(r2)
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r23)
    fcmpo cr0, f4, f0
    fmuls f3, f2, f1
    ble @L_800428BC
    fmr f1, f4
    b @L_800428C0
    @L_800428BC:
    fneg f1, f4
    @L_800428C0:
    lfs f0, lbl_8047BCD0(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800428D8
    stfs f5, 0x30(r24)
    b @L_8004299C
    @L_800428D8:
    lbz r4, 0x48(r22)
    extsb r0, r4
    cmpwi r0, 0x1
    bne @L_80042918
    lbz r3, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0xc0(r1)
    extsb r0, r3
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xc4(r1)
    lfd f0, 0xc0(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_80042918:
    extsb r0, r4
    cmpwi r0, 0x2
    bne @L_80042954
    lbz r3, lbl_804788C4(r13)
    lis r0, 0x4330
    stw r0, 0xb8(r1)
    extsb r0, r3
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BCD4(r2)
    stw r0, 0xbc(r1)
    lfd f0, 0xb8(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    fmuls f3, f3, f0
    @L_80042954:
    fmuls f2, f4, f3
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f6, f2
    fcmpo cr0, f2, f0
    fsubs f3, f5, f1
    stfs f1, 0x30(r24)
    ble @L_80042974
    b @L_80042978
    @L_80042974:
    fneg f2, f2
    @L_80042978:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f3, f0
    ble @L_80042988
    b @L_8004298C
    @L_80042988:
    fneg f3, f3
    @L_8004298C:
    fcmpo cr0, f3, f2
    cror eq, lt, eq
    bne @L_8004299C
    stfs f5, 0x30(r24)
    @L_8004299C:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x50(r3)
    lfs f2, 0x4c(r3)
    fcmpu cr0, f0, f2
    beq @L_800429FC
    lfs f1, 0x3c(r23)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_800429E4
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r3)
    ble @L_800429FC
    stfs f0, 0x4c(r3)
    b @L_800429FC
    @L_800429E4:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r3)
    bge @L_800429FC
    stfs f0, 0x4c(r3)
    @L_800429FC:
    lis r3, lbl_803A6818@ha
    addi r25, r3, lbl_803A6818@l
    lfs f0, 0x58(r25)
    lfs f2, 0x54(r25)
    fcmpu cr0, f0, f2
    beq @L_80042A5C
    lfs f1, 0x3c(r23)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_80042A44
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r25)
    ble @L_80042A5C
    stfs f0, 0x54(r25)
    b @L_80042A5C
    @L_80042A44:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r25)
    bge @L_80042A5C
    stfs f0, 0x54(r25)
    @L_80042A5C:
    lis r3, lbl_803A6818@ha
    addi r26, r3, lbl_803A6818@l
    lfs f0, 0x60(r26)
    lfs f2, 0x5c(r26)
    fcmpu cr0, f0, f2
    beq @L_80042ABC
    lfs f1, 0x3c(r23)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCC0(r2)
    fdivs f0, f1, f0
    ble @L_80042AA4
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r26)
    ble @L_80042ABC
    stfs f0, 0x5c(r26)
    b @L_80042ABC
    @L_80042AA4:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r26)
    bge @L_80042ABC
    stfs f0, 0x5c(r26)
    @L_80042ABC:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x1dc(r3)
    lfs f1, 0x1e0(r3)
    fcmpu cr0, f0, f1
    beq @L_80042B7C
    fsubs f3, f1, f0
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r23)
    lfs f0, lbl_8047BC98(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80042AF8
    fmr f4, f0
    @L_80042AF8:
    lfs f0, lbl_8047BCC8(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_80042B0C
    fmr f4, f0
    @L_80042B0C:
    lfs f1, 0x1dc(r3)
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f1, f4
    lfs f3, 0x1e0(r3)
    fcmpo cr0, f4, f0
    fsubs f2, f3, f1
    stfs f1, 0x1dc(r3)
    ble @L_80042B30
    b @L_80042B34
    @L_80042B30:
    fneg f4, f4
    @L_80042B34:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80042B48
    fmr f0, f2
    b @L_80042B4C
    @L_80042B48:
    fneg f0, f2
    @L_80042B4C:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80042B78
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80042B68
    b @L_80042B6C
    @L_80042B68:
    fneg f2, f2
    @L_80042B6C:
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f2, f0
    bge @L_80042B7C
    @L_80042B78:
    stfs f3, 0x1dc(r3)
    @L_80042B7C:
    lis r3, lbl_803A6818@ha
    addi r27, r3, lbl_803A6818@l
    lwz r0, 0x1c(r27)
    cmpwi r0, 0x4
    beq @L_80042BD4
    bge @L_80042BA0
    cmpwi r0, 0x1
    bge @L_80042BAC
    b @L_80042FA8
    @L_80042BA0:
    cmpwi r0, 0x6
    bge @L_80042FA8
    b @L_80042D8C
    @L_80042BAC:
    addi r3, r27, 0x7c
    bl fn_80047CC0
    lis r5, lbl_803A6818@ha
    mr r4, r29
    addi r6, r5, lbl_803A6818@l
    mr r5, r28
    stb r3, 0x214(r6)
    mr r3, r20
    bl fn_80043728
    b @L_80042FA8
    @L_80042BD4:
    addi r20, r27, 0x7c
    mr r3, r20
    bl fn_800484A4
    lfs f0, lbl_8047BC94(r2)
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    stb r3, 0x214(r4)
    lwz r3, 0x24(r20)
    stfs f0, 0xa0(r1)
    stfs f0, 0xa4(r1)
    stfs f0, 0xa8(r1)
    bl fn_801DAC3C
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80042FA8
    bl fn_800E3C5C
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7994
    mr r20, r3
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7920
    extsb r4, r3
    extsb r0, r20
    lis r6, 0x4330
    lis r3, lbl_803A6818@ha
    xoris r4, r4, 0x8000
    xoris r0, r0, 0x8000
    stw r4, 0xc4(r1)
    addi r3, r3, lbl_803A6818@l
    lfd f3, lbl_8047BCB0(r2)
    addi r20, r3, 0x6c
    stw r6, 0xc0(r1)
    mr r3, r20
    lfs f4, lbl_8047BCA4(r2)
    mr r4, r20
    lfd f0, 0xc0(r1)
    addi r5, r1, 0xa0
    stw r0, 0xbc(r1)
    fsubs f1, f0, f3
    lfs f2, lbl_8047BCE0(r2)
    stw r6, 0xb8(r1)
    lfs f5, 0x3c(r23)
    lfd f0, 0xb8(r1)
    fmuls f1, f4, f1
    fsubs f0, f0, f3
    fmuls f1, f1, f2
    fmuls f0, f4, f0
    fmuls f1, f5, f1
    fmuls f0, f0, f2
    stfs f1, 0xa0(r1)
    fmuls f0, f5, f0
    stfs f0, 0xa4(r1)
    bl fn_800E019C
    lfs f0, 0x0(r20)
    lfs f1, lbl_8047BCE4(r2)
    fcmpo cr0, f0, f1
    cror eq, gt, eq
    bne @L_80042CD0
    lfs f0, 0xa0(r1)
    fsubs f0, f1, f0
    stfs f0, 0x0(r20)
    @L_80042CD0:
    lfs f0, 0x0(r20)
    lfs f1, lbl_8047BCE8(r2)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_80042CF0
    lfs f0, 0xa0(r1)
    fsubs f0, f1, f0
    stfs f0, 0x0(r20)
    @L_80042CF0:
    mr r3, r28
    addi r4, r1, 0xac
    bl fn_800E3D98
    lfs f0, lbl_8047BC94(r2)
    mr r3, r28
    addi r4, r1, 0xac
    stfs f0, 0xac(r1)
    bl fn_800E43A4
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 25, 25
    cmplwi r0, 0x0
    beq @L_80042D50
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BCEC(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f1, 0x64(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80042FA8
    lfs f0, 0x3c(r23)
    fadds f0, f1, f0
    stfs f0, 0x64(r3)
    b @L_80042FA8
    @L_80042D50:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_80042FA8
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BCBC(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f1, 0x64(r3)
    fcmpo cr0, f1, f0
    ble @L_80042FA8
    lfs f0, 0x3c(r23)
    fsubs f0, f1, f0
    stfs f0, 0x64(r3)
    b @L_80042FA8
    @L_80042D8C:
    addi r19, r27, 0xc4
    addi r3, r27, 0x7c
    mr r4, r19
    bl fn_800478B4
    lis r4, lbl_803A6818@ha
    cmplwi r19, 0x0
    addi r28, r4, lbl_803A6818@l
    stb r3, 0x214(r28)
    bne @L_80042DB8
    li r0, 0x0
    b @L_80042FA4
    @L_80042DB8:
    lwz r20, 0xe8(r28)
    cmplwi r20, 0x0
    bne @L_80042DD0
    li r0, 0x0
    stb r0, 0x0(r19)
    b @L_80042FA4
    @L_80042DD0:
    addi r3, r1, 0x88
    addi r4, r28, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    lfs f3, 0x200(r4)
    lfs f2, 0x204(r4)
    lfs f1, 0x208(r4)
    lfs f0, 0x20c(r4)
    stfs f3, 0x18(r1)
    lfs f31, 0x210(r4)
    stfs f2, 0x1c(r1)
    stfs f1, 0x20(r1)
    stfs f0, 0x24(r1)
    bl fn_800E3C5C
    addi r4, r1, 0x94
    bl fn_80190E34
    li r3, 0x3
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0x18
    addi r20, r3, lbl_803A6818@l
    addi r5, r1, 0x1c
    lwz r3, 0xfc(r20)
    addi r6, r1, 0x20
    addi r7, r1, 0x24
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0x88
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r20)
    addi r4, r1, 0x88
    bl fn_800D207C
    lwz r3, 0xfc(r20)
    lfs f1, 0x18(r1)
    lfs f2, 0x1c(r1)
    lfs f3, 0x20(r1)
    lfs f4, 0x24(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r20)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x70
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x88
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x64(r1)
    stw r6, 0x68(r1)
    stw r0, 0x6c(r1)
    bl memcpy
    lfs f3, 0x70(r1)
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BC98(r2)
    addi r20, r3, lbl_803A6818@l
    lfs f1, 0x74(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r20)
    fadds f0, f1, f0
    stfs f2, 0x70(r1)
    stfs f0, 0x74(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r20)
    addi r4, r1, 0x64
    bl fn_800DCC84
    lwz r3, 0x108(r20)
    addi r4, r1, 0x70
    bl fn_800DCC60
    lwz r3, 0x108(r20)
    addi r4, r1, 0x7c
    bl fn_800DCC3C
    lwz r3, 0x108(r20)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r20, 0x114(r4)
    cmplwi r20, 0x0
    beq @L_80042F90
    mr r3, r20
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_80042F90:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_80042FA4:
    stb r0, 0x214(r28)
    @L_80042FA8:
    cmpwi r31, 0x0
    beq @L_80043010
    lbz r0, 0x48(r22)
    extsb r0, r0
    cmpwi r0, 0x2
    bge @L_80043028
    lis r3, lbl_803A6818@ha
    lfs f1, 0x3c(r23)
    addi r3, r3, lbl_803A6818@l
    lfs f0, lbl_8047BCBC(r2)
    lfs f2, 0x44(r3)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, 0x44(r3)
    cror eq, gt, eq
    bne @L_80042FF0
    li r0, 0x1
    stb r0, 0x48(r22)
    @L_80042FF0:
    lfs f1, 0x44(r3)
    lfs f0, lbl_8047BCA8(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80043028
    li r0, 0x2
    stb r0, 0x48(r22)
    b @L_80043028
    @L_80043010:
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BC94(r2)
    addi r3, r3, lbl_803A6818@l
    li r0, 0x0
    stfs f0, 0x44(r3)
    stb r0, 0x48(r22)
    @L_80043028:
    cmpwi r29, 0x0
    beq @L_800433AC
    lfs f1, 0x58(r25)
    lfs f0, 0x54(r25)
    fcmpu cr0, f1, f0
    bne @L_800433AC
    lhz r0, 0x6(r21)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_800431CC
    lis r3, lbl_803A6818@ha
    addi r20, r3, lbl_803A6818@l
    lwz r0, 0x28(r20)
    cmpwi r0, 0x0
    ble @L_800431CC
    cmpwi r0, 0x1
    beq @L_800430AC
    bge @L_8004307C
    cmpwi r0, 0x0
    bge @L_80043088
    b @L_800430E0
    @L_8004307C:
    cmpwi r0, 0x3
    bge @L_800430E0
    b @L_800430D0
    @L_80043088:
    li r3, 0x3c
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    b @L_800430E0
    @L_800430AC:
    li r3, 0xa1
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    b @L_800430E0
    @L_800430D0:
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    @L_800430E0:
    lwz r6, 0x28(r20)
    li r3, 0x23
    li r4, 0x0
    li r5, 0x0
    subi r0, r6, 0x1
    stw r0, 0x28(r20)
    bl fn_80166AB8
    lwz r0, 0x28(r20)
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BC94(r2)
    addi r20, r3, lbl_803A6818@l
    lfs f0, lbl_8047BCF0(r2)
    cmpwi r0, 0x1
    stfs f1, 0x6c(r20)
    stfs f1, 0x70(r20)
    stfs f1, 0x74(r20)
    stfs f0, 0x64(r20)
    beq @L_80043170
    bge @L_80043138
    cmpwi r0, 0x0
    bge @L_80043144
    b @L_800431CC
    @L_80043138:
    cmpwi r0, 0x3
    bge @L_800431CC
    b @L_800431A4
    @L_80043144:
    li r3, 0xd
    li r0, 0x2
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_800431CC
    bl fn_800E3BC0
    b @L_800431CC
    @L_80043170:
    li r3, 0xd
    li r0, 0x4
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_80043198
    bl fn_800E3BC0
    @L_80043198:
    lfs f0, lbl_8047BCA8(r2)
    stfs f0, 0x64(r20)
    b @L_800431CC
    @L_800431A4:
    li r3, 0xd
    li r0, 0x5
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_800431CC
    bl fn_800E3BC0
    @L_800431CC:
    lhz r0, 0x6(r21)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800433A4
    lis r3, lbl_803A6818@ha
    addi r29, r3, lbl_803A6818@l
    lwz r0, 0x28(r29)
    cmpwi r0, 0x2
    bge @L_800433A4
    lwz r0, 0x0(r29)
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r0, r3, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80043214
    li r0, 0x0
    b @L_80043218
    @L_80043214:
    li r0, 0x1
    @L_80043218:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_800433A4
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lwz r0, 0x20(r3)
    cmpwi r0, 0x1
    beq @L_800433A4
    lwz r0, 0x28(r29)
    cmpwi r0, 0x1
    beq @L_80043284
    bge @L_80043254
    cmpwi r0, 0x0
    bge @L_80043260
    b @L_800432B8
    @L_80043254:
    cmpwi r0, 0x3
    bge @L_800432B8
    b @L_800432A8
    @L_80043260:
    li r3, 0x3c
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    b @L_800432B8
    @L_80043284:
    li r3, 0xa1
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    b @L_800432B8
    @L_800432A8:
    li r3, 0x96
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    @L_800432B8:
    lwz r6, 0x28(r29)
    li r3, 0x23
    li r4, 0x0
    li r5, 0x0
    addi r0, r6, 0x1
    stw r0, 0x28(r29)
    bl fn_80166AB8
    lwz r0, 0x28(r29)
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BC94(r2)
    addi r20, r3, lbl_803A6818@l
    lfs f0, lbl_8047BCF0(r2)
    cmpwi r0, 0x1
    stfs f1, 0x6c(r20)
    stfs f1, 0x70(r20)
    stfs f1, 0x74(r20)
    stfs f0, 0x64(r20)
    beq @L_80043348
    bge @L_80043310
    cmpwi r0, 0x0
    bge @L_8004331C
    b @L_800433A4
    @L_80043310:
    cmpwi r0, 0x3
    bge @L_800433A4
    b @L_8004337C
    @L_8004331C:
    li r3, 0xd
    li r0, 0x2
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_800433A4
    bl fn_800E3BC0
    b @L_800433A4
    @L_80043348:
    li r3, 0xd
    li r0, 0x4
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_80043370
    bl fn_800E3BC0
    @L_80043370:
    lfs f0, lbl_8047BCA8(r2)
    stfs f0, 0x64(r20)
    b @L_800433A4
    @L_8004337C:
    li r3, 0xd
    li r0, 0x5
    stw r3, 0x1c(r27)
    lwz r3, 0xa0(r20)
    stw r0, 0x24(r20)
    stfs f1, 0x60(r26)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_800433A4
    bl fn_800E3BC0
    @L_800433A4:
    lfs f0, 0x38(r24)
    stfs f0, 0x30(r24)
    @L_800433AC:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lwz r4, 0x0(r3)
    cmpw r30, r4
    beq @L_80043708
    lwz r0, lbl_804788C0(r13)
    cmpwi r0, 0x0
    beq @L_80043708
    lwz r19, lbl_8047A4E0(r13)
    cmplwi r19, 0x0
    beq @L_80043450
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r4, 1
    lhzx r20, r3, r0
    cmplwi r20, 0x8000
    blt @L_800433F0
    clrlwi r20, r20, 18
    @L_800433F0:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r19
    clrlwi r4, r20, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r20
    li r3, 0x0
    bl fn_8025FDDC
    mr r21, r3
    mr r4, r20
    li r3, 0x0
    bl fn_8025FD34
    mr r20, r3
    mr r3, r19
    mr r4, r21
    bl fn_8011DFE0
    mr r3, r19
    mr r4, r20
    bl fn_8011DF90
    lwz r20, lbl_8047A4E0(r13)
    b @L_80043454
    @L_80043450:
    li r20, 0x0
    @L_80043454:
    lis r3, lbl_803A6818@ha
    mr r4, r20
    addi r3, r3, lbl_803A6818@l
    addi r19, r3, 0x7c
    mr r3, r19
    bl fn_8010A210
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8004348C
    mr r3, r19
    bl fn_80109BFC
    mr r3, r19
    mr r4, r20
    bl fn_80109C88
    @L_8004348C:
    lwz r0, 0x1c(r27)
    lis r3, lbl_803A6818@ha
    addi r29, r3, lbl_803A6818@l
    li r3, 0x0
    cmpwi r0, 0x5
    stb r3, 0x214(r29)
    beq @L_800434EC
    bge @L_800434C0
    cmpwi r0, 0x4
    bge @L_80043708
    cmpwi r0, 0x0
    bge @L_800434DC
    b @L_80043708
    @L_800434C0:
    cmpwi r0, 0xb
    bge @L_800434D4
    cmpwi r0, 0x9
    bge @L_80043708
    b @L_800434DC
    @L_800434D4:
    cmpwi r0, 0xd
    bge @L_80043708
    @L_800434DC:
    mr r3, r19
    bl fn_80047CC0
    stb r3, 0x214(r29)
    b @L_80043708
    @L_800434EC:
    addi r21, r29, 0xc4
    mr r3, r19
    mr r4, r21
    bl fn_800478B4
    cmplwi r21, 0x0
    stb r3, 0x214(r29)
    bne @L_80043510
    li r0, 0x0
    b @L_80043704
    @L_80043510:
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r20, 0xe8(r4)
    cmplwi r20, 0x0
    bne @L_80043530
    li r0, 0x0
    stb r0, 0x0(r21)
    b @L_80043704
    @L_80043530:
    addi r3, r1, 0x4c
    addi r4, r4, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    lfs f3, 0x200(r4)
    lfs f2, 0x204(r4)
    lfs f1, 0x208(r4)
    lfs f0, 0x20c(r4)
    stfs f3, 0x8(r1)
    lfs f31, 0x210(r4)
    stfs f2, 0xc(r1)
    stfs f1, 0x10(r1)
    stfs f0, 0x14(r1)
    bl fn_800E3C5C
    addi r4, r1, 0x58
    bl fn_80190E34
    li r3, 0x3
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0x8
    addi r20, r3, lbl_803A6818@l
    addi r5, r1, 0xc
    lwz r3, 0xfc(r20)
    addi r6, r1, 0x10
    addi r7, r1, 0x14
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0x4c
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r20)
    addi r4, r1, 0x4c
    bl fn_800D207C
    lwz r3, 0xfc(r20)
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    lfs f4, 0x14(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r20)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x34
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x4c
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x28(r1)
    stw r6, 0x2c(r1)
    stw r0, 0x30(r1)
    bl memcpy
    lfs f3, 0x34(r1)
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BC98(r2)
    addi r20, r3, lbl_803A6818@l
    lfs f1, 0x38(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r20)
    fadds f0, f1, f0
    stfs f2, 0x34(r1)
    stfs f0, 0x38(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r20)
    addi r4, r1, 0x28
    bl fn_800DCC84
    lwz r3, 0x108(r20)
    addi r4, r1, 0x34
    bl fn_800DCC60
    lwz r3, 0x108(r20)
    addi r4, r1, 0x40
    bl fn_800DCC3C
    lwz r3, 0x108(r20)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r20, 0x114(r4)
    cmplwi r20, 0x0
    beq @L_800436F0
    mr r3, r20
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r20
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_800436F0:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_80043704:
    stb r0, 0x214(r29)
    @L_80043708:
    li r3, 0x0
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    lmw r19, 0xcc(r1)
    lwz r0, 0x114(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
