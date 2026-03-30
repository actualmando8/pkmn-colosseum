    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r5, lbl_80274AA0@ha
    stw r0, 0x24(r1)
    li r0, 0x0
    stmw r27, 0xc(r1)
    mr r29, r3
    addi r31, r5, lbl_80274AA0@l
    mr r30, r4
    stw r0, 0x10(r3)
    li r3, 0x0
    li r0, 0x0
    stw r3, 0x8(r29)
    stw r0, 0xc(r29)
    lwz r3, 0x14(r29)
    lwz r0, 0x4(r4)
    or r0, r3, r0
    stw r0, 0x14(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 17, 17
    beq @L_801A10F8
    lwz r0, 0x10(r30)
    stw r0, 0x18(r29)
    b @L_801A113C
    @L_801A10F8:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_801A1130
    lwz r0, 0x10(r30)
    stw r0, 0x18(r29)
    lwz r3, 0x10(r30)
    b @L_801A1124
    @L_801A1114:
    lwz r0, 0x4(r3)
    oris r0, r0, 0x8000
    stw r0, 0x4(r3)
    lwz r3, 0x0(r3)
    @L_801A1124:
    cmplwi r3, 0x0
    bne @L_801A1114
    b @L_801A113C
    @L_801A1130:
    lwz r3, 0x10(r30)
    bl fn_801992D8
    stw r3, 0x18(r29)
    @L_801A113C:
    lwz r3, 0x3c(r30)
    bl fn_801AE5E8
    mr r0, r3
    addi r3, r29, 0x44
    stw r0, 0x80(r29)
    lfs f0, 0x14(r30)
    stfs f0, 0x1c(r29)
    lfs f0, 0x18(r30)
    stfs f0, 0x20(r29)
    lfs f0, 0x1c(r30)
    stfs f0, 0x24(r29)
    lwz r4, 0x20(r30)
    lwz r0, 0x24(r30)
    stw r4, 0x2c(r29)
    stw r0, 0x30(r29)
    lwz r0, 0x28(r30)
    stw r0, 0x34(r29)
    lwz r4, 0x2c(r30)
    lwz r0, 0x30(r30)
    stw r4, 0x38(r29)
    stw r0, 0x3c(r29)
    lwz r0, 0x34(r30)
    stw r0, 0x40(r29)
    bl fn_800A2D38
    li r0, 0x0
    stw r0, 0x74(r29)
    lwz r0, 0x38(r30)
    cmplwi r0, 0x0
    beq @L_801A11C8
    bl fn_801A8524
    stw r3, 0x78(r29)
    li r5, 0x30
    lwz r3, 0x78(r29)
    lwz r4, 0x38(r30)
    bl memcpy
    @L_801A11C8:
    mr r4, r30
    mr r5, r29
    li r3, 0x0
    bl fn_8019C264
    stw r30, 0x84(r29)
    lwz r0, 0x4(r30)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A13B4
    lwz r30, 0x8(r30)
    b @L_801A13AC
    @L_801A11F0:
    cmplwi r30, 0x0
    bne @L_801A1200
    li r28, 0x0
    b @L_801A1290
    @L_801A1200:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801A121C
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801A1258
    @L_801A121C:
    lwz r0, lbl_8047B298(r13)
    cmplwi r0, 0x0
    beq @L_801A1230
    lwz r3, lbl_8047B298(r13)
    b @L_801A1238
    @L_801A1230:
    lis r3, lbl_8036C8E0@ha
    addi r3, r3, lbl_8036C8E0@l
    @L_801A1238:
    bl fn_80193828
    mr. r28, r3
    bne @L_801A1274
    la r3, lbl_8047DB20(r2)
    li r4, 0x7df
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    b @L_801A1274
    @L_801A1258:
    bl fn_80193828
    mr. r28, r3
    bne @L_801A1274
    la r3, lbl_8047DB20(r2)
    li r4, 0x3d5
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    @L_801A1274:
    lwz r6, 0x0(r28)
    mr r3, r28
    mr r4, r30
    mr r5, r29
    lwz r12, 0x3c(r6)
    mtctr r12
    bctrl
    @L_801A1290:
    mr. r27, r28
    bne @L_801A12A0
    li r3, -0x1
    b @L_801A13B8
    @L_801A12A0:
    cmplwi r29, 0x0
    beq @L_801A13A8
    cmplwi r27, 0x0
    beq @L_801A13A8
    lwz r0, 0xc(r27)
    cmplwi r0, 0x0
    beq @L_801A12D8
    addi r3, r31, 0x178
    crxor 6, 6, 6
    bl OSReport
    addi r5, r31, 0x194
    la r3, lbl_8047DB20(r2)
    li r4, 0x552
    bl fn_80196E10
    @L_801A12D8:
    lwz r0, 0x8(r27)
    cmplwi r0, 0x0
    beq @L_801A1300
    addi r3, r31, 0x1ac
    crxor 6, 6, 6
    bl OSReport
    addi r5, r31, 0x1cc
    la r3, lbl_8047DB20(r2)
    li r4, 0x553
    bl fn_80196E10
    @L_801A1300:
    lwz r0, 0x10(r29)
    cmplwi r0, 0x0
    bne @L_801A1314
    stw r27, 0x10(r29)
    b @L_801A1364
    @L_801A1314:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A1330
    addi r5, r31, 0x1e0
    la r3, lbl_8047DB20(r2)
    li r4, 0x559
    bl fn_80196E10
    @L_801A1330:
    lwz r28, 0x10(r29)
    b @L_801A1354
    @L_801A1338:
    cmplw r28, r27
    bne @L_801A1350
    addi r5, r31, 0x200
    la r3, lbl_8047DB20(r2)
    li r4, 0x55c
    bl fn_80196E10
    @L_801A1350:
    lwz r28, 0x8(r28)
    @L_801A1354:
    lwz r0, 0x8(r28)
    cmplwi r0, 0x0
    bne @L_801A1338
    stw r27, 0x8(r28)
    @L_801A1364:
    stw r29, 0xc(r27)
    mr r4, r29
    lwz r0, 0x14(r27)
    lwz r3, 0x14(r27)
    slwi r0, r0, 10
    or r0, r3, r0
    rlwinm r3, r0, 0, 1, 3
    b @L_801A13A0
    @L_801A1384:
    lwz r0, 0x14(r4)
    andc. r0, r3, r0
    beq @L_801A13A8
    lwz r0, 0x14(r4)
    or r0, r0, r3
    stw r0, 0x14(r4)
    lwz r4, 0xc(r4)
    @L_801A13A0:
    cmplwi r4, 0x0
    bne @L_801A1384
    @L_801A13A8:
    lwz r30, 0xc(r30)
    @L_801A13AC:
    cmplwi r30, 0x0
    bne @L_801A11F0
    @L_801A13B4:
    li r3, 0x0
    @L_801A13B8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
