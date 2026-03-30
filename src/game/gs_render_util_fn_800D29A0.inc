    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, -23984(r13)
    lwz r31, -23988(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800D29DC
@L_800D29C4:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800D29D4
    b @L_800D29E0
@L_800D29D4:
    addi r31, r31, 0x128
    bdnz @L_800D29C4
@L_800D29DC:
    li r31, 0x0
@L_800D29E0:
    cmplwi r31, 0x0
    bne @L_800D29F0
    li r3, 0x0
    b @L_800D2AC0
@L_800D29F0:
    li r8, 0x0
    lfs f5, -27912(r2)
    stw r8, 0x10(r31)
    li r7, 0x1
    lfs f4, -27920(r2)
    li r6, 0x280
    stfs f5, 0x14(r31)
    li r5, 0x1e0
    addi r4, r31, 0x10
    addi r0, r31, 0x24
    stfs f5, 0x18(r31)
    addi r3, r31, 0x38
    lfs f3, -27916(r2)
    stfs f5, 0x1c(r31)
    lfs f2, -27872(r2)
    stw r8, 0x20(r31)
    lfs f1, -27852(r2)
    stw r8, 0x24(r31)
    lfs f0, -27848(r2)
    stfs f5, 0x28(r31)
    stfs f5, 0x2c(r31)
    stfs f4, 0x30(r31)
    stw r8, 0x34(r31)
    stw r8, 0x38(r31)
    sth r8, 0x3c(r31)
    sth r7, 0x3e(r31)
    sth r8, 0x40(r31)
    sth r6, 0x42(r31)
    sth r8, 0x44(r31)
    sth r5, 0x46(r31)
    sth r8, 0x48(r31)
    sth r6, 0x4a(r31)
    sth r8, 0x4c(r31)
    sth r5, 0x4e(r31)
    stw r4, 0x50(r31)
    stw r0, 0x54(r31)
    stfs f5, 0x58(r31)
    stw r8, 0x5c(r31)
    stfs f3, 0x60(r31)
    stfs f2, 0x64(r31)
    stfs f1, 0x68(r31)
    stfs f0, 0x6c(r31)
    bl fn_80193F44
    stw r3, 0xc(r31)
    li r4, 0x1
    li r0, 0x0
    mr r3, r31
    stb r4, 0x0(r31)
    stb r0, 0x1(r31)
    stb r0, 0x4(r31)
    stb r0, 0x3(r31)
    stw r0, 0x8(r31)
@L_800D2AC0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
