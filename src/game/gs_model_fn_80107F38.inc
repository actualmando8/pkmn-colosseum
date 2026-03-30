    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801080AC
    lis r31, lbl_80404B68@ha
    lwz r29, 0x20(r3)
    rlwinm r30, r28, 0, 16, 31
    addi r31, r31, lbl_80404B68@l
    b @L_801080A4
@L_80107F78:
    cmplwi r30, 0x0
    bne @L_80107F90
    li r0, 0x0
    stw r0, 0xc(r29)
    sth r0, 0x10(r29)
    b @L_80107FAC
@L_80107F90:
    addi r3, r29, 0xc
    li r4, 0x0
    li r5, 0x3c
    bl memset
    mr r3, r30
    bl fn_8005D830
    stw r3, 0xc(r29)
@L_80107FAC:
    lha r0, 0x50(r29)
    sth r0, 0x0(r31)
    lha r0, 0x52(r29)
    sth r0, 0x2(r31)
    lwz r0, 0x64(r29)
    stw r0, 0x4(r31)
    lfs f0, 0x68(r29)
    stfs f0, 0xc(r31)
    lfs f0, 0x6c(r29)
    stfs f0, 0x10(r31)
    lbz r0, 0x4(r29)
    stb r0, 0x20(r31)
    lwz r0, 0x58(r29)
    stw r0, 0x14(r31)
    lha r0, 0x5c(r29)
    sth r0, 0x18(r31)
    lha r0, 0x5e(r29)
    sth r0, 0x1a(r31)
    lha r0, 0x60(r29)
    sth r0, 0x1c(r31)
    lha r0, 0x62(r29)
    sth r0, 0x1e(r31)
    lha r3, 0x6(r29)
    bl fn_8005D934
    lha r0, 0x2(r3)
    sth r0, 0x8(r31)
    lha r3, 0x6(r29)
    bl fn_8005D934
    lha r0, 0x4(r3)
    li r28, 0x0
    sth r0, 0xa(r31)
    b @L_8010803C
@L_8010802C:
    mr r3, r31
    addi r4, r29, 0xc
    bl fn_801074D4
    addi r28, r28, 0x1
@L_8010803C:
    bl fn_800D3088
    cmplw r28, r3
    blt @L_8010802C
    lha r0, 0x0(r31)
    sth r0, 0x50(r29)
    lha r0, 0x2(r31)
    sth r0, 0x52(r29)
    lwz r0, 0x4(r31)
    stw r0, 0x64(r29)
    lfs f0, 0xc(r31)
    stfs f0, 0x68(r29)
    lfs f0, 0x10(r31)
    stfs f0, 0x6c(r29)
    lbz r0, 0x20(r31)
    stb r0, 0x4(r29)
    lwz r0, 0x14(r31)
    stw r0, 0x58(r29)
    lha r0, 0x18(r31)
    sth r0, 0x5c(r29)
    lha r0, 0x1a(r31)
    sth r0, 0x5e(r29)
    lha r0, 0x1c(r31)
    sth r0, 0x60(r29)
    lha r0, 0x1e(r31)
    sth r0, 0x62(r29)
    lwz r29, 0x0(r29)
@L_801080A4:
    cmplwi r29, 0x0
    bne @L_80107F78
@L_801080AC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
