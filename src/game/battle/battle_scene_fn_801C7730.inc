    nofralloc
    stwu r1, -0x2e0(r1)
    mflr r0
    stw r0, 0x2e4(r1)
    stfd f31, 0x2d0(r1)
    psq_st f31, 0x2d8(r1), 0, 0
    stfd f30, 0x2c0(r1)
    psq_st f30, 0x2c8(r1), 0, 0
    stfd f29, 0x2b0(r1)
    psq_st f29, 0x2b8(r1), 0, 0
    stfd f28, 0x2a0(r1)
    psq_st f28, 0x2a8(r1), 0, 0
    stfd f27, 0x290(r1)
    psq_st f27, 0x298(r1), 0, 0
    stmw r26, 0x278(r1)
    mr r27, r3
    mr r26, r4
    lis r4, -32729
    addi r3, r1, 0x14
    addi r8, r4, 22688
    addi r4, r1, 0x10
    lwz r7, 0x0(r8)
    li r5, 0x1
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x110(r1)
    stw r6, 0x114(r1)
    stw r0, 0x118(r1)
    bl fn_8012EFB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C77B4
    li r3, 0x0
    b @L_801C84C0
@L_801C77B4:
    lwz r3, 0x14(r1)
    addi r5, r1, 0x110
    lwz r4, 0x10(r1)
    bl fn_8018BDF4
    addi r3, r1, 0x1c
    addi r4, r1, 0x18
    li r5, 0x0
    bl fn_8012EFB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C77E8
    li r3, 0x0
    b @L_801C84C0
@L_801C77E8:
    lwz r3, 0x1c(r1)
    addi r5, r1, 0x104
    lwz r4, 0x18(r1)
    bl fn_8018BDF4
    mr r3, r27
    mr r4, r26
    addi r5, r1, 0xf8
    bl fn_8018BDF4
    mr r3, r27
    mr r4, r26
    bl fn_8018D998
    bl fn_8018D928
    cmplwi r3, 0x0
    bne @L_801C7828
    li r3, 0x0
    b @L_801C84C0
@L_801C7828:
    lwz r3, 0x30(r3)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    bne @L_801C7840
    li r3, 0x0
    b @L_801C84C0
@L_801C7840:
    bl fn_8018F5E4
    lfs f0, -21920(r2)
    addi r3, r1, 0x104
    lwz r6, 0x110(r1)
    addi r4, r1, 0xf8
    lwz r5, 0x114(r1)
    fadds f27, f1, f0
    lwz r0, 0x118(r1)
    stw r6, 0xe0(r1)
    stw r5, 0xe4(r1)
    stw r0, 0xe8(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C7898
    lfs f0, -21920(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xf8
    addi r5, r1, 0xe0
    fadds f1, f0, f27
    bl fn_8018D680
    b @L_801C78FC
@L_801C7898:
    addi r3, r1, 0xec
    addi r4, r1, 0x104
    addi r5, r1, 0xf8
    bl fn_800E019C
    addi r3, r1, 0xec
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f0, -21920(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xec
    addi r5, r1, 0xe0
    fadds f27, f0, f27
    fmr f1, f27
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C78E8
    li r3, 0x1
    b @L_801C78FC
@L_801C78E8:
    fmr f1, f27
    addi r3, r1, 0xec
    addi r4, r1, 0xf8
    addi r5, r1, 0xe0
    bl fn_8018D680
@L_801C78FC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7910
    li r3, 0x0
    b @L_801C84C0
@L_801C7910:
    addi r28, r1, 0x1a0
    li r27, 0x0
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfs f30, -21908(r2)
    lfs f29, -21904(r2)
    lfs f28, -21900(r2)
@L_801C792C:
    xoris r0, r27, 0x8000
    stw r26, 0x260(r1)
    stw r0, 0x264(r1)
    lfd f0, 0x260(r1)
    stw r27, 0x0(r28)
    fsubs f0, f0, f31
    fmuls f27, f0, f30
    fmr f1, f27
    bl fn_800CDBE0
    frsp f0, f1
    stfs f28, 0xd8(r1)
    fmr f1, f27
    fmuls f0, f29, f0
    stfs f0, 0xd4(r1)
    bl fn_800CE148
    frsp f0, f1
    addi r3, r28, 0x8
    addi r4, r1, 0xd4
    addi r5, r1, 0x104
    fmuls f0, f29, f0
    stfs f0, 0xdc(r1)
    bl fn_800E019C
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r28, 0x8
    li r5, 0x0
    bl fn_8010F320
    cmpwi r3, 0x0
    beq @L_801C79AC
    li r0, 0x1
    stw r0, 0x4(r28)
    b @L_801C79B4
@L_801C79AC:
    li r0, 0x0
    stw r0, 0x4(r28)
@L_801C79B4:
    addi r3, r28, 0x8
    addi r4, r1, 0x110
    bl fn_800E0040
    stfs f1, 0x14(r28)
    addi r27, r27, 0x1
    addi r28, r28, 0x18
    cmpwi r27, 0x8
    blt @L_801C792C
    lis r4, -32739
    addi r3, r1, 0x1a0
    addi r6, r4, -31492
    li r5, 0x18
    li r4, 0x8
    bl fn_800CA620
    addi r31, r1, 0x1a0
    addi r29, r1, 0x188
    mr r30, r31
    addi r28, r1, 0x194
    addi r27, r1, 0x14c
    li r26, 0x0
@L_801C7A04:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x1
    beq @L_801C7FEC
    lwz r6, 0x8(r30)
    addi r3, r1, 0x104
    lwz r5, 0xc(r30)
    addi r4, r1, 0xf8
    lwz r0, 0x10(r30)
    stw r6, 0xbc(r1)
    stw r5, 0xc0(r1)
    stw r0, 0xc4(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C7A58
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xf8
    addi r5, r1, 0xbc
    bl fn_8018D680
    b @L_801C7AB4
@L_801C7A58:
    addi r3, r1, 0xc8
    addi r4, r1, 0x104
    addi r5, r1, 0xf8
    bl fn_800E019C
    addi r3, r1, 0xc8
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xc8
    addi r5, r1, 0xbc
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C7AA0
    li r3, 0x1
    b @L_801C7AB4
@L_801C7AA0:
    lfs f1, -21896(r2)
    addi r3, r1, 0xc8
    addi r4, r1, 0xf8
    addi r5, r1, 0xbc
    bl fn_8018D680
@L_801C7AB4:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7FEC
    lwz r6, 0x104(r1)
    addi r3, r1, 0x110
    lwz r5, 0x108(r1)
    addi r4, r30, 0x8
    lwz r0, 0x10c(r1)
    stw r6, 0xa4(r1)
    stw r5, 0xa8(r1)
    stw r0, 0xac(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C7B08
    lfs f1, -21896(r2)
    addi r3, r1, 0x110
    addi r4, r30, 0x8
    addi r5, r1, 0xa4
    bl fn_8018D680
    b @L_801C7B64
@L_801C7B08:
    addi r3, r1, 0xb0
    addi r4, r1, 0x110
    addi r5, r30, 0x8
    bl fn_800E019C
    addi r3, r1, 0xb0
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f1, -21896(r2)
    addi r3, r1, 0x110
    addi r4, r1, 0xb0
    addi r5, r1, 0xa4
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C7B50
    li r3, 0x1
    b @L_801C7B64
@L_801C7B50:
    lfs f1, -21896(r2)
    addi r3, r1, 0xb0
    addi r4, r30, 0x8
    addi r5, r1, 0xa4
    bl fn_8018D680
@L_801C7B64:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7FEC
    lwz r6, 0xfc(r1)
    mr r5, r29
    lwz r7, 0xf8(r1)
    addi r3, r1, 0x140
    lwz r0, 0x100(r1)
    addi r4, r1, 0x17c
    lwz r10, 0x104(r1)
    lwz r9, 0x108(r1)
    lwz r8, 0x10c(r1)
    stw r6, 0x198(r1)
    lfs f0, -21920(r2)
    stw r10, 0x17c(r1)
    stw r9, 0x180(r1)
    stw r8, 0x184(r1)
    stw r7, 0x188(r1)
    stw r6, 0x18c(r1)
    stw r0, 0x190(r1)
    stw r7, 0x194(r1)
    stw r0, 0x19c(r1)
    stfs f0, 0x198(r1)
    bl fn_800E0168
    addi r3, r1, 0x140
    mr r4, r3
    bl fn_800E0060
    mr r3, r27
    mr r5, r28
    addi r4, r1, 0x17c
    bl fn_800E0168
    mr r3, r27
    mr r4, r27
    bl fn_800E0060
    mr r5, r27
    addi r3, r1, 0x98
    addi r4, r1, 0x140
    bl fn_800DFFCC
    addi r3, r1, 0x8c
    addi r4, r1, 0xc
    addi r5, r1, 0x98
    addi r6, r1, 0x17c
    addi r7, r1, 0x110
    addi r8, r30, 0x8
    bl fn_8010F4B8
    cmpwi r3, 0x0
    bne @L_801C7C28
    li r0, 0x0
    b @L_801C7C50
@L_801C7C28:
    lfs f1, 0xc(r1)
    lfs f0, -21900(r2)
    fcmpo cr0, f1, f0
    blt @L_801C7C44
    lfs f0, -21920(r2)
    fcmpo cr0, f1, f0
    ble @L_801C7C4C
@L_801C7C44:
    li r0, 0x0
    b @L_801C7C50
@L_801C7C4C:
    li r0, 0x1
@L_801C7C50:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7FEC
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    bl fn_80183350
    mulli r0, r26, 0x18
    addi r27, r1, 0x1a0
    lwz r3, 0x14(r1)
    li r5, 0x1
    lwz r4, 0x10(r1)
    add r27, r27, r0
    addi r6, r27, 0x8
    bl fn_8018AACC
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C7CFC
@L_801C7CA0:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7CC4
    li r3, 0x0
    b @L_801C7D08
@L_801C7CC4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x260(r1)
    stw r0, 0x264(r1)
    lfd f0, 0x260(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x26c(r1)
    stw r26, 0x268(r1)
    lfd f0, 0x268(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C7CFC:
    fcmpo cr0, f27, f28
    bgt @L_801C7CA0
    li r3, 0x1
@L_801C7D08:
    rlwinm r0, r3, 0, 24, 31
    mr r26, r3
    cmplwi r0, 0x0
    bne @L_801C7F2C
    lwz r3, 0x0(r27)
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_801C7F2C
    cmpwi r3, 0x3
    li r4, -0x1
    beq @L_801C7D3C
    cmpwi r3, 0x5
    bne @L_801C7D40
@L_801C7D3C:
    li r4, 0x4
@L_801C7D40:
    cmpwi r3, 0x1
    beq @L_801C7D50
    cmpwi r3, 0x7
    bne @L_801C7D54
@L_801C7D50:
    li r4, 0x0
@L_801C7D54:
    addi r27, r1, 0x1a0
    lwz r0, 0x1a0(r1)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
    lwz r0, 0x0(r27)
    cmpw r0, r4
    beq @L_801C7DD8
    addi r27, r27, 0x18
@L_801C7DD8:
    lwz r0, 0x4(r27)
    cmpwi r0, 0x0
    bne @L_801C7F2C
    lwz r6, 0x8(r27)
    addi r3, r1, 0x104
    lwz r5, 0xc(r27)
    addi r4, r1, 0xf8
    lwz r0, 0x10(r27)
    stw r6, 0x74(r1)
    stw r5, 0x78(r1)
    stw r0, 0x7c(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C7E2C
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xf8
    addi r5, r1, 0x74
    bl fn_8018D680
    b @L_801C7E88
@L_801C7E2C:
    addi r3, r1, 0x80
    addi r4, r1, 0x104
    addi r5, r1, 0xf8
    bl fn_800E019C
    addi r3, r1, 0x80
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0x80
    addi r5, r1, 0x74
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C7E74
    li r3, 0x1
    b @L_801C7E88
@L_801C7E74:
    lfs f1, -21896(r2)
    addi r3, r1, 0x80
    addi r4, r1, 0xf8
    addi r5, r1, 0x74
    bl fn_8018D680
@L_801C7E88:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7F2C
    lwz r3, 0x14(r1)
    addi r6, r27, 0x8
    lwz r4, 0x10(r1)
    li r5, 0x1
    bl fn_8018AACC
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C7F1C
@L_801C7EC0:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7EE4
    li r0, 0x0
    b @L_801C7F28
@L_801C7EE4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x268(r1)
    stw r0, 0x26c(r1)
    lfd f0, 0x268(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x264(r1)
    stw r26, 0x260(r1)
    lfd f0, 0x260(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C7F1C:
    fcmpo cr0, f27, f28
    bgt @L_801C7EC0
    li r0, 0x1
@L_801C7F28:
    mr r26, r0
@L_801C7F2C:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C7FD8
    lwz r3, 0x14(r1)
    addi r5, r1, 0x11c
    lwz r4, 0x10(r1)
    bl fn_8018BA04
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    lfs f1, 0x11c(r1)
    lfs f2, 0x120(r1)
    lfs f3, 0x124(r1)
    lfs f4, -21920(r2)
    bl fn_80187D48
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C7FD0
@L_801C7F7C:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C7FD8
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x268(r1)
    stw r0, 0x26c(r1)
    lfd f0, 0x268(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x264(r1)
    stw r26, 0x260(r1)
    lfd f0, 0x260(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C7FD0:
    fcmpo cr0, f27, f28
    bgt @L_801C7F7C
@L_801C7FD8:
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    bl fn_80183018
    li r3, 0x0
    b @L_801C84C0
@L_801C7FEC:
    addi r30, r30, 0x18
    addi r26, r26, 0x1
    cmpwi r26, 0x8
    blt @L_801C7A04
    addi r28, r1, 0x1a0
    li r27, 0x0
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfs f30, -21908(r2)
    lfs f29, -21904(r2)
    lfs f28, -21900(r2)
@L_801C8018:
    xoris r0, r27, 0x8000
    stw r26, 0x268(r1)
    stw r0, 0x26c(r1)
    lfd f0, 0x268(r1)
    stw r27, 0x0(r28)
    fsubs f0, f0, f31
    fmuls f27, f0, f30
    fmr f1, f27
    bl fn_800CDBE0
    frsp f0, f1
    stfs f28, 0x6c(r1)
    fmr f1, f27
    fmuls f0, f29, f0
    stfs f0, 0x68(r1)
    bl fn_800CE148
    frsp f0, f1
    addi r3, r28, 0x8
    addi r4, r1, 0x68
    addi r5, r1, 0x110
    fmuls f0, f29, f0
    stfs f0, 0x70(r1)
    bl fn_800E019C
    lfs f1, -21896(r2)
    addi r3, r1, 0x110
    addi r4, r28, 0x8
    li r5, 0x0
    bl fn_8010F320
    cmpwi r3, 0x0
    beq @L_801C8098
    li r0, 0x1
    stw r0, 0x4(r28)
    b @L_801C80A0
@L_801C8098:
    li r0, 0x0
    stw r0, 0x4(r28)
@L_801C80A0:
    addi r3, r28, 0x8
    addi r4, r1, 0x110
    bl fn_800E0040
    stfs f1, 0x14(r28)
    addi r27, r27, 0x1
    addi r28, r28, 0x18
    cmpwi r27, 0x8
    blt @L_801C8018
    lis r4, -32739
    addi r3, r1, 0x1a0
    addi r6, r4, -31492
    li r5, 0x18
    li r4, 0x8
    bl fn_800CA620
    addi r27, r1, 0x164
    addi r28, r1, 0x170
    addi r29, r1, 0x134
    li r30, 0x0
@L_801C80E8:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    beq @L_801C84AC
    lwz r6, 0x104(r1)
    addi r3, r1, 0x110
    lwz r5, 0x108(r1)
    addi r4, r31, 0x8
    lwz r0, 0x10c(r1)
    stw r6, 0x50(r1)
    stw r5, 0x54(r1)
    stw r0, 0x58(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C813C
    lfs f1, -21896(r2)
    addi r3, r1, 0x110
    addi r4, r31, 0x8
    addi r5, r1, 0x50
    bl fn_8018D680
    b @L_801C8198
@L_801C813C:
    addi r3, r1, 0x5c
    addi r4, r1, 0x110
    addi r5, r31, 0x8
    bl fn_800E019C
    addi r3, r1, 0x5c
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f1, -21896(r2)
    addi r3, r1, 0x110
    addi r4, r1, 0x5c
    addi r5, r1, 0x50
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C8184
    li r3, 0x1
    b @L_801C8198
@L_801C8184:
    lfs f1, -21896(r2)
    addi r3, r1, 0x5c
    addi r4, r31, 0x8
    addi r5, r1, 0x50
    bl fn_8018D680
@L_801C8198:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C84AC
    lwz r6, 0xfc(r1)
    mr r5, r27
    lwz r7, 0xf8(r1)
    addi r3, r1, 0x128
    lwz r0, 0x100(r1)
    addi r4, r1, 0x158
    lwz r10, 0x104(r1)
    lwz r9, 0x108(r1)
    lwz r8, 0x10c(r1)
    stw r6, 0x174(r1)
    lfs f0, -21920(r2)
    stw r10, 0x158(r1)
    stw r9, 0x15c(r1)
    stw r8, 0x160(r1)
    stw r7, 0x164(r1)
    stw r6, 0x168(r1)
    stw r0, 0x16c(r1)
    stw r7, 0x170(r1)
    stw r0, 0x178(r1)
    stfs f0, 0x174(r1)
    bl fn_800E0168
    addi r3, r1, 0x128
    mr r4, r3
    bl fn_800E0060
    mr r3, r29
    mr r5, r28
    addi r4, r1, 0x158
    bl fn_800E0168
    mr r3, r29
    mr r4, r29
    bl fn_800E0060
    mr r5, r29
    addi r3, r1, 0x44
    addi r4, r1, 0x128
    bl fn_800DFFCC
    addi r3, r1, 0x38
    addi r4, r1, 0x8
    addi r5, r1, 0x44
    addi r6, r1, 0x158
    addi r7, r1, 0x110
    addi r8, r31, 0x8
    bl fn_8010F4B8
    cmpwi r3, 0x0
    bne @L_801C825C
    li r0, 0x0
    b @L_801C8284
@L_801C825C:
    lfs f1, 0x8(r1)
    lfs f0, -21900(r2)
    fcmpo cr0, f1, f0
    blt @L_801C8278
    lfs f0, -21920(r2)
    fcmpo cr0, f1, f0
    ble @L_801C8280
@L_801C8278:
    li r0, 0x0
    b @L_801C8284
@L_801C8280:
    li r0, 0x1
@L_801C8284:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C84AC
    lwz r6, 0x8(r31)
    addi r3, r1, 0x104
    lwz r5, 0xc(r31)
    addi r4, r1, 0xf8
    lwz r0, 0x10(r31)
    stw r6, 0x20(r1)
    stw r5, 0x24(r1)
    stw r0, 0x28(r1)
    bl fn_800E0040
    lfs f0, -21916(r2)
    fcmpo cr0, f1, f0
    bge @L_801C82D8
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0xf8
    addi r5, r1, 0x20
    bl fn_8018D680
    b @L_801C8334
@L_801C82D8:
    addi r3, r1, 0x2c
    addi r4, r1, 0x104
    addi r5, r1, 0xf8
    bl fn_800E019C
    addi r3, r1, 0x2c
    lfs f1, -21912(r2)
    mr r4, r3
    bl fn_800E013C
    lfs f1, -21896(r2)
    addi r3, r1, 0x104
    addi r4, r1, 0x2c
    addi r5, r1, 0x20
    bl fn_8018D680
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C8320
    li r3, 0x1
    b @L_801C8334
@L_801C8320:
    lfs f1, -21896(r2)
    addi r3, r1, 0x2c
    addi r4, r1, 0xf8
    addi r5, r1, 0x20
    bl fn_8018D680
@L_801C8334:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C84AC
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    bl fn_80183350
    mulli r6, r30, 0x18
    addi r0, r1, 0x1a0
    lwz r3, 0x14(r1)
    li r5, 0x1
    lwz r4, 0x10(r1)
    addi r6, r6, 0x8
    add r6, r0, r6
    bl fn_8018AACC
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r26, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C83E0
@L_801C8384:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C83A8
    li r0, 0x0
    b @L_801C83EC
@L_801C83A8:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x268(r1)
    stw r0, 0x26c(r1)
    lfd f0, 0x268(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x264(r1)
    stw r26, 0x260(r1)
    lfd f0, 0x260(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C83E0:
    fcmpo cr0, f27, f28
    bgt @L_801C8384
    li r0, 0x1
@L_801C83EC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C8498
    lwz r3, 0x14(r1)
    addi r5, r1, 0x11c
    lwz r4, 0x10(r1)
    bl fn_8018BA04
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    lfs f1, 0x11c(r1)
    lfs f2, 0x120(r1)
    lfs f3, 0x124(r1)
    lfs f4, -21920(r2)
    bl fn_80187D48
    lfs f27, -21892(r2)
    lfd f28, -21888(r2)
    lis r26, 0x4330
    lfd f30, -21880(r2)
    lfs f31, -21900(r2)
    b @L_801C8490
@L_801C843C:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C8498
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r26, 0x268(r1)
    stw r0, 0x26c(r1)
    lfd f0, 0x268(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x264(r1)
    stw r26, 0x260(r1)
    lfd f0, 0x260(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fsubs f27, f27, f0
@L_801C8490:
    fcmpo cr0, f27, f31
    bgt @L_801C843C
@L_801C8498:
    lwz r3, 0x14(r1)
    lwz r4, 0x10(r1)
    bl fn_80183018
    li r3, 0x0
    b @L_801C84C0
@L_801C84AC:
    addi r31, r31, 0x18
    addi r30, r30, 0x1
    cmpwi r30, 0x8
    blt @L_801C80E8
    li r3, 0x0
@L_801C84C0:
    psq_l f31, 0x2d8(r1), 0, 0
    lfd f31, 0x2d0(r1)
    psq_l f30, 0x2c8(r1), 0, 0
    lfd f30, 0x2c0(r1)
    psq_l f29, 0x2b8(r1), 0, 0
    lfd f29, 0x2b0(r1)
    psq_l f28, 0x2a8(r1), 0, 0
    lfd f28, 0x2a0(r1)
    psq_l f27, 0x298(r1), 0, 0
    lfd f27, 0x290(r1)
    lmw r26, 0x278(r1)
    lwz r0, 0x2e4(r1)
    mtlr r0
    addi r1, r1, 0x2e0
    blr
