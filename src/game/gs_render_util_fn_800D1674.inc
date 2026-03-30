    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    addi r4, r30, 0x70
    lbz r0, 0x4(r3)
    addi r3, r31, 0x4
    stb r0, 0x0(r31)
    bl fn_800E01D0
    addi r3, r31, 0x10
    addi r4, r30, 0x7c
    bl fn_800E01D0
    addi r3, r31, 0x1c
    addi r4, r30, 0x88
    bl fn_800E01D0
    addi r3, r31, 0x28
    addi r4, r30, 0x94
    bl fn_800E0628
    addi r3, r31, 0x58
    addi r4, r30, 0xc4
    bl fn_800E0628
    addi r3, r31, 0x88
    addi r4, r30, 0xf4
    bl fn_800E01D0
    addi r3, r31, 0x94
    addi r4, r30, 0x100
    bl fn_800E01D0
    lwz r0, 0x10c(r30)
    stw r0, 0xa0(r31)
    lwz r0, 0x114(r30)
    stw r0, 0xa4(r31)
    lfs f0, 0x118(r30)
    stfs f0, 0xa8(r31)
    lfs f0, 0x11c(r30)
    stfs f0, 0xac(r31)
    lbz r0, 0x124(r30)
    stb r0, 0x1(r31)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
