    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r4
    lwz r3, 0x8(r3)
    bl fn_8019FF48
    cmplwi r3, 0x0
    li r29, 0x0
    li r28, 0x0
    li r27, 0x0
    li r26, 0x0
    bne @L_8013D0E4
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D0E4:
    beq @L_8013D0F0
    lwz r30, 0xc(r3)
    b @L_8013D0F4
    @L_8013D0F0:
    li r30, 0x0
    @L_8013D0F4:
    cmplwi r3, 0x0
    beq @L_8013D104
    lwz r25, 0x8(r3)
    b @L_8013D108
    @L_8013D104:
    li r25, 0x0
    @L_8013D108:
    cmplwi r25, 0x0
    beq @L_8013D124
    mr r3, r25
    lis r4, 0x4000
    bl fn_801A8440
    mr r3, r25
    bl fn_801A6FF0
    @L_8013D124:
    cmplwi r30, 0x0
    bne @L_8013D134
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D134:
    lwz r3, 0x8(r30)
    cmplwi r3, 0x0
    mr r4, r3
    bne @L_8013D14C
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D14C:
    li r5, 0x1
    b @L_8013D28C
    @L_8013D154:
    cmpwi r0, 0xb
    beq @L_8013D1FC
    bge @L_8013D170
    cmpwi r0, 0x9
    beq @L_8013D17C
    bge @L_8013D1BC
    b @L_8013D27C
    @L_8013D170:
    cmpwi r0, 0xd
    beq @L_8013D23C
    b @L_8013D27C
    @L_8013D17C:
    lwz r0, 0x8(r4)
    mr r29, r5
    cmpwi r0, 0x1
    beq @L_8013D194
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D194:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013D1A8
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D1A8:
    lhz r0, 0x12(r4)
    cmplwi r0, 0xc
    beq @L_8013D284
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D1BC:
    lwz r0, 0x8(r4)
    mr r28, r5
    cmpwi r0, 0x0
    beq @L_8013D1D4
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D1D4:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013D1E8
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D1E8:
    lhz r0, 0x12(r4)
    cmplwi r0, 0xc
    beq @L_8013D284
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D1FC:
    lwz r0, 0x8(r4)
    mr r26, r5
    cmpwi r0, 0x1
    beq @L_8013D214
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D214:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x5
    beq @L_8013D228
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D228:
    lhz r0, 0x12(r4)
    cmplwi r0, 0x4
    beq @L_8013D284
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D23C:
    lwz r0, 0x8(r4)
    mr r27, r5
    cmpwi r0, 0x1
    beq @L_8013D254
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D254:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013D268
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D268:
    lhz r0, 0x12(r4)
    cmplwi r0, 0x8
    beq @L_8013D284
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D27C:
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D284:
    addi r4, r4, 0x18
    addi r5, r5, 0x1
    @L_8013D28C:
    lwz r0, 0x0(r4)
    cmpwi r0, 0xff
    bne @L_8013D154
    cmpwi r29, 0x0
    beq @L_8013D2B8
    cmpwi r28, 0x0
    beq @L_8013D2B8
    cmpwi r26, 0x0
    beq @L_8013D2B8
    cmpwi r27, 0x0
    bne @L_8013D2C0
    @L_8013D2B8:
    li r3, 0x0
    b @L_8013D5F0
    @L_8013D2C0:
    mr r5, r31
    b @L_8013D344
    @L_8013D2C8:
    lwz r4, 0x4(r3)
    li r0, 0x3
    stw r4, 0x20(r5)
    lwz r4, 0x14(r3)
    stw r4, 0x30(r5)
    stw r0, 0x4(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xb
    beq @L_8013D328
    bge @L_8013D300
    cmpwi r0, 0x9
    beq @L_8013D30C
    bge @L_8013D318
    b @L_8013D33C
    @L_8013D300:
    cmpwi r0, 0xd
    beq @L_8013D334
    b @L_8013D33C
    @L_8013D30C:
    lwz r0, 0x4(r31)
    stw r0, 0x14(r3)
    b @L_8013D33C
    @L_8013D318:
    lis r4, lbl_80363CB8@ha
    addi r0, r4, lbl_80363CB8@l
    stw r0, 0x14(r3)
    b @L_8013D33C
    @L_8013D328:
    lwz r0, 0xc(r31)
    stw r0, 0x14(r3)
    b @L_8013D33C
    @L_8013D334:
    lwz r0, 0x8(r31)
    stw r0, 0x14(r3)
    @L_8013D33C:
    addi r3, r3, 0x18
    addi r5, r5, 0x4
    @L_8013D344:
    lwz r0, 0x0(r3)
    cmpwi r0, 0xff
    bne @L_8013D2C8
    lhz r3, 0x1a(r31)
    li r7, 0x0
    lhz r4, 0x18(r31)
    lwz r9, 0x10(r31)
    subi r5, r3, 0x1
    slwi r10, r4, 1
    slwi r11, r3, 3
    b @L_8013D5B8
    @L_8013D370:
    li r0, 0x98
    cmpwi r4, 0x0
    stb r0, 0x0(r9)
    addi r8, r9, 0x3
    li r6, 0x0
    sth r10, 0x1(r9)
    ble @L_8013D5B0
    cmpwi r4, 0x8
    subi r12, r4, 0x8
    ble @L_8013D55C
    addi r0, r12, 0x7
    mr r9, r6
    srwi r0, r0, 3
    mtctr r0
    cmpwi r12, 0x0
    ble @L_8013D55C
    @L_8013D3B0:
    addi r29, r9, 0x1
    addi r27, r6, 0x1
    add r29, r7, r29
    addi r26, r6, 0x2
    sth r29, 0x0(r8)
    li r0, 0x0
    mullw r27, r3, r27
    addi r12, r6, 0x3
    sth r0, 0x2(r8)
    subi r28, r29, 0x1
    sth r29, 0x4(r8)
    sth r29, 0x6(r8)
    addi r24, r27, 0x1
    sth r28, 0x8(r8)
    add r24, r7, r24
    mullw r26, r3, r26
    sth r0, 0xa(r8)
    subi r27, r24, 0x1
    sth r28, 0xc(r8)
    sth r28, 0xe(r8)
    addi r25, r26, 0x1
    add r25, r7, r25
    sth r24, 0x10(r8)
    subi r26, r25, 0x1
    mullw r12, r3, r12
    sth r0, 0x12(r8)
    sth r24, 0x14(r8)
    sth r24, 0x16(r8)
    addi r24, r12, 0x1
    add r24, r7, r24
    sth r27, 0x18(r8)
    sth r0, 0x1a(r8)
    sth r27, 0x1c(r8)
    sth r27, 0x1e(r8)
    sth r25, 0x20(r8)
    sth r0, 0x22(r8)
    sth r25, 0x24(r8)
    sth r25, 0x26(r8)
    sth r26, 0x28(r8)
    sth r0, 0x2a(r8)
    sth r26, 0x2c(r8)
    sth r26, 0x2e(r8)
    sth r24, 0x30(r8)
    sth r0, 0x32(r8)
    sth r24, 0x34(r8)
    addi r12, r6, 0x4
    mullw r28, r3, r12
    addi r27, r6, 0x5
    sth r24, 0x36(r8)
    subi r29, r24, 0x1
    addi r26, r6, 0x6
    sth r29, 0x38(r8)
    sth r0, 0x3a(r8)
    addi r12, r6, 0x7
    addi r25, r28, 0x1
    add r9, r9, r11
    sth r29, 0x3c(r8)
    add r25, r7, r25
    mullw r27, r3, r27
    addi r6, r6, 0x8
    sth r29, 0x3e(r8)
    subi r28, r25, 0x1
    sth r25, 0x40(r8)
    sth r0, 0x42(r8)
    addi r24, r27, 0x1
    sth r25, 0x44(r8)
    add r24, r7, r24
    mullw r26, r3, r26
    sth r25, 0x46(r8)
    subi r27, r24, 0x1
    sth r28, 0x48(r8)
    sth r0, 0x4a(r8)
    addi r29, r26, 0x1
    add r29, r7, r29
    sth r28, 0x4c(r8)
    mullw r12, r3, r12
    subi r26, r29, 0x1
    sth r28, 0x4e(r8)
    sth r24, 0x50(r8)
    addi r25, r12, 0x1
    sth r0, 0x52(r8)
    add r25, r7, r25
    subi r12, r25, 0x1
    sth r24, 0x54(r8)
    sth r24, 0x56(r8)
    sth r27, 0x58(r8)
    sth r0, 0x5a(r8)
    sth r27, 0x5c(r8)
    sth r27, 0x5e(r8)
    sth r29, 0x60(r8)
    sth r0, 0x62(r8)
    sth r29, 0x64(r8)
    sth r29, 0x66(r8)
    sth r26, 0x68(r8)
    sth r0, 0x6a(r8)
    sth r26, 0x6c(r8)
    sth r26, 0x6e(r8)
    sth r25, 0x70(r8)
    sth r0, 0x72(r8)
    sth r25, 0x74(r8)
    sth r25, 0x76(r8)
    sth r12, 0x78(r8)
    sth r0, 0x7a(r8)
    sth r12, 0x7c(r8)
    sth r12, 0x7e(r8)
    addi r8, r8, 0x80
    bdnz @L_8013D3B0
    @L_8013D55C:
    mullw r12, r6, r3
    subf r0, r6, r4
    li r9, 0x0
    mtctr r0
    cmpw r6, r4
    bge @L_8013D5B0
    @L_8013D574:
    addi r25, r12, 0x1
    add r12, r12, r3
    add r25, r7, r25
    addi r6, r6, 0x1
    sth r25, 0x0(r8)
    subi r0, r25, 0x1
    sth r9, 0x2(r8)
    sth r25, 0x4(r8)
    sth r25, 0x6(r8)
    sth r0, 0x8(r8)
    sth r9, 0xa(r8)
    sth r0, 0xc(r8)
    sth r0, 0xe(r8)
    addi r8, r8, 0x10
    bdnz @L_8013D574
    @L_8013D5B0:
    mr r9, r8
    addi r7, r7, 0x1
    @L_8013D5B8:
    cmpw r7, r5
    blt @L_8013D370
    lwz r4, 0x10(r30)
    li r0, 0x1
    li r3, 0x1
    stw r4, 0x40(r31)
    lhz r4, 0xe(r30)
    sth r4, 0x44(r31)
    lwz r4, 0x10(r31)
    stw r4, 0x10(r30)
    lwz r4, 0x14(r31)
    srawi r4, r4, 5
    sth r4, 0xe(r30)
    stb r0, 0x46(r31)
    @L_8013D5F0:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
