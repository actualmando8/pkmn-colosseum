    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r29, r3
    mr r30, r4
    lwz r5, lbl_8047AA80(r13)
    lbz r0, 0x47e(r5)
    cmplwi r0, 0x1
    bne @L_800D76E8
    lwz r24, 0x24(r5)
    stw r29, 0x24(r5)
    bl fn_800D7940
    lwz r3, lbl_8047AA80(r13)
    stw r24, 0x24(r3)
    b @L_800D780C
    @L_800D76E8:
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D7710
    mr r5, r29
    clrlwi r6, r30, 16
    li r3, 0x47
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D780C
    @L_800D7710:
    lis r3, lbl_80314370@ha
    lis r4, lbl_803143B4@ha
    addi r31, r3, lbl_80314370@l
    mr r25, r29
    lis r3, lbl_803143D8@ha
    addi r27, r4, lbl_803143B4@l
    mr r24, r31
    li r26, 0x0
    addi r28, r3, lbl_803143D8@l
    @L_800D7734:
    lbz r0, 0x8(r25)
    cmplwi r0, 0x0
    beq @L_800D7770
    cmpwi r26, 0x0
    beq @L_800D7770
    lwz r3, 0x10(r25)
    lwz r0, 0x14(r25)
    slwi r5, r3, 2
    lwz r3, 0x4(r29)
    slwi r0, r0, 2
    lwz r4, 0x0(r24)
    lwzx r5, r27, r5
    lwzx r6, r28, r0
    lbz r7, 0x18(r25)
    bl fn_800B7D74
    @L_800D7770:
    addi r26, r26, 0x1
    addi r24, r24, 0x4
    cmpwi r26, 0xe
    addi r25, r25, 0x1c
    blt @L_800D7734
    bl fn_800B7D3C
    lis r3, lbl_803143A8@ha
    mr r24, r29
    li r25, 0x0
    addi r28, r3, lbl_803143A8@l
    @L_800D7798:
    lbz r0, 0x8(r24)
    cmplwi r0, 0x0
    beq @L_800D77D0
    lwz r0, 0xc(r24)
    lwz r3, 0x0(r31)
    slwi r0, r0, 2
    lwzx r4, r28, r0
    bl fn_800B7874
    lwz r4, 0x1c(r24)
    cmplwi r4, 0x0
    beq @L_800D77D0
    lwz r3, 0x0(r31)
    lbz r5, 0x20(r24)
    bl fn_800B84E0
    @L_800D77D0:
    addi r25, r25, 0x1
    addi r31, r31, 0x4
    cmpwi r25, 0xe
    addi r24, r24, 0x1c
    blt @L_800D7798
    lis r4, lbl_804001F0@ha
    mr r3, r29
    addi r5, r4, lbl_804001F0@l
    lwz r4, 0x14(r5)
    addi r0, r4, 0x1
    stw r0, 0x14(r5)
    bl fn_800D892C
    mr r3, r29
    mr r4, r30
    bl fn_800D7940
    @L_800D780C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
