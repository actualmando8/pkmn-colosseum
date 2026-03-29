    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r25, 0x44(r1)
    lis r3, lbl_803A9768@ha
    addi r4, r3, lbl_803A9768@l
    lwz r5, 0x0(r4)
    cmpwi r5, 0x6
    beq @L_80057648
    lis r3, lbl_80267698@ha
    slwi r0, r5, 2
    addi r3, r3, lbl_80267698@l
    lwzx r0, r3, r0
    cmpwi r0, 0x0
    bgt @L_8005757C
    li r3, 0x1
    b @L_80057680
    @L_8005757C:
    lis r3, lbl_802676B4@ha
    lwz r0, 0x4(r4)
    addi r25, r3, lbl_802676B4@l
    slwi r5, r5, 3
    lwz r26, 0x0(r25)
    cmpwi r0, 0x0
    lwz r27, 0x4(r25)
    addi r4, r1, 0x8
    lwz r28, 0x8(r25)
    li r3, 0xa0
    lwz r29, 0xc(r25)
    lwz r30, 0x10(r25)
    lwz r31, 0x14(r25)
    lwz r12, 0x18(r25)
    lwz r11, 0x1c(r25)
    lwz r10, 0x20(r25)
    lwz r9, 0x24(r25)
    lwz r8, 0x28(r25)
    lwz r7, 0x2c(r25)
    lwz r6, 0x30(r25)
    lwz r0, 0x34(r25)
    stw r26, 0x8(r1)
    stw r27, 0xc(r1)
    stw r28, 0x10(r1)
    stw r29, 0x14(r1)
    stw r30, 0x18(r1)
    stw r31, 0x1c(r1)
    stw r12, 0x20(r1)
    stw r11, 0x24(r1)
    stw r10, 0x28(r1)
    stw r9, 0x2c(r1)
    stw r8, 0x30(r1)
    stw r7, 0x34(r1)
    stw r6, 0x38(r1)
    stw r0, 0x3c(r1)
    beq @L_80057614
    li r0, 0x1
    b @L_80057618
    @L_80057614:
    li r0, 0x0
    @L_80057618:
    slwi r0, r0, 2
    add r0, r5, r0
    lwzx r0, r4, r0
    clrlwi r4, r0, 16
    bl fn_80107170
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80057640
    li r3, 0x0
    b @L_80057680
    @L_80057640:
    li r3, 0x1
    b @L_80057680
    @L_80057648:
    li r3, 0xa0
    li r4, 0x76e
    bl fn_80107170
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80057668
    li r3, 0x0
    b @L_80057680
    @L_80057668:
    li r3, 0xa0
    li r4, 0x77e
    bl fn_80107170
    clrlwi r0, r3, 24
    cntlzw r0, r0
    srwi r3, r0, 5
    @L_80057680:
    lmw r25, 0x44(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
