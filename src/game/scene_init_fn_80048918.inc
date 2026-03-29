    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    stw r0, 0x124(r1)
    stfd f31, 0x110(r1)
    psq_st f31, 0x118(r1), 0, 0
    stmw r21, 0xe4(r1)
    lis r4, lbl_80267150@ha
    li r3, 0x0
    addi r31, r4, lbl_80267150@l
    bl fn_801096F8
    lis r3, lbl_803A6818@ha
    li r0, 0x0
    addi r28, r3, lbl_803A6818@l
    li r3, 0x0
    stw r0, 0x110(r28)
    li r4, 0x0
    stw r0, 0x114(r28)
    stw r0, 0x10c(r28)
    bl fn_801CB954
    bl fn_80113F48
    li r4, 0x0
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80048980
    li r4, 0x0
    bl fn_800E4014
    @L_80048980:
    li r3, 0x0
    bl fn_801CB9D8
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BC94(r2)
    addi r23, r3, lbl_803A6818@l
    lfs f0, lbl_8047BCF0(r2)
    lis r3, 0xd17
    stfs f1, 0x6c(r23)
    addi r3, r3, 0x1000
    stfs f1, 0x70(r23)
    stfs f1, 0x74(r23)
    stfs f0, 0x64(r23)
    bl fn_801CBA0C
    stw r3, 0x10c(r28)
    mr r22, r3
    bl fn_80113F48
    mr r4, r22
    bl fn_800F9318
    stw r3, 0x110(r28)
    lfs f1, lbl_8047BD18(r2)
    bl fn_800EC9DC
    lwz r9, 0x0(r31)
    lis r3, 0xd73
    lwz r8, 0x4(r31)
    addi r4, r3, 0x1800
    lwz r7, 0x8(r31)
    li r3, 0x17
    lwz r6, 0xc(r31)
    lwz r5, 0x10(r31)
    lwz r0, 0x14(r31)
    stw r9, 0xb8(r1)
    stw r8, 0xbc(r1)
    stw r7, 0xc0(r1)
    stw r6, 0xc4(r1)
    stw r5, 0xc8(r1)
    stw r0, 0xcc(r1)
    bl fn_800F9318
    mr r24, r3
    addi r4, r1, 0xc4
    stw r24, 0x114(r28)
    bl fn_800D1F84
    mr r3, r24
    addi r4, r1, 0xd0
    bl fn_800D1F58
    lfs f0, 0x100(r24)
    mr r3, r24
    addi r4, r1, 0x2c
    addi r5, r1, 0x30
    stfs f0, 0xb8(r1)
    addi r6, r1, 0x34
    addi r7, r1, 0x38
    lfs f0, 0x104(r24)
    stfs f0, 0xbc(r1)
    lfs f0, 0x108(r24)
    stfs f0, 0xc0(r1)
    bl fn_800D1FDC
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0xc4
    addi r3, r3, lbl_803A6818@l
    addi r22, r3, 0x218
    mr r3, r22
    bl fn_800E01D0
    addi r3, r22, 0xc
    addi r4, r1, 0xb8
    bl fn_800E01D0
    lfs f1, 0x2c(r1)
    mr r3, r24
    lfs f2, 0x30(r1)
    lfs f3, 0x34(r1)
    lfs f4, 0x38(r1)
    stfs f1, 0x18(r22)
    stfs f2, 0x1c(r22)
    stfs f3, 0x20(r22)
    stfs f4, 0x24(r22)
    bl fn_800D20CC
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0xc4
    addi r3, r3, lbl_803A6818@l
    addi r25, r3, 0x118
    mr r3, r25
    bl fn_800E01D0
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0xd0
    addi r3, r3, lbl_803A6818@l
    addi r26, r3, 0x124
    mr r3, r26
    bl fn_800E01D0
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0xb8
    addi r3, r3, lbl_803A6818@l
    addi r3, r3, 0x130
    bl fn_800E01D0
    lis r3, lbl_803A6818@ha
    lfs f3, 0x2c(r1)
    addi r30, r3, lbl_803A6818@l
    lfs f2, 0x30(r1)
    lfs f1, 0x34(r1)
    lis r3, 0xd73
    lfs f0, 0x38(r1)
    addi r4, r3, 0x1800
    stfs f3, 0x13c(r30)
    li r3, 0x17
    li r5, 0x0
    li r6, 0x1
    stfs f2, 0x140(r30)
    stfs f1, 0x144(r30)
    stfs f0, 0x148(r30)
    bl fn_80176E0C
    li r3, 0x4
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    li r0, 0x0
    addi r3, r3, lbl_803A6818@l
    li r4, 0x280
    addi r27, r3, 0x7c
    stw r0, 0x78(r3)
    mr r3, r27
    li r5, 0x1e0
    bl fn_8010A5BC
    lis r3, lbl_803A6818@ha
    li r4, 0x280
    addi r3, r3, lbl_803A6818@l
    li r5, 0x1e0
    addi r29, r3, 0xc4
    mr r3, r29
    bl fn_8010A5BC
    li r0, 0x0
    li r3, 0x140
    stw r0, lbl_8047A4E0(r13)
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80048BA0
    bl fn_800E27B0
    b @L_80048BA4
    @L_80048BA0:
    li r3, 0x0
    @L_80048BA4:
    stw r3, lbl_8047A4E0(r13)
    li r3, 0x0
    bl fn_8025FEE4
    clrlwi r0, r3, 16
    li r3, 0x400
    stw r0, lbl_804788C0(r13)
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80048BD8
    bl fn_800E27B0
    b @L_80048BDC
    @L_80048BD8:
    li r3, 0x0
    @L_80048BDC:
    lfs f1, lbl_8047BCF0(r2)
    lis r4, lbl_803A6818@ha
    lfs f2, lbl_8047BC94(r2)
    addi r24, r4, lbl_803A6818@l
    lfs f0, lbl_8047BCBC(r2)
    li r4, 0x0
    stfs f1, 0x64(r23)
    li r0, 0x5
    stw r3, lbl_8047A4E4(r13)
    stfs f2, 0x2c(r24)
    stfs f2, 0x30(r24)
    stfs f2, 0x34(r24)
    stfs f2, 0x38(r24)
    stw r4, 0x0(r24)
    stw r4, 0x28(r24)
    stw r4, 0x14(r24)
    stw r4, 0x4(r24)
    stw r4, 0x20(r24)
    stfs f2, 0x44(r24)
    stb r4, 0x48(r24)
    stfs f1, 0x68(r24)
    stfs f2, 0x6c(r23)
    stfs f2, 0x70(r23)
    stfs f2, 0x74(r23)
    stfs f1, 0x64(r23)
    stw r4, 0x1c(r24)
    stfs f2, 0x4c(r24)
    stfs f0, 0x50(r24)
    stfs f2, 0x58(r24)
    stfs f0, 0x54(r24)
    stfs f0, 0x5c(r24)
    stfs f0, 0x60(r24)
    stb r0, lbl_804788C4(r13)
    stb r4, 0x158(r24)
    stb r4, 0x159(r24)
    stb r4, 0x15b(r24)
    stb r4, 0x15a(r24)
    lfs f0, lbl_8047BCCC(r2)
    stb r4, 0x15c(r24)
    stb r4, 0x15d(r24)
    stb r4, 0x15f(r24)
    stb r4, 0x15e(r24)
    stw r4, 0x154(r24)
    stfs f0, 0x1e0(r24)
    stfs f0, 0x1dc(r24)
    stfs f2, 0x1cc(r24)
    stfs f2, 0x1d0(r24)
    stfs f2, 0x1d4(r24)
    stfs f2, 0x1d8(r24)
    stb r4, 0x164(r24)
    stb r4, 0x178(r24)
    stb r4, 0x18c(r24)
    stb r4, 0x1a0(r24)
    stb r4, 0x1b4(r24)
    stw r4, 0x1e4(r24)
    bl fn_8003D4C8
    lwz r0, lbl_804788C0(r13)
    cmpwi r0, 0x0
    beq @L_80048FE0
    lwz r23, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r23, 0x0
    lwz r0, 0x0(r3)
    beq @L_80048D58
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r22, r3, r0
    cmplwi r22, 0x8000
    blt @L_80048CF8
    clrlwi r22, r22, 18
    @L_80048CF8:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r23
    clrlwi r4, r22, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r22
    li r3, 0x0
    bl fn_8025FDDC
    mr r21, r3
    mr r4, r22
    li r3, 0x0
    bl fn_8025FD34
    mr r22, r3
    mr r3, r23
    mr r4, r21
    bl fn_8011DFE0
    mr r3, r23
    mr r4, r22
    bl fn_8011DF90
    lwz r22, lbl_8047A4E0(r13)
    b @L_80048D5C
    @L_80048D58:
    li r22, 0x0
    @L_80048D5C:
    mr r3, r27
    mr r4, r22
    bl fn_8010A210
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80048D88
    mr r3, r27
    bl fn_80109BFC
    mr r3, r27
    mr r4, r22
    bl fn_80109C88
    @L_80048D88:
    lwz r0, 0x1c(r24)
    lis r3, lbl_803A6818@ha
    addi r23, r3, lbl_803A6818@l
    li r3, 0x0
    cmpwi r0, 0x5
    stb r3, 0x214(r23)
    beq @L_80048DE8
    bge @L_80048DBC
    cmpwi r0, 0x4
    bge @L_80048FE0
    cmpwi r0, 0x0
    bge @L_80048DD8
    b @L_80048FE0
    @L_80048DBC:
    cmpwi r0, 0xb
    bge @L_80048DD0
    cmpwi r0, 0x9
    bge @L_80048FE0
    b @L_80048DD8
    @L_80048DD0:
    cmpwi r0, 0xd
    bge @L_80048FE0
    @L_80048DD8:
    mr r3, r27
    bl fn_80047CC0
    stb r3, 0x214(r23)
    b @L_80048FE0
    @L_80048DE8:
    mr r3, r27
    mr r4, r29
    bl fn_800478B4
    cmplwi r29, 0x0
    stb r3, 0x214(r23)
    bne @L_80048E08
    li r0, 0x0
    b @L_80048FDC
    @L_80048E08:
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r21, 0xe8(r4)
    cmplwi r21, 0x0
    bne @L_80048E28
    li r0, 0x0
    stb r0, 0x0(r29)
    b @L_80048FDC
    @L_80048E28:
    addi r3, r1, 0xa0
    addi r4, r4, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r21
    addi r4, r4, lbl_803A6818@l
    lfs f3, 0x200(r4)
    lfs f2, 0x204(r4)
    lfs f1, 0x208(r4)
    lfs f0, 0x20c(r4)
    stfs f3, 0x1c(r1)
    lfs f31, 0x210(r4)
    stfs f2, 0x20(r1)
    stfs f1, 0x24(r1)
    stfs f0, 0x28(r1)
    bl fn_800E3C5C
    addi r4, r1, 0xac
    bl fn_80190E34
    li r3, 0x3
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0x1c
    addi r22, r3, lbl_803A6818@l
    addi r5, r1, 0x20
    lwz r3, 0xfc(r22)
    addi r6, r1, 0x24
    addi r7, r1, 0x28
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0xa0
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r22)
    addi r4, r1, 0xa0
    bl fn_800D207C
    lwz r3, 0xfc(r22)
    lfs f1, 0x1c(r1)
    lfs f2, 0x20(r1)
    lfs f3, 0x24(r1)
    lfs f4, 0x28(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r22)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lwz r7, 0x30(r31)
    addi r3, r1, 0x64
    lwz r6, 0x34(r31)
    addi r4, r1, 0xa0
    lwz r0, 0x38(r31)
    li r5, 0xc
    stw r7, 0x58(r1)
    stw r6, 0x5c(r1)
    stw r0, 0x60(r1)
    bl memcpy
    lfs f3, 0x64(r1)
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BC98(r2)
    addi r22, r3, lbl_803A6818@l
    lfs f1, 0x68(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r22)
    fadds f0, f1, f0
    stfs f2, 0x64(r1)
    stfs f0, 0x68(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r22)
    addi r4, r1, 0x58
    bl fn_800DCC84
    lwz r3, 0x108(r22)
    addi r4, r1, 0x64
    bl fn_800DCC60
    lwz r3, 0x108(r22)
    addi r4, r1, 0x94
    bl fn_800DCC3C
    lwz r3, 0x108(r22)
    li r4, 0x1
    bl fn_800DCC34
    lwz r21, 0x114(r28)
    cmplwi r21, 0x0
    beq @L_80048FC8
    mr r3, r21
    mr r4, r25
    bl fn_800D207C
    mr r3, r21
    mr r4, r26
    bl fn_800D203C
    lfs f1, 0x13c(r30)
    mr r3, r21
    lfs f2, 0x140(r30)
    lfs f3, 0x144(r30)
    lfs f4, 0x148(r30)
    bl fn_800D20CC
    @L_80048FC8:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_80048FDC:
    stb r0, 0x214(r23)
    @L_80048FE0:
    lis r4, 0xf7
    mr r3, r29
    addi r4, r4, 0x400
    bl fn_8010A010
    lwz r0, 0x1c(r24)
    cmpwi r0, 0x5
    beq @L_80049048
    bge @L_80049014
    cmpwi r0, 0x4
    bge @L_80049240
    cmpwi r0, 0x0
    bge @L_80049030
    b @L_80049240
    @L_80049014:
    cmpwi r0, 0xb
    bge @L_80049028
    cmpwi r0, 0x9
    bge @L_80049240
    b @L_80049030
    @L_80049028:
    cmpwi r0, 0xd
    bge @L_80049240
    @L_80049030:
    mr r3, r27
    bl fn_80047CC0
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    stb r3, 0x214(r4)
    b @L_80049240
    @L_80049048:
    mr r3, r27
    mr r4, r29
    bl fn_800478B4
    lis r4, lbl_803A6818@ha
    cmplwi r29, 0x0
    addi r24, r4, lbl_803A6818@l
    stb r3, 0x214(r24)
    bne @L_80049070
    li r0, 0x0
    b @L_8004923C
    @L_80049070:
    lwz r21, 0xe8(r24)
    cmplwi r21, 0x0
    bne @L_80049088
    li r0, 0x0
    stb r0, 0x0(r29)
    b @L_8004923C
    @L_80049088:
    addi r3, r1, 0x7c
    addi r4, r24, 0x1e8
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    lis r3, lbl_802E5448@ha
    addi r4, r4, lbl_803A6818@l
    addi r3, r3, lbl_802E5448@l
    addi r4, r4, 0x1f4
    bl fn_800E01D0
    lis r4, lbl_803A6818@ha
    mr r3, r21
    addi r4, r4, lbl_803A6818@l
    lfs f3, 0x200(r4)
    lfs f2, 0x204(r4)
    lfs f1, 0x208(r4)
    lfs f0, 0x20c(r4)
    stfs f3, 0xc(r1)
    lfs f31, 0x210(r4)
    stfs f2, 0x10(r1)
    stfs f1, 0x14(r1)
    stfs f0, 0x18(r1)
    bl fn_800E3C5C
    addi r4, r1, 0x88
    bl fn_80190E34
    li r3, 0x3
    bl fn_80177A44
    lis r3, lbl_803A6818@ha
    addi r4, r1, 0xc
    addi r23, r3, lbl_803A6818@l
    addi r5, r1, 0x10
    lwz r3, 0xfc(r23)
    addi r6, r1, 0x14
    addi r7, r1, 0x18
    bl fn_800D1FDC
    lfs f1, lbl_8047BC94(r2)
    fmr f3, f31
    addi r3, r1, 0x7c
    fmr f2, f1
    bl fn_800E01F4
    lwz r3, 0xfc(r23)
    addi r4, r1, 0x7c
    bl fn_800D207C
    lwz r3, 0xfc(r23)
    lfs f1, 0xc(r1)
    lfs f2, 0x10(r1)
    lfs f3, 0x14(r1)
    lfs f4, 0x18(r1)
    bl fn_800D20CC
    lis r3, lbl_802E543C@ha
    lis r5, lbl_802E5448@ha
    addi r4, r3, lbl_802E543C@l
    lwz r3, 0xfc(r23)
    addi r5, r5, lbl_802E5448@l
    bl fn_800D1F04
    lwz r7, 0x30(r31)
    addi r3, r1, 0x4c
    lwz r6, 0x34(r31)
    addi r4, r1, 0x7c
    lwz r0, 0x38(r31)
    li r5, 0xc
    stw r7, 0x40(r1)
    stw r6, 0x44(r1)
    stw r0, 0x48(r1)
    bl memcpy
    lfs f3, 0x4c(r1)
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BC98(r2)
    addi r23, r3, lbl_803A6818@l
    lfs f1, 0x50(r1)
    li r4, 0x2
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    lwz r3, 0x108(r23)
    fadds f0, f1, f0
    stfs f2, 0x4c(r1)
    stfs f0, 0x50(r1)
    bl fn_800DCCF0
    lwz r3, 0x108(r23)
    addi r4, r1, 0x40
    bl fn_800DCC84
    lwz r3, 0x108(r23)
    addi r4, r1, 0x4c
    bl fn_800DCC60
    lwz r3, 0x108(r23)
    addi r4, r1, 0x70
    bl fn_800DCC3C
    lwz r3, 0x108(r23)
    li r4, 0x1
    bl fn_800DCC34
    lwz r21, 0x114(r28)
    cmplwi r21, 0x0
    beq @L_80049228
    mr r3, r21
    mr r4, r25
    bl fn_800D207C
    mr r3, r21
    mr r4, r26
    bl fn_800D203C
    lfs f1, 0x13c(r30)
    mr r3, r21
    lfs f2, 0x140(r30)
    lfs f3, 0x144(r30)
    lfs f4, 0x148(r30)
    bl fn_800D20CC
    @L_80049228:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r0, 0x1
    @L_8004923C:
    stb r0, 0x214(r24)
    @L_80049240:
    lis r3, 0xf7
    addi r3, r3, 0x400
    bl fn_8018F6F4
    addi r5, r1, 0x3c
    addi r6, r1, 0x8
    li r4, 0x1
    bl fn_8018F4C8
    lwz r4, 0x3c(r1)
    mr r3, r29
    bl fn_80109894
    lhz r4, lbl_8047A4E8(r13)
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BC94(r2)
    addi r3, r3, lbl_803A6818@l
    li r0, -0x5
    cmplwi r4, 0x5
    stfs f0, 0x40(r3)
    stw r0, 0x8(r3)
    blt @L_80049298
    li r0, 0x5
    stw r0, 0xc(r3)
    b @L_8004929C
    @L_80049298:
    stw r4, 0xc(r3)
    @L_8004929C:
    lis r3, lbl_803A6818@ha
    li r0, 0x0
    addi r3, r3, lbl_803A6818@l
    stw r4, 0x10(r3)
    stw r0, 0x14c(r3)
    psq_l f31, 0x118(r1), 0, 0
    lfd f31, 0x110(r1)
    lmw r21, 0xe4(r1)
    lwz r0, 0x124(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
