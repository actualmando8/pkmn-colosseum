    nofralloc
    stwu r1, -0x3a0(r1)
    mflr r0
    stw r0, 0x3a4(r1)
    stmw r24, 0x380(r1)
    mr r29, r3
    mr r30, r4
    addi r3, r1, 0x30
    la r4, lbl_80478890(r13)
    bl fn_80142EF8
    addi r3, r1, 0x30
    bl fn_801431AC
    cmpwi r3, 0xc
    bge @L_80021B60
    cmpwi r3, 0x0
    beq @L_80021B6C
    blt @L_8002201C
    cmpwi r3, 0x3
    bge @L_80021E3C
    b @L_8002201C
    @L_80021B60:
    cmpwi r3, 0x15
    beq @L_8002200C
    b @L_8002201C
    @L_80021B6C:
    bl fn_80014110
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    bl fn_80014118
    lwz r3, 0x10(r1)
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80021BCC
    lwz r3, 0x10(r1)
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x424d
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_8002203C
    @L_80021BCC:
    lwz r4, 0x10(r1)
    addi r3, r1, 0x1fc
    lwz r5, 0x14(r1)
    clrlwi r6, r29, 16
    li r7, 0x0
    bl fn_80144574
    mr r31, r3
    extsh r0, r31
    cmpwi r0, 0x0
    ble @L_80021C90
    lis r3, lbl_80266DB0@ha
    li r5, 0x0
    addi r3, r3, lbl_80266DB0@l
    lwz r4, 0x0(r3)
    lwz r0, 0x4(r3)
    lhz r3, 0x8(r3)
    stw r4, 0x24(r1)
    stw r0, 0x28(r1)
    sth r3, 0x2c(r1)
    lhz r0, 0x24(r1)
    cmplw r29, r0
    beq @L_80021C68
    li r5, 0x1
    lhz r0, 0x26(r1)
    cmplw r29, r0
    beq @L_80021C68
    li r5, 0x2
    lhz r0, 0x28(r1)
    cmplw r29, r0
    beq @L_80021C68
    li r5, 0x3
    lhz r0, 0x2a(r1)
    cmplw r29, r0
    beq @L_80021C68
    li r5, 0x4
    clrlwi r0, r3, 16
    cmplw r29, r0
    beq @L_80021C68
    li r5, 0x5
    @L_80021C68:
    cmpwi r5, 0x5
    bge @L_80021C78
    li r3, 0x466
    b @L_80021C7C
    @L_80021C78:
    li r3, 0x465
    @L_80021C7C:
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    bl fn_8001D378
    @L_80021C90:
    lis r3, lbl_80266D78@ha
    extsh r4, r31
    addi r24, r3, lbl_80266D78@l
    lwz r3, 0x10(r1)
    lwz r25, 0x0(r24)
    cmpwi r4, 0x0
    lwz r26, 0x4(r24)
    lwz r27, 0x8(r24)
    lwz r28, 0xc(r24)
    lwz r29, 0x10(r24)
    lwz r12, 0x14(r24)
    lwz r11, 0x18(r24)
    lwz r10, 0x1c(r24)
    lwz r9, 0x20(r24)
    lwz r8, 0x24(r24)
    lwz r7, 0x28(r24)
    lwz r6, 0x2c(r24)
    lwz r5, 0x30(r24)
    lwz r0, 0x34(r24)
    stw r25, 0x40(r1)
    stw r26, 0x44(r1)
    stw r27, 0x48(r1)
    stw r28, 0x4c(r1)
    stw r29, 0x50(r1)
    stw r12, 0x54(r1)
    stw r11, 0x58(r1)
    stw r10, 0x5c(r1)
    stw r9, 0x60(r1)
    stw r8, 0x64(r1)
    stw r7, 0x68(r1)
    stw r6, 0x6c(r1)
    stw r5, 0x70(r1)
    stw r0, 0x74(r1)
    bgt @L_80021D2C
    addi r3, r1, 0x2fc
    li r4, 0x41
    li r5, 0x4261
    bl fn_800F96E4
    b @L_80021DF0
    @L_80021D2C:
    addi r9, r1, 0x40
    li r6, 0x0
    @L_80021D34:
    lwz r5, 0x0(r9)
    addi r8, r1, 0x1fc
    li r7, 0x0
    mtctr r4
    cmpwi r4, 0x0
    ble @L_80021D64
    @L_80021D4C:
    lwz r0, 0x0(r8)
    cmpw r5, r0
    beq @L_80021D64
    addi r8, r8, 0x8
    addi r7, r7, 0x1
    bdnz @L_80021D4C
    @L_80021D64:
    cmpw r7, r4
    blt @L_80021D7C
    addi r9, r9, 0x8
    addi r6, r6, 0x1
    cmpwi r6, 0x7
    blt @L_80021D34
    @L_80021D7C:
    cmpwi r6, 0x7
    blt @L_80021D98
    addi r3, r1, 0x2fc
    li r4, 0x41
    li r5, 0x4261
    bl fn_800F96E4
    b @L_80021DF0
    @L_80021D98:
    slwi r0, r6, 3
    addi r6, r1, 0x44
    slwi r7, r7, 3
    addi r5, r1, 0x200
    addi r4, r1, 0x202
    lwzx r24, r6, r0
    lhax r26, r5, r7
    lhax r25, r4, r7
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    mr r4, r26
    li r3, 0x2f
    bl fn_80132A38
    mr r4, r25
    li r3, 0x30
    bl fn_80132A38
    mr r5, r24
    addi r3, r1, 0x2fc
    li r4, 0x41
    bl fn_800F96E4
    @L_80021DF0:
    addi r4, r1, 0x2fc
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0xe0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    extsh r0, r31
    cmpwi r0, 0x0
    ble @L_80021E34
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x0(r30)
    b @L_8002203C
    @L_80021E34:
    li r3, 0x1
    b @L_8002203C
    @L_80021E3C:
    mr r3, r29
    li r4, 0x1
    bl fn_800141BC
    mr r25, r3
    cmpwi r25, 0x0
    blt @L_80021FB4
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    bl fn_80014118
    lwz r3, 0x8(r1)
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80021F80
    lwz r4, 0x8(r1)
    addi r3, r1, 0x78
    lwz r5, 0xc(r1)
    clrlwi r6, r29, 16
    li r7, 0x0
    bl fn_80144574
    mr r24, r3
    extsh r0, r24
    cmpwi r0, 0x0
    ble @L_80021F3C
    lis r3, lbl_80266DB0@ha
    li r5, 0x0
    addi r3, r3, lbl_80266DB0@l
    lwz r4, 0x0(r3)
    lwz r0, 0x4(r3)
    lhz r3, 0x8(r3)
    stw r4, 0x18(r1)
    stw r0, 0x1c(r1)
    sth r3, 0x20(r1)
    lhz r0, 0x18(r1)
    cmplw r29, r0
    beq @L_80021F14
    li r5, 0x1
    lhz r0, 0x1a(r1)
    cmplw r29, r0
    beq @L_80021F14
    li r5, 0x2
    lhz r0, 0x1c(r1)
    cmplw r29, r0
    beq @L_80021F14
    li r5, 0x3
    lhz r0, 0x1e(r1)
    cmplw r29, r0
    beq @L_80021F14
    li r5, 0x4
    clrlwi r0, r3, 16
    cmplw r29, r0
    beq @L_80021F14
    li r5, 0x5
    @L_80021F14:
    cmpwi r5, 0x5
    bge @L_80021F24
    li r3, 0x466
    b @L_80021F28
    @L_80021F24:
    li r3, 0x465
    @L_80021F28:
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    bl fn_8001D378
    @L_80021F3C:
    lwz r7, 0x8(r1)
    mr r6, r24
    addi r3, r1, 0x178
    addi r5, r1, 0x78
    li r4, 0x40
    bl fn_800216E8
    addi r4, r1, 0x178
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0xe0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80021FB4
    @L_80021F80:
    lwz r3, 0x8(r1)
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x424d
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r24, 0x0
    @L_80021FB4:
    mr r3, r25
    bl fn_80014198
    cmpwi r25, 0x0
    blt @L_80022000
    extsh r0, r24
    cmpwi r0, 0x0
    ble @L_80022000
    cmpwi r29, 0x2c
    bge @L_80021FF0
    cmpwi r29, 0x27
    bge @L_80021FE4
    b @L_80021FF0
    @L_80021FE4:
    li r0, 0x0
    stw r0, 0x0(r30)
    b @L_80021FF8
    @L_80021FF0:
    li r0, 0x1
    stw r0, 0x0(r30)
    @L_80021FF8:
    li r0, 0x0
    b @L_80022004
    @L_80022000:
    li r0, 0x1
    @L_80022004:
    mr r3, r0
    b @L_8002203C
    @L_8002200C:
    mr r3, r29
    mr r4, r30
    bl fn_80023968
    b @L_8002203C
    @L_8002201C:
    li r3, 0x2
    li r4, 0x426a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    @L_8002203C:
    lmw r24, 0x380(r1)
    lwz r0, 0x3a4(r1)
    mtlr r0
    addi r1, r1, 0x3a0
    blr
