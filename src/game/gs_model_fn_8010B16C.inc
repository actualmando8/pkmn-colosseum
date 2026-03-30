    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
@L_8010B194:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8010B2C8
    cmplwi r30, 0x0
    li r28, 0x0
    beq @L_8010B1C0
    mr r12, r30
    mr r3, r31
    mtctr r12
    bctrl
    mr r28, r3
@L_8010B1C0:
    cmplwi r28, 0x0
    beq @L_8010B2C8
    bne @L_8010B1D4
    li r0, 0x0
    b @L_8010B2C4
@L_8010B1D4:
    mr r3, r28
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B1F0
    li r0, 0x33d
    b @L_8010B2C4
@L_8010B1F0:
    mr r3, r28
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B20C
    li r0, 0x33d
    b @L_8010B2C4
@L_8010B20C:
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    cmplwi r29, 0xc9
    bne @L_8010B27C
    mr r3, r28
    bl fn_8011F5B0
    bl fn_80122334
    rlwinm r29, r3, 0, 24, 31
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B264
    lis r3, lbl_8035B478@ha
    rlwinm r0, r29, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    lhzx r0, r3, r0
    b @L_8010B2C4
@L_8010B264:
    lis r3, lbl_8035B478@ha
    rlwinm r4, r29, 2, 0, 29
    addi r0, r3, lbl_8035B478@l
    add r3, r0, r4
    lhz r0, 0x2(r3)
    b @L_8010B2C4
@L_8010B27C:
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B2AC
    mr r4, r29
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    b @L_8010B2C4
@L_8010B2AC:
    mr r4, r29
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x1
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
@L_8010B2C4:
    mr r29, r0
@L_8010B2C8:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8010B3CC
    lwz r6, lbl_8047AD4C(r13)
    li r3, 0x0
    lwz r5, lbl_8047AD48(r13)
    mr r4, r6
    mtctr r5
    cmpwi r5, 0x0
    ble @L_8010B308
@L_8010B2F0:
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8010B308
    addi r4, r4, 0x10
    addi r3, r3, 0x1
    bdnz @L_8010B2F0
@L_8010B308:
    mr r4, r6
    mtctr r3
    cmpwi r3, 0x0
    ble @L_8010B334
@L_8010B318:
    lbz r0, 0x6(r4)
    cmplwi r0, 0x1
    bne @L_8010B32C
    li r0, 0x1
    b @L_8010B338
@L_8010B32C:
    addi r4, r4, 0x10
    bdnz @L_8010B318
@L_8010B334:
    li r0, 0x0
@L_8010B338:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B34C
    li r3, 0x0
    b @L_8010B540
@L_8010B34C:
    mr r3, r6
    li r4, 0x0
    mtctr r5
    cmpwi r5, 0x0
    ble @L_8010B378
@L_8010B360:
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8010B378
    addi r3, r3, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010B360
@L_8010B378:
    mr r3, r6
    li r5, 0x0
    mtctr r4
    cmpwi r4, 0x0
    ble @L_8010B3B4
@L_8010B38C:
    lbz r0, 0x6(r3)
    cmplwi r0, 0x0
    bne @L_8010B3A8
    rlwinm r3, r5, 4, 0, 27
    addi r0, r3, 0x4
    lhzx r29, r6, r0
    b @L_8010B3B4
@L_8010B3A8:
    addi r3, r3, 0x10
    addi r5, r5, 0x1
    bdnz @L_8010B38C
@L_8010B3B4:
    cmpw r5, r4
    bne @L_8010B43C
    li r3, 0x5c0
    bl fn_8017B1CC
    li r3, 0x0
    b @L_8010B540
@L_8010B3CC:
    mr r3, r29
    bl fn_8010C3FC
    lwz r5, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B408
@L_8010B3EC:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010B3FC
    b @L_8010B40C
@L_8010B3FC:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010B3EC
@L_8010B408:
    li r4, -0x1
@L_8010B40C:
    cmpwi r4, 0x0
    blt @L_8010B42C
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_8010BD6C
    li r29, 0x0
    b @L_8010B194
@L_8010B42C:
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_8010BD6C
@L_8010B43C:
    mr r3, r29
    bl fn_8010C3FC
    mr r28, r3
    cmplwi r28, 0x0
    bne @L_8010B458
    li r29, 0x0
    b @L_8010B194
@L_8010B458:
    lwz r5, lbl_8047AD4C(r13)
    li r3, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r4, r5
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B48C
@L_8010B474:
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8010B48C
    addi r4, r4, 0x10
    addi r3, r3, 0x1
    bdnz @L_8010B474
@L_8010B48C:
    mtctr r3
    cmpwi r3, 0x0
    ble @L_8010B4B4
@L_8010B498:
    lbz r0, 0x6(r5)
    cmplwi r0, 0x1
    bne @L_8010B4AC
    li r0, 0x1
    b @L_8010B4B8
@L_8010B4AC:
    addi r5, r5, 0x10
    bdnz @L_8010B498
@L_8010B4B4:
    li r0, 0x0
@L_8010B4B8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B4CC
    li r3, 0x1
    b @L_8010B540
@L_8010B4CC:
    mr r3, r29
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B50C
@L_8010B4F0:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010B500
    b @L_8010B510
@L_8010B500:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010B4F0
@L_8010B50C:
    li r4, -0x1
@L_8010B510:
    rlwinm r0, r4, 4, 0, 27
    lis r3, fn_8010B5C4@ha
    add r4, r6, r0
    rlwinm r7, r29, 0, 16, 31
    li r0, 0x1
    addi r5, r3, fn_8010B5C4@l
    stb r0, 0x6(r4)
    mr r4, r28
    li r3, 0x5c0
    li r6, 0x0
    bl fn_8017B000
    li r3, 0x1
@L_8010B540:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

