    nofralloc
    stwu r1, -0x90(r1)
    lbz r0, 0x1(r3)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80039370
    bge @L_8003914C
    cmpwi r0, 0x0
    beq @L_80039158
    b @L_8003948C
    @L_8003914C:
    cmpwi r0, 0x4
    bge @L_8003948C
    b @L_80039474
    @L_80039158:
    lbz r0, 0x2(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003948C
    lis r4, lbl_80267060@ha
    lis r0, 0x4330
    addi r5, r4, lbl_80267060@l
    lis r4, lbl_803A65B0@ha
    lwz r6, 0x4(r5)
    addi r4, r4, lbl_803A65B0@l
    lwz r8, 0x8(r5)
    lwz r7, 0x1c(r5)
    xoris r12, r6, 0x8000
    lwz r6, 0x20(r5)
    xoris r11, r8, 0x8000
    lwz r8, 0x34(r5)
    xoris r10, r7, 0x8000
    lwz r7, 0x38(r5)
    xoris r9, r6, 0x8000
    lwz r6, 0x4c(r5)
    xoris r8, r8, 0x8000
    xoris r7, r7, 0x8000
    stw r12, 0xc(r1)
    lfs f8, lbl_8047BAA8(r2)
    xoris r6, r6, 0x8000
    stw r0, 0x8(r1)
    lfd f0, lbl_8047BAA0(r2)
    lfd f1, 0x8(r1)
    stw r11, 0x14(r1)
    fsubs f7, f1, f0
    stw r0, 0x10(r1)
    lfd f1, 0x10(r1)
    stw r10, 0x1c(r1)
    fsubs f6, f1, f0
    stw r0, 0x18(r1)
    lfd f1, 0x18(r1)
    stw r9, 0x24(r1)
    fsubs f5, f1, f0
    stw r0, 0x20(r1)
    lfd f1, 0x20(r1)
    stw r8, 0x2c(r1)
    fsubs f4, f1, f0
    stw r0, 0x28(r1)
    lfd f1, 0x28(r1)
    stw r7, 0x34(r1)
    fsubs f3, f1, f0
    stw r0, 0x30(r1)
    lfd f1, 0x30(r1)
    stw r6, 0x3c(r1)
    fsubs f2, f1, f0
    stw r0, 0x38(r1)
    lfd f1, 0x38(r1)
    stfs f7, 0x0(r4)
    fsubs f1, f1, f0
    stfs f6, 0x4(r4)
    stfs f8, 0x8(r4)
    stfs f5, 0xc(r4)
    stfs f4, 0x10(r4)
    stfs f8, 0x14(r4)
    stfs f3, 0x18(r4)
    stfs f2, 0x1c(r4)
    stfs f8, 0x20(r4)
    stfs f1, 0x24(r4)
    lwz r6, 0x50(r5)
    lwz r8, 0x64(r5)
    lwz r7, 0x68(r5)
    xoris r12, r6, 0x8000
    lwz r6, 0x7c(r5)
    xoris r11, r8, 0x8000
    lwz r8, 0x80(r5)
    xoris r10, r7, 0x8000
    lwz r7, 0x94(r5)
    xoris r9, r6, 0x8000
    lwz r6, 0x98(r5)
    xoris r8, r8, 0x8000
    xoris r7, r7, 0x8000
    stw r12, 0x44(r1)
    xoris r6, r6, 0x8000
    stw r0, 0x40(r1)
    lfd f1, 0x40(r1)
    stw r11, 0x4c(r1)
    fsubs f7, f1, f0
    stw r0, 0x48(r1)
    lfd f1, 0x48(r1)
    stw r10, 0x54(r1)
    fsubs f6, f1, f0
    stw r0, 0x50(r1)
    lfd f1, 0x50(r1)
    stw r9, 0x5c(r1)
    fsubs f5, f1, f0
    stw r0, 0x58(r1)
    lfd f1, 0x58(r1)
    stw r8, 0x64(r1)
    fsubs f4, f1, f0
    stw r0, 0x60(r1)
    lfd f1, 0x60(r1)
    stw r7, 0x6c(r1)
    fsubs f3, f1, f0
    stw r0, 0x68(r1)
    lfd f1, 0x68(r1)
    stw r6, 0x74(r1)
    fsubs f2, f1, f0
    stw r0, 0x70(r1)
    lfd f1, 0x70(r1)
    stfs f7, 0x28(r4)
    fsubs f1, f1, f0
    stfs f8, 0x2c(r4)
    stfs f6, 0x30(r4)
    stfs f5, 0x34(r4)
    stfs f8, 0x38(r4)
    stfs f4, 0x3c(r4)
    stfs f3, 0x40(r4)
    stfs f8, 0x44(r4)
    stfs f2, 0x48(r4)
    stfs f1, 0x4c(r4)
    lwz r7, 0xac(r5)
    li r6, 0x1
    lwz r5, 0xb0(r5)
    xoris r7, r7, 0x8000
    stw r0, 0x78(r1)
    xoris r5, r5, 0x8000
    stw r7, 0x7c(r1)
    lfd f1, 0x78(r1)
    stw r5, 0x84(r1)
    fsubs f2, f1, f0
    stw r0, 0x80(r1)
    lfd f1, 0x80(r1)
    stfs f8, 0x50(r4)
    fsubs f0, f1, f0
    stfs f2, 0x54(r4)
    stfs f0, 0x58(r4)
    stfs f8, 0x5c(r4)
    stb r6, 0x2(r3)
    b @L_8003948C
    @L_80039370:
    lis r4, lbl_80267060@ha
    lis r3, lbl_803A65B0@ha
    addi r6, r4, lbl_80267060@l
    li r5, 0x0
    addi r7, r3, lbl_803A65B0@l
    li r0, 0x4
    lfs f5, lbl_8047BA90(r2)
    lfd f3, lbl_8047BAA0(r2)
    lis r3, 0x4330
    lfs f2, lbl_8047BA94(r2)
    lfs f0, lbl_8047BA98(r2)
    mtctr r0
    @L_800393A0:
    lwz r0, 0xc(r6)
    lfs f1, 0x0(r7)
    xoris r4, r0, 0x8000
    stw r3, 0x80(r1)
    fadds f4, f1, f5
    stw r4, 0x84(r1)
    lfd f1, 0x80(r1)
    stfs f4, 0x0(r7)
    fsubs f1, f1, f3
    fcmpo cr0, f4, f1
    ble @L_800393E4
    lis r0, 0x4330
    stw r4, 0x84(r1)
    stw r0, 0x80(r1)
    lfd f1, 0x80(r1)
    fsubs f1, f1, f3
    stfs f1, 0x0(r7)
    @L_800393E4:
    lfs f1, 0x8(r7)
    fadds f1, f1, f2
    fcmpo cr0, f1, f0
    stfs f1, 0x8(r7)
    ble @L_800393FC
    stfs f0, 0x8(r7)
    @L_800393FC:
    addi r6, r6, 0x18
    addi r7, r7, 0xc
    lwz r0, 0xc(r6)
    lfs f1, 0x0(r7)
    xoris r4, r0, 0x8000
    stw r3, 0x80(r1)
    fadds f4, f1, f5
    stw r4, 0x84(r1)
    lfd f1, 0x80(r1)
    stfs f4, 0x0(r7)
    fsubs f1, f1, f3
    fcmpo cr0, f4, f1
    ble @L_80039448
    lis r0, 0x4330
    stw r4, 0x84(r1)
    stw r0, 0x80(r1)
    lfd f1, 0x80(r1)
    fsubs f1, f1, f3
    stfs f1, 0x0(r7)
    @L_80039448:
    lfs f1, 0x8(r7)
    fadds f1, f1, f2
    fcmpo cr0, f1, f0
    stfs f1, 0x8(r7)
    ble @L_80039460
    stfs f0, 0x8(r7)
    @L_80039460:
    addi r6, r6, 0x18
    addi r7, r7, 0xc
    addi r5, r5, 0x1
    bdnz @L_800393A0
    b @L_8003948C
    @L_80039474:
    lbz r0, 0x2(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003948C
    li r0, 0x1
    stb r0, 0x2(r3)
    @L_8003948C:
    li r3, 0x0
    addi r1, r1, 0x90
    blr
