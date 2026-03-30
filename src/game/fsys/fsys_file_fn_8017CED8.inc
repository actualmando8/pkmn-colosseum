    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x1
    stw r0, 0xc4(r1)
    stmw r17, 0x84(r1)
    mr r31, r3
    lwz r23, 0x1c(r3)
    lwz r3, 0xf8(r3)
    bl fn_8017F794
    mr. r21, r3
    bne @L_8017D110
    addi r0, r23, 0x1f
    lwz r3, 0x40(r31)
    clrrwi r4, r0, 5
    bl DCFlushRange
    bl fn_8017FA5C
    addi r0, r23, 0x1f
    mr r18, r3
    clrrwi r0, r0, 5
    cmplw r0, r18
    ble @L_8017D0DC
    li r0, 0x0
    lwz r25, lbl_8047B1B8(r13)
    stw r0, 0x5c(r1)
    b @L_8017CF6C
    @L_8017CF40:
    lwz r3, 0x0(r25)
    lwz r0, 0x8(r31)
    cmpw r3, r0
    bne @L_8017CF5C
    lwz r0, 0x0(r25)
    stw r0, 0x60(r1)
    b @L_8017CF84
    @L_8017CF5C:
    lwz r3, 0x5c(r1)
    addi r25, r25, 0x8
    addi r0, r3, 0x1
    stw r0, 0x5c(r1)
    @L_8017CF6C:
    lwz r3, 0x5c(r1)
    lwz r0, lbl_8047B1BC(r13)
    cmpw r3, r0
    blt @L_8017CF40
    li r0, -0x1
    stw r0, 0x60(r1)
    @L_8017CF84:
    lwz r0, 0x60(r1)
    cmpwi r0, 0x0
    bge @L_8017D0DC
    li r0, 0x0
    stw r0, 0x74(r1)
    @L_8017CF98:
    lwz r0, lbl_8047B1B8(r13)
    stw r0, 0x54(r1)
    lwz r3, 0x54(r1)
    lwz r0, 0x0(r3)
    stw r0, 0x50(r1)
    lwz r0, 0x50(r1)
    stw r0, 0x58(r1)
    lwz r19, 0x58(r1)
    cmpwi r19, 0x0
    blt @L_8017D0DC
    mr r3, r19
    bl fn_8017F800
    li r0, 0x0
    stw r19, 0x40(r1)
    lwz r30, lbl_8047B1B8(r13)
    li r28, 0x0
    stw r0, 0x44(r1)
    b @L_8017D010
    @L_8017CFE0:
    lwz r3, 0x0(r30)
    lwz r0, 0x40(r1)
    cmpw r3, r0
    bne @L_8017D008
    li r3, -0x1
    li r0, 0x1
    stw r3, 0x0(r30)
    stw r0, 0x44(r1)
    stw r28, 0x48(r1)
    b @L_8017D01C
    @L_8017D008:
    addi r30, r30, 0x8
    addi r28, r28, 0x1
    @L_8017D010:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r28, r0
    blt @L_8017CFE0
    @L_8017D01C:
    lwz r0, 0x44(r1)
    cmpwi r0, 0x0
    beq @L_8017D0A0
    lwz r30, lbl_8047B1B8(r13)
    li r28, 0x0
    b @L_8017D068
    @L_8017D034:
    lwz r0, 0x48(r1)
    cmpw r28, r0
    blt @L_8017D064
    addi r3, r28, 0x1
    slwi r0, r28, 3
    slwi r3, r3, 3
    add r4, r30, r3
    add r5, r30, r0
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x0(r5)
    stw r0, 0x4(r5)
    @L_8017D064:
    addi r28, r28, 0x1
    @L_8017D068:
    lwz r3, lbl_8047B1BC(r13)
    subi r0, r3, 0x1
    cmpw r28, r0
    blt @L_8017D034
    lwz r4, lbl_8047B1BC(r13)
    li r0, 0x0
    li r3, -0x1
    stw r0, 0x4c(r1)
    subi r0, r4, 0x1
    stw r0, lbl_8047B1BC(r13)
    lwz r0, lbl_8047B1BC(r13)
    slwi r0, r0, 3
    stwx r3, r30, r0
    b @L_8017D0A8
    @L_8017D0A0:
    li r0, -0x1
    stw r0, 0x4c(r1)
    @L_8017D0A8:
    lwz r0, 0x4c(r1)
    cmpwi r0, 0x0
    blt @L_8017D0DC
    bl fn_8017FA5C
    addi r0, r23, 0x1f
    mr r18, r3
    clrrwi r0, r0, 5
    cmplw r0, r18
    ble @L_8017D0DC
    lwz r3, 0x74(r1)
    addi r0, r3, 0x1
    stw r0, 0x74(r1)
    b @L_8017CF98
    @L_8017D0DC:
    addi r0, r23, 0x1f
    lwz r4, 0xf8(r31)
    clrrwi r3, r0, 5
    li r5, 0x0
    li r6, 0x1
    bl fn_8017F928
    mr. r21, r3
    beq @L_8017D110
    addi r0, r23, 0x1f
    lwz r3, 0x40(r31)
    mr r4, r21
    clrrwi r5, r0, 5
    bl fn_80180450
    @L_8017D110:
    bl fn_8017FA5C
    stw r3, 0x6c(r1)
    lwz r4, 0x20(r31)
    lwz r0, 0x6c(r1)
    cmplw r4, r0
    ble @L_8017D2D4
    li r0, 0x0
    lwz r24, lbl_8047B1B8(r13)
    stw r0, 0x38(r1)
    b @L_8017D164
    @L_8017D138:
    lwz r3, 0x0(r24)
    lwz r0, 0x8(r31)
    cmpw r3, r0
    bne @L_8017D154
    lwz r0, 0x0(r24)
    stw r0, 0x3c(r1)
    b @L_8017D17C
    @L_8017D154:
    lwz r3, 0x38(r1)
    addi r24, r24, 0x8
    addi r0, r3, 0x1
    stw r0, 0x38(r1)
    @L_8017D164:
    lwz r3, 0x38(r1)
    lwz r0, lbl_8047B1BC(r13)
    cmpw r3, r0
    blt @L_8017D138
    li r0, -0x1
    stw r0, 0x3c(r1)
    @L_8017D17C:
    lwz r0, 0x3c(r1)
    cmpwi r0, 0x0
    bge @L_8017D2D4
    li r0, 0x0
    stw r0, 0x70(r1)
    @L_8017D190:
    lwz r0, lbl_8047B1B8(r13)
    stw r0, 0x30(r1)
    lwz r3, 0x30(r1)
    lwz r0, 0x0(r3)
    stw r0, 0x2c(r1)
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    lwz r17, 0x34(r1)
    cmpwi r17, 0x0
    blt @L_8017D2D4
    mr r3, r17
    bl fn_8017F800
    li r0, 0x0
    stw r17, 0x1c(r1)
    lwz r29, lbl_8047B1B8(r13)
    li r27, 0x0
    stw r0, 0x20(r1)
    b @L_8017D208
    @L_8017D1D8:
    lwz r3, 0x0(r29)
    lwz r0, 0x1c(r1)
    cmpw r3, r0
    bne @L_8017D200
    li r3, -0x1
    li r0, 0x1
    stw r3, 0x0(r29)
    stw r0, 0x20(r1)
    stw r27, 0x24(r1)
    b @L_8017D214
    @L_8017D200:
    addi r29, r29, 0x8
    addi r27, r27, 0x1
    @L_8017D208:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r27, r0
    blt @L_8017D1D8
    @L_8017D214:
    lwz r0, 0x20(r1)
    cmpwi r0, 0x0
    beq @L_8017D298
    lwz r29, lbl_8047B1B8(r13)
    li r27, 0x0
    b @L_8017D260
    @L_8017D22C:
    lwz r0, 0x24(r1)
    cmpw r27, r0
    blt @L_8017D25C
    addi r3, r27, 0x1
    slwi r0, r27, 3
    slwi r3, r3, 3
    add r4, r29, r3
    add r5, r29, r0
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x0(r5)
    stw r0, 0x4(r5)
    @L_8017D25C:
    addi r27, r27, 0x1
    @L_8017D260:
    lwz r3, lbl_8047B1BC(r13)
    subi r0, r3, 0x1
    cmpw r27, r0
    blt @L_8017D22C
    lwz r4, lbl_8047B1BC(r13)
    li r0, 0x0
    li r3, -0x1
    stw r0, 0x28(r1)
    subi r0, r4, 0x1
    stw r0, lbl_8047B1BC(r13)
    lwz r0, lbl_8047B1BC(r13)
    slwi r0, r0, 3
    stwx r3, r29, r0
    b @L_8017D2A0
    @L_8017D298:
    li r0, -0x1
    stw r0, 0x28(r1)
    @L_8017D2A0:
    lwz r0, 0x28(r1)
    cmpwi r0, 0x0
    blt @L_8017D2D4
    bl fn_8017FA5C
    stw r3, 0x6c(r1)
    lwz r4, 0x20(r31)
    lwz r0, 0x6c(r1)
    cmplw r4, r0
    ble @L_8017D2D4
    lwz r3, 0x70(r1)
    addi r0, r3, 0x1
    stw r0, 0x70(r1)
    b @L_8017D190
    @L_8017D2D4:
    mr r3, r31
    bl fn_8017D68C
    lwz r0, 0xc(r31)
    li r20, 0x0
    stw r0, 0x64(r1)
    b @L_8017D36C
    @L_8017D2EC:
    lwz r22, 0x40(r31)
    stw r20, 0x8(r1)
    cmplwi r22, 0x0
    beq @L_8017D33C
    stw r22, 0xc(r1)
    lwz r0, 0x8(r1)
    lwz r3, 0xc(r1)
    slwi r0, r0, 2
    lwz r3, 0x18(r3)
    add r3, r22, r3
    stw r3, 0x10(r1)
    lwz r3, 0x10(r1)
    lwz r3, 0x0(r3)
    add r3, r22, r3
    stw r3, 0x14(r1)
    lwz r3, 0x14(r1)
    lwzx r0, r3, r0
    add r0, r22, r0
    stw r0, 0x18(r1)
    b @L_8017D344
    @L_8017D33C:
    li r0, 0x0
    stw r0, 0x18(r1)
    @L_8017D344:
    lwz r5, 0x18(r1)
    li r4, 0x0
    li r3, 0x0
    li r0, 0x0
    stw r5, 0x68(r1)
    addi r20, r20, 0x1
    lwz r5, 0x68(r1)
    stw r4, 0x2c(r5)
    stw r3, 0x28(r5)
    stw r0, 0x30(r5)
    @L_8017D36C:
    lwz r0, 0x64(r1)
    cmpw r20, r0
    blt @L_8017D2EC
    li r3, 0x1
    li r0, 0x64
    stw r3, 0x5c(r31)
    li r3, 0x0
    stw r0, 0x48(r31)
    lmw r17, 0x84(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
