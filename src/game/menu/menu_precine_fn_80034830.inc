    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r5
    lis r5, lbl_803A3334@ha
    lwz r6, lbl_8047A430(r13)
    addi r5, r5, lbl_803A3334@l
    li r11, 0x1
    lbz r8, 0x24(r5)
    li r10, 0x0
    lbz r9, 0x26(r5)
    lbz r7, 0x8(r5)
    extsb r8, r8
    extsb r9, r9
    li r0, 0x2
    mtctr r0
@L_80034878:
    lbz r0, 0x41e4(r6)
    cmplw r0, r7
    bne @L_800348A4
    lbz r0, 0x41e5(r6)
    cmpw r0, r8
    bne @L_800348A4
    lbz r0, 0x41e6(r6)
    cmpw r0, r9
    bne @L_800348A4
    li r11, 0x0
    b @L_80034960
@L_800348A4:
    lbz r0, 0x42ae(r6)
    cmplw r0, r7
    bne @L_800348D0
    lbz r0, 0x42af(r6)
    cmpw r0, r8
    bne @L_800348D0
    lbz r0, 0x42b0(r6)
    cmpw r0, r9
    bne @L_800348D0
    li r11, 0x0
    b @L_80034960
@L_800348D0:
    lbz r0, 0x4378(r6)
    cmplw r0, r7
    bne @L_800348FC
    lbz r0, 0x4379(r6)
    cmpw r0, r8
    bne @L_800348FC
    lbz r0, 0x437a(r6)
    cmpw r0, r9
    bne @L_800348FC
    li r11, 0x0
    b @L_80034960
@L_800348FC:
    lbz r0, 0x4442(r6)
    cmplw r0, r7
    bne @L_80034928
    lbz r0, 0x4443(r6)
    cmpw r0, r8
    bne @L_80034928
    lbz r0, 0x4444(r6)
    cmpw r0, r9
    bne @L_80034928
    li r11, 0x0
    b @L_80034960
@L_80034928:
    lbz r0, 0x450c(r6)
    cmplw r0, r7
    bne @L_80034954
    lbz r0, 0x450d(r6)
    cmpw r0, r8
    bne @L_80034954
    lbz r0, 0x450e(r6)
    cmpw r0, r9
    bne @L_80034954
    li r11, 0x0
    b @L_80034960
@L_80034954:
    addi r6, r6, 0x3f2
    addi r10, r10, 0x4
    bdnz @L_80034878
@L_80034960:
    rlwinm r0, r11, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80034B44
    li r6, 0x0
    cmpwi r6, 0x9
    bge @L_80034A28
    mr r10, r6
@L_8003497C:
    lwz r7, lbl_8047A430(r13)
    li r0, 0x19
    add r7, r7, r10
    addi r9, r7, 0x41e0
    addi r8, r7, 0x42aa
    mtctr r0
@L_80034994:
    lwz r7, 0x4(r8)
    lwzu r0, 0x8(r8)
    stw r7, 0x4(r9)
    stwu r0, 0x8(r9)
    bdnz @L_80034994
    lhz r7, 0x4(r8)
    li r0, 0x19
    sth r7, 0x4(r9)
    lwz r7, lbl_8047A430(r13)
    add r7, r7, r10
    addi r9, r7, 0x42aa
    addi r8, r7, 0x4374
    mtctr r0
@L_800349C8:
    lwz r7, 0x4(r8)
    lwzu r0, 0x8(r8)
    stw r7, 0x4(r9)
    stwu r0, 0x8(r9)
    bdnz @L_800349C8
    lhz r7, 0x4(r8)
    li r0, 0x19
    sth r7, 0x4(r9)
    lwz r7, lbl_8047A430(r13)
    add r7, r7, r10
    addi r9, r7, 0x4374
    addi r8, r7, 0x443e
    mtctr r0
@L_800349FC:
    lwz r7, 0x4(r8)
    lwzu r0, 0x8(r8)
    stw r7, 0x4(r9)
    stwu r0, 0x8(r9)
    bdnz @L_800349FC
    lhz r0, 0x4(r8)
    addi r10, r10, 0x25e
    addi r6, r6, 0x3
    sth r0, 0x4(r9)
    cmpwi r6, 0x9
    blt @L_8003497C
@L_80034A28:
    lwz r7, lbl_8047A430(r13)
    rlwinm r0, r4, 0, 24, 31
    lbz r6, 0x8(r5)
    mr r4, r30
    addi r31, r7, 0x48fe
    stb r6, 0x0(r31)
    lbz r6, 0x24(r5)
    stb r6, 0x1(r31)
    lbz r5, 0x26(r5)
    stb r5, 0x2(r31)
    stb r3, 0x3(r31)
    addi r3, r31, 0x6
    stb r0, 0x4(r31)
    bl fn_800F9E70
    lbz r0, 0xc(r30)
    li r3, 0x0
    stb r0, 0x12(r31)
    lhz r0, 0x12(r30)
    sth r0, 0x14(r31)
    lhz r0, 0x14(r30)
    sth r0, 0x16(r31)
    lhz r0, 0x16(r30)
    sth r0, 0x18(r31)
    lhz r0, 0x18(r30)
    sth r0, 0x1a(r31)
    lwz r0, 0x1c(r30)
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x1c(r31)
    lhz r0, 0x20(r30)
    sth r0, 0x1e(r31)
    lbz r0, 0x24(r30)
    stb r0, 0x20(r31)
    li r0, 0x4
    mtctr r0
@L_80034AB0:
    addi r0, r3, 0xd
    lbzx r0, r30, r0
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_80034AD0
    li r0, 0x0
    sth r0, 0x22(r31)
    b @L_80034B38
@L_80034AD0:
    mulli r5, r0, 0x2a
    lis r4, lbl_803A3334@ha
    addi r0, r4, lbl_803A3334@l
    add r5, r0, r5
    lwz r4, 0x514(r5)
    lwz r0, 0x518(r5)
    stw r4, 0x22(r31)
    stw r0, 0x26(r31)
    lwz r4, 0x51c(r5)
    lwz r0, 0x520(r5)
    stw r4, 0x2a(r31)
    stw r0, 0x2e(r31)
    lwz r4, 0x524(r5)
    lwz r0, 0x528(r5)
    stw r4, 0x32(r31)
    stw r0, 0x36(r31)
    lwz r4, 0x52c(r5)
    lwz r0, 0x530(r5)
    stw r4, 0x3a(r31)
    stw r0, 0x3e(r31)
    lwz r4, 0x534(r5)
    lwz r0, 0x538(r5)
    stw r4, 0x42(r31)
    stw r0, 0x46(r31)
    lhz r0, 0x53c(r5)
    sth r0, 0x4a(r31)
@L_80034B38:
    addi r31, r31, 0x2a
    addi r3, r3, 0x1
    bdnz @L_80034AB0
@L_80034B44:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
