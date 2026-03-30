    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    lfs f1, 0x0(r3)
    bl fn_800C46B0
    lwz r7, -29632(r2)
    lis r4, 0x6
    li r6, 0x10
    stw r3, 0x420(r7)
    lis r5, 0xcc01
    addi r3, r4, 0x1020
    lfs f0, 0x4(r31)
    li r0, 0x1
    stfs f0, 0x424(r7)
    lfs f0, 0x8(r31)
    stfs f0, 0x428(r7)
    lfs f0, 0xc(r31)
    stfs f0, 0x42c(r7)
    lfs f0, 0x10(r31)
    stfs f0, 0x430(r7)
    lfs f0, 0x14(r31)
    stfs f0, 0x434(r7)
    lfs f0, 0x18(r31)
    stfs f0, 0x438(r7)
    stb r6, -0x8000(r5)
    stw r3, -0x8000(r5)
    lfs f0, 0x424(r7)
    stfs f0, -0x8000(r5)
    lfs f0, 0x428(r7)
    stfs f0, -0x8000(r5)
    lfs f0, 0x42c(r7)
    stfs f0, -0x8000(r5)
    lfs f0, 0x430(r7)
    stfs f0, -0x8000(r5)
    lfs f0, 0x434(r7)
    stfs f0, -0x8000(r5)
    lfs f0, 0x438(r7)
    stfs f0, -0x8000(r5)
    lwz r3, 0x420(r7)
    stw r3, -0x8000(r5)
    sth r0, 0x2(r7)
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
