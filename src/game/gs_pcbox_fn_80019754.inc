    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lwz r0, lbl_80478868(r13)
    cmpwi r0, 0x0
    beq @L_8001982C
    lis r4, lbl_802E4E10@ha
    lis r6, lbl_802EF0A8@ha
    addi r30, r4, lbl_802E4E10@l
    li r0, 0x0
    lwz r5, 0x0(r30)
    addi r8, r6, lbl_802EF0A8@l
    lwz r4, 0xc(r30)
    addi r29, r8, 0x4
    lwz r7, 0x18(r30)
    mulli r9, r5, 0x1c
    lwz r6, 0x24(r30)
    addi r28, r8, 0x8
    lwz r5, 0x30(r30)
    mulli r8, r4, 0x1c
    lwz r4, 0x3c(r30)
    lhax r10, r29, r9
    lhax r9, r28, r9
    mulli r7, r7, 0x1c
    lhax r31, r29, r8
    lhax r12, r28, r8
    sth r10, 0x4(r30)
    mulli r6, r6, 0x1c
    lhax r11, r29, r7
    lhax r10, r28, r7
    sth r9, 0x6(r30)
    mulli r5, r5, 0x1c
    lhax r9, r29, r6
    lhax r8, r28, r6
    sth r31, 0x10(r30)
    mulli r4, r4, 0x1c
    lhax r7, r29, r5
    lhax r6, r28, r5
    sth r12, 0x12(r30)
    lhax r5, r29, r4
    lhax r4, r28, r4
    sth r11, 0x1c(r30)
    sth r10, 0x1e(r30)
    sth r9, 0x28(r30)
    sth r8, 0x2a(r30)
    sth r7, 0x34(r30)
    sth r6, 0x36(r30)
    sth r5, 0x40(r30)
    sth r4, 0x42(r30)
    stw r0, lbl_80478868(r13)
    @L_8001982C:
    li r30, 0x0
    stw r3, 0x8(r1)
    mr r31, r30
    addi r29, r1, 0x8
    b @L_80019880
    @L_80019840:
    mr r3, r31
    bl fn_801440A0
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    bne @L_8001987C
    mr r4, r31
    li r3, 0x0
    bl fn_80129B2C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001987C
    sth r31, 0x4(r29)
    addi r29, r29, 0x2
    addi r30, r30, 0x1
    @L_8001987C:
    addi r31, r31, 0x1
    @L_80019880:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r31, 16
    cmplw r3, r0
    bge @L_80019898
    cmpwi r30, 0x3
    blt @L_80019840
    @L_80019898:
    slwi r4, r30, 1
    addi r3, r1, 0xc
    li r5, 0x0
    addi r0, r30, 0x1
    sthx r5, r3, r4
    stw r0, 0x14(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x5f
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0x5f
    bl fn_80102510
    li r3, 0x5f
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, 0x0
    blt @L_80019914
    lwz r0, 0x14(r1)
    cmpw r31, r0
    bge @L_80019914
    slwi r0, r31, 1
    addi r3, r1, 0xc
    lhzx r3, r3, r0
    b @L_80019918
    @L_80019914:
    li r3, 0x0
    @L_80019918:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
