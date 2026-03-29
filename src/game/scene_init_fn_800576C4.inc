    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r22, 0x48(r1)
    mr r29, r3
    lis r4, lbl_803A9768@ha
    li r3, 0xa0
    addi r4, r4, lbl_803A9768@l
    stw r29, 0x0(r4)
    bl fn_80104704
    lis r4, lbl_80267698@ha
    slwi r0, r29, 2
    addi r4, r4, lbl_80267698@l
    mr r30, r3
    lwzx r31, r4, r0
    cmpwi r31, 0x0
    ble @L_800577CC
    cmplwi r30, 0x0
    beq @L_800577CC
    lis r4, lbl_803A9768@ha
    lis r6, lbl_802676B4@ha
    addi r5, r4, lbl_803A9768@l
    addi r4, r1, 0x8
    addi r7, r6, lbl_802676B4@l
    lwz r0, 0x4(r5)
    lwz r6, 0x0(r7)
    lwz r22, 0x4(r7)
    cmpwi r0, 0x0
    lwz r23, 0x8(r7)
    lwz r24, 0xc(r7)
    lwz r25, 0x10(r7)
    lwz r26, 0x14(r7)
    lwz r27, 0x18(r7)
    lwz r28, 0x1c(r7)
    lwz r12, 0x20(r7)
    lwz r11, 0x24(r7)
    lwz r10, 0x28(r7)
    lwz r9, 0x2c(r7)
    lwz r8, 0x30(r7)
    lwz r7, 0x34(r7)
    lwz r0, 0x0(r5)
    stw r6, 0x8(r1)
    slwi r6, r0, 3
    stw r22, 0xc(r1)
    stw r23, 0x10(r1)
    stw r24, 0x14(r1)
    stw r25, 0x18(r1)
    stw r26, 0x1c(r1)
    stw r27, 0x20(r1)
    stw r28, 0x24(r1)
    stw r12, 0x28(r1)
    stw r11, 0x2c(r1)
    stw r10, 0x30(r1)
    stw r9, 0x34(r1)
    stw r8, 0x38(r1)
    stw r7, 0x3c(r1)
    beq @L_800577B0
    li r0, 0x1
    b @L_800577B4
    @L_800577B0:
    li r0, 0x0
    @L_800577B4:
    slwi r0, r0, 2
    clrlwi r5, r31, 16
    add r0, r6, r0
    lwzx r0, r4, r0
    clrlwi r4, r0, 16
    bl fn_801081F8
    @L_800577CC:
    cmpwi r29, 0x2
    beq @L_800577DC
    cmpwi r29, 0x4
    bne @L_800577F4
    @L_800577DC:
    cmpwi r31, 0x0
    ble @L_800577F4
    mr r3, r30
    clrlwi r5, r31, 16
    li r4, 0x76e
    bl fn_801081F8
    @L_800577F4:
    cmpwi r29, 0x6
    bne @L_8005781C
    mr r3, r30
    li r4, 0x76e
    li r5, 0xe5
    bl fn_801081F8
    mr r3, r30
    li r4, 0x77e
    li r5, 0xec
    bl fn_801081F8
    @L_8005781C:
    lmw r22, 0x48(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
