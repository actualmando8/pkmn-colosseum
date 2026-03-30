    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r30, r3
    cmplwi r30, 0x0
    li r21, 0x0
    beq @L_80103470
    lbz r0, 0x1a(r30)
    cmplwi r0, 0x2
    bne @L_80102F68
    li r21, 0x1
@L_80102F68:
    lwz r3, 0x4(r30)
    bl fn_8005DA18
    mr r22, r3
    mr r3, r30
    bl fn_80104318
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80102F94
    li r0, 0x0
    stb r0, 0x95(r30)
    b @L_80103470
@L_80102F94:
    li r25, 0x10
    li r24, 0xc
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r23, r0, 0, 31, 31
    rlwinm r27, r0, 0, 30, 30
    rlwinm r28, r0, 0, 29, 29
    rlwinm r29, r0, 0, 28, 28
@L_80102FB4:
    lha r3, 0x4(r22)
    li r26, 0x0
    bl fn_8005D934
    cmpwi r23, 0x0
    beq @L_80103040
    li r20, 0x1e0
@L_80102FCC:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80103024
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_80103020
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103008
    neg r0, r0
@L_80103008:
    cmpw r0, r25
    bge @L_80103020
    cmpw r20, r5
    ble @L_80103020
    mr r20, r5
    stb r26, 0x95(r30)
@L_80103020:
    addi r26, r26, 0x1
@L_80103024:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801031C0
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80102FCC
@L_80103040:
    cmpwi r27, 0x0
    beq @L_801030C0
    li r20, 0x1e0
@L_8010304C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801030A4
    lha r4, 0x4(r31)
    lha r0, 0x4(r3)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_801030A0
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103088
    neg r0, r0
@L_80103088:
    cmpw r0, r25
    bge @L_801030A0
    cmpw r20, r5
    ble @L_801030A0
    mr r20, r5
    stb r26, 0x95(r30)
@L_801030A0:
    addi r26, r26, 0x1
@L_801030A4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801031C0
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_8010304C
@L_801030C0:
    cmpwi r28, 0x0
    beq @L_80103140
    li r20, 0x280
@L_801030CC:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80103124
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_80103120
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103108
    neg r0, r0
@L_80103108:
    cmpw r0, r24
    bge @L_80103120
    cmpw r20, r5
    ble @L_80103120
    mr r20, r5
    stb r26, 0x95(r30)
@L_80103120:
    addi r26, r26, 0x1
@L_80103124:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801031C0
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_801030CC
@L_80103140:
    cmpwi r29, 0x0
    beq @L_801031C0
    li r20, 0x280
@L_8010314C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801031A4
    lha r4, 0x2(r31)
    lha r0, 0x2(r3)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_801031A0
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103188
    neg r0, r0
@L_80103188:
    cmpw r0, r24
    bge @L_801031A0
    cmpw r20, r5
    ble @L_801031A0
    mr r20, r5
    stb r26, 0x95(r30)
@L_801031A0:
    addi r26, r26, 0x1
@L_801031A4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801031C0
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_8010314C
@L_801031C0:
    lbz r3, 0x97(r30)
    lbz r0, 0x95(r30)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    bne @L_80103208
    addi r25, r25, 0x10
    addi r24, r24, 0xc
    cmpwi r25, 0x280
    blt @L_801031EC
    li r25, 0x280
@L_801031EC:
    cmpwi r24, 0x1e0
    blt @L_801031F8
    li r24, 0x1e0
@L_801031F8:
    cmpwi r25, 0x280
    bne @L_80102FB4
    cmpwi r24, 0x1e0
    bne @L_80102FB4
@L_80103208:
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_80103470
    li r24, 0x10
    li r25, 0xc
@L_8010321C:
    lbz r3, 0x97(r30)
    lbz r0, 0x95(r30)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    bne @L_80103470
    lha r3, 0x4(r22)
    bl fn_8005D934
    cmpwi r23, 0x0
    li r26, 0x0
    beq @L_801032C0
    mr r21, r26
@L_8010324C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801032A4
    lha r4, 0x4(r31)
    lha r0, 0x4(r3)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_801032A0
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103288
    neg r0, r0
@L_80103288:
    cmpw r0, r24
    bge @L_801032A0
    cmpw r21, r5
    bge @L_801032A0
    mr r21, r5
    stb r26, 0x95(r30)
@L_801032A0:
    addi r26, r26, 0x1
@L_801032A4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80103440
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_8010324C
@L_801032C0:
    cmpwi r27, 0x0
    beq @L_80103340
    mr r21, r26
@L_801032CC:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80103324
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_80103320
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103308
    neg r0, r0
@L_80103308:
    cmpw r0, r24
    bge @L_80103320
    cmpw r21, r5
    bge @L_80103320
    mr r21, r5
    stb r26, 0x95(r30)
@L_80103320:
    addi r26, r26, 0x1
@L_80103324:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80103440
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_801032CC
@L_80103340:
    cmpwi r28, 0x0
    beq @L_801033C0
    mr r21, r26
@L_8010334C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801033A4
    lha r4, 0x2(r31)
    lha r0, 0x2(r3)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_801033A0
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103388
    neg r0, r0
@L_80103388:
    cmpw r0, r25
    bge @L_801033A0
    cmpw r21, r5
    bge @L_801033A0
    mr r21, r5
    stb r26, 0x95(r30)
@L_801033A0:
    addi r26, r26, 0x1
@L_801033A4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80103440
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_8010334C
@L_801033C0:
    cmpwi r29, 0x0
    beq @L_80103440
    mr r21, r26
@L_801033CC:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80103424
    lha r4, 0x2(r3)
    lha r0, 0x2(r31)
    subf r5, r4, r0
    cmpwi r5, 0x0
    ble @L_80103420
    lha r4, 0x4(r3)
    lha r0, 0x4(r31)
    subf r0, r4, r0
    cmpwi r0, 0x0
    bge @L_80103408
    neg r0, r0
@L_80103408:
    cmpw r0, r25
    bge @L_80103420
    cmpw r21, r5
    bge @L_80103420
    mr r21, r5
    stb r26, 0x95(r30)
@L_80103420:
    addi r26, r26, 0x1
@L_80103424:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80103440
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_801033CC
@L_80103440:
    addi r24, r24, 0x10
    addi r25, r25, 0xc
    cmpwi r24, 0x280
    blt @L_80103454
    li r24, 0x280
@L_80103454:
    cmpwi r25, 0x1e0
    blt @L_80103460
    li r25, 0x1e0
@L_80103460:
    cmpwi r24, 0x280
    bne @L_8010321C
    cmpwi r25, 0x1e0
    bne @L_8010321C
@L_80103470:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
