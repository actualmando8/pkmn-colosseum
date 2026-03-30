    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_8036D3F0@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r4, lbl_8036D3F0@l
    lwz r4, 0x14(r3)
    mr r3, r31
    lwz r12, 0x2c(r4)
    mtctr r12
    bctrl
    cmpwi r3, 0x0
    blt @L_801BBD28
    lis r4, 0x1
    li r3, 0x0
    subi r4, r4, 0x1
    sth r4, 0x4a(r31)
    @L_801BBD28:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
