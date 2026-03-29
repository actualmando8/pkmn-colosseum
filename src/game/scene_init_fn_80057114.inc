    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r5, lbl_803A9768@ha
    addi r5, r5, lbl_803A9768@l
    lwz r5, 0x278(r5)
    bl fn_80056C54
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
