    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r4, 0x0
    li r5, 0x446
    bl memset
    li r0, 0x0
    mr r4, r31
    stb r0, 0x442(r31)
    li r0, 0x10
    li r3, 0xff
    mtctr r0
@L_801D1FE0:
    sth r3, 0x0(r4)
    sth r3, 0x2(r4)
    sth r3, 0x4(r4)
    sth r3, 0x6(r4)
    sth r3, 0x8(r4)
    sth r3, 0xa(r4)
    sth r3, 0xc(r4)
    sth r3, 0xe(r4)
    sth r3, 0x10(r4)
    sth r3, 0x12(r4)
    sth r3, 0x14(r4)
    sth r3, 0x16(r4)
    sth r3, 0x18(r4)
    sth r3, 0x1a(r4)
    sth r3, 0x1c(r4)
    sth r3, 0x1e(r4)
    sth r3, 0x20(r4)
    sth r3, 0x22(r4)
    sth r3, 0x24(r4)
    sth r3, 0x26(r4)
    sth r3, 0x28(r4)
    sth r3, 0x2a(r4)
    sth r3, 0x2c(r4)
    sth r3, 0x2e(r4)
    sth r3, 0x30(r4)
    sth r3, 0x32(r4)
    sth r3, 0x34(r4)
    sth r3, 0x36(r4)
    sth r3, 0x38(r4)
    sth r3, 0x3a(r4)
    sth r3, 0x3c(r4)
    sth r3, 0x3e(r4)
    addi r4, r4, 0x40
    bdnz @L_801D1FE0
    bl fn_801D2B08
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
