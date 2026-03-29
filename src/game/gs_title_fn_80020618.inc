    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    bl fn_80105624
    mr r30, r3
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047B878(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    lis r4, lbl_803A1FC8@ha
    stw r3, 0x14(r1)
    addi r29, r4, lbl_803A1FC8@l
    lfd f3, lbl_8047B880(r2)
    stw r0, 0x10(r1)
    lfs f1, 0x24(r29)
    lfd f2, 0x10(r1)
    lfs f0, lbl_8047B868(r2)
    fsubs f2, f2, f3
    fdivs f2, f2, f31
    fadds f1, f1, f2
    stfs f2, 0x1c(r29)
    fcmpo cr0, f1, f0
    stfs f1, 0x24(r29)
    cror eq, gt, eq
    bne @L_800206B0
    lfs f0, lbl_8047B86C(r2)
    stfs f0, 0x24(r29)
    @L_800206B0:
    li r3, 0x0
    li r4, 0x9
    bl fn_80135168
    cmpwi r3, 0x1
    bne @L_80020708
    lis r3, lbl_803A1FC8@ha
    lfs f3, lbl_8047B86C(r2)
    addi r3, r3, lbl_803A1FC8@l
    lfs f2, 0x20(r3)
    fcmpu cr0, f3, f2
    beq @L_80020780
    lfs f1, 0x1c(r29)
    lfs f0, lbl_8047B868(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, 0x20(r3)
    cror eq, gt, eq
    bne @L_80020780
    li r0, 0x0
    stfs f3, 0x20(r3)
    stw r0, 0x8(r3)
    b @L_80020780
    @L_80020708:
    lis r3, lbl_803A1FC8@ha
    lfs f1, 0x1c(r29)
    addi r5, r3, lbl_803A1FC8@l
    lfs f0, lbl_8047B868(r2)
    lfs f2, 0x20(r5)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, 0x20(r5)
    cror eq, gt, eq
    bne @L_80020780
    lwz r3, 0x8(r5)
    lfs f0, lbl_8047B86C(r2)
    addi r4, r3, 0x1
    srwi r3, r4, 31
    stfs f0, 0x20(r5)
    clrlwi r0, r4, 31
    xor r0, r0, r3
    stw r4, 0x8(r5)
    subf r0, r3, r0
    cmpwi r0, 0x0
    bne @L_80020780
    lwz r0, 0xc(r5)
    xori r0, r0, 0x1
    cmpwi r0, 0x0
    stw r0, 0xc(r5)
    beq @L_80020778
    stfs f0, 0x18(r5)
    b @L_80020780
    @L_80020778:
    lfs f0, lbl_8047B870(r2)
    stfs f0, 0x18(r5)
    @L_80020780:
    lis r3, lbl_803A1FC8@ha
    lfs f2, lbl_8047B874(r2)
    addi r29, r3, lbl_803A1FC8@l
    lfs f1, 0x20(r29)
    lfs f0, 0x28(r29)
    fmuls f1, f2, f1
    fmuls f1, f1, f0
    bl fn_800CE148
    lis r3, lbl_803A1FC8@ha
    frsp f1, f1
    addi r31, r3, lbl_803A1FC8@l
    lfs f0, 0x18(r31)
    fmuls f0, f0, f1
    stfs f0, 0x10(r31)
    lhz r3, 0x6(r30)
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800207E0
    lwz r3, 0x0(r31)
    cmpwi r3, 0x2
    bge @L_800208F4
    addi r0, r3, 0x1
    stw r0, 0x0(r31)
    b @L_800208F4
    @L_800207E0:
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_80020804
    lwz r3, 0x0(r31)
    cmpwi r3, 0x0
    ble @L_800208F4
    subi r0, r3, 0x1
    stw r0, 0x0(r31)
    b @L_800208F4
    @L_80020804:
    bl fn_80105624
    lis r4, lbl_803A1FC8@ha
    mr r30, r3
    addi r3, r4, lbl_803A1FC8@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    beq @L_80020864
    bge @L_800208F4
    cmpwi r0, 0x0
    bge @L_80020830
    b @L_800208F4
    @L_80020830:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80020848
    li r3, 0x1
    bl fn_80166CC0
    @L_80020848:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800208F4
    li r3, 0x0
    bl fn_80166CC0
    b @L_800208F4
    @L_80020864:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_800208CC
    li r3, 0x0
    li r4, 0x9
    bl fn_80135168
    cmpwi r3, 0x0
    beq @L_800208A0
    li r3, 0x1
    li r4, 0x0
    li r5, 0xff
    li r6, 0x1e
    li r7, 0x0
    bl fn_800F78A4
    @L_800208A0:
    li r3, 0x0
    li r4, 0x9
    li r5, 0x0
    bl fn_80135030
    lfs f0, lbl_8047B868(r2)
    lis r3, lbl_803A1FC8@ha
    addi r3, r3, lbl_803A1FC8@l
    li r0, 0x1
    stw r0, 0x8(r3)
    stfs f0, 0x20(r29)
    stw r0, 0xc(r3)
    @L_800208CC:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800208F4
    li r3, 0x0
    li r4, 0x9
    li r5, 0x1
    bl fn_80135030
    lfs f0, lbl_8047B86C(r2)
    stfs f0, 0x18(r31)
    @L_800208F4:
    li r3, 0x0
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
