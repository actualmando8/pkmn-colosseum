    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    li r0, 0x0
    stmw r24, 0x10(r1)
    mr r27, r5
    rlwinm. r5, r5, 0, 16, 31
    mr r30, r8
    mr r29, r7
    mr r26, r4
    mr r28, r6
    stw r0, 0x0(r8)
    stw r0, 0x0(r7)
    beq @L_801DE150
    cmplwi r5, 0x4
    ble @L_801DDF28
    b @L_801DE150
@L_801DDF28:
    cmplwi r3, 0x0
    beq @L_801DE054
    lbz r0, 0x75(r3)
    cmplwi r0, 0x0
    beq @L_801DE054
    rlwinm. r0, r28, 0, 24, 31
    lhz r31, 0x70(r3)
    beq @L_801DDF7C
    cmplwi r5, 0x4
    beq @L_801DDF7C
    cmplwi r5, 0x2
    beq @L_801DDF7C
    mr r3, r26
    mr r4, r27
    bl fn_801DDC10
    rlwinm r0, r28, 0, 24, 31
    cmpw r0, r3
    bge @L_801DDF7C
    li r25, 0x162
    li r24, 0x16e
    b @L_801DDFD4
@L_801DDF7C:
    rlwinm r0, r27, 0, 16, 31
    cmpwi r0, 0x3
    beq @L_801DDFC0
    bge @L_801DDF9C
    cmpwi r0, 0x1
    beq @L_801DDFA8
    bge @L_801DDFB4
    b @L_801DDFD4
@L_801DDF9C:
    cmpwi r0, 0x5
    bge @L_801DDFD4
    b @L_801DDFCC
@L_801DDFA8:
    li r25, 0x1
    li r24, 0x11c
    b @L_801DDFD4
@L_801DDFB4:
    li r25, 0x11e
    li r24, 0x12a
    b @L_801DDFD4
@L_801DDFC0:
    li r25, 0x12c
    li r24, 0x15e
    b @L_801DDFD4
@L_801DDFCC:
    li r25, 0x160
    li r24, 0x160
@L_801DDFD4:
    lis r3, -32713
    rlwinm r8, r25, 0, 16, 31
    rlwinm r7, r24, 0, 16, 31
    clrlslwi r6, r25, 16, 4
    addi r5, r3, 14160
    rlwinm r4, r26, 0, 16, 31
    subf r0, r8, r7
    rlwinm r3, r28, 0, 24, 31
    add r5, r5, r6
    mtctr r0
    cmplw r8, r7
    bge @L_801DE054
@L_801DE004:
    lhz r0, 0x2(r5)
    cmpw r0, r31
    bne @L_801DE04C
    lhz r0, 0x4(r5)
    cmplw r0, r4
    bne @L_801DE04C
    lbz r0, 0x0(r5)
    cmplw r0, r3
    bne @L_801DE04C
    lwz r0, 0x8(r5)
    lwz r3, 0xc(r5)
    cmplwi r0, 0x0
    beq @L_801DE054
    cmplwi r3, 0x0
    beq @L_801DE054
    stw r0, 0x0(r29)
    stw r3, 0x0(r30)
    b @L_801DE150
@L_801DE04C:
    addi r5, r5, 0x10
    bdnz @L_801DE004
@L_801DE054:
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x3
    ble @L_801DE09C
    rlwinm. r3, r26, 0, 16, 31
    beq @L_801DE150
    lwz r0, -31552(r13)
    cmplw r3, r0
    blt @L_801DE078
    b @L_801DE150
@L_801DE078:
    mulli r4, r3, 0xc
    lis r3, -32713
    addi r3, r3, 10184
    lwzx r0, r3, r4
    add r3, r3, r4
    stw r0, 0x0(r29)
    lwz r0, 0x4(r3)
    stw r0, 0x0(r30)
    b @L_801DE150
@L_801DE09C:
    rlwinm. r5, r26, 0, 16, 31
    subi r0, r27, 0x1
    rlwinm r7, r0, 0, 16, 31
    beq @L_801DE150
    lwz r0, -31584(r13)
    cmplw r5, r0
    blt @L_801DE0BC
    b @L_801DE150
@L_801DE0BC:
    mulli r6, r5, 0x1c
    lis r3, -32713
    clrlslwi r7, r7, 16, 3
    addi r0, r3, -7856
    add r0, r0, r6
    rlwinm. r4, r28, 0, 24, 31
    add r3, r0, r7
    lwz r0, 0x4(r3)
    stw r0, 0x0(r29)
    lwz r0, 0x8(r3)
    stw r0, 0x0(r30)
    beq @L_801DE150
    lis r3, -32713
    lwz r6, -31544(r13)
    addi r0, r3, 12816
    mr r3, r0
    mtctr r6
    cmplwi r6, 0x0
    ble @L_801DE150
@L_801DE108:
    lwz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_801DE148
    lwz r0, 0x4(r3)
    cmplw r0, r4
    bne @L_801DE148
    add r3, r3, r7
    lwz r0, 0x8(r3)
    lwz r3, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801DE150
    cmplwi r3, 0x0
    beq @L_801DE150
    stw r0, 0x0(r29)
    stw r3, 0x0(r30)
    b @L_801DE150
@L_801DE148:
    addi r3, r3, 0x20
    bdnz @L_801DE108
@L_801DE150:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
