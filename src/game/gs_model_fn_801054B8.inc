    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, lbl_8047CDE8(r13)
    lis r3, lbl_80404AB0@ha
    addi r3, r3, lbl_80404AB0@l
    li r4, 0x0
    stw r0, 0x8(r1)
    li r5, 0x1a
    bl memset
    lis r4, lbl_80404ACC@ha
    lis r3, lbl_80404AB0@ha
    addi r11, r4, lbl_80404ACC@l
    addi r12, r1, 0x8
    rlwinm r31, r29, 0, 24, 31
    addi r10, r3, lbl_80404AB0@l
    li r30, 0x0
    li r0, 0x2
    mtctr r0
@L_80105514:
    lbz r0, 0x0(r12)
    addi r29, r11, 0x2a
    and r0, r31, r0
    cmpwi r0, 0x0
    beq @L_80105580
    lis r3, lbl_80404AB0@ha
    lhz r0, 0x0(r29)
    addi r9, r3, lbl_80404AB0@l
    lhz r5, 0x2(r10)
    lhz r3, 0x0(r9)
    lhz r4, 0x2(r29)
    or r8, r3, r0
    lhz r3, 0x4(r10)
    lhz r0, 0x4(r29)
    or r7, r5, r4
    lhz r5, 0x6(r10)
    lhz r4, 0x6(r29)
    or r6, r3, r0
    lhz r3, 0x8(r10)
    lhz r0, 0x8(r29)
    or r4, r5, r4
    sth r8, 0x0(r9)
    or r0, r3, r0
    sth r7, 0x2(r10)
    sth r6, 0x4(r10)
    sth r4, 0x6(r10)
    sth r0, 0x8(r10)
@L_80105580:
    addi r11, r11, 0x1a
    lbz r0, 0x1(r12)
    addi r29, r11, 0x2a
    and r0, r31, r0
    cmpwi r0, 0x0
    beq @L_801055F0
    lis r3, lbl_80404AB0@ha
    lhz r0, 0x0(r29)
    addi r9, r3, lbl_80404AB0@l
    lhz r5, 0x2(r10)
    lhz r3, 0x0(r9)
    lhz r4, 0x2(r29)
    or r8, r3, r0
    lhz r3, 0x4(r10)
    lhz r0, 0x4(r29)
    or r7, r5, r4
    lhz r5, 0x6(r10)
    lhz r4, 0x6(r29)
    or r6, r3, r0
    lhz r3, 0x8(r10)
    lhz r0, 0x8(r29)
    or r4, r5, r4
    sth r8, 0x0(r9)
    or r0, r3, r0
    sth r7, 0x2(r10)
    sth r6, 0x4(r10)
    sth r4, 0x6(r10)
    sth r0, 0x8(r10)
@L_801055F0:
    addi r11, r11, 0x1a
    addi r12, r12, 0x2
    addi r30, r30, 0x1
    bdnz @L_80105514
    lis r3, lbl_80404AB0@ha
    addi r3, r3, lbl_80404AB0@l
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
