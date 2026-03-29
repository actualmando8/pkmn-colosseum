    nofralloc
    stwu r1, -0x800(r1)
    mflr r0
    stw r0, 0x804(r1)
    stmw r23, 0x7dc(r1)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r4, 0x15a(r3)
    extsb r0, r4
    cmpwi r0, 0x0
    bne @L_8003DC8C
    lbz r0, 0x15b(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_8003E380
    @L_8003DC8C:
    extsb r27, r4
    lbz r28, 0x15b(r3)
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    extsb r28, r28
    beq @L_8003DCB8
    clrlwi r0, r28, 16
    cmplwi r0, 0x0
    beq @L_8003DCB8
    li r0, 0x0
    b @L_8003DCCC
    @L_8003DCB8:
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    bne @L_8003DCC8
    mr r27, r28
    @L_8003DCC8:
    li r0, 0x1
    @L_8003DCCC:
    clrlwi r0, r0, 16
    cmplwi r0, 0x0
    bne @L_8003E03C
    li r29, 0x0
    mr r30, r29
    b @L_8003DF34
    @L_8003DCE4:
    clrlwi r0, r30, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r31, r3, r0
    rlwinm r0, r31, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003DF30
    lwz r24, lbl_8047A4E0(r13)
    mr r26, r31
    cmplwi r24, 0x0
    beq @L_8003DD74
    cmplwi r31, 0x8000
    blt @L_8003DD1C
    clrlwi r26, r31, 18
    @L_8003DD1C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r25, r3
    mr r4, r26
    li r3, 0x0
    bl fn_8025FD34
    mr r26, r3
    mr r3, r24
    mr r4, r25
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r26
    bl fn_8011DF90
    @L_8003DD74:
    lwz r24, lbl_8047A4E0(r13)
    mr r25, r31
    cmplwi r24, 0x0
    beq @L_8003DDF0
    cmplwi r31, 0x8000
    blt @L_8003DD90
    clrlwi r25, r31, 18
    @L_8003DD90:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r25, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r25
    li r3, 0x0
    bl fn_8025FDDC
    mr r26, r3
    mr r4, r25
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r24
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r25
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003DDF4
    @L_8003DDF0:
    li r3, 0x0
    @L_8003DDF4:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    blt @L_8003DE28
    subi r3, r3, 0x1
    @L_8003DE28:
    lwz r23, lbl_8047A4E0(r13)
    addi r0, r3, 0x1
    mr r25, r31
    cmplwi r23, 0x0
    clrlwi r26, r0, 16
    beq @L_8003DEAC
    cmplwi r31, 0x8000
    blt @L_8003DE4C
    clrlwi r25, r31, 18
    @L_8003DE4C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r23
    clrlwi r4, r25, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r25
    li r3, 0x0
    bl fn_8025FDDC
    mr r24, r3
    mr r4, r25
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r23
    mr r4, r24
    bl fn_8011DFE0
    mr r3, r23
    mr r4, r25
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003DEB0
    @L_8003DEAC:
    li r3, 0x0
    @L_8003DEB0:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    blt @L_8003DEE4
    subi r3, r3, 0x1
    @L_8003DEE4:
    clrlwi r0, r27, 16
    addi r3, r3, 0x1
    cmplw r0, r26
    clrlwi r3, r3, 16
    bne @L_8003DF04
    clrlwi r0, r28, 16
    cmplw r0, r3
    beq @L_8003DF1C
    @L_8003DF04:
    clrlwi r0, r27, 16
    cmplw r0, r3
    bne @L_8003DF30
    clrlwi r0, r28, 16
    cmplw r0, r26
    bne @L_8003DF30
    @L_8003DF1C:
    clrlwi r0, r29, 16
    addi r3, r1, 0x3f0
    slwi r0, r0, 1
    addi r29, r29, 0x1
    sthx r31, r3, r0
    @L_8003DF30:
    addi r30, r30, 0x1
    @L_8003DF34:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r30, 16
    cmplw r3, r0
    blt @L_8003DCE4
    clrlwi r0, r29, 16
    li r3, 0x0
    cmplwi r0, 0x0
    ble @L_8003E034
    cmplwi r0, 0x8
    subi r0, r29, 0x8
    ble @L_8003E004
    clrlwi r0, r0, 16
    b @L_8003DFF8
    @L_8003DF68:
    clrlwi r4, r3, 16
    addi r23, r1, 0x3f0
    slwi r12, r4, 1
    lwz r5, lbl_8047A4E4(r13)
    add r23, r23, r12
    addi r3, r3, 0x8
    lhz r7, 0x0(r23)
    addi r4, r12, 0x2
    lhz r10, 0x2(r23)
    addi r6, r12, 0x4
    sthx r7, r5, r12
    addi r11, r12, 0x6
    lhz r27, 0x4(r23)
    addi r9, r12, 0x8
    lwz r8, lbl_8047A4E4(r13)
    addi r7, r12, 0xa
    lhz r28, 0x6(r23)
    addi r5, r12, 0xc
    sthx r10, r8, r4
    addi r4, r12, 0xe
    lhz r12, 0x8(r23)
    lwz r8, lbl_8047A4E4(r13)
    lhz r10, 0xa(r23)
    sthx r27, r8, r6
    lhz r8, 0xc(r23)
    lwz r27, lbl_8047A4E4(r13)
    lhz r6, 0xe(r23)
    sthx r28, r27, r11
    lwz r11, lbl_8047A4E4(r13)
    sthx r12, r11, r9
    lwz r9, lbl_8047A4E4(r13)
    sthx r10, r9, r7
    lwz r7, lbl_8047A4E4(r13)
    sthx r8, r7, r5
    lwz r5, lbl_8047A4E4(r13)
    sthx r6, r5, r4
    @L_8003DFF8:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003DF68
    @L_8003E004:
    addi r6, r1, 0x3f0
    clrlwi r0, r29, 16
    b @L_8003E028
    @L_8003E010:
    clrlwi r5, r3, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r7, r5, 1
    addi r3, r3, 0x1
    lhzx r5, r6, r7
    sthx r5, r4, r7
    @L_8003E028:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003E010
    @L_8003E034:
    sth r29, lbl_8047A4E8(r13)
    b @L_8003E380
    @L_8003E03C:
    li r29, 0x0
    mr r30, r29
    b @L_8003E27C
    @L_8003E048:
    clrlwi r0, r30, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r31, r3, r0
    rlwinm r0, r31, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003E278
    lwz r23, lbl_8047A4E0(r13)
    mr r26, r31
    cmplwi r23, 0x0
    beq @L_8003E0D8
    cmplwi r31, 0x8000
    blt @L_8003E080
    clrlwi r26, r31, 18
    @L_8003E080:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r23
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r26
    mr r26, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r23
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r23
    mr r4, r25
    bl fn_8011DF90
    @L_8003E0D8:
    lwz r23, lbl_8047A4E0(r13)
    mr r25, r31
    cmplwi r23, 0x0
    beq @L_8003E154
    cmplwi r31, 0x8000
    blt @L_8003E0F4
    clrlwi r25, r31, 18
    @L_8003E0F4:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r23
    clrlwi r4, r25, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r25
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r25
    mr r25, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r24, r3
    mr r3, r23
    mr r4, r25
    bl fn_8011DFE0
    mr r3, r23
    mr r4, r24
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003E158
    @L_8003E154:
    li r3, 0x0
    @L_8003E158:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    blt @L_8003E18C
    subi r3, r3, 0x1
    @L_8003E18C:
    lwz r23, lbl_8047A4E0(r13)
    addi r0, r3, 0x1
    mr r25, r31
    cmplwi r23, 0x0
    clrlwi r26, r0, 16
    beq @L_8003E210
    cmplwi r31, 0x8000
    blt @L_8003E1B0
    clrlwi r25, r31, 18
    @L_8003E1B0:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r23
    clrlwi r4, r25, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r25
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r25
    mr r25, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r24, r3
    mr r3, r23
    mr r4, r25
    bl fn_8011DFE0
    mr r3, r23
    mr r4, r24
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003E214
    @L_8003E210:
    li r3, 0x0
    @L_8003E214:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    clrlwi r3, r3, 16
    cmplwi r3, 0x9
    blt @L_8003E248
    subi r3, r3, 0x1
    @L_8003E248:
    clrlwi r0, r27, 16
    addi r3, r3, 0x1
    cmplw r0, r26
    clrlwi r3, r3, 16
    beq @L_8003E264
    cmplw r0, r3
    bne @L_8003E278
    @L_8003E264:
    clrlwi r0, r29, 16
    addi r3, r1, 0x8
    slwi r0, r0, 1
    addi r29, r29, 0x1
    sthx r31, r3, r0
    @L_8003E278:
    addi r30, r30, 0x1
    @L_8003E27C:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r30, 16
    cmplw r3, r0
    blt @L_8003E048
    clrlwi r0, r29, 16
    li r3, 0x0
    cmplwi r0, 0x0
    ble @L_8003E37C
    cmplwi r0, 0x8
    subi r0, r29, 0x8
    ble @L_8003E34C
    clrlwi r0, r0, 16
    b @L_8003E340
    @L_8003E2B0:
    clrlwi r4, r3, 16
    addi r23, r1, 0x8
    slwi r12, r4, 1
    lwz r5, lbl_8047A4E4(r13)
    add r23, r23, r12
    addi r3, r3, 0x8
    lhz r7, 0x0(r23)
    addi r4, r12, 0x2
    lhz r10, 0x2(r23)
    addi r6, r12, 0x4
    sthx r7, r5, r12
    addi r11, r12, 0x6
    lhz r27, 0x4(r23)
    addi r9, r12, 0x8
    lwz r8, lbl_8047A4E4(r13)
    addi r7, r12, 0xa
    lhz r28, 0x6(r23)
    addi r5, r12, 0xc
    sthx r10, r8, r4
    addi r4, r12, 0xe
    lhz r12, 0x8(r23)
    lwz r8, lbl_8047A4E4(r13)
    lhz r10, 0xa(r23)
    sthx r27, r8, r6
    lhz r8, 0xc(r23)
    lwz r27, lbl_8047A4E4(r13)
    lhz r6, 0xe(r23)
    sthx r28, r27, r11
    lwz r11, lbl_8047A4E4(r13)
    sthx r12, r11, r9
    lwz r9, lbl_8047A4E4(r13)
    sthx r10, r9, r7
    lwz r7, lbl_8047A4E4(r13)
    sthx r8, r7, r5
    lwz r5, lbl_8047A4E4(r13)
    sthx r6, r5, r4
    @L_8003E340:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003E2B0
    @L_8003E34C:
    addi r6, r1, 0x8
    clrlwi r0, r29, 16
    b @L_8003E370
    @L_8003E358:
    clrlwi r5, r3, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r7, r5, 1
    addi r3, r3, 0x1
    lhzx r5, r6, r7
    sthx r5, r4, r7
    @L_8003E370:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003E358
    @L_8003E37C:
    sth r29, lbl_8047A4E8(r13)
    @L_8003E380:
    lmw r23, 0x7dc(r1)
    lwz r0, 0x804(r1)
    mtlr r0
    addi r1, r1, 0x800
    blr
