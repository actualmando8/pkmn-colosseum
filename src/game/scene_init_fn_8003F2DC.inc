    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r30, r3
    mr r31, r4
    cmpwi r5, 0x0
    bne @L_8003F3A8
    srwi r0, r31, 31
    add r0, r0, r31
    srawi r24, r0, 1
    b @L_8003F39C
    @L_8003F30C:
    mr r25, r24
    slwi r28, r24, 2
    b @L_8003F388
    @L_8003F318:
    subf r26, r24, r25
    slwi r0, r26, 2
    add r29, r30, r0
    b @L_8003F360
    @L_8003F328:
    mr r4, r29
    addi r3, r1, 0x8
    li r5, 0x4
    bl memcpy
    mr r3, r29
    mr r4, r27
    li r5, 0x4
    bl memcpy
    mr r3, r27
    addi r4, r1, 0x8
    li r5, 0x4
    bl memcpy
    subf r29, r28, r29
    subf r26, r24, r26
    @L_8003F360:
    cmpwi r26, 0x0
    blt @L_8003F384
    add r0, r26, r24
    lhz r3, 0x2(r29)
    slwi r0, r0, 2
    add r27, r30, r0
    lhz r0, 0x2(r27)
    cmplw r3, r0
    bgt @L_8003F328
    @L_8003F384:
    addi r25, r25, 0x1
    @L_8003F388:
    cmpw r25, r31
    blt @L_8003F318
    srwi r0, r24, 31
    add r0, r0, r24
    srawi r24, r0, 1
    @L_8003F39C:
    cmpwi r24, 0x0
    bgt @L_8003F30C
    b @L_8003F450
    @L_8003F3A8:
    srwi r0, r31, 31
    add r0, r0, r31
    srawi r26, r0, 1
    b @L_8003F448
    @L_8003F3B8:
    mr r25, r26
    slwi r29, r26, 2
    b @L_8003F434
    @L_8003F3C4:
    subf r24, r26, r25
    slwi r0, r24, 2
    add r28, r30, r0
    b @L_8003F40C
    @L_8003F3D4:
    mr r4, r28
    addi r3, r1, 0x8
    li r5, 0x4
    bl memcpy
    mr r3, r28
    mr r4, r27
    li r5, 0x4
    bl memcpy
    mr r3, r27
    addi r4, r1, 0x8
    li r5, 0x4
    bl memcpy
    subf r28, r29, r28
    subf r24, r26, r24
    @L_8003F40C:
    cmpwi r24, 0x0
    blt @L_8003F430
    add r0, r24, r26
    lhz r3, 0x2(r28)
    slwi r0, r0, 2
    add r27, r30, r0
    lhz r0, 0x2(r27)
    cmplw r3, r0
    blt @L_8003F3D4
    @L_8003F430:
    addi r25, r25, 0x1
    @L_8003F434:
    cmpw r25, r31
    blt @L_8003F3C4
    srwi r0, r26, 31
    add r0, r0, r26
    srawi r26, r0, 1
    @L_8003F448:
    cmpwi r26, 0x0
    bgt @L_8003F3B8
    @L_8003F450:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
