    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x4
    bl fn_80177A44
    lis r3, lbl_803A67FC@ha
    addi r3, r3, lbl_803A67FC@l
    bl fn_800439BC
    mr r3, r31
    li r4, 0x0
    bl fn_80041E48
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
