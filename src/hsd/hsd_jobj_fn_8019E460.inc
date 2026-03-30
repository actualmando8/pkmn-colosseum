    nofralloc
    stwu r1, -0x140(r1)
    mflr r0
    stw r0, 0x144(r1)
    stfd f31, 0x130(r1)
    psq_st f31, 0x138(r1), 0, 0
    stfd f30, 0x120(r1)
    psq_st f30, 0x128(r1), 0, 0
    stfd f29, 0x110(r1)
    psq_st f29, 0x118(r1), 0, 0
    stfd f28, 0x100(r1)
    psq_st f28, 0x108(r1), 0, 0
    stfd f27, 0xf0(r1)
    psq_st f27, 0xf8(r1), 0, 0
    stfd f26, 0xe0(r1)
    psq_st f26, 0xe8(r1), 0, 0
    stw r31, 0xdc(r1)
    stw r30, 0xd8(r1)
    stw r29, 0xd4(r1)
    stw r28, 0xd0(r1)
    lis r4, lbl_80274AA0@ha
    mr r29, r3
    addi r7, r4, lbl_80274AA0@l
    lwz r5, 0x10(r3)
    lwz r4, 0x0(r7)
    li r31, 0x0
    lwz r3, 0x4(r7)
    lwz r0, 0x8(r7)
    lfs f30, lbl_8047DB48(r2)
    stw r4, 0x88(r1)
    stw r3, 0x8c(r1)
    stw r0, 0x90(r1)
    b @L_8019E4FC
    @L_8019E4E0:
    lwz r0, 0x14(r5)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x40
    cmplwi r0, 0x0
    bne @L_8019E4F8
    b @L_8019E508
    @L_8019E4F8:
    lwz r5, 0x8(r5)
    @L_8019E4FC:
    cmplwi r5, 0x0
    bne @L_8019E4E0
    li r5, 0x0
    @L_8019E508:
    lwz r3, 0xc(r7)
    mr r30, r5
    lwz r6, 0x10(r7)
    lwz r0, 0x14(r7)
    stw r3, 0x7c(r1)
    lwz r5, 0x18(r7)
    lwz r4, 0x1c(r7)
    lwz r3, 0x20(r7)
    stw r6, 0x80(r1)
    stw r0, 0x84(r1)
    lwz r0, 0x74(r29)
    stw r5, 0x70(r1)
    cmplwi r0, 0x0
    stw r4, 0x74(r1)
    stw r3, 0x78(r1)
    beq @L_8019E564
    lwz r5, 0x74(r29)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x88(r1)
    stw r3, 0x8c(r1)
    stw r0, 0x90(r1)
    @L_8019E564:
    lwz r3, 0x80(r29)
    lis r4, 0x4000
    li r5, 0x0
    bl fn_801B00E0
    mr. r28, r3
    bne @L_8019E58C
    la r3, lbl_8047DB20(r2)
    li r4, 0x85c
    la r5, lbl_8047DB6C(r2)
    bl fn_80196E10
    @L_8019E58C:
    lfs f0, 0x88(r1)
    cmplwi r30, 0x0
    lfs f1, 0x8(r28)
    lfs f26, 0xc(r28)
    fmuls f31, f1, f0
    beq @L_8019E654
    lwz r3, 0x80(r30)
    lis r4, 0x4000
    li r5, 0x0
    bl fn_801B00E0
    mr. r28, r3
    bne @L_8019E5CC
    la r3, lbl_8047DB20(r2)
    li r4, 0x867
    la r5, lbl_8047DB6C(r2)
    bl fn_80196E10
    @L_8019E5CC:
    lfs f0, 0x2c(r30)
    lfs f1, 0x8(r28)
    lwz r0, 0x4(r28)
    fmuls f0, f1, f0
    lfs f1, 0x88(r1)
    lwz r30, 0x10(r30)
    extrwi r31, r0, 1, 29
    fmuls f30, f1, f0
    b @L_8019E60C
    @L_8019E5F0:
    lwz r0, 0x14(r30)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    bne @L_8019E608
    b @L_8019E618
    @L_8019E608:
    lwz r30, 0x8(r30)
    @L_8019E60C:
    cmplwi r30, 0x0
    bne @L_8019E5F0
    li r30, 0x0
    @L_8019E618:
    cmplwi r30, 0x0
    bne @L_8019E630
    la r3, lbl_8047DB20(r2)
    li r4, 0x82d
    la r5, lbl_8047DB68(r2)
    bl fn_80196E10
    @L_8019E630:
    lwz r3, 0x80(r30)
    lis r4, 0x1000
    li r5, 0x1
    bl fn_801B00E0
    cmplwi r3, 0x0
    beq @L_8019E64C
    b @L_8019E6BC
    @L_8019E64C:
    li r30, 0x0
    b @L_8019E6BC
    @L_8019E654:
    lwz r30, 0x10(r29)
    b @L_8019E678
    @L_8019E65C:
    lwz r0, 0x14(r30)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    bne @L_8019E674
    b @L_8019E684
    @L_8019E674:
    lwz r30, 0x8(r30)
    @L_8019E678:
    cmplwi r30, 0x0
    bne @L_8019E65C
    li r30, 0x0
    @L_8019E684:
    cmplwi r30, 0x0
    bne @L_8019E69C
    la r3, lbl_8047DB20(r2)
    li r4, 0x82d
    la r5, lbl_8047DB68(r2)
    bl fn_80196E10
    @L_8019E69C:
    lwz r3, 0x80(r30)
    lis r4, 0x1000
    li r5, 0x1
    bl fn_801B00E0
    cmplwi r3, 0x0
    beq @L_8019E6B8
    b @L_8019E6BC
    @L_8019E6B8:
    li r30, 0x0
    @L_8019E6BC:
    cmplwi r30, 0x0
    beq @L_8019EFCC
    lwz r3, 0x80(r29)
    lis r4, 0x1000
    li r5, 0x3
    bl fn_801B00E0
    cmplwi r3, 0x0
    bne @L_8019E764
    cmplwi r29, 0x0
    beq @L_8019E764
    lwz r0, 0x80(r29)
    cmplwi r0, 0x0
    beq @L_8019E764
    lwz r5, 0x0(r29)
    mr r4, r29
    lwz r3, 0x80(r29)
    lwz r5, 0x50(r5)
    bl fn_801AED88
    cmplwi r29, 0x0
    bne @L_8019E71C
    la r3, lbl_8047DB34(r2)
    li r4, 0x25d
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    @L_8019E71C:
    lwz r0, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_8019E73C
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_8019E73C
    li r3, 0x1
    @L_8019E73C:
    cmpwi r3, 0x0
    beq @L_8019E764
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x40(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r29)
    rlwinm r0, r0, 0, 26, 24
    stw r0, 0x14(r29)
    @L_8019E764:
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    beq @L_8019E780
    lwz r3, 0xc(r29)
    addi r4, r1, 0x7c
    addi r3, r3, 0x44
    bl fn_801A9570
    @L_8019E780:
    lwz r3, 0x80(r30)
    addi r5, r30, 0x38
    li r4, 0x1
    bl fn_801AFCAC
    addi r3, r30, 0x38
    addi r4, r1, 0x7c
    addi r5, r1, 0x58
    bl fn_800A3A9C
    addi r3, r1, 0x58
    addi r4, r1, 0x58
    bl fn_800A3B7C
    fmr f29, f1
    lfs f0, lbl_8047DB7C(r2)
    fcmpo cr0, f29, f0
    ble @L_8019ED98
    lwz r6, 0x58(r1)
    addi r5, r1, 0x28
    lwz r3, 0x5c(r1)
    li r4, 0x3
    lwz r0, 0x60(r1)
    stw r6, 0x34(r1)
    stw r3, 0x38(r1)
    stw r0, 0x3c(r1)
    lwz r3, 0x80(r29)
    bl fn_801AFCAC
    cmpwi r3, 0x0
    beq @L_8019E84C
    addi r3, r1, 0x28
    addi r4, r1, 0x7c
    addi r5, r1, 0x28
    bl fn_800A3A9C
    lfs f0, lbl_8047DB48(r2)
    fcmpu cr0, f0, f26
    beq @L_8019E828
    fmr f1, f26
    addi r3, r1, 0x94
    addi r4, r1, 0x34
    bl fn_800A3244
    addi r3, r1, 0x94
    addi r4, r1, 0x28
    addi r5, r1, 0x28
    bl fn_800A37CC
    @L_8019E828:
    addi r3, r1, 0x34
    addi r4, r1, 0x28
    addi r5, r1, 0x1c
    bl fn_800A3B9C
    addi r3, r1, 0x1c
    addi r4, r1, 0x34
    addi r5, r1, 0x28
    bl fn_800A3B9C
    b @L_8019E884
    @L_8019E84C:
    lfs f0, 0x4c(r29)
    addi r3, r1, 0x1c
    addi r4, r1, 0x34
    addi r5, r1, 0x28
    stfs f0, 0x1c(r1)
    lfs f0, 0x5c(r29)
    stfs f0, 0x20(r1)
    lfs f0, 0x6c(r29)
    stfs f0, 0x24(r1)
    bl fn_800A3B9C
    addi r3, r1, 0x34
    addi r4, r1, 0x28
    addi r5, r1, 0x1c
    bl fn_800A3B9C
    @L_8019E884:
    addi r3, r1, 0x1c
    addi r4, r1, 0x1c
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f8, f2, f1
    fcmpo cr0, f8, f0
    ble @L_8019E904
    frsqrte f7, f8
    lfd f6, lbl_8047DB50(r2)
    lfd f3, lbl_8047DB58(r2)
    lfd f4, lbl_8047DB50(r2)
    fmul f5, f7, f7
    lfd f1, lbl_8047DB58(r2)
    lfd f2, lbl_8047DB50(r2)
    fmul f6, f6, f7
    lfd f0, lbl_8047DB58(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_8019E990
    @L_8019E904:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f8, f0
    bge @L_8019E91C
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_8019E990
    @L_8019E91C:
    stfs f8, 0x18(r1)
    lis r0, 0x7f80
    lwz r3, 0x18(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019E944
    bge @L_8019E97C
    cmpwi r3, 0x0
    beq @L_8019E960
    b @L_8019E97C
    @L_8019E944:
    lwz r0, 0x18(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E958
    li r0, 0x1
    b @L_8019E980
    @L_8019E958:
    li r0, 0x2
    b @L_8019E980
    @L_8019E960:
    lwz r0, 0x18(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E974
    li r0, 0x5
    b @L_8019E980
    @L_8019E974:
    li r0, 0x3
    b @L_8019E980
    @L_8019E97C:
    li r0, 0x4
    @L_8019E980:
    cmpwi r0, 0x1
    bne @L_8019E990
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    @L_8019E990:
    fmr f1, f8
    addi r3, r1, 0x1c
    addi r4, r1, 0x4c
    bl fn_800A3AC0
    addi r3, r1, 0x28
    addi r4, r1, 0x28
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f8, f2, f1
    fcmpo cr0, f8, f0
    ble @L_8019EA20
    frsqrte f7, f8
    lfd f6, lbl_8047DB50(r2)
    lfd f3, lbl_8047DB58(r2)
    lfd f4, lbl_8047DB50(r2)
    fmul f5, f7, f7
    lfd f1, lbl_8047DB58(r2)
    lfd f2, lbl_8047DB50(r2)
    fmul f6, f6, f7
    lfd f0, lbl_8047DB58(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_8019EAAC
    @L_8019EA20:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f8, f0
    bge @L_8019EA38
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_8019EAAC
    @L_8019EA38:
    stfs f8, 0x14(r1)
    lis r0, 0x7f80
    lwz r3, 0x14(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019EA60
    bge @L_8019EA98
    cmpwi r3, 0x0
    beq @L_8019EA7C
    b @L_8019EA98
    @L_8019EA60:
    lwz r0, 0x14(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EA74
    li r0, 0x1
    b @L_8019EA9C
    @L_8019EA74:
    li r0, 0x2
    b @L_8019EA9C
    @L_8019EA7C:
    lwz r0, 0x14(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EA90
    li r0, 0x5
    b @L_8019EA9C
    @L_8019EA90:
    li r0, 0x3
    b @L_8019EA9C
    @L_8019EA98:
    li r0, 0x4
    @L_8019EA9C:
    cmpwi r0, 0x1
    bne @L_8019EAAC
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    @L_8019EAAC:
    fmr f1, f8
    addi r3, r1, 0x28
    addi r4, r1, 0x40
    bl fn_800A3AC0
    fmuls f6, f31, f31
    lfs f4, lbl_8047DB84(r2)
    fmuls f28, f30, f30
    lfs f5, lbl_8047DB80(r2)
    lfs f0, lbl_8047DB48(r2)
    fsubs f2, f6, f28
    fsubs f1, f6, f28
    fadds f3, f6, f28
    fmuls f1, f2, f1
    fmsubs f2, f4, f3, f29
    fdivs f1, f1, f29
    fsubs f1, f2, f1
    fmuls f27, f5, f1
    fcmpo cr0, f27, f0
    bge @L_8019EAFC
    lfs f27, lbl_8047DB48(r2)
    @L_8019EAFC:
    fsubs f0, f6, f27
    lfs f2, lbl_8047DB44(r2)
    lfs f3, lbl_8047DB30(r2)
    lfs f1, lbl_8047DB48(r2)
    fdivs f0, f0, f29
    fadds f2, f2, f0
    fdivs f2, f3, f2
    fcmpo cr0, f2, f1
    ble @L_8019EB88
    lfs f1, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f1, f0
    lfd f7, lbl_8047DB50(r2)
    lfd f4, lbl_8047DB58(r2)
    lfd f5, lbl_8047DB50(r2)
    fdivs f9, f2, f1
    lfd f2, lbl_8047DB58(r2)
    lfd f3, lbl_8047DB50(r2)
    lfd f1, lbl_8047DB58(r2)
    frsqrte f8, f9
    fmul f6, f8, f8
    fmul f7, f7, f8
    fnmsub f4, f9, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f9, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f1, f9, f2, f1
    fmul f8, f3, f1
    fmul f1, f9, f8
    frsp f1, f1
    b @L_8019EC48
    @L_8019EB88:
    lfs f1, lbl_8047DB44(r2)
    lfs f3, lbl_8047DB30(r2)
    fadds f2, f1, f0
    lfd f1, lbl_8047DB60(r2)
    fdivs f2, f3, f2
    fcmpo cr0, f2, f1
    bge @L_8019EBB0
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_80478AC0@l(r3)
    b @L_8019EC48
    @L_8019EBB0:
    lfs f1, lbl_8047DB44(r2)
    lis r0, 0x7f80
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f1, f0
    fdivs f1, f2, f1
    stfs f1, 0x10(r1)
    lwz r3, 0x10(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019EBE8
    bge @L_8019EC20
    cmpwi r3, 0x0
    beq @L_8019EC04
    b @L_8019EC20
    @L_8019EBE8:
    lwz r0, 0x10(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EBFC
    li r0, 0x1
    b @L_8019EC24
    @L_8019EBFC:
    li r0, 0x2
    b @L_8019EC24
    @L_8019EC04:
    lwz r0, 0x10(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EC18
    li r0, 0x5
    b @L_8019EC24
    @L_8019EC18:
    li r0, 0x3
    b @L_8019EC24
    @L_8019EC20:
    li r0, 0x4
    @L_8019EC24:
    cmpwi r0, 0x1
    bne @L_8019EC38
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_80478AC0@l(r3)
    b @L_8019EC48
    @L_8019EC38:
    lfs f1, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f1, f0
    fdivs f1, f2, f1
    @L_8019EC48:
    lfs f2, lbl_8047DB44(r2)
    fmuls f1, f0, f1
    lfs f3, lbl_8047DB30(r2)
    fadds f2, f2, f27
    lfs f0, lbl_8047DB48(r2)
    fdivs f2, f3, f2
    fcmpo cr0, f2, f0
    ble @L_8019ECD0
    lfs f0, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f0, f0, f27
    lfd f7, lbl_8047DB50(r2)
    lfd f4, lbl_8047DB58(r2)
    lfd f5, lbl_8047DB50(r2)
    fdivs f9, f2, f0
    lfd f2, lbl_8047DB58(r2)
    lfd f3, lbl_8047DB50(r2)
    lfd f0, lbl_8047DB58(r2)
    frsqrte f8, f9
    fmul f6, f8, f8
    fmul f7, f7, f8
    fnmsub f4, f9, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f9, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f9, f2, f0
    fmul f8, f3, f0
    fmul f0, f9, f8
    frsp f0, f0
    b @L_8019ED90
    @L_8019ECD0:
    lfs f0, lbl_8047DB44(r2)
    lfs f3, lbl_8047DB30(r2)
    fadds f2, f0, f27
    lfd f0, lbl_8047DB60(r2)
    fdivs f2, f3, f2
    fcmpo cr0, f2, f0
    bge @L_8019ECF8
    lis r3, lbl_80478AC0@ha
    lfs f0, lbl_80478AC0@l(r3)
    b @L_8019ED90
    @L_8019ECF8:
    lfs f0, lbl_8047DB44(r2)
    lis r0, 0x7f80
    lfs f2, lbl_8047DB30(r2)
    fadds f0, f0, f27
    fdivs f0, f2, f0
    stfs f0, 0xc(r1)
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019ED30
    bge @L_8019ED68
    cmpwi r3, 0x0
    beq @L_8019ED4C
    b @L_8019ED68
    @L_8019ED30:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_8019ED44
    li r0, 0x1
    b @L_8019ED6C
    @L_8019ED44:
    li r0, 0x2
    b @L_8019ED6C
    @L_8019ED4C:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_8019ED60
    li r0, 0x5
    b @L_8019ED6C
    @L_8019ED60:
    li r0, 0x3
    b @L_8019ED6C
    @L_8019ED68:
    li r0, 0x4
    @L_8019ED6C:
    cmpwi r0, 0x1
    bne @L_8019ED80
    lis r3, lbl_80478AC0@ha
    lfs f0, lbl_80478AC0@l(r3)
    b @L_8019ED90
    @L_8019ED80:
    lfs f0, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f0, f0, f27
    fdivs f0, f2, f0
    @L_8019ED90:
    fmuls f30, f27, f0
    b @L_8019EDA0
    @L_8019ED98:
    lfs f1, lbl_8047DB48(r2)
    fmr f30, f31
    @L_8019EDA0:
    cmpwi r31, 0x0
    beq @L_8019EDAC
    fneg f30, f30
    @L_8019EDAC:
    fsubs f0, f28, f27
    fcmpo cr0, f0, f29
    bge @L_8019EDC8
    addi r3, r1, 0x58
    addi r4, r1, 0x64
    bl fn_800A3AC0
    b @L_8019EDD8
    @L_8019EDC8:
    fneg f1, f1
    addi r3, r1, 0x58
    addi r4, r1, 0x64
    bl fn_800A3AC0
    @L_8019EDD8:
    fmr f1, f30
    addi r3, r1, 0x40
    addi r4, r1, 0x28
    bl fn_800A3AC0
    addi r3, r1, 0x64
    addi r4, r1, 0x28
    addi r5, r1, 0x64
    bl fn_800A3A78
    addi r3, r1, 0x64
    addi r4, r1, 0x64
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f8, f2, f1
    fcmpo cr0, f8, f0
    ble @L_8019EE78
    frsqrte f7, f8
    lfd f6, lbl_8047DB50(r2)
    lfd f3, lbl_8047DB58(r2)
    lfd f4, lbl_8047DB50(r2)
    fmul f5, f7, f7
    lfd f1, lbl_8047DB58(r2)
    lfd f2, lbl_8047DB50(r2)
    fmul f6, f6, f7
    lfd f0, lbl_8047DB58(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_8019EF04
    @L_8019EE78:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f8, f0
    bge @L_8019EE90
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_8019EF04
    @L_8019EE90:
    stfs f8, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019EEB8
    bge @L_8019EEF0
    cmpwi r3, 0x0
    beq @L_8019EED4
    b @L_8019EEF0
    @L_8019EEB8:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EECC
    li r0, 0x1
    b @L_8019EEF4
    @L_8019EECC:
    li r0, 0x2
    b @L_8019EEF4
    @L_8019EED4:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_8019EEE8
    li r0, 0x5
    b @L_8019EEF4
    @L_8019EEE8:
    li r0, 0x3
    b @L_8019EEF4
    @L_8019EEF0:
    li r0, 0x4
    @L_8019EEF4:
    cmpwi r0, 0x1
    bne @L_8019EF04
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    @L_8019EF04:
    fmr f1, f8
    addi r3, r1, 0x64
    addi r4, r1, 0x64
    bl fn_800A3AC0
    lfs f2, 0x64(r1)
    addi r3, r1, 0x4c
    lfs f0, 0x88(r1)
    addi r4, r1, 0x64
    lfs f1, 0x88(r1)
    addi r5, r1, 0x28
    fmuls f2, f2, f0
    lfs f0, 0x88(r1)
    stfs f2, 0x44(r29)
    lfs f2, 0x68(r1)
    fmuls f1, f2, f1
    stfs f1, 0x54(r29)
    lfs f1, 0x6c(r1)
    fmuls f0, f1, f0
    stfs f0, 0x64(r29)
    bl fn_800A3B9C
    lfs f1, 0x28(r1)
    lfs f0, 0x8c(r1)
    lfs f4, 0x8c(r1)
    fmuls f0, f1, f0
    lfs f3, 0x8c(r1)
    lfs f2, 0x90(r1)
    lfs f1, 0x90(r1)
    stfs f0, 0x48(r29)
    lfs f0, 0x90(r1)
    lfs f5, 0x2c(r1)
    fmuls f4, f5, f4
    stfs f4, 0x58(r29)
    lfs f4, 0x30(r1)
    fmuls f3, f4, f3
    stfs f3, 0x68(r29)
    lfs f3, 0x4c(r1)
    fmuls f2, f3, f2
    stfs f2, 0x4c(r29)
    lfs f2, 0x50(r1)
    fmuls f1, f2, f1
    stfs f1, 0x5c(r29)
    lfs f1, 0x54(r1)
    fmuls f0, f1, f0
    stfs f0, 0x6c(r29)
    lfs f0, 0x7c(r1)
    stfs f0, 0x50(r29)
    lfs f0, 0x80(r1)
    stfs f0, 0x60(r29)
    lfs f0, 0x84(r1)
    stfs f0, 0x70(r29)
    @L_8019EFCC:
    psq_l f31, 0x138(r1), 0, 0
    lfd f31, 0x130(r1)
    psq_l f30, 0x128(r1), 0, 0
    lfd f30, 0x120(r1)
    psq_l f29, 0x118(r1), 0, 0
    lfd f29, 0x110(r1)
    psq_l f28, 0x108(r1), 0, 0
    lfd f28, 0x100(r1)
    psq_l f27, 0xf8(r1), 0, 0
    lfd f27, 0xf0(r1)
    psq_l f26, 0xe8(r1), 0, 0
    lfd f26, 0xe0(r1)
    lwz r31, 0xdc(r1)
    lwz r30, 0xd8(r1)
    lwz r29, 0xd4(r1)
    lwz r0, 0x144(r1)
    lwz r28, 0xd0(r1)
    mtlr r0
    addi r1, r1, 0x140
    blr
