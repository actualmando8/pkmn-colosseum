    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r25, 0x14(r1)
    mr r26, r3
    lwz r3, 0x80(r3)
    lhz r0, 0x20(r3)
    addi r27, r3, 0x0
    cmplwi r0, 0x0
    bne @L_800B2258
    lhz r3, 0x22(r27)
    lhz r0, 0x8(r26)
    cmplw r3, r0
    beq @L_800B2260
    @L_800B2258:
    li r3, -0x6
    b @L_800B2490
    @L_800B2260:
    li r0, 0x1fc
    srawi r0, r0, 1
    addze. r0, r0
    addi r4, r27, 0x0
    li r7, 0x0
    mr r3, r0
    li r6, 0x0
    ble @L_800B2338
    srwi. r0, r3, 3
    mtctr r0
    beq @L_800B231C
    @L_800B228C:
    lhz r5, 0x0(r4)
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0x2(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0x4(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0x6(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0x8(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0xa(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0xc(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    lhz r5, 0xe(r4)
    add r7, r7, r0
    nor r0, r5, r5
    add r6, r6, r5
    add r7, r7, r0
    addi r4, r4, 0x10
    bdnz @L_800B228C
    andi. r3, r3, 0x7
    beq @L_800B2338
    @L_800B231C:
    mtctr r3
    @L_800B2320:
    lhz r5, 0x0(r4)
    addi r4, r4, 0x2
    nor r0, r5, r5
    add r6, r6, r5
    add r7, r7, r0
    bdnz @L_800B2320
    @L_800B2338:
    clrlwi r0, r6, 16
    cmplwi r0, 0xffff
    bne @L_800B2348
    li r6, 0x0
    @L_800B2348:
    clrlwi r0, r7, 16
    cmplwi r0, 0xffff
    bne @L_800B2358
    li r7, 0x0
    @L_800B2358:
    lhz r3, 0x1fc(r27)
    clrlwi r0, r6, 16
    cmplw r3, r0
    bne @L_800B2378
    lhz r3, 0x1fe(r27)
    clrlwi r0, r7, 16
    cmplw r3, r0
    beq @L_800B2380
    @L_800B2378:
    li r3, -0x6
    b @L_800B2490
    @L_800B2380:
    lwz r31, 0xc(r27)
    lwz r28, 0x10(r27)
    bl __OSLockSramEx
    lis r4, lbl_803FC620@ha
    addi r0, r4, lbl_803FC620@l
    lis r4, 0x7878
    subf r0, r0, r26
    addi r4, r4, 0x7879
    mulhw r0, r4, r0
    srawi r0, r0, 7
    srwi r4, r0, 31
    add r0, r0, r4
    mulli r0, r0, 0xc
    lis r4, 0x41c6
    addi r25, r27, 0x0
    add r29, r3, r0
    addi r30, r4, 0x4e6d
    li r26, 0x0
    @L_800B23C8:
    mullw r5, r31, r30
    mulhwu r3, r28, r30
    li r31, 0x0
    add r5, r5, r3
    mullw r3, r28, r31
    mullw r0, r28, r30
    li r28, 0x3039
    addc r4, r0, r28
    add r0, r5, r3
    adde r3, r0, r31
    li r5, 0x10
    bl fn_800C4C98
    lbz r0, 0x0(r29)
    lbz r6, 0x0(r25)
    addc r0, r4, r0
    clrlwi r0, r0, 24
    cmplw r6, r0
    beq @L_800B2420
    li r3, 0x0
    bl __OSUnlockSramEx
    li r3, -0x6
    b @L_800B2490
    @L_800B2420:
    mullw r5, r3, r30
    mulhwu r3, r4, r30
    add r5, r5, r3
    mullw r3, r4, r31
    mullw r0, r4, r30
    addc r4, r0, r28
    add r0, r5, r3
    adde r3, r0, r31
    li r5, 0x10
    bl fn_800C4C98
    addi r26, r26, 0x1
    cmpwi r26, 0xc
    li r0, 0x7fff
    and r28, r4, r0
    and r31, r3, r31
    addi r29, r29, 0x1
    addi r25, r25, 0x1
    blt @L_800B23C8
    li r3, 0x0
    bl __OSUnlockSramEx
    bl fn_800B016C
    lhz r0, 0x24(r27)
    clrlwi r3, r3, 16
    cmplw r0, r3
    beq @L_800B248C
    li r3, -0xd
    b @L_800B2490
    @L_800B248C:
    li r3, 0x0
    @L_800B2490:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
