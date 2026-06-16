    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x8(r3)
    bl fn_801A6D08
    lwz r3, 0xc(r31)
    bl HSD_PObjRemoveAll
    lwz r3, 0x10(r31)
    bl fn_801C25E4
    lis r4, lbl_8036C7A0@ha
    mr r3, r31
    addi r4, r4, lbl_8036C7A0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
