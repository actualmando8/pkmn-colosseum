    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stfd f28, 0x40(r1)
    psq_st f28, 0x48(r1), 0, 0
    stfd f27, 0x30(r1)
    psq_st f27, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    li r28, -0x1
    bl fn_8025F350
    li r3, 0x0
    bl fn_8007B090
    li r0, 0x0
    li r31, 0x1
    stw r0, 0x8(r1)
    li r29, 0x0
@L_800342E4:
    bl fn_801046B8
    cmpw r28, r3
    bne @L_80034358
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80034358
    cmpwi r31, 0x12
    bne @L_80034314
    bl fn_8007AAFC
    b @L_80034358
@L_80034314:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_80034324
    bl fn_8007AAA8
@L_80034324:
    li r3, 0x3c7
    bl fn_80166A28
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x8
    li r4, 0x44ed
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_8003455C
@L_80034358:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8007AB10
    mr r30, r3
    bl fn_800F0308
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80034380
    cmpwi r30, 0x0
    beq @L_800342E4
@L_80034380:
    cmpwi r30, 0x0
    li r29, 0x0
    beq @L_80034390
    mr r31, r30
@L_80034390:
    cmplwi r31, 0x15
    bgt @L_800342E4
    lis r3, -32722
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 20556
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x8
    li r4, 0x3b88
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lfs f27, -31912(r2)
    lfd f31, -31896(r2)
    lis r31, 0x4330
    lfd f29, -31888(r2)
    lfs f28, -31864(r2)
    b @L_80034414
@L_800343DC:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x1c(r1)
    stw r31, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80034414:
    fcmpo cr0, f27, f28
    blt @L_800343DC
    li r3, 0x3c7
    bl fn_80166A28
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_80034434
    bl fn_8007AAA8
@L_80034434:
    li r3, 0x8
    li r4, 0x3b91
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    b @L_8003455C
    li r3, 0x8
    li r4, 0x3b88
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lfs f27, -31912(r2)
    lfd f28, -31896(r2)
    lis r31, 0x4330
    lfd f30, -31888(r2)
    lfs f31, -31864(r2)
    b @L_800344B4
@L_8003447C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x18(r1)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_800344B4:
    fcmpo cr0, f27, f31
    blt @L_8003447C
    addi r4, r1, 0x8
    li r31, 0x12
    li r3, 0x12
    bl fn_8007AB10
    b @L_800342E4
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_800344E0
    bl fn_8007AAA8
@L_800344E0:
    li r3, 0x3c7
    bl fn_80166A28
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x8
    li r4, 0x44ed
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_8003455C
    li r3, 0x8
    li r4, 0x3b8e
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    bl fn_801046B8
    li r29, 0x1
    mr r28, r3
    b @L_800342E4
    bl fn_8007AAA8
    lis r4, -32705
    lis r3, -32710
    addi r4, r4, 31280
    li r5, 0x1040
    addi r3, r3, 13108
    addi r4, r4, 0x2388
    bl fn_80080310
    li r3, 0x2
@L_8003455C:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    psq_l f28, 0x48(r1), 0, 0
    lfd f28, 0x40(r1)
    psq_l f27, 0x38(r1), 0, 0
    lfd f27, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x84(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
