    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    mr r31, r4
    lis r4, lbl_80266C54@ha
    li r29, 0x0
    addi r30, r4, lbl_80266C54@l
    lwz r12, 0x0(r30)
    lwz r11, 0x4(r30)
    lwz r10, 0x8(r30)
    lwz r9, 0xc(r30)
    lwz r8, 0x10(r30)
    lwz r7, 0x14(r30)
    lwz r6, 0x18(r30)
    lwz r5, 0x1c(r30)
    lwz r4, 0x20(r30)
    lwz r0, 0x24(r30)
    stw r12, 0x8(r1)
    stw r11, 0xc(r1)
    stw r10, 0x10(r1)
    stw r9, 0x14(r1)
    stw r8, 0x18(r1)
    stw r7, 0x1c(r1)
    stw r6, 0x20(r1)
    stw r5, 0x24(r1)
    stw r4, 0x28(r1)
    stw r0, 0x2c(r1)
    lhz r0, 0x8(r1)
    cmplw r3, r0
    beq @L_800227E8
    li r29, 0x1
    lhz r0, 0x10(r1)
    cmplw r3, r0
    beq @L_800227E8
    li r29, 0x2
    lhz r0, 0x18(r1)
    cmplw r3, r0
    beq @L_800227E8
    li r29, 0x3
    lhz r0, 0x20(r1)
    cmplw r3, r0
    beq @L_800227E8
    li r29, 0x4
    lhz r0, 0x28(r1)
    cmplw r3, r0
    beq @L_800227E8
    li r29, 0x5
    @L_800227E8:
    slwi r0, r29, 3
    addi r3, r1, 0xc
    lwzx r4, r3, r0
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
