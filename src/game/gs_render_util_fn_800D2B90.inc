    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    lis r4, -32729
    stw r0, 0x74(r1)
    addi r6, r4, 848
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    mr r30, r3
    lwz r31, -23956(r13)
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    cmplwi r31, 0x0
    lwz r3, 0x8(r6)
    lwz r0, 0xc(r6)
    stw r5, 0x18(r1)
    stw r4, 0x1c(r1)
    stw r3, 0x20(r1)
    stw r0, 0x24(r1)
    beq @L_800D2C54
    beq @L_800D2C4C
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r0, r3, 0x1
    rlwinm r0, r0, 0, 16, 31
    subf r0, r4, r0
    cntlzw r0, r0
    rlwinm. r0, r0, 27, 5, 31
    beq @L_800D2C04
    b @L_800D2C14
@L_800D2C04:
    subi r3, r4, 0x1
    cntlzw r0, r4
    sth r3, 0x4(r31)
    rlwinm r0, r0, 27, 5, 31
@L_800D2C14:
    cmpwi r0, 0x0
    beq @L_800D2C4C
    cmplwi r31, 0x0
    beq @L_800D2C4C
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
@L_800D2C4C:
    li r0, 0x0
    stw r0, -23956(r13)
@L_800D2C54:
    cmplwi r30, 0x0
    bne @L_800D2C6C
    li r3, 0x0
    bl fn_8019BD18
    bl fn_8016EA88
    b @L_800D2DD0
@L_800D2C6C:
    mr r3, r30
    bl fn_8019BB78
    stw r3, -23956(r13)
    bl fn_8016EB30
    lbz r3, 0x10(r30)
    lis r5, 0x4330
    lbz r4, 0x11(r30)
    li r0, 0x1
    stw r3, 0x2c(r1)
    lbz r3, 0x12(r30)
    stw r5, 0x28(r1)
    lfd f6, -27816(r2)
    lfd f0, 0x28(r1)
    stw r4, 0x34(r1)
    fsubs f0, f0, f6
    lfs f5, -27824(r2)
    stw r5, 0x30(r1)
    lbz r4, 0x13(r30)
    lfd f1, 0x30(r1)
    fdivs f3, f0, f5
    stw r3, 0x3c(r1)
    lwz r3, -23968(r13)
    stw r5, 0x38(r1)
    lfd f0, 0x38(r1)
    stw r4, 0x44(r1)
    fsubs f1, f1, f6
    stw r5, 0x40(r1)
    fsubs f0, f0, f6
    lfd f2, 0x40(r1)
    fdivs f1, f1, f5
    stfs f3, 0x18(r1)
    lwz r4, 0x18(r1)
    stb r0, 0x19(r3)
    stw r4, 0x8(r1)
    lwz r3, -23968(r13)
    fdivs f4, f0, f5
    stfs f1, 0x1c(r1)
    lfs f1, 0x8(r1)
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    lfs f0, 0xc(r1)
    fsubs f3, f2, f6
    stfs f4, 0x20(r1)
    fmuls f2, f5, f1
    lwz r0, 0x20(r1)
    fmuls f1, f5, f0
    fdivs f4, f3, f5
    stw r0, 0x10(r1)
    lfs f0, 0x10(r1)
    fctiwz f3, f2
    stfs f4, 0x24(r1)
    fctiwz f2, f1
    lwz r0, 0x24(r1)
    fmuls f0, f5, f0
    stfd f3, 0x48(r1)
    stw r0, 0x14(r1)
    fctiwz f1, f0
    lwz r0, 0x4c(r1)
    stfd f2, 0x50(r1)
    lfs f0, 0x14(r1)
    stb r0, 0x1c(r3)
    fmuls f0, f5, f0
    lwz r0, 0x54(r1)
    lwz r3, -23968(r13)
    stfd f1, 0x58(r1)
    fctiwz f0, f0
    stb r0, 0x1d(r3)
    lwz r0, 0x5c(r1)
    lwz r3, -23968(r13)
    stfd f0, 0x60(r1)
    stb r0, 0x1e(r3)
    lwz r0, 0x64(r1)
    lwz r3, -23968(r13)
    stb r0, 0x1f(r3)
    lwz r3, -23968(r13)
    lbz r0, 0x1c(r3)
    cmplwi r0, 0x0
    bne @L_800D2DD0
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x0
    bne @L_800D2DD0
    lbz r0, 0x1e(r3)
    cmplwi r0, 0x0
    bne @L_800D2DD0
    lbz r0, 0x1f(r3)
    cmplwi r0, 0x0
    bne @L_800D2DD0
    li r0, 0x0
    stb r0, 0x19(r3)
@L_800D2DD0:
    lwz r0, 0x74(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
