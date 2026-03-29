    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    stw r28, 0x40(r1)
    mr r31, r4
    lis r3, lbl_802672F0@ha
    addi r29, r1, 0x8
    addi r30, r3, lbl_802672F0@l
    li r28, 0x0
    lwz r12, 0x0(r30)
    lwz r11, 0x4(r30)
    lwz r10, 0x8(r30)
    lwz r9, 0xc(r30)
    lwz r8, 0x10(r30)
    lwz r7, 0x14(r30)
    lwz r6, 0x18(r30)
    lwz r5, 0x1c(r30)
    lwz r4, 0x20(r30)
    lwz r3, 0x24(r30)
    lwz r0, 0x28(r30)
    stw r12, 0x8(r1)
    stw r11, 0xc(r1)
    stw r10, 0x10(r1)
    stw r9, 0x14(r1)
    stw r8, 0x18(r1)
    stw r7, 0x1c(r1)
    stw r6, 0x20(r1)
    stw r5, 0x24(r1)
    stw r4, 0x28(r1)
    stw r3, 0x2c(r1)
    stw r0, 0x30(r1)
    li r0, 0xb
    mtctr r0
    @L_8004DD50:
    lha r3, 0x6(r31)
    lwz r0, 0x0(r29)
    cmpw r3, r0
    beq @L_8004DD6C
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    bdnz @L_8004DD50
    @L_8004DD6C:
    cmpwi r28, 0xb
    blt @L_8004DD7C
    li r3, 0x0
    b @L_8004DDA0
    @L_8004DD7C:
    clrlwi r3, r28, 24
    bl fn_801D1620
    cmplwi r3, 0x0
    beq @L_8004DD94
    stw r3, 0x4c(r31)
    b @L_8004DD9C
    @L_8004DD94:
    li r0, 0x36cd
    stw r0, 0x4c(r31)
    @L_8004DD9C:
    li r3, 0x0
    @L_8004DDA0:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r28, 0x40(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
