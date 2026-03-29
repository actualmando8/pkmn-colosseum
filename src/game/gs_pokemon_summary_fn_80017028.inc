    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r22, 0x38(r1)
    mr r31, r3
    bl fn_80105624
    lis r4, lbl_80266B88@ha
    lfs f1, lbl_8047B748(r2)
    addi r7, r4, lbl_80266B88@l
    lfs f0, lbl_8047A2D4(r13)
    lwz r6, 0x0(r7)
    mr r30, r3
    lwz r5, 0x4(r7)
    fcmpu cr0, f1, f0
    lwz r4, 0x8(r7)
    li r25, 0x0
    lwz r3, 0xc(r7)
    li r27, 0x0
    lwz r0, 0x10(r7)
    stw r6, 0x1c(r1)
    stw r5, 0x20(r1)
    stw r4, 0x24(r1)
    stw r3, 0x28(r1)
    stw r0, 0x2c(r1)
    bne @L_80017098
    lfs f0, lbl_8047A2D0(r13)
    fcmpu cr0, f1, f0
    beq @L_800170A0
    @L_80017098:
    li r3, 0x0
    b @L_80017750
    @L_800170A0:
    lbz r0, 0x95(r31)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    addi r26, r3, 0x1c
    lwzx r0, r26, r0
    clrlwi r3, r0, 16
    bl fn_80103E68
    lbz r0, 0x95(r31)
    lis r4, lbl_80266918@ha
    addi r4, r4, lbl_80266918@l
    srwi r3, r3, 16
    extsb r0, r0
    sth r3, 0x18(r1)
    mulli r0, r0, 0x4c
    addi r29, r4, 0x4
    lwzx r0, r29, r0
    cmpwi r0, 0x0
    blt @L_80017110
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x10
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80017128
    @L_80017110:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80017128:
    mr r24, r3
    li r28, 0x0
    b @L_80017144
    @L_80017134:
    mr r3, r24
    bl fn_801429E8
    addi r28, r28, 0x1
    addi r24, r24, 0x4
    @L_80017144:
    lhz r0, 0x10(r1)
    cmpw r28, r0
    blt @L_80017134
    lwz r0, lbl_8047A2E8(r13)
    lbz r4, 0x18(r1)
    lbz r3, 0x19(r1)
    cmpwi r0, 0x0
    extsb r4, r4
    extsb r0, r3
    add r28, r4, r0
    bge @L_80017268
    lhz r0, 0x4(r30)
    rlwinm r0, r0, 0, 24, 25
    cmpwi r0, 0x0
    beq @L_800173A8
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x3
    beq @L_800173A8
    cmpwi r0, 0x4
    beq @L_800173A8
    lbz r0, 0x95(r31)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r4, r0, 0x4c
    add r3, r3, r4
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    beq @L_800173A8
    lwzx r0, r29, r4
    cmpwi r0, 0x0
    blt @L_800171E4
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xe
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800171FC
    @L_800171E4:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xe
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800171FC:
    mr r22, r3
    li r24, -0x1
    li r23, 0x0
    b @L_80017240
    @L_8001720C:
    mr r3, r22
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017238
    addi r24, r24, 0x1
    cmpw r24, r28
    blt @L_80017238
    mr r3, r22
    bl fn_80143C68
    b @L_80017250
    @L_80017238:
    addi r23, r23, 0x1
    addi r22, r22, 0x4
    @L_80017240:
    lhz r0, 0xe(r1)
    cmpw r23, r0
    blt @L_8001720C
    li r3, 0x0
    @L_80017250:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_800173A8
    stw r28, lbl_8047A2E8(r13)
    li r27, 0x24
    b @L_800173A8
    @L_80017268:
    lhz r3, 0x4(r30)
    andi. r0, r3, 0xd0
    cmpwi r0, 0x0
    beq @L_80017390
    lbz r0, 0x95(r31)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lwzx r0, r29, r0
    cmpwi r0, 0x0
    blt @L_800172B0
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xc
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800172C8
    @L_800172B0:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800172C8:
    mr r24, r3
    li r22, -0x1
    li r23, 0x0
    b @L_8001730C
    @L_800172D8:
    mr r3, r24
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017304
    addi r22, r22, 0x1
    cmpw r22, r28
    blt @L_80017304
    mr r3, r24
    bl fn_80143C68
    b @L_8001731C
    @L_80017304:
    addi r23, r23, 0x1
    addi r24, r24, 0x4
    @L_8001730C:
    lhz r0, 0xc(r1)
    cmpw r23, r0
    blt @L_800172D8
    li r3, 0x0
    @L_8001731C:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80017380
    lbz r0, 0x95(r31)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    lwz r5, lbl_8047A2E8(r13)
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    bne @L_80017364
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r5, 16
    clrlwi r5, r28, 16
    bl fn_80129514
    b @L_80017378
    @L_80017364:
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    clrlwi r5, r5, 16
    clrlwi r6, r28, 16
    bl fn_80129948
    @L_80017378:
    li r27, 0x24
    b @L_80017384
    @L_80017380:
    li r27, 0x25
    @L_80017384:
    li r0, -0x1
    stw r0, lbl_8047A2E8(r13)
    b @L_800173A8
    @L_80017390:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800173A8
    li r0, -0x1
    li r27, 0x25
    stw r0, lbl_8047A2E8(r13)
    @L_800173A8:
    lbz r0, 0x95(r31)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lwzx r0, r26, r0
    clrlwi r3, r0, 16
    bl fn_80103E68
    lbz r0, 0x95(r31)
    srwi r3, r3, 16
    sth r3, 0x18(r1)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lwzx r0, r29, r0
    cmpwi r0, 0x0
    blt @L_80017400
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xa
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80017418
    @L_80017400:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xa
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80017418:
    li r22, 0x0
    mr r24, r3
    mr r23, r22
    b @L_80017448
    @L_80017428:
    mr r3, r24
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017440
    addi r22, r22, 0x1
    @L_80017440:
    addi r23, r23, 0x1
    addi r24, r24, 0x4
    @L_80017448:
    lhz r0, 0xa(r1)
    cmpw r23, r0
    blt @L_80017428
    lhz r3, 0x4(r30)
    addi r7, r22, 0x1
    lhz r0, 0x8(r30)
    or r0, r3, r0
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800174E4
    lbz r4, 0x19(r1)
    lbz r3, 0x18(r1)
    addi r5, r4, 0x1
    clrlwi r6, r5, 24
    extsb r0, r3
    extsb r4, r6
    stb r5, 0x19(r1)
    add r0, r4, r0
    cmpw r0, r7
    blt @L_800174A4
    subi r0, r6, 0x1
    stb r0, 0x19(r1)
    b @L_800174E0
    @L_800174A4:
    cmpwi r4, 0x8
    blt @L_800174CC
    addi r4, r3, 0x1
    subi r3, r6, 0x1
    li r0, 0x1
    stb r4, 0x18(r1)
    li r27, 0x23
    stb r3, 0x19(r1)
    stw r0, lbl_8047A2C8(r13)
    b @L_800174D8
    @L_800174CC:
    li r0, 0x0
    li r27, 0x23
    stw r0, lbl_8047A2C8(r13)
    @L_800174D8:
    lfs f0, lbl_8047B788(r2)
    stfs f0, lbl_8047A2D0(r13)
    @L_800174E0:
    li r25, 0x1
    @L_800174E4:
    lhz r3, 0x4(r30)
    lhz r0, 0x8(r30)
    or r0, r3, r0
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80017570
    lbz r0, 0x19(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_8001751C
    lbz r0, 0x18(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_8001756C
    @L_8001751C:
    lbz r3, 0x19(r1)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x19(r1)
    cmpwi r0, 0x0
    bge @L_80017558
    lbz r3, 0x18(r1)
    li r4, 0x0
    li r0, 0x1
    stb r4, 0x19(r1)
    subi r3, r3, 0x1
    li r27, 0x23
    stb r3, 0x18(r1)
    stw r0, lbl_8047A2C8(r13)
    b @L_80017564
    @L_80017558:
    li r0, 0x0
    li r27, 0x23
    stw r0, lbl_8047A2C8(r13)
    @L_80017564:
    lfs f0, lbl_8047B78C(r2)
    stfs f0, lbl_8047A2D0(r13)
    @L_8001756C:
    li r25, 0x1
    @L_80017570:
    lhz r0, 0x18(r1)
    addi r4, r1, 0x14
    sth r0, 0x14(r1)
    lbz r0, 0x95(r31)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lwzx r0, r26, r0
    clrlwi r3, r0, 16
    bl fn_80103EAC
    lwz r0, lbl_8047A2E8(r13)
    cmpwi r0, 0x0
    bge @L_80017618
    cmpwi r25, 0x0
    bne @L_80017618
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800175E0
    lbz r3, 0x95(r31)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r31)
    cmpwi r0, 0x1
    bge @L_800175D8
    li r0, 0x5
    stb r0, 0x95(r31)
    @L_800175D8:
    lfs f0, lbl_8047B750(r2)
    stfs f0, lbl_8047A2D4(r13)
    @L_800175E0:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80017618
    lbz r3, 0x95(r31)
    addi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r31)
    cmpwi r0, 0x6
    blt @L_80017610
    li r0, 0x1
    stb r0, 0x95(r31)
    @L_80017610:
    lfs f0, lbl_8047B790(r2)
    stfs f0, lbl_8047A2D4(r13)
    @L_80017618:
    cmplwi r27, 0x0
    beq @L_80017634
    mr r3, r27
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    @L_80017634:
    lwz r0, lbl_8047A2E8(r13)
    cmpwi r0, 0x0
    blt @L_80017648
    li r3, 0x2b2b
    b @L_80017748
    @L_80017648:
    lbz r0, 0x95(r31)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lwzx r0, r26, r0
    clrlwi r3, r0, 16
    bl fn_80103E68
    lbz r0, 0x95(r31)
    srwi r3, r3, 16
    sth r3, 0x18(r1)
    extsb r0, r0
    mulli r0, r0, 0x4c
    lbz r4, 0x18(r1)
    lbz r3, 0x19(r1)
    extsb r4, r4
    lwzx r5, r29, r0
    extsb r0, r3
    add r25, r4, r0
    cmpwi r5, 0x0
    blt @L_800176B4
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r5, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800176CC
    @L_800176B4:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800176CC:
    mr r24, r3
    li r22, -0x1
    li r23, 0x0
    b @L_80017710
    @L_800176DC:
    mr r3, r24
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017708
    addi r22, r22, 0x1
    cmpw r22, r25
    blt @L_80017708
    mr r3, r24
    bl fn_80143C68
    b @L_80017720
    @L_80017708:
    addi r23, r23, 0x1
    addi r24, r24, 0x4
    @L_80017710:
    lhz r0, 0x8(r1)
    cmpw r23, r0
    blt @L_800176DC
    li r3, 0x0
    @L_80017720:
    clrlwi r3, r3, 16
    cmplwi r3, 0x0
    beq @L_80017738
    bl fn_801440A0
    bl fn_80143F84
    b @L_80017748
    @L_80017738:
    lwz r0, lbl_8047A2E0(r13)
    addi r3, r1, 0x1c
    slwi r0, r0, 2
    lwzx r3, r3, r0
    @L_80017748:
    stw r3, lbl_8047A2DC(r13)
    li r3, 0x0
    @L_80017750:
    lmw r22, 0x38(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
