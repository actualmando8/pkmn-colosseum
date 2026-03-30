    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, lbl_80314370@ha
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    addi r31, r4, lbl_80314370@l
    lis r4, lbl_803143B4@ha
    lis r3, lbl_803143D8@ha
    mr r28, r25
    mr r27, r31
    addi r29, r4, lbl_803143B4@l
    addi r30, r3, lbl_803143D8@l
    li r26, 0x0
    @L_800D7AA8:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D7AE4
    cmpwi r26, 0x0
    beq @L_800D7AE4
    lwz r3, 0x10(r28)
    lwz r0, 0x14(r28)
    slwi r5, r3, 2
    lwz r3, 0x4(r25)
    slwi r0, r0, 2
    lwz r4, 0x0(r27)
    lwzx r5, r29, r5
    lwzx r6, r30, r0
    lbz r7, 0x18(r28)
    bl fn_800B7D74
    @L_800D7AE4:
    addi r26, r26, 0x1
    addi r27, r27, 0x4
    cmpwi r26, 0xe
    addi r28, r28, 0x1c
    blt @L_800D7AA8
    bl fn_800B7D3C
    lis r3, lbl_803143A8@ha
    mr r27, r25
    li r28, 0x0
    addi r30, r3, lbl_803143A8@l
    @L_800D7B0C:
    lbz r0, 0x8(r27)
    cmplwi r0, 0x0
    beq @L_800D7B44
    lwz r0, 0xc(r27)
    lwz r3, 0x0(r31)
    slwi r0, r0, 2
    lwzx r4, r30, r0
    bl fn_800B7874
    lwz r4, 0x1c(r27)
    cmplwi r4, 0x0
    beq @L_800D7B44
    lwz r3, 0x0(r31)
    lbz r5, 0x20(r27)
    bl fn_800B84E0
    @L_800D7B44:
    addi r28, r28, 0x1
    addi r31, r31, 0x4
    cmpwi r28, 0xe
    addi r27, r27, 0x1c
    blt @L_800D7B0C
    lis r3, lbl_804001F0@ha
    addi r4, r3, lbl_804001F0@l
    lwz r3, 0x14(r4)
    addi r0, r3, 0x1
    stw r0, 0x14(r4)
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
