    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r25, 0x14(r1)
    mr r29, r3
    mr r30, r4
    lis r3, lbl_804024E8@ha
    li r4, 0x0
    addi r3, r3, lbl_804024E8@l
    li r5, 0x2c
    bl memset
    clrlwi r27, r29, 16
    mulli r3, r27, 0x68
    bl fn_800E3534
    lwz r4, lbl_80478B08(r13)
    sth r3, 0x2(r4)
    lwz r3, lbl_80478B08(r13)
    lhz r3, 0x2(r3)
    cmplwi r3, 0x0
    bne @L_800FC598
    lis r3, lbl_802717B4@ha
    addi r3, r3, lbl_802717B4@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, -0x1
    b @L_800FC7C4
@L_800FC598:
    bl fn_800E27B0
    lwz r4, lbl_80478B08(r13)
    clrlwi r31, r30, 16
    stw r3, 0x20(r4)
    slwi r3, r31, 3
    bl fn_800E3534
    lwz r4, lbl_80478B08(r13)
    sth r3, 0x6(r4)
    lwz r3, lbl_80478B08(r13)
    lhz r3, 0x6(r3)
    cmplwi r3, 0x0
    bne @L_800FC5E0
    lis r3, lbl_802717B4@ha
    addi r3, r3, lbl_802717B4@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, -0x1
    b @L_800FC7C4
@L_800FC5E0:
    bl fn_800E27B0
    lwz r4, lbl_80478B08(r13)
    li r25, 0x0
    li r28, 0x0
    stw r3, 0x24(r4)
    lfs f31, lbl_8047CD08(r2)
    b @L_800FC628
@L_800FC5FC:
    lwz r3, lbl_80478B08(r13)
    li r4, 0x0
    li r5, 0x68
    lwz r0, 0x20(r3)
    add r26, r0, r28
    mr r3, r26
    bl memset
    stfs f31, 0x60(r26)
    addi r28, r28, 0x68
    addi r25, r25, 0x1
    stfs f31, 0x64(r26)
@L_800FC628:
    cmpw r25, r27
    blt @L_800FC5FC
    lwz r4, lbl_80478B08(r13)
    cmpwi r31, 0x0
    li r3, 0x0
    sth r29, 0x0(r4)
    ble @L_800FC778
    cmpwi r31, 0x8
    subi r5, r31, 0x8
    ble @L_800FC73C
    mr r7, r3
    addi r0, r5, 0x7
    lis r4, 0x1
    srwi r0, r0, 3
    subi r6, r4, 0x1
    mtctr r0
    cmpwi r5, 0x0
    ble @L_800FC73C
@L_800FC670:
    lwz r4, lbl_80478B08(r13)
    li r5, 0x0
    addi r28, r7, 0x8
    addi r27, r7, 0x10
    lwz r0, 0x24(r4)
    addi r12, r7, 0x18
    addi r11, r7, 0x20
    addi r10, r7, 0x28
    add r4, r0, r7
    addi r9, r7, 0x30
    sth r6, 0x0(r4)
    addi r8, r7, 0x38
    addi r7, r7, 0x40
    addi r3, r3, 0x8
    stw r5, 0x4(r4)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r28, r0, r28
    sth r6, 0x0(r28)
    stw r5, 0x4(r28)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r27, r0, r27
    sth r6, 0x0(r27)
    stw r5, 0x4(r27)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r12, r0, r12
    sth r6, 0x0(r12)
    stw r5, 0x4(r12)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r11, r0, r11
    sth r6, 0x0(r11)
    stw r5, 0x4(r11)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r10, r0, r10
    sth r6, 0x0(r10)
    stw r5, 0x4(r10)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r9, r0, r9
    sth r6, 0x0(r9)
    stw r5, 0x4(r9)
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x24(r4)
    add r8, r0, r8
    sth r6, 0x0(r8)
    stw r5, 0x4(r8)
    bdnz @L_800FC670
@L_800FC73C:
    slwi r6, r3, 3
    lis r4, 0x1
    subf r0, r3, r31
    subi r5, r4, 0x1
    li r4, 0x0
    mtctr r0
    cmpw r3, r31
    bge @L_800FC778
@L_800FC75C:
    lwz r3, lbl_80478B08(r13)
    lwz r0, 0x24(r3)
    add r3, r0, r6
    addi r6, r6, 0x8
    sth r5, 0x0(r3)
    stw r4, 0x4(r3)
    bdnz @L_800FC75C
@L_800FC778:
    lwz r6, lbl_80478B08(r13)
    li r3, 0x200
    li r4, 0x200
    li r5, 0x40
    sth r30, 0x4(r6)
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    lwz r7, lbl_80478B08(r13)
    li r4, 0x200
    li r5, 0x40
    li r6, 0x0
    stw r3, 0xc(r7)
    li r3, 0x200
    li r7, 0x0
    bl fn_800EF5FC
    lwz r4, lbl_80478B08(r13)
    stw r3, 0x10(r4)
    li r3, 0x0
@L_800FC7C4:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r25, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
