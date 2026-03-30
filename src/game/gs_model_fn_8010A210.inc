    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    mr r30, r4
    cmplwi r31, 0x0
    bne @L_8010A23C
    li r3, 0x0
    b @L_8010A408
@L_8010A23C:
    li r0, 0x1
    mr r3, r30
    stb r0, 0x8(r1)
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    sth r4, 0x14(r1)
    li r5, 0x66
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r30
    sth r0, 0xc(r1)
    li r4, 0x0
    li r5, 0xc2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8010A2C4
    mr r3, r30
    li r4, 0x3e
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8010A2B8
    li r0, 0x87
    sth r0, 0xe(r1)
    b @L_8010A2CC
@L_8010A2B8:
    li r0, 0x25
    sth r0, 0xe(r1)
    b @L_8010A2CC
@L_8010A2C4:
    li r0, 0x0
    sth r0, 0xe(r1)
@L_8010A2CC:
    mr r3, r30
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    stw r3, 0x10(r1)
    mr r3, r30
    li r4, 0x0
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    lbz r0, 0x1(r31)
    rlwinm r4, r3, 0, 24, 31
    stb r4, 0x16(r1)
    cmplwi r0, 0x0
    beq @L_8010A38C
    lbz r3, 0x4(r31)
    lbz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_8010A380
    cmplwi r3, 0x0
    beq @L_8010A368
    lhz r3, 0x8(r31)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A380
    lhz r3, 0xa(r31)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_8010A380
    lwz r3, 0xc(r31)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    bne @L_8010A380
    lbz r0, 0x12(r31)
    cmplw r0, r4
    bne @L_8010A380
    li r0, 0x1
    b @L_8010A384
@L_8010A368:
    lwz r3, 0x8(r31)
    lwz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A380
    li r0, 0x1
    b @L_8010A384
@L_8010A380:
    li r0, 0x0
@L_8010A384:
    mr r3, r0
    b @L_8010A408
@L_8010A38C:
    lbz r3, 0x14(r31)
    lbz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_8010A400
    cmplwi r3, 0x0
    beq @L_8010A3E8
    lhz r3, 0x18(r31)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A400
    lhz r3, 0x1a(r31)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_8010A400
    lwz r3, 0x1c(r31)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    bne @L_8010A400
    lbz r0, 0x22(r31)
    cmplw r0, r4
    bne @L_8010A400
    li r0, 0x1
    b @L_8010A404
@L_8010A3E8:
    lwz r3, 0x18(r31)
    lwz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A400
    li r0, 0x1
    b @L_8010A404
@L_8010A400:
    li r0, 0x0
@L_8010A404:
    mr r3, r0
@L_8010A408:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
