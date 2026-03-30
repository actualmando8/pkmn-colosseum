    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    mr r31, r4
    cmplwi r30, 0x0
    bne @L_80109CB4
    li r3, 0x0
    b @L_80109FF8
@L_80109CB4:
    bne @L_80109CC0
    li r0, 0x0
    b @L_80109E84
@L_80109CC0:
    li r0, 0x1
    mr r3, r31
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
    mr r3, r31
    sth r0, 0xc(r1)
    li r4, 0x0
    li r5, 0xc2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_80109D48
    mr r3, r31
    li r4, 0x3e
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80109D3C
    li r0, 0x87
    sth r0, 0xe(r1)
    b @L_80109D50
@L_80109D3C:
    li r0, 0x25
    sth r0, 0xe(r1)
    b @L_80109D50
@L_80109D48:
    li r0, 0x0
    sth r0, 0xe(r1)
@L_80109D50:
    mr r3, r31
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    stw r3, 0x10(r1)
    mr r3, r31
    li r4, 0x0
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    lbz r0, 0x1(r30)
    rlwinm r4, r3, 0, 24, 31
    stb r4, 0x16(r1)
    cmplwi r0, 0x0
    beq @L_80109E0C
    lbz r3, 0x4(r30)
    lbz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_80109E04
    cmplwi r3, 0x0
    beq @L_80109DEC
    lhz r3, 0x8(r30)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_80109E04
    lhz r3, 0xa(r30)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_80109E04
    lwz r3, 0xc(r30)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    bne @L_80109E04
    lbz r0, 0x12(r30)
    cmplw r0, r4
    bne @L_80109E04
    li r0, 0x1
    b @L_80109E84
@L_80109DEC:
    lwz r3, 0x8(r30)
    lwz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_80109E04
    li r0, 0x1
    b @L_80109E84
@L_80109E04:
    li r0, 0x0
    b @L_80109E84
@L_80109E0C:
    lbz r3, 0x14(r30)
    lbz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_80109E80
    cmplwi r3, 0x0
    beq @L_80109E68
    lhz r3, 0x18(r30)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_80109E80
    lhz r3, 0x1a(r30)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_80109E80
    lwz r3, 0x1c(r30)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    bne @L_80109E80
    lbz r0, 0x22(r30)
    cmplw r0, r4
    bne @L_80109E80
    li r0, 0x1
    b @L_80109E84
@L_80109E68:
    lwz r3, 0x18(r30)
    lwz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_80109E80
    li r0, 0x1
    b @L_80109E84
@L_80109E80:
    li r0, 0x0
@L_80109E84:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80109E98
    li r3, 0x0
    b @L_80109FF8
@L_80109E98:
    lbz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_80109EB0
    blt @L_80109EB8
    cmpwi r0, 0x3
    b @L_80109EB8
@L_80109EB0:
    li r0, 0x1
    stb r0, 0x0(r30)
@L_80109EB8:
    li r0, 0x1
    mr r3, r31
    stb r0, 0x4(r30)
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    sth r4, 0x10(r30)
    li r5, 0x66
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r31
    sth r0, 0x8(r30)
    li r4, 0x0
    li r5, 0xc2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_80109F40
    mr r3, r31
    li r4, 0x3e
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80109F34
    li r0, 0x87
    sth r0, 0xa(r30)
    b @L_80109F48
@L_80109F34:
    li r0, 0x25
    sth r0, 0xa(r30)
    b @L_80109F48
@L_80109F40:
    li r0, 0x0
    sth r0, 0xa(r30)
@L_80109F48:
    mr r3, r31
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    stw r3, 0xc(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
    li r0, 0x1
    stb r3, 0x12(r30)
    stb r0, 0x1(r30)
    lwz r3, 0x28(r30)
    cmplwi r3, 0x0
    beq @L_80109FB0
    bl fn_800F04BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80109FA8
    li r3, 0x1
    b @L_80109FF8
@L_80109FA8:
    lwz r3, 0x28(r30)
    bl fn_800F0494
@L_80109FB0:
    bl fn_800FF560
    lis r8, fn_8010A88C@ha
    mr r4, r3
    addi r8, r8, fn_8010A88C@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    stw r3, 0x28(r30)
    lwz r3, 0x28(r30)
    cmplwi r3, 0x0
    beq @L_80109FF4
    mr r5, r30
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800F0654
@L_80109FF4:
    li r3, 0x1
@L_80109FF8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
