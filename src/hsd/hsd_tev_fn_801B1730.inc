    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0x8046
    stw r0, 0x14(r1)
    addi r3, r3, 22240
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    bl fn_801AA4CC
    li r4, 0x0
    mr r31, r3
    li r5, 0x28
    bl memset
    bl fn_80194258
    stw r3, 0x4(r31)
    bl fn_801BBDDC
    li r0, 0x0
    lis r4, 0x54
    stw r0, 0x10(r3)
    mr r30, r3
    li r3, 0x0
    li r0, 0x0
    stw r3, 0x40(r30)
    addi r4, r4, 0x103
    stw r0, 0x44(r30)
    stw r4, 0x4c(r30)
    bl fn_801BBD84
    stw r3, 0x58(r30)
    li r3, 0x0
    lfs f1, -22704(r2)
    li r6, 0x0
    stw r30, 0x8(r31)
    li r5, 0x100
    lfs f0, -22700(r2)
    li r4, 0x100
    stfs f1, 0xc(r31)
    li r0, 0x0
    lfs f1, -22704(r2)
    stfs f0, 0x10(r31)
    lfs f0, -22704(r2)
    stfs f1, 0x14(r31)
    lfs f1, -22752(r2)
    stfs f0, 0x18(r31)
    lfs f2, -22696(r2)
    stb r3, 0x20(r31)
    lfs f3, -22752(r2)
    lwz r3, 0x8(r31)
    lfs f4, -22696(r2)
    lwz r3, 0x58(r3)
    stw r6, 0x8(r3)
    lwz r3, 0x8(r31)
    lwz r3, 0x58(r3)
    sth r5, 0x4(r3)
    lwz r3, 0x8(r31)
    lwz r3, 0x58(r3)
    sth r4, 0x6(r3)
    lwz r3, 0x8(r31)
    lwz r3, 0x58(r3)
    stw r0, 0x0(r3)
    lwz r3, 0x4(r31)
    bl fn_801943E4
    lwz r3, 0x4(r31)
    li r4, 0x0
    li r5, 0x100
    li r6, 0x0
    li r7, 0x100
    bl fn_801944A4
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
