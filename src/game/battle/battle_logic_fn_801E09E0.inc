    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stfd f31, 0xe0(r1)
    psq_st f31, 0xe8(r1), 0, 0
    stfd f30, 0xd0(r1)
    psq_st f30, 0xd8(r1), 0, 0
    stfd f29, 0xc0(r1)
    psq_st f29, 0xc8(r1), 0, 0
    stfd f28, 0xb0(r1)
    psq_st f28, 0xb8(r1), 0, 0
    stfd f27, 0xa0(r1)
    psq_st f27, 0xa8(r1), 0, 0
    stmw r24, 0x80(r1)
    lis r3, -32728
    lis r4, -32713
    addi r31, r3, -26112
    li r29, 0x0
    lwz r7, 0x34(r31)
    addi r30, r4, 20680
    lwz r6, 0x38(r31)
    li r25, 0x0
    lwz r5, 0x3c(r31)
    li r28, 0x1
    lwz r4, 0x40(r31)
    lwz r3, 0x44(r31)
    lwz r0, 0x48(r31)
    stw r7, 0x14(r1)
    stw r6, 0x18(r1)
    stw r5, 0x1c(r1)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_800FF56C
    mr r27, r3
@L_801E0A6C:
    cmpwi r25, 0xa
    beq @L_801E0DC0
    bge @L_801E0A9C
    cmpwi r25, 0x1
    beq @L_801E0B9C
    bge @L_801E0A90
    cmpwi r25, 0x0
    bge @L_801E0AA8
    b @L_801E0F34
@L_801E0A90:
    cmpwi r25, 0x3
    bge @L_801E0F34
    b @L_801E0EAC
@L_801E0A9C:
    cmpwi r25, 0x64
    beq @L_801E0F0C
    b @L_801E0F34
@L_801E0AA8:
    addi r5, r1, 0x20
    li r3, 0x4d
    li r4, 0x1
    bl fn_8018BDF4
    li r3, 0x4d
    li r4, 0x1
    bl fn_80183350
    lis r3, 0x1da
    addi r3, r3, 0x1002
    bl fn_800F92D4
    li r4, 0x0
    bl fn_800EE150
    addi r4, r1, 0x2c
    mr r24, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r24
    bl fn_800EE828
    addi r3, r1, 0x2c
    addi r4, r30, 0xc
    bl fn_800E01D0
    lfs f1, 0x2c(r1)
    li r3, 0x4d
    lfs f2, 0x30(r1)
    li r4, 0x1
    lfs f3, 0x34(r1)
    li r5, 0x1
    bl fn_80185EE8
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x1
    bl fn_8018A280
    mr r3, r27
    li r4, 0x2c
    li r5, 0x0
    bl fn_80116958
    lfs f27, -21164(r2)
    lis r24, 0x4330
    lfd f31, -21152(r2)
    lfd f29, -21144(r2)
    lfs f28, -21128(r2)
    b @L_801E0B8C
@L_801E0B54:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x70(r1)
    stw r0, 0x74(r1)
    lfd f0, 0x70(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x7c(r1)
    stw r24, 0x78(r1)
    lfd f0, 0x78(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0B8C:
    fcmpo cr0, f27, f28
    blt @L_801E0B54
    li r25, 0x1
    b @L_801E0F34
@L_801E0B9C:
    addi r3, r1, 0x2c
    addi r4, r30, 0x18
    bl fn_800E01D0
    lfs f1, 0x2c(r1)
    li r3, 0x4d
    lfs f2, 0x30(r1)
    li r4, 0x1
    lfs f3, 0x34(r1)
    li r5, 0x1
    bl fn_80185EE8
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x1
    bl fn_8018A280
    mr r3, r27
    li r4, 0x2c
    li r5, 0x2
    bl fn_80116958
    lfs f27, -21164(r2)
    lis r24, 0x4330
    lfd f31, -21152(r2)
    lfd f29, -21144(r2)
    lfs f28, -21168(r2)
    b @L_801E0C34
@L_801E0BFC:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x78(r1)
    stw r0, 0x7c(r1)
    lfd f0, 0x78(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x74(r1)
    stw r24, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0C34:
    fcmpo cr0, f27, f28
    blt @L_801E0BFC
    lfs f2, -21136(r2)
    li r3, 0x4d
    lfs f0, 0x34(r1)
    li r4, 0x1
    lfs f1, 0x2c(r1)
    li r5, 0x1
    fadds f3, f2, f0
    lfs f2, 0x30(r1)
    bl fn_80185EE8
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x1
    bl fn_8018A280
    mr r3, r27
    li r4, 0x2c
    li r5, 0x0
    bl fn_80116958
    addi r3, r1, 0x2c
    addi r4, r30, 0xc
    bl fn_800E01D0
    lfs f1, 0x2c(r1)
    li r3, 0x4d
    lfs f2, 0x30(r1)
    li r4, 0x1
    lfs f3, 0x34(r1)
    li r5, 0x1
    bl fn_80185EE8
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x1
    bl fn_8018A280
    mr r3, r27
    li r4, 0x2c
    li r5, 0x2
    bl fn_80116958
    lfs f27, -21164(r2)
    lis r24, 0x4330
    lfd f31, -21152(r2)
    lfd f29, -21144(r2)
    lfs f28, -21124(r2)
    b @L_801E0D18
@L_801E0CE0:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x78(r1)
    stw r0, 0x7c(r1)
    lfd f0, 0x78(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x74(r1)
    stw r24, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0D18:
    fcmpo cr0, f27, f28
    blt @L_801E0CE0
    lfs f1, 0x20(r1)
    li r3, 0x4d
    lfs f2, 0x24(r1)
    li r4, 0x1
    lfs f3, 0x28(r1)
    li r5, 0x1
    bl fn_80185EE8
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x1
    bl fn_8018A280
    lfs f1, -21164(r2)
    li r3, 0x4d
    lfs f2, -21136(r2)
    li r4, 0x1
    bl fn_8018805C
    lfs f27, -21164(r2)
    li r25, 0xa
    lfd f31, -21152(r2)
    lis r24, 0x4330
    lfd f29, -21144(r2)
    lfs f28, -21120(r2)
    b @L_801E0DB4
@L_801E0D7C:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x78(r1)
    stw r0, 0x7c(r1)
    lfd f0, 0x78(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x74(r1)
    stw r24, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0DB4:
    fcmpo cr0, f27, f28
    blt @L_801E0D7C
    b @L_801E0F34
@L_801E0DC0:
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F550
    lwz r24, 0x0(r31)
    clrlslwi r0, r3, 24, 2
    lwz r25, 0x4(r31)
    addi r3, r1, 0x38
    lwz r29, 0x8(r31)
    lwz r26, 0xc(r31)
    lwz r12, 0x10(r31)
    lwz r11, 0x14(r31)
    lwz r10, 0x18(r31)
    lwz r9, 0x1c(r31)
    lwz r8, 0x20(r31)
    lwz r7, 0x24(r31)
    lwz r6, 0x28(r31)
    lwz r5, 0x2c(r31)
    lwz r4, 0x30(r31)
    stw r24, 0x38(r1)
    stw r25, 0x3c(r1)
    stw r29, 0x40(r1)
    stw r26, 0x44(r1)
    stw r12, 0x48(r1)
    stw r11, 0x4c(r1)
    stw r10, 0x50(r1)
    stw r9, 0x54(r1)
    stw r8, 0x58(r1)
    stw r7, 0x5c(r1)
    stw r6, 0x60(r1)
    stw r5, 0x64(r1)
    stw r4, 0x68(r1)
    lwzx r3, r3, r0
    bl fn_80113D58
    mr r0, r3
    addi r3, r1, 0x14
    mr r29, r0
    addi r4, r30, 0x0
    bl fn_800E01D0
    li r3, 0x0
    bl fn_801ED2DC
    cmplwi r3, 0x0
    beq @L_801E0E8C
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_801E0E8C
    bl fn_8011E15C
    clrlwi r3, r3, 16
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
@L_801E0E8C:
    mr r3, r29
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_800E407C
    li r25, 0x2
    b @L_801E0F34
@L_801E0EAC:
    lfs f27, -21164(r2)
    lis r26, 0x4330
    lfd f28, -21152(r2)
    lfd f30, -21144(r2)
    lfs f31, -21132(r2)
    b @L_801E0EFC
@L_801E0EC4:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x78(r1)
    stw r0, 0x7c(r1)
    lfd f0, 0x78(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x74(r1)
    stw r26, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0EFC:
    fcmpo cr0, f27, f31
    blt @L_801E0EC4
    li r25, 0x64
    b @L_801E0F34
@L_801E0F0C:
    li r28, 0x0
    li r3, 0x4d
    li r4, 0x1
    bl fn_80183018
    mr r3, r29
    li r4, 0x0
    bl fn_800E4014
    mr r3, r29
    bl fn_800E4BF4
    li r29, 0x0
@L_801E0F34:
    cmpwi r28, 0x0
    bne @L_801E0A6C
    psq_l f31, 0xe8(r1), 0, 0
    lfd f31, 0xe0(r1)
    psq_l f30, 0xd8(r1), 0, 0
    lfd f30, 0xd0(r1)
    psq_l f29, 0xc8(r1), 0, 0
    lfd f29, 0xc0(r1)
    psq_l f28, 0xb8(r1), 0, 0
    lfd f28, 0xb0(r1)
    psq_l f27, 0xa8(r1), 0, 0
    lfd f27, 0xa0(r1)
    lmw r24, 0x80(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
