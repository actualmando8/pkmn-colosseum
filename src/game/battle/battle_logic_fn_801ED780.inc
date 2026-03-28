    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    lbz r0, -21088(r13)
    cmplwi r0, 0x0
    beq @L_801EDFF8
    lbz r0, -21087(r13)
    cmplwi r0, 0x0
    bne @L_801ED7D4
    b @L_801EDFF8
@L_801ED7D4:
    bl fn_800D2584
    cmplwi r3, 0x0
    beq @L_801EDFF8
    addi r4, r1, 0x8
    bl fn_800D1F84
    lis r3, -32713
    addi r4, r1, 0x8
    addi r3, r3, 21040
    bl fn_80111C24
    cmpwi r3, 0x1
    beq @L_801EDFF8
    lis r3, -32713
    addi r4, r1, 0x2c
    addi r3, r3, 21040
    bl fn_800D2F34
    mr. r27, r3
    beq @L_801EDFF8
    lfs f3, -20944(r2)
    addi r3, r1, 0x20
    lfs f0, 0x2c(r1)
    lfs f2, -20940(r2)
    lfs f1, 0x30(r1)
    fsubs f3, f3, f0
    lfs f0, -20936(r2)
    fsubs f1, f2, f1
    stfs f3, 0x20(r1)
    stfs f1, 0x24(r1)
    stfs f0, 0x28(r1)
    bl fn_800E008C
    fmr f31, f1
    lfs f0, -20936(r2)
    fcmpo cr0, f31, f0
    ble @L_801ED85C
    b @L_801ED860
@L_801ED85C:
    fneg f1, f31
@L_801ED860:
    lfs f0, -20932(r2)
    fcmpo cr0, f1, f0
    blt @L_801EDFF8
    lwz r3, -21084(r13)
    lis r0, 0x4330
    stw r0, 0x38(r1)
    lwz r0, 0x8(r3)
    lfd f1, -20904(r2)
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f31, f0
    ble @L_801ED898
    b @L_801EDFF8
@L_801ED898:
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lfs f1, -20936(r2)
    lfs f3, -20928(r2)
    fmr f2, f1
    lfs f4, -20924(r2)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x1
    bl fn_800D9ED8
    cmpwi r27, 0x2
    bne @L_801EDE64
    fmr f1, f31
    addi r3, r1, 0x20
    mr r4, r3
    bl fn_800E00AC
    lwz r5, -21084(r13)
    lis r3, 0x4330
    stw r3, 0x38(r1)
    lwz r4, 0xc(r5)
    lfd f4, -20904(r2)
    stw r4, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f4
    fcmpo cr0, f31, f0
    bge @L_801ED944
    lfs f1, -20920(r2)
    b @L_801ED9AC
@L_801ED944:
    lwz r0, 0x8(r5)
    stw r4, 0x44(r1)
    subf r0, r4, r0
    lfs f0, -20936(r2)
    stw r3, 0x40(r1)
    lfd f1, 0x40(r1)
    stw r0, 0x3c(r1)
    fsubs f1, f1, f4
    stw r3, 0x38(r1)
    lfd f3, 0x38(r1)
    fsubs f2, f31, f1
    stw r0, 0x4c(r1)
    fsubs f3, f3, f4
    stw r3, 0x48(r1)
    lfd f1, 0x48(r1)
    fsubs f2, f3, f2
    fsubs f1, f1, f4
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_801ED99C
    fmr f1, f0
    b @L_801ED9AC
@L_801ED99C:
    lfs f0, -20920(r2)
    fcmpo cr0, f1, f0
    ble @L_801ED9AC
    fmr f1, f0
@L_801ED9AC:
    lfs f0, -20916(r2)
    li r3, 0x4
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r28, 0x4c(r1)
    bl fn_800D6A00
    lis r3, -32719
    addi r3, r3, 19576
    bl fn_800D7820
    lwz r4, -21084(r13)
    lis r3, -32697
    lwz r30, -21080(r13)
    addi r31, r3, -10704
    lhz r27, 0x6(r4)
    b @L_801EDCD0
@L_801ED9EC:
    lhz r3, 0x2(r30)
    lbz r0, 0x0(r30)
    slwi r3, r3, 2
    cmplwi r0, 0x1
    lwzx r29, r31, r3
    bne @L_801EDAD8
    mr r3, r29
    bl fn_800EF4FC
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x48(r1)
    lfd f3, -20896(r2)
    stw r3, 0x4c(r1)
    mr r3, r29
    lfs f1, 0x8(r30)
    lfd f2, 0x48(r1)
    lfs f0, 0x2c(r1)
    fsubs f2, f2, f3
    fnmsubs f30, f1, f2, f0
    bl fn_800EF4F4
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x40(r1)
    lfd f3, -20896(r2)
    stw r3, 0x44(r1)
    mr r3, r29
    lfs f1, 0x8(r30)
    lfd f2, 0x40(r1)
    lfs f0, 0x30(r1)
    fsubs f2, f2, f3
    fnmsubs f29, f1, f2, f0
    bl fn_800EF4FC
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x38(r1)
    lfd f3, -20896(r2)
    stw r3, 0x3c(r1)
    mr r3, r29
    lfs f1, 0x8(r30)
    lfd f2, 0x38(r1)
    lfs f0, 0x2c(r1)
    fsubs f2, f2, f3
    fmadds f28, f1, f2, f0
    bl fn_800EF4F4
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x50(r1)
    lfd f3, -20896(r2)
    stw r3, 0x54(r1)
    lfs f1, 0x8(r30)
    lfd f2, 0x50(r1)
    lfs f0, 0x30(r1)
    fsubs f2, f2, f3
    fmadds f27, f1, f2, f0
    b @L_801EDBE4
@L_801EDAD8:
    lfs f1, -20912(r2)
    addi r3, r1, 0x14
    lfs f0, 0x4(r30)
    addi r4, r1, 0x20
    fmuls f0, f1, f0
    fmuls f1, f0, f31
    bl fn_800E013C
    mr r3, r29
    bl fn_800EF4FC
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x50(r1)
    lfd f3, -20896(r2)
    stw r3, 0x54(r1)
    mr r3, r29
    lfs f1, 0x14(r1)
    lfd f2, 0x50(r1)
    lfs f0, -20944(r2)
    fsubs f3, f2, f3
    lfs f2, 0x8(r30)
    fadds f0, f1, f0
    fnmsubs f30, f2, f3, f0
    bl fn_800EF4F4
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x48(r1)
    lfd f3, -20896(r2)
    stw r3, 0x4c(r1)
    mr r3, r29
    lfs f1, 0x18(r1)
    lfd f2, 0x48(r1)
    lfs f0, -20940(r2)
    fsubs f3, f2, f3
    lfs f2, 0x8(r30)
    fadds f0, f1, f0
    fnmsubs f29, f2, f3, f0
    bl fn_800EF4FC
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x40(r1)
    lfd f3, -20896(r2)
    stw r3, 0x44(r1)
    mr r3, r29
    lfs f1, 0x14(r1)
    lfd f2, 0x40(r1)
    lfs f0, -20944(r2)
    fsubs f3, f2, f3
    lfs f2, 0x8(r30)
    fadds f0, f1, f0
    fmadds f28, f2, f3, f0
    bl fn_800EF4F4
    extrwi r3, r3, 15, 16
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x38(r1)
    lfd f3, -20896(r2)
    stw r3, 0x3c(r1)
    lfs f1, 0x18(r1)
    lfd f2, 0x38(r1)
    lfs f0, -20940(r2)
    fsubs f3, f2, f3
    lfs f2, 0x8(r30)
    fadds f0, f1, f0
    fmadds f27, f2, f3, f0
@L_801EDBE4:
    mr r4, r29
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D67BC
    fmr f1, f30
    lfs f3, -20936(r2)
    fmr f2, f29
    bl fn_800D6680
    clrlwi r4, r28, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20936(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    fmr f1, f28
    lfs f3, -20936(r2)
    fmr f2, f29
    bl fn_800D6680
    clrlwi r4, r28, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20920(r2)
    li r3, 0x0
    lfs f2, -20936(r2)
    bl fn_800D59B8
    fmr f1, f30
    lfs f3, -20936(r2)
    fmr f2, f27
    bl fn_800D6680
    clrlwi r4, r28, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20936(r2)
    li r3, 0x0
    lfs f2, -20920(r2)
    bl fn_800D59B8
    fmr f1, f28
    lfs f3, -20936(r2)
    fmr f2, f27
    bl fn_800D6680
    clrlwi r4, r28, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20920(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    addi r30, r30, 0xc
@L_801EDCD0:
    cmpwi r27, 0x0
    subi r27, r27, 0x1
    bne @L_801ED9EC
    lwz r3, -21084(r13)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    lwz r5, 0xc(r3)
    lfd f5, -20904(r2)
    stw r5, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f5
    fcmpo cr0, f31, f0
    bge @L_801EDFF8
    lwz r4, 0x10(r3)
    stw r0, 0x50(r1)
    stw r4, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f5
    fcmpo cr0, f31, f0
    bge @L_801EDD28
    lfs f1, 0x14(r3)
    b @L_801EDD94
@L_801EDD28:
    stw r4, 0x4c(r1)
    subf r4, r4, r5
    lfs f4, 0x14(r3)
    stw r0, 0x48(r1)
    lfs f0, -20936(r2)
    lfd f1, 0x48(r1)
    stw r4, 0x54(r1)
    fsubs f1, f1, f5
    stw r0, 0x50(r1)
    lfd f3, 0x50(r1)
    fsubs f2, f31, f1
    stw r4, 0x44(r1)
    fsubs f3, f3, f5
    stw r0, 0x40(r1)
    lfd f1, 0x40(r1)
    fsubs f2, f3, f2
    fsubs f1, f1, f5
    fdivs f1, f2, f1
    fmuls f1, f4, f1
    fcmpo cr0, f1, f0
    bge @L_801EDD84
    fmr f1, f0
    b @L_801EDD94
@L_801EDD84:
    lfs f0, -20920(r2)
    fcmpo cr0, f1, f0
    ble @L_801EDD94
    fmr f1, f0
@L_801EDD94:
    lfs f0, -20916(r2)
    li r3, 0x2
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x50(r1)
    lwz r27, 0x54(r1)
    bl fn_800D888C
    li r3, 0x4
    bl fn_800D6A00
    lis r3, -32719
    addi r3, r3, 18776
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, -20936(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f2, -20936(r2)
    lfs f1, -20928(r2)
    fmr f3, f2
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20936(r2)
    lfs f2, -20924(r2)
    fmr f3, f1
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20928(r2)
    lfs f2, -20924(r2)
    lfs f3, -20936(r2)
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    bl fn_800D6728
    b @L_801EDFF8
@L_801EDE64:
    li r3, 0x2
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA4C4
    lwz r3, -21084(r13)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    lwz r5, 0xc(r3)
    lfd f5, -20904(r2)
    stw r5, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f5
    fcmpo cr0, f31, f0
    bge @L_801EDFF8
    lwz r4, 0x10(r3)
    stw r0, 0x50(r1)
    stw r4, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f5
    fcmpo cr0, f31, f0
    bge @L_801EDEC0
    lfs f1, 0x18(r3)
    b @L_801EDF2C
@L_801EDEC0:
    stw r4, 0x4c(r1)
    subf r4, r4, r5
    lfs f4, 0x18(r3)
    stw r0, 0x48(r1)
    lfs f0, -20936(r2)
    lfd f1, 0x48(r1)
    stw r4, 0x54(r1)
    fsubs f1, f1, f5
    stw r0, 0x50(r1)
    lfd f3, 0x50(r1)
    fsubs f2, f31, f1
    stw r4, 0x44(r1)
    fsubs f3, f3, f5
    stw r0, 0x40(r1)
    lfd f1, 0x40(r1)
    fsubs f2, f3, f2
    fsubs f1, f1, f5
    fdivs f1, f2, f1
    fmuls f1, f4, f1
    fcmpo cr0, f1, f0
    bge @L_801EDF1C
    fmr f1, f0
    b @L_801EDF2C
@L_801EDF1C:
    lfs f0, -20920(r2)
    fcmpo cr0, f1, f0
    ble @L_801EDF2C
    fmr f1, f0
@L_801EDF2C:
    lfs f0, -20916(r2)
    li r3, 0x2
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x50(r1)
    lwz r27, 0x54(r1)
    bl fn_800D888C
    li r3, 0x4
    bl fn_800D6A00
    lis r3, -32719
    addi r3, r3, 18776
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, -20936(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f2, -20936(r2)
    lfs f1, -20928(r2)
    fmr f3, f2
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20936(r2)
    lfs f2, -20924(r2)
    fmr f3, f1
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    lfs f1, -20928(r2)
    lfs f2, -20924(r2)
    lfs f3, -20936(r2)
    bl fn_800D6680
    clrlwi r4, r27, 24
    li r3, 0x0
    mr r5, r4
    mr r6, r4
    bl fn_800D5C18
    bl fn_800D6728
@L_801EDFF8:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
