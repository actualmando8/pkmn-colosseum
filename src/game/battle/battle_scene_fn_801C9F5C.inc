    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    bl fn_8012BCA4
    li r3, 0x43
    bl fn_801EEAD0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9FB8
    li r30, 0x4
    b @L_801CA1EC
@L_801C9FB8:
    li r3, 0xada
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA084
    li r3, 0xa0f
    bl fn_80190528
    li r3, 0xc
    bl fn_80117AE4
    lfs f27, -21900(r2)
    lfd f31, -21888(r2)
    lis r31, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21892(r2)
    b @L_801CA02C
@L_801C9FF4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_801CA02C:
    fcmpo cr0, f27, f28
    blt @L_801C9FF4
    li r3, 0x439f
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    li r3, 0xf
    bl fn_80117AE4
    li r3, 0x43a0
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    bl fn_801CA358
    bl fn_8012BBA8
    li r3, 0x1
    b @L_801CA318
@L_801CA084:
    li r3, 0xadb
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA150
    li r3, 0x483
    bl fn_80190528
    li r3, 0xc
    bl fn_80117AE4
    lfs f27, -21900(r2)
    lfd f31, -21888(r2)
    lis r31, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21892(r2)
    b @L_801CA0F8
@L_801CA0C0:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_801CA0F8:
    fcmpo cr0, f27, f28
    blt @L_801CA0C0
    li r3, 0x439d
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    li r3, 0xe
    bl fn_80117AE4
    li r3, 0x439e
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    bl fn_801CA358
    bl fn_8012BBA8
    li r3, 0x1
    b @L_801CA318
@L_801CA150:
    li r3, 0x42c
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA174
    li r3, 0x42d
    bl fn_80190528
    li r30, 0x4
    b @L_801CA1EC
@L_801CA174:
    li r3, 0xad3
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA198
    li r3, 0xa08
    bl fn_80190528
    li r30, 0x3
    b @L_801CA1EC
@L_801CA198:
    li r3, 0xad0
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA1BC
    li r3, 0xa07
    bl fn_80190528
    li r30, 0x2
    b @L_801CA1EC
@L_801CA1BC:
    li r3, 0xacf
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA1E0
    li r3, 0xa06
    bl fn_80190528
    li r30, 0x1
    b @L_801CA1EC
@L_801CA1E0:
    li r3, 0xa05
    bl fn_80190528
    li r30, 0x0
@L_801CA1EC:
    bl fn_801CA358
    lfs f27, -21900(r2)
    lfd f28, -21888(r2)
    lis r31, 0x4330
    lfd f30, -21880(r2)
    lfs f31, -21892(r2)
    b @L_801CA240
@L_801CA208:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_801CA240:
    fcmpo cr0, f27, f31
    blt @L_801CA208
    cmpwi r30, 0x2
    beq @L_801CA2B4
    bge @L_801CA264
    cmpwi r30, 0x0
    beq @L_801CA274
    bge @L_801CA294
    b @L_801CA310
@L_801CA264:
    cmpwi r30, 0x4
    beq @L_801CA2F4
    bge @L_801CA310
    b @L_801CA2D4
@L_801CA274:
    li r3, 0x426e
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    b @L_801CA310
@L_801CA294:
    li r3, 0x426f
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    b @L_801CA310
@L_801CA2B4:
    li r3, 0x4270
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    b @L_801CA310
@L_801CA2D4:
    li r3, 0x4271
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
    b @L_801CA310
@L_801CA2F4:
    li r3, 0x4272
    li r4, 0x1
    li r5, 0x0
    li r6, 0x5
    bl fn_80106698
    li r3, 0x1
    bl fn_801065B8
@L_801CA310:
    bl fn_8012BBA8
    li r3, 0x1
@L_801CA318:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x74(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
