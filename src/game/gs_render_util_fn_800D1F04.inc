    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    stw r30, 0x8(r1)
    mr r30, r3
    addi r3, r30, 0xf4
    bl fn_800E01D0
    mr r4, r31
    addi r3, r30, 0x100
    bl fn_800E01D0
    li r0, 0x1
    stb r0, 0x2(r30)
    stb r0, 0x1(r30)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
