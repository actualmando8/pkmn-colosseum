    nofralloc
    mflr r0
    lis r4, lbl_803FC620@ha
    stw r0, 0x4(r1)
    addi r4, r4, lbl_803FC620@l
    addi r0, r4, 0x30
    stwu r1, -0x88(r1)
    cmplw r0, r3
    stw r31, 0x84(r1)
    li r31, 0x0
    stw r30, 0x80(r1)
    stw r29, 0x7c(r1)
    addi r29, r4, 0x0
    stw r28, 0x78(r1)
    beq @L_800B14B8
    addi r0, r4, 0x140
    cmplw r0, r3
    addi r4, r4, 0x110
    addi r29, r4, 0x0
    li r31, 0x1
    beq @L_800B14B8
    li r31, 0x2
    @L_800B14B8:
    lwz r3, 0x80(r29)
    addi r0, r3, 0x2f
    clrrwi r3, r0, 5
    lwz r30, 0x20(r3)
    bl fn_800B07D8
    lwz r0, 0x2c(r29)
    addi r28, r3, 0x0
    addi r6, r28, 0x0
    xor r0, r30, r0
    clrrwi r4, r0, 16
    addi r3, r31, 0x0
    addi r5, r1, 0x34
    li r7, 0x1
    bl fn_800B0694
    cmpwi r3, 0x0
    bge @L_800B1510
    mr r3, r31
    bl EXIUnlock
    addi r3, r31, 0x0
    li r4, -0x3
    bl fn_800B3604
    b @L_800B1768
    @L_800B1510:
    lwz r4, 0x14(r29)
    li r3, 0x0
    lwz r0, 0x2c(r29)
    add r4, r28, r4
    addi r4, r4, 0x4
    slwi r4, r4, 3
    addi r4, r4, 0x1
    cmplwi r4, 0x0
    ble @L_800B1698
    cmplwi r4, 0x8
    subi r6, r4, 0x8
    ble @L_800B1660
    addi r5, r6, 0x7
    srwi r5, r5, 3
    cmplwi r6, 0x0
    mtctr r5
    ble @L_800B1660
    @L_800B1554:
    slwi r5, r0, 7
    slwi r6, r0, 15
    xor r5, r0, r5
    xor r5, r6, r5
    slwi r7, r0, 23
    eqv r5, r7, r5
    slwi r6, r0, 1
    rlwinm r0, r5, 2, 30, 30
    or r7, r6, r0
    slwi r0, r7, 7
    slwi r5, r7, 15
    xor r0, r7, r0
    slwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r8, r0, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r0, r8, 7
    slwi r5, r8, 15
    xor r0, r8, r0
    slwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r7, r0, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r0, r7, 7
    slwi r5, r7, 15
    xor r0, r7, r0
    slwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r8, r0, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r0, r8, 7
    slwi r5, r8, 15
    xor r0, r8, r0
    slwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r7, r0, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r0, r7, 7
    slwi r5, r7, 15
    xor r0, r7, r0
    slwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r8, r0, 2, 30, 30
    rlwimi r8, r7, 1, 0, 29
    slwi r0, r8, 7
    slwi r5, r8, 15
    xor r0, r8, r0
    slwi r6, r8, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r7, r0, 2, 30, 30
    rlwimi r7, r8, 1, 0, 29
    slwi r0, r7, 7
    slwi r5, r7, 15
    xor r0, r7, r0
    slwi r6, r7, 23
    xor r0, r5, r0
    eqv r0, r6, r0
    rlwinm r0, r0, 2, 30, 30
    rlwimi r0, r7, 1, 0, 29
    addi r3, r3, 0x8
    bdnz @L_800B1554
    @L_800B1660:
    subf r5, r3, r4
    cmplw r3, r4
    mtctr r5
    bge @L_800B1698
    @L_800B1670:
    slwi r3, r0, 7
    slwi r4, r0, 15
    xor r3, r0, r3
    slwi r5, r0, 23
    xor r3, r4, r3
    eqv r3, r5, r3
    slwi r4, r0, 1
    rlwinm r0, r3, 2, 30, 30
    or r0, r4, r0
    bdnz @L_800B1670
    @L_800B1698:
    slwi r3, r0, 7
    slwi r4, r0, 15
    xor r3, r0, r3
    slwi r5, r0, 23
    xor r3, r4, r3
    eqv r3, r5, r3
    srwi r3, r3, 31
    or r0, r0, r3
    stw r0, 0x2c(r29)
    bl fn_800B07D8
    lwz r0, 0x2c(r29)
    slwi r4, r30, 16
    addi r6, r3, 0x0
    xor r0, r4, r0
    clrrwi r4, r0, 16
    addi r3, r31, 0x0
    addi r5, r1, 0x34
    li r7, 0x1
    bl fn_800B0694
    cmpwi r3, 0x0
    bge @L_800B1704
    mr r3, r31
    bl EXIUnlock
    addi r3, r31, 0x0
    li r4, -0x3
    bl fn_800B3604
    b @L_800B1768
    @L_800B1704:
    addi r3, r31, 0x0
    addi r4, r1, 0x30
    bl fn_800AF660
    addi r28, r3, 0x0
    addi r3, r31, 0x0
    bl fn_80098944
    cmpwi r3, 0x0
    bne @L_800B173C
    mr r3, r31
    bl EXIUnlock
    addi r3, r31, 0x0
    li r4, -0x3
    bl fn_800B3604
    b @L_800B1768
    @L_800B173C:
    cmpwi r28, 0x0
    bne @L_800B175C
    lbz r0, 0x30(r1)
    rlwinm. r0, r0, 0, 25, 25
    bne @L_800B175C
    mr r3, r31
    bl EXIUnlock
    li r28, -0x5
    @L_800B175C:
    addi r3, r31, 0x0
    addi r4, r28, 0x0
    bl fn_800B3604
    @L_800B1768:
    lwz r0, 0x8c(r1)
    lwz r31, 0x84(r1)
    lwz r30, 0x80(r1)
    lwz r29, 0x7c(r1)
    lwz r28, 0x78(r1)
    addi r1, r1, 0x88
    mtlr r0
    blr
