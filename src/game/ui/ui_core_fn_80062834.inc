    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80062858
    li r29, 0x5c3
    b @L_8006285C
@L_80062858:
    li r29, 0x5c4
@L_8006285C:
    mr r3, r29
    bl fn_8017B1CC
    mr r3, r29
    bl fn_800F915C
    lis r3, lbl_803A9A60@ha
    addi r3, r3, lbl_803A9A60@l
    addi r30, r3, 0xc
    mr r28, r30
    bl fn_8025DA88
    cmpwi r3, 0x2
    bne @L_8006289C
    lis r3, lbl_803A9A60@ha
    li r0, 0x4
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x30(r3)
    b @L_800628AC
@L_8006289C:
    lis r3, lbl_803A9A60@ha
    li r0, 0x2
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x30(r3)
@L_800628AC:
    lis r3, lbl_803A9A60@ha
    li r27, 0x0
    addi r31, r3, lbl_803A9A60@l
    b @L_800628D0
@L_800628BC:
    lwz r4, 0x4(r28)
    mr r3, r29
    bl fn_800F9210
    addi r28, r28, 0x8
    addi r27, r27, 0x1
@L_800628D0:
    lwz r0, 0x30(r31)
    cmpw r27, r0
    blt @L_800628BC
    lis r3, lbl_803A9A60@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9A60@l
    stb r0, 0x34(r3)
    stw r0, 0x2c(r3)
    bl fn_8025DA88
    cmpwi r3, 0x2
    bne @L_80062908
    li r0, 0x4
    stw r0, 0x30(r31)
    b @L_80062910
@L_80062908:
    li r0, 0x2
    stw r0, 0x30(r31)
@L_80062910:
    li r0, 0x0
    stw r0, 0x4(r30)
    stw r0, 0x0(r30)
    stw r0, 0xc(r30)
    stw r0, 0x8(r30)
    stw r0, 0x14(r30)
    stw r0, 0x10(r30)
    stw r0, 0x1c(r30)
    stw r0, 0x18(r30)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
