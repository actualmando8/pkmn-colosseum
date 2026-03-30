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
    mr r29, r3
    mr r30, r4
    li r5, 0x3b61
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_801C8D68
    bl fn_8025F350
    li r3, 0x2
    li r4, 0x0
    bl fn_8008ABE4
    li r3, 0x1
    bl fn_8007B090
    mr r3, r29
    mr r4, r30
    li r5, 0x3b62
    bl fn_80189990
    li r0, 0x0
    li r28, 0x1
    stw r0, 0x8(r1)
    li r31, 0x0
@L_801C8A94:
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8007AB10
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C8AB4
    cmpwi r3, 0x0
    beq @L_801C8A94
@L_801C8AB4:
    cmpwi r3, 0x0
    li r31, 0x0
    beq @L_801C8AC4
    mr r28, r3
@L_801C8AC4:
    cmplwi r28, 0x13
    bgt @L_801C8D50
    lis r3, -32713
    rlwinm r0, r28, 2, 0, 29
    addi r3, r3, -8676
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r29
    mr r4, r30
    li r5, 0x3b63
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8B0C
    li r28, 0x1d
    b @L_801C8D50
@L_801C8B0C:
    li r28, 0x4
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b64
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8B3C
    li r28, 0x9
    b @L_801C8D50
@L_801C8B3C:
    li r28, 0x5
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b66
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8B6C
    li r28, 0x1d
    b @L_801C8D50
@L_801C8B6C:
    li r28, 0x6
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b82
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8B9C
    li r28, 0x1d
    b @L_801C8BA0
@L_801C8B9C:
    li r28, 0x8
@L_801C8BA0:
    addi r4, r1, 0x8
    li r3, 0x7
    bl fn_8007AB10
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b6c
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8BD8
    li r28, 0x1d
    b @L_801C8BDC
@L_801C8BD8:
    li r28, 0xa
@L_801C8BDC:
    addi r4, r1, 0x8
    li r3, 0x9
    bl fn_8007AB10
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b82
    bl fn_80189990
    bl fn_8001E184
    extsb r28, r3
    addi r4, r1, 0x8
    li r3, 0x9
    bl fn_8007AB10
    cmpwi r28, 0x0
    beq @L_801C8C20
    li r28, 0x1d
    b @L_801C8C24
@L_801C8C20:
    li r28, 0xc
@L_801C8C24:
    addi r4, r1, 0x8
    li r3, 0xb
    bl fn_8007AB10
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b6f
    bl fn_80189990
    li r31, 0x1
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b80
    bl fn_80189990
    li r31, 0x1
    b @L_801C8D50
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_801C8C74
    bl fn_8007AAA8
@L_801C8C74:
    mr r3, r29
    mr r4, r30
    li r5, 0x3b81
    bl fn_80189990
    mr r3, r29
    mr r4, r30
    li r5, 0x3b6a
    bl fn_80189990
    mr r3, r29
    mr r4, r30
    bl fn_80189490
    li r3, 0x1
    b @L_801C8D88
    mr r3, r29
    mr r4, r30
    li r5, 0x3b68
    bl fn_80189990
    lfs f27, -21900(r2)
    lfd f28, -21888(r2)
    lis r28, 0x4330
    lfd f30, -21880(r2)
    lfs f31, -21920(r2)
    b @L_801C8D08
@L_801C8CD0:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r28, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x1c(r1)
    stw r28, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_801C8D08:
    fcmpo cr0, f27, f31
    blt @L_801C8CD0
    li r28, 0x10
    b @L_801C8D50
    mr r3, r29
    mr r4, r30
    li r5, 0x3b82
    bl fn_80189990
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_801C8D40
    li r28, 0x1d
    b @L_801C8D44
@L_801C8D40:
    li r28, 0x1
@L_801C8D44:
    addi r4, r1, 0x8
    li r3, 0x13
    bl fn_8007AB10
@L_801C8D50:
    cmpwi r28, 0x1d
    bne @L_801C8A94
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_801C8D68
    bl fn_8007AAA8
@L_801C8D68:
    mr r3, r29
    mr r4, r30
    li r5, 0x3b83
    bl fn_80189990
    mr r3, r29
    mr r4, r30
    bl fn_80189490
    li r3, 0x0
@L_801C8D88:
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
