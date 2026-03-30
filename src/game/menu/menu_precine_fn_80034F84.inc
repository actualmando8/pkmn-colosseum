    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, lbl_803A3288@ha
    li r3, 0x37
    addi r4, r4, lbl_803A3288@l
    bl fn_80132A38
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
