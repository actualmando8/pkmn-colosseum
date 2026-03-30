    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    addi r4, r31, 0x70
    stb r0, 0x4(r3)
    li r0, 0x1
    stb r0, 0x1(r3)
    lwz r3, 0xc(r3)
    bl fn_80195904
    lwz r3, 0xc(r31)
    addi r4, r31, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r31)
    addi r4, r31, 0x100
    bl fn_801959DC
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
