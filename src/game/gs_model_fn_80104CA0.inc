    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    mr r31, r3
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r29, 0x0
@L_80104CE0:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80104D0C
    lbz r0, 0x95(r28)
    extsb r0, r0
    cmpw r0, r29
    bne @L_80104D08
    mr r30, r3
    b @L_80104D2C
@L_80104D08:
    addi r29, r29, 0x1
@L_80104D0C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80104D28
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80104CE0
@L_80104D28:
    li r30, 0x0
@L_80104D2C:
    cmplwi r30, 0x0
    beq @L_80104E60
    addi r3, r28, 0x20
    bl fn_8010925C
    lha r3, 0xc(r30)
    cmpwi r3, 0x0
    beq @L_80104E60
    bl fn_8005D858
    mr r29, r3
@L_80104D50:
    addi r3, r28, 0x20
    bl fn_80109290
    cmplwi r3, 0x0
    bne @L_80104D6C
    addi r3, r28, 0x20
    bl fn_8010925C
    b @L_80104E60
@L_80104D6C:
    lbz r0, 0x0(r29)
    rlwinm r0, r0, 28, 30, 31
    cmplwi r0, 0x1
    bne @L_80104D98
    lbz r0, 0x5(r3)
    ori r0, r0, 0x1
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x5(r3)
    lwz r0, 0x10(r29)
    stw r0, 0x58(r3)
    b @L_80104DB8
@L_80104D98:
    cmplwi r0, 0x2
    bne @L_80104DB8
    lbz r0, 0x5(r3)
    ori r0, r0, 0x2
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x5(r3)
    lwz r0, 0x10(r29)
    stw r0, 0x8(r3)
@L_80104DB8:
    lha r0, 0x4(r31)
    sth r0, 0x6(r3)
    lbz r0, 0x5(r29)
    lha r4, 0x2(r30)
    extsb r0, r0
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x50(r3)
    lbz r0, 0x6(r29)
    lha r4, 0x4(r30)
    extsb r0, r0
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x52(r3)
    lha r0, 0xc(r29)
    sth r0, 0x54(r3)
    lha r0, 0xe(r29)
    sth r0, 0x56(r3)
    lha r0, 0x8(r29)
    sth r0, 0x5c(r3)
    lha r0, 0xa(r29)
    sth r0, 0x5e(r3)
    lha r0, 0xc(r29)
    sth r0, 0x60(r3)
    lha r0, 0xe(r29)
    sth r0, 0x62(r3)
    lbz r0, 0x7(r29)
    stb r0, 0x67(r3)
    lhz r0, 0x0(r29)
    rlwinm r4, r0, 0, 20, 31
    cmplwi r4, 0x0
    beq @L_80104E40
    addi r3, r3, 0xc
    bl fn_80108518
@L_80104E40:
    lbz r0, 0x0(r29)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    bne @L_80104E60
    lha r3, 0x14(r29)
    bl fn_8005D858
    mr r29, r3
    b @L_80104D50
@L_80104E60:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
