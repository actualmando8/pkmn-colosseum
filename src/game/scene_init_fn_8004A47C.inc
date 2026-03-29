    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    stw r29, 0x64(r1)
    lwz r6, lbl_8047BD78(r2)
    li r3, 0x3f0
    lwz r5, lbl_8047BD7C(r2)
    lwz r4, lbl_8047BD80(r2)
    lwz r0, lbl_8047BD84(r2)
    stw r6, 0x20(r1)
    stw r5, 0x24(r1)
    stw r4, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8004A514
    lha r3, 0x20(r1)
    lis r4, lbl_802EF0A8@ha
    lha r5, 0x22(r1)
    addi r7, r4, lbl_802EF0A8@l
    lha r4, 0x24(r1)
    subi r0, r3, 0x6
    lha r3, 0x26(r1)
    subi r5, r5, 0x6
    subi r4, r4, 0x6
    extsh r6, r0
    subi r0, r3, 0x6
    extsh r5, r5
    extsh r3, r4
    sth r6, 0x7608(r7)
    extsh r0, r0
    sth r5, 0x7624(r7)
    sth r3, 0x7640(r7)
    sth r0, 0x765c(r7)
    b @L_8004A55C
    @L_8004A514:
    lha r3, 0x18(r1)
    lis r4, lbl_802EF0A8@ha
    lha r5, 0x1a(r1)
    addi r7, r4, lbl_802EF0A8@l
    lha r4, 0x1c(r1)
    subi r0, r3, 0x6
    lha r3, 0x1e(r1)
    subi r5, r5, 0x6
    subi r4, r4, 0x6
    extsh r6, r0
    subi r0, r3, 0x6
    extsh r5, r5
    extsh r3, r4
    sth r6, 0x7608(r7)
    extsh r0, r0
    sth r5, 0x7624(r7)
    sth r3, 0x7640(r7)
    sth r0, 0x765c(r7)
    @L_8004A55C:
    lis r3, lbl_8026719C@ha
    lis r4, lbl_802671B4@ha
    addi r5, r3, lbl_8026719C@l
    li r3, 0x3f0
    addi r9, r4, lbl_802671B4@l
    lwz r29, 0x0(r5)
    lwz r30, 0x4(r5)
    lwz r31, 0x8(r5)
    lwz r12, 0xc(r5)
    lwz r11, 0x10(r5)
    lwz r10, 0x14(r5)
    lwz r8, 0x0(r9)
    lwz r7, 0x4(r9)
    lwz r6, 0x8(r9)
    lwz r5, 0xc(r9)
    lwz r4, 0x10(r9)
    lwz r0, 0x14(r9)
    stw r29, 0x40(r1)
    stw r30, 0x44(r1)
    stw r31, 0x48(r1)
    stw r12, 0x4c(r1)
    stw r11, 0x50(r1)
    stw r10, 0x54(r1)
    stw r8, 0x28(r1)
    stw r7, 0x2c(r1)
    stw r6, 0x30(r1)
    stw r5, 0x34(r1)
    stw r4, 0x38(r1)
    stw r0, 0x3c(r1)
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8004A650
    lis r3, lbl_802EF0A8@ha
    lha r31, 0x40(r1)
    addi r3, r3, lbl_802EF0A8@l
    lha r12, 0x42(r1)
    addis r29, r3, 0x1
    lha r11, 0x44(r1)
    lha r10, 0x46(r1)
    lha r9, 0x48(r1)
    lha r8, 0x4a(r1)
    lha r7, 0x4c(r1)
    lha r6, 0x4e(r1)
    lha r5, 0x50(r1)
    lha r4, 0x52(r1)
    lha r3, 0x54(r1)
    lha r0, 0x56(r1)
    sth r31, -0x3eb8(r29)
    sth r12, -0x3e9c(r29)
    sth r11, -0x3e80(r29)
    sth r10, -0x31d0(r29)
    sth r9, -0x31b4(r29)
    sth r8, -0x3198(r29)
    sth r7, -0x317c(r29)
    sth r6, -0x3160(r29)
    sth r5, -0x3144(r29)
    sth r4, -0x3128(r29)
    sth r3, -0x310c(r29)
    sth r0, -0x30f0(r29)
    b @L_8004A6BC
    @L_8004A650:
    lis r3, lbl_802EF0A8@ha
    lha r31, 0x28(r1)
    addi r3, r3, lbl_802EF0A8@l
    lha r12, 0x2a(r1)
    addis r29, r3, 0x1
    lha r11, 0x2c(r1)
    lha r10, 0x2e(r1)
    lha r9, 0x30(r1)
    lha r8, 0x32(r1)
    lha r7, 0x34(r1)
    lha r6, 0x36(r1)
    lha r5, 0x38(r1)
    lha r4, 0x3a(r1)
    lha r3, 0x3c(r1)
    lha r0, 0x3e(r1)
    sth r31, -0x3eb8(r29)
    sth r12, -0x3e9c(r29)
    sth r11, -0x3e80(r29)
    sth r10, -0x31d0(r29)
    sth r9, -0x31b4(r29)
    sth r8, -0x3198(r29)
    sth r7, -0x317c(r29)
    sth r6, -0x3160(r29)
    sth r5, -0x3144(r29)
    sth r4, -0x3128(r29)
    sth r3, -0x310c(r29)
    sth r0, -0x30f0(r29)
    @L_8004A6BC:
    lwz r6, lbl_8047BD88(r2)
    li r3, 0x3f0
    lwz r5, lbl_8047BD8C(r2)
    lwz r4, lbl_8047BD90(r2)
    lwz r0, lbl_8047BD94(r2)
    stw r6, 0x10(r1)
    stw r5, 0x14(r1)
    stw r4, 0x8(r1)
    stw r0, 0xc(r1)
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8004A740
    lha r3, 0x10(r1)
    lis r4, lbl_802EF0A8@ha
    lha r5, 0x12(r1)
    addi r7, r4, lbl_802EF0A8@l
    lha r4, 0x14(r1)
    subi r6, r3, 0x6
    lha r3, 0x16(r1)
    subi r5, r5, 0x6
    subi r4, r4, 0x6
    addis r7, r7, 0x1
    subi r0, r3, 0x6
    extsh r6, r6
    extsh r5, r5
    extsh r3, r4
    extsh r0, r0
    sth r6, -0x3f28(r7)
    sth r5, -0x3f0c(r7)
    sth r3, -0x3ef0(r7)
    sth r0, -0x3ed4(r7)
    b @L_8004A78C
    @L_8004A740:
    lha r3, 0x8(r1)
    lis r4, lbl_802EF0A8@ha
    lha r5, 0xa(r1)
    addi r7, r4, lbl_802EF0A8@l
    lha r4, 0xc(r1)
    subi r6, r3, 0x6
    lha r3, 0xe(r1)
    subi r5, r5, 0x6
    subi r4, r4, 0x6
    addis r7, r7, 0x1
    subi r0, r3, 0x6
    extsh r6, r6
    extsh r5, r5
    extsh r3, r4
    extsh r0, r0
    sth r6, -0x3f28(r7)
    sth r5, -0x3f0c(r7)
    sth r3, -0x3ef0(r7)
    sth r0, -0x3ed4(r7)
    @L_8004A78C:
    lwz r0, 0x74(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    lwz r29, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
