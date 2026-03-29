    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stmw r25, 0xb4(r1)
    mr r31, r3
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lfs f0, 0x50(r4)
    addi r28, r4, 0x154
    lfs f2, 0x4c(r4)
    fcmpu cr0, f0, f2
    beq @L_80040388
    lfs f1, 0x3c(r4)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_80040370
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    ble @L_80040388
    stfs f0, 0x4c(r4)
    b @L_80040388
    @L_80040370:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    bge @L_80040388
    stfs f0, 0x4c(r4)
    @L_80040388:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x58(r3)
    lfs f2, 0x54(r3)
    fcmpu cr0, f0, f2
    beq @L_800403E8
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_800403D0
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r3)
    ble @L_800403E8
    stfs f0, 0x54(r3)
    b @L_800403E8
    @L_800403D0:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r3)
    bge @L_800403E8
    stfs f0, 0x54(r3)
    @L_800403E8:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x60(r3)
    lfs f2, 0x5c(r3)
    fcmpu cr0, f0, f2
    beq @L_80040448
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCC0(r2)
    fdivs f0, f1, f0
    ble @L_80040430
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r3)
    ble @L_80040448
    stfs f0, 0x5c(r3)
    b @L_80040448
    @L_80040430:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r3)
    bge @L_80040448
    stfs f0, 0x5c(r3)
    @L_80040448:
    lfs f0, 0x88(r28)
    lfs f1, 0x8c(r28)
    fcmpu cr0, f0, f1
    beq @L_80040508
    fsubs f2, f1, f0
    lfs f1, lbl_8047BCC4(r2)
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BC98(r2)
    addi r3, r3, lbl_803A6818@l
    fmuls f2, f1, f2
    lfs f1, 0x3c(r3)
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80040484
    fmr f4, f0
    @L_80040484:
    lfs f0, lbl_8047BCC8(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_80040498
    fmr f4, f0
    @L_80040498:
    lfs f1, 0x88(r28)
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f1, f4
    lfs f3, 0x8c(r28)
    fcmpo cr0, f4, f0
    fsubs f2, f3, f1
    stfs f1, 0x88(r28)
    ble @L_800404BC
    b @L_800404C0
    @L_800404BC:
    fneg f4, f4
    @L_800404C0:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_800404D4
    fmr f0, f2
    b @L_800404D8
    @L_800404D4:
    fneg f0, f2
    @L_800404D8:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80040504
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_800404F4
    b @L_800404F8
    @L_800404F4:
    fneg f2, f2
    @L_800404F8:
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f2, f0
    bge @L_80040508
    @L_80040504:
    stfs f3, 0x88(r28)
    @L_80040508:
    lfs f1, 0x50(r4)
    lfs f0, 0x4c(r4)
    fcmpu cr0, f1, f0
    bne @L_800410F8
    cmplwi r31, 0x0
    beq @L_800410F8
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    bl fn_80105624
    lhz r29, 0x4(r3)
    rlwinm r0, r29, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80040C9C
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lbz r0, 0x9(r28)
    extsb r4, r0
    cmpwi r4, 0x4
    bne @L_80040970
    lwz r0, 0x90(r28)
    cmpwi r0, 0x1
    beq @L_80040600
    bge @L_80040578
    cmpwi r0, 0x0
    bge @L_80040584
    b @L_80040C9C
    @L_80040578:
    cmpwi r0, 0x3
    bge @L_80040C9C
    b @L_80040964
    @L_80040584:
    bl fn_8003D4C8
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_800405F4
    lhz r6, lbl_8047A4E8(r13)
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BC94(r2)
    addi r3, r3, lbl_803A6818@l
    li r5, 0x1
    li r4, 0x0
    li r0, -0x5
    cmplwi r6, 0x5
    stw r5, 0x90(r28)
    stw r4, 0x0(r3)
    stfs f0, 0x2c(r3)
    stfs f0, 0x30(r3)
    stfs f0, 0x34(r3)
    stfs f0, 0x38(r3)
    stw r0, 0x8(r3)
    blt @L_800405E0
    li r0, 0x5
    stw r0, 0xc(r3)
    b @L_800405E4
    @L_800405E0:
    stw r6, 0xc(r3)
    @L_800405E4:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    stw r6, 0x10(r3)
    b @L_80040C9C
    @L_800405F4:
    li r0, 0x2
    stw r0, 0x90(r28)
    b @L_80040C9C
    @L_80040600:
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    lwz r0, lbl_804788C0(r13)
    cmpwi r0, 0x0
    beq @L_80040C9C
    lwz r26, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r26, 0x0
    lwz r0, 0x0(r3)
    beq @L_800406A8
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    cmplwi r27, 0x8000
    blt @L_80040648
    clrlwi r27, r27, 18
    @L_80040648:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r30, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r30
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r27, lbl_8047A4E0(r13)
    b @L_800406AC
    @L_800406A8:
    li r27, 0x0
    @L_800406AC:
    lis r3, lbl_803A6818@ha
    mr r4, r27
    addi r3, r3, lbl_803A6818@l
    addi r26, r3, 0x7c
    mr r3, r26
    bl fn_8010A210
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800406E4
    mr r3, r26
    bl fn_80109BFC
    mr r3, r26
    mr r4, r27
    bl fn_80109C88
    @L_800406E4:
    lis r3, lbl_803A6818@ha
    li r4, 0x0
    addi r30, r3, lbl_803A6818@l
    lwz r0, 0x1c(r30)
    stb r4, 0x214(r30)
    cmpwi r0, 0x5
    beq @L_80040744
    bge @L_80040718
    cmpwi r0, 0x4
    bge @L_80040C9C
    cmpwi r0, 0x0
    bge @L_80040734
    b @L_80040C9C
    @L_80040718:
    cmpwi r0, 0xb
    bge @L_8004072C
    cmpwi r0, 0x9
    bge @L_80040C9C
    b @L_80040734
    @L_8004072C:
    cmpwi r0, 0xd
    bge @L_80040C9C
    @L_80040734:
    mr r3, r26
    bl fn_80047CC0
    stb r3, 0x214(r30)
    b @L_80040C9C
    @L_80040744:
    addi r25, r30, 0xc4
    mr r3, r26
    mr r4, r25
    bl fn_800478B4
    cmplwi r25, 0x0
    stb r3, 0x214(r30)
    bne @L_80040768
    li r0, 0x0
    b @L_8004095C
    @L_80040768:
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r27, 0xe8(r4)
    cmplwi r27, 0x0
    bne @L_80040788
    li r0, 0x0
    stb r0, 0x0(r25)
    b @L_8004095C
    @L_80040788:
    addi r3, r1, 0x88
    addi r4, r4, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r27
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
    addi r27, r3, lbl_803A6818@l
    addi r5, r1, 0x1c
    lwz r3, 0xfc(r27)
    addi r6, r1, 0x20
    addi r7, r1, 0x24
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0x88
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r27)
    addi r4, r1, 0x88
    bl fn_800D207C
    lwz r3, 0xfc(r27)
    lfs f1, 0x18(r1)
    lfs f2, 0x1c(r1)
    lfs f3, 0x20(r1)
    lfs f4, 0x24(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r27)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x4c
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x88
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x40(r1)
    stw r6, 0x44(r1)
    stw r0, 0x48(r1)
    bl memcpy
    lfs f3, 0x4c(r1)
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BC98(r2)
    addi r27, r3, lbl_803A6818@l
    lfs f1, 0x50(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r27)
    fadds f0, f1, f0
    stfs f2, 0x4c(r1)
    stfs f0, 0x50(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r27)
    addi r4, r1, 0x40
    bl fn_800DCC84
    lwz r3, 0x108(r27)
    addi r4, r1, 0x4c
    bl fn_800DCC60
    lwz r3, 0x108(r27)
    addi r4, r1, 0x7c
    bl fn_800DCC3C
    lwz r3, 0x108(r27)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r27, 0x114(r4)
    cmplwi r27, 0x0
    beq @L_80040948
    mr r3, r27
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r27
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r27
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_80040948:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_8004095C:
    stb r0, 0x214(r30)
    b @L_80040C9C
    @L_80040964:
    li r0, 0x0
    stw r0, 0x90(r28)
    b @L_80040C9C
    @L_80040970:
    lwz r0, 0x0(r28)
    cmpwi r0, 0x1
    beq @L_80040B68
    bge @L_80040C9C
    cmpwi r0, 0x0
    bge @L_8004098C
    b @L_80040C9C
    @L_8004098C:
    lfs f1, lbl_8047BCCC(r2)
    lfs f0, 0x88(r28)
    fcmpu cr0, f1, f0
    bne @L_80040C9C
    la r3, lbl_804788D4(r13)
    lfs f0, lbl_8047BC94(r2)
    lbzx r0, r3, r4
    li r3, 0x1
    cmpwi r4, 0x2
    stw r3, 0x0(r28)
    extsb r0, r0
    stfs f0, 0x8c(r28)
    stb r0, 0xc(r28)
    beq @L_800409FC
    bge @L_800409D8
    cmpwi r4, 0x0
    beq @L_800409E4
    bge @L_800409F0
    b @L_80040A1C
    @L_800409D8:
    cmpwi r4, 0x4
    bge @L_80040A1C
    b @L_80040A14
    @L_800409E4:
    lbz r0, 0x4(r28)
    stb r0, 0xb(r28)
    b @L_80040A1C
    @L_800409F0:
    lbz r0, 0x5(r28)
    stb r0, 0xb(r28)
    b @L_80040A1C
    @L_800409FC:
    lbz r0, 0xa(r28)
    extsb r0, r0
    add r3, r28, r0
    lbz r0, 0x6(r3)
    stb r0, 0xb(r28)
    b @L_80040A1C
    @L_80040A14:
    lbz r0, 0x8(r28)
    stb r0, 0xb(r28)
    @L_80040A1C:
    lbz r3, 0xc(r28)
    extsb r0, r3
    cmpwi r0, 0xa
    blt @L_80040A90
    lbz r3, 0xb(r28)
    subi r0, r3, 0x9
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_80040A48
    li r0, 0x0
    stb r0, 0x74(r28)
    @L_80040A48:
    lbz r3, 0x74(r28)
    lis r0, 0x4330
    li r4, 0xa
    stw r0, 0xa0(r1)
    extsb r0, r3
    addi r3, r3, 0xa
    mulli r0, r0, -0x1d
    lfd f1, lbl_8047BCB0(r2)
    stb r4, 0x75(r28)
    extsb r3, r3
    xoris r0, r0, 0x8000
    stb r3, 0x75(r28)
    stw r0, 0xa4(r1)
    lfd f0, 0xa0(r1)
    fsubs f0, f0, f1
    stfs f0, 0x7c(r28)
    stfs f0, 0x84(r28)
    b @L_80040AA8
    @L_80040A90:
    lfs f0, lbl_8047BC94(r2)
    li r0, 0x0
    stb r0, 0x74(r28)
    stb r3, 0x75(r28)
    stfs f0, 0x7c(r28)
    stfs f0, 0x84(r28)
    @L_80040AA8:
    lbz r0, 0x9(r28)
    li r3, 0x0
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80040AE8
    bge @L_80040AD0
    cmpwi r0, 0x0
    beq @L_80040B0C
    bge @L_80040AE0
    b @L_80040B0C
    @L_80040AD0:
    cmpwi r0, 0x4
    bge @L_80040B0C
    b @L_80040B08
    b @L_80040B0C
    @L_80040AE0:
    li r3, 0x1
    b @L_80040B0C
    @L_80040AE8:
    lbz r0, 0xa(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80040B00
    li r3, 0x2
    b @L_80040B0C
    @L_80040B00:
    li r3, 0x3
    b @L_80040B0C
    @L_80040B08:
    li r3, 0x4
    @L_80040B0C:
    mulli r3, r3, 0x14
    addi r3, r3, 0x10
    add r3, r28, r3
    lbz r0, 0x0(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_80040B58
    lbz r0, 0x1(r3)
    stb r0, 0x74(r28)
    lbz r0, 0x2(r3)
    stb r0, 0x75(r28)
    lfs f0, 0x4(r3)
    stfs f0, 0x78(r28)
    lfs f0, 0x8(r3)
    stfs f0, 0x7c(r28)
    lfs f0, 0xc(r3)
    stfs f0, 0x80(r28)
    lfs f0, 0x10(r3)
    stfs f0, 0x84(r28)
    @L_80040B58:
    lwz r3, 0x4(r31)
    li r4, 0x2
    bl fn_80103484
    b @L_80040C9C
    @L_80040B68:
    lfs f1, lbl_8047BC94(r2)
    lfs f0, 0x88(r28)
    fcmpu cr0, f1, f0
    bne @L_80040C9C
    cmpwi r4, 0x2
    beq @L_80040BB8
    bge @L_80040B94
    cmpwi r4, 0x0
    beq @L_80040BA0
    bge @L_80040BAC
    b @L_80040BD8
    @L_80040B94:
    cmpwi r4, 0x4
    bge @L_80040BD8
    b @L_80040BD0
    @L_80040BA0:
    lbz r0, 0xb(r28)
    stb r0, 0x4(r28)
    b @L_80040BD8
    @L_80040BAC:
    lbz r0, 0xb(r28)
    stb r0, 0x5(r28)
    b @L_80040BD8
    @L_80040BB8:
    lbz r0, 0xa(r28)
    lbz r4, 0xb(r28)
    extsb r0, r0
    add r3, r28, r0
    stb r4, 0x6(r3)
    b @L_80040BD8
    @L_80040BD0:
    lbz r0, 0xb(r28)
    stb r0, 0x8(r28)
    @L_80040BD8:
    lbz r0, 0x9(r28)
    li r3, 0x0
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80040C18
    bge @L_80040C00
    cmpwi r0, 0x0
    beq @L_80040C3C
    bge @L_80040C10
    b @L_80040C3C
    @L_80040C00:
    cmpwi r0, 0x4
    bge @L_80040C3C
    b @L_80040C38
    b @L_80040C3C
    @L_80040C10:
    li r3, 0x1
    b @L_80040C3C
    @L_80040C18:
    lbz r0, 0xa(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80040C30
    li r3, 0x2
    b @L_80040C3C
    @L_80040C30:
    li r3, 0x3
    b @L_80040C3C
    @L_80040C38:
    li r3, 0x4
    @L_80040C3C:
    mulli r5, r3, 0x14
    li r3, 0x1
    lfs f0, lbl_8047BCCC(r2)
    li r0, 0x0
    li r4, 0x2
    addi r5, r5, 0x10
    add r5, r28, r5
    stb r3, 0x0(r5)
    lbz r3, 0x74(r28)
    stb r3, 0x1(r5)
    lbz r3, 0x75(r28)
    stb r3, 0x2(r5)
    lfs f1, 0x78(r28)
    stfs f1, 0x4(r5)
    lfs f1, 0x7c(r28)
    stfs f1, 0x8(r5)
    lfs f1, 0x80(r28)
    stfs f1, 0xc(r5)
    lfs f1, 0x84(r28)
    stfs f1, 0x10(r5)
    stw r0, 0x0(r28)
    stfs f0, 0x8c(r28)
    lwz r3, 0x4(r31)
    bl fn_80103484
    @L_80040C9C:
    rlwinm r0, r29, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800410F8
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lwz r0, 0x0(r28)
    cmpwi r0, 0x1
    beq @L_800410CC
    bge @L_800410F8
    cmpwi r0, 0x0
    bge @L_80040CD4
    b @L_800410F8
    @L_80040CD4:
    lfs f1, lbl_8047BCCC(r2)
    lfs f0, 0x88(r28)
    fcmpu cr0, f1, f0
    bne @L_800410F8
    lwz r0, 0x90(r28)
    cmpwi r0, 0x1
    beq @L_800410F8
    bl fn_8003D818
    lhz r5, lbl_8047A4E8(r13)
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BC94(r2)
    addi r3, r3, lbl_803A6818@l
    li r4, 0x0
    li r0, -0x5
    cmplwi r5, 0x5
    stw r4, 0x0(r3)
    stfs f0, 0x2c(r3)
    stfs f0, 0x30(r3)
    stfs f0, 0x34(r3)
    stfs f0, 0x38(r3)
    stw r0, 0x8(r3)
    blt @L_80040D38
    li r0, 0x5
    stw r0, 0xc(r3)
    b @L_80040D3C
    @L_80040D38:
    stw r5, 0xc(r3)
    @L_80040D3C:
    lwz r0, lbl_804788C0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmpwi r0, 0x0
    stw r5, 0x10(r3)
    beq @L_80041094
    lwz r25, lbl_8047A4E0(r13)
    lwz r0, 0x0(r3)
    cmplwi r25, 0x0
    beq @L_80040DDC
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    cmplwi r27, 0x8000
    blt @L_80040D7C
    clrlwi r27, r27, 18
    @L_80040D7C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r28, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r25
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r27
    bl fn_8011DF90
    lwz r27, lbl_8047A4E0(r13)
    b @L_80040DE0
    @L_80040DDC:
    li r27, 0x0
    @L_80040DE0:
    lis r3, lbl_803A6818@ha
    mr r4, r27
    addi r3, r3, lbl_803A6818@l
    addi r25, r3, 0x7c
    mr r3, r25
    bl fn_8010A210
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80040E18
    mr r3, r25
    bl fn_80109BFC
    mr r3, r25
    mr r4, r27
    bl fn_80109C88
    @L_80040E18:
    lis r3, lbl_803A6818@ha
    li r4, 0x0
    addi r30, r3, lbl_803A6818@l
    lwz r0, 0x1c(r30)
    stb r4, 0x214(r30)
    cmpwi r0, 0x5
    beq @L_80040E78
    bge @L_80040E4C
    cmpwi r0, 0x4
    bge @L_80041094
    cmpwi r0, 0x0
    bge @L_80040E68
    b @L_80041094
    @L_80040E4C:
    cmpwi r0, 0xb
    bge @L_80040E60
    cmpwi r0, 0x9
    bge @L_80041094
    b @L_80040E68
    @L_80040E60:
    cmpwi r0, 0xd
    bge @L_80041094
    @L_80040E68:
    mr r3, r25
    bl fn_80047CC0
    stb r3, 0x214(r30)
    b @L_80041094
    @L_80040E78:
    addi r26, r30, 0xc4
    mr r3, r25
    mr r4, r26
    bl fn_800478B4
    cmplwi r26, 0x0
    stb r3, 0x214(r30)
    bne @L_80040E9C
    li r0, 0x0
    b @L_80041090
    @L_80040E9C:
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r27, 0xe8(r4)
    cmplwi r27, 0x0
    bne @L_80040EBC
    li r0, 0x0
    stb r0, 0x0(r26)
    b @L_80041090
    @L_80040EBC:
    addi r3, r1, 0x64
    addi r4, r4, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r27
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
    addi r4, r1, 0x70
    bl fn_80190E34
    li r3, 0x3
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0x8
    addi r28, r3, lbl_803A6818@l
    addi r5, r1, 0xc
    lwz r3, 0xfc(r28)
    addi r6, r1, 0x10
    addi r7, r1, 0x14
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0x64
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r28)
    addi r4, r1, 0x64
    bl fn_800D207C
    lwz r3, 0xfc(r28)
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    lfs f4, 0x14(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r28)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x34
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x64
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
    addi r28, r3, lbl_803A6818@l
    lfs f1, 0x38(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r28)
    fadds f0, f1, f0
    stfs f2, 0x34(r1)
    stfs f0, 0x38(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r28)
    addi r4, r1, 0x28
    bl fn_800DCC84
    lwz r3, 0x108(r28)
    addi r4, r1, 0x34
    bl fn_800DCC60
    lwz r3, 0x108(r28)
    addi r4, r1, 0x58
    bl fn_800DCC3C
    lwz r3, 0x108(r28)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r27, 0x114(r4)
    cmplwi r27, 0x0
    beq @L_8004107C
    mr r3, r27
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r27
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r27
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_8004107C:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_80041090:
    stb r0, 0x214(r30)
    @L_80041094:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    addi r3, r3, 0x7c
    bl fn_80047CC0
    lis r4, lbl_803A6818@ha
    li r0, 0x1
    addi r5, r4, lbl_803A6818@l
    li r4, 0x3
    stb r3, 0x214(r5)
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    lwz r3, 0x4(r31)
    bl fn_80103484
    b @L_800410F8
    @L_800410CC:
    lfs f1, lbl_8047BC94(r2)
    lfs f0, 0x88(r28)
    fcmpu cr0, f1, f0
    bne @L_800410F8
    lfs f0, lbl_8047BCCC(r2)
    li r0, 0x0
    stw r0, 0x0(r28)
    li r4, 0x2
    stfs f0, 0x8c(r28)
    lwz r3, 0x4(r31)
    bl fn_80103484
    @L_800410F8:
    psq_l f31, 0xd8(r1), 0, 0
    lfd f31, 0xd0(r1)
    lmw r25, 0xb4(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
