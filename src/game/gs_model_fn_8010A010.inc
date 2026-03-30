    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010A034
    li r3, 0x0
    b @L_8010A1FC
@L_8010A034:
    bne @L_8010A040
    li r0, 0x0
    b @L_8010A140
@L_8010A040:
    lbz r0, 0x1(r31)
    li r5, 0x0
    stb r5, 0x8(r1)
    cmplwi r0, 0x0
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    beq @L_8010A0D0
    lbz r0, 0x4(r31)
    cmplw r0, r5
    bne @L_8010A0C8
    cmplwi r0, 0x0
    beq @L_8010A0B4
    lhz r3, 0x8(r31)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A0C8
    lhz r3, 0xa(r31)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_8010A0C8
    lwz r0, 0xc(r31)
    cmplw r0, r5
    bne @L_8010A0C8
    lbz r3, 0x12(r31)
    lbz r0, 0x16(r1)
    cmplw r3, r0
    bne @L_8010A0C8
    li r0, 0x1
    b @L_8010A140
@L_8010A0B4:
    lwz r0, 0x8(r31)
    cmplw r0, r4
    bne @L_8010A0C8
    li r0, 0x1
    b @L_8010A140
@L_8010A0C8:
    li r0, 0x0
    b @L_8010A140
@L_8010A0D0:
    lbz r0, 0x14(r31)
    cmplw r0, r5
    bne @L_8010A13C
    cmplwi r0, 0x0
    beq @L_8010A128
    lhz r3, 0x18(r31)
    lhz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8010A13C
    lhz r3, 0x1a(r31)
    lhz r0, 0xe(r1)
    cmplw r3, r0
    bne @L_8010A13C
    lwz r0, 0x1c(r31)
    cmplw r0, r5
    bne @L_8010A13C
    lbz r3, 0x22(r31)
    lbz r0, 0x16(r1)
    cmplw r3, r0
    bne @L_8010A13C
    li r0, 0x1
    b @L_8010A140
@L_8010A128:
    lwz r0, 0x18(r31)
    cmplw r0, r4
    bne @L_8010A13C
    li r0, 0x1
    b @L_8010A140
@L_8010A13C:
    li r0, 0x0
@L_8010A140:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010A154
    li r3, 0x0
    b @L_8010A1FC
@L_8010A154:
    lbz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_8010A16C
    blt @L_8010A174
    cmpwi r0, 0x3
    b @L_8010A174
@L_8010A16C:
    li r0, 0x1
    stb r0, 0x0(r31)
@L_8010A174:
    li r3, 0x0
    li r0, 0x1
    stb r3, 0x4(r31)
    stw r4, 0x8(r31)
    stb r0, 0x1(r31)
    lwz r3, 0x28(r31)
    cmplwi r3, 0x0
    beq @L_8010A1B4
    bl fn_800F04BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010A1AC
    li r3, 0x1
    b @L_8010A1FC
@L_8010A1AC:
    lwz r3, 0x28(r31)
    bl fn_800F0494
@L_8010A1B4:
    bl fn_800FF560
    lis r8, fn_8010A88C@ha
    mr r4, r3
    addi r8, r8, fn_8010A88C@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    stw r3, 0x28(r31)
    lwz r3, 0x28(r31)
    cmplwi r3, 0x0
    beq @L_8010A1F8
    mr r5, r31
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800F0654
@L_8010A1F8:
    li r3, 0x0
@L_8010A1FC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
