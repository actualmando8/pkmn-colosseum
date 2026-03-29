    nofralloc
    stwu r1, -0x1c0(r1)
    mflr r0
    stw r0, 0x1c4(r1)
    stw r31, 0x1bc(r1)
    stw r30, 0x1b8(r1)
    stw r29, 0x1b4(r1)
    stw r28, 0x1b0(r1)
    mr r30, r3
    mr r31, r4
    addi r3, r1, 0x1c
    la r4, lbl_80478888(r13)
    bl fn_80142EF8
    addi r3, r1, 0x1c
    bl fn_801431AC
    cmplwi r3, 0x15
    bgt @L_800226E0
    lis r4, jumptable_802E4F00@ha
    slwi r0, r3, 2
    addi r3, r4, jumptable_802E4F00@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r30
    li r4, 0x1
    bl fn_800141BC
    mr r28, r3
    cmpwi r28, 0x0
    blt @L_80022648
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    bl fn_80014118
    lwz r3, 0x8(r1)
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80022614
    lwz r4, 0x8(r1)
    addi r3, r1, 0x2c
    lwz r5, 0xc(r1)
    clrlwi r6, r30, 16
    li r7, 0x0
    bl fn_80144574
    mr r29, r3
    extsh r0, r29
    cmpwi r0, 0x0
    ble @L_800225D0
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
    cmplw r30, r0
    beq @L_800225A8
    li r5, 0x1
    lhz r0, 0x12(r1)
    cmplw r30, r0
    beq @L_800225A8
    li r5, 0x2
    lhz r0, 0x14(r1)
    cmplw r30, r0
    beq @L_800225A8
    li r5, 0x3
    lhz r0, 0x16(r1)
    cmplw r30, r0
    beq @L_800225A8
    li r5, 0x4
    clrlwi r0, r3, 16
    cmplw r30, r0
    beq @L_800225A8
    li r5, 0x5
    @L_800225A8:
    cmpwi r5, 0x5
    bge @L_800225B8
    li r3, 0x466
    b @L_800225BC
    @L_800225B8:
    li r3, 0x465
    @L_800225BC:
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    bl fn_8001D378
    @L_800225D0:
    lwz r7, 0x8(r1)
    mr r6, r29
    addi r3, r1, 0x12c
    addi r5, r1, 0x2c
    li r4, 0x40
    bl fn_800216E8
    addi r4, r1, 0x12c
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0xe0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80022648
    @L_80022614:
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
    li r29, 0x0
    @L_80022648:
    mr r3, r28
    bl fn_80014198
    cmpwi r28, 0x0
    blt @L_80022694
    extsh r0, r29
    cmpwi r0, 0x0
    ble @L_80022694
    cmpwi r30, 0x2c
    bge @L_80022684
    cmpwi r30, 0x27
    bge @L_80022678
    b @L_80022684
    @L_80022678:
    li r0, 0x0
    stw r0, 0x0(r31)
    b @L_8002268C
    @L_80022684:
    li r0, 0x1
    stw r0, 0x0(r31)
    @L_8002268C:
    li r0, 0x0
    b @L_80022698
    @L_80022694:
    li r0, 0x1
    @L_80022698:
    mr r3, r0
    b @L_80022700
    mr r3, r30
    mr r4, r31
    bl fn_80023760
    b @L_80022700
    mr r3, r30
    mr r4, r31
    bl fn_800232F0
    b @L_80022700
    mr r3, r30
    mr r4, r31
    bl fn_80023068
    b @L_80022700
    mr r3, r30
    mr r4, r31
    bl fn_80023968
    b @L_80022700
    @L_800226E0:
    li r3, 0x2
    li r4, 0x426a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    @L_80022700:
    lwz r0, 0x1c4(r1)
    lwz r31, 0x1bc(r1)
    lwz r30, 0x1b8(r1)
    lwz r29, 0x1b4(r1)
    lwz r28, 0x1b0(r1)
    mtlr r0
    addi r1, r1, 0x1c0
    blr
