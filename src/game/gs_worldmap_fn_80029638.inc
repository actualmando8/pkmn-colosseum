    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, lbl_803A2068@ha
    addi r4, r4, lbl_803A2068@l
    bl fn_800F9E70
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
