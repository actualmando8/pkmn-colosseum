    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801070D8
    lis r31, lbl_80404B68@ha
    lwz r30, 0x20(r3)
    addi r31, r31, lbl_80404B68@l
    b @L_801070D0
@L_80106FCC:
    li r0, 0x0
    stw r0, 0xc(r30)
    sth r0, 0x10(r30)
    lha r0, 0x50(r30)
    sth r0, 0x0(r31)
    lha r0, 0x52(r30)
    sth r0, 0x2(r31)
    lwz r0, 0x64(r30)
    stw r0, 0x4(r31)
    lfs f0, 0x68(r30)
    stfs f0, 0xc(r31)
    lfs f0, 0x6c(r30)
    stfs f0, 0x10(r31)
    lbz r0, 0x4(r30)
    stb r0, 0x20(r31)
    lwz r0, 0x58(r30)
    stw r0, 0x14(r31)
    lha r0, 0x5c(r30)
    sth r0, 0x18(r31)
    lha r0, 0x5e(r30)
    sth r0, 0x1a(r31)
    lha r0, 0x60(r30)
    sth r0, 0x1c(r31)
    lha r0, 0x62(r30)
    sth r0, 0x1e(r31)
    lha r3, 0x6(r30)
    bl fn_8005D934
    lha r0, 0x2(r3)
    sth r0, 0x8(r31)
    lha r3, 0x6(r30)
    bl fn_8005D934
    lha r0, 0x4(r3)
    li r29, 0x0
    sth r0, 0xa(r31)
    b @L_80107068
@L_80107058:
    mr r3, r31
    addi r4, r30, 0xc
    bl fn_801074D4
    addi r29, r29, 0x1
@L_80107068:
    bl fn_800D3088
    cmplw r29, r3
    blt @L_80107058
    lha r0, 0x0(r31)
    sth r0, 0x50(r30)
    lha r0, 0x2(r31)
    sth r0, 0x52(r30)
    lwz r0, 0x4(r31)
    stw r0, 0x64(r30)
    lfs f0, 0xc(r31)
    stfs f0, 0x68(r30)
    lfs f0, 0x10(r31)
    stfs f0, 0x6c(r30)
    lbz r0, 0x20(r31)
    stb r0, 0x4(r30)
    lwz r0, 0x14(r31)
    stw r0, 0x58(r30)
    lha r0, 0x18(r31)
    sth r0, 0x5c(r30)
    lha r0, 0x1a(r31)
    sth r0, 0x5e(r30)
    lha r0, 0x1c(r31)
    sth r0, 0x60(r30)
    lha r0, 0x1e(r31)
    sth r0, 0x62(r30)
    lwz r30, 0x0(r30)
@L_801070D0:
    cmplwi r30, 0x0
    bne @L_80106FCC
@L_801070D8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
