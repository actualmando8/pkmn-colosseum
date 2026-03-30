    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r28, r4
    lis r4, lbl_80274EE0@ha
    mr r30, r3
    addi r31, r4, lbl_80274EE0@l
    lwz r29, 0x4(r28)
    cmplwi r29, 0x0
    beq @L_801AD410
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801AD39C
    lwz r3, 0x0(r29)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801AD3D8
@L_801AD39C:
    lwz r0, lbl_8047B2E8(r13)
    cmplwi r0, 0x0
    beq @L_801AD3B0
    lwz r3, lbl_8047B2E8(r13)
    b @L_801AD3B8
@L_801AD3B0:
    lis r3, lbl_8036CCD0@ha
    addi r3, r3, lbl_8036CCD0@l
@L_801AD3B8:
    bl fn_80193828
    mr. r26, r3
    bne @L_801AD3F4
    addi r3, r2, -23016
    li r4, 0x2a9
    addi r5, r2, -22928
    bl fn_80196E10
    b @L_801AD3F4
@L_801AD3D8:
    bl fn_80193828
    mr. r26, r3
    bne @L_801AD3F4
    addi r3, r2, -23016
    li r4, 0x247
    addi r5, r2, -22928
    bl fn_80196E10
@L_801AD3F4:
    lwz r5, 0x0(r26)
    mr r3, r26
    mr r4, r29
    lwz r12, 0x44(r5)
    mtctr r12
    bctrl
    b @L_801AD414
@L_801AD410:
    li r26, 0x0
@L_801AD414:
    stw r26, 0x4(r30)
    lwz r0, 0x8(r28)
    stw r0, 0x8(r30)
    lhz r0, 0xc(r28)
    sth r0, 0xc(r30)
    lhz r0, 0xe(r28)
    sth r0, 0xe(r30)
    lwz r0, 0x10(r28)
    stw r0, 0x10(r30)
    lhz r0, 0xc(r30)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    beq @L_801AD464
    bge @L_801AD458
    cmpwi r0, 0x0
    beq @L_801AD5FC
    b @L_801AD5EC
@L_801AD458:
    cmpwi r0, 0x2000
    beq @L_801AD530
    b @L_801AD5EC
@L_801AD464:
    lwz r28, 0x14(r28)
    li r3, 0x20
    bl fn_80193B10
    mr. r29, r3
    bne @L_801AD488
    addi r5, r31, 0x264
    addi r3, r2, -23016
    li r4, 0x1e2
    bl fn_80196E10
@L_801AD488:
    mr r3, r29
    li r4, 0x0
    li r5, 0x20
    bl memset
    lhz r0, 0x0(r28)
    sth r0, 0x0(r29)
    lhz r0, 0x2(r28)
    sth r0, 0x2(r29)
    lwz r0, 0x4(r28)
    stw r0, 0x4(r29)
    lwz r0, 0x8(r28)
    stw r0, 0x8(r29)
    lwz r0, 0xc(r28)
    stw r0, 0xc(r29)
    lwz r0, 0x10(r28)
    stw r0, 0x10(r29)
    lwz r0, 0x14(r28)
    stw r0, 0x14(r29)
    lwz r0, 0x18(r28)
    stw r0, 0x18(r29)
    lhz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801AD520
    lhz r0, 0x2(r29)
    rlwinm r3, r0, 2, 0, 29
    bl fn_801A6928
    stw r3, 0x1c(r29)
    li r4, 0x0
    b @L_801AD510
@L_801AD4FC:
    lfs f0, -22988(r2)
    rlwinm r0, r4, 2, 0, 29
    lwz r3, 0x1c(r29)
    addi r4, r4, 0x1
    stfsx f0, r3, r0
@L_801AD510:
    lhz r0, 0x2(r29)
    cmpw r4, r0
    blt @L_801AD4FC
    b @L_801AD528
@L_801AD520:
    lfs f0, -22988(r2)
    stfs f0, 0x1c(r29)
@L_801AD528:
    stw r29, 0x14(r30)
    b @L_801AD5FC
@L_801AD530:
    lwz r26, 0x14(r28)
    li r0, 0x0
    addi r27, r1, 0x8
    cmplwi r26, 0x0
    stw r0, 0x8(r1)
    bne @L_801AD5D4
    li r0, 0x0
    b @L_801AD5E4
@L_801AD550:
    li r0, 0x0
    addi r29, r1, 0xc
    stw r0, 0xc(r1)
    lwz r28, 0x0(r26)
    b @L_801AD5AC
@L_801AD564:
    li r3, 0xc
    bl fn_80193B10
    mr. r25, r3
    bne @L_801AD584
    addi r5, r31, 0x38
    addi r3, r2, -23016
    li r4, 0x1a9
    bl fn_80196E10
@L_801AD584:
    mr r3, r25
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r25, 0x0(r29)
    lfs f0, 0x4(r28)
    addi r28, r28, 0x8
    lwz r3, 0x0(r29)
    stfs f0, 0x8(r3)
    lwz r29, 0x0(r29)
@L_801AD5AC:
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    bne @L_801AD564
    bl fn_801A3F48
    stw r3, 0x0(r27)
    addi r26, r26, 0x4
    lwz r0, 0xc(r1)
    lwz r3, 0x0(r27)
    stw r0, 0x4(r3)
    lwz r27, 0x0(r27)
@L_801AD5D4:
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    bne @L_801AD550
    lwz r0, 0x8(r1)
@L_801AD5E4:
    stw r0, 0x14(r30)
    b @L_801AD5FC
@L_801AD5EC:
    addi r5, r31, 0x270
    addi r3, r2, -23016
    li r4, 0x22a
    bl fn_80196D78
@L_801AD5FC:
    li r3, 0x1
    bl fn_8019C6EC
    lmw r25, 0x14(r1)
    li r3, 0x0
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
