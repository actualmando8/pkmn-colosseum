    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r23, 0x4c(r1)
    lwz r0, lbl_804788A0(r13)
    lis r3, lbl_80266DC0@ha
    addi r29, r3, lbl_80266DC0@l
    cmpwi r0, 0x0
    beq @L_8002901C
    lis r3, lbl_802EF0A8@ha
    li r0, 0x0
    addi r7, r3, lbl_802EF0A8@l
    stw r0, lbl_804788A0(r13)
    addis r3, r7, 0x2
    lha r6, 0x6fca(r7)
    lha r5, 0x6fcc(r7)
    lha r4, 0xec2(r3)
    lha r3, 0x73ba(r7)
    lha r0, 0x70fe(r7)
    stw r6, lbl_8047A3C0(r13)
    stw r5, lbl_8047A3BC(r13)
    stw r4, lbl_8047A3B8(r13)
    stw r3, lbl_8047A3B4(r13)
    stw r0, lbl_8047A3B0(r13)
    @L_8002901C:
    lis r3, lbl_803A2068@ha
    addi r30, r3, lbl_803A2068@l
    lwz r0, 0x18(r30)
    lwz r3, 0x1c(r30)
    cmpwi r0, 0x2
    beq @L_8002906C
    bge @L_80029048
    cmpwi r0, 0x0
    beq @L_80029054
    bge @L_80029060
    b @L_800290B4
    @L_80029048:
    cmpwi r0, 0x4
    bge @L_800290B4
    b @L_800290A4
    @L_80029054:
    lwz r3, 0x0(r29)
    bl fn_800FA280
    b @L_800290B8
    @L_80029060:
    lwz r3, 0xc(r29)
    bl fn_800FA280
    b @L_800290B8
    @L_8002906C:
    clrlwi r5, r3, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r26, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80029098
    li r3, 0x0
    b @L_800290B8
    @L_80029098:
    mr r3, r26
    bl fn_8011F4F0
    b @L_800290B8
    @L_800290A4:
    extsb r4, r3
    li r3, 0x0
    bl fn_80134A98
    b @L_800290B8
    @L_800290B4:
    li r3, 0x0
    @L_800290B8:
    cmplwi r3, 0x0
    bne @L_800290C8
    li r0, 0x0
    b @L_800290D8
    @L_800290C8:
    mr r4, r3
    addi r3, r1, 0x30
    bl fn_800F9E70
    li r0, 0x1
    @L_800290D8:
    cmpwi r0, 0x0
    bne @L_800290E8
    li r0, 0x0
    sth r0, 0x30(r1)
    @L_800290E8:
    lwz r26, 0x18(r30)
    lwz r5, 0x88(r29)
    lwz r4, 0x8c(r29)
    cmpwi r26, 0x2
    lwz r3, 0x90(r29)
    lwz r0, 0x94(r29)
    stw r5, 0x20(r1)
    lwz r27, 0x1c(r30)
    stw r4, 0x24(r1)
    stw r3, 0x28(r1)
    stw r0, 0x2c(r1)
    bne @L_80029120
    li r3, 0xd3a
    b @L_80029124
    @L_80029120:
    li r3, 0xd39
    @L_80029124:
    bl fn_8005D934
    lis r4, lbl_803A2094@ha
    lha r5, 0x8(r3)
    addi r0, r4, lbl_803A2094@l
    lha r4, 0x6(r3)
    mr r3, r0
    bl fn_8010A5BC
    cmpwi r26, 0x2
    beq @L_800291AC
    bge @L_80029158
    cmpwi r26, 0x0
    bge @L_80029164
    b @L_800291FC
    @L_80029158:
    cmpwi r26, 0x4
    bge @L_800291FC
    b @L_800291E4
    @L_80029164:
    slwi r0, r26, 2
    addi r3, r1, 0x20
    lwzx r26, r3, r0
    lis r3, lbl_803A2094@ha
    addi r3, r3, lbl_803A2094@l
    mr r4, r26
    bl fn_8010A010
    mr r3, r26
    bl fn_8018F6F4
    addi r5, r1, 0xc
    addi r6, r1, 0x8
    li r4, 0x1
    bl fn_8018F4C8
    lis r3, lbl_803A2094@ha
    lwz r4, 0xc(r1)
    addi r3, r3, lbl_803A2094@l
    bl fn_80109894
    b @L_800291FC
    @L_800291AC:
    clrlwi r5, r27, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r26, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800291FC
    lis r3, lbl_803A2094@ha
    mr r4, r26
    addi r3, r3, lbl_803A2094@l
    bl fn_80109C88
    b @L_800291FC
    @L_800291E4:
    slwi r0, r26, 2
    addi r4, r1, 0x20
    lis r3, lbl_803A2094@ha
    lwzx r4, r4, r0
    addi r3, r3, lbl_803A2094@l
    bl fn_8010A010
    @L_800291FC:
    lis r3, lbl_803A2094@ha
    li r4, 0x1
    addi r3, r3, lbl_803A2094@l
    bl fn_80109B90
    lwz r3, 0x18(r30)
    lwz r0, 0x1c(r30)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x18
    li r3, 0x6f
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    lis r3, lbl_803A2068@ha
    addi r28, r3, lbl_803A2068@l
    lwz r0, 0x28(r28)
    cmpwi r0, 0x0
    bne @L_8002926C
    lfs f1, lbl_8047B940(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    @L_8002926C:
    lwz r5, 0x18(r30)
    addi r3, r29, 0x18
    slwi r0, r5, 4
    add r3, r3, r0
    lwz r0, 0xc(r3)
    cmpwi r0, 0x0
    ble @L_800293E4
    addi r27, r1, 0x10
    lis r3, lbl_803A2068@ha
    addi r26, r3, lbl_803A2068@l
    @L_80029294:
    lwz r0, 0x18(r30)
    addi r3, r29, 0x18
    slwi r0, r0, 4
    add r3, r3, r0
    lwz r24, 0xc(r3)
    lwz r25, 0x8(r3)
    @L_800292AC:
    stw r25, 0x10(r1)
    stw r24, 0x14(r1)
    bl fn_801046B8
    mr r4, r3
    mr r9, r27
    li r3, 0x70
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, 0x0
    bne @L_800292F4
    li r3, 0x24
    bl fn_80166A28
    li r25, 0x0
    b @L_80029380
    @L_800292F4:
    cmpwi r3, -0x1
    beq @L_800292AC
    slwi r3, r3, 2
    subi r0, r3, 0x4
    lwzx r3, r25, r0
    bl fn_800FA280
    mr r31, r3
    li r3, 0x440
    bl fn_80166A28
    mr r4, r31
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2ef6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r23, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r23, 0x1
    beq @L_80029368
    cmpwi r23, -0x1
    bne @L_80029370
    @L_80029368:
    li r0, 0x0
    b @L_80029374
    @L_80029370:
    li r0, 0x1
    @L_80029374:
    cmpwi r0, 0x0
    beq @L_800292AC
    li r25, 0x1
    @L_80029380:
    li r3, 0x70
    bl fn_80102510
    li r3, 0x70
    li r4, 0x1
    bl fn_80102428
    cmpwi r25, 0x0
    beq @L_800293B4
    lis r3, lbl_803A2068@ha
    mr r4, r31
    addi r3, r3, lbl_803A2068@l
    bl fn_800F9E70
    li r0, 0x1
    b @L_800293B8
    @L_800293B4:
    li r0, 0x0
    @L_800293B8:
    cmpwi r0, 0x0
    bne @L_800293FC
    lwz r5, 0x18(r30)
    mr r3, r26
    lwz r6, 0x1c(r30)
    addi r4, r1, 0x30
    li r7, 0x0
    bl fn_800281F4
    cmpwi r3, 0x0
    bne @L_800293FC
    b @L_80029294
    @L_800293E4:
    lis r3, lbl_803A2068@ha
    lwz r6, 0x1c(r30)
    addi r3, r3, lbl_803A2068@l
    addi r4, r1, 0x30
    li r7, 0x1
    bl fn_800281F4
    @L_800293FC:
    lfs f1, lbl_8047B940(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, lbl_803A2094@ha
    addi r3, r3, lbl_803A2094@l
    bl fn_8010A420
    li r3, 0x6f
    bl fn_80102510
    li r3, 0x6f
    li r4, 0x1
    bl fn_80102428
    lwz r0, 0x18(r30)
    lwz r4, 0x1c(r30)
    cmpwi r0, 0x2
    beq @L_80029478
    bge @L_80029454
    cmpwi r0, 0x0
    beq @L_800294C4
    bge @L_80029460
    b @L_800294C4
    @L_80029454:
    cmpwi r0, 0x4
    bge @L_800294C4
    b @L_800294B0
    @L_80029460:
    lis r4, lbl_803A2068@ha
    li r3, 0x0
    addi r5, r4, lbl_803A2068@l
    li r4, 0x17
    bl fn_8012A450
    b @L_800294C4
    @L_80029478:
    clrlwi r5, r4, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r24, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800294C4
    lis r4, lbl_803A2068@ha
    mr r3, r24
    addi r4, r4, lbl_803A2068@l
    bl fn_8011DEE4
    b @L_800294C4
    @L_800294B0:
    lis r3, lbl_803A2068@ha
    extsb r4, r4
    addi r5, r3, lbl_803A2068@l
    li r3, 0x0
    bl fn_801349DC
    @L_800294C4:
    lis r3, lbl_803A2068@ha
    addi r4, r1, 0x30
    addi r3, r3, lbl_803A2068@l
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bne @L_800294F0
    lis r3, lbl_803A2068@ha
    li r0, 0x0
    addi r3, r3, lbl_803A2068@l
    stw r0, 0x20(r3)
    b @L_80029500
    @L_800294F0:
    lis r3, lbl_803A2068@ha
    li r0, 0x1
    addi r3, r3, lbl_803A2068@l
    stw r0, 0x20(r3)
    @L_80029500:
    lwz r0, 0x28(r28)
    cmpwi r0, 0x0
    beq @L_80029544
    bl fn_800FF660
    lis r3, lbl_803A2068@ha
    addi r3, r3, lbl_803A2068@l
    lwz r0, 0x24(r3)
    cmpwi r0, 0x0
    beq @L_80029538
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    b @L_80029544
    @L_80029538:
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_80029544:
    lmw r23, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
