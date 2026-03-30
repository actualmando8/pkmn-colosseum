    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, 0x4000
    stw r0, 0x14(r1)
    addi r4, r4, 0x600f
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x8(r3)
    bl fn_801A8428
    lwz r3, 0x8(r31)
    lwz r4, 0x4(r31)
    bl fn_801A8440
    lwz r3, 0x8(r31)
    bl fn_801A6FF0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
