    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r4, lbl_80271068@ha
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r27, r3
    addi r31, r4, lbl_80271068@l
    lwz r3, 0x14(r3)
    addi r0, r3, 0x1
    stw r0, 0x14(r27)
    lwz r4, 0x14(r27)
    lbz r28, 0x0(r3)
    addi r0, r4, 0x1
    stw r0, 0x14(r27)
    lwz r3, 0x14(r27)
    lbz r30, 0x0(r4)
    lhz r29, 0x0(r3)
    addi r3, r3, 0x2
    rlwinm. r0, r30, 0, 24, 24
    stw r3, 0x14(r27)
    beq @L_800F22CC
    addi r3, r31, 0x48
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, 0x1
    b @L_800F24E0
    @L_800F22CC:
    rlwinm. r0, r28, 0, 24, 24
    beq @L_800F22F4
    lwz r0, 0x28(r27)
    subf r0, r29, r0
    stw r0, 0x28(r27)
    lwz r0, 0x28(r27)
    slwi r3, r0, 2
    addi r28, r3, 0x6c
    add r28, r27, r28
    b @L_800F2364
    @L_800F22F4:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2318
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0xc(r1)
    b @L_800F2330
    @L_800F2318:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F2330:
    lwz r4, 0xc(r1)
    rlwinm. r0, r28, 0, 25, 25
    stw r4, 0x18(r1)
    beq @L_800F2358
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r28, r3, 0x6c
    add r28, r27, r28
    b @L_800F2364
    @L_800F2358:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r28, r3, r0
    @L_800F2364:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2388
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x8(r1)
    b @L_800F23A0
    @L_800F2388:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F23A0:
    lwz r4, 0x8(r1)
    rlwinm. r0, r30, 0, 25, 25
    stw r4, 0x14(r1)
    beq @L_800F23C8
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r6, r3, 0x6c
    add r6, r27, r6
    b @L_800F23D4
    @L_800F23C8:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r6, r3, r0
    @L_800F23D4:
    cmpwi r29, 0x0
    li r7, 0x0
    ble @L_800F2488
    cmpwi r29, 0x8
    subi r3, r29, 0x8
    ble @L_800F2458
    addi r0, r3, 0x7
    mr r4, r28
    srwi r0, r0, 3
    mr r5, r6
    mtctr r0
    cmpwi r3, 0x0
    ble @L_800F2458
    @L_800F2408:
    lwz r0, 0x0(r4)
    addi r7, r7, 0x8
    stw r0, 0x0(r5)
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
    lwz r0, 0x8(r4)
    stw r0, 0x8(r5)
    lwz r0, 0xc(r4)
    stw r0, 0xc(r5)
    lwz r0, 0x10(r4)
    stw r0, 0x10(r5)
    lwz r0, 0x14(r4)
    stw r0, 0x14(r5)
    lwz r0, 0x18(r4)
    stw r0, 0x18(r5)
    lwz r0, 0x1c(r4)
    addi r4, r4, 0x20
    stw r0, 0x1c(r5)
    addi r5, r5, 0x20
    bdnz @L_800F2408
    @L_800F2458:
    slwi r4, r7, 2
    subf r0, r7, r29
    add r3, r28, r4
    add r4, r6, r4
    mtctr r0
    cmpw r7, r29
    bge @L_800F2488
    @L_800F2474:
    lwz r0, 0x0(r3)
    addi r3, r3, 0x4
    stw r0, 0x0(r4)
    addi r4, r4, 0x4
    bdnz @L_800F2474
    @L_800F2488:
    mr r28, r6
    li r30, 0x0
    b @L_800F24D4
    @L_800F2494:
    lwz r3, 0x28(r27)
    lwz r4, 0x0(r28)
    cmpwi r3, 0x40
    stw r4, 0x10(r1)
    ble @L_800F24B8
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F24CC
    @L_800F24B8:
    addi r0, r3, 0x1
    slwi r3, r3, 2
    stw r0, 0x28(r27)
    addi r0, r3, 0x6c
    stwx r4, r27, r0
    @L_800F24CC:
    addi r28, r28, 0x4
    addi r30, r30, 0x1
    @L_800F24D4:
    cmpw r30, r29
    blt @L_800F2494
    li r3, 0x1
    @L_800F24E0:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
