    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, fn_801F2804@ha
    li r5, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, fn_801F2804@l
    li r6, 0x0
    bl fn_801F37B0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
