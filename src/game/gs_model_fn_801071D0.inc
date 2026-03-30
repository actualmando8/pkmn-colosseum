    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r3
    lis r30, lbl_80404B8C@ha
    lha r0, 0x84(r29)
    addi r30, r30, lbl_80404B8C@l
    sth r0, 0x0(r30)
    lha r0, 0x86(r29)
    sth r0, 0x2(r30)
    lwz r0, 0x88(r29)
    stw r0, 0x4(r30)
    lbz r0, 0x0(r29)
    stb r0, 0x20(r30)
    lwz r3, 0x4(r29)
    bl fn_8005DA18
    lis r3, lbl_80404B8C@ha
    lha r0, 0x6(r3)
    addi r3, r3, lbl_80404B8C@l
    sth r0, 0x8(r3)
    lwz r3, 0x4(r29)
    bl fn_8005DA18
    lis r31, lbl_80404B8C@ha
    lha r0, 0x8(r3)
    addi r31, r31, lbl_80404B8C@l
    li r28, 0x0
    sth r0, 0xa(r31)
    b @L_80107260
@L_80107250:
    mr r3, r31
    addi r4, r29, 0x24
    bl fn_801074D4
    addi r28, r28, 0x1
@L_80107260:
    bl fn_800D3088
    cmplw r28, r3
    blt @L_80107250
    lis r4, lbl_80404B8C@ha
    lis r31, lbl_80404B68@ha
    addi r4, r4, lbl_80404B8C@l
    lha r0, 0x0(r4)
    addi r31, r31, lbl_80404B68@l
    sth r0, 0x84(r29)
    lha r0, 0x2(r30)
    sth r0, 0x86(r29)
    lwz r0, 0x4(r30)
    stw r0, 0x88(r29)
    lbz r0, 0x20(r30)
    stb r0, 0x0(r29)
    lwz r30, 0x1c(r29)
    b @L_8010739C
@L_801072A4:
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
    li r28, 0x0
    sth r0, 0xa(r31)
    b @L_80107334
@L_80107324:
    mr r3, r31
    addi r4, r30, 0xc
    bl fn_801074D4
    addi r28, r28, 0x1
@L_80107334:
    bl fn_800D3088
    cmplw r28, r3
    blt @L_80107324
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
@L_8010739C:
    cmplwi r30, 0x0
    bne @L_801072A4
    lwz r30, 0x20(r29)
    lis r29, lbl_80404B68@ha
    addi r29, r29, lbl_80404B68@l
    b @L_801074AC
@L_801073B4:
    lha r0, 0x50(r30)
    sth r0, 0x0(r29)
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
    li r28, 0x0
    sth r0, 0xa(r31)
    b @L_80107444
@L_80107434:
    mr r3, r29
    addi r4, r30, 0xc
    bl fn_801074D4
    addi r28, r28, 0x1
@L_80107444:
    bl fn_800D3088
    cmplw r28, r3
    blt @L_80107434
    lha r0, 0x0(r29)
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
@L_801074AC:
    cmplwi r30, 0x0
    bne @L_801073B4
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
