    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x120(r1)
    stmw r22, 0xf8(r1)
    addi r24, r3, 0x0
    lis r3, lbl_803FC620@ha
    mulli r5, r24, 0x110
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    addi r23, r4, 0x0
    addi r30, r31, 0x30
    lwz r3, 0x80(r31)
    addi r0, r3, 0x2f
    clrrwi r28, r0, 5
    addi r29, r3, 0x0
    addi r22, r28, 0x20
    bl OSGetTick
    stw r3, lbl_80478A50(r13)
    lis r3, 0x41c6
    lis r5, 0x7fed
    lwz r4, lbl_80478A50(r13)
    addi r0, r3, 0x4e6d
    addi r25, r5, -0x8000
    mullw r3, r4, r0
    addi r0, r3, 0x3039
    stw r0, lbl_80478A50(r13)
    lwz r0, lbl_80478A50(r13)
    extrwi r0, r0, 15, 1
    or r25, r25, r0
    clrrwi r25, r25, 12
    bl fn_800B07D8
    addi r26, r3, 0x0
    addi r6, r26, 0x0
    addi r3, r24, 0x0
    addi r4, r25, 0x0
    addi r5, r1, 0xa4
    li r7, 0x0
    bl fn_800B0694
    cmpwi r3, 0x0
    bge @L_800B0944
    li r3, -0x3
    b @L_800B13E0
    @L_800B0944:
    slwi r3, r26, 3
    addi r4, r3, 0x1
    cmplwi r4, 0x0
    li r3, 0x0
    ble @L_800B0ABC
    cmplwi r4, 0x8
    subi r5, r4, 0x8
    ble @L_800B0A84
    addi r0, r5, 0x7
    srwi r0, r0, 3
    cmplwi r5, 0x0
    mtctr r0
    ble @L_800B0A84
    @L_800B0978:
    srwi r0, r25, 7
    srwi r5, r25, 15
    xor r0, r25, r0
    xor r0, r5, r0
    srwi r6, r25, 23
    eqv r0, r6, r0
    srwi r5, r25, 1
    clrlslwi r0, r0, 31, 30
    or r7, r5, r0
    srwi r0, r7, 7
    srwi r5, r7, 15
    xor r0, r7, r0
    srwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r8, r0, 31, 30
    rlwimi r8, r7, 31, 2, 31
    srwi r0, r8, 7
    srwi r5, r8, 15
    xor r0, r8, r0
    srwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r7, r0, 31, 30
    rlwimi r7, r8, 31, 2, 31
    srwi r0, r7, 7
    srwi r5, r7, 15
    xor r0, r7, r0
    srwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r8, r0, 31, 30
    rlwimi r8, r7, 31, 2, 31
    srwi r0, r8, 7
    srwi r5, r8, 15
    xor r0, r8, r0
    srwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r7, r0, 31, 30
    rlwimi r7, r8, 31, 2, 31
    srwi r0, r7, 7
    srwi r5, r7, 15
    xor r0, r7, r0
    srwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r8, r0, 31, 30
    rlwimi r8, r7, 31, 2, 31
    srwi r0, r8, 7
    srwi r5, r8, 15
    xor r0, r8, r0
    srwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r7, r0, 31, 30
    rlwimi r7, r8, 31, 2, 31
    srwi r0, r7, 7
    srwi r5, r7, 15
    xor r0, r7, r0
    srwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    clrlslwi r25, r0, 31, 30
    rlwimi r25, r7, 31, 2, 31
    addi r3, r3, 0x8
    bdnz @L_800B0978
    @L_800B0A84:
    subf r0, r3, r4
    cmplw r3, r4
    mtctr r0
    bge @L_800B0ABC
    @L_800B0A94:
    srwi r0, r25, 7
    srwi r3, r25, 15
    xor r0, r25, r0
    srwi r4, r25, 23
    xor r0, r3, r0
    eqv r0, r4, r0
    srwi r3, r25, 1
    clrlslwi r0, r0, 31, 30
    or r25, r3, r0
    bdnz @L_800B0A94
    @L_800B0ABC:
    srwi r0, r25, 7
    srwi r3, r25, 15
    xor r0, r25, r0
    srwi r4, r25, 23
    xor r0, r3, r0
    eqv r0, r4, r0
    slwi r0, r0, 31
    or r0, r25, r0
    stw r0, 0x2c(r31)
    lwz r3, 0x2c(r31)
    bl fn_800B0528
    stw r3, 0x2c(r31)
    bl fn_800B07D8
    addi r27, r3, 0x0
    addi r6, r27, 0x14
    addi r3, r24, 0x0
    addi r5, r1, 0xa4
    li r4, 0x0
    li r7, 0x1
    bl fn_800B0694
    cmpwi r3, 0x0
    bge @L_800B0B1C
    li r3, -0x3
    b @L_800B13E0
    @L_800B0B1C:
    li r3, 0x4
    lwz r7, 0x2c(r31)
    lwz r25, 0xa4(r1)
    mtctr r3
    lwz r26, 0xa8(r1)
    lwz r24, 0xac(r1)
    xor r25, r25, r7
    lwz r0, 0xb0(r1)
    lwz r5, 0xb4(r1)
    @L_800B0B40:
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    xor r3, r4, r3
    slwi r6, r7, 23
    eqv r3, r6, r3
    slwi r4, r7, 1
    rlwinm r3, r3, 2, 30, 30
    or r7, r4, r3
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r9, r3, 2, 30, 30
    rlwimi r9, r7, 1, 0, 29
    slwi r3, r9, 7
    slwi r4, r9, 15
    xor r3, r9, r3
    slwi r6, r9, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    bdnz @L_800B0B40
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    srwi r3, r3, 31
    or r3, r7, r3
    stw r3, 0x2c(r31)
    li r3, 0x4
    mtctr r3
    lwz r7, 0x2c(r31)
    xor r26, r26, r7
    @L_800B0C7C:
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    xor r3, r4, r3
    slwi r6, r7, 23
    eqv r3, r6, r3
    slwi r4, r7, 1
    rlwinm r3, r3, 2, 30, 30
    or r7, r4, r3
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r9, r3, 2, 30, 30
    rlwimi r9, r7, 1, 0, 29
    slwi r3, r9, 7
    slwi r4, r9, 15
    xor r3, r9, r3
    slwi r6, r9, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    bdnz @L_800B0C7C
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    srwi r3, r3, 31
    or r3, r7, r3
    stw r3, 0x2c(r31)
    li r3, 0x4
    mtctr r3
    lwz r7, 0x2c(r31)
    xor r24, r24, r7
    @L_800B0DB8:
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    xor r3, r4, r3
    slwi r6, r7, 23
    eqv r3, r6, r3
    slwi r4, r7, 1
    rlwinm r3, r3, 2, 30, 30
    or r7, r4, r3
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r9, r3, 2, 30, 30
    rlwimi r9, r7, 1, 0, 29
    slwi r3, r9, 7
    slwi r4, r9, 15
    xor r3, r9, r3
    slwi r6, r9, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    bdnz @L_800B0DB8
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    srwi r3, r3, 31
    or r3, r7, r3
    stw r3, 0x2c(r31)
    li r3, 0x4
    mtctr r3
    lwz r7, 0x2c(r31)
    xor r0, r0, r7
    @L_800B0EF4:
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    xor r3, r4, r3
    slwi r6, r7, 23
    eqv r3, r6, r3
    slwi r4, r7, 1
    rlwinm r3, r3, 2, 30, 30
    or r7, r4, r3
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r9, r3, 2, 30, 30
    rlwimi r9, r7, 1, 0, 29
    slwi r3, r9, 7
    slwi r4, r9, 15
    xor r3, r9, r3
    slwi r6, r9, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    slwi r6, r7, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r8, r3, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r3, r8, 7
    slwi r4, r8, 15
    xor r3, r8, r3
    slwi r6, r8, 23
    xor r3, r4, r3
    eqv r3, r6, r3
    rlwinm r7, r3, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    bdnz @L_800B0EF4
    slwi r3, r7, 7
    slwi r4, r7, 15
    xor r3, r7, r3
    xor r3, r4, r3
    slwi r6, r7, 23
    eqv r3, r6, r3
    srwi r3, r3, 31
    or r3, r7, r3
    stw r3, 0x2c(r31)
    slwi r4, r27, 3
    cmplwi r4, 0x0
    lwz r6, 0x2c(r31)
    li r3, 0x0
    xor r5, r5, r6
    ble @L_800B119C
    cmplwi r4, 0x8
    subi r8, r4, 0x8
    ble @L_800B1164
    addi r7, r8, 0x7
    srwi r7, r7, 3
    cmplwi r8, 0x0
    mtctr r7
    ble @L_800B1164
    @L_800B1058:
    slwi r7, r6, 7
    slwi r8, r6, 15
    xor r7, r6, r7
    xor r7, r8, r7
    slwi r9, r6, 23
    eqv r7, r9, r7
    slwi r8, r6, 1
    rlwinm r6, r7, 2, 30, 30
    or r9, r8, r6
    slwi r6, r9, 7
    slwi r7, r9, 15
    xor r6, r9, r6
    slwi r8, r9, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r10, r6, 2, 30, 30
    rlwimi r10, r9, 1, 0, 29
    slwi r6, r10, 7
    slwi r7, r10, 15
    xor r6, r10, r6
    slwi r8, r10, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r9, r6, 2, 30, 30
    rlwimi r9, r10, 1, 0, 29
    slwi r6, r9, 7
    slwi r7, r9, 15
    xor r6, r9, r6
    slwi r8, r9, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r10, r6, 2, 30, 30
    rlwimi r10, r9, 1, 0, 29
    slwi r6, r10, 7
    slwi r7, r10, 15
    xor r6, r10, r6
    slwi r8, r10, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r9, r6, 2, 30, 30
    rlwimi r9, r10, 1, 0, 29
    slwi r6, r9, 7
    slwi r7, r9, 15
    xor r6, r9, r6
    slwi r8, r9, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r10, r6, 2, 30, 30
    rlwimi r10, r9, 1, 0, 29
    slwi r6, r10, 7
    slwi r7, r10, 15
    xor r6, r10, r6
    slwi r8, r10, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r9, r6, 2, 30, 30
    rlwimi r9, r10, 1, 0, 29
    slwi r6, r9, 7
    slwi r7, r9, 15
    xor r6, r9, r6
    slwi r8, r9, 23
    xor r6, r7, r6
    eqv r6, r8, r6
    rlwinm r6, r6, 2, 30, 30
    rlwimi r6, r9, 1, 0, 29
    addi r3, r3, 0x8
    bdnz @L_800B1058
    @L_800B1164:
    subf r7, r3, r4
    cmplw r3, r4
    mtctr r7
    bge @L_800B119C
    @L_800B1174:
    slwi r3, r6, 7
    slwi r4, r6, 15
    xor r3, r6, r3
    slwi r7, r6, 23
    xor r3, r4, r3
    eqv r3, r7, r3
    slwi r4, r6, 1
    rlwinm r3, r3, 2, 30, 30
    or r6, r4, r3
    bdnz @L_800B1174
    @L_800B119C:
    slwi r3, r6, 7
    slwi r4, r6, 15
    xor r3, r6, r3
    slwi r7, r6, 23
    xor r3, r4, r3
    eqv r3, r7, r3
    srwi r3, r3, 31
    or r3, r6, r3
    stw r3, 0x2c(r31)
    li r3, 0x4
    mtctr r3
    li r3, 0x0
    lwz r4, 0x2c(r31)
    @L_800B11D0:
    slwi r6, r4, 7
    slwi r7, r4, 15
    xor r6, r4, r6
    xor r6, r7, r6
    slwi r8, r4, 23
    eqv r6, r8, r6
    slwi r7, r4, 1
    rlwinm r4, r6, 2, 30, 30
    or r8, r7, r4
    slwi r4, r8, 7
    slwi r6, r8, 15
    xor r4, r8, r4
    slwi r7, r8, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r9, r4, 2, 30, 30
    rlwimi r9, r8, 1, 0, 29
    slwi r4, r9, 7
    slwi r6, r9, 15
    xor r4, r9, r4
    slwi r7, r9, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r8, r4, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r4, r8, 7
    slwi r6, r8, 15
    xor r4, r8, r4
    slwi r7, r8, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r9, r4, 2, 30, 30
    rlwimi r9, r8, 1, 0, 29
    slwi r4, r9, 7
    slwi r6, r9, 15
    xor r4, r9, r4
    slwi r7, r9, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r8, r4, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r4, r8, 7
    slwi r6, r8, 15
    xor r4, r8, r4
    slwi r7, r8, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r9, r4, 2, 30, 30
    rlwimi r9, r8, 1, 0, 29
    slwi r4, r9, 7
    slwi r6, r9, 15
    xor r4, r9, r4
    slwi r7, r9, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r8, r4, 2, 30, 30
    rlwimi r8, r9, 1, 0, 29
    slwi r4, r8, 7
    slwi r6, r8, 15
    xor r4, r8, r4
    slwi r7, r8, 23
    xor r4, r6, r4
    eqv r4, r7, r4
    rlwinm r4, r4, 2, 30, 30
    rlwimi r4, r8, 1, 0, 29
    addi r3, r3, 0x8
    bdnz @L_800B11D0
    subfic r6, r3, 0x21
    cmplwi r3, 0x21
    mtctr r6
    bge @L_800B1314
    @L_800B12EC:
    slwi r3, r4, 7
    slwi r6, r4, 15
    xor r3, r4, r3
    slwi r7, r4, 23
    xor r3, r6, r3
    eqv r3, r7, r3
    slwi r4, r4, 1
    rlwinm r3, r3, 2, 30, 30
    or r4, r4, r3
    bdnz @L_800B12EC
    @L_800B1314:
    slwi r3, r4, 7
    slwi r6, r4, 15
    xor r3, r4, r3
    slwi r7, r4, 23
    xor r3, r6, r3
    eqv r3, r7, r3
    srwi r3, r3, 31
    or r3, r4, r3
    stw r3, 0x2c(r31)
    li r6, 0x8
    li r27, 0x0
    stw r0, 0x0(r28)
    addi r3, r28, 0x0
    li r4, 0x8
    stw r5, 0x4(r28)
    stw r28, 0x0(r29)
    stw r6, 0x4(r29)
    stw r22, 0xc(r29)
    stw r27, 0x8(r29)
    bl DCFlushRange
    addi r3, r22, 0x0
    li r4, 0x4
    bl DCInvalidateRange
    addi r3, r29, 0x0
    li r4, 0x10
    bl DCFlushRange
    li r0, 0xff
    lis r3, lbl_80312800@ha
    stw r0, 0x4(r30)
    addi r3, r3, lbl_80312800@l
    addis r0, r3, 0x8000
    stw r0, 0xc(r30)
    li r0, 0x160
    lis r4, fn_800B13F4@ha
    stw r0, 0x10(r30)
    lis r3, fn_800B1464@ha
    li r5, 0x10
    stw r27, 0x14(r30)
    addi r4, r4, fn_800B13F4@l
    addi r0, r3, fn_800B1464@l
    sth r5, 0x24(r30)
    mr r3, r30
    stw r4, 0x28(r30)
    stw r27, 0x2c(r30)
    stw r0, 0x30(r30)
    stw r27, 0x34(r30)
    bl fn_800AE93C
    stw r25, 0x0(r23)
    li r3, 0x0
    stw r26, 0x4(r23)
    stw r24, 0x8(r23)
    @L_800B13E0:
    lmw r22, 0xf8(r1)
    lwz r0, 0x124(r1)
    addi r1, r1, 0x120
    mtlr r0
    blr
