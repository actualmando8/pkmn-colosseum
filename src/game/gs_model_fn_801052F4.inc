    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    li r0, 0x0
    li r31, 0x280
    sth r0, 0x0(r26)
    li r30, 0x1e0
    li r29, 0x0
    li r28, 0x0
    sth r0, 0x0(r27)
@L_8010532C:
    lha r3, 0xa(r25)
    bl fn_8005D858
    lwz r3, 0x10(r25)
    cmplwi r3, 0x0
    beq @L_80105348
    bl fn_800FA444
    b @L_8010534C
@L_80105348:
    li r3, 0x0
@L_8010534C:
    lha r5, 0x2(r25)
    rlwinm r4, r3, 16, 16, 31
    rlwinm r0, r3, 0, 16, 31
    cmpw r31, r5
    extsh r3, r4
    extsh r4, r0
    ble @L_8010536C
    mr r31, r5
@L_8010536C:
    lha r0, 0x6(r25)
    add r0, r5, r0
    cmpw r29, r0
    bge @L_80105380
    mr r29, r0
@L_80105380:
    add r0, r5, r3
    cmpw r29, r0
    bge @L_80105390
    mr r29, r0
@L_80105390:
    lha r3, 0x4(r25)
    cmpw r30, r3
    ble @L_801053A0
    mr r30, r3
@L_801053A0:
    lha r0, 0x8(r25)
    add r0, r3, r0
    cmpw r28, r0
    bge @L_801053B4
    mr r28, r0
@L_801053B4:
    add r0, r3, r4
    cmpw r28, r0
    bge @L_801053C4
    mr r28, r0
@L_801053C4:
    lbz r0, 0x0(r25)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801053E4
    lha r3, 0x18(r25)
    bl fn_8005D934
    mr r25, r3
    b @L_8010532C
@L_801053E4:
    subf r3, r31, r29
    subf r0, r30, r28
    extsh r3, r3
    sth r3, 0x0(r26)
    extsh r0, r0
    sth r0, 0x0(r27)
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
