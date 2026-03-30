    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    addi r4, r31, 0x4
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r3)
    stb r0, 0x0(r31)
    lbz r0, 0x3(r3)
    stb r0, 0x1(r31)
    lwz r3, 0xc(r3)
    bl fn_801A497C
    lwz r3, 0xc(r30)
    addi r4, r31, 0x10
    bl fn_801A48B0
    lwz r0, 0x60(r30)
    stw r0, 0x1c(r31)
    lfs f0, 0x68(r30)
    stfs f0, 0x20(r31)
    lfs f0, 0x64(r30)
    stfs f0, 0x24(r31)
    lwz r0, 0x5c(r30)
    stw r0, 0x28(r31)
    lbz r0, 0x70(r30)
    stb r0, 0x2(r31)
    lbz r0, 0x71(r30)
    stb r0, 0x3(r31)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
