    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    lis r3, -32704
    li r29, 0x0
    addi r30, r3, -19584
    lhz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_80096FD4
    li r28, 0x5
    b @L_80096FD8
@L_80096FD4:
    li r28, 0x4
@L_80096FD8:
    bl fn_80105624
    lis r4, -32704
    lhz r5, 0x6(r3)
    addi r31, r4, -19584
    lbz r0, 0x1(r31)
    cmplwi r0, 0x8
    bgt @L_800973D8
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -3968
    lwzx r0, r3, r0
    mtctr r0
    bctr
    rlwinm r3, r5, 0, 16, 31
    lbz r4, 0x95(r27)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80097028
    addi r4, r4, 0x1
    b @L_80097038
@L_80097028:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80097038
    subi r4, r4, 0x1
@L_80097038:
    extsb r0, r4
    cmpwi r0, 0x2
    ble @L_80097048
    li r4, 0x2
@L_80097048:
    extsb r0, r4
    cmpwi r0, 0x0
    bge @L_80097058
    li r4, 0x0
@L_80097058:
    extsb r0, r4
    stb r4, 0x95(r27)
    cmpwi r0, 0x1
    beq @L_80097090
    bge @L_80097078
    cmpwi r0, 0x0
    bge @L_80097084
    b @L_800970A4
@L_80097078:
    cmpwi r0, 0x3
    bge @L_800970A4
    b @L_8009709C
@L_80097084:
    li r0, 0x1
    stb r0, 0x1(r31)
    b @L_800970A4
@L_80097090:
    li r0, 0x2
    stb r0, 0x1(r31)
    b @L_800970A4
@L_8009709C:
    li r0, 0x5
    stb r0, 0x1(r31)
@L_800970A4:
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_800970B8
    li r29, 0x1
    b @L_800970C8
@L_800970B8:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800970C8
    li r29, 0x2
@L_800970C8:
    cmpwi r29, 0x0
    beq @L_800973D8
    lis r3, -32704
    addi r28, r3, -19584
    lwz r12, 0x10(r28)
    cmplwi r12, 0x0
    beq @L_800973D8
    mr r4, r29
    lwz r3, 0xc(r28)
    lwz r5, 0x14(r28)
    mtctr r12
    bctrl
    lwz r0, 0xc(r28)
    mr r25, r3
    cmplw r0, r25
    beq @L_800973D8
    cmplwi r25, 0x0
    beq @L_8009712C
    lwz r3, 0x4(r27)
    li r4, 0x1
    bl fn_80103484
    lis r3, -32704
    mr r4, r25
    addi r3, r3, -19656
    bl fn_80109C88
@L_8009712C:
    stw r25, 0xc(r28)
    b @L_800973D8
    rlwinm r4, r5, 0, 16, 31
    lis r3, -32704
    rlwinm r0, r4, 0, 31, 31
    addi r31, r3, -19584
    cmpwi r0, 0x0
    lbz r29, 0x2(r31)
    beq @L_80097158
    subi r29, r29, 0x1
    b @L_80097168
@L_80097158:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80097168
    addi r29, r29, 0x1
@L_80097168:
    extsb r3, r29
    extsb r0, r28
    cmpw r3, r0
    blt @L_80097180
    subi r0, r28, 0x1
    extsb r29, r0
@L_80097180:
    extsb r0, r29
    cmpwi r0, 0x0
    bge @L_80097190
    li r29, 0x0
@L_80097190:
    extsb r25, r29
    lis r3, -32704
    rlwinm r0, r25, 0, 16, 31
    addi r3, r3, -19584
    cmplwi r0, 0x4
    lwz r26, 0xc(r3)
    bne @L_800971B4
    lhz r28, 0x18(r30)
    b @L_800971E8
@L_800971B4:
    mr r3, r26
    mr r6, r25
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r26
    mr r4, r25
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800971E8
    li r28, 0x0
@L_800971E8:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_800973D8
    lbz r0, 0x2(r31)
    extsb r3, r29
    extsb r0, r0
    cmpw r3, r0
    beq @L_800973D8
    lwz r3, 0x4(r27)
    li r4, 0x1
    bl fn_80103484
    stb r29, 0x2(r31)
    b @L_800973D8
    lbz r0, 0x1a(r31)
    lis r3, 0x38e4
    rlwinm r8, r5, 0, 16, 31
    extsb r7, r0
    subi r3, r3, 0x71c7
    mulhw r6, r3, r7
    rlwinm r3, r8, 0, 31, 31
    cmpwi r3, 0x0
    srawi r4, r6, 1
    rlwinm r5, r4, 1, 31, 31
    srawi r3, r6, 1
    add r4, r4, r5
    mulli r5, r4, 0x9
    rlwinm r4, r3, 1, 31, 31
    add r4, r3, r4
    subf r3, r5, r7
    beq @L_800972B8
    mr r8, r4
    b @L_800972A8
@L_80097268:
    add r6, r31, r8
    rlwinm r5, r3, 2, 0, 29
    mr r7, r3
    add r6, r6, r5
@L_80097278:
    lbz r5, 0x20(r6)
    extsb r5, r5
    cmpwi r5, 0x0
    blt @L_80097298
    mr r4, r8
    mr r3, r7
    li r8, -0x1
    b @L_800972A8
@L_80097298:
    cmpwi r7, 0x0
    subi r6, r6, 0x4
    subi r7, r7, 0x1
    bgt @L_80097278
@L_800972A8:
    cmpwi r8, 0x0
    subi r8, r8, 0x1
    bgt @L_80097268
    b @L_800973B0
@L_800972B8:
    rlwinm r5, r8, 0, 30, 30
    cmpwi r5, 0x0
    beq @L_8009731C
    mr r7, r4
    b @L_8009730C
@L_800972CC:
    add r6, r31, r7
    rlwinm r5, r3, 2, 0, 29
    mr r8, r3
    add r6, r6, r5
@L_800972DC:
    lbz r5, 0x20(r6)
    extsb r5, r5
    cmpwi r5, 0x0
    blt @L_800972FC
    mr r4, r7
    mr r3, r8
    li r7, 0x5
    b @L_8009730C
@L_800972FC:
    cmpwi r8, 0x0
    subi r6, r6, 0x4
    subi r8, r8, 0x1
    bgt @L_800972DC
@L_8009730C:
    addi r7, r7, 0x1
    cmpwi r7, 0x4
    blt @L_800972CC
    b @L_800973B0
@L_8009731C:
    rlwinm r5, r8, 0, 28, 28
    cmpwi r5, 0x0
    beq @L_80097368
    mr r7, r3
    addi r3, r3, 0x1
    cmpwi r3, 0x9
    blt @L_8009733C
    li r3, 0x8
@L_8009733C:
    lis r6, -32704
    rlwinm r5, r3, 2, 0, 29
    addi r6, r6, -19584
    add r5, r6, r5
    add r5, r5, r4
    lbz r5, 0x20(r5)
    extsb r5, r5
    cmpwi r5, 0x0
    bge @L_800973B0
    mr r3, r7
    b @L_800973B0
@L_80097368:
    rlwinm r5, r8, 0, 29, 29
    cmpwi r5, 0x0
    beq @L_800973B0
    mr r7, r3
    subi r3, r3, 0x1
    cmpwi r3, 0x0
    bge @L_80097388
    li r3, 0x0
@L_80097388:
    lis r6, -32704
    rlwinm r5, r3, 2, 0, 29
    addi r6, r6, -19584
    add r5, r6, r5
    add r5, r5, r4
    lbz r5, 0x20(r5)
    extsb r5, r5
    cmpwi r5, 0x0
    bge @L_800973B0
    mr r3, r7
@L_800973B0:
    mulli r4, r4, 0x9
    extsb r0, r0
    add r3, r3, r4
    extsb r25, r3
    cmpw r25, r0
    beq @L_800973D8
    lwz r3, 0x4(r27)
    li r4, 0x1
    bl fn_80103484
    stb r25, 0x1a(r31)
@L_800973D8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
