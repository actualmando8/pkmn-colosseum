    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stfd f27, 0x40(r1)
    psq_st f27, 0x48(r1), 0, 0
    stmw r23, 0x1c(r1)
    li r26, 0x0
    li r29, 0x1
@L_801DFC70:
    cmplwi r26, 0x15
    bgt @L_801E0390
    lis r3, -32713
    rlwinm r0, r26, 2, 0, 29
    addi r3, r3, 20832
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    bl fn_801ED218
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DFCA8
    li r26, 0x9
    b @L_801E0390
@L_801DFCA8:
    li r26, 0x1
    b @L_801E0390
    li r3, 0x3b11
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801DFCD8
    li r26, 0x2
    b @L_801E0390
@L_801DFCD8:
    li r26, 0x8
    b @L_801E0390
    li r24, 0x0
    mr r26, r24
@L_801DFCE8:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r26, 0, 16, 31
    bl fn_8012AC08
    mr r25, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DFD2C
    mr r3, r25
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DFD2C
    addi r24, r24, 0x1
@L_801DFD2C:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_801DFCE8
    cmpwi r24, 0x1
    bne @L_801DFD48
    li r26, 0x3
    b @L_801E0390
@L_801DFD48:
    li r26, 0x5
    b @L_801E0390
    li r3, 0x3b13
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x15
    b @L_801E0390
    li r3, 0x3b15
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x15
    b @L_801E0390
    li r3, 0x3b17
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x6
    b @L_801E0390
    li r3, 0x6
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    mr. r31, r3
    blt @L_801DFDC4
    li r26, 0x7
    b @L_801E0390
@L_801DFDC4:
    li r26, 0x8
    b @L_801E0390
    mr r3, r31
    bl fn_801E075C
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b19
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x15
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    mr r28, r3
    bl fn_8012AC08
    mr r27, r3
    li r23, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r24, 0x0
    mr r25, r3
    b @L_801DFE70
@L_801DFE48:
    mr r3, r25
    mr r4, r24
    bl fn_8012AC08
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DFE6C
    rlwinm r3, r23, 0, 16, 31
    addi r0, r3, 0x1
    rlwinm r23, r0, 0, 16, 31
@L_801DFE6C:
    addi r24, r24, 0x1
@L_801DFE70:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801DFE48
    mr r4, r27
    rlwinm r24, r23, 0, 16, 31
    li r3, 0x0
    bl fn_801ED0CC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801E0390
    cmpwi r31, 0x6
    bge @L_801E0390
    mr r23, r31
    subi r25, r24, 0x1
    b @L_801DFF00
@L_801DFEA8:
    mr r3, r28
    rlwinm r4, r23, 0, 16, 31
    bl fn_8012AC08
    addi r0, r23, 0x1
    mr r24, r3
    mr r3, r28
    rlwinm r4, r0, 0, 16, 31
    bl fn_8012AC08
    mr r27, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DFF08
    li r0, 0x27
    subi r5, r24, 0x4
    subi r4, r27, 0x4
    mtctr r0
@L_801DFEE8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801DFEE8
    addi r23, r23, 0x1
@L_801DFF00:
    cmpw r23, r25
    blt @L_801DFEA8
@L_801DFF08:
    mr r3, r27
    bl fn_80124A60
    b @L_801E0390
    li r3, 0x3b1b
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x15
    b @L_801E0390
    li r3, 0x3b1c
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0xa
    b @L_801E0390
    li r3, 0x0
    bl fn_801ED294
    mr r26, r3
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F4A8
    rlwinm r4, r26, 0, 24, 31
    rlwinm r0, r3, 0, 24, 31
    subf. r0, r4, r0
    mr r30, r0
    beq @L_801DFF80
    li r26, 0xb
    b @L_801E0390
@L_801DFF80:
    li r26, 0xc
    b @L_801E0390
    li r3, 0x0
    bl fn_801ED294
    mr r26, r3
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F4A8
    rlwinm r0, r3, 0, 24, 31
    rlwinm r4, r26, 0, 24, 31
    li r3, 0x2f
    subf r4, r4, r0
    bl fn_80132A38
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b1d
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0xc
    b @L_801E0390
    li r3, 0x3b1e
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801E0010
    li r26, 0xd
    b @L_801E0390
@L_801E0010:
    li r26, 0x4
    b @L_801E0390
    li r23, 0x0
    mr r24, r23
@L_801E0020:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r24, 0, 16, 31
    bl fn_8012AC08
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801E0044
    addi r23, r23, 0x1
@L_801E0044:
    addi r24, r24, 0x1
    cmpwi r24, 0x6
    blt @L_801E0020
    cmpwi r23, 0x6
    bne @L_801E0060
    li r26, 0xe
    b @L_801E0390
@L_801E0060:
    li r26, 0xf
    b @L_801E0390
    li r3, 0x3b12
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x4
    b @L_801E0390
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x0
    bl fn_801ED24C
    cmplwi r3, 0x0
    beq @L_801E0100
    li r3, 0x0
    bl fn_801ED2DC
    li r3, 0x0
    bl fn_801ED24C
    mr r26, r3
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011EE28
    lis r4, 0x51ec
    subf r0, r3, r26
    subi r3, r4, 0x7ae1
    mulhw r0, r3, r0
    srawi r0, r0, 5
    rlwinm r3, r0, 1, 31, 31
    add. r0, r0, r3
    beq @L_801E00F8
    mulli r3, r0, 0x64
    addi r4, r3, 0x64
    b @L_801E0104
@L_801E00F8:
    li r4, 0x0
    b @L_801E0104
@L_801E0100:
    li r4, 0x0
@L_801E0104:
    mulli r0, r30, 0x64
    li r3, 0x4b
    add r4, r0, r4
    addi r4, r4, 0x64
    bl fn_80132A38
    li r3, 0x3b14
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801E0140
    li r26, 0x10
    b @L_801E0390
@L_801E0140:
    li r26, 0x4
    b @L_801E0390
    li r3, 0x0
    bl fn_801ED24C
    cmplwi r3, 0x0
    beq @L_801E01AC
    li r3, 0x0
    bl fn_801ED2DC
    li r3, 0x0
    bl fn_801ED24C
    mr r26, r3
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011EE28
    lis r4, 0x51ec
    subf r0, r3, r26
    subi r3, r4, 0x7ae1
    mulhw r0, r3, r0
    srawi r0, r0, 5
    rlwinm r3, r0, 1, 31, 31
    add. r0, r0, r3
    beq @L_801E01A4
    mulli r3, r0, 0x64
    addi r6, r3, 0x64
    b @L_801E01B0
@L_801E01A4:
    li r6, 0x0
    b @L_801E01B0
@L_801E01AC:
    li r6, 0x0
@L_801E01B0:
    mulli r0, r30, 0x64
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    add r23, r0, r6
    addi r23, r23, 0x64
    bl fn_8012A5B0
    cmpw r3, r23
    blt @L_801E0258
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r4, r23
    bl fn_80129474
    li r3, 0x3cb
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f27, -21164(r2)
    lis r26, 0x4330
    lfd f28, -21152(r2)
    lfd f30, -21144(r2)
    lfs f31, -21160(r2)
    b @L_801E0248
@L_801E0210:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r26, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    bl fn_800F0308
@L_801E0248:
    fcmpo cr0, f27, f31
    blt @L_801E0210
    li r26, 0x11
    b @L_801E0390
@L_801E0258:
    li r26, 0x12
    b @L_801E0390
    li r3, 0x1
    bl fn_801069FC
    mr r3, r31
    bl fn_801E09E0
    li r26, 0x13
    b @L_801E0390
    li r3, 0x3b16
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x4
    b @L_801E0390
    li r3, 0x3b18
    li r4, 0x1
    li r5, 0x0
    li r6, 0x2
    bl fn_80106698
    li r26, 0x14
    b @L_801E0390
    li r3, 0x0
    bl fn_801ED2DC
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b1a
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r26, 0x8
    li r24, 0x0
@L_801E02E0:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r24, 0, 16, 31
    bl fn_8012AC08
    mr r23, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801E0308
    b @L_801E0320
@L_801E0308:
    mr r3, r23
    bl fn_8011EE28
    addi r24, r24, 0x1
    cmpwi r24, 0x6
    blt @L_801E02E0
    li r24, -0x1
@L_801E0320:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r24, 0, 16, 31
    bl fn_8012AC08
    mr r4, r3
    li r3, 0x0
    bl fn_801ECFE0
    li r3, 0x4d
    li r4, 0x1
    bl fn_80183350
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x8
    bl fn_8018C69C
    li r3, 0x4d
    li r4, 0x1
    li r5, 0x5
    li r6, 0x0
    li r7, 0x1
    bl fn_8018B76C
    li r3, 0x4d
    li r4, 0x1
    bl fn_80183018
    b @L_801E0390
    li r3, 0x1
    bl fn_801069FC
    li r29, 0x0
@L_801E0390:
    cmpwi r29, 0x0
    bne @L_801DFC70
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    psq_l f27, 0x48(r1), 0, 0
    lfd f27, 0x40(r1)
    lmw r23, 0x1c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
