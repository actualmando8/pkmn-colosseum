    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lis r4, lbl_803A9A18@ha
    li r3, 0x37
    addi r4, r4, lbl_803A9A18@l
    bl fn_80132A38
    li r0, 0xce
    li r3, 0x0
    stw r0, 0x4c(r31)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
