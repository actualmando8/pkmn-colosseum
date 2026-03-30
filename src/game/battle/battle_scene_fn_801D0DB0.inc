    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stfd f30, 0xb0(r1)
    psq_st f30, 0xb8(r1), 0, 0
    stfd f29, 0xa0(r1)
    psq_st f29, 0xa8(r1), 0, 0
    stfd f28, 0x90(r1)
    psq_st f28, 0x98(r1), 0, 0
    stfd f27, 0x80(r1)
    psq_st f27, 0x88(r1), 0, 0
    stmw r21, 0x54(r1)
    lis r6, -32728
    lis r5, -32728
    addi r7, r6, -27872
    lwz r0, -21792(r2)
    addi r6, r5, -27860
    lwz r9, 0x0(r7)
    lwz r8, 0x4(r7)
    lis r10, -32713
    lwz r7, 0x8(r7)
    mr r27, r3
    lwz r5, 0x0(r6)
    mr r28, r4
    lwz r4, 0x4(r6)
    addi r26, r10, -8144
    lwz r3, 0x8(r6)
    li r31, 0x1
    stw r9, 0x30(r1)
    li r22, 0x0
    lfs f31, -21784(r2)
    stw r8, 0x34(r1)
    stw r7, 0x38(r1)
    stw r5, 0x24(r1)
    stw r4, 0x28(r1)
    stw r3, 0x2c(r1)
    stw r0, 0x8(r1)
@L_801D0E4C:
    cmpwi r22, 0x5
    beq @L_801D1118
    bge @L_801D0E80
    cmpwi r22, 0x2
    beq @L_801D12D8
    bge @L_801D0E74
    cmpwi r22, 0x0
    beq @L_801D0EA4
    bge @L_801D0FC4
    b @L_801D12F4
@L_801D0E74:
    cmpwi r22, 0x4
    bge @L_801D109C
    b @L_801D0FDC
@L_801D0E80:
    cmpwi r22, 0x8
    beq @L_801D129C
    bge @L_801D0E98
    cmpwi r22, 0x7
    bge @L_801D11F8
    b @L_801D1184
@L_801D0E98:
    cmpwi r22, 0x3e8
    beq @L_801D12F0
    b @L_801D12F4
@L_801D0EA4:
    addi r22, r26, 0x58
    li r30, 0x0
    bl fn_80113F48
    li r0, 0xb
    mtctr r0
@L_801D0EB8:
    lwz r0, 0x0(r22)
    cmpw r0, r3
    bne @L_801D0ECC
    lwz r3, 0x4(r22)
    b @L_801D0ED8
@L_801D0ECC:
    addi r22, r22, 0x8
    bdnz @L_801D0EB8
    li r3, 0x0
@L_801D0ED8:
    bl fn_800F92D4
    mr r0, r3
    li r3, 0x0
    mr r24, r0
    bl fn_801D0AFC
    lis r4, -32698
    rlwinm r25, r3, 0, 16, 31
    addi r5, r4, 29560
    li r0, 0x0
    stw r0, 0x0(r5)
    mr r3, r27
    mr r4, r28
    stw r0, 0x4(r5)
    stw r0, 0x8(r5)
    stw r0, 0xc(r5)
    stw r0, 0x10(r5)
    stw r0, 0x14(r5)
    bl fn_8018D998
    bl fn_8018D928
    mr. r22, r3
    beq @L_801D0FB8
    mr r3, r24
    li r23, 0x1
    li r4, 0xe
    bl fn_800EE150
    addi r4, r1, 0x30
    mr r21, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r21
    bl fn_800EE828
    mr r3, r22
    bl fn_8018FC50
    lwz r5, 0x0(r3)
    lwz r4, 0x4(r3)
    lwz r0, 0x8(r3)
    mr r3, r22
    stw r5, 0x18(r1)
    stw r4, 0x1c(r1)
    stw r0, 0x20(r1)
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0xc
    addi r4, r1, 0x30
    bl fn_800E0168
    lfs f1, 0xc(r1)
    lfs f2, 0x14(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f2, -21780(r2)
    mr r3, r27
    mr r4, r28
    bl fn_8018805C
    li r22, 0x1
    b @L_801D12F4
@L_801D0FB8:
    li r23, 0x0
    li r22, 0x3
    b @L_801D12F4
@L_801D0FC4:
    mr r3, r27
    mr r4, r28
    li r5, 0x1
    bl fn_8018A280
    li r22, 0x3
    b @L_801D12F4
@L_801D0FDC:
    li r3, 0x0
    li r4, 0x406
    bl fn_801662E8
    mr r22, r3
    addis r0, r22, 0x1
    cmplwi r0, 0xffff
    beq @L_801D1004
    li r4, 0x1068
    li r5, 0x0
    bl fn_801668DC
@L_801D1004:
    rlwinm r21, r30, 2, 0, 29
    addi r3, r26, 0x34
    lfs f31, -21784(r2)
    mr r29, r22
    lwzx r4, r3, r21
    mr r3, r24
    bl fn_800EE150
    addi r4, r1, 0x30
    mr r22, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r22
    bl fn_800EE828
    rlwinm r3, r30, 0, 16, 31
    bl fn_801D0AA0
    clrlslwi r0, r3, 24, 2
    addi r3, r26, 0x0
    lwzx r3, r3, r0
    bl fn_80113D58
    lis r5, -32698
    addi r4, r1, 0x30
    addi r22, r5, 29560
    stwx r3, r22, r21
    lwzx r3, r22, r21
    bl fn_800E43A4
    lwzx r3, r22, r21
    addi r4, r1, 0x24
    bl fn_800E407C
    lwzx r3, r22, r21
    addi r4, r1, 0x8
    bl fn_800E5BE0
    li r3, 0x3c3
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r22, 0x4
    b @L_801D12F4
@L_801D109C:
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x44(r1)
    lfd f1, -21760(r2)
    stw r0, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f27, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f2, -21752(r2)
    stw r0, 0x48(r1)
    lfs f0, -21776(r2)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f27
    fadds f31, f31, f1
    stfs f1, -21568(r13)
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_801D1110
    addi r30, r30, 0x1
    cmpw r30, r25
    bne @L_801D1108
    li r22, 0x5
    b @L_801D12F4
@L_801D1108:
    li r22, 0x3
    b @L_801D12F4
@L_801D1110:
    bl fn_800F0308
    b @L_801D12F4
@L_801D1118:
    rlwinm r4, r25, 1, 0, 30
    cmplwi r24, 0x0
    addi r3, r26, 0x4c
    subi r0, r4, 0x2
    lhax r4, r3, r0
    beq @L_801D1164
    mr r3, r24
    bl fn_800ECCA8
    lfs f1, -21784(r2)
    mr r3, r24
    bl fn_800ECA78
    lfs f1, -21772(r2)
    mr r3, r24
    bl fn_800EC9DC
    mr r3, r24
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r24
    bl fn_800EC990
@L_801D1164:
    li r3, 0x19
    li r4, 0x7d0
    li r5, 0xff
    bl fn_801653CC
    li r22, 0x6
    b @L_801D12F4
    b @L_801D1184
@L_801D1180:
    bl fn_800F0308
@L_801D1184:
    mr r3, r24
    bl fn_800EC960
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D1180
    lfs f31, -21784(r2)
    lis r22, 0x4330
    lfd f30, -21760(r2)
    lfd f28, -21752(r2)
    lfs f27, -21768(r2)
    b @L_801D11E8
@L_801D11AC:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r22, 0x48(r1)
    stw r0, 0x4c(r1)
    lfd f0, 0x48(r1)
    fsubs f29, f0, f30
    bl fn_800D3088
    stw r3, 0x44(r1)
    stw r22, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f28
    fdivs f0, f0, f29
    fadds f31, f31, f0
    stfs f0, -21568(r13)
    bl fn_800F0308
@L_801D11E8:
    fcmpo cr0, f31, f27
    blt @L_801D11AC
    li r22, 0x7
    b @L_801D12F4
@L_801D11F8:
    lis r3, -32698
    li r21, 0x0
    addi r22, r3, 29560
@L_801D1204:
    lwz r3, 0x0(r22)
    cmplwi r3, 0x0
    beq @L_801D1220
    li r4, 0x0
    bl fn_800E4014
    lwz r3, 0x0(r22)
    bl fn_800E4BF4
@L_801D1220:
    addi r21, r21, 0x1
    addi r22, r22, 0x4
    cmpwi r21, 0x6
    blt @L_801D1204
    lfs f31, -21784(r2)
    lis r22, 0x4330
    lfd f27, -21760(r2)
    lfd f29, -21752(r2)
    lfs f30, -21768(r2)
    b @L_801D1284
@L_801D1248:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r22, 0x48(r1)
    stw r0, 0x4c(r1)
    lfd f0, 0x48(r1)
    fsubs f28, f0, f27
    bl fn_800D3088
    stw r3, 0x44(r1)
    stw r22, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f28
    fadds f31, f31, f0
    stfs f0, -21568(r13)
    bl fn_800F0308
@L_801D1284:
    fcmpo cr0, f31, f30
    blt @L_801D1248
    mr r3, r29
    bl fn_80165548
    li r22, 0x8
    b @L_801D12F4
@L_801D129C:
    rlwinm. r0, r23, 0, 24, 31
    beq @L_801D12D0
    mr r3, r27
    mr r4, r28
    bl fn_8018D998
    bl fn_8018D928
    lfs f1, 0x1c(r1)
    mr r3, r27
    lfs f2, -21780(r2)
    mr r4, r28
    bl fn_8018805C
    li r22, 0x2
    b @L_801D12F4
@L_801D12D0:
    li r22, 0x3e8
    b @L_801D12F4
@L_801D12D8:
    mr r3, r27
    mr r4, r28
    li r5, 0x1
    bl fn_8018A280
    li r22, 0x3e8
    b @L_801D12F4
@L_801D12F0:
    li r31, 0x0
@L_801D12F4:
    cmpwi r31, 0x0
    bne @L_801D0E4C
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    psq_l f28, 0x98(r1), 0, 0
    lfd f28, 0x90(r1)
    psq_l f27, 0x88(r1), 0, 0
    lfd f27, 0x80(r1)
    lmw r21, 0x54(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
