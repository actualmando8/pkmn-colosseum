    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, lbl_80315568@ha
    stw r0, 0x14(r1)
    addi r0, r5, lbl_80315568@l
    li r5, 0x30
    stw r31, 0xc(r1)
    mr r31, r4
    mr r4, r0
    stw r30, 0x8(r1)
    mr r30, r3
    bl memcpy
    lfs f0, 0x0(r31)
    stfs f0, 0xc(r30)
    lfs f0, 0x4(r31)
    stfs f0, 0x1c(r30)
    lfs f0, 0x8(r31)
    stfs f0, 0x2c(r30)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
