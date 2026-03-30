    nofralloc
    stwu r1, -0x3b0(r1)
    mflr r0
    stw r0, 0x3b4(r1)
    stfd f31, 0x3a0(r1)
    psq_st f31, 0x3a8(r1), 0, 0
    stfd f30, 0x390(r1)
    psq_st f30, 0x398(r1), 0, 0
    stmw r23, 0x36c(r1)
    mr r31, r3
    li r30, 0x0
@L_8009381C:
    mr r3, r31
    bl fn_8009F7B4
    lwz r0, 0x4340(r31)
    cmpwi r0, 0xd
    beq @L_80093858
    stw r30, 0x433c(r31)
    li r0, 0x0
    stw r0, 0x4340(r31)
    b @L_8009384C
@L_80093840:
    mr r4, r31
    addi r3, r31, 0x18
    bl fn_8009F9E8
@L_8009384C:
    lwz r0, 0x4340(r31)
    cmpwi r0, 0x0
    beq @L_80093840
@L_80093858:
    lwz r28, 0x4340(r31)
    mr r3, r31
    bl fn_8009F890
    cmplwi r28, 0xd
    li r29, 0x0
    bgt @L_80093ACC
    lis r3, -32721
    rlwinm r0, r28, 2, 0, 29
    addi r3, r3, -4880
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lbz r0, 0x43c4(r31)
    addi r3, r31, 0x4344
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_800938A4
    addi r4, r31, 0x43c4
    b @L_800938A8
@L_800938A4:
    li r4, 0x0
@L_800938A8:
    bl fn_80073E8C
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093ACC
    b @L_800938CC
@L_800938BC:
    bl fn_800A13F8
    li r4, 0x10
    bl fn_800A257C
    bl fn_800A1990
@L_800938CC:
    bl fn_80073E84
    cmpwi r3, 0x0
    beq @L_800938BC
    li r30, 0x1
    b @L_80093ACC
    bl OSGetTime
    lwz r0, 0x4344(r31)
    mr r23, r4
    mr r24, r3
    cmpwi r0, 0x0
    bne @L_80093900
    lfs f30, -29872(r2)
    b @L_80093904
@L_80093900:
    lfs f30, -29872(r2)
@L_80093904:
    lfd f31, -29864(r2)
    lis r26, 0x8000
    lis r27, 0x4330
    b @L_8009396C
@L_80093914:
    cmpwi r29, 0x3e8
    beq @L_80093ACC
    bl OSGetTime
    lwz r0, 0xf8(r26)
    subfc r25, r23, r4
    stw r27, 0x358(r1)
    subfe r3, r24, r3
    rlwinm r0, r0, 30, 2, 31
    stw r0, 0x35c(r1)
    lfd f0, 0x358(r1)
    fsubs f0, f0, f31
    fmuls f1, f30, f0
    bl fn_800C4CC0
    cmpw r4, r25
    bgt @L_8009395C
    lis r3, 0x2
    addi r30, r3, 0x2
    b @L_80093ACC
@L_8009395C:
    bl fn_800A13F8
    li r4, 0x10
    bl fn_800A257C
    bl fn_800A1990
@L_8009396C:
    lwz r3, 0x4338(r31)
    bl fn_80074324
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093914
    lwz r3, 0x4338(r31)
    lwz r4, 0x4344(r31)
    bl fn_800745B4
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093ACC
    li r30, 0x2
    b @L_80093ACC
    lwz r3, 0x4338(r31)
    addi r4, r1, 0xe0
    bl fn_80073690
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093ACC
    lwz r3, 0x4344(r31)
    addi r4, r1, 0xe0
    bl fn_800895A4
    lbz r3, 0xe3(r1)
    li r30, 0x4
    lbz r0, 0xe2(r1)
    lbz r5, 0xe1(r1)
    rlwinm r4, r3, 24, 0, 7
    rlwinm r0, r0, 16, 0, 15
    lbz r6, 0xe0(r1)
    rlwinm r5, r5, 8, 0, 23
    lwz r3, 0x4348(r31)
    or r0, r4, r0
    or r0, r5, r0
    or r0, r6, r0
    stw r0, 0x0(r3)
    b @L_80093ACC
    li r30, 0x5
    b @L_80093ACC
    li r30, 0x6
    b @L_80093ACC
    li r30, 0x7
    b @L_80093ACC
    li r30, 0x8
    b @L_80093ACC
    li r30, 0x9
    b @L_80093ACC
    li r30, 0xa
    b @L_80093ACC
    lwz r3, 0x4338(r31)
    addi r4, r1, 0x8
    bl fn_80071E34
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093ACC
    lwz r3, 0x4344(r31)
    addi r4, r1, 0x8
    bl fn_80089380
    li r30, 0xb
    b @L_80093ACC
    lwz r3, 0x4338(r31)
    addi r4, r31, 0x4344
    addi r3, r3, 0x1
    bl fn_80089D30
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093ACC
@L_80093A74:
    lwz r3, 0x4338(r31)
    addi r3, r3, 0x1
    bl fn_80089CA8
    mr r29, r3
    cmpwi r29, 0x0
    bne @L_80093A9C
    lwz r3, 0x4338(r31)
    addi r3, r3, 0x1
    bl fn_80089C84
    mr r29, r3
@L_80093A9C:
    cmpwi r29, 0x0
    bge @L_80093AB8
    bl fn_800A13F8
    li r4, 0x10
    bl fn_800A257C
    bl fn_800A1990
    b @L_80093A74
@L_80093AB8:
    bne @L_80093ACC
    li r30, 0xc
    b @L_80093ACC
    li r3, 0x0
    b @L_80093AE0
@L_80093ACC:
    cmpwi r29, 0x0
    beq @L_8009381C
    rlwinm r0, r28, 0, 16, 31
    oris r30, r0, 0x1
    b @L_8009381C
@L_80093AE0:
    psq_l f31, 0x3a8(r1), 0, 0
    lfd f31, 0x3a0(r1)
    psq_l f30, 0x398(r1), 0, 0
    lfd f30, 0x390(r1)
    lmw r23, 0x36c(r1)
    lwz r0, 0x3b4(r1)
    mtlr r0
    addi r1, r1, 0x3b0
    blr
