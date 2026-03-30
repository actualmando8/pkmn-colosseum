    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_804007E8@ha
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    addi r31, r3, lbl_804007E8@l
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x49f(r4)
    cmplwi r0, 0x0
    beq @L_800D7210
    lbz r3, 0x47e(r4)
    cmplwi r3, 0x0
    bne @L_800D6B4C
    lwz r0, 0x14(r4)
    cmpwi r0, 0x7
    beq @L_800D6B60
    @L_800D6B4C:
    cmplwi r3, 0x1
    bne @L_800D70F8
    lwz r0, 0x488(r4)
    cmpwi r0, 0x7
    bne @L_800D70F8
    @L_800D6B60:
    lbz r0, 0x18(r4)
    cmplwi r0, 0x1
    bne @L_800D70B4
    addi r3, r31, 0x98
    addi r4, r4, 0x4ac
    li r5, 0x18
    bl memcpy
    lwz r4, lbl_8047AA80(r13)
    addi r3, r31, 0x80
    li r5, 0x18
    addi r4, r4, 0x4e8
    bl memcpy
    lwz r4, lbl_8047AA80(r13)
    addi r3, r31, 0x0
    li r5, 0x80
    addi r4, r4, 0x520
    bl memcpy
    lbz r0, 0x148(r31)
    addi r6, r31, 0x148
    lwz r3, lbl_8047AA80(r13)
    addi r4, r31, 0x130
    li r5, 0x18
    stb r0, 0x4ac(r3)
    lhz r0, 0x4(r6)
    lwz r3, lbl_8047AA80(r13)
    sth r0, 0x4b0(r3)
    lfs f0, 0xc(r6)
    lwz r3, lbl_8047AA80(r13)
    stfs f0, 0x4b8(r3)
    lwz r3, lbl_8047AA80(r13)
    addi r3, r3, 0x4e8
    bl memcpy
    lbz r0, 0xb0(r31)
    addi r3, r31, 0xb0
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x520(r4)
    lhz r0, 0x2(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x522(r4)
    lfs f0, 0x8(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x528(r4)
    lbz r0, 0x10(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x530(r4)
    lhz r0, 0x12(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x532(r4)
    lfs f0, 0x18(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x538(r4)
    lbz r0, 0x20(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x540(r4)
    lhz r0, 0x22(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x542(r4)
    lfs f0, 0x28(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x548(r4)
    lbz r0, 0x30(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x550(r4)
    lhz r0, 0x32(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x552(r4)
    lfs f0, 0x38(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x558(r4)
    lbz r0, 0x40(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x560(r4)
    lhz r0, 0x42(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x562(r4)
    lfs f0, 0x48(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x568(r4)
    lbz r0, 0x50(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x570(r4)
    lhz r0, 0x52(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x572(r4)
    lfs f0, 0x58(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x578(r4)
    lbz r0, 0x60(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x580(r4)
    lhz r0, 0x62(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x582(r4)
    lfs f0, 0x68(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x588(r4)
    lbz r0, 0x70(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r0, 0x590(r4)
    lhz r0, 0x72(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r0, 0x592(r4)
    lfs f0, 0x78(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f0, 0x598(r3)
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x1
    bne @L_800D6D1C
    bl fn_800DB098
    b @L_800D6E10
    @L_800D6D1C:
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800D6E10
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800D6E10
    lwz r29, 0x24(r4)
    lbz r0, 0x8(r29)
    cmplwi r0, 0x0
    beq @L_800D6D5C
    lwz r12, 0x4a0(r4)
    li r3, 0x0
    mtctr r12
    bctrl
    @L_800D6D5C:
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4a8(r4)
    mtctr r12
    bctrl
    lbz r0, 0x40(r29)
    cmplwi r0, 0x0
    beq @L_800D6D90
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4c4(r4)
    mtctr r12
    bctrl
    @L_800D6D90:
    li r30, 0x4
    addi r28, r29, 0x70
    @L_800D6D98:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D6DC0
    subi r3, r30, 0x4
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x4e0
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D6DC0:
    addi r30, r30, 0x1
    addi r28, r28, 0x1c
    cmpwi r30, 0x5
    ble @L_800D6D98
    li r30, 0x6
    addi r28, r29, 0xa8
    @L_800D6DD8:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D6E00
    subi r3, r30, 0x6
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x500
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D6E00:
    addi r30, r30, 0x1
    addi r28, r28, 0x1c
    cmpwi r30, 0xd
    ble @L_800D6DD8
    @L_800D6E10:
    lwz r3, lbl_8047AA80(r13)
    addi r4, r31, 0x98
    li r5, 0x18
    addi r3, r3, 0x4ac
    bl memcpy
    lwz r3, lbl_8047AA80(r13)
    addi r4, r31, 0x80
    li r5, 0x18
    addi r3, r3, 0x4e8
    bl memcpy
    lwz r3, lbl_8047AA80(r13)
    addi r4, r31, 0x0
    li r5, 0x80
    addi r3, r3, 0x520
    bl memcpy
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x1
    bne @L_800D6E64
    bl fn_800DB098
    b @L_800D6F58
    @L_800D6E64:
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800D6F58
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800D6F58
    lwz r30, 0x24(r4)
    lbz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_800D6EA4
    lwz r12, 0x4a0(r4)
    li r3, 0x0
    mtctr r12
    bctrl
    @L_800D6EA4:
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4a8(r4)
    mtctr r12
    bctrl
    lbz r0, 0x40(r30)
    cmplwi r0, 0x0
    beq @L_800D6ED8
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4c4(r4)
    mtctr r12
    bctrl
    @L_800D6ED8:
    li r29, 0x4
    addi r28, r30, 0x70
    @L_800D6EE0:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D6F08
    subi r3, r29, 0x4
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x4e0
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D6F08:
    addi r29, r29, 0x1
    addi r28, r28, 0x1c
    cmpwi r29, 0x5
    ble @L_800D6EE0
    li r29, 0x6
    addi r28, r30, 0xa8
    @L_800D6F20:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D6F48
    subi r3, r29, 0x6
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x500
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D6F48:
    addi r29, r29, 0x1
    addi r28, r28, 0x1c
    cmpwi r29, 0xd
    ble @L_800D6F20
    @L_800D6F58:
    addi r6, r31, 0x148
    lwz r4, lbl_8047AA80(r13)
    lbz r5, 0x1(r6)
    addi r3, r31, 0xb0
    li r0, 0x0
    stb r5, 0x4ad(r4)
    lhz r5, 0x6(r6)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x4b2(r4)
    lfs f0, 0x10(r6)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x4bc(r4)
    lbz r5, 0x1(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x521(r4)
    lhz r5, 0x4(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x524(r4)
    lfs f0, 0xc(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x52c(r4)
    lbz r5, 0x11(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x531(r4)
    lhz r5, 0x14(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x534(r4)
    lfs f0, 0x1c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x53c(r4)
    lbz r5, 0x21(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x541(r4)
    lhz r5, 0x24(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x544(r4)
    lfs f0, 0x2c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x54c(r4)
    lbz r5, 0x31(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x551(r4)
    lhz r5, 0x34(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x554(r4)
    lfs f0, 0x3c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x55c(r4)
    lbz r5, 0x41(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x561(r4)
    lhz r5, 0x44(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x564(r4)
    lfs f0, 0x4c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x56c(r4)
    lbz r5, 0x51(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x571(r4)
    lhz r5, 0x54(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x574(r4)
    lfs f0, 0x5c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x57c(r4)
    lbz r5, 0x61(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x581(r4)
    lhz r5, 0x64(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x584(r4)
    lfs f0, 0x6c(r3)
    lwz r4, lbl_8047AA80(r13)
    stfs f0, 0x58c(r4)
    lbz r5, 0x71(r3)
    lwz r4, lbl_8047AA80(r13)
    stb r5, 0x591(r4)
    lhz r5, 0x74(r3)
    lwz r4, lbl_8047AA80(r13)
    sth r5, 0x594(r4)
    lfs f0, 0x7c(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f0, 0x59c(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x18(r3)
    b @L_800D70F8
    @L_800D70B4:
    addi r3, r31, 0x148
    addi r4, r4, 0x4ac
    li r5, 0x18
    bl memcpy
    lwz r4, lbl_8047AA80(r13)
    addi r3, r31, 0x130
    li r5, 0x18
    addi r4, r4, 0x4e8
    bl memcpy
    lwz r4, lbl_8047AA80(r13)
    addi r3, r31, 0xb0
    li r5, 0x80
    addi r4, r4, 0x520
    bl memcpy
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x1
    stb r0, 0x18(r3)
    @L_800D70F8:
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x1
    bne @L_800D7110
    bl fn_800DB098
    b @L_800D7204
    @L_800D7110:
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800D7204
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800D7204
    lwz r30, 0x24(r4)
    lbz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_800D7150
    lwz r12, 0x4a0(r4)
    li r3, 0x0
    mtctr r12
    bctrl
    @L_800D7150:
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4a8(r4)
    mtctr r12
    bctrl
    lbz r0, 0x40(r30)
    cmplwi r0, 0x0
    beq @L_800D7184
    lwz r4, lbl_8047AA80(r13)
    li r3, 0x0
    lwz r12, 0x4c4(r4)
    mtctr r12
    bctrl
    @L_800D7184:
    li r29, 0x4
    addi r28, r30, 0x70
    @L_800D718C:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D71B4
    subi r3, r29, 0x4
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x4e0
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D71B4:
    addi r29, r29, 0x1
    addi r28, r28, 0x1c
    cmpwi r29, 0x5
    ble @L_800D718C
    li r29, 0x6
    addi r28, r30, 0xa8
    @L_800D71CC:
    lbz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_800D71F4
    subi r3, r29, 0x6
    lwz r5, lbl_8047AA80(r13)
    slwi r4, r3, 2
    addi r0, r4, 0x500
    lwzx r12, r5, r0
    mtctr r12
    bctrl
    @L_800D71F4:
    addi r29, r29, 0x1
    addi r28, r28, 0x1c
    cmpwi r29, 0xd
    ble @L_800D71CC
    @L_800D7204:
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x0
    stb r0, 0x49f(r3)
    @L_800D7210:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
