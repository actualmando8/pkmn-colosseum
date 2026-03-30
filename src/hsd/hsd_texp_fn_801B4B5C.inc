    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r30, r3
    lis r3, -32729
    mr r29, r4
    addi r31, r3, 21416
    mr r27, r5
    mr r28, r6
    bne @L_801B4B98
    addi r3, r2, -22576
    li r4, 0x528
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B4B98:
    cmplwi r29, 0x0
    bne @L_801B4BB0
    addi r3, r2, -22576
    li r4, 0x529
    addi r5, r2, -22536
    bl fn_80196E10
@L_801B4BB0:
    cmplwi r30, 0x0
    bne @L_801B4BC0
    li r0, 0x0
    b @L_801B4BEC
@L_801B4BC0:
    addis r0, r30, 0x1
    cmplwi r0, 0xffff
    bne @L_801B4BD4
    li r0, 0x2
    b @L_801B4BEC
@L_801B4BD4:
    addis r0, r30, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B4BE8
    li r0, 0x3
    b @L_801B4BEC
@L_801B4BE8:
    lwz r0, 0x0(r30)
@L_801B4BEC:
    cmpwi r0, 0x1
    beq @L_801B4C04
    addi r5, r31, 0x34
    addi r3, r2, -22576
    li r4, 0x52a
    bl fn_80196E10
@L_801B4C04:
    li r0, 0x0
    stw r0, 0x0(r29)
    li r0, 0x1
    stw r0, 0x4(r29)
    lwz r0, 0x74(r30)
    stw r0, 0x84(r29)
    lwz r0, 0x74(r30)
    cmplwi r0, 0x0
    bne @L_801B4C38
    li r3, 0xff
    li r0, 0xff
    stw r3, 0xc(r29)
    stw r0, 0x10(r29)
@L_801B4C38:
    lbz r0, 0x78(r30)
    cmplwi r0, 0xff
    bne @L_801B4C4C
    li r0, 0xff
    b @L_801B4C50
@L_801B4C4C:
    lbz r0, 0x78(r30)
@L_801B4C50:
    stw r0, 0x14(r29)
    lbz r0, 0x1f(r30)
    cmplwi r0, 0xff
    bne @L_801B4C68
    li r0, 0x0
    b @L_801B4C6C
@L_801B4C68:
    lbz r0, 0x1f(r30)
@L_801B4C6C:
    stw r0, 0x64(r29)
    lbz r0, 0x1e(r30)
    cmplwi r0, 0xff
    bne @L_801B4C84
    li r0, 0x0
    b @L_801B4C88
@L_801B4C84:
    lbz r0, 0x1e(r30)
@L_801B4C88:
    stw r0, 0x68(r29)
    lbz r0, 0x20(r30)
    cmplwi r0, 0xff
    bne @L_801B4CA0
    li r0, 0x0
    b @L_801B4CA4
@L_801B4CA0:
    lbz r0, 0x20(r30)
@L_801B4CA4:
    stw r0, 0x6c(r29)
    lbz r0, 0x21(r30)
    cmplwi r0, 0xff
    bne @L_801B4CBC
    li r0, 0x0
    b @L_801B4CC0
@L_801B4CBC:
    lbz r0, 0x21(r30)
@L_801B4CC0:
    stw r0, 0x70(r29)
    lwz r0, 0x24(r30)
    cmpwi r0, 0xff
    bne @L_801B4CD8
    li r0, 0x0
    b @L_801B4CDC
@L_801B4CD8:
    lwz r0, 0x24(r30)
@L_801B4CDC:
    stw r0, 0x74(r29)
    lwz r0, 0x28(r30)
    cmpwi r0, 0xff
    bne @L_801B4CF4
    li r0, 0x1
    b @L_801B4CF8
@L_801B4CF4:
    lwz r0, 0x28(r30)
@L_801B4CF8:
    stw r0, 0x78(r29)
    lwz r0, 0x2c(r30)
    cmpwi r0, 0xff
    bne @L_801B4D10
    li r0, 0x2
    b @L_801B4D14
@L_801B4D10:
    lwz r0, 0x2c(r30)
@L_801B4D14:
    stw r0, 0x7c(r29)
    lwz r0, 0x30(r30)
    cmpwi r0, 0xff
    bne @L_801B4D2C
    li r0, 0x3
    b @L_801B4D30
@L_801B4D2C:
    lwz r0, 0x30(r30)
@L_801B4D30:
    stw r0, 0x80(r29)
    lbz r0, 0xd(r30)
    cmplwi r0, 0xff
    beq @L_801B4D94
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    bne @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0x8
    beq @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0x9
    beq @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0xa
    beq @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0xb
    beq @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0xc
    beq @L_801B4E00
    lbz r0, 0x19(r30)
    cmplwi r0, 0xd
    beq @L_801B4E00
@L_801B4D94:
    li r0, 0x0
    li r4, 0xf
    stw r0, 0x18(r29)
    li r3, 0xf
    li r0, 0xf
    stw r4, 0x1c(r29)
    stw r3, 0x20(r29)
    stw r0, 0x24(r29)
    lwz r0, 0x0(r27)
    cmpwi r0, 0x0
    beq @L_801B4DD4
    li r3, 0x0
    li r0, 0xf
    stw r3, 0x0(r27)
    stw r0, 0x28(r29)
    b @L_801B4DDC
@L_801B4DD4:
    li r0, 0x0
    stw r0, 0x28(r29)
@L_801B4DDC:
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x2c(r29)
    li r3, 0x0
    li r0, 0x0
    stw r4, 0x30(r29)
    stb r3, 0x34(r29)
    stw r0, 0x38(r29)
    b @L_801B4F10
@L_801B4E00:
    lbz r0, 0xd(r30)
    stw r0, 0x18(r29)
    lbz r0, 0x36(r30)
    cmplwi r0, 0xff
    bne @L_801B4E1C
    li r0, 0xf
    b @L_801B4E20
@L_801B4E1C:
    lbz r0, 0x36(r30)
@L_801B4E20:
    stw r0, 0x1c(r29)
    lbz r0, 0x3e(r30)
    cmplwi r0, 0xff
    bne @L_801B4E38
    li r0, 0xf
    b @L_801B4E3C
@L_801B4E38:
    lbz r0, 0x3e(r30)
@L_801B4E3C:
    stw r0, 0x20(r29)
    lbz r0, 0x46(r30)
    cmplwi r0, 0xff
    bne @L_801B4E54
    li r0, 0xf
    b @L_801B4E58
@L_801B4E54:
    lbz r0, 0x46(r30)
@L_801B4E58:
    stw r0, 0x24(r29)
    lbz r0, 0x4e(r30)
    cmplwi r0, 0xff
    bne @L_801B4E70
    li r0, 0xf
    b @L_801B4E74
@L_801B4E70:
    lbz r0, 0x4e(r30)
@L_801B4E74:
    stw r0, 0x28(r29)
    lbz r0, 0x10(r30)
    cmplwi r0, 0xff
    bne @L_801B4E8C
    li r0, 0x0
    b @L_801B4E90
@L_801B4E8C:
    lbz r0, 0x10(r30)
@L_801B4E90:
    stw r0, 0x2c(r29)
    lbz r0, 0xf(r30)
    cmplwi r0, 0xff
    bne @L_801B4EA8
    li r0, 0x0
    b @L_801B4EAC
@L_801B4EA8:
    lbz r0, 0xf(r30)
@L_801B4EAC:
    stw r0, 0x30(r29)
    lbz r3, 0xe(r30)
    neg r0, r3
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x34(r29)
    lbz r0, 0xc(r30)
    cmplwi r0, 0xff
    bne @L_801B4EE0
    addi r5, r31, 0x58
    addi r3, r2, -22576
    li r4, 0x571
    bl fn_80196E10
@L_801B4EE0:
    lbz r4, 0xc(r30)
    lis r3, -32713
    addi r0, r3, -11496
    rlwinm r3, r4, 2, 0, 29
    add r3, r0, r3
    lwz r0, 0x0(r3)
    stw r0, 0x38(r29)
    lwz r0, 0x38(r29)
    cmpwi r0, 0x0
    bne @L_801B4F10
    li r0, 0x0
    stw r0, 0x0(r27)
@L_801B4F10:
    lbz r0, 0x19(r30)
    cmplwi r0, 0xff
    beq @L_801B4F28
    lwz r0, 0x14(r30)
    cmpwi r0, 0x0
    bne @L_801B4F94
@L_801B4F28:
    li r0, 0x0
    li r4, 0x7
    stw r0, 0x3c(r29)
    li r3, 0x7
    li r0, 0x7
    stw r4, 0x40(r29)
    stw r3, 0x44(r29)
    stw r0, 0x48(r29)
    lwz r0, 0x0(r28)
    cmpwi r0, 0x0
    beq @L_801B4F68
    li r3, 0x0
    li r0, 0x7
    stw r3, 0x0(r28)
    stw r0, 0x4c(r29)
    b @L_801B4F70
@L_801B4F68:
    li r0, 0x0
    stw r0, 0x4c(r29)
@L_801B4F70:
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x50(r29)
    li r3, 0x0
    li r0, 0x0
    stw r4, 0x54(r29)
    stb r3, 0x58(r29)
    stw r0, 0x5c(r29)
    b @L_801B50A4
@L_801B4F94:
    lbz r0, 0x19(r30)
    stw r0, 0x3c(r29)
    lbz r0, 0x56(r30)
    cmplwi r0, 0xff
    bne @L_801B4FB0
    li r0, 0x7
    b @L_801B4FB4
@L_801B4FB0:
    lbz r0, 0x56(r30)
@L_801B4FB4:
    stw r0, 0x40(r29)
    lbz r0, 0x5e(r30)
    cmplwi r0, 0xff
    bne @L_801B4FCC
    li r0, 0x7
    b @L_801B4FD0
@L_801B4FCC:
    lbz r0, 0x5e(r30)
@L_801B4FD0:
    stw r0, 0x44(r29)
    lbz r0, 0x66(r30)
    cmplwi r0, 0xff
    bne @L_801B4FE8
    li r0, 0x7
    b @L_801B4FEC
@L_801B4FE8:
    lbz r0, 0x66(r30)
@L_801B4FEC:
    stw r0, 0x48(r29)
    lbz r0, 0x6e(r30)
    cmplwi r0, 0xff
    bne @L_801B5004
    li r0, 0x7
    b @L_801B5008
@L_801B5004:
    lbz r0, 0x6e(r30)
@L_801B5008:
    stw r0, 0x4c(r29)
    lbz r0, 0x1c(r30)
    cmplwi r0, 0xff
    bne @L_801B5020
    li r0, 0x0
    b @L_801B5024
@L_801B5020:
    lbz r0, 0x1c(r30)
@L_801B5024:
    stw r0, 0x50(r29)
    lbz r0, 0x1b(r30)
    cmplwi r0, 0xff
    bne @L_801B503C
    li r0, 0x0
    b @L_801B5040
@L_801B503C:
    lbz r0, 0x1b(r30)
@L_801B5040:
    stw r0, 0x54(r29)
    lbz r3, 0x1a(r30)
    neg r0, r3
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x58(r29)
    lbz r0, 0x18(r30)
    cmplwi r0, 0xff
    bne @L_801B5074
    addi r5, r31, 0x74
    addi r3, r2, -22576
    li r4, 0x595
    bl fn_80196E10
@L_801B5074:
    lbz r4, 0x18(r30)
    lis r3, -32713
    addi r0, r3, -11496
    rlwinm r3, r4, 2, 0, 29
    add r3, r0, r3
    lwz r0, 0x0(r3)
    stw r0, 0x5c(r29)
    lwz r0, 0x5c(r29)
    cmpwi r0, 0x0
    bne @L_801B50A4
    li r0, 0x0
    stw r0, 0x0(r28)
@L_801B50A4:
    li r0, 0x0
    stw r0, 0x60(r29)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
