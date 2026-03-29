    nofralloc
    stwu r1, -0x410(r1)
    mflr r0
    stw r0, 0x414(r1)
    stmw r25, 0x3f4(r1)
    li r28, 0x0
    mr r29, r28
    b @L_8003D508
    @L_8003D4E4:
    mr r4, r29
    li r3, 0x0
    bl fn_8025FE84
    clrlwi r0, r29, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    addi r28, r28, 0x1
    sthx r3, r4, r0
    addi r29, r29, 0x1
    @L_8003D508:
    li r3, 0x0
    bl fn_8025FEE4
    clrlwi r3, r3, 16
    clrlwi r0, r29, 16
    cmplw r0, r3
    blt @L_8003D4E4
    sth r28, lbl_8047A4E8(r13)
    bl fn_8003D8CC
    lis r3, lbl_803A6818@ha
    addi r31, r3, lbl_803A6818@l
    lbz r0, 0x159(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_8003D7C0
    li r28, 0x0
    mr r29, r28
    b @L_8003D6BC
    @L_8003D54C:
    clrlwi r0, r29, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r30, r3, r0
    rlwinm r0, r30, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003D6B8
    lwz r25, lbl_8047A4E0(r13)
    mr r27, r30
    cmplwi r25, 0x0
    beq @L_8003D5DC
    cmplwi r30, 0x8000
    blt @L_8003D584
    clrlwi r27, r30, 18
    @L_8003D584:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r26, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r25
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r27
    bl fn_8011DF90
    @L_8003D5DC:
    lwz r25, lbl_8047A4E0(r13)
    cmplwi r25, 0x0
    beq @L_8003D68C
    mr r26, r30
    beq @L_8003D65C
    cmplwi r30, 0x8000
    blt @L_8003D5FC
    clrlwi r26, r30, 18
    @L_8003D5FC:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r27, r3
    mr r4, r26
    li r3, 0x0
    bl fn_8025FD34
    mr r26, r3
    mr r3, r25
    mr r4, r27
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r26
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003D660
    @L_8003D65C:
    li r3, 0x0
    @L_8003D660:
    cmplwi r3, 0x0
    beq @L_8003D684
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003D68C
    li r4, 0x0
    bl fn_8011E1D4
    b @L_8003D690
    @L_8003D684:
    li r3, 0x0
    b @L_8003D690
    @L_8003D68C:
    li r3, 0x0
    @L_8003D690:
    lbz r0, 0x159(r31)
    clrlwi r3, r3, 24
    extsb r0, r0
    cmpw r3, r0
    bne @L_8003D6B8
    clrlwi r0, r28, 16
    addi r3, r1, 0x8
    slwi r0, r0, 1
    addi r28, r28, 0x1
    sthx r30, r3, r0
    @L_8003D6B8:
    addi r29, r29, 0x1
    @L_8003D6BC:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r29, 16
    cmplw r3, r0
    blt @L_8003D54C
    clrlwi r0, r28, 16
    li r3, 0x0
    cmplwi r0, 0x0
    ble @L_8003D7BC
    cmplwi r0, 0x8
    subi r0, r28, 0x8
    ble @L_8003D78C
    clrlwi r0, r0, 16
    b @L_8003D780
    @L_8003D6F0:
    clrlwi r4, r3, 16
    addi r25, r1, 0x8
    slwi r12, r4, 1
    lwz r5, lbl_8047A4E4(r13)
    add r25, r25, r12
    addi r3, r3, 0x8
    lhz r7, 0x0(r25)
    addi r4, r12, 0x2
    lhz r10, 0x2(r25)
    addi r6, r12, 0x4
    sthx r7, r5, r12
    addi r11, r12, 0x6
    lhz r29, 0x4(r25)
    addi r9, r12, 0x8
    lwz r8, lbl_8047A4E4(r13)
    addi r7, r12, 0xa
    lhz r30, 0x6(r25)
    addi r5, r12, 0xc
    sthx r10, r8, r4
    addi r4, r12, 0xe
    lhz r12, 0x8(r25)
    lwz r8, lbl_8047A4E4(r13)
    lhz r10, 0xa(r25)
    sthx r29, r8, r6
    lhz r8, 0xc(r25)
    lwz r29, lbl_8047A4E4(r13)
    lhz r6, 0xe(r25)
    sthx r30, r29, r11
    lwz r11, lbl_8047A4E4(r13)
    sthx r12, r11, r9
    lwz r9, lbl_8047A4E4(r13)
    sthx r10, r9, r7
    lwz r7, lbl_8047A4E4(r13)
    sthx r8, r7, r5
    lwz r5, lbl_8047A4E4(r13)
    sthx r6, r5, r4
    @L_8003D780:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003D6F0
    @L_8003D78C:
    addi r6, r1, 0x8
    clrlwi r0, r28, 16
    b @L_8003D7B0
    @L_8003D798:
    clrlwi r5, r3, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r7, r5, 1
    addi r3, r3, 0x1
    lhzx r5, r6, r7
    sthx r5, r4, r7
    @L_8003D7B0:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003D798
    @L_8003D7BC:
    sth r28, lbl_8047A4E8(r13)
    @L_8003D7C0:
    bl fn_8003DC54
    bl fn_8003E394
    lhz r3, lbl_8047A4E8(r13)
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    li r0, -0x5
    cmplwi r3, 0x5
    stw r0, 0x8(r4)
    blt @L_8003D7F0
    li r0, 0x5
    stw r0, 0xc(r4)
    b @L_8003D7F4
    @L_8003D7F0:
    stw r3, 0xc(r4)
    @L_8003D7F4:
    lis r4, lbl_803A6818@ha
    addi r4, r4, lbl_803A6818@l
    stw r3, 0x10(r4)
    stw r3, 0x10(r4)
    lmw r25, 0x3f4(r1)
    lwz r0, 0x414(r1)
    mtlr r0
    addi r1, r1, 0x410
    blr
