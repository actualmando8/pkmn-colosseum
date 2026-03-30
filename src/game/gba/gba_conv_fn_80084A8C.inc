    nofralloc
    stwu r1, -0xcd0(r1)
    mflr r0
    stw r0, 0xcd4(r1)
    stfd f31, 0xcc0(r1)
    psq_st f31, -0x338(r1), 0, 0
    stfd f30, 0xcb0(r1)
    psq_st f30, -0x348(r1), 0, 0
    stfd f29, 0xca0(r1)
    psq_st f29, -0x358(r1), 0, 0
    stfd f28, 0xc90(r1)
    psq_st f28, -0x368(r1), 0, 0
    stfd f27, 0xc80(r1)
    psq_st f27, -0x378(r1), 0, 0
    stmw r15, 0xc3c(r1)
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    lis r3, -32729
    li r15, 0x0
    addi r0, r3, -3352
    stw r0, 0xc34(r1)
    li r3, 0x1
    bl fn_80093698
    b @L_80084AF4
@L_80084AF0:
    bl fn_800F0308
@L_80084AF4:
    lwz r3, 0xc34(r1)
    li r5, 0x0
    addi r4, r3, 0x190
    li r3, 0x1
    bl fn_800932F0
    cmpwi r3, 0x0
    beq @L_80084AF0
    li r3, 0xe4
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xe4
    bl fn_80104704
    mr r16, r3
    cmplwi r16, 0x0
    bne @L_80084B44
    lwz r3, 0xc34(r1)
    li r4, 0x1f4
    addi r5, r2, -29952
    addi r3, r3, 0x184
    bl fn_80196E10
@L_80084B44:
    cmplwi r16, 0x0
    bne @L_80084B58
    li r3, 0xa6
    bl fn_80104704
    mr r16, r3
@L_80084B58:
    mr r3, r16
    bl fn_801040A0
    lwz r0, 0x8(r1)
    li r5, 0x0
    lwz r16, 0x0(r3)
    addi r30, r13, -32460
    rlwinm r4, r0, 4, 0, 27
    lwz r0, 0xc(r1)
    lwz r3, 0xc34(r1)
    stw r0, 0x24(r16)
    lwz r0, 0x8(r1)
    addi r29, r3, 0x30
    add r29, r29, r4
    stw r0, 0x2c(r16)
    lbz r0, -32460(r13)
    stb r0, 0x21(r16)
    li r0, 0x5
    lbz r3, 0x0(r30)
    cmpwi r5, 0x0
    extsb r3, r3
    rlwinm r5, r3, 2, 0, 29
    lwzx r3, r29, r5
    stwx r3, r16, r5
    bge @L_80084BC8
    lwzx r3, r16, r5
    cmpwi r3, 0x1
    bne @L_80084BC8
    stwx r0, r16, r5
@L_80084BC8:
    addi r4, r30, 0x1
    li r5, 0x1
    lbz r3, 0x0(r4)
    cmpwi r5, 0x0
    extsb r3, r3
    rlwinm r5, r3, 2, 0, 29
    lwzx r3, r29, r5
    stwx r3, r16, r5
    bge @L_80084BFC
    lwzx r3, r16, r5
    cmpwi r3, 0x1
    bne @L_80084BFC
    stwx r0, r16, r5
@L_80084BFC:
    addi r4, r4, 0x1
    li r5, 0x2
    lbz r3, 0x0(r4)
    cmpwi r5, 0x0
    extsb r3, r3
    rlwinm r5, r3, 2, 0, 29
    lwzx r3, r29, r5
    stwx r3, r16, r5
    bge @L_80084C30
    lwzx r3, r16, r5
    cmpwi r3, 0x1
    bne @L_80084C30
    stwx r0, r16, r5
@L_80084C30:
    addi r4, r4, 0x1
    li r5, 0x3
    lbz r3, 0x0(r4)
    cmpwi r5, 0x0
    extsb r3, r3
    rlwinm r5, r3, 2, 0, 29
    lwzx r3, r29, r5
    stwx r3, r16, r5
    bge @L_80084C64
    lwzx r3, r16, r5
    cmpwi r3, 0x1
    bne @L_80084C64
    stwx r0, r16, r5
@L_80084C64:
    lwz r0, 0xc(r1)
    mr r24, r16
    rlwinm r0, r0, 0, 27, 27
    cmplwi r0, 0x0
    beq @L_80084D98
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_80084C8C
    cmpwi r0, 0x2
    bne @L_80084D98
@L_80084C8C:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    lwz r0, 0xc(r1)
    li r4, 0x0
    stb r4, 0x21(r16)
    li r4, 0x8
    rlwinm r0, r0, 0, 30, 30
    mr r18, r3
    cmplwi r0, 0x0
    stw r4, 0x0(r16)
    beq @L_80084D6C
    li r3, 0x2f
    li r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3d88
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lfs f27, -29944(r2)
    lfd f31, -29936(r2)
    lis r17, 0x4330
    lfd f29, -29928(r2)
    lfs f28, -29940(r2)
    b @L_80084D2C
@L_80084CF4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r17, 0xc10(r1)
    stw r0, 0xc14(r1)
    lfd f0, 0xc10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc1c(r1)
    stw r17, 0xc18(r1)
    lfd f0, 0xc18(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80084D2C:
    fcmpo cr0, f27, f28
    blt @L_80084CF4
    mr r3, r18
    bl fn_800776E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084D6C
    li r3, 0xe4
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r18
    li r4, 0x0
    bl fn_8005CF2C
    li r3, 0x0
    b @L_80087AAC
@L_80084D6C:
    lwz r0, 0x10(r1)
    cmplwi r0, 0x0
    beq @L_80084D90
    mr r3, r0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_80084D90
    mr r4, r18
    bl fn_8012AC64
@L_80084D90:
    li r0, 0xa
    stw r0, 0x0(r16)
@L_80084D98:
    lwz r0, 0xc(r1)
    li r25, 0x0
    stw r30, 0xc30(r1)
    rlwinm r0, r0, 0, 25, 25
    stw r0, 0xc2c(r1)
    lwz r0, 0xc(r1)
    rlwinm r0, r0, 0, 31, 31
    stw r0, 0xc28(r1)
    lwz r0, 0xc(r1)
    rlwinm r0, r0, 0, 30, 30
    stw r0, 0xc24(r1)
    lwz r0, 0xc(r1)
    rlwinm r0, r0, 0, 28, 28
    stw r0, 0xc20(r1)
    lwz r0, 0xc(r1)
    rlwinm r31, r0, 0, 26, 26
@L_80084DD8:
    lwz r3, 0xc30(r1)
    lwz r0, 0x10(r1)
    lbz r23, 0x0(r3)
    cmplwi r0, 0x0
    stb r23, 0x21(r24)
    beq @L_80084E10
    extsb r0, r23
    lwz r3, 0x10(r1)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    beq @L_80084E10
    mr r22, r0
    b @L_80084E14
@L_80084E10:
    addi r22, r1, 0xf4
@L_80084E14:
    lwz r0, 0x14(r1)
    cmplwi r0, 0x0
    beq @L_80084E34
    extsb r0, r23
    cmpwi r0, 0x1
    bne @L_80084E34
    lwz r21, 0x14(r1)
    b @L_80084E38
@L_80084E34:
    addi r21, r1, 0x1c
@L_80084E38:
    extsb r28, r23
    addi r26, r28, 0x1
    rlwinm r27, r28, 2, 0, 29
@L_80084E44:
    bl fn_80103CB0
    addi r4, r13, -32464
    lbzx r0, r4, r28
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    lwz r0, 0x28(r24)
    cmplwi r0, 0x4
    beq @L_8008507C
    li r0, 0x2
    mr r4, r26
    stwx r0, r27, r24
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c42
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    mr r3, r24
    li r4, 0x6
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008507C
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_80084EB4:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_80084EC4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_80084ED8
    cmpwi r0, 0x4
    bne @L_80084F10
@L_80084ED8:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084F10
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80084F10:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80084F20
    li r16, 0x1
@L_80084F20:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_80084EC4
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084F50
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_80084EB4
@L_80084F50:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80084F9C
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80084F9C
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80084F9C
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80084F9C
    li r15, 0x4
@L_80084F9C:
    cmpwi r15, 0x3
    ble @L_80084FBC
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80084FBC
    li r3, 0x0
    b @L_80085010
@L_80084FBC:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80084FF0
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085004
@L_80084FF0:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085004:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085010:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085030
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80085030:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80085054
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085068
@L_80085054:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085068:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_8008507C:
    mr r4, r26
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c43
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    rlwinm r0, r15, 0, 24, 31
    li r3, 0x3
    cmplwi r0, 0x0
    stwx r3, r27, r24
    bne @L_8008531C
    li r0, 0x0
    stw r0, 0x28(r24)
    b @L_80085114
@L_800850BC:
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800850D8
    bl fn_800F0308
    b @L_80085114
@L_800850D8:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800850FC
    li r0, 0x2
    li r3, 0xe
    stw r0, 0x28(r24)
    b @L_8008513C
@L_800850FC:
    lwz r0, 0x28(r24)
    cmplwi r0, 0x8
    bne @L_80085110
    li r3, 0xe
    b @L_8008513C
@L_80085110:
    bl fn_800F0308
@L_80085114:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_800934E4
    cmpwi r3, 0x0
    beq @L_800850BC
    li r0, 0x0
    stw r0, 0x28(r24)
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093610
@L_8008513C:
    cmpwi r3, 0xe
    bne @L_80085318
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_80085150:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_80085160:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_80085174
    cmpwi r0, 0x4
    bne @L_800851AC
@L_80085174:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800851AC
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_800851AC:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_800851BC
    li r16, 0x1
@L_800851BC:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_80085160
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800851EC
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_80085150
@L_800851EC:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80085238
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80085238
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80085238
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80085238
    li r15, 0x4
@L_80085238:
    cmpwi r15, 0x3
    ble @L_80085258
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80085258
    li r3, 0x0
    b @L_800852AC
@L_80085258:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_8008528C
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_800852A0
@L_8008528C:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_800852A0:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_800852AC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800852CC
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_800852CC:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_800852F0
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085304
@L_800852F0:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085304:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_80085318:
    li r15, 0x1
@L_8008531C:
    mr r3, r28
    li r4, 0x0
    bl fn_80093160
    li r0, 0x0
    stw r0, 0x28(r24)
    b @L_8008538C
@L_80085334:
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80085350
    bl fn_800F0308
    b @L_8008538C
@L_80085350:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80085374
    li r0, 0x2
    li r3, 0xe
    stw r0, 0x28(r24)
    b @L_800853B4
@L_80085374:
    lwz r0, 0x28(r24)
    cmplwi r0, 0x8
    bne @L_80085388
    li r3, 0xe
    b @L_800853B4
@L_80085388:
    bl fn_800F0308
@L_8008538C:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_800934E4
    cmpwi r3, 0x0
    beq @L_80085334
    li r0, 0x0
    stw r0, 0x28(r24)
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093610
@L_800853B4:
    cmpwi r3, 0xe
    beq @L_800853E0
    bge @L_800853CC
    cmpwi r3, 0x2
    beq @L_800857E0
    b @L_800855B4
@L_800853CC:
    lis r4, 0x2
    addi r0, r4, 0x2
    cmpw r3, r0
    beq @L_800855B4
    b @L_800855B4
@L_800853E0:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800853EC:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800853FC:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_80085410
    cmpwi r0, 0x4
    bne @L_80085448
@L_80085410:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085448
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80085448:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80085458
    li r16, 0x1
@L_80085458:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800853FC
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085488
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800853EC
@L_80085488:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800854D4
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800854D4
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800854D4
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800854D4
    li r15, 0x4
@L_800854D4:
    cmpwi r15, 0x3
    ble @L_800854F4
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800854F4
    li r3, 0x0
    b @L_80085548
@L_800854F4:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80085528
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_8008553C
@L_80085528:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_8008553C:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085548:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085568
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80085568:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_8008558C
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_800855A0
@L_8008558C:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_800855A0:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_800855B4:
    li r20, 0x0
@L_800855B8:
    li r19, 0x0
    mr r17, r24
    mr r18, r19
    addi r16, r13, -32464
@L_800855C8:
    lwz r0, 0x0(r17)
    cmpwi r0, 0x5
    beq @L_800855DC
    cmpwi r0, 0x4
    bne @L_80085614
@L_800855DC:
    addi r3, r18, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085614
    bl fn_80103CB0
    lbz r0, 0x0(r16)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r17)
    stw r0, 0x28(r24)
@L_80085614:
    lwz r0, 0x0(r17)
    cmpwi r0, 0x7
    bne @L_80085624
    li r19, 0x1
@L_80085624:
    addi r17, r17, 0x4
    addi r16, r16, 0x1
    addi r18, r18, 0x1
    cmpwi r18, 0x3
    ble @L_800855C8
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085654
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_800855B8
@L_80085654:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800856A0
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800856A0
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800856A0
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800856A0
    li r16, 0x4
@L_800856A0:
    cmpwi r16, 0x3
    ble @L_800856C0
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800856C0
    li r3, 0x0
    b @L_80085714
@L_800856C0:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_800856F4
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085708
@L_800856F4:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085708:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085714:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80085740
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80085738
    li r0, 0x1
    b @L_800857CC
@L_80085738:
    li r0, 0x0
    b @L_800857CC
@L_80085740:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c47
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_8008579C
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_800857BC
@L_8008579C:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800857BC
    li r0, 0x1
    b @L_800857CC
@L_800857BC:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_800857CC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_800857E0:
    li r0, 0x4
    li r16, 0x0
    stwx r0, r27, r24
@L_800857EC:
    mr r3, r26
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008581C
    bl fn_80103CB0
    addi r4, r13, -32464
    lbzx r0, r4, r28
    or r0, r0, r3
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    b @L_8008582C
@L_8008581C:
    bl fn_800F0308
    addi r16, r16, 0x1
    cmpwi r16, 0x12c
    blt @L_800857EC
@L_8008582C:
    lwz r0, 0xc2c(r1)
    cmplwi r0, 0x0
    bne @L_800858B0
    mr r4, r26
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c4d
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lfs f27, -29944(r2)
    lfd f28, -29936(r2)
    lis r16, 0x4330
    lfd f30, -29928(r2)
    lfs f31, -29940(r2)
    b @L_800858A8
@L_80085870:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r16, 0xc18(r1)
    stw r0, 0xc1c(r1)
    lfd f0, 0xc18(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0xc14(r1)
    stw r16, 0xc10(r1)
    lfd f0, 0xc10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_800858A8:
    fcmpo cr0, f27, f31
    blt @L_80085870
@L_800858B0:
    mr r3, r22
    bl fn_8012A248
    li r0, 0x0
    mr r3, r28
    stw r0, 0x18(r1)
    mr r4, r22
    addi r5, r1, 0x18
    bl fn_80092FC8
    li r0, 0x0
    stw r0, 0x28(r24)
    b @L_80085934
@L_800858DC:
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800858F8
    bl fn_800F0308
    b @L_80085934
@L_800858F8:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8008591C
    li r0, 0x2
    li r16, 0xe
    stw r0, 0x28(r24)
    b @L_80085960
@L_8008591C:
    lwz r0, 0x28(r24)
    cmplwi r0, 0x8
    bne @L_80085930
    li r16, 0xe
    b @L_80085960
@L_80085930:
    bl fn_800F0308
@L_80085934:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_800934E4
    cmpwi r3, 0x0
    beq @L_800858DC
    li r0, 0x0
    stw r0, 0x28(r24)
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093610
    mr r16, r3
@L_80085960:
    cmpwi r16, 0xe
    beq @L_8008596C
    b @L_80085B40
@L_8008596C:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_80085978:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_80085988:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_8008599C
    cmpwi r0, 0x4
    bne @L_800859D4
@L_8008599C:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800859D4
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_800859D4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_800859E4
    li r16, 0x1
@L_800859E4:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_80085988
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085A14
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_80085978
@L_80085A14:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80085A60
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80085A60
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80085A60
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80085A60
    li r15, 0x4
@L_80085A60:
    cmpwi r15, 0x3
    ble @L_80085A80
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80085A80
    li r3, 0x0
    b @L_80085AD4
@L_80085A80:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80085AB4
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085AC8
@L_80085AB4:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085AC8:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085AD4:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085AF4
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80085AF4:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80085B18
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085B2C
@L_80085B18:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085B2C:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_80085B40:
    lwz r3, 0x18(r1)
    rlwinm r0, r3, 24, 30, 31
    cmplwi r0, 0x0
    beq @L_80085D7C
    li r20, 0x0
@L_80085B54:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80085B64:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80085B78
    cmpwi r0, 0x4
    bne @L_80085BB0
@L_80085B78:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085BB0
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_80085BB0:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_80085BC0
    li r18, 0x1
@L_80085BC0:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80085B64
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085BF0
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80085B54
@L_80085BF0:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80085C3C
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80085C3C
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80085C3C
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80085C3C
    li r16, 0x4
@L_80085C3C:
    cmpwi r16, 0x3
    ble @L_80085C5C
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80085C5C
    li r3, 0x0
    b @L_80085CB0
@L_80085C5C:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_80085C90
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085CA4
@L_80085C90:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085CA4:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085CB0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80085CDC
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80085CD4
    li r0, 0x1
    b @L_80085D68
@L_80085CD4:
    li r0, 0x0
    b @L_80085D68
@L_80085CDC:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c49
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80085D38
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80085D58
@L_80085D38:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80085D58
    li r0, 0x1
    b @L_80085D68
@L_80085D58:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80085D68:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_80085D7C:
    rlwinm r0, r3, 28, 28, 31
    cmplwi r0, 0x7
    bgt @L_80085DD0
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -5256
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r17, 0x1
    b @L_80085DD8
    li r17, 0x2
    b @L_80085DD8
    li r17, 0x4
    b @L_80085DD8
    li r17, 0x5
    b @L_80085DD8
    li r17, 0x3
    b @L_80085DD8
    li r17, 0x6
    b @L_80085DD8
@L_80085DD0:
    li r0, 0x0
    b @L_80085E1C
@L_80085DD8:
    bl fn_80128E24
    cmplwi r3, 0x0
    beq @L_80085E18
    bl fn_80128E04
    cmplwi r3, 0x0
    beq @L_80085E18
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_80085E18
    bl fn_80135A70
    rlwinm r3, r3, 0, 24, 31
    rlwinm r0, r17, 0, 24, 31
    cmplw r3, r0
    bne @L_80085E18
    li r0, 0x1
    b @L_80085E1C
@L_80085E18:
    li r0, 0x0
@L_80085E1C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086054
    li r20, 0x0
@L_80085E2C:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80085E3C:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80085E50
    cmpwi r0, 0x4
    bne @L_80085E88
@L_80085E50:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085E88
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_80085E88:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_80085E98
    li r18, 0x1
@L_80085E98:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80085E3C
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80085EC8
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80085E2C
@L_80085EC8:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80085F14
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80085F14
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80085F14
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80085F14
    li r16, 0x4
@L_80085F14:
    cmpwi r16, 0x3
    ble @L_80085F34
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80085F34
    li r3, 0x0
    b @L_80085F88
@L_80085F34:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_80085F68
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80085F7C
@L_80085F68:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80085F7C:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80085F88:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80085FB4
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80085FAC
    li r0, 0x1
    b @L_80086040
@L_80085FAC:
    li r0, 0x0
    b @L_80086040
@L_80085FB4:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x44f0
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086010
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80086030
@L_80086010:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086030
    li r0, 0x1
    b @L_80086040
@L_80086030:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086040:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_80086054:
    cmpwi r16, 0x4
    beq @L_80086060
    b @L_80086070
@L_80086060:
    lwz r3, 0x18(r1)
    rlwinm r0, r3, 0, 30, 30
    cmplwi r0, 0x0
    bne @L_8008629C
@L_80086070:
    li r20, 0x0
@L_80086074:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80086084:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80086098
    cmpwi r0, 0x4
    bne @L_800860D0
@L_80086098:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800860D0
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_800860D0:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_800860E0
    li r18, 0x1
@L_800860E0:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80086084
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086110
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80086074
@L_80086110:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_8008615C
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_8008615C
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_8008615C
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_8008615C
    li r16, 0x4
@L_8008615C:
    cmpwi r16, 0x3
    ble @L_8008617C
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_8008617C
    li r3, 0x0
    b @L_800861D0
@L_8008617C:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_800861B0
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_800861C4
@L_800861B0:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_800861C4:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_800861D0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800861FC
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_800861F4
    li r0, 0x1
    b @L_80086288
@L_800861F4:
    li r0, 0x0
    b @L_80086288
@L_800861FC:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c49
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086258
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80086278
@L_80086258:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086278
    li r0, 0x1
    b @L_80086288
@L_80086278:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086288:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_8008629C:
    lwz r0, 0xc28(r1)
    cmplwi r0, 0x0
    beq @L_80086514
    rlwinm r0, r3, 0, 29, 29
    li r4, 0x1
    cmplwi r0, 0x0
    beq @L_800862CC
    rlwinm r0, r3, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800862DC
    li r4, 0x0
    b @L_800862DC
@L_800862CC:
    rlwinm r0, r3, 0, 31, 31
    cmplwi r0, 0x0
    bne @L_800862DC
    li r4, 0x0
@L_800862DC:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086514
    li r20, 0x0
@L_800862EC:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_800862FC:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80086310
    cmpwi r0, 0x4
    bne @L_80086348
@L_80086310:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086348
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_80086348:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_80086358
    li r18, 0x1
@L_80086358:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_800862FC
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086388
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_800862EC
@L_80086388:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800863D4
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800863D4
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800863D4
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800863D4
    li r16, 0x4
@L_800863D4:
    cmpwi r16, 0x3
    ble @L_800863F4
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800863F4
    li r3, 0x0
    b @L_80086448
@L_800863F4:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_80086428
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_8008643C
@L_80086428:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_8008643C:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80086448:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086474
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_8008646C
    li r0, 0x1
    b @L_80086500
@L_8008646C:
    li r0, 0x0
    b @L_80086500
@L_80086474:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x44c3
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800864D0
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_800864F0
@L_800864D0:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800864F0
    li r0, 0x1
    b @L_80086500
@L_800864F0:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086500:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_80086514:
    lwz r0, 0xc24(r1)
    cmplwi r0, 0x0
    beq @L_800868EC
    rlwinm r0, r3, 0, 31, 31
    cmplwi r0, 0x0
    bne @L_80086758
    li r20, 0x0
@L_80086530:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80086540:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80086554
    cmpwi r0, 0x4
    bne @L_8008658C
@L_80086554:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008658C
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_8008658C:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_8008659C
    li r18, 0x1
@L_8008659C:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80086540
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800865CC
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80086530
@L_800865CC:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80086618
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80086618
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80086618
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80086618
    li r16, 0x4
@L_80086618:
    cmpwi r16, 0x3
    ble @L_80086638
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086638
    li r3, 0x0
    b @L_8008668C
@L_80086638:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_8008666C
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80086680
@L_8008666C:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80086680:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_8008668C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800866B8
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_800866B0
    li r0, 0x1
    b @L_80086744
@L_800866B0:
    li r0, 0x0
    b @L_80086744
@L_800866B8:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x44c3
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086714
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80086734
@L_80086714:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086734
    li r0, 0x1
    b @L_80086744
@L_80086734:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086744:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_80086758:
    mr r3, r22
    bl fn_800776E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800868EC
    li r3, 0xe4
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r22
    li r4, 0x1
    bl fn_8005CF2C
    lwz r0, 0xc20(r1)
    cmplwi r0, 0x0
    bne @L_8008679C
    li r3, 0x0
    b @L_80087AAC
@L_8008679C:
    li r3, 0xe4
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xe4
    bl fn_80104704
    mr r16, r3
    cmplwi r16, 0x0
    bne @L_800867D0
    lwz r3, 0xc34(r1)
    li r4, 0x1f4
    addi r5, r2, -29952
    addi r3, r3, 0x184
    bl fn_80196E10
@L_800867D0:
    cmplwi r16, 0x0
    bne @L_800867E4
    li r3, 0xa6
    bl fn_80104704
    mr r16, r3
@L_800867E4:
    mr r3, r16
    bl fn_801040A0
    lwz r5, 0x0(r3)
    li r4, 0x0
    lwz r0, 0xc(r1)
    lwz r3, 0xc30(r1)
    stw r0, 0x24(r5)
    lwz r0, 0x8(r1)
    stw r0, 0x2c(r5)
    lbz r0, 0x0(r3)
    stb r0, 0x21(r5)
    li r0, 0x5
    lbz r3, 0x0(r30)
    cmpw r4, r25
    extsb r3, r3
    rlwinm r4, r3, 2, 0, 29
    lwzx r3, r29, r4
    stwx r3, r5, r4
    bge @L_80086840
    lwzx r3, r5, r4
    cmpwi r3, 0x1
    bne @L_80086840
    stwx r0, r5, r4
@L_80086840:
    addi r6, r30, 0x1
    li r4, 0x1
    lbz r3, 0x0(r6)
    cmpw r4, r25
    extsb r3, r3
    rlwinm r4, r3, 2, 0, 29
    lwzx r3, r29, r4
    stwx r3, r5, r4
    bge @L_80086874
    lwzx r3, r5, r4
    cmpwi r3, 0x1
    bne @L_80086874
    stwx r0, r5, r4
@L_80086874:
    addi r6, r6, 0x1
    li r4, 0x2
    lbz r3, 0x0(r6)
    cmpw r4, r25
    extsb r3, r3
    rlwinm r4, r3, 2, 0, 29
    lwzx r3, r29, r4
    stwx r3, r5, r4
    bge @L_800868A8
    lwzx r3, r5, r4
    cmpwi r3, 0x1
    bne @L_800868A8
    stwx r0, r5, r4
@L_800868A8:
    addi r6, r6, 0x1
    li r4, 0x3
    lbz r3, 0x0(r6)
    cmpw r4, r25
    extsb r3, r3
    rlwinm r4, r3, 2, 0, 29
    lwzx r3, r29, r4
    stwx r3, r5, r4
    bge @L_800868DC
    lwzx r3, r5, r4
    cmpwi r3, 0x1
    bne @L_800868DC
    stwx r0, r5, r4
@L_800868DC:
    li r0, 0x6
    mr r24, r5
    stwx r0, r27, r5
    b @L_80084E44
@L_800868EC:
    lwz r0, 0x14(r1)
    cmplwi r0, 0x0
    beq @L_80086904
    extsb r0, r23
    cmpwi r0, 0x1
    beq @L_8008690C
@L_80086904:
    cmplwi r31, 0x0
    beq @L_80086DBC
@L_8008690C:
    mr r3, r28
    mr r4, r21
    bl fn_80092E38
    li r0, 0x0
    stw r0, 0x28(r24)
    b @L_8008697C
@L_80086924:
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086940
    bl fn_800F0308
    b @L_8008697C
@L_80086940:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80086964
    li r0, 0x2
    li r3, 0xe
    stw r0, 0x28(r24)
    b @L_800869A4
@L_80086964:
    lwz r0, 0x28(r24)
    cmplwi r0, 0x8
    bne @L_80086978
    li r3, 0xe
    b @L_800869A4
@L_80086978:
    bl fn_800F0308
@L_8008697C:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_800934E4
    cmpwi r3, 0x0
    beq @L_80086924
    li r0, 0x0
    stw r0, 0x28(r24)
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093610
@L_800869A4:
    cmpwi r3, 0xe
    beq @L_800869BC
    bge @L_80086B90
    cmpwi r3, 0xb
    beq @L_80086DBC
    b @L_80086B90
@L_800869BC:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800869C8:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800869D8:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800869EC
    cmpwi r0, 0x4
    bne @L_80086A24
@L_800869EC:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086A24
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80086A24:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80086A34
    li r16, 0x1
@L_80086A34:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800869D8
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086A64
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800869C8
@L_80086A64:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80086AB0
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80086AB0
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80086AB0
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80086AB0
    li r15, 0x4
@L_80086AB0:
    cmpwi r15, 0x3
    ble @L_80086AD0
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086AD0
    li r3, 0x0
    b @L_80086B24
@L_80086AD0:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80086B04
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80086B18
@L_80086B04:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80086B18:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80086B24:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086B44
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80086B44:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80086B68
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80086B7C
@L_80086B68:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80086B7C:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_80086B90:
    li r20, 0x0
@L_80086B94:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80086BA4:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80086BB8
    cmpwi r0, 0x4
    bne @L_80086BF0
@L_80086BB8:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086BF0
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_80086BF0:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_80086C00
    li r18, 0x1
@L_80086C00:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80086BA4
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086C30
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80086B94
@L_80086C30:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80086C7C
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80086C7C
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80086C7C
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80086C7C
    li r16, 0x4
@L_80086C7C:
    cmpwi r16, 0x3
    ble @L_80086C9C
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086C9C
    li r3, 0x0
    b @L_80086CF0
@L_80086C9C:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_80086CD0
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80086CE4
@L_80086CD0:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80086CE4:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80086CF0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086D1C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80086D14
    li r0, 0x1
    b @L_80086DA8
@L_80086D14:
    li r0, 0x0
    b @L_80086DA8
@L_80086D1C:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c47
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086D78
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80086D98
@L_80086D78:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086D98
    li r0, 0x1
    b @L_80086DA8
@L_80086D98:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086DA8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x0
    b @L_80087AAC
@L_80086DBC:
    cmplwi r31, 0x0
    beq @L_8008702C
    lwz r0, 0x8(r21)
    rlwinm r0, r0, 0, 27, 27
    cmplwi r0, 0x0
    bne @L_8008702C
    li r20, 0x0
@L_80086DD8:
    li r18, 0x0
    mr r16, r24
    mr r19, r18
    addi r17, r13, -32464
@L_80086DE8:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x5
    beq @L_80086DFC
    cmpwi r0, 0x4
    bne @L_80086E34
@L_80086DFC:
    addi r3, r19, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086E34
    bl fn_80103CB0
    lbz r0, 0x0(r17)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r16)
    stw r0, 0x28(r24)
@L_80086E34:
    lwz r0, 0x0(r16)
    cmpwi r0, 0x7
    bne @L_80086E44
    li r18, 0x1
@L_80086E44:
    addi r16, r16, 0x4
    addi r17, r17, 0x1
    addi r19, r19, 0x1
    cmpwi r19, 0x3
    ble @L_80086DE8
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80086E74
    bl fn_800F0308
    addi r20, r20, 0x1
    cmpwi r20, 0xf
    blt @L_80086DD8
@L_80086E74:
    li r3, 0x26
    bl fn_80166A28
    li r16, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_80086EC0
    li r16, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_80086EC0
    li r16, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_80086EC0
    li r16, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_80086EC0
    li r16, 0x4
@L_80086EC0:
    cmpwi r16, 0x3
    ble @L_80086EE0
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086EE0
    li r3, 0x0
    b @L_80086F34
@L_80086EE0:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r16, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r16, 0x0
    bne @L_80086F14
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80086F28
@L_80086F14:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80086F28:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80086F34:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086F60
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80086F58
    li r0, 0x1
    b @L_80086FEC
@L_80086F58:
    li r0, 0x0
    b @L_80086FEC
@L_80086F60:
    lbz r0, 0x21(r24)
    li r4, 0x6
    li r3, 0x2f
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r4, r24, r0
    lbz r0, 0x21(r24)
    extsb r4, r0
    addi r4, r4, 0x1
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x4417
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x24(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_80086FBC
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    b @L_80086FDC
@L_80086FBC:
    mr r3, r24
    li r4, 0x7
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80086FDC
    li r0, 0x1
    b @L_80086FEC
@L_80086FDC:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r0, 0x0
@L_80086FEC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80084E44
    li r3, 0x2f
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x44cf
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    mr r3, r24
    li r4, 0x1
    bl fn_80087AE8
    li r3, 0x0
    b @L_80087AAC
@L_8008702C:
    mr r3, r28
    bl fn_80093698
    li r3, 0x3cc
    bl fn_80166A28
    li r0, 0x5
    stwx r0, r24, r27
    bl fn_80103CB0
    addi r4, r13, -32464
    lbzx r0, r4, r28
    or r0, r0, r3
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    mr r4, r26
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x7
    li r4, 0x3c4b
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    lwz r0, 0x8(r1)
    cmpwi r0, 0x2
    beq @L_8008749C
    bge @L_8008709C
    cmpwi r0, 0x0
    beq @L_800870A8
    bge @L_8008729C
    b @L_8008789C
@L_8008709C:
    cmpwi r0, 0x4
    bge @L_8008789C
    b @L_8008769C
@L_800870A8:
    mr r3, r24
    li r4, 0x3
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800870C8
    li r3, 0x1
    b @L_80087AAC
@L_800870C8:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800870D4:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800870E4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800870F8
    cmpwi r0, 0x4
    bne @L_80087130
@L_800870F8:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087130
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80087130:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80087140
    li r16, 0x1
@L_80087140:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800870E4
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087170
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800870D4
@L_80087170:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800871BC
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800871BC
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800871BC
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800871BC
    li r15, 0x4
@L_800871BC:
    cmpwi r15, 0x3
    ble @L_800871DC
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800871DC
    li r3, 0x0
    b @L_80087230
@L_800871DC:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80087210
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087224
@L_80087210:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087224:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80087230:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087250
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80087250:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80087274
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087288
@L_80087274:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087288:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_8008729C:
    extsb r0, r23
    cmpwi r0, 0x2
    bne @L_8008789C
    mr r3, r24
    li r4, 0x3
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800872C8
    li r3, 0x1
    b @L_80087AAC
@L_800872C8:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800872D4:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800872E4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800872F8
    cmpwi r0, 0x4
    bne @L_80087330
@L_800872F8:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087330
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80087330:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80087340
    li r16, 0x1
@L_80087340:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800872E4
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087370
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800872D4
@L_80087370:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800873BC
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800873BC
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800873BC
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800873BC
    li r15, 0x4
@L_800873BC:
    cmpwi r15, 0x3
    ble @L_800873DC
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800873DC
    li r3, 0x0
    b @L_80087430
@L_800873DC:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80087410
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087424
@L_80087410:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087424:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80087430:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087450
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80087450:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80087474
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087488
@L_80087474:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087488:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_8008749C:
    extsb r0, r23
    cmpwi r0, 0x3
    bne @L_8008789C
    mr r3, r24
    li r4, 0x3
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800874C8
    li r3, 0x1
    b @L_80087AAC
@L_800874C8:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800874D4:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800874E4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800874F8
    cmpwi r0, 0x4
    bne @L_80087530
@L_800874F8:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087530
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80087530:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80087540
    li r16, 0x1
@L_80087540:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800874E4
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087570
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800874D4
@L_80087570:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800875BC
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800875BC
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800875BC
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800875BC
    li r15, 0x4
@L_800875BC:
    cmpwi r15, 0x3
    ble @L_800875DC
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800875DC
    li r3, 0x0
    b @L_80087630
@L_800875DC:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80087610
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087624
@L_80087610:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087624:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80087630:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087650
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80087650:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80087674
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087688
@L_80087674:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087688:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_8008769C:
    extsb r0, r23
    cmpwi r0, 0x0
    bne @L_8008789C
    mr r3, r24
    li r4, 0x3
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800876C8
    li r3, 0x1
    b @L_80087AAC
@L_800876C8:
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800876D4:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800876E4:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800876F8
    cmpwi r0, 0x4
    bne @L_80087730
@L_800876F8:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087730
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80087730:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80087740
    li r16, 0x1
@L_80087740:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800876E4
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087770
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800876D4
@L_80087770:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800877BC
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800877BC
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800877BC
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800877BC
    li r15, 0x4
@L_800877BC:
    cmpwi r15, 0x3
    ble @L_800877DC
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800877DC
    li r3, 0x0
    b @L_80087830
@L_800877DC:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80087810
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087824
@L_80087810:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087824:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80087830:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087850
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80087850:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80087874
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087888
@L_80087874:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087888:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_8008789C:
    lwz r3, 0xc30(r1)
    li r4, 0x7
    lbz r0, 0x1(r3)
    mr r3, r24
    stb r0, 0x21(r24)
    bl fn_80087AE8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087A94
    li r3, 0x1
    bl fn_80103CC0
    li r15, 0x0
@L_800878CC:
    li r16, 0x0
    mr r18, r24
    mr r17, r16
    addi r19, r13, -32464
@L_800878DC:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x5
    beq @L_800878F0
    cmpwi r0, 0x4
    bne @L_80087928
@L_800878F0:
    addi r3, r17, 0x1
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087928
    bl fn_80103CB0
    lbz r0, 0x0(r19)
    andc r0, r3, r0
    rlwinm r3, r0, 0, 24, 31
    bl fn_80103CC0
    li r3, 0x7
    li r0, 0x8
    stw r3, 0x0(r18)
    stw r0, 0x28(r24)
@L_80087928:
    lwz r0, 0x0(r18)
    cmpwi r0, 0x7
    bne @L_80087938
    li r16, 0x1
@L_80087938:
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    addi r17, r17, 0x1
    cmpwi r17, 0x3
    ble @L_800878DC
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087968
    bl fn_800F0308
    addi r15, r15, 0x1
    cmpwi r15, 0xf
    blt @L_800878CC
@L_80087968:
    li r3, 0x26
    bl fn_80166A28
    li r15, 0x0
    lwz r0, 0x0(r24)
    cmpwi r0, 0x7
    beq @L_800879B4
    li r15, 0x1
    lwz r0, 0x4(r24)
    cmpwi r0, 0x7
    beq @L_800879B4
    li r15, 0x2
    lwz r0, 0x8(r24)
    cmpwi r0, 0x7
    beq @L_800879B4
    li r15, 0x3
    lwz r0, 0xc(r24)
    cmpwi r0, 0x7
    beq @L_800879B4
    li r15, 0x4
@L_800879B4:
    cmpwi r15, 0x3
    ble @L_800879D4
    lwz r0, 0x28(r24)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    bne @L_800879D4
    li r3, 0x0
    b @L_80087A28
@L_800879D4:
    li r3, 0x1
    bl fn_80103CC0
    addi r4, r15, 0x1
    li r3, 0x2f
    bl fn_80132A38
    cmpwi r15, 0x0
    bne @L_80087A08
    li r3, 0x7
    li r4, 0x44c0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087A1C
@L_80087A08:
    li r3, 0x7
    li r4, 0x44b8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087A1C:
    li r0, 0x8
    li r3, 0x1
    stw r0, 0x28(r24)
@L_80087A28:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80087A48
    lbz r0, 0x21(r24)
    li r3, 0x6
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    stwx r3, r24, r0
@L_80087A48:
    lwz r0, 0x2c(r24)
    cmpwi r0, 0x3
    bne @L_80087A6C
    li r3, 0x7
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_80087A80
@L_80087A6C:
    li r3, 0x7
    li r4, 0x44e6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_80087A80:
    lbz r3, 0x21(r24)
    extsb r3, r3
    bl fn_80093698
    li r3, 0x0
    b @L_80087AAC
@L_80087A94:
    lwz r4, 0xc30(r1)
    addi r25, r25, 0x1
    addi r4, r4, 0x1
    stw r4, 0xc30(r1)
    cmplwi r25, 0x4
    blt @L_80084DD8
@L_80087AAC:
    psq_l f31, -0x338(r1), 0, 0
    lfd f31, 0xcc0(r1)
    psq_l f30, -0x348(r1), 0, 0
    lfd f30, 0xcb0(r1)
    psq_l f29, -0x358(r1), 0, 0
    lfd f29, 0xca0(r1)
    psq_l f28, -0x368(r1), 0, 0
    lfd f28, 0xc90(r1)
    psq_l f27, -0x378(r1), 0, 0
    lfd f27, 0xc80(r1)
    lmw r15, 0xc3c(r1)
    lwz r0, 0xcd4(r1)
    mtlr r0
    addi r1, r1, 0xcd0
    blr
