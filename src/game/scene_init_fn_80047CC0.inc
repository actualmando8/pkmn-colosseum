    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stfd f31, 0xf0(r1)
    psq_st f31, 0xf8(r1), 0, 0
    stfd f30, 0xe0(r1)
    psq_st f30, 0xe8(r1), 0, 0
    stfd f29, 0xd0(r1)
    psq_st f29, 0xd8(r1), 0, 0
    stw r31, 0xcc(r1)
    stw r30, 0xc8(r1)
    stw r29, 0xc4(r1)
    stw r28, 0xc0(r1)
    mr r30, r3
    lwz r28, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BD38(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f0, lbl_8047BCBC(r2)
    cmplwi r28, 0x0
    stfs f1, 0x18(r1)
    lfs f29, lbl_8047BCC0(r2)
    stfs f0, 0x14(r1)
    lwz r0, 0x0(r3)
    beq @L_80047D9C
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r31, r3, r0
    cmplwi r31, 0x8000
    blt @L_80047D3C
    clrlwi r31, r31, 18
    @L_80047D3C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r28
    clrlwi r4, r31, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r31
    li r3, 0x0
    bl fn_8025FDDC
    mr r29, r3
    mr r4, r31
    li r3, 0x0
    bl fn_8025FD34
    mr r31, r3
    mr r3, r28
    mr r4, r29
    bl fn_8011DFE0
    mr r3, r28
    mr r4, r31
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80047DA0
    @L_80047D9C:
    li r3, 0x0
    @L_80047DA0:
    bl fn_8011F5C8
    addi r4, r1, 0x14
    addi r5, r1, 0x18
    bl fn_8025FA20
    lfs f0, 0x14(r1)
    cmplwi r30, 0x0
    fmuls f29, f29, f0
    bne @L_80047DC8
    li r3, 0x0
    b @L_8004846C
    @L_80047DC8:
    lwz r0, 0x34(r30)
    cmplwi r0, 0x0
    bne @L_80047DDC
    li r3, 0x0
    b @L_8004846C
    @L_80047DDC:
    mr r3, r30
    li r4, 0x0
    bl fn_80109B90
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80047DFC
    li r3, 0x0
    b @L_8004846C
    @L_80047DFC:
    lbz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_80047E90
    lwz r3, 0x24(r30)
    bl fn_801DAC3C
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80047E24
    li r3, 0x0
    b @L_8004846C
    @L_80047E24:
    lwz r3, 0x24(r30)
    bl fn_801DAC24
    cmpwi r3, 0x1
    beq @L_80047E78
    bge @L_80047E50
    cmpwi r3, -0x1
    beq @L_80047E68
    bge @L_80047E70
    cmpwi r3, -0x2
    bge @L_80047E60
    b @L_80047E94
    @L_80047E50:
    cmpwi r3, 0x3
    beq @L_80047E88
    bge @L_80047E94
    b @L_80047E80
    @L_80047E60:
    lfs f30, lbl_8047BD3C(r2)
    b @L_80047E94
    @L_80047E68:
    lfs f30, lbl_8047BD40(r2)
    b @L_80047E94
    @L_80047E70:
    lfs f30, lbl_8047BD44(r2)
    b @L_80047E94
    @L_80047E78:
    lfs f30, lbl_8047BD44(r2)
    b @L_80047E94
    @L_80047E80:
    lfs f30, lbl_8047BD48(r2)
    b @L_80047E94
    @L_80047E88:
    lfs f30, lbl_8047BD4C(r2)
    b @L_80047E94
    @L_80047E90:
    lwz r31, 0x24(r30)
    @L_80047E94:
    li r3, 0x3
    bl fn_80177A44
    lwz r3, 0x38(r30)
    addi r4, r1, 0x28
    addi r5, r1, 0x24
    addi r6, r1, 0x20
    addi r7, r1, 0x1c
    bl fn_800D1FDC
    mr r3, r31
    bl fn_800E3C5C
    addi r4, r1, 0x74
    bl fn_80190E34
    lwz r4, 0x2c(r30)
    lis r5, 0x4330
    lwz r0, 0x30(r30)
    mr r3, r31
    xoris r4, r4, 0x8000
    lfs f0, lbl_8047BD30(r2)
    xoris r0, r0, 0x8000
    stw r4, 0xb4(r1)
    lfd f3, lbl_8047BCB0(r2)
    addi r4, r1, 0x44
    stw r5, 0xb0(r1)
    lfd f1, 0xb0(r1)
    stw r0, 0xbc(r1)
    fsubs f2, f1, f3
    stw r5, 0xb8(r1)
    lfd f1, 0xb8(r1)
    stfs f0, 0x28(r1)
    fsubs f0, f1, f3
    fdivs f0, f2, f0
    stfs f0, 0x24(r1)
    bl fn_800E3D98
    lfs f0, 0x78(r1)
    lfs f1, 0x74(r1)
    fmuls f2, f0, f0
    lfs f0, lbl_8047BC94(r2)
    fmuls f1, f1, f1
    fadds f31, f2, f1
    fcmpo cr0, f31, f0
    ble @L_80047F80
    frsqrte f1, f31
    lfd f3, lbl_8047BD50(r2)
    lfd f2, lbl_8047BD58(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f31, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f31, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f31, f0, f2
    fmul f0, f1, f0
    fmul f31, f31, f0
    frsp f31, f31
    b @L_80048014
    @L_80047F80:
    lfd f0, lbl_8047BD60(r2)
    fcmpo cr0, f31, f0
    bge @L_80047F9C
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f31, 0x0(r3)
    b @L_80048014
    @L_80047F9C:
    stfs f31, 0x10(r1)
    lis r0, 0x7f80
    lwz r4, 0x10(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_80047FC4
    bge @L_80047FFC
    cmpwi r3, 0x0
    beq @L_80047FE0
    b @L_80047FFC
    @L_80047FC4:
    clrlwi r0, r4, 9
    cmpwi r0, 0x0
    beq @L_80047FD8
    li r0, 0x1
    b @L_80048000
    @L_80047FD8:
    li r0, 0x2
    b @L_80048000
    @L_80047FE0:
    clrlwi r0, r4, 9
    cmpwi r0, 0x0
    beq @L_80047FF4
    li r0, 0x5
    b @L_80048000
    @L_80047FF4:
    li r0, 0x3
    b @L_80048000
    @L_80047FFC:
    li r0, 0x4
    @L_80048000:
    cmpwi r0, 0x1
    bne @L_80048014
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f31, 0x0(r3)
    @L_80048014:
    lfs f2, lbl_8047BD68(r2)
    lfs f1, 0x28(r1)
    lfs f0, lbl_8047BD18(r2)
    fmuls f1, f2, f1
    fmuls f1, f1, f0
    bl fn_800CE220
    fdivs f3, f31, f30
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r3, 0x38(r30)
    lfs f0, 0x68(r4)
    lfs f2, 0x24(r1)
    frsp f6, f1
    lfs f1, 0x28(r1)
    fmuls f5, f29, f3
    lfs f3, 0x20(r1)
    lfs f4, 0x1c(r1)
    fdivs f30, f5, f6
    fmuls f30, f30, f0
    bl fn_800D20CC
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BC94(r2)
    addi r4, r3, lbl_803A6818@l
    lfs f2, 0x228(r4)
    lfs f0, 0x21c(r4)
    fsubs f0, f2, f0
    fmuls f0, f0, f0
    fcmpo cr0, f0, f1
    ble @L_800480D0
    frsqrte f2, f0
    lfd f4, lbl_8047BD50(r2)
    lfd f3, lbl_8047BD58(r2)
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f0, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f0, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f0, f1, f3
    fmul f1, f2, f1
    fmul f0, f0, f1
    frsp f0, f0
    b @L_80048164
    @L_800480D0:
    lfd f1, lbl_8047BD60(r2)
    fcmpo cr0, f0, f1
    bge @L_800480EC
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f0, 0x0(r3)
    b @L_80048164
    @L_800480EC:
    stfs f0, 0xc(r1)
    lis r0, 0x7f80
    lwz r5, 0xc(r1)
    rlwinm r3, r5, 0, 1, 8
    cmpw r3, r0
    beq @L_80048114
    bge @L_8004814C
    cmpwi r3, 0x0
    beq @L_80048130
    b @L_8004814C
    @L_80048114:
    clrlwi r0, r5, 9
    cmpwi r0, 0x0
    beq @L_80048128
    li r0, 0x1
    b @L_80048150
    @L_80048128:
    li r0, 0x2
    b @L_80048150
    @L_80048130:
    clrlwi r0, r5, 9
    cmpwi r0, 0x0
    beq @L_80048144
    li r0, 0x5
    b @L_80048150
    @L_80048144:
    li r0, 0x3
    b @L_80048150
    @L_8004814C:
    li r0, 0x4
    @L_80048150:
    cmpwi r0, 0x1
    bne @L_80048164
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f0, 0x0(r3)
    @L_80048164:
    lfs f4, 0x224(r4)
    lfs f1, 0x218(r4)
    lfs f3, 0x22c(r4)
    lfs f2, 0x220(r4)
    fsubs f4, f4, f1
    lfs f1, lbl_8047BC94(r2)
    fsubs f2, f3, f2
    fmuls f3, f4, f4
    fmuls f2, f2, f2
    fadds f5, f3, f2
    fcmpo cr0, f5, f1
    ble @L_800481DC
    frsqrte f2, f5
    lfd f4, lbl_8047BD50(r2)
    lfd f3, lbl_8047BD58(r2)
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f1, f2, f1
    fmul f5, f5, f1
    frsp f5, f5
    b @L_80048270
    @L_800481DC:
    lfd f1, lbl_8047BD60(r2)
    fcmpo cr0, f5, f1
    bge @L_800481F8
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f5, 0x0(r3)
    b @L_80048270
    @L_800481F8:
    stfs f5, 0x8(r1)
    lis r0, 0x7f80
    lwz r4, 0x8(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_80048220
    bge @L_80048258
    cmpwi r3, 0x0
    beq @L_8004823C
    b @L_80048258
    @L_80048220:
    clrlwi r0, r4, 9
    cmpwi r0, 0x0
    beq @L_80048234
    li r0, 0x1
    b @L_8004825C
    @L_80048234:
    li r0, 0x2
    b @L_8004825C
    @L_8004823C:
    clrlwi r0, r4, 9
    cmpwi r0, 0x0
    beq @L_80048250
    li r0, 0x5
    b @L_8004825C
    @L_80048250:
    li r0, 0x3
    b @L_8004825C
    @L_80048258:
    li r0, 0x4
    @L_8004825C:
    cmpwi r0, 0x1
    bne @L_80048270
    lis r3, lbl_80478AC0@ha
    addi r3, r3, lbl_80478AC0@l
    lfs f5, 0x0(r3)
    @L_80048270:
    fdivs f1, f0, f5
    bl fn_800CD85C
    frsp f29, f1
    mr r3, r31
    addi r4, r1, 0x44
    bl fn_800E3D98
    lfs f0, lbl_8047BC94(r2)
    addi r3, r1, 0x80
    stfs f0, 0x68(r1)
    stfs f0, 0x70(r1)
    stfs f0, 0x6c(r1)
    bl fn_800E064C
    addi r3, r1, 0x80
    addi r4, r1, 0x68
    bl fn_800E03B4
    lfs f1, 0x18(r1)
    addi r3, r1, 0x80
    bl fn_800E032C
    mr r3, r31
    addi r4, r1, 0x80
    bl fn_800E4598
    mr r3, r31
    bl fn_800E6BC8
    mr r3, r31
    bl fn_800EE0E8
    mr r4, r3
    mr r3, r31
    subi r4, r4, 0x1
    bl fn_800EE150
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80048308
    addi r4, r1, 0x50
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    @L_80048308:
    mr r3, r31
    bl fn_800E6B20
    fmr f1, f29
    bl fn_800CE148
    frsp f2, f1
    lfs f1, lbl_8047BC94(r2)
    lfs f0, 0x54(r1)
    fmr f3, f30
    addi r3, r1, 0x5c
    fmuls f2, f30, f2
    fadds f0, f1, f0
    fadds f2, f2, f0
    bl fn_800E01F4
    lwz r3, 0x38(r30)
    addi r4, r1, 0x5c
    bl fn_800D207C
    lis r3, lbl_802E5418@ha
    lfs f2, lbl_8047BC94(r2)
    addi r5, r3, lbl_802E5418@l
    lfs f1, 0x54(r1)
    lfs f0, 0x58(r1)
    lis r3, lbl_802E540C@ha
    stfs f2, 0x0(r5)
    addi r4, r3, lbl_802E540C@l
    stfs f1, 0x4(r5)
    stfs f0, 0x8(r5)
    lwz r3, 0x38(r30)
    bl fn_800D1F04
    lis r4, lbl_80267180@ha
    addi r3, r1, 0x38
    addi r8, r4, lbl_80267180@l
    addi r4, r1, 0x5c
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x2c(r1)
    stw r6, 0x30(r1)
    stw r0, 0x34(r1)
    bl memcpy
    lfs f3, 0x38(r1)
    li r4, 0x2
    lfs f2, lbl_8047BC98(r2)
    lfs f1, 0x3c(r1)
    lfs f0, lbl_8047BC9C(r2)
    fsubs f2, f3, f2
    fadds f0, f1, f0
    stfs f2, 0x38(r1)
    stfs f0, 0x3c(r1)
    lwz r3, 0x44(r30)
    bl fn_800DCCF0
    lwz r3, 0x44(r30)
    addi r4, r1, 0x2c
    bl fn_800DCC84
    lwz r3, 0x44(r30)
    addi r4, r1, 0x38
    bl fn_800DCC60
    lwz r3, 0x44(r30)
    addi r4, r1, 0x68
    bl fn_800DCC3C
    lwz r3, 0x44(r30)
    li r4, 0x1
    bl fn_800DCC34
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lwz r30, 0x114(r4)
    cmplwi r30, 0x0
    beq @L_80048458
    mr r3, r30
    addi r4, r4, 0x118
    bl fn_800D207C
    lis r4, lbl_803A6818@ha
    mr r3, r30
    addi r4, r4, lbl_803A6818@l
    addi r4, r4, 0x124
    bl fn_800D203C
    lis r4, lbl_803A6818@ha
    mr r3, r30
    addi r4, r4, lbl_803A6818@l
    lfs f1, 0x13c(r4)
    lfs f2, 0x140(r4)
    lfs f3, 0x144(r4)
    lfs f4, 0x148(r4)
    bl fn_800D20CC
    @L_80048458:
    li r3, 0x3
    bl fn_80177A44
    li r3, 0x4
    bl fn_80177A44
    li r3, 0x1
    @L_8004846C:
    psq_l f31, 0xf8(r1), 0, 0
    lfd f31, 0xf0(r1)
    psq_l f30, 0xe8(r1), 0, 0
    lfd f30, 0xe0(r1)
    psq_l f29, 0xd8(r1), 0, 0
    lfd f29, 0xd0(r1)
    lwz r31, 0xcc(r1)
    lwz r30, 0xc8(r1)
    lwz r29, 0xc4(r1)
    lwz r0, 0x104(r1)
    lwz r28, 0xc0(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
