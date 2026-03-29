    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r23, 0x5c(r1)
    mr r31, r3
    lis r3, lbl_803A9768@ha
    lis r7, lbl_802676B4@ha
    addi r5, r3, lbl_803A9768@l
    lha r6, 0x6(r4)
    addi r7, r7, lbl_802676B4@l
    lwz r0, 0x4(r5)
    lwz r23, 0x0(r7)
    mr r3, r4
    lwz r24, 0x4(r7)
    cmpwi r0, 0x0
    lwz r25, 0x8(r7)
    addi r4, r1, 0x8
    lwz r26, 0xc(r7)
    lwz r27, 0x10(r7)
    lwz r28, 0x14(r7)
    lwz r29, 0x18(r7)
    lwz r30, 0x1c(r7)
    lwz r12, 0x20(r7)
    lwz r11, 0x24(r7)
    lwz r10, 0x28(r7)
    lwz r9, 0x2c(r7)
    lwz r8, 0x30(r7)
    lwz r7, 0x34(r7)
    lwz r0, 0x0(r5)
    stw r23, 0x8(r1)
    slwi r5, r0, 3
    stw r24, 0xc(r1)
    stw r25, 0x10(r1)
    stw r26, 0x14(r1)
    stw r27, 0x18(r1)
    stw r28, 0x1c(r1)
    stw r29, 0x20(r1)
    stw r30, 0x24(r1)
    stw r12, 0x28(r1)
    stw r11, 0x2c(r1)
    stw r10, 0x30(r1)
    stw r9, 0x34(r1)
    stw r8, 0x38(r1)
    stw r7, 0x3c(r1)
    beq @L_80057200
    li r0, 0x1
    b @L_80057204
    @L_80057200:
    li r0, 0x0
    @L_80057204:
    slwi r0, r0, 2
    add r0, r5, r0
    lwzx r0, r4, r0
    cmpw r6, r0
    bne @L_80057220
    li r4, 0x1
    b @L_80057224
    @L_80057220:
    li r4, 0x0
    @L_80057224:
    bl fn_80109220
    lis r4, lbl_803A9768@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9768@l
    lfs f0, 0x27c(r4)
    fctiwz f0, f0
    stfd f0, 0x40(r1)
    lwz r0, 0x44(r1)
    sth r0, 0x84(r31)
    lfs f0, 0x280(r4)
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    sth r0, 0x86(r31)
    lmw r23, 0x5c(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
