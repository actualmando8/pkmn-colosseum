    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r26, r3
    rlwinm. r3, r3, 0, 16, 31
    mr r27, r4
    mr r28, r5
    li r29, 0x0
    beq @L_801DE1C4
    lwz r0, -31568(r13)
    cmplw r3, r0
    blt @L_801DE1CC
@L_801DE1C4:
    li r3, 0x0
    b @L_801DE404
@L_801DE1CC:
    cmplwi r3, 0xc9
    bne @L_801DE20C
    mr r3, r27
    bl fn_80122334
    lwz r5, -31536(r13)
    rlwinm r6, r3, 0, 24, 31
    lis r3, -32713
    divwu r0, r6, r5
    addi r4, r3, 20032
    mullw r0, r0, r5
    subf r0, r0, r6
    clrlslwi r0, r0, 24, 3
    add r3, r4, r0
    lwzx r31, r4, r0
    lwz r30, 0x4(r3)
    b @L_801DE224
@L_801DE20C:
    mulli r0, r3, 0xc
    lis r3, -32713
    addi r4, r3, 3024
    add r3, r4, r0
    lwzx r31, r4, r0
    lwz r30, 0x4(r3)
@L_801DE224:
    cmplwi r31, 0x0
    beq @L_801DE234
    cmplwi r30, 0x0
    bne @L_801DE25C
@L_801DE234:
    lis r3, -32713
    addi r3, r3, 3024
    lwz r31, 0xd38(r3)
    lwz r30, 0xd3c(r3)
    cmplwi r31, 0x0
    beq @L_801DE254
    cmplwi r30, 0x0
    bne @L_801DE25C
@L_801DE254:
    li r3, 0x0
    b @L_801DE404
@L_801DE25C:
    rlwinm. r0, r28, 0, 24, 31
    beq @L_801DE2DC
    lwz r0, -31528(r13)
    lis r3, -32713
    addi r4, r3, 20256
    li r5, 0x0
    rlwinm r3, r26, 0, 16, 31
    li r6, 0x0
    li r7, 0x0
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801DE2C0
@L_801DE28C:
    lhz r0, 0x0(r4)
    cmplw r3, r0
    bne @L_801DE2B4
    mulli r4, r7, 0xc
    lis r3, -32713
    addi r0, r3, 20256
    add r3, r0, r4
    lwz r5, 0x4(r3)
    lwz r6, 0x8(r3)
    b @L_801DE2C0
@L_801DE2B4:
    addi r4, r4, 0xc
    addi r7, r7, 0x1
    bdnz @L_801DE28C
@L_801DE2C0:
    cmplwi r5, 0x0
    beq @L_801DE2DC
    cmplwi r6, 0x0
    beq @L_801DE2DC
    mr r31, r5
    mr r30, r6
    li r29, 0x1
@L_801DE2DC:
    mr r23, r31
    mr r3, r31
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bge @L_801DE2FC
    mr r3, r31
    bl fn_8017B3E4
    b @L_801DE330
@L_801DE2FC:
    mr r3, r31
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bne @L_801DE330
    cmplwi r30, 0x0
    beq @L_801DE330
    mr r3, r31
    mr r4, r30
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DE330
    mr r3, r31
    bl fn_8017B3E4
@L_801DE330:
    lis r3, -32728
    addi r25, r3, -26168
@L_801DE338:
    mr r3, r23
    bl fn_8017B2CC
    mr. r24, r3
    bge @L_801DE354
    mr r3, r25
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DE354:
    cmpwi r24, 0x0
    beq @L_801DE364
    bl fn_800F0308
    b @L_801DE338
@L_801DE364:
    mr r3, r31
    mr r4, r30
    bl fn_800F9318
    mr. r23, r3
    bne @L_801DE380
    li r3, 0x0
    b @L_801DE404
@L_801DE380:
    bl fn_801DB154
    mr. r24, r3
    bne @L_801DE394
    li r3, 0x0
    b @L_801DE404
@L_801DE394:
    sth r26, 0x70(r24)
    li r0, 0x1
    mr r4, r23
    sth r31, 0x72(r24)
    stb r0, 0x75(r24)
    stw r27, 0x78(r24)
    stb r28, 0x77(r24)
    bl fn_801DD45C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DE3CC
    mr r3, r24
    bl fn_801DB100
    li r3, 0x0
    b @L_801DE404
@L_801DE3CC:
    rlwinm. r0, r28, 0, 24, 31
    beq @L_801DE3E4
    rlwinm. r0, r29, 0, 24, 31
    bne @L_801DE3E4
    mr r3, r24
    bl fn_801DCE0C
@L_801DE3E4:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x134
    bne @L_801DE400
    lwz r4, 0x24(r24)
    mr r5, r27
    addi r3, r24, 0x50
    bl fn_80140190
@L_801DE400:
    mr r3, r24
@L_801DE404:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
