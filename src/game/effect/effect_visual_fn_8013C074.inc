    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r26, r4
    lwz r3, 0x8(r3)
    bl fn_8019FF48
    cmplwi r3, 0x0
    li r30, 0x0
    li r29, 0x0
    li r28, 0x0
    li r27, 0x0
    bne @L_8013C0B0
    li r3, 0x0
    b @L_8013C58C
    @L_8013C0B0:
    beq @L_8013C0BC
    lwz r31, 0xc(r3)
    b @L_8013C0C0
    @L_8013C0BC:
    li r31, 0x0
    @L_8013C0C0:
    cmplwi r3, 0x0
    beq @L_8013C0D0
    lwz r25, 0x8(r3)
    b @L_8013C0D4
    @L_8013C0D0:
    li r25, 0x0
    @L_8013C0D4:
    cmplwi r25, 0x0
    beq @L_8013C0F0
    mr r3, r25
    lis r4, 0x4000
    bl fn_801A8440
    mr r3, r25
    bl fn_801A6FF0
    @L_8013C0F0:
    cmplwi r31, 0x0
    bne @L_8013C100
    li r3, 0x0
    b @L_8013C58C
    @L_8013C100:
    lwz r3, 0x8(r31)
    cmplwi r3, 0x0
    mr r4, r3
    bne @L_8013C118
    li r3, 0x0
    b @L_8013C58C
    @L_8013C118:
    li r5, 0x1
    b @L_8013C258
    @L_8013C120:
    cmpwi r0, 0xb
    beq @L_8013C1C8
    bge @L_8013C13C
    cmpwi r0, 0x9
    beq @L_8013C148
    bge @L_8013C188
    b @L_8013C248
    @L_8013C13C:
    cmpwi r0, 0xd
    beq @L_8013C208
    b @L_8013C248
    @L_8013C148:
    lwz r0, 0x8(r4)
    mr r30, r5
    cmpwi r0, 0x1
    beq @L_8013C160
    li r3, 0x0
    b @L_8013C58C
    @L_8013C160:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013C174
    li r3, 0x0
    b @L_8013C58C
    @L_8013C174:
    lhz r0, 0x12(r4)
    cmplwi r0, 0xc
    beq @L_8013C250
    li r3, 0x0
    b @L_8013C58C
    @L_8013C188:
    lwz r0, 0x8(r4)
    mr r29, r5
    cmpwi r0, 0x0
    beq @L_8013C1A0
    li r3, 0x0
    b @L_8013C58C
    @L_8013C1A0:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013C1B4
    li r3, 0x0
    b @L_8013C58C
    @L_8013C1B4:
    lhz r0, 0x12(r4)
    cmplwi r0, 0xc
    beq @L_8013C250
    li r3, 0x0
    b @L_8013C58C
    @L_8013C1C8:
    lwz r0, 0x8(r4)
    mr r27, r5
    cmpwi r0, 0x1
    beq @L_8013C1E0
    li r3, 0x0
    b @L_8013C58C
    @L_8013C1E0:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x5
    beq @L_8013C1F4
    li r3, 0x0
    b @L_8013C58C
    @L_8013C1F4:
    lhz r0, 0x12(r4)
    cmplwi r0, 0x4
    beq @L_8013C250
    li r3, 0x0
    b @L_8013C58C
    @L_8013C208:
    lwz r0, 0x8(r4)
    mr r28, r5
    cmpwi r0, 0x1
    beq @L_8013C220
    li r3, 0x0
    b @L_8013C58C
    @L_8013C220:
    lwz r0, 0xc(r4)
    cmpwi r0, 0x4
    beq @L_8013C234
    li r3, 0x0
    b @L_8013C58C
    @L_8013C234:
    lhz r0, 0x12(r4)
    cmplwi r0, 0x8
    beq @L_8013C250
    li r3, 0x0
    b @L_8013C58C
    @L_8013C248:
    li r3, 0x0
    b @L_8013C58C
    @L_8013C250:
    addi r4, r4, 0x18
    addi r5, r5, 0x1
    @L_8013C258:
    lwz r0, 0x0(r4)
    cmpwi r0, 0xff
    bne @L_8013C120
    cmpwi r30, 0x0
    beq @L_8013C284
    cmpwi r29, 0x0
    beq @L_8013C284
    cmpwi r27, 0x0
    beq @L_8013C284
    cmpwi r28, 0x0
    bne @L_8013C2F4
    @L_8013C284:
    li r3, 0x0
    b @L_8013C58C
    b @L_8013C2F4
    @L_8013C290:
    li r0, 0x3
    stw r0, 0x4(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xb
    beq @L_8013C2DC
    bge @L_8013C2B8
    cmpwi r0, 0x9
    beq @L_8013C2C4
    bge @L_8013C2D0
    b @L_8013C2F0
    @L_8013C2B8:
    cmpwi r0, 0xd
    beq @L_8013C2E8
    b @L_8013C2F0
    @L_8013C2C4:
    lwz r0, 0x4(r26)
    stw r0, 0x14(r3)
    b @L_8013C2F0
    @L_8013C2D0:
    lwz r0, 0x8(r26)
    stw r0, 0x14(r3)
    b @L_8013C2F0
    @L_8013C2DC:
    lwz r0, 0x10(r26)
    stw r0, 0x14(r3)
    b @L_8013C2F0
    @L_8013C2E8:
    lwz r0, 0xc(r26)
    stw r0, 0x14(r3)
    @L_8013C2F0:
    addi r3, r3, 0x18
    @L_8013C2F4:
    lwz r0, 0x0(r3)
    cmpwi r0, 0xff
    bne @L_8013C290
    lhz r3, 0x1e(r26)
    li r6, 0x0
    lhz r4, 0x1c(r26)
    lwz r8, 0x14(r26)
    subi r0, r3, 0x1
    slwi r9, r4, 1
    slwi r10, r3, 3
    b @L_8013C560
    @L_8013C320:
    li r5, 0x98
    cmpwi r4, 0x0
    stb r5, 0x0(r8)
    addi r7, r8, 0x3
    li r5, 0x0
    sth r9, 0x1(r8)
    ble @L_8013C558
    cmpwi r4, 0x8
    subi r12, r4, 0x8
    ble @L_8013C508
    addi r11, r12, 0x7
    mr r8, r5
    srwi r11, r11, 3
    mtctr r11
    cmpwi r12, 0x0
    ble @L_8013C508
    @L_8013C360:
    addi r30, r8, 0x1
    addi r11, r5, 0x1
    add r30, r6, r30
    addi r29, r5, 0x2
    sth r30, 0x0(r7)
    mullw r11, r3, r11
    addi r28, r5, 0x3
    sth r30, 0x2(r7)
    addi r27, r5, 0x4
    addi r12, r5, 0x5
    sth r30, 0x4(r7)
    sth r30, 0x6(r7)
    addi r24, r11, 0x1
    subi r25, r30, 0x1
    addi r11, r5, 0x6
    sth r25, 0x8(r7)
    add r24, r6, r24
    mullw r11, r3, r11
    sth r25, 0xa(r7)
    subi r30, r24, 0x1
    sth r25, 0xc(r7)
    sth r25, 0xe(r7)
    mullw r29, r3, r29
    addi r11, r11, 0x1
    sth r24, 0x10(r7)
    add r11, r6, r11
    sth r24, 0x12(r7)
    addi r25, r29, 0x1
    sth r24, 0x14(r7)
    add r25, r6, r25
    subi r29, r25, 0x1
    sth r24, 0x16(r7)
    mullw r28, r3, r28
    sth r30, 0x18(r7)
    sth r30, 0x1a(r7)
    addi r24, r28, 0x1
    sth r30, 0x1c(r7)
    add r24, r6, r24
    subi r28, r24, 0x1
    sth r30, 0x1e(r7)
    mullw r27, r3, r27
    sth r25, 0x20(r7)
    sth r25, 0x22(r7)
    addi r30, r27, 0x1
    sth r25, 0x24(r7)
    add r30, r6, r30
    subi r27, r30, 0x1
    sth r25, 0x26(r7)
    mullw r12, r3, r12
    sth r29, 0x28(r7)
    sth r29, 0x2a(r7)
    addi r25, r12, 0x1
    sth r29, 0x2c(r7)
    add r25, r6, r25
    subi r12, r25, 0x1
    sth r29, 0x2e(r7)
    sth r24, 0x30(r7)
    sth r24, 0x32(r7)
    sth r24, 0x34(r7)
    sth r24, 0x36(r7)
    sth r28, 0x38(r7)
    sth r28, 0x3a(r7)
    sth r28, 0x3c(r7)
    sth r28, 0x3e(r7)
    sth r30, 0x40(r7)
    sth r30, 0x42(r7)
    sth r30, 0x44(r7)
    sth r30, 0x46(r7)
    sth r27, 0x48(r7)
    sth r27, 0x4a(r7)
    sth r27, 0x4c(r7)
    sth r27, 0x4e(r7)
    sth r25, 0x50(r7)
    sth r25, 0x52(r7)
    sth r25, 0x54(r7)
    sth r25, 0x56(r7)
    sth r12, 0x58(r7)
    sth r12, 0x5a(r7)
    sth r12, 0x5c(r7)
    sth r12, 0x5e(r7)
    sth r11, 0x60(r7)
    sth r11, 0x62(r7)
    sth r11, 0x64(r7)
    sth r11, 0x66(r7)
    subi r12, r11, 0x1
    addi r11, r5, 0x7
    add r8, r8, r10
    sth r12, 0x68(r7)
    mullw r11, r3, r11
    addi r5, r5, 0x8
    sth r12, 0x6a(r7)
    sth r12, 0x6c(r7)
    addi r25, r11, 0x1
    sth r12, 0x6e(r7)
    add r25, r6, r25
    subi r11, r25, 0x1
    sth r25, 0x70(r7)
    sth r25, 0x72(r7)
    sth r25, 0x74(r7)
    sth r25, 0x76(r7)
    sth r11, 0x78(r7)
    sth r11, 0x7a(r7)
    sth r11, 0x7c(r7)
    sth r11, 0x7e(r7)
    addi r7, r7, 0x80
    bdnz @L_8013C360
    @L_8013C508:
    mullw r11, r5, r3
    subf r8, r5, r4
    mtctr r8
    cmpw r5, r4
    bge @L_8013C558
    @L_8013C51C:
    addi r12, r11, 0x1
    add r11, r11, r3
    add r12, r6, r12
    addi r5, r5, 0x1
    sth r12, 0x0(r7)
    subi r8, r12, 0x1
    sth r12, 0x2(r7)
    sth r12, 0x4(r7)
    sth r12, 0x6(r7)
    sth r8, 0x8(r7)
    sth r8, 0xa(r7)
    sth r8, 0xc(r7)
    sth r8, 0xe(r7)
    addi r7, r7, 0x10
    bdnz @L_8013C51C
    @L_8013C558:
    mr r8, r7
    addi r6, r6, 0x1
    @L_8013C560:
    cmpw r6, r0
    blt @L_8013C320
    li r3, 0x0
    li r4, 0x0
    bl fn_800E0E14
    lwz r0, 0x14(r26)
    li r3, 0x1
    stw r0, 0x10(r31)
    lwz r0, 0x18(r26)
    srawi r0, r0, 5
    sth r0, 0xe(r31)
    @L_8013C58C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
