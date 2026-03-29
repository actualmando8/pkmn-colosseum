    nofralloc
    stwu r1, -0x200(r1)
    mflr r0
    stw r0, 0x204(r1)
    stmw r24, 0x1e0(r1)
    mr r24, r3
    mr r30, r4
    bl fn_80014110
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_80014118
    lwz r3, 0xc(r1)
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800221F8
    lwz r3, 0xc(r1)
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x424d
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    b @L_80022464
    @L_800221F8:
    lwz r4, 0xc(r1)
    addi r3, r1, 0xd8
    lwz r5, 0x8(r1)
    clrlwi r6, r24, 16
    li r7, 0x0
    bl fn_80144574
    mr r31, r3
    extsh r0, r31
    cmpwi r0, 0x0
    ble @L_800222BC
    lis r3, lbl_80266DB0@ha
    li r5, 0x0
    addi r3, r3, lbl_80266DB0@l
    lwz r4, 0x0(r3)
    lwz r0, 0x4(r3)
    lhz r3, 0x8(r3)
    stw r4, 0x10(r1)
    stw r0, 0x14(r1)
    sth r3, 0x18(r1)
    lhz r0, 0x10(r1)
    cmplw r24, r0
    beq @L_80022294
    li r5, 0x1
    lhz r0, 0x12(r1)
    cmplw r24, r0
    beq @L_80022294
    li r5, 0x2
    lhz r0, 0x14(r1)
    cmplw r24, r0
    beq @L_80022294
    li r5, 0x3
    lhz r0, 0x16(r1)
    cmplw r24, r0
    beq @L_80022294
    li r5, 0x4
    clrlwi r0, r3, 16
    cmplw r24, r0
    beq @L_80022294
    li r5, 0x5
    @L_80022294:
    cmpwi r5, 0x5
    bge @L_800222A4
    li r3, 0x466
    b @L_800222A8
    @L_800222A4:
    li r3, 0x465
    @L_800222A8:
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    bl fn_8001D378
    @L_800222BC:
    lis r3, lbl_80266D78@ha
    extsh r4, r31
    addi r24, r3, lbl_80266D78@l
    lwz r3, 0xc(r1)
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
    stw r25, 0x1c(r1)
    stw r26, 0x20(r1)
    stw r27, 0x24(r1)
    stw r28, 0x28(r1)
    stw r29, 0x2c(r1)
    stw r12, 0x30(r1)
    stw r11, 0x34(r1)
    stw r10, 0x38(r1)
    stw r9, 0x3c(r1)
    stw r8, 0x40(r1)
    stw r7, 0x44(r1)
    stw r6, 0x48(r1)
    stw r5, 0x4c(r1)
    stw r0, 0x50(r1)
    bgt @L_80022358
    addi r3, r1, 0x54
    li r4, 0x41
    li r5, 0x4261
    bl fn_800F96E4
    b @L_8002241C
    @L_80022358:
    addi r9, r1, 0x1c
    li r6, 0x0
    @L_80022360:
    lwz r5, 0x0(r9)
    addi r8, r1, 0xd8
    li r7, 0x0
    mtctr r4
    cmpwi r4, 0x0
    ble @L_80022390
    @L_80022378:
    lwz r0, 0x0(r8)
    cmpw r5, r0
    beq @L_80022390
    addi r8, r8, 0x8
    addi r7, r7, 0x1
    bdnz @L_80022378
    @L_80022390:
    cmpw r7, r4
    blt @L_800223A8
    addi r9, r9, 0x8
    addi r6, r6, 0x1
    cmpwi r6, 0x7
    blt @L_80022360
    @L_800223A8:
    cmpwi r6, 0x7
    blt @L_800223C4
    addi r3, r1, 0x54
    li r4, 0x41
    li r5, 0x4261
    bl fn_800F96E4
    b @L_8002241C
    @L_800223C4:
    slwi r0, r6, 3
    addi r6, r1, 0x20
    slwi r7, r7, 3
    addi r5, r1, 0xdc
    addi r4, r1, 0xde
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
    addi r3, r1, 0x54
    li r4, 0x41
    bl fn_800F96E4
    @L_8002241C:
    addi r4, r1, 0x54
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
    ble @L_80022460
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x0(r30)
    b @L_80022464
    @L_80022460:
    li r3, 0x1
    @L_80022464:
    lmw r24, 0x1e0(r1)
    lwz r0, 0x204(r1)
    mtlr r0
    addi r1, r1, 0x200
    blr
