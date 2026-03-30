    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r25, r5
    cmplwi r3, 0x0
    bne @L_801083A8
    lis r3, lbl_80404B68@ha
    rlwinm r29, r4, 0, 16, 31
    rlwinm r28, r25, 0, 16, 31
    li r26, 0x0
    addi r30, r3, lbl_80404B68@l
    li r31, 0x0
@L_8010822C:
    lwz r0, lbl_8047AD1C(r13)
    add r27, r0, r31
    lbz r0, 0x4(r27)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_80108394
    lha r0, 0x6(r27)
    cmpw r0, r29
    bne @L_80108394
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8010826C
    li r0, 0x0
    stw r0, 0xc(r27)
    sth r0, 0x10(r27)
    b @L_80108288
@L_8010826C:
    addi r3, r27, 0xc
    li r4, 0x0
    li r5, 0x3c
    bl memset
    mr r3, r28
    bl fn_8005D830
    stw r3, 0xc(r27)
@L_80108288:
    lis r3, lbl_80404B68@ha
    lha r0, 0x50(r27)
    addi r3, r3, lbl_80404B68@l
    sth r0, 0x0(r3)
    lha r0, 0x52(r27)
    sth r0, 0x2(r30)
    lwz r0, 0x64(r27)
    stw r0, 0x4(r30)
    lfs f0, 0x68(r27)
    stfs f0, 0xc(r30)
    lfs f0, 0x6c(r27)
    stfs f0, 0x10(r30)
    lbz r0, 0x4(r27)
    stb r0, 0x20(r30)
    lwz r0, 0x58(r27)
    stw r0, 0x14(r30)
    lha r0, 0x5c(r27)
    sth r0, 0x18(r30)
    lha r0, 0x5e(r27)
    sth r0, 0x1a(r30)
    lha r0, 0x60(r27)
    sth r0, 0x1c(r30)
    lha r0, 0x62(r27)
    sth r0, 0x1e(r30)
    lha r3, 0x6(r27)
    bl fn_8005D934
    lha r0, 0x2(r3)
    sth r0, 0x8(r30)
    lha r3, 0x6(r27)
    bl fn_8005D934
    lha r0, 0x4(r3)
    li r23, 0x0
    sth r0, 0xa(r30)
    lis r3, lbl_80404B68@ha
    addi r24, r3, lbl_80404B68@l
    b @L_80108328
@L_80108318:
    mr r3, r24
    addi r4, r27, 0xc
    bl fn_801074D4
    addi r23, r23, 0x1
@L_80108328:
    bl fn_800D3088
    cmplw r23, r3
    blt @L_80108318
    lis r3, lbl_80404B68@ha
    addi r3, r3, lbl_80404B68@l
    lha r0, 0x0(r3)
    sth r0, 0x50(r27)
    lha r0, 0x2(r30)
    sth r0, 0x52(r27)
    lwz r0, 0x4(r30)
    stw r0, 0x64(r27)
    lfs f0, 0xc(r30)
    stfs f0, 0x68(r27)
    lfs f0, 0x10(r30)
    stfs f0, 0x6c(r27)
    lbz r0, 0x20(r30)
    stb r0, 0x4(r27)
    lwz r0, 0x14(r30)
    stw r0, 0x58(r27)
    lha r0, 0x18(r30)
    sth r0, 0x5c(r27)
    lha r0, 0x1a(r30)
    sth r0, 0x5e(r27)
    lha r0, 0x1c(r30)
    sth r0, 0x60(r27)
    lha r0, 0x1e(r30)
    sth r0, 0x62(r27)
@L_80108394:
    addi r31, r31, 0x78
    addi r26, r26, 0x1
    cmpwi r26, 0x168
    blt @L_8010822C
    b @L_80108504
@L_801083A8:
    bl fn_801046C8
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80108504
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_801083D4
    li r0, 0x0
    stw r0, 0xc(r30)
    sth r0, 0x10(r30)
    b @L_801083F0
@L_801083D4:
    addi r3, r30, 0xc
    li r4, 0x0
    li r5, 0x3c
    bl memset
    rlwinm r3, r25, 0, 16, 31
    bl fn_8005D830
    stw r3, 0xc(r30)
@L_801083F0:
    lis r3, lbl_80404B68@ha
    lha r0, 0x50(r30)
    addi r31, r3, lbl_80404B68@l
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
    lis r4, lbl_80404B68@ha
    lha r0, 0x2(r3)
    addi r3, r4, lbl_80404B68@l
    sth r0, 0x8(r3)
    lha r3, 0x6(r30)
    bl fn_8005D934
    lis r4, lbl_80404B68@ha
    lha r0, 0x4(r3)
    addi r25, r4, lbl_80404B68@l
    li r23, 0x0
    sth r0, 0xa(r25)
    b @L_80108498
@L_80108488:
    mr r3, r25
    addi r4, r30, 0xc
    bl fn_801074D4
    addi r23, r23, 0x1
@L_80108498:
    bl fn_800D3088
    cmplw r23, r3
    blt @L_80108488
    lis r3, lbl_80404B68@ha
    addi r3, r3, lbl_80404B68@l
    lha r0, 0x0(r3)
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
@L_80108504:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

