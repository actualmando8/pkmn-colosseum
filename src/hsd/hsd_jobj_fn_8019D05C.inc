    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lis r3, lbl_80274AA0@ha
    lwz r0, 0x10(r28)
    addi r31, r3, lbl_80274AA0@l
    cmplwi r0, 0x0
    beq @L_8019D3DC
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_8019D1D0
    lwz r30, 0x10(r28)
    cmplwi r30, 0x0
    beq @L_8019D3D4
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_8019D0CC
    b @L_8019D0E4
    @L_8019D0CC:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
    @L_8019D0E4:
    cmpwi r3, 0x0
    beq @L_8019D3D4
    lhz r3, 0x6(r30)
    subic. r0, r3, 0x1
    bge @L_8019D12C
    cmplwi r30, 0x0
    beq @L_8019D3D4
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_8019D3D4
    @L_8019D12C:
    lhz r3, 0x6(r30)
    addi r0, r3, 0x1
    sth r0, 0x6(r30)
    lhz r0, 0x6(r30)
    cmplwi r0, 0x0
    bne @L_8019D154
    addi r3, r31, 0x54
    addi r5, r31, 0x60
    li r4, 0x9e
    bl fn_80196E10
    @L_8019D154:
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lhz r0, 0x6(r30)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019D17C
    b @L_8019D194
    @L_8019D17C:
    lhz r3, 0x6(r30)
    subi r0, r3, 0x1
    sth r0, 0x6(r30)
    lhz r0, 0x6(r30)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_8019D194:
    cmpwi r0, 0x0
    beq @L_8019D3D4
    cmplwi r30, 0x0
    beq @L_8019D3D4
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_8019D3D4
    @L_8019D1D0:
    lwz r3, 0x10(r28)
    li r0, 0x0
    stw r0, 0xc(r3)
    lwz r29, 0x10(r28)
    cmplwi r29, 0x0
    beq @L_8019D3D4
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    beq @L_8019D3CC
    cmplwi r29, 0x0
    beq @L_8019D208
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    bne @L_8019D210
    @L_8019D208:
    li r3, 0x0
    b @L_8019D264
    @L_8019D210:
    lwz r3, 0xc(r29)
    lwz r0, 0x10(r3)
    cmplw r29, r0
    bne @L_8019D228
    li r3, 0x0
    b @L_8019D264
    @L_8019D228:
    lwz r3, 0xc(r29)
    lwz r3, 0x10(r3)
    b @L_8019D248
    @L_8019D234:
    lwz r0, 0x8(r3)
    cmplw r0, r29
    bne @L_8019D244
    b @L_8019D264
    @L_8019D244:
    lwz r3, 0x8(r3)
    @L_8019D248:
    cmplwi r3, 0x0
    bne @L_8019D234
    addi r5, r31, 0x88
    la r3, lbl_8047DB20(r2)
    li r4, 0x5f8
    bl fn_80196D78
    li r3, 0x0
    @L_8019D264:
    cmplwi r3, 0x0
    beq @L_8019D278
    li r0, 0x0
    stw r0, 0x8(r3)
    b @L_8019D3CC
    @L_8019D278:
    lwz r3, 0xc(r29)
    li r0, 0x0
    stw r0, 0x10(r3)
    b @L_8019D3CC
    @L_8019D288:
    lwz r30, 0x8(r29)
    li r3, 0x0
    cmplwi r29, 0x0
    li r0, 0x0
    stw r3, 0xc(r29)
    stw r0, 0x8(r29)
    beq @L_8019D3C8
    lis r3, 0x1
    lhz r4, 0x4(r29)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_8019D2C8
    b @L_8019D2E0
    @L_8019D2C8:
    lhz r0, 0x4(r29)
    lhz r3, 0x4(r29)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r29)
    srwi r3, r4, 5
    @L_8019D2E0:
    cmpwi r3, 0x0
    beq @L_8019D3C8
    lhz r3, 0x6(r29)
    subic. r0, r3, 0x1
    bge @L_8019D328
    cmplwi r29, 0x0
    beq @L_8019D3C8
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_8019D3C8
    @L_8019D328:
    lhz r3, 0x6(r29)
    addi r0, r3, 0x1
    sth r0, 0x6(r29)
    lhz r0, 0x6(r29)
    cmplwi r0, 0x0
    bne @L_8019D350
    addi r3, r31, 0x54
    addi r5, r31, 0x60
    li r4, 0x9e
    bl fn_80196E10
    @L_8019D350:
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lhz r0, 0x6(r29)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019D378
    b @L_8019D390
    @L_8019D378:
    lhz r3, 0x6(r29)
    subi r0, r3, 0x1
    sth r0, 0x6(r29)
    lhz r0, 0x6(r29)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_8019D390:
    cmpwi r0, 0x0
    beq @L_8019D3C8
    cmplwi r29, 0x0
    beq @L_8019D3C8
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_8019D3C8:
    mr r29, r30
    @L_8019D3CC:
    cmplwi r29, 0x0
    bne @L_8019D288
    @L_8019D3D4:
    li r0, 0x0
    stw r0, 0x10(r28)
    @L_8019D3DC:
    lwz r0, 0xc(r28)
    cmplwi r0, 0x0
    beq @L_8019D518
    cmplwi r28, 0x0
    beq @L_8019D518
    lwz r0, 0xc(r28)
    lwz r29, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_8019D510
    lwz r3, 0xc(r28)
    lwz r0, 0x10(r3)
    cmplw r0, r28
    bne @L_8019D41C
    lwz r3, 0xc(r28)
    stw r29, 0x10(r3)
    b @L_8019D4A8
    @L_8019D41C:
    cmplwi r28, 0x0
    beq @L_8019D430
    lwz r0, 0xc(r28)
    cmplwi r0, 0x0
    bne @L_8019D438
    @L_8019D430:
    li r30, 0x0
    b @L_8019D48C
    @L_8019D438:
    lwz r3, 0xc(r28)
    lwz r0, 0x10(r3)
    cmplw r28, r0
    bne @L_8019D450
    li r30, 0x0
    b @L_8019D48C
    @L_8019D450:
    lwz r3, 0xc(r28)
    lwz r30, 0x10(r3)
    b @L_8019D470
    @L_8019D45C:
    lwz r0, 0x8(r30)
    cmplw r0, r28
    bne @L_8019D46C
    b @L_8019D48C
    @L_8019D46C:
    lwz r30, 0x8(r30)
    @L_8019D470:
    cmplwi r30, 0x0
    bne @L_8019D45C
    addi r5, r31, 0x88
    la r3, lbl_8047DB20(r2)
    li r4, 0x5f8
    bl fn_80196D78
    li r30, 0x0
    @L_8019D48C:
    cmplwi r30, 0x0
    bne @L_8019D4A4
    la r3, lbl_8047DB20(r2)
    li r4, 0x57b
    la r5, lbl_8047DB28(r2)
    bl fn_80196E10
    @L_8019D4A4:
    stw r29, 0x8(r30)
    @L_8019D4A8:
    lwz r6, 0xc(r28)
    b @L_8019D500
    @L_8019D4B0:
    lis r4, 0x9000
    lwz r5, 0x10(r6)
    subi r4, r4, 0x1
    b @L_8019D4DC
    @L_8019D4C0:
    lwz r0, 0x14(r5)
    lwz r3, 0x14(r5)
    slwi r0, r0, 10
    lwz r5, 0x8(r5)
    or r0, r3, r0
    rlwinm r0, r0, 0, 1, 3
    or r4, r4, r0
    @L_8019D4DC:
    cmplwi r5, 0x0
    bne @L_8019D4C0
    lwz r0, 0x14(r6)
    andc. r0, r0, r4
    beq @L_8019D508
    lwz r0, 0x14(r6)
    and r0, r0, r4
    stw r0, 0x14(r6)
    lwz r6, 0x8(r6)
    @L_8019D500:
    cmplwi r6, 0x0
    bne @L_8019D4B0
    @L_8019D508:
    li r0, 0x0
    stw r0, 0xc(r28)
    @L_8019D510:
    li r0, 0x0
    stw r0, 0x8(r28)
    @L_8019D518:
    lwz r0, 0x14(r28)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_8019D548
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_8019D548
    lwz r3, 0x18(r28)
    bl fn_80199264
    li r0, 0x0
    stw r0, 0x18(r28)
    @L_8019D548:
    lwz r0, 0x80(r28)
    cmplwi r0, 0x0
    beq @L_8019D564
    lwz r3, 0x80(r28)
    bl fn_801AE50C
    li r0, 0x0
    stw r0, 0x80(r28)
    @L_8019D564:
    lwz r0, 0x7c(r28)
    cmplwi r0, 0x0
    beq @L_8019D580
    lwz r3, 0x7c(r28)
    bl fn_801C25E4
    li r0, 0x0
    stw r0, 0x7c(r28)
    @L_8019D580:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
