    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stfd f30, 0xc0(r1)
    psq_st f30, 0xc8(r1), 0, 0
    stfd f29, 0xb0(r1)
    psq_st f29, 0xb8(r1), 0, 0
    stfd f28, 0xa0(r1)
    psq_st f28, 0xa8(r1), 0, 0
    stfd f27, 0x90(r1)
    psq_st f27, 0x98(r1), 0, 0
    stmw r25, 0x74(r1)
    lis r4, -32728
    mr r29, r3
    addi r31, r4, -26112
    li r25, 0x0
    lwz r7, 0x4c(r31)
    li r26, 0x0
    lwz r6, 0x50(r31)
    li r30, 0x1
    lwz r5, 0x54(r31)
    lwz r4, 0x58(r31)
    lwz r3, 0x5c(r31)
    lwz r0, 0x60(r31)
    stw r7, 0x14(r1)
    stw r6, 0x18(r1)
    stw r5, 0x1c(r1)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
@L_801E07DC:
    cmpwi r26, 0x1
    beq @L_801E0920
    bge @L_801E07F4
    cmpwi r26, 0x0
    bge @L_801E0800
    b @L_801E099C
@L_801E07F4:
    cmpwi r26, 0x64
    beq @L_801E0980
    b @L_801E099C
@L_801E0800:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r29, 16
    bl fn_8012AC08
    bl fn_8011F550
    lwz r25, 0x0(r31)
    clrlslwi r0, r3, 24, 2
    lwz r26, 0x4(r31)
    addi r3, r1, 0x20
    lwz r27, 0x8(r31)
    lwz r28, 0xc(r31)
    lwz r12, 0x10(r31)
    lwz r11, 0x14(r31)
    lwz r10, 0x18(r31)
    lwz r9, 0x1c(r31)
    lwz r8, 0x20(r31)
    lwz r7, 0x24(r31)
    lwz r6, 0x28(r31)
    lwz r5, 0x2c(r31)
    lwz r4, 0x30(r31)
    stw r25, 0x20(r1)
    stw r26, 0x24(r1)
    stw r27, 0x28(r1)
    stw r28, 0x2c(r1)
    stw r12, 0x30(r1)
    stw r11, 0x34(r1)
    stw r10, 0x38(r1)
    stw r9, 0x3c(r1)
    stw r8, 0x40(r1)
    stw r7, 0x44(r1)
    stw r6, 0x48(r1)
    stw r5, 0x4c(r1)
    stw r4, 0x50(r1)
    lwzx r3, r3, r0
    bl fn_80113D58
    mr r0, r3
    lis r4, -32713
    addi r4, r4, 20680
    addi r3, r1, 0x14
    mr r25, r0
    bl fn_800E01D0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r29, 16
    bl fn_8012AC08
    mr r26, r3
    bl fn_8011F5C8
    mr r0, r3
    mr r3, r26
    mr r26, r0
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801E0900
    mr r3, r26
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_801E0900
    bl fn_8011E15C
    clrlwi r3, r3, 16
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
@L_801E0900:
    mr r3, r25
    addi r4, r1, 0x14
    bl fn_800E43A4
    mr r3, r25
    addi r4, r1, 0x8
    bl fn_800E407C
    li r26, 0x1
    b @L_801E099C
@L_801E0920:
    lfs f27, -21164(r2)
    lis r28, 0x4330
    lfd f28, -21152(r2)
    lfd f30, -21144(r2)
    lfs f31, -21132(r2)
    b @L_801E0970
@L_801E0938:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r28, 0x58(r1)
    stw r0, 0x5c(r1)
    lfd f0, 0x58(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x64(r1)
    stw r28, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0970:
    fcmpo cr0, f27, f31
    blt @L_801E0938
    li r26, 0x64
    b @L_801E099C
@L_801E0980:
    mr r3, r25
    li r30, 0x0
    li r4, 0x0
    bl fn_800E4014
    mr r3, r25
    bl fn_800E4BF4
    li r25, 0x0
@L_801E099C:
    cmpwi r30, 0x0
    bne @L_801E07DC
    psq_l f31, 0xd8(r1), 0, 0
    lfd f31, 0xd0(r1)
    psq_l f30, 0xc8(r1), 0, 0
    lfd f30, 0xc0(r1)
    psq_l f29, 0xb8(r1), 0, 0
    lfd f29, 0xb0(r1)
    psq_l f28, 0xa8(r1), 0, 0
    lfd f28, 0xa0(r1)
    psq_l f27, 0x98(r1), 0, 0
    lfd f27, 0x90(r1)
    lmw r25, 0x74(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
