    nofralloc
    mflr r0
    li r6, 0x8
    stw r0, 0x4(r1)
    lis r5, 0xcc01
    li r3, 0x50
    stwu r1, -0x8(r1)
    li r0, 0x60
    stb r6, -0x8000(r5)
    lwz r4, -29632(r2)
    stb r3, -0x8000(r5)
    lwz r3, 0x14(r4)
    stw r3, -0x8000(r5)
    stb r6, -0x8000(r5)
    stb r0, -0x8000(r5)
    lwz r0, 0x18(r4)
    stw r0, -0x8000(r5)
    bl fn_800B771C
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
