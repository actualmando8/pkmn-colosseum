    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r3
    lwz r3, 0x4(r29)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    mr r28, r3
    bl fn_8005D934
    mr r0, r3
    addi r3, r29, 0x1c
    mr r30, r0
    bl fn_8010925C
    mr r3, r30
    addi r4, r1, 0xa
    addi r5, r1, 0x8
    bl fn_801052F4
@L_80104AE8:
    extsh r3, r28
    bl fn_8005D934
    mr r30, r3
    addi r3, r29, 0x1c
    bl fn_80109290
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80104B18
    addi r3, r29, 0x1c
    bl fn_8010925C
    li r3, 0x1
    b @L_80104C80
@L_80104B18:
    sth r28, 0x6(r31)
    lha r0, 0x2(r30)
    sth r0, 0x50(r31)
    lha r0, 0x4(r30)
    sth r0, 0x52(r31)
    lha r0, 0x6(r30)
    sth r0, 0x54(r31)
    lha r0, 0x8(r30)
    sth r0, 0x56(r31)
    lbz r0, 0x1(r30)
    stb r0, 0x67(r31)
    lbz r0, 0x0(r30)
    rlwinm r0, r0, 28, 30, 31
    stb r0, 0x74(r31)
    lha r3, 0xa(r30)
    cmpwi r3, 0x0
    beq @L_80104C34
    bl fn_8005D858
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 28, 30, 31
    cmplwi r0, 0x1
    bne @L_80104B8C
    lbz r0, 0x5(r31)
    ori r0, r0, 0x1
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x5(r31)
    lwz r0, 0x10(r3)
    stw r0, 0x58(r31)
    b @L_80104BAC
@L_80104B8C:
    cmplwi r0, 0x2
    bne @L_80104BAC
    lbz r0, 0x5(r31)
    ori r0, r0, 0x2
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x5(r31)
    lwz r0, 0x10(r3)
    stw r0, 0x8(r31)
@L_80104BAC:
    lha r0, 0x8(r3)
    lis r4, 0x8081
    subi r5, r4, 0x7f7f
    sth r0, 0x5c(r31)
    lha r0, 0xa(r3)
    sth r0, 0x5e(r31)
    lha r0, 0xc(r3)
    sth r0, 0x60(r31)
    lha r0, 0xe(r3)
    sth r0, 0x62(r31)
    lbz r4, 0x67(r31)
    lbz r0, 0x7(r3)
    mullw r0, r4, r0
    mulhw r4, r5, r0
    add r0, r4, r0
    srawi r0, r0, 7
    rlwinm r4, r0, 1, 31, 31
    add r0, r0, r4
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x67(r31)
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    beq @L_80104C1C
    lha r0, 0xa(r1)
    sth r0, 0x54(r31)
    lha r0, 0x8(r1)
    sth r0, 0x56(r31)
@L_80104C1C:
    lhz r0, 0x0(r3)
    rlwinm r4, r0, 0, 20, 31
    cmplwi r4, 0x0
    beq @L_80104C34
    addi r3, r31, 0xc
    bl fn_80108518
@L_80104C34:
    lwz r0, 0x10(r30)
    stw r0, 0x4c(r31)
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_80104C60
    lbz r0, 0x5(r31)
    ori r0, r0, 0x8
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x5(r31)
    lwz r0, 0x14(r30)
    stw r0, 0x48(r31)
@L_80104C60:
    lbz r0, 0x0(r30)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80104C7C
    lha r0, 0x18(r30)
    mr r28, r0
    b @L_80104AE8
@L_80104C7C:
    li r3, 0x0
@L_80104C80:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
